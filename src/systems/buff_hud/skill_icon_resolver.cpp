#include "skill_icon_resolver.hpp"

#include "core/services.hpp"

#include <D2RLPlugin/api.h>
#include <D2RLPlugin/localization.h>

#include <algorithm>
#include <array>
#include <cstring>
#include <cctype>
#include <mutex>
#include <string>
#include <vector>

namespace BuffPanel::Systems::BuffHud::Internal {
namespace {

// These are deliberately stock *witnesses*, not a production icon/name
// database. Their only job is to identify the active compiled-row layout at
// runtime. Amazon skills 6..11 are Magic Arrow, Fire Arrow, Inner Sight,
// Critical Strike, Jab and Cold Arrow and use IconCel 0,2,4,6,8,10 in build
// 93847's stock data. Once the layout is qualified, every actual icon and
// localized name is read from the active compiled tables, including mod-added
// skills.
constexpr std::array<std::uint32_t, 6> LinkWitnessSkillIds{6, 7, 8, 9, 10, 11};
constexpr std::array<std::uint8_t, 6> LinkWitnessIconCels{0, 2, 4, 6, 8, 10};

// One stock skill from the start of each original class block. Their compiled
// charclass values are the native class ids 0..6. Warlock then naturally uses
// 7 on RotW data; blank/non-class skills resolve to Global.
constexpr std::array<std::uint32_t, 7> ClassWitnessSkillIds{6, 36, 66, 96, 126, 221, 251};
constexpr std::array<std::uint8_t, 7> ClassWitnessValues{0, 1, 2, 3, 4, 5, 6};
constexpr std::uint32_t MaximumIdentityScanBytes = 96;
constexpr std::uint32_t MaximumClassScanBytes = 128;
constexpr std::uint32_t MaximumNameSearchDistance = 12;
constexpr std::size_t MaximumCachedSkillNameBytes = 256;

struct Layout final {
    std::uint32_t skillDescLinkOffset{};
    std::uint32_t skillClassOffset{};
    std::uint32_t iconCelOffset{};
    std::uint32_t skillNameStringIdOffset{};
};

struct State final {
    std::vector<SkillIconDescriptor> descriptors;
    std::vector<std::uint8_t> valid;
    std::vector<std::string> names;
    SkillIconResolverStatus status{};
};

std::mutex StateMutex;
State Cache{};

[[nodiscard]] bool ReadU8(
    const void* row,
    std::uint32_t rowSize,
    std::uint32_t offset,
    std::uint8_t& value) noexcept {
    if (row == nullptr || offset >= rowSize) return false;
    std::memcpy(&value, static_cast<const std::uint8_t*>(row) + offset, sizeof(value));
    return true;
}

[[nodiscard]] bool ReadU16(
    const void* row,
    std::uint32_t rowSize,
    std::uint32_t offset,
    std::uint16_t& value) noexcept {
    if (row == nullptr || offset > rowSize || sizeof(value) > rowSize - offset) return false;
    std::memcpy(&value, static_cast<const std::uint8_t*>(row) + offset, sizeof(value));
    return true;
}

[[nodiscard]] const void* RowAt(
    const D2RL::DataTables::TableView& table,
    std::uint32_t rowIndex) noexcept {
    if (table.rows == nullptr || rowIndex >= table.rowCount || table.rowSize == 0) return nullptr;
    return static_cast<const std::uint8_t*>(table.rows)
        + static_cast<std::size_t>(rowIndex) * table.rowSize;
}

[[nodiscard]] bool GetSkillRow(
    const D2RL::DataTableService* tables,
    const D2RL::PluginContext* context,
    D2RL::DataTables::Bank bank,
    std::uint32_t skillId,
    D2RL::DataTables::RowView& row) noexcept {
    row = {};
    row.structSize = D2RL::DataTables::RowViewSize;
    return tables != nullptr
        && tables->findRowById != nullptr
        && tables->findRowById(
               context,
               bank,
               D2RL::DataTables::TableId::Skills,
               skillId,
               &row) == D2RL::DataTables::Result::Success
        && row.row != nullptr
        && row.rowSize != 0;
}

[[nodiscard]] Core::BuffIconAtlas AtlasFromClass(std::uint8_t classId) noexcept {
    switch (classId) {
    case 0: return Core::BuffIconAtlas::Amazon;
    case 1: return Core::BuffIconAtlas::Sorceress;
    case 2: return Core::BuffIconAtlas::Necromancer;
    case 3: return Core::BuffIconAtlas::Paladin;
    case 4: return Core::BuffIconAtlas::Barbarian;
    case 5: return Core::BuffIconAtlas::Druid;
    case 6: return Core::BuffIconAtlas::Assassin;
    case 7: return Core::BuffIconAtlas::Warlock;
    default: return Core::BuffIconAtlas::Global;
    }
}

[[nodiscard]] bool ResolveLocalizedString(
    const D2RL::LocalizationService* localization,
    const D2RL::PluginContext* context,
    std::uint16_t stringId,
    std::string& output) {
    output.clear();
    if (localization == nullptr || context == nullptr || localization->getStringById == nullptr
        || stringId == 0) {
        return false;
    }

    std::array<char, MaximumCachedSkillNameBytes> buffer{};
    std::uint32_t required{};
    if (localization->getStringById(
            context,
            stringId,
            buffer.data(),
            static_cast<std::uint32_t>(buffer.size()),
            &required) != D2RL::Localization::Result::Success
        || buffer[0] == '\0') {
        return false;
    }
    output.assign(buffer.data());
    return !output.empty();
}

[[nodiscard]] bool IsMissingLocalizationSentinel(std::string_view value) noexcept {
    constexpr std::string_view Missing = "missing string";
    if (value.size() != Missing.size()) return false;
    for (std::size_t i = 0; i < value.size(); ++i) {
        if (static_cast<char>(std::tolower(static_cast<unsigned char>(value[i]))) != Missing[i]) return false;
    }
    return true;
}

[[nodiscard]] bool ResolvePlausibleLocalizedString(
    const D2RL::LocalizationService* localization,
    const D2RL::PluginContext* context,
    std::uint16_t stringId,
    std::string& resolved) {
    return ResolveLocalizedString(localization, context, stringId, resolved)
        && !IsMissingLocalizationSentinel(resolved)
        && resolved.find('\n') == std::string::npos
        && resolved.find('\r') == std::string::npos;
}

struct LinkCandidate final {
    std::uint32_t linkOffset{};
    std::uint32_t iconOffset{};
};

[[nodiscard]] std::vector<LinkCandidate> FindLinkCandidates(
    const D2RL::DataTableService* tables,
    const D2RL::PluginContext* context,
    D2RL::DataTables::Bank bank,
    const D2RL::DataTables::TableView& skillDesc) {
    std::array<D2RL::DataTables::RowView, LinkWitnessSkillIds.size()> skills{};
    for (std::size_t i = 0; i < skills.size(); ++i) {
        if (!GetSkillRow(tables, context, bank, LinkWitnessSkillIds[i], skills[i])) return {};
    }

    std::vector<LinkCandidate> result;
    const auto skillScan = skills[0].rowSize;
    const auto descScan = std::min<std::uint32_t>(skillDesc.rowSize, MaximumIdentityScanBytes);

    // wSkillDesc has historically been a WORD. Restricting this to aligned
    // uint16 candidates both matches the engine's representation and avoids
    // accepting overlapping byte-pattern accidents as separate layouts.
    for (std::uint32_t linkOffset = 0; linkOffset + sizeof(std::uint16_t) <= skillScan; linkOffset += 2) {
        std::array<std::uint16_t, LinkWitnessSkillIds.size()> links{};
        bool linksValid = true;
        for (std::size_t i = 0; i < skills.size(); ++i) {
            if (!ReadU16(skills[i].row, skills[i].rowSize, linkOffset, links[i])
                || links[i] >= skillDesc.rowCount) {
                linksValid = false;
                break;
            }
        }
        if (!linksValid) continue;

        // Six adjacent Amazon skills must resolve to six distinct descriptors.
        auto sorted = links;
        std::sort(sorted.begin(), sorted.end());
        if (std::adjacent_find(sorted.begin(), sorted.end()) != sorted.end()) continue;

        for (std::uint32_t iconOffset = 0; iconOffset < descScan; ++iconOffset) {
            bool match = true;
            for (std::size_t i = 0; i < links.size(); ++i) {
                const void* descRow = RowAt(skillDesc, links[i]);
                std::uint8_t icon{};
                if (!ReadU8(descRow, skillDesc.rowSize, iconOffset, icon)
                    || icon != LinkWitnessIconCels[i]) {
                    match = false;
                    break;
                }
            }
            if (match) result.push_back({linkOffset, iconOffset});
        }
    }
    return result;
}

[[nodiscard]] std::vector<std::uint32_t> FindClassCandidates(
    const D2RL::DataTableService* tables,
    const D2RL::PluginContext* context,
    D2RL::DataTables::Bank bank) {
    std::array<D2RL::DataTables::RowView, ClassWitnessSkillIds.size()> skills{};
    for (std::size_t i = 0; i < skills.size(); ++i) {
        if (!GetSkillRow(tables, context, bank, ClassWitnessSkillIds[i], skills[i])) return {};
    }

    std::vector<std::uint32_t> result;
    const auto scan = std::min<std::uint32_t>(skills[0].rowSize, MaximumClassScanBytes);
    for (std::uint32_t offset = 0; offset < scan; ++offset) {
        bool match = true;
        for (std::size_t i = 0; i < skills.size(); ++i) {
            std::uint8_t value{};
            if (!ReadU8(skills[i].row, skills[i].rowSize, offset, value)
                || value != ClassWitnessValues[i]) {
                match = false;
                break;
            }
        }
        if (match) result.push_back(offset);
    }
    return result;
}

[[nodiscard]] std::vector<std::uint32_t> FindNameCandidates(
    const D2RL::DataTableService* tables,
    const D2RL::LocalizationService* localization,
    const D2RL::PluginContext* context,
    D2RL::DataTables::Bank bank,
    const D2RL::DataTables::TableView& skillDesc,
    const LinkCandidate& link) {
    if (localization == nullptr || localization->getStringById == nullptr) return {};

    std::array<D2RL::DataTables::RowView, LinkWitnessSkillIds.size()> skills{};
    std::array<std::uint16_t, LinkWitnessSkillIds.size()> links{};
    for (std::size_t i = 0; i < skills.size(); ++i) {
        if (!GetSkillRow(tables, context, bank, LinkWitnessSkillIds[i], skills[i])
            || !ReadU16(skills[i].row, skills[i].rowSize, link.linkOffset, links[i])
            || links[i] >= skillDesc.rowCount) {
            return {};
        }
    }

    // `str name` is the first WORD-aligned field after the icon byte(s). This
    // remains layout-derived rather than an absolute hardcode: classic rows put
    // IconCel at +0x07 and D2R build 93847 puts IconCel at +0x06 followed by the
    // HireableIconCel byte at +0x07; both layouts therefore resolve the first
    // WORD after the icon region to +0x08. The previous scanner additionally
    // required str-short and str-long to be populated for every witness skill,
    // which is not true for the active D2R table and left names unqualified.
    const std::uint32_t candidate = (link.iconOffset + 2U) & ~1U;
    if (candidate + sizeof(std::uint16_t) > skillDesc.rowSize) return {};

    std::array<std::string, LinkWitnessSkillIds.size()> witnessNames{};
    for (std::size_t i = 0; i < links.size(); ++i) {
        const void* descRow = RowAt(skillDesc, links[i]);
        std::uint16_t stringId{};
        if (!ReadU16(descRow, skillDesc.rowSize, candidate, stringId)
            || !ResolvePlausibleLocalizedString(
                localization,
                context,
                stringId,
                witnessNames[i])) {
            return {};
        }
    }

    auto sortedNames = witnessNames;
    std::sort(sortedNames.begin(), sortedNames.end());
    if (std::adjacent_find(sortedNames.begin(), sortedNames.end()) != sortedNames.end()) {
        return {};
    }
    return {candidate};
}

[[nodiscard]] bool BuildForBank(
    D2RL::DataTables::Bank bank,
    std::uint64_t revision,
    State& output) {
    const auto& services = Core::Services();
    const auto* tables = Core::DataTableService();
    const auto* context = services.context;
    const auto* localization = Core::LocalizationService();
    if (tables == nullptr || context == nullptr || tables->getTable == nullptr) return false;

    // Localization is deliberately optional. Core::LocalizationService() retries
    // the ABI-4 service query here, at data-table cache rebuild time, and caches
    // it if D2RLoader has activated the service since initial plugin discovery.
    // Failure only disables localized names; icon qualification remains valid.

    D2RL::DataTables::TableView skills{};
    skills.structSize = D2RL::DataTables::TableViewSize;
    D2RL::DataTables::TableView skillDesc{};
    skillDesc.structSize = D2RL::DataTables::TableViewSize;
    if (tables->getTable(context, bank, D2RL::DataTables::TableId::Skills, &skills)
            != D2RL::DataTables::Result::Success
        || tables->getTable(context, bank, D2RL::DataTables::TableId::SkillDesc, &skillDesc)
            != D2RL::DataTables::Result::Success
        || skills.rows == nullptr || skills.rowCount == 0 || skills.rowSize == 0
        || skillDesc.rows == nullptr || skillDesc.rowCount == 0 || skillDesc.rowSize == 0) {
        return false;
    }

    const auto links = FindLinkCandidates(tables, context, bank, skillDesc);
    const auto classes = FindClassCandidates(tables, context, bank);

    output.status = {};
    output.status.bank = static_cast<std::uint32_t>(bank);
    output.status.skillsRowSize = skills.rowSize;
    output.status.skillDescRowSize = skillDesc.rowSize;
    output.status.skillCount = skills.rowCount;
    output.status.skillDescCount = skillDesc.rowCount;
    output.status.linkCandidateCount = static_cast<std::uint32_t>(links.size());
    output.status.classCandidateCount = static_cast<std::uint32_t>(classes.size());
    output.status.tableRevision = revision;

    if (links.size() != 1 || classes.size() != 1) return false;

    const auto nameCandidates = FindNameCandidates(
        tables,
        localization,
        context,
        bank,
        skillDesc,
        links[0]);
    output.status.nameCandidateCount = static_cast<std::uint32_t>(nameCandidates.size());

    const Layout layout{
        .skillDescLinkOffset = links[0].linkOffset,
        .skillClassOffset = classes[0],
        .iconCelOffset = links[0].iconOffset,
        .skillNameStringIdOffset = nameCandidates.empty() ? 0U : nameCandidates.front(),
    };
    output.status.skillDescLinkOffset = layout.skillDescLinkOffset;
    output.status.skillClassOffset = layout.skillClassOffset;
    output.status.iconCelOffset = layout.iconCelOffset;
    output.status.skillNameStringIdOffset = layout.skillNameStringIdOffset;
    output.status.namesReady = !nameCandidates.empty();

    try {
        output.descriptors.assign(skills.rowCount, {});
        output.valid.assign(skills.rowCount, 0);
        output.names.assign(skills.rowCount, {});
    } catch (...) {
        output.descriptors.clear();
        output.valid.clear();
        output.names.clear();
        return false;
    }

    for (std::uint32_t skillId = 0; skillId < skills.rowCount; ++skillId) {
        D2RL::DataTables::RowView skill{};
        if (!GetSkillRow(tables, context, bank, skillId, skill)) continue;

        std::uint16_t descIndex{};
        std::uint8_t classId{0xFF};
        if (!ReadU16(skill.row, skill.rowSize, layout.skillDescLinkOffset, descIndex)
            || descIndex >= skillDesc.rowCount
            || !ReadU8(skill.row, skill.rowSize, layout.skillClassOffset, classId)) {
            continue;
        }

        const void* descRow = RowAt(skillDesc, descIndex);
        std::uint8_t icon{};
        if (!ReadU8(descRow, skillDesc.rowSize, layout.iconCelOffset, icon)) continue;

        output.descriptors[skillId] = {
            .atlas = AtlasFromClass(classId),
            .frame = icon,
        };
        output.valid[skillId] = 1;

        if (output.status.namesReady) {
            std::uint16_t stringId{};
            if (ReadU16(
                    descRow,
                    skillDesc.rowSize,
                    layout.skillNameStringIdOffset,
                    stringId)) {
                std::string resolved;
                if (ResolveLocalizedString(localization, context, stringId, resolved)) {
                    output.names[skillId] = std::move(resolved);
                }
            }
        }
    }

    output.status.ready = true;
    return true;
}

} // namespace

bool RebuildSkillIconCache(std::uint64_t tableRevision) noexcept {
    const auto& services = Core::Services();
    if (Core::DataTableService() == nullptr || services.context == nullptr) return false;

    State next{};
    try {
        // Prefer the active RotW bank, then retain compatibility with LoD/classic
        // data sets used by development environments.
        for (const auto bank : {
                 D2RL::DataTables::Bank::Rotw,
                 D2RL::DataTables::Bank::Lod,
                 D2RL::DataTables::Bank::Classic}) {
            if (BuildForBank(bank, tableRevision, next)) {
                std::lock_guard lock(StateMutex);
                Cache = std::move(next);
                return true;
            }
            // Preserve the most useful failure diagnostics from the first bank that
            // exposed real tables, even if its runtime layout did not qualify.
            if (next.status.skillCount != 0) break;
        }
    } catch (...) {
        next.descriptors.clear();
        next.valid.clear();
        next.names.clear();
        next.status.ready = false;
        next.status.namesReady = false;
    }

    std::lock_guard lock(StateMutex);
    Cache = std::move(next);
    return false;
}

void ResetSkillIconCache() noexcept {
    std::lock_guard lock(StateMutex);
    Cache = {};
}

bool TryResolveSkillIcon(
    std::int32_t skillId,
    SkillIconDescriptor& descriptor) noexcept {
    descriptor = {};
    if (skillId < 0) return false;
    std::lock_guard lock(StateMutex);
    const auto index = static_cast<std::size_t>(skillId);
    if (!Cache.status.ready || index >= Cache.valid.size() || Cache.valid[index] == 0) {
        return false;
    }
    descriptor = Cache.descriptors[index];
    return true;
}

bool TryResolveSkillName(
    std::int32_t skillId,
    char* output,
    std::size_t outputSize) noexcept {
    if (output == nullptr || outputSize == 0) return false;
    output[0] = '\0';
    if (skillId < 0) return false;

    std::lock_guard lock(StateMutex);
    const auto index = static_cast<std::size_t>(skillId);
    if (!Cache.status.ready || !Cache.status.namesReady
        || index >= Cache.names.size() || Cache.names[index].empty()) {
        return false;
    }
    const auto& name = Cache.names[index];
    if (name.size() + 1 > outputSize) return false;
    std::memcpy(output, name.c_str(), name.size() + 1);
    return true;
}

SkillIconResolverStatus SkillIconStatus() noexcept {
    std::lock_guard lock(StateMutex);
    return Cache.status;
}

} // namespace BuffPanel::Systems::BuffHud::Internal
