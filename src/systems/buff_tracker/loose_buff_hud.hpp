#pragma once

#include <filesystem>
#include <string>
#include <string_view>

namespace BuffPanel::Systems::BuffTracker::LooseBuffHud {

inline constexpr std::string_view VirtualPath =
    "data/global/excel/d2rloader/buff-panel/buff-hud.txt";
inline constexpr std::size_t MaximumBytes = 1024u * 1024u;

enum class Source { Embedded, ActiveMod, InvalidOverride };

struct Selection final {
    Source source{Source::Embedded};
    std::filesystem::path path{};
    std::string bytes{};
    std::string error{};
};

// Load at plugin initialization, BEFORE registering the custom table.
// The active mod must be an unpacked directory; .mpq archive contents cannot
// be read using std::ifstream. An existing invalid override is an error, not a
// signal to silently replace the user's choices with embedded defaults.
[[nodiscard]] Selection Select(
    const wchar_t* modDirectory,
    const char* activeMod,
    std::string_view embedded) noexcept;

[[nodiscard]] bool ValidateAndNormalize(std::string& bytes, std::string& error) noexcept;

} // namespace BuffPanel::Systems::BuffTracker::LooseBuffHud
