#pragma once

#include <cstdint>
#include <filesystem>
#include <string>
#include <string_view>

namespace BuffPanel::Systems::BuffHud::LooseLayout {

inline constexpr std::string_view VirtualPath =
    "data/global/ui/layouts/buff-panel/BuffHudhd.json";
inline constexpr std::size_t MaximumBytes = 2u * 1024u * 1024u;

enum class Source { Embedded, ActiveMod, InvalidOverride };

// D2RLoader reports widget rect coordinates relative to the widget's parent.
// Capture the matching BuffGrid rect from the JSON actually registered at load,
// not from a subsequent disk read (which may have changed without a restart).
struct GridRect final {
    std::int32_t x{};
    std::int32_t y{};
    std::int32_t width{};
    std::int32_t height{};
};

[[nodiscard]] bool ReadBuffGridRect(std::string_view bytes, GridRect& rect) noexcept;

struct Selection final {
    Source source{Source::Embedded};
    std::filesystem::path path{};
    std::string bytes{};
    std::string error{};
};

// Choose a physical, unpacked active-mod layout before ResourceService registers
// the panel. The selected JSON is copied by D2RLoader at registration time.
// An existing invalid override fails closed; it is never silently ignored.
[[nodiscard]] Selection Select(
    const wchar_t* modDirectory,
    const char* activeMod,
    std::string_view embedded) noexcept;

[[nodiscard]] bool Validate(std::string_view bytes, std::string& error) noexcept;

} // namespace BuffPanel::Systems::BuffHud::LooseLayout
