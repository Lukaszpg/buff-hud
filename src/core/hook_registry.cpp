#include "hook_registry.hpp"

namespace BuffPanel::Core {
namespace {
HookRegistry Registry{};
}

void HookRegistry::Reset() noexcept {
    std::lock_guard<std::mutex> lock(mutex_);
    records_ = {};
    count_ = 0;
}

HookSnapshot HookRegistry::Snapshot() const noexcept {
    std::lock_guard<std::mutex> lock(mutex_);
    HookSnapshot result{};
    result.count = count_;
    for (std::size_t i = 0; i < count_; ++i) result.records[i] = records_[i];
    return result;
}

HookRegistry& Hooks() noexcept {
    return Registry;
}

} // namespace BuffPanel::Core
