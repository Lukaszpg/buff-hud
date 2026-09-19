#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <mutex>
#include <string_view>

namespace BuffPanel::Core {

inline constexpr std::size_t MaximumPublishedBuffs = 64;
inline constexpr std::size_t MaximumDisplayedBuffs = 21;
inline constexpr std::int32_t NoSourceSkillId = -1;
inline constexpr std::uint16_t AutoIconFrame = 0xFFFFu;


enum class BuffDisplayMode : std::uint8_t {
    Timer = 0,
    Resource = 1,
};

enum class BuffIconAtlas : std::uint8_t {
    Auto = 0,
    Amazon,
    Sorceress,
    Necromancer,
    Paladin,
    Barbarian,
    Druid,
    Assassin,
    Warlock,
    Global,
};

struct BuffDisplayEntry final {
    std::uint64_t key{};
    std::int32_t sourceSkillId{NoSourceSkillId};
    BuffIconAtlas iconAtlas{BuffIconAtlas::Auto};
    std::uint16_t iconFrame{AutoIconFrame};
    BuffDisplayMode displayMode{BuffDisplayMode::Timer};
    std::uint32_t expireGameFrame{};
    std::int32_t valueStatId{};
    std::int32_t maxStatId{};
    std::int32_t currentValue{};
    std::int32_t maximumValue{};
    std::uint16_t stacks{1};
    std::int16_t priority{};
    std::uint64_t sequence{};
};

struct BuffDisplaySnapshot final {
    std::array<BuffDisplayEntry, MaximumPublishedBuffs> entries{};
    std::size_t count{};
    std::uint64_t revision{};
    std::uint64_t sessionGeneration{};
    std::uint32_t currentGameFrame{};
    bool hasGameFrame{};
};

class BuffDisplayBus final {
public:
    BuffDisplayBus() = default;
    BuffDisplayBus(const BuffDisplayBus&) = delete;
    BuffDisplayBus& operator=(const BuffDisplayBus&) = delete;

    // key must be stable and non-zero. Upserting an existing key preserves its
    // sequence so refreshes do not reorder an already-visible buff.
    [[nodiscard]] bool Upsert(BuffDisplayEntry entry) noexcept;
    [[nodiscard]] bool Remove(std::uint64_t key) noexcept;
    void Clear() noexcept;

    // Gameplay systems publish D2's authoritative 25-Hz frame counter here.
    // BuffHud never invents a gameplay clock. sessionGeneration=0 is rejected.
    void PublishGameFrame(std::uint64_t sessionGeneration, std::uint32_t frame) noexcept;
    void BeginSession(std::uint64_t sessionGeneration) noexcept;
    void EndSession(std::uint64_t sessionGeneration = 0) noexcept;

    [[nodiscard]] BuffDisplaySnapshot Snapshot() const noexcept;
    [[nodiscard]] std::size_t Count() const noexcept;

private:
    mutable std::mutex mutex_;
    std::array<BuffDisplayEntry, MaximumPublishedBuffs> entries_{};
    std::size_t count_{};
    std::uint64_t revision_{};
    std::uint64_t nextSequence_{1};
    std::uint64_t sessionGeneration_{};
    std::uint32_t currentGameFrame_{};
    bool hasGameFrame_{};
};

[[nodiscard]] BuffDisplayBus& BuffDisplays() noexcept;

// Stable compile-time/runtime FNV-1a helper for producers that want readable
// static keys without allocating strings on the gameplay path.
[[nodiscard]] constexpr std::uint64_t MakeBuffKey(std::string_view text) noexcept {
    std::uint64_t hash = 14695981039346656037ULL;
    for (const unsigned char ch : text) {
        hash ^= ch;
        hash *= 1099511628211ULL;
    }
    return hash == 0 ? 1 : hash;
}

} // namespace BuffPanel::Core
