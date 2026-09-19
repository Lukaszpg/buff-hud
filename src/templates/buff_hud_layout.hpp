#pragma once

namespace BuffPanel::Systems::BuffHud::Internal {

// Generated as adjacent raw-string literals. Keep each source literal below
// MSVC's pre-concatenation C2026 limit; the compiler joins them into one array.
inline constexpr char BuffHudLayout[] =
@BUFF_PANEL_LAYOUT_LITERALS@
;

} // namespace BuffPanel::Systems::BuffHud::Internal
