#pragma once
#include <array>
#include <cstddef>
#include <cstdint>

// Build-93847 qualified fields and bridge signatures copied verbatim from SoE 0.18.148.
// Never use this DLL on a different D2R build without requalification.
namespace BuffPanel::Native::Contract {
inline constexpr std::uintptr_t GetUnitStatRva = 0x2F5020;
inline constexpr std::uintptr_t GetUnitStatBridgeSlotRva = 0x3E2A1B0;
inline constexpr std::array<std::uint8_t, 10> GetUnitStatExpected{
    0xFF, 0x25, 0x8A, 0x51, 0xB3, 0x03, 0x90, 0x90, 0x90, 0x90,
};

inline constexpr std::size_t BlizzardStringCapacityFlagsOffset = 0x10;

inline constexpr std::uint64_t BlizzardStringCapacityMask = 0x7FFFFFFFFFFFFFFFULL;

inline constexpr std::size_t BlizzardStringDataOffset = 0x00;

inline constexpr std::uint64_t BlizzardStringEmbeddedFlag = 0x8000000000000000ULL;

inline constexpr std::size_t BlizzardStringSizeOffset = 0x08;

inline constexpr std::size_t ButtonFrameDescriptorMemberOffsetField = 0x10;

inline constexpr std::size_t ButtonFrameDescriptorStorageWidthField = 0x20;

inline constexpr std::size_t ButtonFrameDescriptorStride = 0x30;

inline constexpr std::size_t ButtonFrameRenderSubobjectOffset = 0x680;

inline constexpr std::uintptr_t ButtonLiveFrameSetterRva = 0x859700;

inline constexpr std::uintptr_t ButtonNormalApplyBlockRva = 0x870298;

inline constexpr std::array<std::uint8_t, 26> ButtonNormalApplyExpected{
    0x8B, 0x93, 0xE8, 0x07, 0x00, 0x00,
    0x48, 0x8D, 0x8B, 0x80, 0x06, 0x00, 0x00,
    0xE8, 0x56, 0x94, 0xFE, 0xFF,
    0x48, 0x8B, 0xCB,
    0xE8, 0xAE, 0x01, 0x00, 0x00,
};

inline constexpr std::uintptr_t ButtonNormalFrameDescriptorRva = 0x2535580;

inline constexpr std::uintptr_t ButtonNormalFrameNameRva = 0x1D75670;

inline constexpr std::size_t ButtonNormalFrameOffset = 0x7E8;

inline constexpr std::uintptr_t ButtonNormalRefreshCallRva = 0x8702AD;

inline constexpr std::uintptr_t ButtonNormalSetterCallRva = 0x8702A5;

inline constexpr std::uintptr_t ButtonPressedApplyBlockRva = 0x870209;

inline constexpr std::array<std::uint8_t, 26> ButtonPressedApplyExpected{
    0x48, 0x81, 0xC1, 0x80, 0x06, 0x00, 0x00,
    0x8B, 0x93, 0xEC, 0x07, 0x00, 0x00,
    0xE8, 0xE5, 0x94, 0xFE, 0xFF,
    0x48, 0x8B, 0xCB,
    0xE8, 0x3D, 0x02, 0x00, 0x00,
};

inline constexpr std::uintptr_t ButtonPressedFrameDescriptorRva = 0x25355B0;

inline constexpr std::uintptr_t ButtonPressedFrameNameRva = 0x1D75680;

inline constexpr std::size_t ButtonPressedFrameOffset = 0x7EC;

inline constexpr std::uintptr_t ButtonPressedRefreshCallRva = 0x87021E;

inline constexpr std::uintptr_t ButtonPressedSetterCallRva = 0x870216;

inline constexpr std::uintptr_t ButtonRefreshRva = 0x870460;

inline constexpr std::array<std::uint8_t, 32> FindChildWidgetByNameExpected{
    0x48, 0x89, 0x5C, 0x24, 0x10, 0x48, 0x89, 0x74,
    0x24, 0x18, 0x57, 0x48, 0x83, 0xEC, 0x20, 0x48,
    0x8B, 0x59, 0x58, 0x48, 0x8B, 0xF2, 0x48, 0x8B,
    0x41, 0x60, 0x48, 0x8D, 0x3C, 0xC3, 0x48, 0x3B,
};

inline constexpr std::uintptr_t FindChildWidgetByNameRva = 0x856220;

inline constexpr std::array<std::uint8_t, 22> FindTopLevelPanelExpected{
    0x48, 0x8B, 0xD1, 0x48, 0x8B, 0x0D, 0xF6, 0x9F,
    0xBF, 0x02, 0x48, 0x85, 0xC9, 0x0F, 0x85, 0xDD,
    0x95, 0x05, 0x00, 0x33, 0xC0, 0xC3,
};

inline constexpr std::uintptr_t FindTopLevelPanelRva = 0x846170;

inline constexpr std::size_t GameFrameOffset = 0x170;

inline constexpr std::array<std::uint8_t, 5> GenericCurseStateLookupCallExpected{
    0xE8, 0x3F, 0x19, 0xEC, 0xFF,
};

inline constexpr std::uintptr_t GenericCurseStateLookupCallRva = 0x433FFC;

inline constexpr std::array<std::uint8_t, 32> GetGameFromUnitExpected{
    0x40, 0x53, 0x48, 0x83, 0xEC, 0x20, 0x48, 0x8B,
    0xD9, 0x48, 0x85, 0xC9, 0x75, 0x20, 0x88, 0x4C,
    0x24, 0x30, 0x48, 0x8D, 0x4C, 0x24, 0x30, 0xE8,
    0x44, 0xD9, 0xFF, 0xFF, 0x84, 0xC0, 0x74, 0x01,
};

inline constexpr std::uintptr_t GetGameFromUnitRva = 0x48FF00;

inline constexpr std::uintptr_t GetStatListFromUnitAndStateRva = 0x2F5940;

inline constexpr std::size_t HudTextPointerOffset = 0x88;

inline constexpr std::uint64_t MaximumTooltipCapacityBytes = 16384;

inline constexpr std::uintptr_t PostStatListBridgeSlotRva = 0x3E2A208;

inline constexpr std::array<std::uint8_t, 10> PostStatListExpected{
    0xFF, 0x25, 0x52, 0x75, 0xB3, 0x03, 0x90, 0x90, 0x90, 0x90,
};

inline constexpr std::uintptr_t PostStatListRva = 0x2F2CB0;

inline constexpr std::size_t StatListBuffExpireFrameFloatOffset = 0x24;

inline constexpr std::size_t StatListBuffFlagsOffset = 0x1C;

inline constexpr std::size_t StatListBuffMetadataBytes = 0x30;

inline constexpr std::size_t StatListBuffSkillIdOffset = 0x28;

inline constexpr std::size_t StatListBuffSkillLevelOffset = 0x2C;

inline constexpr std::size_t StatListBuffStateOffset = 0x20;
} // namespace BuffPanel::Native::Contract
