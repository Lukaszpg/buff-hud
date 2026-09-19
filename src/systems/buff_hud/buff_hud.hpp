#pragma once

#include <D2RLPlugin/api.h>

namespace BuffPanel::Systems::BuffHud {

[[nodiscard]] bool Initialize(const D2RL::PluginContext* context) noexcept;
void Shutdown() noexcept;

} // namespace BuffPanel::Systems::BuffHud
