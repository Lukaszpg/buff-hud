#pragma once

#include <D2RLPlugin/api.h>

namespace BuffPanel::Core {

struct ServiceRegistry final {
    const D2RL::PluginContext* context{};
    const D2RL::ResourceService* resources{};
    const D2RL::PanelService* panels{};
    const D2RL::CustomTableService* customTables{};
    const D2RL::LifecycleService* lifecycle{};
    const D2RL::ThreadService* threads{};
    const D2RL::ItemService* items{};
    const D2RL::InventoryService* inventory{};
    const D2RL::ItemInteractionService* interactions{};
    const D2RL::SharedEventService* sharedEvents{};
    const D2RL::WidgetService* widgets{};
    const D2RL::DataTableService* dataTables{};
    const D2RL::DiagnosticsService* diagnostics{};
    const D2RL::LocalizationService* localization{};
};

[[nodiscard]] bool InitializeServices(const D2RL::PluginContext* context) noexcept;
void ShutdownServices() noexcept;
[[nodiscard]] const ServiceRegistry& Services() noexcept;

// Data-table/localization services may be unavailable during the earliest load
// phase. Re-query them at the point of use and cache them once active.
[[nodiscard]] const D2RL::LocalizationService* LocalizationService() noexcept;
[[nodiscard]] const D2RL::DataTableService* DataTableService() noexcept;

} // namespace BuffPanel::Core
