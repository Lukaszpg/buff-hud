#pragma once

#include "services.hpp"

#include <Windows.h>

#include <array>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <limits>

namespace BuffPanel::Core {

struct RipIndirectBridgeResolution final {
    std::uintptr_t entryRva{};
    std::uintptr_t slotRva{};
    std::uintptr_t targetAddress{};
    std::intptr_t slotDelta{};
};

namespace Detail {

[[nodiscard]] inline bool IsReadableProtection(DWORD protection) noexcept {
    if ((protection & (PAGE_GUARD | PAGE_NOACCESS)) != 0) return false;
    switch (protection & 0xFFu) {
    case PAGE_READONLY:
    case PAGE_READWRITE:
    case PAGE_WRITECOPY:
    case PAGE_EXECUTE:
    case PAGE_EXECUTE_READ:
    case PAGE_EXECUTE_READWRITE:
    case PAGE_EXECUTE_WRITECOPY:
        return true;
    default:
        return false;
    }
}

[[nodiscard]] inline bool IsExecutableProtection(DWORD protection) noexcept {
    if ((protection & (PAGE_GUARD | PAGE_NOACCESS)) != 0) return false;
    switch (protection & 0xFFu) {
    case PAGE_EXECUTE:
    case PAGE_EXECUTE_READ:
    case PAGE_EXECUTE_READWRITE:
    case PAGE_EXECUTE_WRITECOPY:
        return true;
    default:
        return false;
    }
}

[[nodiscard]] inline bool IsReadableRange(const void* address, std::size_t size) noexcept {
    if (address == nullptr || size == 0) return false;

    const auto begin = reinterpret_cast<std::uintptr_t>(address);
    if (begin > std::numeric_limits<std::uintptr_t>::max() - size) return false;
    const auto end = begin + size;

    MEMORY_BASIC_INFORMATION mbi{};
    if (VirtualQuery(address, &mbi, sizeof(mbi)) != sizeof(mbi)
        || mbi.State != MEM_COMMIT
        || !IsReadableProtection(mbi.Protect)) {
        return false;
    }

    const auto regionBegin = reinterpret_cast<std::uintptr_t>(mbi.BaseAddress);
    if (regionBegin > std::numeric_limits<std::uintptr_t>::max() - mbi.RegionSize) return false;
    const auto regionEnd = regionBegin + mbi.RegionSize;
    return begin >= regionBegin && end <= regionEnd;
}

[[nodiscard]] inline bool IsExecutableAddress(std::uintptr_t address) noexcept {
    if (address == 0) return false;
    MEMORY_BASIC_INFORMATION mbi{};
    const auto* pointer = reinterpret_cast<const void*>(address);
    return VirtualQuery(pointer, &mbi, sizeof(mbi)) == sizeof(mbi)
        && mbi.State == MEM_COMMIT
        && IsExecutableProtection(mbi.Protect);
}

[[nodiscard]] inline bool IsWithinRvaDrift(
    std::uintptr_t actual,
    std::uintptr_t qualified,
    std::uintptr_t maximumDrift) noexcept {
    return actual >= qualified
        ? actual - qualified <= maximumDrift
        : qualified - actual <= maximumDrift;
}

} // namespace Detail

// D2RLoader owns these bridge entries. The entry shape and qualified trailing
// bytes are stable D2R/loader contract evidence, but the RIP displacement is
// loader-layout metadata and may legitimately change when the loader reorders
// its qword bridge table. Decode that displacement from the live image instead
// of fingerprinting its exact value.
template <std::size_t N>
[[nodiscard]] inline bool ResolveRipIndirectBridge(
    const D2RL::PluginContext* context,
    std::uintptr_t entryRva,
    const std::array<std::uint8_t, N>& qualifiedTemplate,
    std::uintptr_t qualifiedSlotRva,
    RipIndirectBridgeResolution* output,
    std::uintptr_t maximumSlotDrift = 0x10000u) noexcept {
    static_assert(N >= 6);

    if (output != nullptr) *output = {};
    if (context == nullptr || context->exeBase == 0 || output == nullptr
        || qualifiedTemplate[0] != 0xFFu || qualifiedTemplate[1] != 0x25u) {
        return false;
    }

    const auto entryAddress = context->exeBase + entryRva;
    if (!Detail::IsReadableRange(reinterpret_cast<const void*>(entryAddress), N)) {
        return false;
    }

    std::array<std::uint8_t, N> live{};
    std::memcpy(live.data(), reinterpret_cast<const void*>(entryAddress), N);
    if (live[0] != 0xFFu || live[1] != 0x25u) return false;

    // Bytes [2,6) are the relocatable rel32. Everything after the instruction
    // remains part of the qualified witness and must still match exactly.
    for (std::size_t i = 6; i < N; ++i) {
        if (live[i] != qualifiedTemplate[i]) return false;
    }

    std::int32_t displacement{};
    std::memcpy(&displacement, live.data() + 2, sizeof(displacement));
    const auto signedSlot = static_cast<std::int64_t>(entryRva)
        + 6LL
        + static_cast<std::int64_t>(displacement);
    if (signedSlot < 0
        || static_cast<std::uint64_t>(signedSlot)
            > static_cast<std::uint64_t>(std::numeric_limits<std::uintptr_t>::max())) {
        return false;
    }

    const auto slotRva = static_cast<std::uintptr_t>(signedSlot);
    if ((slotRva & (alignof(std::uintptr_t) - 1u)) != 0u
        || !Detail::IsWithinRvaDrift(slotRva, qualifiedSlotRva, maximumSlotDrift)) {
        return false;
    }

    const auto slotAddress = context->exeBase + slotRva;
    if (!Detail::IsReadableRange(
            reinterpret_cast<const void*>(slotAddress),
            sizeof(std::uintptr_t))) {
        return false;
    }

    std::uintptr_t targetAddress{};
    std::memcpy(
        &targetAddress,
        reinterpret_cast<const void*>(slotAddress),
        sizeof(targetAddress));
    if (!Detail::IsExecutableAddress(targetAddress)) return false;

    output->entryRva = entryRva;
    output->slotRva = slotRva;
    output->targetAddress = targetAddress;
    output->slotDelta = slotRva >= qualifiedSlotRva
        ? static_cast<std::intptr_t>(slotRva - qualifiedSlotRva)
        : -static_cast<std::intptr_t>(qualifiedSlotRva - slotRva);
    return true;
}

struct Rel32CallResolution final {
    std::uintptr_t callRva{};
    std::uintptr_t targetRva{};
    std::intptr_t targetDelta{};
};

// Used for loader-generated relays whose own RVA can move. Anchor discovery at
// a qualified native D2R callsite, decode the live E8 rel32 target, and accept
// only a bounded movement from the previously qualified relay address.
[[nodiscard]] inline bool ResolveRel32CallTarget(
    const D2RL::PluginContext* context,
    std::uintptr_t callRva,
    std::uintptr_t qualifiedTargetRva,
    Rel32CallResolution* output,
    std::uintptr_t maximumTargetDrift = 0x10000u) noexcept {
    if (output != nullptr) *output = {};
    if (context == nullptr || context->exeBase == 0 || output == nullptr) return false;

    const auto callAddress = context->exeBase + callRva;
    if (!Detail::IsReadableRange(reinterpret_cast<const void*>(callAddress), 5u)) return false;

    std::array<std::uint8_t, 5> live{};
    std::memcpy(live.data(), reinterpret_cast<const void*>(callAddress), live.size());
    if (live[0] != 0xE8u) return false;

    std::int32_t displacement{};
    std::memcpy(&displacement, live.data() + 1, sizeof(displacement));
    const auto signedTarget = static_cast<std::int64_t>(callRva)
        + 5LL
        + static_cast<std::int64_t>(displacement);
    if (signedTarget < 0
        || static_cast<std::uint64_t>(signedTarget)
            > static_cast<std::uint64_t>(std::numeric_limits<std::uintptr_t>::max())) {
        return false;
    }

    const auto targetRva = static_cast<std::uintptr_t>(signedTarget);
    if (!Detail::IsWithinRvaDrift(targetRva, qualifiedTargetRva, maximumTargetDrift)
        || !Detail::IsExecutableAddress(context->exeBase + targetRva)) {
        return false;
    }

    output->callRva = callRva;
    output->targetRva = targetRva;
    output->targetDelta = targetRva >= qualifiedTargetRva
        ? static_cast<std::intptr_t>(targetRva - qualifiedTargetRva)
        : -static_cast<std::intptr_t>(qualifiedTargetRva - targetRva);
    return true;
}

} // namespace BuffPanel::Core
