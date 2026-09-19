#include "buff_hud.hpp"

#include "buff_hud_layout.hpp"
#include "core/buff_display_bus.hpp"
#include "core/services.hpp"
#include "icon_frame_backend.hpp"
#include "loose_layout.hpp"
#include "native/native_contract.hpp"
#include "skill_icon_resolver.hpp"

#include <Windows.h>

#include <D2RLPlugin/localization.h>

#include <algorithm>
#include <array>
#include <atomic>
#include <charconv>
#include <cctype>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <limits>
#include <string>
#include <string_view>
#include <system_error>

namespace BuffPanel::Systems::BuffHud {
namespace {

using Internal::IconFrameBackendState;
using Internal::SkillIconDescriptor;

constexpr std::size_t SlotCount = Core::MaximumDisplayedBuffs;
constexpr std::size_t AtlasCount = 9;
constexpr std::uint32_t FramesPerSecond = 25;
constexpr char TimerReserve[] = "__BUFF_PANEL_TIMER_RESERVE_00__";
constexpr std::size_t TimerReserveBytes = sizeof(TimerReserve);
constexpr std::size_t TooltipReserveBytes = 128;
constexpr std::size_t TooltipReserveLength = 126;
constexpr std::size_t TooltipReserveCodepoints = TooltipReserveLength / 3;
static_assert(TooltipReserveCodepoints * 3 == TooltipReserveLength);
constexpr std::size_t TooltipNativeScanBytes = 0x1000;
constexpr std::uint32_t NoRenderedSeconds = std::numeric_limits<std::uint32_t>::max();
constexpr std::int32_t NoRenderedValue = std::numeric_limits<std::int32_t>::min();
constexpr std::uint64_t DebugKeyBase = 0x4250464255460000ULL; // "BP FBUF" + slot

enum class TooltipStorageEncoding : std::uint8_t {
    Unknown = 0,
    BlizzardUtf8 = 1,
    BlizzardUtf16 = 2,
};
static_assert(TimerReserveBytes == 32);

constexpr std::array<Core::BuffIconAtlas, AtlasCount> AtlasOrder{
    Core::BuffIconAtlas::Amazon,
    Core::BuffIconAtlas::Sorceress,
    Core::BuffIconAtlas::Necromancer,
    Core::BuffIconAtlas::Paladin,
    Core::BuffIconAtlas::Barbarian,
    Core::BuffIconAtlas::Druid,
    Core::BuffIconAtlas::Assassin,
    Core::BuffIconAtlas::Warlock,
    Core::BuffIconAtlas::Global,
};

constexpr std::array<const char*, AtlasCount> AtlasWidgetNames{
    "IconAmazon",
    "IconSorceress",
    "IconNecromancer",
    "IconPaladin",
    "IconBarbarian",
    "IconDruid",
    "IconAssassin",
    "IconWarlock",
    "IconGlobal",
};

using FindTopLevelPanelFn = void*(__fastcall*)(const char* name) noexcept;
using FindChildWidgetByNameFn = void*(__fastcall*)(void* parent, const char* name) noexcept;

const D2RL::PluginContext* Context{};
const D2RL::ResourceService* Resources{};
const D2RL::PanelService* Panels{};
const D2RL::WidgetService* Widgets{};
const D2RL::ThreadService* Threads{};
const D2RL::LifecycleService* Lifecycle{};
D2RL::Resources::RegistrationHandle LayoutResource{D2RL::Resources::InvalidHandle};
LooseLayout::Source LayoutSource{LooseLayout::Source::Embedded};
std::size_t LayoutBytes{};
D2RL::Panels::RegistrationHandle RegisteredPanel{D2RL::Panels::InvalidHandle};
D2RL::Lifecycle::ListenerHandle DataTablesListener{D2RL::Lifecycle::InvalidHandle};
std::array<D2RL::Lifecycle::ListenerHandle, 3> GameplayListeners{};

FindTopLevelPanelFn FindTopLevelPanel{};
FindChildWidgetByNameFn FindChildWidgetByName{};

std::atomic<std::uint64_t> CurrentSessionGeneration{};
std::atomic<bool> PollScheduled{};
std::atomic<bool> DebugClockActive{};
std::atomic<std::uint64_t> DebugClockStartTick{};
std::atomic<std::uint32_t> DebugClockStartFrame{1};
std::atomic<std::uint64_t> DebugClockSession{};

std::atomic<std::uint64_t> Polls{};
std::atomic<std::uint64_t> PollQueueFailures{};
std::atomic<std::uint64_t> LayoutRefreshes{};
std::atomic<std::uint64_t> TimerWrites{};
std::atomic<std::uint64_t> TimerWriteFailures{};
std::atomic<std::uint64_t> IconResolveFailures{};
std::atomic<std::uint64_t> IconFrameFallbacks{};
std::atomic<std::uint64_t> ExpiredEntriesRemoved{};
std::atomic<std::uint64_t> PanelOpenFailures{};
std::atomic<std::uint64_t> WidgetResolveFailures{};
std::atomic<std::uint64_t> WidgetEnableFailures{};
std::atomic<std::uint64_t> TooltipNameResolveFailures{};
std::atomic<std::uint64_t> TooltipQualificationFailures{};
std::atomic<std::uint64_t> TooltipWrites{};
std::atomic<std::uint64_t> TooltipWriteFailures{};

struct SlotHandles final {
    D2RL::Widgets::WidgetHandle slot{D2RL::Widgets::InvalidHandle};
    std::array<D2RL::Widgets::WidgetHandle, AtlasCount> icons{};
    D2RL::Widgets::WidgetHandle countdown{D2RL::Widgets::InvalidHandle};
};

struct SlotRenderState final {
    bool visible{};
    std::uint64_t key{};
    Core::BuffIconAtlas atlas{Core::BuffIconAtlas::Global};
    std::uint16_t requestedFrame{};
    std::uint16_t renderedFrame{};
    Core::BuffDisplayMode displayMode{Core::BuffDisplayMode::Timer};
    std::uint32_t seconds{NoRenderedSeconds};
    std::int32_t currentValue{NoRenderedValue};
    std::int32_t maximumValue{NoRenderedValue};
    std::int32_t sourceSkillId{Core::NoSourceSkillId};
    bool timerVisible{};
    bool tooltipVisible{};
    std::uintptr_t qualifiedTimerBuffer{};
    std::uintptr_t qualifiedTooltipWidget{};
    std::uintptr_t qualifiedTooltipBuffer{};
    std::size_t qualifiedTooltipFieldOffset{std::numeric_limits<std::size_t>::max()};
    TooltipStorageEncoding qualifiedTooltipEncoding{TooltipStorageEncoding::Unknown};
};

D2RL::Widgets::WidgetHandle HudPanel{D2RL::Widgets::InvalidHandle};
D2RL::Widgets::WidgetHandle GridWidget{D2RL::Widgets::InvalidHandle};
std::array<SlotHandles, SlotCount> Handles{};
std::array<SlotRenderState, SlotCount> RenderStates{};
bool HandlesResolved{};
std::uint64_t LastRenderedRevision{std::numeric_limits<std::uint64_t>::max()};

[[nodiscard]] const char* AtlasName(Core::BuffIconAtlas atlas) noexcept {
    switch (atlas) {
    case Core::BuffIconAtlas::Amazon: return "amazon";
    case Core::BuffIconAtlas::Sorceress: return "sorceress";
    case Core::BuffIconAtlas::Necromancer: return "necromancer";
    case Core::BuffIconAtlas::Paladin: return "paladin";
    case Core::BuffIconAtlas::Barbarian: return "barbarian";
    case Core::BuffIconAtlas::Druid: return "druid";
    case Core::BuffIconAtlas::Assassin: return "assassin";
    case Core::BuffIconAtlas::Warlock: return "warlock";
    case Core::BuffIconAtlas::Global: return "global";
    case Core::BuffIconAtlas::Auto:
    default: return "auto";
    }
}

[[nodiscard]] const char* TooltipEncodingName(TooltipStorageEncoding encoding) noexcept {
    switch (encoding) {
    case TooltipStorageEncoding::BlizzardUtf8: return "blz-utf8";
    case TooltipStorageEncoding::BlizzardUtf16: return "blz-utf16";
    case TooltipStorageEncoding::Unknown:
    default: return "unknown";
    }
}

[[nodiscard]] std::size_t AtlasIndex(Core::BuffIconAtlas atlas) noexcept {
    for (std::size_t i = 0; i < AtlasOrder.size(); ++i) {
        if (AtlasOrder[i] == atlas) return i;
    }
    return AtlasOrder.size() - 1;
}

[[nodiscard]] bool IsReadableRange(const void* address, std::size_t size) noexcept {
    if (address == nullptr || size == 0) return false;
    MEMORY_BASIC_INFORMATION info{};
    if (VirtualQuery(address, &info, sizeof(info)) == 0
        || info.State != MEM_COMMIT
        || (info.Protect & PAGE_GUARD) != 0
        || (info.Protect & PAGE_NOACCESS) != 0) {
        return false;
    }
    const auto start = reinterpret_cast<std::uintptr_t>(address);
    const auto regionStart = reinterpret_cast<std::uintptr_t>(info.BaseAddress);
    const auto regionEnd = regionStart + info.RegionSize;
    return start >= regionStart && start <= regionEnd && size <= regionEnd - start;
}

[[nodiscard]] bool IsWritableRange(void* address, std::size_t size) noexcept {
    if (!IsReadableRange(address, size)) return false;
    MEMORY_BASIC_INFORMATION info{};
    if (VirtualQuery(address, &info, sizeof(info)) == 0) return false;
    const DWORD baseProtection = info.Protect & 0xFFU;
    return baseProtection == PAGE_READWRITE
        || baseProtection == PAGE_WRITECOPY
        || baseProtection == PAGE_EXECUTE_READWRITE
        || baseProtection == PAGE_EXECUTE_WRITECOPY;
}

template <typename T>
[[nodiscard]] bool ReadNativeField(const void* base, std::size_t offset, T& value) noexcept {
    if (base == nullptr) return false;
    const auto baseValue = reinterpret_cast<std::uintptr_t>(base);
    if (offset > static_cast<std::size_t>(UINTPTR_MAX - baseValue)) return false;
    const auto* address = reinterpret_cast<const void*>(baseValue + offset);
    if (!IsReadableRange(address, sizeof(T))) return false;
    std::memcpy(&value, address, sizeof(T));
    return true;
}

[[nodiscard]] bool IsTimerReserve(const char* buffer) noexcept {
    return buffer != nullptr
        && IsReadableRange(buffer, TimerReserveBytes)
        && std::memcmp(buffer, TimerReserve, TimerReserveBytes) == 0;
}

void MakeTooltipReserve(std::size_t slotIndex, char (&output)[TooltipReserveBytes]) noexcept {
    // Use a per-slot *invisible* UTF-8 qualification token instead of a long
    // run of ASCII spaces. The old reserve was visually blank but D2R still
    // measured all 127 spaces when the native tooltip field could not be
    // uniquely qualified, producing a huge empty tooltip. U+200B ZERO WIDTH
    // SPACE and U+200C ZERO WIDTH NON-JOINER give us 42 zero-width codepoints
    // (126 UTF-8 bytes) while the low five codepoints encode the slot id so the
    // backing std::string is unique for every one of the 21 reusable slots.
    constexpr std::array<unsigned char, 3> ZeroWidthSpace{0xE2, 0x80, 0x8B};
    constexpr std::array<unsigned char, 3> ZeroWidthNonJoiner{0xE2, 0x80, 0x8C};
    std::memset(output, 0, TooltipReserveBytes);
    for (std::size_t i = 0; i < TooltipReserveCodepoints; ++i) {
        const bool idBit = i < 5 && ((slotIndex >> i) & 1U) != 0;
        const auto& codepoint = idBit ? ZeroWidthNonJoiner : ZeroWidthSpace;
        std::memcpy(output + i * 3, codepoint.data(), codepoint.size());
    }
    output[TooltipReserveLength] = '\0';
}

void MakeTooltipReserveUtf16(std::size_t slotIndex, std::array<std::uint16_t, TooltipReserveCodepoints + 1>& output) noexcept {
    output.fill(0);
    for (std::size_t i = 0; i < TooltipReserveCodepoints; ++i) {
        const bool idBit = i < 5 && ((slotIndex >> i) & 1U) != 0;
        output[i] = static_cast<std::uint16_t>(idBit ? 0x200CU : 0x200BU);
    }
}

[[nodiscard]] bool Utf8ToUtf16(
    std::string_view input,
    std::array<std::uint16_t, TooltipReserveCodepoints + 1>& output,
    std::size_t& outputLength) noexcept {
    output.fill(0);
    outputLength = 0;
    std::size_t i{};
    while (i < input.size()) {
        const auto b0 = static_cast<unsigned char>(input[i]);
        std::uint32_t cp{};
        std::size_t count{};
        if (b0 < 0x80U) {
            cp = b0; count = 1;
        } else if ((b0 & 0xE0U) == 0xC0U && i + 1 < input.size()) {
            cp = static_cast<std::uint32_t>(b0 & 0x1FU) << 6;
            cp |= static_cast<unsigned char>(input[i + 1]) & 0x3FU;
            count = 2;
            if (cp < 0x80U) return false;
        } else if ((b0 & 0xF0U) == 0xE0U && i + 2 < input.size()) {
            cp = static_cast<std::uint32_t>(b0 & 0x0FU) << 12;
            cp |= static_cast<std::uint32_t>(static_cast<unsigned char>(input[i + 1]) & 0x3FU) << 6;
            cp |= static_cast<unsigned char>(input[i + 2]) & 0x3FU;
            count = 3;
            if (cp < 0x800U || (cp >= 0xD800U && cp <= 0xDFFFU)) return false;
        } else if ((b0 & 0xF8U) == 0xF0U && i + 3 < input.size()) {
            cp = static_cast<std::uint32_t>(b0 & 0x07U) << 18;
            cp |= static_cast<std::uint32_t>(static_cast<unsigned char>(input[i + 1]) & 0x3FU) << 12;
            cp |= static_cast<std::uint32_t>(static_cast<unsigned char>(input[i + 2]) & 0x3FU) << 6;
            cp |= static_cast<unsigned char>(input[i + 3]) & 0x3FU;
            count = 4;
            if (cp < 0x10000U || cp > 0x10FFFFU) return false;
        } else {
            return false;
        }
        for (std::size_t j = 1; j < count; ++j) {
            if ((static_cast<unsigned char>(input[i + j]) & 0xC0U) != 0x80U) return false;
        }
        if (cp <= 0xFFFFU) {
            if (outputLength >= TooltipReserveCodepoints) return false;
            output[outputLength++] = static_cast<std::uint16_t>(cp);
        } else {
            if (outputLength + 2 > TooltipReserveCodepoints) return false;
            cp -= 0x10000U;
            output[outputLength++] = static_cast<std::uint16_t>(0xD800U + (cp >> 10));
            output[outputLength++] = static_cast<std::uint16_t>(0xDC00U + (cp & 0x3FFU));
        }
        i += count;
    }
    return true;
}

[[nodiscard]] bool SetVisible(D2RL::Widgets::WidgetHandle handle, bool visible) noexcept;
[[nodiscard]] bool SetEnabled(D2RL::Widgets::WidgetHandle handle, bool enabled) noexcept;

[[nodiscard]] bool ResolveWidgetHandles() noexcept {
    if (HandlesResolved) return true;
    if (Context == nullptr || Widgets == nullptr) return false;

    HudPanel = D2RL::Widgets::InvalidHandle;
    GridWidget = D2RL::Widgets::InvalidHandle;
    Handles = {};

    if (Widgets->findPanel(Context, "buff-panel/BuffHud", &HudPanel) != D2RL::Widgets::Result::Success
        || Widgets->findWidget(Context, HudPanel, "BuffGrid", &GridWidget) != D2RL::Widgets::Result::Success) {
        WidgetResolveFailures.fetch_add(1, std::memory_order_relaxed);
        return false;
    }

    for (std::size_t i = 0; i < Handles.size(); ++i) {
        char slotName[24]{};
        std::snprintf(slotName, sizeof(slotName), "BuffSlot%02zu", i);
        auto& slot = Handles[i];
        if (Widgets->findWidget(Context, GridWidget, slotName, &slot.slot)
            != D2RL::Widgets::Result::Success) {
            WidgetResolveFailures.fetch_add(1, std::memory_order_relaxed);
            Handles = {};
            return false;
        }
        for (std::size_t atlas = 0; atlas < AtlasCount; ++atlas) {
            if (Widgets->findWidget(Context, slot.slot, AtlasWidgetNames[atlas], &slot.icons[atlas])
                != D2RL::Widgets::Result::Success) {
                WidgetResolveFailures.fetch_add(1, std::memory_order_relaxed);
                Handles = {};
                return false;
            }
        }
        if (Widgets->findWidget(Context, slot.slot, "Countdown", &slot.countdown)
            != D2RL::Widgets::Result::Success) {
            WidgetResolveFailures.fetch_add(1, std::memory_order_relaxed);
            Handles = {};
            return false;
        }

        // ButtonWidget visibility alone does not guarantee that D2R removes its
        // hit target. Empty BuffHud slots are display placeholders, so disable
        // every atlas button as soon as handles are resolved. Occupied slots
        // keep the selected atlas visible but disabled. A separate slot-local
        // FocusableWidget owns hover-only tooltip presentation.
        for (const auto icon : slot.icons) {
            if (!SetEnabled(icon, false)) {
                WidgetEnableFailures.fetch_add(1, std::memory_order_relaxed);
            }
        }
    }

    HandlesResolved = true;
    return true;
}

void RestoreQualifiedCountdownBuffers() noexcept {
    for (auto& state : RenderStates) {
        if (state.qualifiedTimerBuffer == 0) continue;
        auto* buffer = reinterpret_cast<char*>(state.qualifiedTimerBuffer);
        if (IsWritableRange(buffer, TimerReserveBytes)) {
            std::memcpy(buffer, TimerReserve, TimerReserveBytes);
        }
    }
}

void RestoreQualifiedTooltipBuffer(std::size_t slotIndex, SlotRenderState& state) noexcept {
    if (slotIndex >= SlotCount
        || state.qualifiedTooltipWidget == 0
        || state.qualifiedTooltipBuffer == 0
        || state.qualifiedTooltipFieldOffset == std::numeric_limits<std::size_t>::max()
        || state.qualifiedTooltipEncoding == TooltipStorageEncoding::Unknown) {
        return;
    }

    const auto stringObjectAddress = state.qualifiedTooltipWidget + state.qualifiedTooltipFieldOffset;
    if (!IsWritableRange(
            reinterpret_cast<void*>(stringObjectAddress + Native::Contract::BlizzardStringSizeOffset),
            sizeof(std::uint64_t))) {
        return;
    }

    std::uint64_t length{};
    if (state.qualifiedTooltipEncoding == TooltipStorageEncoding::BlizzardUtf8) {
        if (!IsWritableRange(reinterpret_cast<void*>(state.qualifiedTooltipBuffer), TooltipReserveBytes)) return;
        char reserve[TooltipReserveBytes]{};
        MakeTooltipReserve(slotIndex, reserve);
        std::memcpy(reinterpret_cast<void*>(state.qualifiedTooltipBuffer), reserve, TooltipReserveBytes);
        length = TooltipReserveLength;
    } else if (state.qualifiedTooltipEncoding == TooltipStorageEncoding::BlizzardUtf16) {
        constexpr std::size_t Utf16Bytes = (TooltipReserveCodepoints + 1) * sizeof(std::uint16_t);
        if (!IsWritableRange(reinterpret_cast<void*>(state.qualifiedTooltipBuffer), Utf16Bytes)) return;
        std::array<std::uint16_t, TooltipReserveCodepoints + 1> reserve{};
        MakeTooltipReserveUtf16(slotIndex, reserve);
        std::memcpy(reinterpret_cast<void*>(state.qualifiedTooltipBuffer), reserve.data(), Utf16Bytes);
        length = TooltipReserveCodepoints;
    } else {
        return;
    }

    std::memcpy(
        reinterpret_cast<void*>(stringObjectAddress + Native::Contract::BlizzardStringSizeOffset),
        &length,
        sizeof(length));
}

void RestoreQualifiedTooltipBuffers() noexcept {
    for (std::size_t slotIndex = 0; slotIndex < RenderStates.size(); ++slotIndex) {
        RestoreQualifiedTooltipBuffer(slotIndex, RenderStates[slotIndex]);
    }
}

void InvalidateWidgetHandles() noexcept {
    HudPanel = D2RL::Widgets::InvalidHandle;
    GridWidget = D2RL::Widgets::InvalidHandle;
    Handles = {};
    RenderStates = {};
    HandlesResolved = false;
    LastRenderedRevision = std::numeric_limits<std::uint64_t>::max();
}

[[nodiscard]] bool SetVisible(D2RL::Widgets::WidgetHandle handle, bool visible) noexcept {
    if (Context == nullptr || Widgets == nullptr || handle == D2RL::Widgets::InvalidHandle) return false;
    return Widgets->setWidgetVisible(Context, handle, visible) == D2RL::Widgets::Result::Success;
}

[[nodiscard]] bool SetEnabled(D2RL::Widgets::WidgetHandle handle, bool enabled) noexcept {
    if (Context == nullptr || Widgets == nullptr || handle == D2RL::Widgets::InvalidHandle) return false;
    return Widgets->setWidgetEnabled(Context, handle, enabled) == D2RL::Widgets::Result::Success;
}

[[nodiscard]] void* ResolveNativeSlotChild(std::size_t slotIndex, const char* childName) noexcept {
    if (FindTopLevelPanel == nullptr || FindChildWidgetByName == nullptr
        || slotIndex >= SlotCount || childName == nullptr) {
        return nullptr;
    }
    void* panel = FindTopLevelPanel("buff-panel/BuffHud");
    void* grid = panel != nullptr ? FindChildWidgetByName(panel, "BuffGrid") : nullptr;
    char slotName[24]{};
    std::snprintf(slotName, sizeof(slotName), "BuffSlot%02zu", slotIndex);
    void* slot = grid != nullptr ? FindChildWidgetByName(grid, slotName) : nullptr;
    return slot != nullptr ? FindChildWidgetByName(slot, childName) : nullptr;
}

[[nodiscard]] bool WriteCountdownText(std::size_t slotIndex, const char* text) noexcept {
    if (slotIndex >= SlotCount || text == nullptr) return false;
    const auto length = std::strlen(text);
    if (length + 1 > TimerReserveBytes) return false;

    void* widget = ResolveNativeSlotChild(slotIndex, "Countdown");
    std::uintptr_t pointer{};
    if (widget == nullptr
        || !ReadNativeField(widget, Native::Contract::HudTextPointerOffset, pointer)
        || pointer == 0) {
        TimerWriteFailures.fetch_add(1, std::memory_order_relaxed);
        return false;
    }

    auto* buffer = reinterpret_cast<char*>(pointer);
    if (!IsWritableRange(buffer, TimerReserveBytes)) {
        TimerWriteFailures.fetch_add(1, std::memory_order_relaxed);
        return false;
    }

    auto& state = RenderStates[slotIndex];
    if (state.qualifiedTimerBuffer != pointer) {
        if (!IsTimerReserve(buffer)) {
            TimerWriteFailures.fetch_add(1, std::memory_order_relaxed);
            return false;
        }
        state.qualifiedTimerBuffer = pointer;
    }

    std::memcpy(buffer, TimerReserve, TimerReserveBytes);
    std::memcpy(buffer, text, length + 1);
    if (std::memcmp(buffer, text, length + 1) != 0) {
        TimerWriteFailures.fetch_add(1, std::memory_order_relaxed);
        return false;
    }
    TimerWrites.fetch_add(1, std::memory_order_relaxed);
    return true;
}

[[nodiscard]] bool QualifyTooltipString(
    std::size_t slotIndex,
    void* widget,
    SlotRenderState& state) noexcept {
    if (widget == nullptr || slotIndex >= SlotCount) return false;

    char expectedUtf8[TooltipReserveBytes]{};
    MakeTooltipReserve(slotIndex, expectedUtf8);
    std::array<std::uint16_t, TooltipReserveCodepoints + 1> expectedUtf16{};
    MakeTooltipReserveUtf16(slotIndex, expectedUtf16);
    constexpr std::size_t Utf16Bytes = (TooltipReserveCodepoints + 1) * sizeof(std::uint16_t);

    std::size_t candidateCount{};
    std::size_t candidateOffset{};
    std::uintptr_t candidateBuffer{};
    TooltipStorageEncoding candidateEncoding{TooltipStorageEncoding::Unknown};

    // Build 93847 already has one independently runtime-qualified Blizzard
    // string layout: TooltipsPanel+0x180. It is {data@+0x00, size@+0x08,
    // capacity/flags@+0x10}, not an MSVC std::string. The previous BuffHud
    // implementation searched FocusableWidget for an MSVC basic_string tuple,
    // which explains the runtime witness tooltipQualFailures>0 with
    // skillNames=ready/name=Fade. Qualify the same blz::basic_string layout
    // here. Accept both char and 16-bit character storage because UI layout
    // fields are not guaranteed to use the same character width.
    for (std::size_t offset = 0;
         offset + Native::Contract::BlizzardStringCapacityFlagsOffset + sizeof(std::uint64_t) <= TooltipNativeScanBytes;
         offset += alignof(std::uintptr_t)) {
        std::uintptr_t dataPointer{};
        std::uint64_t size{};
        std::uint64_t capacityFlags{};
        if (!ReadNativeField(widget, offset + Native::Contract::BlizzardStringDataOffset, dataPointer)
            || !ReadNativeField(widget, offset + Native::Contract::BlizzardStringSizeOffset, size)
            || !ReadNativeField(widget, offset + Native::Contract::BlizzardStringCapacityFlagsOffset, capacityFlags)) {
            continue;
        }

        const auto capacity = capacityFlags & Native::Contract::BlizzardStringCapacityMask;
        const bool embedded = (capacityFlags & Native::Contract::BlizzardStringEmbeddedFlag) != 0;
        if (embedded
            || dataPointer == 0
            || capacity > Native::Contract::MaximumTooltipCapacityBytes) {
            continue;
        }

        TooltipStorageEncoding encoding{TooltipStorageEncoding::Unknown};
        if (size == TooltipReserveLength
            && capacity >= TooltipReserveLength
            && IsWritableRange(reinterpret_cast<void*>(dataPointer), TooltipReserveBytes)
            && std::memcmp(reinterpret_cast<const void*>(dataPointer), expectedUtf8, TooltipReserveBytes) == 0) {
            encoding = TooltipStorageEncoding::BlizzardUtf8;
        } else if (size == TooltipReserveCodepoints
            && capacity >= TooltipReserveCodepoints
            && IsWritableRange(reinterpret_cast<void*>(dataPointer), Utf16Bytes)
            && std::memcmp(reinterpret_cast<const void*>(dataPointer), expectedUtf16.data(), Utf16Bytes) == 0) {
            encoding = TooltipStorageEncoding::BlizzardUtf16;
        }
        if (encoding == TooltipStorageEncoding::Unknown) continue;

        ++candidateCount;
        candidateOffset = offset;
        candidateBuffer = dataPointer;
        candidateEncoding = encoding;
    }

    if (candidateCount != 1) {
        TooltipQualificationFailures.fetch_add(1, std::memory_order_relaxed);
        return false;
    }

    state.qualifiedTooltipWidget = reinterpret_cast<std::uintptr_t>(widget);
    state.qualifiedTooltipBuffer = candidateBuffer;
    state.qualifiedTooltipFieldOffset = candidateOffset;
    state.qualifiedTooltipEncoding = candidateEncoding;
    return true;
}

[[nodiscard]] bool EnsureTooltipQualified(std::size_t slotIndex, SlotRenderState& state) noexcept {
    if (slotIndex >= SlotCount) return false;
    void* widget = ResolveNativeSlotChild(slotIndex, "Tooltip");
    if (widget == nullptr) {
        TooltipWriteFailures.fetch_add(1, std::memory_order_relaxed);
        return false;
    }

    const auto widgetAddress = reinterpret_cast<std::uintptr_t>(widget);
    if (state.qualifiedTooltipWidget == widgetAddress
        && state.qualifiedTooltipBuffer != 0
        && state.qualifiedTooltipFieldOffset != std::numeric_limits<std::size_t>::max()
        && state.qualifiedTooltipEncoding != TooltipStorageEncoding::Unknown) {
        return true;
    }

    RestoreQualifiedTooltipBuffer(slotIndex, state);
    state.qualifiedTooltipWidget = 0;
    state.qualifiedTooltipBuffer = 0;
    state.qualifiedTooltipFieldOffset = std::numeric_limits<std::size_t>::max();
    state.qualifiedTooltipEncoding = TooltipStorageEncoding::Unknown;
    return QualifyTooltipString(slotIndex, widget, state);
}

[[nodiscard]] bool WriteTooltipText(std::size_t slotIndex, const char* text) noexcept {
    if (slotIndex >= SlotCount || text == nullptr) return false;
    const auto utf8Length = std::strlen(text);
    if (utf8Length > TooltipReserveLength) {
        TooltipWriteFailures.fetch_add(1, std::memory_order_relaxed);
        return false;
    }

    auto& state = RenderStates[slotIndex];
    if (!EnsureTooltipQualified(slotIndex, state)) {
        TooltipWriteFailures.fetch_add(1, std::memory_order_relaxed);
        return false;
    }

    const auto stringObject = state.qualifiedTooltipWidget + state.qualifiedTooltipFieldOffset;
    std::uintptr_t dataPointer{};
    std::uint64_t capacityFlags{};
    if (!ReadNativeField(
            reinterpret_cast<void*>(state.qualifiedTooltipWidget),
            state.qualifiedTooltipFieldOffset + Native::Contract::BlizzardStringDataOffset,
            dataPointer)
        || !ReadNativeField(
            reinterpret_cast<void*>(state.qualifiedTooltipWidget),
            state.qualifiedTooltipFieldOffset + Native::Contract::BlizzardStringCapacityFlagsOffset,
            capacityFlags)
        || dataPointer == 0
        || dataPointer != state.qualifiedTooltipBuffer) {
        TooltipWriteFailures.fetch_add(1, std::memory_order_relaxed);
        return false;
    }

    const auto capacity = capacityFlags & Native::Contract::BlizzardStringCapacityMask;
    const bool embedded = (capacityFlags & Native::Contract::BlizzardStringEmbeddedFlag) != 0;
    if (embedded
        || !IsWritableRange(
            reinterpret_cast<void*>(stringObject + Native::Contract::BlizzardStringSizeOffset),
            sizeof(std::uint64_t))) {
        TooltipWriteFailures.fetch_add(1, std::memory_order_relaxed);
        return false;
    }

    std::uint64_t nativeLength{};
    if (state.qualifiedTooltipEncoding == TooltipStorageEncoding::BlizzardUtf8) {
        if (capacity < TooltipReserveLength
            || !IsWritableRange(reinterpret_cast<void*>(dataPointer), TooltipReserveBytes)) {
            TooltipWriteFailures.fetch_add(1, std::memory_order_relaxed);
            return false;
        }
        auto* buffer = reinterpret_cast<char*>(dataPointer);
        std::memset(buffer, 0, TooltipReserveBytes);
        if (utf8Length != 0) std::memcpy(buffer, text, utf8Length);
        nativeLength = utf8Length;
        if (std::memcmp(buffer, text, utf8Length + 1) != 0) {
            TooltipWriteFailures.fetch_add(1, std::memory_order_relaxed);
            return false;
        }
    } else if (state.qualifiedTooltipEncoding == TooltipStorageEncoding::BlizzardUtf16) {
        if (capacity < TooltipReserveCodepoints) {
            TooltipWriteFailures.fetch_add(1, std::memory_order_relaxed);
            return false;
        }
        std::array<std::uint16_t, TooltipReserveCodepoints + 1> converted{};
        std::size_t convertedLength{};
        if (!Utf8ToUtf16(text, converted, convertedLength)) {
            TooltipWriteFailures.fetch_add(1, std::memory_order_relaxed);
            return false;
        }
        constexpr std::size_t Utf16Bytes = (TooltipReserveCodepoints + 1) * sizeof(std::uint16_t);
        if (!IsWritableRange(reinterpret_cast<void*>(dataPointer), Utf16Bytes)) {
            TooltipWriteFailures.fetch_add(1, std::memory_order_relaxed);
            return false;
        }
        std::memset(reinterpret_cast<void*>(dataPointer), 0, Utf16Bytes);
        if (convertedLength != 0) {
            std::memcpy(reinterpret_cast<void*>(dataPointer), converted.data(), convertedLength * sizeof(std::uint16_t));
        }
        nativeLength = convertedLength;
    } else {
        TooltipWriteFailures.fetch_add(1, std::memory_order_relaxed);
        return false;
    }

    std::memcpy(
        reinterpret_cast<void*>(stringObject + Native::Contract::BlizzardStringSizeOffset),
        &nativeLength,
        sizeof(nativeLength));

    std::uint64_t verifyLength{};
    if (!ReadNativeField(
            reinterpret_cast<void*>(state.qualifiedTooltipWidget),
            state.qualifiedTooltipFieldOffset + Native::Contract::BlizzardStringSizeOffset,
            verifyLength)
        || verifyLength != nativeLength) {
        TooltipWriteFailures.fetch_add(1, std::memory_order_relaxed);
        return false;
    }

    TooltipWrites.fetch_add(1, std::memory_order_relaxed);
    return true;
}

void ClearTooltip(std::size_t slotIndex) noexcept {
    if (slotIndex >= SlotCount) return;
    if (WriteTooltipText(slotIndex, "")) {
        RenderStates[slotIndex].tooltipVisible = false;
    }
}

void ApplyTooltip(
    std::size_t slotIndex,
    std::int32_t sourceSkillId) noexcept {
    if (slotIndex >= SlotCount || !ResolveWidgetHandles()) return;

    char localizedName[TooltipReserveBytes]{};
    if (sourceSkillId == Core::NoSourceSkillId
        || !Internal::TryResolveSkillName(sourceSkillId, localizedName, sizeof(localizedName))) {
        TooltipNameResolveFailures.fetch_add(1, std::memory_order_relaxed);
        ClearTooltip(slotIndex);
        return;
    }

    if (!WriteTooltipText(slotIndex, localizedName)) {
        RenderStates[slotIndex].tooltipVisible = false;
        return;
    }

    // The selected atlas ButtonWidget remains disabled/click-through. Hover is
    // supplied by the slot-local FocusableWidget instead. Its tooltip buffer is
    // blank while the slot is inactive and contains only the localized SkillDesc
    // name while occupied; it has no click action and therefore does not turn the
    // buff icon back into a button.
    RenderStates[slotIndex].tooltipVisible = true;
}

[[nodiscard]] bool ResolveEntryIcon(
    const Core::BuffDisplayEntry& entry,
    SkillIconDescriptor& descriptor) noexcept {
    descriptor = {};
    if (entry.iconFrame != Core::AutoIconFrame) {
        descriptor.frame = entry.iconFrame;
        descriptor.atlas = entry.iconAtlas == Core::BuffIconAtlas::Auto
            ? Core::BuffIconAtlas::Global
            : entry.iconAtlas;
        return true;
    }
    if (entry.sourceSkillId != Core::NoSourceSkillId
        && Internal::TryResolveSkillIcon(entry.sourceSkillId, descriptor)) {
        return true;
    }
    descriptor = {.atlas = Core::BuffIconAtlas::Global, .frame = 0};
    return false;
}

[[nodiscard]] bool IsFrameInFuture(std::uint32_t expire, std::uint32_t now) noexcept {
    return static_cast<std::int32_t>(expire - now) > 0;
}

[[nodiscard]] std::uint32_t RemainingSeconds(std::uint32_t expire, std::uint32_t now) noexcept {
    const auto delta = static_cast<std::uint32_t>(expire - now);
    return (delta + FramesPerSecond - 1) / FramesPerSecond;
}

void FormatSeconds(std::uint32_t seconds, char (&output)[16]) noexcept {
    if (seconds >= 3600) {
        const auto hours = seconds / 3600;
        const auto minutes = (seconds % 3600) / 60;
        std::snprintf(output, sizeof(output), "%u:%02u", hours, minutes);
    } else if (seconds >= 60) {
        std::snprintf(output, sizeof(output), "%u:%02u", seconds / 60, seconds % 60);
    } else {
        std::snprintf(output, sizeof(output), "%u", seconds);
    }
}

[[nodiscard]] bool ApplyIcon(std::size_t slotIndex, const SkillIconDescriptor& icon) noexcept {
    if (slotIndex >= SlotCount || !ResolveWidgetHandles()) return false;
    auto& handles = Handles[slotIndex];
    const auto selected = AtlasIndex(icon.atlas);

    for (std::size_t i = 0; i < handles.icons.size(); ++i) {
        const bool selectedIcon = i == selected;
        // BuffHud is display-only. Even when a buff is active, its atlas button
        // must stay click-through, so keep every ButtonWidget disabled and only
        // use visibility to choose the currently rendered atlas.
        if (!SetEnabled(handles.icons[i], false)) {
            WidgetEnableFailures.fetch_add(1, std::memory_order_relaxed);
        }
        (void)SetVisible(handles.icons[i], selectedIcon);
    }

    void* nativeIcon = ResolveNativeSlotChild(slotIndex, AtlasWidgetNames[selected]);
    const auto declaredFrame = static_cast<std::uint16_t>(slotIndex * 2);
    if (Internal::TryApplyIconFrame(nativeIcon, icon.frame, declaredFrame)) return true;

    // Fail closed if the qualified live-frame path cannot be applied. The atlas
    // selection remains valid, but the slot keeps its JSON witness frame and the
    // fallback counter makes the failure visible in `buff-panel status`.
    IconFrameFallbacks.fetch_add(1, std::memory_order_relaxed);
    return false;
}

void HideSlot(std::size_t slotIndex) noexcept {
    if (slotIndex >= SlotCount || !ResolveWidgetHandles()) return;
    auto& state = RenderStates[slotIndex];
    if (!state.visible) return;

    // Visibility alone does not remove ButtonWidget hit targets. Once an
    // occupied slot becomes empty, disable every atlas child before hiding it.
    // Initial empty slots were already disabled by ResolveWidgetHandles().
    for (const auto icon : Handles[slotIndex].icons) {
        if (!SetEnabled(icon, false)) {
            WidgetEnableFailures.fetch_add(1, std::memory_order_relaxed);
        }
    }
    ClearTooltip(slotIndex);
    state.tooltipVisible = false;

    (void)SetVisible(Handles[slotIndex].slot, false);
    const auto qualifiedTimerBuffer = state.qualifiedTimerBuffer;
    const auto qualifiedTooltipWidget = state.qualifiedTooltipWidget;
    const auto qualifiedTooltipBuffer = state.qualifiedTooltipBuffer;
    const auto qualifiedTooltipFieldOffset = state.qualifiedTooltipFieldOffset;
    const auto qualifiedTooltipEncoding = state.qualifiedTooltipEncoding;
    state = {};
    state.qualifiedTimerBuffer = qualifiedTimerBuffer;
    state.qualifiedTooltipWidget = qualifiedTooltipWidget;
    state.qualifiedTooltipBuffer = qualifiedTooltipBuffer;
    state.qualifiedTooltipFieldOffset = qualifiedTooltipFieldOffset;
    state.qualifiedTooltipEncoding = qualifiedTooltipEncoding;
}

void RenderSlot(
    std::size_t slotIndex,
    const Core::BuffDisplayEntry& entry,
    std::uint32_t nowFrame,
    bool hasClock) noexcept {
    if (slotIndex >= SlotCount || !ResolveWidgetHandles()) return;

    SkillIconDescriptor icon{};
    const bool iconResolved = ResolveEntryIcon(entry, icon);
    auto& state = RenderStates[slotIndex];
    const bool assignmentChanged = !state.visible
        || state.key != entry.key
        || state.atlas != icon.atlas
        || state.requestedFrame != icon.frame
        || state.displayMode != entry.displayMode
        || state.sourceSkillId != entry.sourceSkillId;

    if (assignmentChanged) {
        if (!iconResolved) IconResolveFailures.fetch_add(1, std::memory_order_relaxed);
        (void)SetVisible(Handles[slotIndex].slot, false);
        const bool iconApplied = ApplyIcon(slotIndex, icon);
        state.key = entry.key;
        state.atlas = icon.atlas;
        state.requestedFrame = icon.frame;
        state.renderedFrame = iconApplied
            ? icon.frame
            : static_cast<std::uint16_t>(slotIndex * 2);
        state.displayMode = entry.displayMode;
        state.sourceSkillId = entry.sourceSkillId;
        state.seconds = NoRenderedSeconds;
        state.currentValue = NoRenderedValue;
        state.maximumValue = NoRenderedValue;
        state.timerVisible = false;
        state.tooltipVisible = false;
        ApplyTooltip(slotIndex, entry.sourceSkillId);
        state.visible = true;
        (void)SetVisible(Handles[slotIndex].slot, true);
        LayoutRefreshes.fetch_add(1, std::memory_order_relaxed);
    }

    if (entry.displayMode == Core::BuffDisplayMode::Resource) {
        if (state.timerVisible && state.currentValue == entry.currentValue) {
            return;
        }

        char text[32]{};
        std::snprintf(text, sizeof(text), "%d", entry.currentValue);
        if (WriteCountdownText(slotIndex, text)) {
            (void)SetVisible(Handles[slotIndex].countdown, true);
            state.timerVisible = true;
            state.seconds = NoRenderedSeconds;
            state.currentValue = entry.currentValue;
            state.maximumValue = entry.maximumValue;
        }
        return;
    }

    const bool wantsTimer = entry.expireGameFrame != 0;
    if (!wantsTimer) {
        if (state.timerVisible) {
            (void)SetVisible(Handles[slotIndex].countdown, false);
            state.timerVisible = false;
            state.seconds = NoRenderedSeconds;
            state.currentValue = NoRenderedValue;
            state.maximumValue = NoRenderedValue;
        }
        return;
    }

    if (!hasClock) {
        if (state.seconds != NoRenderedSeconds || !state.timerVisible) {
            if (WriteCountdownText(slotIndex, "--")) {
                (void)SetVisible(Handles[slotIndex].countdown, true);
                state.timerVisible = true;
                state.seconds = NoRenderedSeconds;
            }
        }
        return;
    }

    const auto seconds = RemainingSeconds(entry.expireGameFrame, nowFrame);
    if (state.timerVisible && state.seconds == seconds) return;

    char text[16]{};
    FormatSeconds(seconds, text);
    if (WriteCountdownText(slotIndex, text)) {
        (void)SetVisible(Handles[slotIndex].countdown, true);
        state.timerVisible = true;
        state.seconds = seconds;
    }
}

[[nodiscard]] bool EffectiveFrame(
    const Core::BuffDisplaySnapshot& snapshot,
    std::uint32_t& frame) noexcept {
    if (snapshot.hasGameFrame) {
        frame = snapshot.currentGameFrame;
        DebugClockActive.store(false, std::memory_order_release);
        return true;
    }

    if (!DebugClockActive.load(std::memory_order_acquire)
        || DebugClockSession.load(std::memory_order_acquire) != snapshot.sessionGeneration) {
        frame = 0;
        return false;
    }

    const auto startTick = DebugClockStartTick.load(std::memory_order_acquire);
    const auto nowTick = GetTickCount64();
    const auto elapsed = nowTick >= startTick ? nowTick - startTick : 0;
    const auto advanced = static_cast<std::uint64_t>(elapsed) * FramesPerSecond / 1000ULL;
    frame = DebugClockStartFrame.load(std::memory_order_acquire)
        + static_cast<std::uint32_t>(advanced);
    return true;
}

void RenderSnapshot() noexcept {
    if (!ResolveWidgetHandles()) return;

    auto snapshot = Core::BuffDisplays().Snapshot();
    std::uint32_t nowFrame{};
    const bool hasClock = EffectiveFrame(snapshot, nowFrame);

    // The display bus is not gameplay state. Once authoritative time says an
    // entry is over, removing it from the display registry is safe and keeps
    // stale producers from occupying HUD slots forever.
    if (hasClock) {
        std::array<std::uint64_t, Core::MaximumPublishedBuffs> expired{};
        std::size_t expiredCount{};
        for (std::size_t i = 0; i < snapshot.count; ++i) {
            const auto& entry = snapshot.entries[i];
            if (entry.expireGameFrame != 0
                && !IsFrameInFuture(entry.expireGameFrame, nowFrame)) {
                expired[expiredCount++] = entry.key;
            }
        }
        if (expiredCount != 0) {
            for (std::size_t i = 0; i < expiredCount; ++i) {
                if (Core::BuffDisplays().Remove(expired[i])) {
                    ExpiredEntriesRemoved.fetch_add(1, std::memory_order_relaxed);
                }
            }
            snapshot = Core::BuffDisplays().Snapshot();
            if (snapshot.count == 0 && !snapshot.hasGameFrame) {
                DebugClockActive.store(false, std::memory_order_release);
            }
        }
    }

    std::array<Core::BuffDisplayEntry, Core::MaximumPublishedBuffs> ordered{};
    std::size_t orderedCount{};
    for (std::size_t i = 0; i < snapshot.count; ++i) ordered[orderedCount++] = snapshot.entries[i];
    std::sort(
        ordered.begin(),
        ordered.begin() + static_cast<std::ptrdiff_t>(orderedCount),
        [](const Core::BuffDisplayEntry& lhs, const Core::BuffDisplayEntry& rhs) noexcept {
            if (lhs.priority != rhs.priority) return lhs.priority > rhs.priority;
            return lhs.sequence < rhs.sequence;
        });

    const auto visibleCount = std::min<std::size_t>(orderedCount, SlotCount);
    for (std::size_t i = 0; i < visibleCount; ++i) {
        RenderSlot(i, ordered[i], nowFrame, hasClock);
    }
    for (std::size_t i = visibleCount; i < SlotCount; ++i) HideSlot(i);

    LastRenderedRevision = snapshot.revision;
}

void QueuePoll() noexcept;

void __cdecl PollOnUiThread(const D2RL::PluginContext* context, void*) noexcept {
    PollScheduled.store(false, std::memory_order_release);
    if (context == nullptr || context != Context
        || CurrentSessionGeneration.load(std::memory_order_acquire) == 0) {
        return;
    }

    Polls.fetch_add(1, std::memory_order_relaxed);
    RenderSnapshot();
    QueuePoll();
}

void QueuePoll() noexcept {
    if (Context == nullptr || Threads == nullptr
        || CurrentSessionGeneration.load(std::memory_order_acquire) == 0) {
        return;
    }
    bool expected = false;
    if (!PollScheduled.compare_exchange_strong(
            expected,
            true,
            std::memory_order_acq_rel,
            std::memory_order_relaxed)) {
        return;
    }
    if (Threads->runOnUiThread(Context, &PollOnUiThread, nullptr)
        != D2RL::Threads::Result::Success) {
        PollScheduled.store(false, std::memory_order_release);
        PollQueueFailures.fetch_add(1, std::memory_order_relaxed);
    }
}

void OpenPanel() noexcept {
    if (Context == nullptr || Panels == nullptr
        || RegisteredPanel == D2RL::Panels::InvalidHandle) return;
    const auto result = Panels->openPanel(Context, RegisteredPanel);
    if (result != D2RL::Panels::Result::Success) {
        PanelOpenFailures.fetch_add(1, std::memory_order_relaxed);
        return;
    }
    InvalidateWidgetHandles();
    if (ResolveWidgetHandles()) {
        for (std::size_t i = 0; i < SlotCount; ++i) {
            (void)SetVisible(Handles[i].slot, false);
            ClearTooltip(i);
        }
    }
}

void ClosePanel() noexcept {
    RestoreQualifiedCountdownBuffers();
    RestoreQualifiedTooltipBuffers();
    if (Context != nullptr && Panels != nullptr
        && RegisteredPanel != D2RL::Panels::InvalidHandle) {
        (void)Panels->closePanel(Context, RegisteredPanel);
    }
    InvalidateWidgetHandles();
}

void __cdecl RebuildIconsOnGameThread(const D2RL::PluginContext*, void*) noexcept {
    const auto previous = Internal::SkillIconStatus();
    const auto revision = previous.tableRevision == 0 ? 1 : previous.tableRevision;
    const bool ready = Internal::RebuildSkillIconCache(revision);
    if (Context != nullptr) {
        const auto status = Internal::SkillIconStatus();
        char line[384]{};
        std::snprintf(
            line,
            sizeof(line),
            "BuffPanel BuffHud: manual skill presentation cache rebuild %s bank=%u skills=%u desc=%u rowSize=%u/%u candidates(link=%u class=%u name=%u) offsets(link=0x%X class=0x%X icon=0x%X nameId=0x%X) names=%s.",
            ready ? "succeeded" : "FAILED",
            status.bank,
            status.skillCount,
            status.skillDescCount,
            status.skillsRowSize,
            status.skillDescRowSize,
            status.linkCandidateCount,
            status.classCandidateCount,
            status.nameCandidateCount,
            status.skillDescLinkOffset,
            status.skillClassOffset,
            status.iconCelOffset,
            status.skillNameStringIdOffset,
            status.namesReady ? "ready" : "not-qualified");
        ready ? Context->LogInfo(line) : Context->LogWarn(line);
    }
}

void __cdecl OnDataTablesLoaded(
    const D2RL::PluginContext*,
    const D2RL::Lifecycle::DataTablesLoadedEvent* event,
    void*) noexcept {
    if (event == nullptr) return;
    const bool ready = Internal::RebuildSkillIconCache(event->revision);
    if (Context == nullptr) return;
    const auto status = Internal::SkillIconStatus();
    char line[384]{};
    std::snprintf(
        line,
        sizeof(line),
        "BuffPanel BuffHud: skill presentation table resolver %s revision=%llu bank=%u skills=%u desc=%u rowSize=%u/%u candidates(link=%u class=%u name=%u) offsets(link=0x%X class=0x%X icon=0x%X nameId=0x%X) names=%s.",
        ready ? "ready" : "not-qualified",
        static_cast<unsigned long long>(event->revision),
        status.bank,
        status.skillCount,
        status.skillDescCount,
        status.skillsRowSize,
        status.skillDescRowSize,
        status.linkCandidateCount,
        status.classCandidateCount,
        status.nameCandidateCount,
        status.skillDescLinkOffset,
        status.skillClassOffset,
        status.iconCelOffset,
        status.skillNameStringIdOffset,
        status.namesReady ? "ready" : "not-qualified");
    ready ? Context->LogInfo(line) : Context->LogWarn(line);
}

void __cdecl OnGameplayEvent(
    const D2RL::PluginContext*,
    const D2RL::Lifecycle::GameplayEvent* event,
    void*) noexcept {
    if (event == nullptr) return;

    switch (event->kind) {
    case D2RL::Lifecycle::GameplayEventKind::GameJoined:
        CurrentSessionGeneration.store(event->sessionGeneration, std::memory_order_release);
        Core::BuffDisplays().BeginSession(event->sessionGeneration);
        DebugClockActive.store(false, std::memory_order_release);
        break;
    case D2RL::Lifecycle::GameplayEventKind::LocalPlayerReady:
        if (CurrentSessionGeneration.load(std::memory_order_acquire) != event->sessionGeneration) {
            CurrentSessionGeneration.store(event->sessionGeneration, std::memory_order_release);
            Core::BuffDisplays().BeginSession(event->sessionGeneration);
        }
        OpenPanel();
        QueuePoll();
        if (!Internal::SkillIconStatus().ready && Threads != nullptr) {
            (void)Threads->runOnGameThread(Context, &RebuildIconsOnGameThread, nullptr);
        }
        break;
    case D2RL::Lifecycle::GameplayEventKind::GameLeft:
        Core::BuffDisplays().EndSession(event->sessionGeneration);
        CurrentSessionGeneration.store(0, std::memory_order_release);
        DebugClockActive.store(false, std::memory_order_release);
        PollScheduled.store(false, std::memory_order_release);
        ClosePanel();
        break;
    default:
        break;
    }
}

[[nodiscard]] std::string_view Trim(std::string_view value) noexcept {
    while (!value.empty() && std::isspace(static_cast<unsigned char>(value.front())) != 0) value.remove_prefix(1);
    while (!value.empty() && std::isspace(static_cast<unsigned char>(value.back())) != 0) value.remove_suffix(1);
    return value;
}

[[nodiscard]] bool NextToken(std::string_view& input, std::string_view& token) noexcept {
    input = Trim(input);
    if (input.empty()) {
        token = {};
        return false;
    }
    std::size_t length{};
    while (length < input.size()
        && std::isspace(static_cast<unsigned char>(input[length])) == 0) ++length;
    token = input.substr(0, length);
    input.remove_prefix(length);
    return true;
}

[[nodiscard]] bool ParseU32(std::string_view token, std::uint32_t& value) noexcept {
    token = Trim(token);
    if (token.empty()) return false;
    const auto result = std::from_chars(token.data(), token.data() + token.size(), value);
    return result.ec == std::errc{} && result.ptr == token.data() + token.size();
}

void PrintStatus(const D2RL::PluginContext* context) noexcept {
    if (context == nullptr) return;
    const auto snapshot = Core::BuffDisplays().Snapshot();
    const auto icons = Internal::SkillIconStatus();
    char line[1536]{};
    std::snprintf(
        line,
        sizeof(line),
        "Buff Panel 1.0.4 BuffHud: layout=3x7-lower-left-fill active=%zu/%zu session=%llu clock=%s frame=%u revision=%llu panel=%s frameBackend=%s skillIcons=%s skillNames=%s bank=%u tableRevision=%llu offsets(link=0x%X class=0x%X icon=0x%X nameId=0x%X) candidates=%u/%u/%u polls=%llu layoutRefresh=%llu timerWrites=%llu timerFailures=%llu tooltipWrites=%llu tooltipWriteFailures=%llu tooltipQualFailures=%llu tooltipNameFailures=%llu tooltipStorage=%s iconResolveFailures=%llu frameFallbacks=%llu expired=%llu widgetFailures=%llu widgetEnableFailures=%llu panelOpenFailures=%llu.",
        snapshot.count,
        SlotCount,
        static_cast<unsigned long long>(snapshot.sessionGeneration),
        snapshot.hasGameFrame ? "game" : (DebugClockActive.load(std::memory_order_relaxed) ? "debug" : "none"),
        snapshot.currentGameFrame,
        static_cast<unsigned long long>(snapshot.revision),
        RegisteredPanel != D2RL::Panels::InvalidHandle ? "registered" : "missing",
        Internal::IconFrameBackendStatus() == IconFrameBackendState::Ready ? "ready" : "unavailable",
        icons.ready ? "ready" : "not-qualified",
        icons.namesReady ? "ready" : "not-qualified",
        icons.bank,
        static_cast<unsigned long long>(icons.tableRevision),
        icons.skillDescLinkOffset,
        icons.skillClassOffset,
        icons.iconCelOffset,
        icons.skillNameStringIdOffset,
        icons.linkCandidateCount,
        icons.classCandidateCount,
        icons.nameCandidateCount,
        static_cast<unsigned long long>(Polls.load(std::memory_order_relaxed)),
        static_cast<unsigned long long>(LayoutRefreshes.load(std::memory_order_relaxed)),
        static_cast<unsigned long long>(TimerWrites.load(std::memory_order_relaxed)),
        static_cast<unsigned long long>(TimerWriteFailures.load(std::memory_order_relaxed)),
        static_cast<unsigned long long>(TooltipWrites.load(std::memory_order_relaxed)),
        static_cast<unsigned long long>(TooltipWriteFailures.load(std::memory_order_relaxed)),
        static_cast<unsigned long long>(TooltipQualificationFailures.load(std::memory_order_relaxed)),
        static_cast<unsigned long long>(TooltipNameResolveFailures.load(std::memory_order_relaxed)),
        TooltipEncodingName(RenderStates[0].qualifiedTooltipEncoding),
        static_cast<unsigned long long>(IconResolveFailures.load(std::memory_order_relaxed)),
        static_cast<unsigned long long>(IconFrameFallbacks.load(std::memory_order_relaxed)),
        static_cast<unsigned long long>(ExpiredEntriesRemoved.load(std::memory_order_relaxed)),
        static_cast<unsigned long long>(WidgetResolveFailures.load(std::memory_order_relaxed)),
        static_cast<unsigned long long>(WidgetEnableFailures.load(std::memory_order_relaxed)),
        static_cast<unsigned long long>(PanelOpenFailures.load(std::memory_order_relaxed)));
    context->WriteConsoleMessage(line);
    char layoutLine[160]{};
    std::snprintf(layoutLine, sizeof(layoutLine),
        "Buff Panel: BuffHudhd.json source=%s bytes=%zu (restart D2R to reload).",
        LayoutSource == LooseLayout::Source::ActiveMod ? "active-mod" : "embedded",
        LayoutBytes);
    context->WriteConsoleMessage(layoutLine);

    const auto visible = std::min<std::size_t>(snapshot.count, SlotCount);
    for (std::size_t i = 0; i < visible; ++i) {
        const auto& entry = snapshot.entries[i];
        SkillIconDescriptor icon{};
        const bool resolved = ResolveEntryIcon(entry, icon);
        char localizedName[TooltipReserveBytes]{};
        const bool nameResolved = Internal::TryResolveSkillName(
            entry.sourceSkillId,
            localizedName,
            sizeof(localizedName));
        char item[448]{};
        std::snprintf(
            item,
            sizeof(item),
            "  key=0x%llX skill=%d name=%s icon=%s/%u%s mode=%s expireFrame=%u value=%d/%d stats=%d/%d stacks=%u priority=%d seq=%llu",
            static_cast<unsigned long long>(entry.key),
            entry.sourceSkillId,
            nameResolved ? localizedName : "<unresolved>",
            AtlasName(icon.atlas),
            static_cast<unsigned>(icon.frame),
            resolved ? "" : "(fallback)",
            entry.displayMode == Core::BuffDisplayMode::Resource ? "resource" : "timer",
            entry.expireGameFrame,
            entry.currentValue,
            entry.maximumValue,
            entry.valueStatId,
            entry.maxStatId,
            static_cast<unsigned>(entry.stacks),
            static_cast<int>(entry.priority),
            static_cast<unsigned long long>(entry.sequence));
        context->WriteConsoleMessage(item);
    }
}

void StartDebugTest(std::uint32_t seconds, std::uint32_t count) noexcept {
    if (seconds == 0 || count == 0) return;
    count = std::min<std::uint32_t>(count, static_cast<std::uint32_t>(SlotCount));
    const auto session = CurrentSessionGeneration.load(std::memory_order_acquire);
    if (session == 0) return;

    const auto snapshot = Core::BuffDisplays().Snapshot();
    std::uint32_t baseFrame = snapshot.hasGameFrame ? snapshot.currentGameFrame : 1;
    if (!snapshot.hasGameFrame) {
        DebugClockSession.store(session, std::memory_order_release);
        DebugClockStartFrame.store(baseFrame, std::memory_order_release);
        DebugClockStartTick.store(GetTickCount64(), std::memory_order_release);
        DebugClockActive.store(true, std::memory_order_release);
    }

    for (std::uint32_t i = 0; i < count; ++i) {
        Core::BuffDisplayEntry entry{};
        entry.key = DebugKeyBase + i + 1;
        entry.sourceSkillId = static_cast<std::int32_t>(6 + i); // Amazon witness sequence
        entry.expireGameFrame = baseFrame + seconds * FramesPerSecond;
        entry.priority = static_cast<std::int16_t>(1000 - static_cast<std::int32_t>(i));
        (void)Core::BuffDisplays().Upsert(entry);
    }
}

D2RL::ConsoleCommandResult __cdecl BuffCommand(
    D2R::Game::Client*,
    const D2RL::ConsoleCommandContext* command,
    void*) noexcept {
    if (command == nullptr || command->plugin == nullptr) return D2RL::ConsoleCommandResult::Failed;

    std::string_view args = command->args != nullptr
        ? std::string_view(command->args, command->argsLength)
        : std::string_view{};
    std::string_view action{};
    if (!NextToken(args, action) || action == "status") {
        PrintStatus(command->plugin);
        return D2RL::ConsoleCommandResult::Handled;
    }

    if (action == "clear") {
        Core::BuffDisplays().Clear();
        DebugClockActive.store(false, std::memory_order_release);
        command->plugin->WriteConsoleMessage("BuffPanel BuffHud: display registry cleared.");
        return D2RL::ConsoleCommandResult::Handled;
    }

    if (action == "test") {
        std::string_view secondsToken{};
        std::string_view countToken{};
        std::uint32_t seconds{60};
        std::uint32_t count{21};
        if (NextToken(args, secondsToken) && !ParseU32(secondsToken, seconds)) {
            return D2RL::ConsoleCommandResult::InvalidArguments;
        }
        if (NextToken(args, countToken) && !ParseU32(countToken, count)) {
            return D2RL::ConsoleCommandResult::InvalidArguments;
        }
        if (seconds == 0 || seconds > 86400 || count == 0 || count > SlotCount) {
            return D2RL::ConsoleCommandResult::InvalidArguments;
        }
        if (CurrentSessionGeneration.load(std::memory_order_acquire) == 0) {
            command->plugin->WriteConsoleWarning("BuffPanel BuffHud: enter a game before starting the display-only test.");
            return D2RL::ConsoleCommandResult::Failed;
        }
        StartDebugTest(seconds, count);
        char message[192]{};
        std::snprintf(message, sizeof(message), "BuffPanel BuffHud: published %u display-only test buffs for %u seconds.", count, seconds);
        command->plugin->WriteConsoleMessage(message);
        return D2RL::ConsoleCommandResult::Handled;
    }

    if (action == "rebuild-icons") {
        if (Threads == nullptr
            || Threads->runOnGameThread(Context, &RebuildIconsOnGameThread, nullptr)
                != D2RL::Threads::Result::Success) {
            command->plugin->WriteConsoleWarning("BuffPanel BuffHud: could not queue skill-icon cache rebuild on the game thread.");
            return D2RL::ConsoleCommandResult::Failed;
        }
        command->plugin->WriteConsoleMessage("BuffPanel BuffHud: skill-icon cache rebuild queued.");
        return D2RL::ConsoleCommandResult::Handled;
    }

    command->plugin->WriteConsoleMessage(
        "Usage: buff-panel [status | test [seconds] [count] | clear | rebuild-icons]");
    return D2RL::ConsoleCommandResult::InvalidArguments;
}

[[nodiscard]] bool RegisterLayoutAndPanel() noexcept {
    if (Context == nullptr || Resources == nullptr || Panels == nullptr) return false;
    const auto selected = LooseLayout::Select(
        Context->modDirectory,
        Context->activeMod,
        std::string_view(Internal::BuffHudLayout, sizeof(Internal::BuffHudLayout) - 1));
    if (selected.source == LooseLayout::Source::InvalidOverride) {
        std::string message = "Buff Panel: BuffHudhd.json rejected: " + selected.error;
        Context->LogError(message.c_str());
        return false;
    }
    const D2RL::Resources::ResourceRegistration resource{
        .structSize = D2RL::Resources::ResourceRegistrationSize,
        .flags = 0,
        .path = "data/global/ui/layouts/buff-panel/BuffHudhd.json",
        .bytes = selected.bytes.data(),
        .byteCount = static_cast<std::uint64_t>(selected.bytes.size()),
    };
    if (Resources->registerResource(Context, &resource, &LayoutResource)
        != D2RL::Resources::Result::Success) {
        Context->LogError("BuffPanel BuffHud: failed to register BuffHudhd.json resource.");
        return false;
    }
    // ResourceService copies selected.bytes before returning. Never keep a
    // pointer to temporary JSON memory after this registration.
    LayoutSource = selected.source;
    LayoutBytes = selected.bytes.size();
    if (selected.source == LooseLayout::Source::ActiveMod) {
        Context->LogInfo("Buff Panel: BuffHudhd.json source=active-mod (unpacked layout override).");
    } else {
        Context->LogInfo("Buff Panel: BuffHudhd.json source=embedded (default layout).");
    }

    const D2RL::Panels::PanelRegistration panel{
        .structSize = D2RL::Panels::PanelRegistrationSize,
        .flags = D2RL::Panels::PanelFlags::None,
        .localId = "BuffHud",
    };
    if (Panels->registerPanel(Context, &panel, &RegisteredPanel)
        != D2RL::Panels::Result::Success) {
        Context->LogError("BuffPanel BuffHud: failed to register plugin-owned BuffHud panel.");
        return false;
    }
    return true;
}

[[nodiscard]] bool RegisterLifecycle() noexcept {
    if (Context == nullptr || Lifecycle == nullptr) return false;
    const D2RL::Lifecycle::DataTablesLoadedListener tableListener{
        .structSize = D2RL::Lifecycle::DataTablesLoadedListenerSize,
        .flags = 0,
        .callback = &OnDataTablesLoaded,
        .userData = nullptr,
    };
    if (Lifecycle->registerDataTablesLoadedListener(Context, &tableListener, &DataTablesListener)
        != D2RL::Lifecycle::Result::Success) {
        return false;
    }

    constexpr std::array kinds{
        D2RL::Lifecycle::GameplayEventKind::GameJoined,
        D2RL::Lifecycle::GameplayEventKind::LocalPlayerReady,
        D2RL::Lifecycle::GameplayEventKind::GameLeft,
    };
    for (std::size_t i = 0; i < kinds.size(); ++i) {
        const D2RL::Lifecycle::GameplayEventListener listener{
            .structSize = D2RL::Lifecycle::GameplayEventListenerSize,
            .flags = 0,
            .kind = kinds[i],
            .reserved = 0,
            .callback = &OnGameplayEvent,
            .userData = nullptr,
        };
        if (Lifecycle->registerGameplayEventListener(Context, &listener, &GameplayListeners[i])
            != D2RL::Lifecycle::Result::Success) {
            return false;
        }
    }
    return true;
}

[[nodiscard]] bool ValidateNativeUiContract() noexcept {
    if (Context == nullptr || Context->exeBase == 0) return false;
    if (!Context->CheckExpectedBytes(
            Native::Contract::FindTopLevelPanelRva,
            Native::Contract::FindTopLevelPanelExpected.data(),
            static_cast<std::uint32_t>(Native::Contract::FindTopLevelPanelExpected.size()))
        || !Context->CheckExpectedBytes(
            Native::Contract::FindChildWidgetByNameRva,
            Native::Contract::FindChildWidgetByNameExpected.data(),
            static_cast<std::uint32_t>(Native::Contract::FindChildWidgetByNameExpected.size()))) {
        Context->LogError("BuffPanel BuffHud: native UI resolver fingerprint mismatch; refusing BuffHud initialization.");
        return false;
    }
    FindTopLevelPanel = reinterpret_cast<FindTopLevelPanelFn>(
        Context->exeBase + Native::Contract::FindTopLevelPanelRva);
    FindChildWidgetByName = reinterpret_cast<FindChildWidgetByNameFn>(
        Context->exeBase + Native::Contract::FindChildWidgetByNameRva);
    return true;
}

void ResetDiagnostics() noexcept {
    Polls.store(0, std::memory_order_relaxed);
    PollQueueFailures.store(0, std::memory_order_relaxed);
    LayoutRefreshes.store(0, std::memory_order_relaxed);
    TimerWrites.store(0, std::memory_order_relaxed);
    TimerWriteFailures.store(0, std::memory_order_relaxed);
    IconResolveFailures.store(0, std::memory_order_relaxed);
    IconFrameFallbacks.store(0, std::memory_order_relaxed);
    ExpiredEntriesRemoved.store(0, std::memory_order_relaxed);
    PanelOpenFailures.store(0, std::memory_order_relaxed);
    WidgetResolveFailures.store(0, std::memory_order_relaxed);
    WidgetEnableFailures.store(0, std::memory_order_relaxed);
    TooltipNameResolveFailures.store(0, std::memory_order_relaxed);
    TooltipQualificationFailures.store(0, std::memory_order_relaxed);
    TooltipWrites.store(0, std::memory_order_relaxed);
    TooltipWriteFailures.store(0, std::memory_order_relaxed);
}

} // namespace

bool Initialize(const D2RL::PluginContext* context) noexcept {
    Shutdown();
    if (context == nullptr) return false;
    Context = context;

    const auto& services = Core::Services();
    Resources = services.resources;
    Panels = services.panels;
    Widgets = services.widgets;
    Threads = services.threads;
    Lifecycle = services.lifecycle;

    const bool resourcesReady = Resources != nullptr
        && D2RL::HasResourceServiceField(Resources, D2RL::ResourceServiceRequiredSize);
    const bool panelsReady = Panels != nullptr
        && D2RL::HasPanelServiceField(Panels, D2RL::PanelServiceRequiredSize);
    const bool widgetsReady = Widgets != nullptr
        && D2RL::HasWidgetServiceField(Widgets, D2RL::WidgetServiceRequiredSize);
    const bool threadsReady = Threads != nullptr
        && D2RL::HasThreadServiceField(Threads, D2RL::ThreadServiceRequiredSize);
    const bool lifecycleReady = Lifecycle != nullptr
        && D2RL::HasLifecycleServiceField(Lifecycle, D2RL::LifecycleServiceRequiredSize);

    if (!resourcesReady || !panelsReady || !widgetsReady || !threadsReady || !lifecycleReady) {
        char serviceLine[256]{};
        std::snprintf(
            serviceLine,
            sizeof(serviceLine),
            "BuffPanel BuffHud: required PluginSDK service unavailable/undersized: Resource=%d Panel=%d Widget=%d Thread=%d Lifecycle=%d.",
            resourcesReady ? 1 : 0,
            panelsReady ? 1 : 0,
            widgetsReady ? 1 : 0,
            threadsReady ? 1 : 0,
            lifecycleReady ? 1 : 0);
        Context->LogError(serviceLine);
        Shutdown();
        return false;
    }

    // D2RLoader ABI 4 does not guarantee that every named service is active for
    // every loader build/runtime phase. Localization is presentation-only here:
    // icons/timers/resources remain fully functional without it. The skill cache
    // retries LocalizationService when data tables are ready, so a service that
    // becomes available later still restores localized hover names.
    const bool localizationReady = Core::LocalizationService() != nullptr;
    if (!localizationReady) {
        Context->LogWarn(
            "BuffPanel BuffHud: LocalizationService unavailable at plugin initialization; BuffHud remains enabled and localized hover names will be retried when the skill cache is rebuilt.");
    }

    if (!ValidateNativeUiContract()
        || !Internal::InitializeIconFrameBackend(Context)
        || !RegisterLayoutAndPanel()
        || !RegisterLifecycle()) {
        Shutdown();
        return false;
    }

    if (!Context->RegisterConsoleCommand(
            "buff-panel",
            &BuffCommand,
            "Show/test Buff Panel temporary-buff HUD state.")) {
        Context->LogWarn("BuffPanel BuffHud: console command 'buff-panel' could not be registered.");
    }

    ResetDiagnostics();
    Context->LogInfo(
        "Buff Panel 1.0.4 BuffHud initialized: production 3x7 lower-left-fill panel, 21 reusable slots, display-only click-through buff icons with slot-local FocusableWidget hover tooltips, timer/resource presentation with single-value resource counters, runtime Skills->SkillDesc icon + localized str-name cache resolver using the first WORD-aligned post-icon name field with Missing-string rejection, nine native skill atlases (including Warlock), stable priority ordering, session reset, and live ButtonWidget frame application through the build-93847 path visually qualified by Skill Icon HUD Probe 0.7.0.");
    return true;
}

void Shutdown() noexcept {
    Core::BuffDisplays().EndSession();
    Internal::ResetSkillIconCache();
    Internal::ShutdownIconFrameBackend();
    CurrentSessionGeneration.store(0, std::memory_order_release);
    PollScheduled.store(false, std::memory_order_release);
    DebugClockActive.store(false, std::memory_order_release);
    DebugClockStartTick.store(0, std::memory_order_release);
    DebugClockStartFrame.store(1, std::memory_order_release);
    DebugClockSession.store(0, std::memory_order_release);
    // Restore any native countdown backing buffers we qualified before dropping
    // their pointers. This is bounded/readability-checked and does not call UI
    // services from the loader shutdown thread.
    RestoreQualifiedCountdownBuffers();
    RestoreQualifiedTooltipBuffers();
    InvalidateWidgetHandles();

    LayoutResource = D2RL::Resources::InvalidHandle;
    LayoutSource = LooseLayout::Source::Embedded;
    LayoutBytes = 0;
    RegisteredPanel = D2RL::Panels::InvalidHandle;
    DataTablesListener = D2RL::Lifecycle::InvalidHandle;
    GameplayListeners = {};
    FindTopLevelPanel = nullptr;
    FindChildWidgetByName = nullptr;
    Resources = nullptr;
    Panels = nullptr;
    Widgets = nullptr;
    Threads = nullptr;
    Lifecycle = nullptr;
    Context = nullptr;
}

} // namespace BuffPanel::Systems::BuffHud
