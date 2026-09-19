#include "stat_read_bus.hpp"

#include <algorithm>
#include <cstdio>

#if defined(_MSC_VER)
#include <intrin.h>
#pragma intrinsic(_ReturnAddress)
#endif

namespace BuffPanel::Core {
namespace {
StatReadBus Bus{};
}

bool StatReadBus::Register(const StatReadInterceptor& interceptor) noexcept {
    const auto* context = Services().context;
    if (!installed_ || original_ == nullptr || interceptor.owner.empty() || interceptor.callback == nullptr) {
        return false;
    }
    if (count_ >= interceptors_.size()) {
        if (context != nullptr) context->LogError("BuffPanel Core: unit-stat interceptor registry capacity exhausted.");
        return false;
    }
    for (std::size_t i = 0; i < count_; ++i) {
        if (interceptors_[i].owner == interceptor.owner) {
            if (context != nullptr) {
                char message[256]{};
                std::snprintf(
                    message,
                    sizeof(message),
                    "BuffPanel Core: unit-stat interceptor owner '%.*s' registered twice.",
                    static_cast<int>(interceptor.owner.size()), interceptor.owner.data());
                context->LogError(message);
            }
            return false;
        }
    }

    std::size_t insertAt = count_;
    while (insertAt > 0 && interceptors_[insertAt - 1].priority > interceptor.priority) {
        interceptors_[insertAt] = interceptors_[insertAt - 1];
        --insertAt;
    }
    interceptors_[insertAt] = interceptor;
    ++count_;
    return true;
}

std::int32_t __fastcall StatReadBus::DispatchHook(
    void* unit,
    std::int32_t statId,
    std::uint16_t layer) noexcept {
    std::uintptr_t callerAddress{};
#if defined(_MSC_VER)
    callerAddress = reinterpret_cast<std::uintptr_t>(_ReturnAddress());
#elif defined(__GNUC__) || defined(__clang__)
    callerAddress = reinterpret_cast<std::uintptr_t>(__builtin_return_address(0));
#endif

    std::uintptr_t callerRva{};
    const auto* context = Services().context;
    if (context != nullptr && context->exeBase != 0 && callerAddress >= context->exeBase) {
        callerRva = callerAddress - context->exeBase;
    }
    return StatReads().Dispatch(unit, statId, layer, callerRva);
}

std::int32_t StatReadBus::ReadEffective(
    void* unit,
    std::int32_t statId,
    std::uint16_t layer) noexcept {
    return Dispatch(unit, statId, layer, 0);
}

std::int32_t StatReadBus::Dispatch(
    void* unit,
    std::int32_t statId,
    std::uint16_t layer,
    std::uintptr_t callerRva) noexcept {
    if (original_ == nullptr) return 0;

    std::int32_t value = original_(unit, statId, layer);
    for (std::size_t i = 0; i < count_; ++i) {
        value = interceptors_[i].callback(
            unit,
            statId,
            layer,
            value,
            callerRva,
            interceptors_[i].userData);
    }
    return value;
}

void StatReadBus::Reset() noexcept {
    interceptors_ = {};
    count_ = 0;
    original_ = nullptr;
    installed_ = false;
}

StatReadBus& StatReads() noexcept {
    return Bus;
}

} // namespace BuffPanel::Core
