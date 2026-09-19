#include "buff_display_bus.hpp"

#include <algorithm>

namespace BuffPanel::Core {
namespace {
BuffDisplayBus Bus{};
}

bool BuffDisplayBus::Upsert(BuffDisplayEntry entry) noexcept {
    if (entry.key == 0) return false;
    if (entry.stacks == 0) entry.stacks = 1;

    std::lock_guard lock(mutex_);
    for (std::size_t i = 0; i < count_; ++i) {
        if (entries_[i].key != entry.key) continue;
        entry.sequence = entries_[i].sequence;
        entries_[i] = entry;
        ++revision_;
        return true;
    }

    if (count_ >= entries_.size()) return false;
    entry.sequence = nextSequence_++;
    if (nextSequence_ == 0) nextSequence_ = 1;
    entries_[count_++] = entry;
    ++revision_;
    return true;
}

bool BuffDisplayBus::Remove(std::uint64_t key) noexcept {
    if (key == 0) return false;
    std::lock_guard lock(mutex_);
    for (std::size_t i = 0; i < count_; ++i) {
        if (entries_[i].key != key) continue;
        if (i + 1 < count_) {
            std::move(entries_.begin() + static_cast<std::ptrdiff_t>(i + 1),
                      entries_.begin() + static_cast<std::ptrdiff_t>(count_),
                      entries_.begin() + static_cast<std::ptrdiff_t>(i));
        }
        entries_[--count_] = {};
        ++revision_;
        return true;
    }
    return false;
}

void BuffDisplayBus::Clear() noexcept {
    std::lock_guard lock(mutex_);
    entries_ = {};
    count_ = 0;
    ++revision_;
}

void BuffDisplayBus::PublishGameFrame(
    std::uint64_t sessionGeneration,
    std::uint32_t frame) noexcept {
    if (sessionGeneration == 0) return;
    std::lock_guard lock(mutex_);
    if (sessionGeneration_ != sessionGeneration) {
        entries_ = {};
        count_ = 0;
        sessionGeneration_ = sessionGeneration;
        ++revision_;
    }
    currentGameFrame_ = frame;
    hasGameFrame_ = true;
}

void BuffDisplayBus::BeginSession(std::uint64_t sessionGeneration) noexcept {
    std::lock_guard lock(mutex_);
    entries_ = {};
    count_ = 0;
    currentGameFrame_ = 0;
    hasGameFrame_ = false;
    sessionGeneration_ = sessionGeneration;
    ++revision_;
}

void BuffDisplayBus::EndSession(std::uint64_t sessionGeneration) noexcept {
    std::lock_guard lock(mutex_);
    if (sessionGeneration != 0 && sessionGeneration_ != sessionGeneration) return;
    entries_ = {};
    count_ = 0;
    currentGameFrame_ = 0;
    hasGameFrame_ = false;
    sessionGeneration_ = 0;
    ++revision_;
}

BuffDisplaySnapshot BuffDisplayBus::Snapshot() const noexcept {
    std::lock_guard lock(mutex_);
    BuffDisplaySnapshot snapshot{};
    snapshot.entries = entries_;
    snapshot.count = count_;
    snapshot.revision = revision_;
    snapshot.sessionGeneration = sessionGeneration_;
    snapshot.currentGameFrame = currentGameFrame_;
    snapshot.hasGameFrame = hasGameFrame_;
    return snapshot;
}

std::size_t BuffDisplayBus::Count() const noexcept {
    std::lock_guard lock(mutex_);
    return count_;
}

BuffDisplayBus& BuffDisplays() noexcept {
    return Bus;
}

} // namespace BuffPanel::Core
