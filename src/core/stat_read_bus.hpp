#pragma once

#include "hook_registry.hpp"

#include <array>
#include <cstdint>
#include <string_view>

namespace BuffPanel::Core {

using RawGetUnitStatFn = std::int32_t(__fastcall*)(
    void* unit,
    std::int32_t statId,
    std::uint16_t layer) noexcept;

using StatReadInterceptorFn = std::int32_t(*)(
    void* unit,
    std::int32_t statId,
    std::uint16_t layer,
    std::int32_t currentValue,
    std::uintptr_t callerRva,
    void* userData) noexcept;

struct StatReadInterceptor final {
    std::string_view owner; // static-lifetime owner name
    std::int32_t priority{};
    StatReadInterceptorFn callback{};
    void* userData{};
};

class StatReadBus final {
public:
    StatReadBus() = default;
    StatReadBus(const StatReadBus&) = delete;
    StatReadBus& operator=(const StatReadBus&) = delete;

    template <std::size_t N>
    [[nodiscard]] bool EnsureInstalled(
        std::uintptr_t rva,
        const std::array<std::uint8_t, N>& expected,
        std::uintptr_t expectedSlotRva) noexcept {
        if (installed_) return true;

        RawGetUnitStatFn original{};
        if (!Hooks().InstallRipIndirectChain(
                "core.unit-stat-read",
                rva,
                expected,
                expectedSlotRva,
                &DispatchHook,
                &original)) {
            return false;
        }
        original_ = original;
        installed_ = original_ != nullptr;
        return installed_;
    }

    // For a standalone display plugin we only need to READ stats. Resolve
    // D2RLoader's currently installed qualified bridge; never install an
    // unnecessary native getter hook or intercept another plugin's reads.
    template <std::size_t N>
    [[nodiscard]] bool ResolveRawGetter(
        std::uintptr_t rva,
        const std::array<std::uint8_t, N>& expected,
        std::uintptr_t qualifiedSlotRva) noexcept {
        if (original_ != nullptr) return true;
        RipIndirectBridgeResolution bridge{};
        if (!ResolveRipIndirectBridge(
                Services().context, rva, expected, qualifiedSlotRva, &bridge)) {
            return false;
        }
        original_ = reinterpret_cast<RawGetUnitStatFn>(bridge.targetAddress);
        return original_ != nullptr;
    }

    [[nodiscard]] bool Register(const StatReadInterceptor& interceptor) noexcept;
    [[nodiscard]] RawGetUnitStatFn RawGetter() const noexcept { return original_; }
    [[nodiscard]] std::int32_t ReadEffective(
        void* unit,
        std::int32_t statId,
        std::uint16_t layer) noexcept;
    [[nodiscard]] std::size_t InterceptorCount() const noexcept { return count_; }
    void Reset() noexcept;

private:
    static std::int32_t __fastcall DispatchHook(
        void* unit,
        std::int32_t statId,
        std::uint16_t layer) noexcept;

    [[nodiscard]] std::int32_t Dispatch(
        void* unit,
        std::int32_t statId,
        std::uint16_t layer,
        std::uintptr_t callerRva) noexcept;

    RawGetUnitStatFn original_{};
    std::array<StatReadInterceptor, 16> interceptors_{};
    std::size_t count_{};
    bool installed_{};
};

[[nodiscard]] StatReadBus& StatReads() noexcept;

} // namespace BuffPanel::Core
