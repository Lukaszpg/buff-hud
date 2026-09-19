#pragma once

#include "hook_registry.hpp"

#include <array>
#include <cstdint>
#include <string_view>

namespace BuffPanel::Core {

using RawPostStatListFn = void(__fastcall*)(
    void* unit,
    void* statList,
    std::int32_t update) noexcept;

enum class StatListPostPhase : std::uint8_t {
    BeforeNative = 0,
    AfterNative = 1,
};

struct StatListPostEvent final {
    void* unit{};
    void* statList{};
    std::int32_t update{};
};

using StatListPostObserverFn = void(*)(
    const StatListPostEvent& event,
    StatListPostPhase phase,
    void* userData) noexcept;

struct StatListPostObserver final {
    std::string_view owner; // static-lifetime owner name
    std::int32_t priority{};
    StatListPostObserverFn callback{};
    void* userData{};
};

class StatListPostBus final {
public:
    StatListPostBus() = default;
    StatListPostBus(const StatListPostBus&) = delete;
    StatListPostBus& operator=(const StatListPostBus&) = delete;

    template <std::size_t N>
    [[nodiscard]] bool EnsureInstalled(
        std::uintptr_t rva,
        const std::array<std::uint8_t, N>& expected,
        std::uintptr_t expectedSlotRva) noexcept {
        if (installed_) return true;

        RawPostStatListFn original{};
        if (!Hooks().InstallRipIndirectChain(
                "core.stat-list-post",
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

    [[nodiscard]] bool Register(const StatListPostObserver& observer) noexcept;
    [[nodiscard]] RawPostStatListFn RawPoster() const noexcept { return original_; }
    [[nodiscard]] std::size_t ObserverCount() const noexcept { return count_; }
    void Reset() noexcept;

private:
    static void __fastcall DispatchHook(
        void* unit,
        void* statList,
        std::int32_t update) noexcept;

    void Dispatch(
        void* unit,
        void* statList,
        std::int32_t update) noexcept;

    RawPostStatListFn original_{};
    std::array<StatListPostObserver, 16> observers_{};
    std::size_t count_{};
    bool installed_{};
};

[[nodiscard]] StatListPostBus& StatListPosts() noexcept;

} // namespace BuffPanel::Core
