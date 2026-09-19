#pragma once

#include "services.hpp"
#include "rip_indirect_bridge.hpp"

#include <array>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <limits>
#include <mutex>
#include <string_view>
#include <type_traits>

namespace BuffPanel::Core {

struct HookRecord final {
    std::uintptr_t rva{};
    std::string_view owner{}; // Install callers must pass static-lifetime names.
};

struct HookSnapshot final {
    std::array<HookRecord, 64> records{};
    std::size_t count{};
};

class HookRegistry final {
public:
    HookRegistry() = default;
    HookRegistry(const HookRegistry&) = delete;
    HookRegistry& operator=(const HookRegistry&) = delete;

    template <typename Function, std::size_t N>
    [[nodiscard]] bool Install(
        std::string_view owner,
        std::uintptr_t rva,
        const std::array<std::uint8_t, N>& expected,
        Function target,
        Function* original = nullptr) noexcept {
        const auto* context = Services().context;
        if (context == nullptr || target == nullptr || owner.empty()) return false;

        // Plugin initialization is serialized by D2RLoader. The mutex also makes
        // console diagnostics safe if they are queried while a future system is
        // still being initialized.
        std::lock_guard<std::mutex> lock(mutex_);
        for (std::size_t i = 0; i < count_; ++i) {
            if (records_[i].rva == rva) {
                char message[256]{};
                std::snprintf(
                    message,
                    sizeof(message),
                    "BuffPanel Core: duplicate native-hook ownership at RVA 0x%llX requested by '%.*s'; already owned by '%.*s'.",
                    static_cast<unsigned long long>(rva),
                    static_cast<int>(owner.size()), owner.data(),
                    static_cast<int>(records_[i].owner.size()), records_[i].owner.data());
                context->LogError(message);
                return false;
            }
        }
        if (count_ >= records_.size()) {
            context->LogError("BuffPanel Core: native-hook registry capacity exhausted; hook refused.");
            return false;
        }

        Function trampoline{};
        Function* trampolineOut = original != nullptr ? original : &trampoline;
        if (!context->InstallInlineHook(
                rva,
                expected.data(),
                static_cast<std::uint32_t>(expected.size()),
                target,
                trampolineOut)) {
            return false;
        }

        records_[count_++] = HookRecord{rva, owner};
        return true;
    }



    // D2RLoader rewrites several public D2R entries into FF 25 rel32
    // bridges. The bridge-table slot is loader layout, not part of the D2R
    // function contract, so resolve it from the live instruction while keeping
    // the qualified opcode/trailing bytes and slot-region checks fail-closed.
    template <typename Function, std::size_t N>
    [[nodiscard]] bool InstallRipIndirectChain(
        std::string_view owner,
        std::uintptr_t rva,
        const std::array<std::uint8_t, N>& expected,
        std::uintptr_t expectedSlotRva,
        Function target,
        Function* previous) noexcept {
        static_assert(std::is_pointer_v<Function>);
        static_assert(std::is_function_v<std::remove_pointer_t<Function>>);

        const auto* context = Services().context;
        if (context == nullptr || target == nullptr || previous == nullptr || owner.empty()) {
            return false;
        }

        std::lock_guard<std::mutex> lock(mutex_);
        for (std::size_t i = 0; i < count_; ++i) {
            if (records_[i].rva == rva) {
                char message[256]{};
                std::snprintf(
                    message,
                    sizeof(message),
                    "BuffPanel Core: duplicate native-hook ownership at RVA 0x%llX requested by '%.*s'; already owned by '%.*s'.",
                    static_cast<unsigned long long>(rva),
                    static_cast<int>(owner.size()), owner.data(),
                    static_cast<int>(records_[i].owner.size()), records_[i].owner.data());
                context->LogError(message);
                return false;
            }
        }
        if (count_ >= records_.size()) {
            context->LogError("BuffPanel Core: native-hook registry capacity exhausted; hook refused.");
            return false;
        }

        RipIndirectBridgeResolution bridge{};
        if (!ResolveRipIndirectBridge(
                context,
                rva,
                expected,
                expectedSlotRva,
                &bridge)) {
            char message[320]{};
            std::snprintf(
                message,
                sizeof(message),
                "BuffPanel Core: '%.*s' expected a structurally qualified D2RLoader RIP-indirect bridge at RVA 0x%llX near slot RVA 0x%llX; chain refused.",
                static_cast<int>(owner.size()), owner.data(),
                static_cast<unsigned long long>(rva),
                static_cast<unsigned long long>(expectedSlotRva));
            context->LogError(message);
            return false;
        }

        const auto replacementAddress = reinterpret_cast<std::uintptr_t>(target);
        if (bridge.targetAddress == replacementAddress) {
            context->LogError("BuffPanel Core: D2RLoader bridge already points at the requested BuffPanel hook; chain refused.");
            return false;
        }

        // Patch only the qword consumed by the existing FF /4 RIP-indirect JMP.
        // The expected bytes are the live target pointer, so another owner
        // changing the slot between resolution and patch still fails closed.
        if (!context->PatchWriteU64(
                static_cast<std::uint64_t>(bridge.slotRva),
                &bridge.targetAddress,
                static_cast<std::uint32_t>(sizeof(bridge.targetAddress)),
                static_cast<std::uint64_t>(replacementAddress))) {
            char message[256]{};
            std::snprintf(
                message,
                sizeof(message),
                "BuffPanel Core: failed to chain '%.*s' through D2RLoader bridge slot RVA 0x%llX.",
                static_cast<int>(owner.size()), owner.data(),
                static_cast<unsigned long long>(bridge.slotRva));
            context->LogError(message);
            return false;
        }

        *previous = reinterpret_cast<Function>(bridge.targetAddress);
        records_[count_++] = HookRecord{rva, owner};

        char message[384]{};
        std::snprintf(
            message,
            sizeof(message),
            "BuffPanel Core: chained '%.*s' through D2RLoader RIP-indirect bridge entry RVA 0x%llX (slot RVA 0x%llX, qualified slot delta=%+lld).",
            static_cast<int>(owner.size()), owner.data(),
            static_cast<unsigned long long>(rva),
            static_cast<unsigned long long>(bridge.slotRva),
            static_cast<long long>(bridge.slotDelta));
        context->LogInfo(message);
        return true;
    }

    void Reset() noexcept;
    [[nodiscard]] HookSnapshot Snapshot() const noexcept;

private:
    mutable std::mutex mutex_;
    std::array<HookRecord, 64> records_{};
    std::size_t count_{};
};

[[nodiscard]] HookRegistry& Hooks() noexcept;

} // namespace BuffPanel::Core
