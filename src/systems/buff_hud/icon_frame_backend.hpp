#pragma once

#include <D2RLPlugin/api.h>

#include <cstdint>

namespace BuffPanel::Systems::BuffHud::Internal {

enum class IconFrameBackendState : std::uint8_t {
    Unavailable = 0,
    Ready = 1,
};

// Validates the exact D2R build-93847 ButtonWidget frame descriptors and live
// apply callsites qualified by Skill Icon HUD Probe 0.7.0. No hook is installed;
// production invokes the qualified setter/refresh path only from BuffHud's UI
// thread when assigning a slot.
[[nodiscard]] bool InitializeIconFrameBackend(
    const D2RL::PluginContext* context) noexcept;
void ShutdownIconFrameBackend() noexcept;
[[nodiscard]] IconFrameBackendState IconFrameBackendStatus() noexcept;

[[nodiscard]] bool TryApplyIconFrame(
    void* buttonWidget,
    std::uint16_t requestedFrame,
    std::uint16_t declaredFrame) noexcept;

} // namespace BuffPanel::Systems::BuffHud::Internal
