#include "services.hpp"

namespace BuffPanel::Core {
namespace {

ServiceRegistry Registry{};

template <typename Service>
[[nodiscard]] const Service* Query(const D2RL::PluginContext* context) noexcept {
    if (context == nullptr) return nullptr;
    const Service* service = nullptr;
    if (context->QueryService(&service) != D2RL::ServiceQueryResult::Success) {
        return nullptr;
    }
    return service;
}

} // namespace

bool InitializeServices(const D2RL::PluginContext* context) noexcept {
    Registry = {};
    // D2RLoader validates the manifest before invoking the plugin, but keep the
    // boundary defensive as well: do not read context fields unless the ABI-4
    // context prefix is present, and reject an unexpectedly older ABI.
    if (!D2RL::HasContext(context)
        || context->abiVersion < D2RL_PLUGIN_ABI_VERSION
        || context->exeBase == 0) {
        return false;
    }

    Registry.context = context;
    Registry.resources = Query<D2RL::ResourceService>(context);
    Registry.panels = Query<D2RL::PanelService>(context);
    Registry.customTables = Query<D2RL::CustomTableService>(context);
    Registry.lifecycle = Query<D2RL::LifecycleService>(context);
    Registry.threads = Query<D2RL::ThreadService>(context);
    Registry.items = Query<D2RL::ItemService>(context);
    Registry.inventory = Query<D2RL::InventoryService>(context);
    Registry.interactions = Query<D2RL::ItemInteractionService>(context);
    Registry.sharedEvents = Query<D2RL::SharedEventService>(context);
    Registry.widgets = Query<D2RL::WidgetService>(context);
    Registry.dataTables = Query<D2RL::DataTableService>(context);
    Registry.diagnostics = Query<D2RL::DiagnosticsService>(context);
    Registry.localization = Query<D2RL::LocalizationService>(context);

    return true;
}

void ShutdownServices() noexcept {
    Registry = {};
}

const ServiceRegistry& Services() noexcept {
    return Registry;
}

const D2RL::LocalizationService* LocalizationService() noexcept {
    if (Registry.localization != nullptr
        && D2RL::HasLocalizationServiceField(
            Registry.localization,
            D2RL::LocalizationServiceRequiredSize)) {
        return Registry.localization;
    }

    const auto* service = Query<D2RL::LocalizationService>(Registry.context);
    if (!D2RL::HasLocalizationServiceField(
            service,
            D2RL::LocalizationServiceRequiredSize)) {
        return nullptr;
    }

    Registry.localization = service;
    return Registry.localization;
}

const D2RL::DataTableService* DataTableService() noexcept {
    if (Registry.dataTables != nullptr
        && D2RL::HasDataTableServiceField(
            Registry.dataTables,
            D2RL::DataTableServiceRequiredSize)) {
        return Registry.dataTables;
    }

    const auto* service = Query<D2RL::DataTableService>(Registry.context);
    if (!D2RL::HasDataTableServiceField(
            service,
            D2RL::DataTableServiceRequiredSize)) {
        return nullptr;
    }

    Registry.dataTables = service;
    return Registry.dataTables;
}

} // namespace BuffPanel::Core
