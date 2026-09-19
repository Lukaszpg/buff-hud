#include "stat_list_post_bus.hpp"

#include <cstdio>

namespace BuffPanel::Core {
namespace {
StatListPostBus Bus{};
}

bool StatListPostBus::Register(const StatListPostObserver& observer) noexcept {
    const auto* context = Services().context;
    if (!installed_ || original_ == nullptr || observer.owner.empty() || observer.callback == nullptr) {
        return false;
    }
    if (count_ >= observers_.size()) {
        if (context != nullptr) context->LogError("BuffPanel Core: StatList-post observer registry capacity exhausted.");
        return false;
    }
    for (std::size_t i = 0; i < count_; ++i) {
        if (observers_[i].owner == observer.owner) {
            if (context != nullptr) {
                char message[256]{};
                std::snprintf(
                    message,
                    sizeof(message),
                    "BuffPanel Core: StatList-post observer owner '%.*s' registered twice.",
                    static_cast<int>(observer.owner.size()), observer.owner.data());
                context->LogError(message);
            }
            return false;
        }
    }

    std::size_t insertAt = count_;
    while (insertAt > 0 && observers_[insertAt - 1].priority > observer.priority) {
        observers_[insertAt] = observers_[insertAt - 1];
        --insertAt;
    }
    observers_[insertAt] = observer;
    ++count_;
    return true;
}

void __fastcall StatListPostBus::DispatchHook(
    void* unit,
    void* statList,
    std::int32_t update) noexcept {
    StatListPosts().Dispatch(unit, statList, update);
}

void StatListPostBus::Dispatch(
    void* unit,
    void* statList,
    std::int32_t update) noexcept {
    if (original_ == nullptr) return;

    const StatListPostEvent event{
        .unit = unit,
        .statList = statList,
        .update = update,
    };

    for (std::size_t i = 0; i < count_; ++i) {
        observers_[i].callback(event, StatListPostPhase::BeforeNative, observers_[i].userData);
    }

    original_(unit, statList, update);

    for (std::size_t i = 0; i < count_; ++i) {
        observers_[i].callback(event, StatListPostPhase::AfterNative, observers_[i].userData);
    }
}

void StatListPostBus::Reset() noexcept {
    observers_ = {};
    count_ = 0;
    original_ = nullptr;
    installed_ = false;
}

StatListPostBus& StatListPosts() noexcept {
    return Bus;
}

} // namespace BuffPanel::Core
