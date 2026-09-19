#include "buff_tracker.hpp"

#include "core/buff_display_bus.hpp"
#include "core/services.hpp"
#include "core/stat_read_bus.hpp"
#include "core/stat_list_post_bus.hpp"
#include "native/native_contract.hpp"

#include "loose_buff_hud.hpp"

#include <Windows.h>

#include <algorithm>
#include <array>
#include <atomic>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <memory>
#include <mutex>
#include <string>
#include <string_view>
#include <vector>

// Compile-time defaults are kept here, not in generated headers or
// authoring TXT/JSON. Charsi may strip non-C++ asset files.
namespace BuffPanel::Systems::BuffTracker::DefaultTables {
constexpr char BuffHud[] = R"BUFFPANELHUD(name	state_id	display_type	value_stat	max_stat	skill_id	value_shift	enabled
resist_fire_aura	3	timer	0	0	0	0	0
resist_cold_aura	4	timer	0	0	0	0	0
resist_lightning_aura	5	timer	0	0	0	0	0
salvation_aura	8	timer	0	0	0	0	0
frozen_armor	10	timer	0	0	0	0	1
blaze	13	timer	0	0	0	0	1
bone_armor	14	resource	132	133	68	8	1
concentrate_attack_state	15	timer	0	0	0	0	0
enchant	16	timer	0	0	0	0	1
chilling_armor	20	timer	0	0	0	0	1
shout	26	timer	0	0	0	0	1
conviction_aura	28	timer	0	0	0	0	0
energy_shield	30	timer	0	0	0	0	1
venom	31	timer	0	0	0	0	1
battle_orders	32	timer	0	0	0	0	1
might_aura	33	timer	0	0	0	0	0
prayer_aura	34	timer	0	0	0	0	0
holy_fire_aura	35	timer	0	0	0	0	0
thorns_aura	36	timer	0	0	0	0	0
defiance_aura	37	timer	0	0	0	0	0
thunder_storm	38	timer	0	0	0	0	1
blessed_aim_aura	40	timer	0	0	0	0	0
vigor_aura	41	timer	0	0	0	0	0
concentration_aura	42	timer	0	0	0	0	0
holy_freeze_caster_aura	43	timer	0	0	0	0	0
cleansing_aura	45	timer	0	0	0	0	0
holy_shock_aura	46	timer	0	0	0	0	0
sanctuary_aura	47	timer	0	0	0	0	0
meditation_aura	48	timer	0	0	0	0	0
fanaticism_aura	49	timer	0	0	0	0	0
redemption_aura	50	timer	0	0	0	0	0
battle_command	51	timer	0	0	0	0	1
critical_strike_passive	64	timer	0	0	0	0	0
dodge_passive	65	timer	0	0	0	0	0
avoid_passive	66	timer	0	0	0	0	0
penetrate_passive	67	timer	0	0	0	0	0
evade_passive	68	timer	0	0	0	0	0
pierce_passive	69	timer	0	0	0	0	0
warmth_passive	70	timer	0	0	0	0	0
fire_mastery_passive	71	timer	0	0	0	0	0
lightning_mastery_passive	72	timer	0	0	0	0	0
cold_mastery_passive	73	timer	0	0	0	0	0
blade_mastery_passive	74	timer	0	0	0	0	0
axe_mastery_passive	75	timer	0	0	0	0	0
mace_mastery_passive	76	timer	0	0	0	0	0
polearm_mastery_passive	77	timer	0	0	0	0	0
throwing_mastery_passive	78	timer	0	0	0	0	0
spear_mastery_passive	79	timer	0	0	0	0	0
increased_stamina_passive	80	timer	0	0	0	0	0
iron_skin_passive	81	timer	0	0	0	0	0
increased_speed_passive	82	timer	0	0	0	0	0
natural_resistance_passive	83	timer	0	0	0	0	0
shiver_armor	88	timer	0	0	0	0	1
frenzy	94	timer	0	0	0	0	1
berserk_attack_state	95	timer	0	0	0	0	0
skeleton_mastery_passive	97	timer	0	0	0	0	0
holy_shield	101	timer	0	0	0	0	1
golem_mastery_passive	111	timer	0	0	0	0	0
maul	117	timer	0	0	0	0	1
feral_rage	120	timer	0	0	0	0	1
tiger_strike_charges	122	timer	0	0	0	0	1
cobra_strike_charges	123	timer	0	0	0	0	1
phoenix_strike_charges	124	timer	0	0	0	0	1
fists_of_fire_charges	125	timer	0	0	0	0	1
blades_of_ice_charges	126	timer	0	0	0	0	1
claws_of_thunder_charges	127	timer	0	0	0	0	1
armor_shrine	128	timer	0	0	0	0	1
combat_shrine	129	timer	0	0	0	0	1
lightning_resist_shrine	130	timer	0	0	0	0	1
fire_resist_shrine	131	timer	0	0	0	0	1
cold_resist_shrine	132	timer	0	0	0	0	1
poison_resist_shrine	133	timer	0	0	0	0	1
skill_shrine	134	timer	0	0	0	0	1
mana_regen_shrine	135	timer	0	0	0	0	1
stamina_shrine	136	timer	0	0	0	0	1
experience_shrine	137	timer	0	0	0	0	1
werewolf	139	timer	0	0	0	0	1
werebear	140	timer	0	0	0	0	1
bloodlust	141	timer	0	0	0	0	0
hurricane	144	timer	0	0	0	0	1
armageddon	145	timer	0	0	0	0	1
spirit_of_barbs_aura	147	timer	0	0	0	0	0
heart_of_wolverine_aura	148	timer	0	0	0	0	0
oak_sage_aura	149	timer	0	0	0	0	0
cyclone_armor	151	resource	132	133	235	8	1
claw_mastery_passive	152	timer	0	0	0	0	0
cloak_of_shadows_caster	153	timer	0	0	0	0	1
weapon_block_passive	155	timer	0	0	0	0	0
burst_of_speed	157	timer	0	0	0	0	1
blade_shield	158	timer	0	0	0	0	1
fade	159	timer	0	0	0	0	1
summon_resist_passive	160	timer	0	0	0	0	0
whirlwind_action_state	174	timer	0	0	0	0	0
delirium	177	timer	0	0	0	0	1
antidote_potion	178	timer	0	0	0	0	1
thawing_potion	179	timer	0	0	0	0	1
stamina_potion	180	timer	0	0	0	0	1
resist_fire_passive	181	timer	0	0	0	0	0
resist_cold_passive	182	timer	0	0	0	0	0
resist_lightning_passive	183	timer	0	0	0	0	0
mark_of_the_bear	190	timer	0	0	0	0	1
mark_of_the_wolf	191	timer	0	0	0	0	1
blood_oath_passive	195	timer	0	0	0	0	0
demonic_mastery_passive	196	timer	0	0	0	0	0
levitate_mastery_passive	197	timer	0	0	0	0	0
hex_bane_caster	198	timer	0	0	0	0	1
hex_siphon_caster	200	timer	0	0	0	0	1
hex_purge_caster	202	timer	0	0	0	0	1
mind_barrier	203	timer	0	0	0	0	0
enhanced_entropy_passive	204	timer	0	0	0	0	0
sigil_caster	205	timer	0	0	0	0	0
psychic_ward	206	resource	361	362	387	8	1
bind_demon_caster	207	timer	0	0	0	0	0
consume	208	timer	0	0	0	0	1
engorge	211	timer	0	0	0	0	1
lightning_enchant	222	timer	0	0	0	0	0
cold_enchant	223	timer	0	0	0	0	0
)BUFFPANELHUD";
} // namespace BuffPanel::Systems::BuffTracker::DefaultTables

namespace BuffPanel::Systems::BuffTracker {
namespace {

using GetGameFromUnitFn = void*(__fastcall*)(void* unit) noexcept;
using GetStatListFromUnitAndStateFn = void*(__fastcall*)(void* unit, std::int32_t state) noexcept;

// StatList semantic field layout is qualified from Player Buff StatList Probe
// 1.0.2 on D2R build 93847. buff-hud.txt is the authoritative whitelist and
// also declares how each state is presented: finite timer or resource pool.
// Native CURSE lists remain excluded as a safety boundary. Timer rows require
// finite future expiry metadata; resource rows read their current/max values
// through the already-owned Core unit-stat reader and do not require an expiry.
constexpr std::uint32_t UnitTypeOffset = 0x00;
constexpr std::uint32_t PlayerUnitType = 0;
constexpr std::uint32_t StatListCurseFlag = 0x00000020u;
constexpr std::uint32_t SharedStashProxyState = 186u; // states.txt: sharedstash; D2RLoader 1.3 proxy marker
constexpr std::uint32_t FramesPerSecond = 25;
constexpr std::uint32_t MaximumPlausibleDurationFrames = FramesPerSecond * 60u * 60u * 24u * 7u;

const D2RL::PluginContext* Context{};
const D2RL::ResourceService* Resources{};
const D2RL::CustomTableService* CustomTables{};
const D2RL::ThreadService* Threads{};
const D2RL::LifecycleService* Lifecycle{};
GetGameFromUnitFn GetGameFromUnit{};
GetStatListFromUnitAndStateFn GetStatListFromUnitAndState{};
D2RL::CustomTables::TableHandle BuffHudTable{D2RL::CustomTables::InvalidHandle};
LooseBuffHud::Source TableSource{LooseBuffHud::Source::Embedded};
std::string TableSourcePath{};
D2RL::Lifecycle::ListenerHandle DataTablesListener{D2RL::Lifecycle::InvalidHandle};
std::array<D2RL::Lifecycle::ListenerHandle, 3> GameplayListeners{};

struct BuffHudRow final {
    char name[48]{};
    std::uint32_t stateId{};
    char displayType[16]{};
    std::uint32_t valueStat{};
    std::uint32_t maxStat{};
    std::uint32_t skillId{};
    std::uint8_t valueShift{};
    std::uint8_t enabled{};
};

struct BuffDefinition final {
    std::uint32_t stateId{};
    Core::BuffDisplayMode displayMode{Core::BuffDisplayMode::Timer};
    std::int32_t valueStatId{};
    std::int32_t maxStatId{};
    std::int32_t sourceSkillId{};
    std::uint8_t valueShift{};
};

struct WhitelistCache final {
    std::uint64_t revision{};
    std::vector<BuffDefinition> definitions;
};

std::atomic<std::shared_ptr<const WhitelistCache>> Whitelist{};

struct TimerPresenceRecord final {
    std::uint64_t key{};
    std::uint32_t stateId{};
    std::uint32_t skillId{};
    std::uint32_t expireFrame{};
    std::uint32_t lastPostFrame{};
};

std::mutex TimerPresenceMutex;
std::array<TimerPresenceRecord, Core::MaximumPublishedBuffs> TimerPresenceRecords{};

std::atomic<std::uint64_t> CurrentSessionGeneration{};
std::atomic<void*> AuthoritativeGame{};
std::atomic<void*> AuthoritativePlayer{};
std::atomic<bool> FramePumpScheduled{};

std::atomic<std::uint64_t> PlayerPosts{};
std::atomic<std::uint64_t> AuthoritativePosts{};
std::atomic<std::uint64_t> BuffPostsAccepted{};
std::atomic<std::uint64_t> RejectedNotWhitelisted{};
std::atomic<std::uint64_t> RejectedCurse{};
std::atomic<std::uint64_t> RejectedSharedStashProxy{};
std::atomic<std::uint64_t> RejectedNoExpiry{};
std::atomic<std::uint64_t> RejectedResourceUnavailable{};
std::atomic<std::uint64_t> RejectedInvalidMetadata{};
std::atomic<std::uint64_t> PublishFailures{};
std::atomic<std::uint64_t> ResourcePublishes{};
std::atomic<std::uint64_t> ResourceRefreshes{};
std::atomic<std::uint64_t> ResourceRemovals{};
std::atomic<std::uint64_t> TimerPresenceChecks{};
std::atomic<std::uint64_t> TimerPresenceHits{};
std::atomic<std::uint64_t> TimerPresenceMisses{};
std::atomic<std::uint64_t> TimerExpiryRefreshes{};
std::atomic<std::uint64_t> TimerExpiryReadRejects{};
std::atomic<std::uint64_t> TimerDiscoveryFrames{};
std::atomic<std::uint64_t> TimerDiscoveryStateHits{};
std::atomic<std::uint64_t> TimerDiscoveryInvalid{};
std::atomic<std::uint64_t> TimerDiscoveryPublishes{};
std::atomic<std::uint64_t> TimerDiscoveryPublishFailures{};
std::atomic<std::uint32_t> LastDiscoveryState{};
std::atomic<std::uint32_t> LastDiscoveryResult{}; // 0=none 1=absent 2=invalid 3=alreadyPublished 4=published 5=full
std::atomic<std::uint32_t> LastDiscoverySkill{};
std::atomic<std::uint32_t> LastDiscoveryExpiry{};
std::atomic<std::uint32_t> LastTimerDiscoveryFrame{}; // once per game frame

std::atomic<std::uint64_t> PrematureTimerRemovals{};
std::atomic<std::uint64_t> PrematureTimerRemoveMisses{};
std::atomic<std::uint64_t> FramePublishes{};
std::atomic<std::uint64_t> FramePumpRuns{};
std::atomic<std::uint64_t> FramePumpQueueFailures{};
std::atomic<std::uint64_t> TableLoads{};
std::atomic<std::uint64_t> TableLoadFailures{};

std::atomic<std::uint32_t> LastFlags{};
std::atomic<std::uint32_t> LastState{};
std::atomic<std::uint32_t> LastSkill{};
std::atomic<std::uint32_t> LastSkillLevel{};
std::atomic<std::uint32_t> LastExpireFrame{};
std::atomic<std::uint32_t> LastCurrentFrame{};
std::atomic<std::int32_t> LastResourceCurrent{};
std::atomic<std::int32_t> LastResourceMaximum{};
std::atomic<std::uint32_t> LastTimerRefreshState{};
std::atomic<std::uint32_t> LastTimerRefreshSkill{};
std::atomic<std::uint32_t> LastTimerRefreshOldExpire{};
std::atomic<std::uint32_t> LastTimerRefreshNewExpire{};
std::atomic<std::uint32_t> LastTimerRefreshFrame{};
std::atomic<std::uint32_t> LastPrematureRemovedState{};
std::atomic<std::uint32_t> LastPrematureRemovedSkill{};
std::atomic<std::uint32_t> LastPrematureRemovedExpire{};
std::atomic<std::uint32_t> LastPrematureRemovedFrame{};

constexpr std::array<D2RL::CustomTables::ColumnDefinition, 8> BuffHudColumns{{
    {
        .structSize = D2RL::CustomTables::ColumnDefinitionSize,
        .name = "name",
        .type = D2RL::CustomTables::ColumnType::Ascii,
        .offset = static_cast<std::uint32_t>(offsetof(BuffHudRow, name)),
        .length = sizeof(BuffHudRow::name),
    },
    {
        .structSize = D2RL::CustomTables::ColumnDefinitionSize,
        .name = "state_id",
        .type = D2RL::CustomTables::ColumnType::Dword,
        .offset = static_cast<std::uint32_t>(offsetof(BuffHudRow, stateId)),
    },
    {
        .structSize = D2RL::CustomTables::ColumnDefinitionSize,
        .name = "display_type",
        .type = D2RL::CustomTables::ColumnType::Ascii,
        .offset = static_cast<std::uint32_t>(offsetof(BuffHudRow, displayType)),
        .length = sizeof(BuffHudRow::displayType),
    },
    {
        .structSize = D2RL::CustomTables::ColumnDefinitionSize,
        .name = "value_stat",
        .type = D2RL::CustomTables::ColumnType::Dword,
        .offset = static_cast<std::uint32_t>(offsetof(BuffHudRow, valueStat)),
    },
    {
        .structSize = D2RL::CustomTables::ColumnDefinitionSize,
        .name = "max_stat",
        .type = D2RL::CustomTables::ColumnType::Dword,
        .offset = static_cast<std::uint32_t>(offsetof(BuffHudRow, maxStat)),
    },
    {
        .structSize = D2RL::CustomTables::ColumnDefinitionSize,
        .name = "skill_id",
        .type = D2RL::CustomTables::ColumnType::Dword,
        .offset = static_cast<std::uint32_t>(offsetof(BuffHudRow, skillId)),
    },
    {
        .structSize = D2RL::CustomTables::ColumnDefinitionSize,
        .name = "value_shift",
        .type = D2RL::CustomTables::ColumnType::Byte,
        .offset = static_cast<std::uint32_t>(offsetof(BuffHudRow, valueShift)),
    },
    {
        .structSize = D2RL::CustomTables::ColumnDefinitionSize,
        .name = "enabled",
        .type = D2RL::CustomTables::ColumnType::Byte,
        .offset = static_cast<std::uint32_t>(offsetof(BuffHudRow, enabled)),
    },
}};

[[nodiscard]] bool IsReadableRange(const void* address, std::size_t size) noexcept {
    if (address == nullptr || size == 0) return false;
    const auto start = reinterpret_cast<std::uintptr_t>(address);
    const auto end = start + size;
    if (end < start) return false;

    auto current = start;
    while (current < end) {
        MEMORY_BASIC_INFORMATION info{};
        if (VirtualQuery(reinterpret_cast<const void*>(current), &info, sizeof(info)) == 0
            || info.State != MEM_COMMIT
            || (info.Protect & PAGE_GUARD) != 0
            || (info.Protect & PAGE_NOACCESS) != 0) {
            return false;
        }
        const DWORD basic = info.Protect & 0xFFu;
        if (basic != PAGE_READONLY
            && basic != PAGE_READWRITE
            && basic != PAGE_WRITECOPY
            && basic != PAGE_EXECUTE
            && basic != PAGE_EXECUTE_READ
            && basic != PAGE_EXECUTE_READWRITE
            && basic != PAGE_EXECUTE_WRITECOPY) {
            return false;
        }
        const auto regionBase = reinterpret_cast<std::uintptr_t>(info.BaseAddress);
        const auto regionEnd = regionBase + info.RegionSize;
        if (regionEnd <= current) return false;
        current = regionEnd < end ? regionEnd : end;
    }
    return true;
}

template <typename T>
[[nodiscard]] bool ReadField(const void* base, std::size_t offset, T& value) noexcept {
    if (base == nullptr) return false;
    const auto start = reinterpret_cast<std::uintptr_t>(base);
    if (offset > static_cast<std::size_t>(UINTPTR_MAX - start)) return false;
    const auto* address = reinterpret_cast<const void*>(start + offset);
    if (!IsReadableRange(address, sizeof(T))) return false;
    std::memcpy(&value, address, sizeof(T));
    return true;
}

[[nodiscard]] bool ReadGameFrame(void* game, std::uint32_t& frame) noexcept {
    frame = 0;
    if (reinterpret_cast<std::uintptr_t>(game) < 0x10000u) return false;
    return ReadField(game, Native::Contract::GameFrameOffset, frame) && frame != 0;
}

[[nodiscard]] const BuffDefinition* FindBuffDefinition(
    const std::shared_ptr<const WhitelistCache>& cache,
    std::uint32_t state) noexcept {
    if (cache == nullptr) return nullptr;
    const auto it = std::lower_bound(
        cache->definitions.begin(),
        cache->definitions.end(),
        state,
        [](const BuffDefinition& definition, std::uint32_t value) noexcept {
            return definition.stateId < value;
        });
    return it != cache->definitions.end() && it->stateId == state ? &*it : nullptr;
}

[[nodiscard]] bool CopyWhitelistRows(
    std::uint64_t& revision,
    std::uint32_t& rowCount,
    std::vector<std::uint8_t>& bytes) noexcept {
    if (Context == nullptr || CustomTables == nullptr
        || BuffHudTable == D2RL::CustomTables::InvalidHandle) {
        return false;
    }
    D2RL::CustomTables::TableInfo info{};
    info.structSize = D2RL::CustomTables::TableInfoSize;
    if (CustomTables->getTableInfo(
            Context, BuffHudTable, D2RL::CustomTables::TableBank::Rotw, &info)
            != D2RL::CustomTables::Result::Success
        || info.state != D2RL::CustomTables::TableState::Ready
        || info.rowSize != sizeof(BuffHudRow)
        || info.byteCount != static_cast<std::uint64_t>(info.rowCount) * sizeof(BuffHudRow)) {
        return false;
    }
    bytes.resize(static_cast<std::size_t>(info.byteCount));
    if (CustomTables->copyRows(
            Context, BuffHudTable, D2RL::CustomTables::TableBank::Rotw,
            info.revision, bytes.empty() ? nullptr : bytes.data(), info.byteCount)
        != D2RL::CustomTables::Result::Success) {
        return false;
    }
    revision = info.revision;
    rowCount = info.rowCount;
    return true;
}

[[nodiscard]] std::shared_ptr<const WhitelistCache> BuildWhitelist() noexcept {
    std::uint64_t revision{};
    std::uint32_t rowCount{};
    std::vector<std::uint8_t> bytes;
    if (!CopyWhitelistRows(revision, rowCount, bytes)) return {};

    auto cache = std::make_shared<WhitelistCache>();
    cache->revision = revision;
    cache->definitions.reserve(rowCount);
    for (std::uint32_t index = 0; index < rowCount; ++index) {
        BuffHudRow row{};
        std::memcpy(&row, bytes.data() + static_cast<std::size_t>(index) * sizeof(row), sizeof(row));
        if (row.enabled == 0) continue;
        if (row.name[0] == '\0' || row.stateId == 0 || row.stateId > 4095 || row.displayType[0] == '\0') {
            return {};
        }

        BuffDefinition definition{};
        definition.stateId = row.stateId;
        const std::string_view displayType{row.displayType, strnlen(row.displayType, sizeof(row.displayType))};
        if (displayType == "timer") {
            if (row.valueStat != 0 || row.maxStat != 0 || row.skillId != 0 || row.valueShift != 0) return {};
            definition.displayMode = Core::BuffDisplayMode::Timer;
        } else if (displayType == "resource") {
            if (row.valueStat == 0 || row.maxStat == 0 || row.skillId == 0
                || row.valueStat > 4095 || row.maxStat > 4095 || row.skillId > 4095
                || row.valueShift > 30) {
                return {};
            }
            definition.displayMode = Core::BuffDisplayMode::Resource;
            definition.valueStatId = static_cast<std::int32_t>(row.valueStat);
            definition.maxStatId = static_cast<std::int32_t>(row.maxStat);
            definition.sourceSkillId = static_cast<std::int32_t>(row.skillId);
            definition.valueShift = row.valueShift;
        } else {
            return {};
        }
        cache->definitions.push_back(definition);
    }

    std::sort(
        cache->definitions.begin(),
        cache->definitions.end(),
        [](const BuffDefinition& lhs, const BuffDefinition& rhs) noexcept {
            return lhs.stateId < rhs.stateId;
        });
    if (std::adjacent_find(
            cache->definitions.begin(),
            cache->definitions.end(),
            [](const BuffDefinition& lhs, const BuffDefinition& rhs) noexcept {
                return lhs.stateId == rhs.stateId;
            }) != cache->definitions.end()) {
        return {};
    }
    return cache;
}

void __cdecl OnTablesLoaded(
    const D2RL::PluginContext* context,
    const D2RL::Lifecycle::DataTablesLoadedEvent* event,
    void*) noexcept {
    if (context == nullptr
        || !D2RL::Lifecycle::HasDataTablesLoadedEventField(
            event, D2RL::Lifecycle::DataTablesLoadedEventRequiredSize)) {
        return;
    }
    const auto cache = BuildWhitelist();
    if (!cache) {
        Whitelist.store({}, std::memory_order_release);
        TableLoadFailures.fetch_add(1, std::memory_order_relaxed);
        context->LogError("BuffPanel BuffTracker: buff-hud.txt rejected; whitelist-driven BuffHud publishing is disabled.");
        return;
    }
    const auto count = cache->definitions.size();
    const auto revision = cache->revision;
    Whitelist.store(cache, std::memory_order_release);
    TableLoads.fetch_add(1, std::memory_order_relaxed);
    char line[256]{};
    std::snprintf(line, sizeof(line),
        "BuffPanel BuffTracker: buff-hud.txt ready; enabledDefinitions=%zu revision=%llu.",
        count, static_cast<unsigned long long>(revision));
    context->LogInfo(line);
}

[[nodiscard]] std::uint64_t AutomaticBuffKey(std::uint32_t state, std::uint32_t skill) noexcept {
    // Numeric FNV-1a namespace so automatic StatList entries cannot collide
    // with the readable keys used by explicit gameplay producers.
    std::uint64_t hash = 14695981039346656037ULL;
    constexpr char tag[] = "buff-panel:auto-statlist-buff";
    for (const unsigned char ch : tag) {
        if (ch == 0) break;
        hash ^= ch;
        hash *= 1099511628211ULL;
    }
    for (unsigned shift = 0; shift < 32; shift += 8) {
        hash ^= static_cast<std::uint8_t>(state >> shift);
        hash *= 1099511628211ULL;
    }
    for (unsigned shift = 0; shift < 32; shift += 8) {
        hash ^= static_cast<std::uint8_t>(skill >> shift);
        hash *= 1099511628211ULL;
    }
    return hash == 0 ? 1 : hash;
}

[[nodiscard]] bool IsFrameInFuture(std::uint32_t future, std::uint32_t now) noexcept {
    return static_cast<std::int32_t>(future - now) > 0;
}

void ClearTimerPresenceRecords() noexcept {
    std::lock_guard lock(TimerPresenceMutex);
    TimerPresenceRecords = {};
}

void RecordTimerPresence(
    std::uint64_t key,
    std::uint32_t stateId,
    std::uint32_t skillId,
    std::uint32_t expireFrame,
    std::uint32_t postFrame) noexcept {
    if (key == 0 || stateId == 0 || skillId == 0 || expireFrame == 0) return;
    std::lock_guard lock(TimerPresenceMutex);
    TimerPresenceRecord* empty = nullptr;
    for (auto& record : TimerPresenceRecords) {
        if (record.key == key) {
            record.stateId = stateId;
            record.skillId = skillId;
            record.expireFrame = expireFrame;
            record.lastPostFrame = postFrame;
            return;
        }
        if (record.key == 0 && empty == nullptr) empty = &record;
    }
    if (empty != nullptr) {
        *empty = TimerPresenceRecord{
            .key = key,
            .stateId = stateId,
            .skillId = skillId,
            .expireFrame = expireFrame,
            .lastPostFrame = postFrame,
        };
    }
}

// The native list can be re-used when a skill such as Venom is recast. A
// STATLIST_PostStatList notification is not guaranteed for an in-place update:
// read the *currently attached* state every frame, not only its presence.
// These five consecutive fields were qualified on build 93847. Copying them
// together requires one readable-range probe per live state, rather than five.
struct NativeTimerMetadata final {
    std::uint32_t flags{};
    std::uint32_t state{};
    float expireFrameFloat{};
    std::uint32_t skill{};
    std::uint32_t skillLevel{};
};
static_assert(sizeof(NativeTimerMetadata) == Native::Contract::StatListBuffMetadataBytes
    - Native::Contract::StatListBuffFlagsOffset);

[[nodiscard]] bool ReadLiveTimerExpiry(
    void* statList,
    const TimerPresenceRecord& record,
    std::uint32_t currentFrame,
    std::uint32_t& liveExpiry) noexcept {
    NativeTimerMetadata metadata{};
    if (!ReadField(statList, Native::Contract::StatListBuffFlagsOffset, metadata)
        || metadata.state != record.stateId
        || metadata.skill != record.skillId
        || metadata.skillLevel == 0
        || (metadata.flags & StatListCurseFlag) != 0
        || !std::isfinite(metadata.expireFrameFloat)) {
        return false;
    }
    const auto rounded = std::floor(metadata.expireFrameFloat + 0.5f);
    if (rounded <= 0.0f || rounded >= 4294967296.0f
        || std::fabs(metadata.expireFrameFloat - rounded) > 0.01f) {
        return false;
    }
    const auto expiry = static_cast<std::uint32_t>(rounded);
    if (!IsFrameInFuture(expiry, currentFrame)
        || expiry - currentFrame > MaximumPlausibleDurationFrames) {
        return false;
    }
    liveExpiry = expiry;
    return true;
}

// Retain the established expiry/presence lifecycle. The renewal path is
// strictly additive: a verified *later* native expiry can extend a published
// timer, but an unreadable or mismatched native metadata block must never
// shorten or remove a countdown. A posted state gets a full frame to attach.
void RefreshTimerPresence(std::uint32_t currentFrame) noexcept {
    auto* player = AuthoritativePlayer.load(std::memory_order_acquire);
    if (player == nullptr || GetStatListFromUnitAndState == nullptr || currentFrame == 0) return;

    struct Removal final {
        std::uint64_t key{};
        std::uint32_t stateId{};
        std::uint32_t skillId{};
        std::uint32_t expireFrame{};
    };
    struct Refresh final {
        std::uint64_t key{};
        std::uint32_t stateId{};
        std::uint32_t skillId{};
        std::uint32_t oldExpireFrame{};
        std::uint32_t newExpireFrame{};
    };
    std::array<Removal, Core::MaximumPublishedBuffs> removals{};
    std::size_t removalCount{};
    std::array<Refresh, Core::MaximumPublishedBuffs> refreshes{};
    std::size_t refreshCount{};

    {
        std::lock_guard lock(TimerPresenceMutex);
        for (auto& record : TimerPresenceRecords) {
            if (record.key == 0) continue;

            // Preserve the original countdown lifecycle: natural expiration
            // belongs to BuffHud; never reinterpret it as early removal.
            if (!IsFrameInFuture(record.expireFrame, currentFrame)) {
                record = {};
                continue;
            }
            if (record.lastPostFrame == currentFrame) continue;

            TimerPresenceChecks.fetch_add(1, std::memory_order_relaxed);
            void* nativeState = GetStatListFromUnitAndState(
                player, static_cast<std::int32_t>(record.stateId));
            if (nativeState != nullptr) {
                TimerPresenceHits.fetch_add(1, std::memory_order_relaxed);
                std::uint32_t liveExpire{};
                if (ReadLiveTimerExpiry(nativeState, record, currentFrame, liveExpire)) {
                    // Only a strictly later expiry is evidence of a renewal.
                    // Never overwrite the original deadline with a shorter
                    // value or reinterpret state-lookup failure as expiry.
                    if (IsFrameInFuture(liveExpire, record.expireFrame)
                        && liveExpire - record.expireFrame >= 3u
                        && refreshCount < refreshes.size()) {
                        refreshes[refreshCount++] = Refresh{
                            .key = record.key,
                            .stateId = record.stateId,
                            .skillId = record.skillId,
                            .oldExpireFrame = record.expireFrame,
                            .newExpireFrame = liveExpire,
                        };
                        record.expireFrame = liveExpire;
                        record.lastPostFrame = currentFrame;
                    }
                } else {
                    TimerExpiryReadRejects.fetch_add(1, std::memory_order_relaxed);
                }
                continue;
            }

            // Preserve the existing state-removal policy.
            TimerPresenceMisses.fetch_add(1, std::memory_order_relaxed);
            if (removalCount < removals.size()) {
                removals[removalCount++] = Removal{
                    .key = record.key,
                    .stateId = record.stateId,
                    .skillId = record.skillId,
                    .expireFrame = record.expireFrame,
                };
            }
            record = {};
        }
    }

    for (std::size_t i = 0; i < refreshCount; ++i) {
        const auto& refresh = refreshes[i];
        const auto snapshot = Core::BuffDisplays().Snapshot();
        for (std::size_t j = 0; j < snapshot.count; ++j) {
            const auto& entry = snapshot.entries[j];
            if (entry.key != refresh.key || entry.displayMode != Core::BuffDisplayMode::Timer
                || entry.expireGameFrame != refresh.oldExpireFrame) continue;
            auto updated = entry;
            updated.expireGameFrame = refresh.newExpireFrame;
            if (!Core::BuffDisplays().Upsert(updated)) {
                PublishFailures.fetch_add(1, std::memory_order_relaxed);
                break;
            }
            TimerExpiryRefreshes.fetch_add(1, std::memory_order_relaxed);
            LastTimerRefreshState.store(refresh.stateId, std::memory_order_relaxed);
            LastTimerRefreshSkill.store(refresh.skillId, std::memory_order_relaxed);
            LastTimerRefreshOldExpire.store(refresh.oldExpireFrame, std::memory_order_relaxed);
            LastTimerRefreshNewExpire.store(refresh.newExpireFrame, std::memory_order_relaxed);
            LastTimerRefreshFrame.store(currentFrame, std::memory_order_relaxed);
            break;
        }
    }

    for (std::size_t i = 0; i < removalCount; ++i) {
        const auto& removal = removals[i];
        LastPrematureRemovedState.store(removal.stateId, std::memory_order_relaxed);
        LastPrematureRemovedSkill.store(removal.skillId, std::memory_order_relaxed);
        LastPrematureRemovedExpire.store(removal.expireFrame, std::memory_order_relaxed);
        LastPrematureRemovedFrame.store(currentFrame, std::memory_order_relaxed);
        if (Core::BuffDisplays().Remove(removal.key)) {
            PrematureTimerRemovals.fetch_add(1, std::memory_order_relaxed);
        } else {
            PrematureTimerRemoveMisses.fetch_add(1, std::memory_order_relaxed);
        }
    }
}

[[nodiscard]] std::int32_t NormalizeResourceValue(std::int32_t rawValue, std::uint8_t valueShift) noexcept {
    if (valueShift == 0) return rawValue;
    const auto divisor = static_cast<std::int64_t>(1) << valueShift;
    return static_cast<std::int32_t>(static_cast<std::int64_t>(rawValue) / divisor);
}

void RefreshResourceBuffs() noexcept {
    auto* player = AuthoritativePlayer.load(std::memory_order_acquire);
    const auto getter = Core::StatReads().RawGetter();
    const auto cache = Whitelist.load(std::memory_order_acquire);
    if (player == nullptr || getter == nullptr || cache == nullptr) return;

    // D2RLoader 1.3 materializes the shared stash as a UNIT_PLAYER-shaped
    // proxy carrying state 186 (sharedstash). Never poll player stats from that
    // proxy: Loader deliberately asserts if its proxy is queried before/without
    // the marker state, and the proxy is not the gameplay player anyway.
    if (GetStatListFromUnitAndState != nullptr
        && GetStatListFromUnitAndState(player, SharedStashProxyState) != nullptr) {
        AuthoritativePlayer.store(nullptr, std::memory_order_release);
        RejectedSharedStashProxy.fetch_add(1, std::memory_order_relaxed);
        return;
    }

    const auto snapshot = Core::BuffDisplays().Snapshot();
    for (const auto& definition : cache->definitions) {
        if (definition.displayMode != Core::BuffDisplayMode::Resource
            || definition.valueStatId <= 0
            || definition.maxStatId <= 0
            || definition.sourceSkillId <= 0) {
            continue;
        }

        const auto key = AutomaticBuffKey(
            definition.stateId,
            static_cast<std::uint32_t>(definition.sourceSkillId));
        const Core::BuffDisplayEntry* existing = nullptr;
        for (std::size_t index = 0; index < snapshot.count; ++index) {
            if (snapshot.entries[index].key == key) {
                existing = &snapshot.entries[index];
                break;
            }
        }

        // Multiple enabled resource states can share one unit-stat pool:
        // Bone Armor and Cyclone Armor both use bonearmor/bonearmormax.
        // Resolve the *actual* attached native state in that case so a single
        // pool never creates two different skill icons. Keep the original
        // stat-only fallback for an unambiguous resource definition.
        bool sharesResourcePair = false;
        for (const auto& other : cache->definitions) {
            if (other.displayMode == Core::BuffDisplayMode::Resource
                && other.stateId != definition.stateId
                && other.valueStatId == definition.valueStatId
                && other.maxStatId == definition.maxStatId) {
                sharesResourcePair = true;
                break;
            }
        }
        if (sharesResourcePair) {
            const bool isAttached = GetStatListFromUnitAndState != nullptr
                && GetStatListFromUnitAndState(
                    player, static_cast<std::int32_t>(definition.stateId)) != nullptr;
            if (!isAttached) {
                if (existing != nullptr && Core::BuffDisplays().Remove(key)) {
                    ResourceRemovals.fetch_add(1, std::memory_order_relaxed);
                }
                continue;
            }
        }

        const auto rawCurrent = getter(player, definition.valueStatId, 0);
        const auto rawMaximum = getter(player, definition.maxStatId, 0);
        const auto current = NormalizeResourceValue(rawCurrent, definition.valueShift);
        const auto maximum = NormalizeResourceValue(rawMaximum, definition.valueShift);
        LastResourceCurrent.store(current, std::memory_order_relaxed);
        LastResourceMaximum.store(maximum, std::memory_order_relaxed);

        if (rawCurrent <= 0 || rawMaximum <= 0) {
            if (existing != nullptr && Core::BuffDisplays().Remove(key)) {
                ResourceRemovals.fetch_add(1, std::memory_order_relaxed);
            }
            if (rawCurrent > 0 && rawMaximum <= 0) {
                RejectedResourceUnavailable.fetch_add(1, std::memory_order_relaxed);
            }
            continue;
        }

        if (existing != nullptr
            && existing->displayMode == Core::BuffDisplayMode::Resource
            && existing->sourceSkillId == definition.sourceSkillId
            && existing->valueStatId == definition.valueStatId
            && existing->maxStatId == definition.maxStatId
            && existing->currentValue == current
            && existing->maximumValue == maximum) {
            continue;
        }

        Core::BuffDisplayEntry entry{};
        entry.key = key;
        entry.sourceSkillId = definition.sourceSkillId;
        entry.displayMode = Core::BuffDisplayMode::Resource;
        entry.valueStatId = definition.valueStatId;
        entry.maxStatId = definition.maxStatId;
        entry.currentValue = current;
        entry.maximumValue = maximum;
        entry.stacks = 1;
        entry.priority = 100;
        if (!Core::BuffDisplays().Upsert(entry)) {
            PublishFailures.fetch_add(1, std::memory_order_relaxed);
            continue;
        }

        if (existing == nullptr) {
            ResourcePublishes.fetch_add(1, std::memory_order_relaxed);
            BuffPostsAccepted.fetch_add(1, std::memory_order_relaxed);
        } else {
            ResourceRefreshes.fetch_add(1, std::memory_order_relaxed);
        }
    }
}

// Some duration effects renew an attached StatList in place, without a new
// STATLIST_PostStatList event. Discover only explicitly whitelisted timer
// states from the authoritative player's currently attached native lists.
// This is a read-only fallback, not a second native hook or a new buff source.
void DiscoverAttachedTimers(std::uint32_t currentFrame) noexcept {
    if (currentFrame == 0 || LastTimerDiscoveryFrame.load(std::memory_order_relaxed) == currentFrame) return;
    LastTimerDiscoveryFrame.store(currentFrame, std::memory_order_relaxed);
    auto* player = AuthoritativePlayer.load(std::memory_order_acquire);
    const auto cache = Whitelist.load(std::memory_order_acquire);
    if (player == nullptr || GetStatListFromUnitAndState == nullptr || cache == nullptr) return;
    // The loader's UNIT_PLAYER-shaped shared stash proxy must never be treated
    // as a gameplay player. This matches RefreshResourceBuffs' safety guard.
    if (GetStatListFromUnitAndState(player, SharedStashProxyState) != nullptr) return;

    TimerDiscoveryFrames.fetch_add(1, std::memory_order_relaxed);
    auto snapshot = Core::BuffDisplays().Snapshot();
    for (const auto& definition : cache->definitions) {
        if (definition.displayMode != Core::BuffDisplayMode::Timer) continue;
        const auto state = definition.stateId;
        LastDiscoveryState.store(state, std::memory_order_relaxed);
        LastDiscoveryResult.store(1, std::memory_order_relaxed);
        LastDiscoverySkill.store(0, std::memory_order_relaxed);
        LastDiscoveryExpiry.store(0, std::memory_order_relaxed);
        auto* nativeState = GetStatListFromUnitAndState(player, static_cast<std::int32_t>(state));
        if (nativeState == nullptr) continue;
        TimerDiscoveryStateHits.fetch_add(1, std::memory_order_relaxed);

        NativeTimerMetadata metadata{};
        if (!ReadField(nativeState, Native::Contract::StatListBuffFlagsOffset, metadata)
            || metadata.state != state || metadata.skill == 0 || metadata.skillLevel == 0
            || (metadata.flags & StatListCurseFlag) != 0) {
            TimerDiscoveryInvalid.fetch_add(1, std::memory_order_relaxed);
            LastDiscoveryResult.store(2, std::memory_order_relaxed);
            continue;
        }
        const TimerPresenceRecord witness{
            .key = AutomaticBuffKey(state, metadata.skill),
            .stateId = state,
            .skillId = metadata.skill,
        };
        std::uint32_t expiry{};
        if (!ReadLiveTimerExpiry(nativeState, witness, currentFrame, expiry)) {
            TimerDiscoveryInvalid.fetch_add(1, std::memory_order_relaxed);
            LastDiscoveryResult.store(2, std::memory_order_relaxed);
            LastDiscoverySkill.store(metadata.skill, std::memory_order_relaxed);
            continue;
        }
        LastDiscoverySkill.store(metadata.skill, std::memory_order_relaxed);
        LastDiscoveryExpiry.store(expiry, std::memory_order_relaxed);
        bool alreadyPublished = false;
        for (std::size_t i = 0; i < snapshot.count; ++i) {
            if (snapshot.entries[i].key == witness.key
                && snapshot.entries[i].displayMode == Core::BuffDisplayMode::Timer) {
                alreadyPublished = true;
                break;
            }
        }
        if (alreadyPublished) {
            LastDiscoveryResult.store(3, std::memory_order_relaxed);
            continue;
        }
        Core::BuffDisplayEntry entry{};
        entry.key = witness.key;
        entry.sourceSkillId = static_cast<std::int32_t>(metadata.skill);
        entry.displayMode = Core::BuffDisplayMode::Timer;
        entry.expireGameFrame = expiry;
        entry.stacks = 1;
        entry.priority = 100;
        if (!Core::BuffDisplays().Upsert(entry)) {
            TimerDiscoveryPublishFailures.fetch_add(1, std::memory_order_relaxed);
            LastDiscoveryResult.store(5, std::memory_order_relaxed);
            continue;
        }
        RecordTimerPresence(witness.key, state, metadata.skill, expiry, currentFrame);
        TimerDiscoveryPublishes.fetch_add(1, std::memory_order_relaxed);
        BuffPostsAccepted.fetch_add(1, std::memory_order_relaxed);
        LastDiscoveryResult.store(4, std::memory_order_relaxed);
        // Keep the per-frame snapshot in step so a duplicate table row or key
        // cannot consume two HUD positions during this scan.
        snapshot = Core::BuffDisplays().Snapshot();
    }
}

void QueueFramePump() noexcept;

void __cdecl FramePumpOnGameThread(const D2RL::PluginContext* context, void*) noexcept {
    FramePumpScheduled.store(false, std::memory_order_release);
    if (context == nullptr || context != Context) return;
    const auto session = CurrentSessionGeneration.load(std::memory_order_acquire);
    if (session == 0) return;

    FramePumpRuns.fetch_add(1, std::memory_order_relaxed);
    auto* game = AuthoritativeGame.load(std::memory_order_acquire);
    std::uint32_t frame{};
    if (ReadGameFrame(game, frame)) {
        Core::BuffDisplays().PublishGameFrame(session, frame);
        LastCurrentFrame.store(frame, std::memory_order_relaxed);
        FramePublishes.fetch_add(1, std::memory_order_relaxed);
        RefreshResourceBuffs();
        RefreshTimerPresence(frame);
        DiscoverAttachedTimers(frame);
        QueueFramePump();
    }
}

void QueueFramePump() noexcept {
    if (Context == nullptr || Threads == nullptr
        || CurrentSessionGeneration.load(std::memory_order_acquire) == 0
        || AuthoritativeGame.load(std::memory_order_acquire) == nullptr) {
        return;
    }
    bool expected = false;
    if (!FramePumpScheduled.compare_exchange_strong(
            expected,
            true,
            std::memory_order_acq_rel,
            std::memory_order_relaxed)) {
        return;
    }
    if (Threads->runOnGameThread(Context, &FramePumpOnGameThread, nullptr)
        != D2RL::Threads::Result::Success) {
        FramePumpScheduled.store(false, std::memory_order_release);
        FramePumpQueueFailures.fetch_add(1, std::memory_order_relaxed);
    }
}

void OnStatListPost(
    const Core::StatListPostEvent& event,
    Core::StatListPostPhase phase,
    void*) noexcept {
    if (event.unit == nullptr || event.statList == nullptr || GetGameFromUnit == nullptr) return;

    std::uint32_t unitType{};
    if (!ReadField(event.unit, UnitTypeOffset, unitType) || unitType != PlayerUnitType) return;

    // Read StatList metadata before any native unit helper. D2RLoader 1.3
    // creates its shared-stash proxy by posting state 186 to a UNIT_PLAYER-
    // shaped proxy. During BeforeNative that marker is not attached yet.
    if (!IsReadableRange(event.statList, Native::Contract::StatListBuffMetadataBytes)) {
        if (phase == Core::StatListPostPhase::BeforeNative) {
            RejectedInvalidMetadata.fetch_add(1, std::memory_order_relaxed);
        }
        return;
    }

    std::uint32_t flags{};
    std::uint32_t state{};
    float expireFrameFloat{};
    std::uint32_t skill{};
    std::uint32_t skillLevel{};
    if (!ReadField(event.statList, Native::Contract::StatListBuffFlagsOffset, flags)
        || !ReadField(event.statList, Native::Contract::StatListBuffStateOffset, state)
        || !ReadField(event.statList, Native::Contract::StatListBuffExpireFrameFloatOffset, expireFrameFloat)
        || !ReadField(event.statList, Native::Contract::StatListBuffSkillIdOffset, skill)
        || !ReadField(event.statList, Native::Contract::StatListBuffSkillLevelOffset, skillLevel)) {
        if (phase == Core::StatListPostPhase::BeforeNative) {
            RejectedInvalidMetadata.fetch_add(1, std::memory_order_relaxed);
        }
        return;
    }

    const bool sharedStashProxy = state == SharedStashProxyState
        || (GetStatListFromUnitAndState != nullptr
            && GetStatListFromUnitAndState(event.unit, SharedStashProxyState) != nullptr);
    if (sharedStashProxy) {
        RejectedSharedStashProxy.fetch_add(1, std::memory_order_relaxed);
        return;
    }

    void* game = GetGameFromUnit(event.unit);
    std::uint32_t currentFrame{};
    if (!ReadGameFrame(game, currentFrame)) return;

    if (phase == Core::StatListPostPhase::BeforeNative) {
        PlayerPosts.fetch_add(1, std::memory_order_relaxed);
        AuthoritativePosts.fetch_add(1, std::memory_order_relaxed);
        AuthoritativeGame.store(game, std::memory_order_release);
        AuthoritativePlayer.store(event.unit, std::memory_order_release);
        const auto session = CurrentSessionGeneration.load(std::memory_order_acquire);
        if (session != 0) {
            Core::BuffDisplays().PublishGameFrame(session, currentFrame);
            FramePublishes.fetch_add(1, std::memory_order_relaxed);
            LastCurrentFrame.store(currentFrame, std::memory_order_relaxed);
            QueueFramePump();
        }
    }

    if (phase == Core::StatListPostPhase::BeforeNative) {
        LastFlags.store(flags, std::memory_order_relaxed);
        LastState.store(state, std::memory_order_relaxed);
        LastSkill.store(skill, std::memory_order_relaxed);
        LastSkillLevel.store(skillLevel, std::memory_order_relaxed);
    }

    const auto cache = Whitelist.load(std::memory_order_acquire);
    const auto* definition = FindBuffDefinition(cache, state);
    if (definition == nullptr) {
        if (phase == Core::StatListPostPhase::BeforeNative) {
            RejectedNotWhitelisted.fetch_add(1, std::memory_order_relaxed);
        }
        return;
    }
    if ((flags & StatListCurseFlag) != 0) {
        if (phase == Core::StatListPostPhase::BeforeNative) {
            RejectedCurse.fetch_add(1, std::memory_order_relaxed);
        }
        return;
    }
    const auto session = CurrentSessionGeneration.load(std::memory_order_acquire);
    if (session == 0) return;

    if (definition->displayMode == Core::BuffDisplayMode::Timer) {
        if (state == 0 || skill == 0 || skillLevel == 0) {
            if (phase == Core::StatListPostPhase::BeforeNative) {
                RejectedInvalidMetadata.fetch_add(1, std::memory_order_relaxed);
            }
            return;
        }
        // Timer semantic fields were runtime-qualified before the native post.
        if (phase != Core::StatListPostPhase::BeforeNative) return;
        if (!std::isfinite(expireFrameFloat)) {
            RejectedInvalidMetadata.fetch_add(1, std::memory_order_relaxed);
            return;
        }
        const auto rounded = std::floor(expireFrameFloat + 0.5f);
        if (rounded <= 0.0f || rounded > static_cast<float>(UINT32_MAX)
            || std::fabs(expireFrameFloat - rounded) > 0.01f) {
            RejectedInvalidMetadata.fetch_add(1, std::memory_order_relaxed);
            return;
        }
        const auto expireFrame = static_cast<std::uint32_t>(rounded);
        LastExpireFrame.store(expireFrame, std::memory_order_relaxed);
        const auto duration = expireFrame - currentFrame;
        if (expireFrame <= currentFrame || duration > MaximumPlausibleDurationFrames) {
            RejectedNoExpiry.fetch_add(1, std::memory_order_relaxed);
            return;
        }

        Core::BuffDisplayEntry entry{};
        entry.key = AutomaticBuffKey(state, skill);
        entry.sourceSkillId = static_cast<std::int32_t>(skill);
        entry.displayMode = Core::BuffDisplayMode::Timer;
        entry.expireGameFrame = expireFrame;
        entry.stacks = 1;
        entry.priority = 100;
        if (!Core::BuffDisplays().Upsert(entry)) {
            PublishFailures.fetch_add(1, std::memory_order_relaxed);
            return;
        }
        RecordTimerPresence(entry.key, state, skill, expireFrame, currentFrame);
        BuffPostsAccepted.fetch_add(1, std::memory_order_relaxed);
        return;
    }

    // Resource-mode rows are not classified from StatList metadata. Some
    // absorb-pool skills (notably Bone Armor) do not expose a reliable
    // state/skill/level tuple on the posted list. The frame pump therefore
    // discovers resource buffs directly from their configured current/max unit
    // stats and uses buff-hud.txt skill_id only to resolve the icon/name.
    return;

}

void __cdecl OnGameplayEvent(
    const D2RL::PluginContext*,
    const D2RL::Lifecycle::GameplayEvent* event,
    void*) noexcept {
    if (event == nullptr) return;
    switch (event->kind) {
    case D2RL::Lifecycle::GameplayEventKind::GameJoined:
        ClearTimerPresenceRecords();
        LastTimerDiscoveryFrame.store(0, std::memory_order_relaxed);
        CurrentSessionGeneration.store(event->sessionGeneration, std::memory_order_release);
        QueueFramePump();
        break;
    case D2RL::Lifecycle::GameplayEventKind::LocalPlayerReady:
        CurrentSessionGeneration.store(event->sessionGeneration, std::memory_order_release);
        QueueFramePump();
        break;
    case D2RL::Lifecycle::GameplayEventKind::GameLeft:
        CurrentSessionGeneration.store(0, std::memory_order_release);
        AuthoritativeGame.store(nullptr, std::memory_order_release);
        AuthoritativePlayer.store(nullptr, std::memory_order_release);
        FramePumpScheduled.store(false, std::memory_order_release);
        ClearTimerPresenceRecords();
        break;
    default:
        break;
    }
}

D2RL::ConsoleCommandResult __cdecl TrackerCommand(
    D2R::Game::Client*,
    const D2RL::ConsoleCommandContext* command,
    void*) noexcept {
    if (command == nullptr || command->plugin == nullptr) return D2RL::ConsoleCommandResult::Failed;
    char line[768]{};
    std::snprintf(
        line,
        sizeof(line),
        "BuffPanel BuffTracker: session=%llu playerPosts=%llu authoritative=%llu accepted=%llu reject(notWhitelisted=%llu curse=%llu noExpiry=%llu resourceUnavailable=%llu invalid=%llu) resource(publish=%llu refresh=%llu remove=%llu) timerPresence(check=%llu hit=%llu miss=%llu removed=%llu removeMiss=%llu) publishFail=%llu framePublishes=%llu pumpRuns=%llu pumpQueueFail=%llu tableLoads=%llu tableFail=%llu definitions=%zu.",
        static_cast<unsigned long long>(CurrentSessionGeneration.load(std::memory_order_relaxed)),
        static_cast<unsigned long long>(PlayerPosts.load(std::memory_order_relaxed)),
        static_cast<unsigned long long>(AuthoritativePosts.load(std::memory_order_relaxed)),
        static_cast<unsigned long long>(BuffPostsAccepted.load(std::memory_order_relaxed)),
        static_cast<unsigned long long>(RejectedNotWhitelisted.load(std::memory_order_relaxed)),
        static_cast<unsigned long long>(RejectedCurse.load(std::memory_order_relaxed)),
        static_cast<unsigned long long>(RejectedNoExpiry.load(std::memory_order_relaxed)),
        static_cast<unsigned long long>(RejectedResourceUnavailable.load(std::memory_order_relaxed)),
        static_cast<unsigned long long>(RejectedInvalidMetadata.load(std::memory_order_relaxed)),
        static_cast<unsigned long long>(ResourcePublishes.load(std::memory_order_relaxed)),
        static_cast<unsigned long long>(ResourceRefreshes.load(std::memory_order_relaxed)),
        static_cast<unsigned long long>(ResourceRemovals.load(std::memory_order_relaxed)),
        static_cast<unsigned long long>(TimerPresenceChecks.load(std::memory_order_relaxed)),
        static_cast<unsigned long long>(TimerPresenceHits.load(std::memory_order_relaxed)),
        static_cast<unsigned long long>(TimerPresenceMisses.load(std::memory_order_relaxed)),
        static_cast<unsigned long long>(PrematureTimerRemovals.load(std::memory_order_relaxed)),
        static_cast<unsigned long long>(PrematureTimerRemoveMisses.load(std::memory_order_relaxed)),
        static_cast<unsigned long long>(PublishFailures.load(std::memory_order_relaxed)),
        static_cast<unsigned long long>(FramePublishes.load(std::memory_order_relaxed)),
        static_cast<unsigned long long>(FramePumpRuns.load(std::memory_order_relaxed)),
        static_cast<unsigned long long>(FramePumpQueueFailures.load(std::memory_order_relaxed)),
        static_cast<unsigned long long>(TableLoads.load(std::memory_order_relaxed)),
        static_cast<unsigned long long>(TableLoadFailures.load(std::memory_order_relaxed)),
        Whitelist.load(std::memory_order_acquire) != nullptr
            ? Whitelist.load(std::memory_order_acquire)->definitions.size() : 0u);
    command->plugin->WriteConsoleMessage(line);
    std::snprintf(line, sizeof(line),
        "BuffPanel BuffTracker timer expiry polling: refresh=%llu rejectedLiveExpiry=%llu lastRefresh(state=%u skill=%u oldExpire=%u newExpire=%u frame=%u).",
        static_cast<unsigned long long>(TimerExpiryRefreshes.load(std::memory_order_relaxed)),
        static_cast<unsigned long long>(TimerExpiryReadRejects.load(std::memory_order_relaxed)),
        LastTimerRefreshState.load(std::memory_order_relaxed),
        LastTimerRefreshSkill.load(std::memory_order_relaxed),
        LastTimerRefreshOldExpire.load(std::memory_order_relaxed),
        LastTimerRefreshNewExpire.load(std::memory_order_relaxed),
        LastTimerRefreshFrame.load(std::memory_order_relaxed));
    command->plugin->WriteConsoleMessage(line);
    std::snprintf(line, sizeof(line),
        "BuffPanel BuffTracker attached timer discovery: frames=%llu nativeHits=%llu invalid=%llu published=%llu publishFail=%llu last(state=%u result=%u skill=%u expiry=%u; result:1=absent 2=invalid 3=alreadyPublished 4=published 5=full).",
        static_cast<unsigned long long>(TimerDiscoveryFrames.load(std::memory_order_relaxed)),
        static_cast<unsigned long long>(TimerDiscoveryStateHits.load(std::memory_order_relaxed)),
        static_cast<unsigned long long>(TimerDiscoveryInvalid.load(std::memory_order_relaxed)),
        static_cast<unsigned long long>(TimerDiscoveryPublishes.load(std::memory_order_relaxed)),
        static_cast<unsigned long long>(TimerDiscoveryPublishFailures.load(std::memory_order_relaxed)),
        LastDiscoveryState.load(std::memory_order_relaxed),
        LastDiscoveryResult.load(std::memory_order_relaxed),
        LastDiscoverySkill.load(std::memory_order_relaxed),
        LastDiscoveryExpiry.load(std::memory_order_relaxed));
    command->plugin->WriteConsoleMessage(line);
    const auto lastExpire = LastExpireFrame.load(std::memory_order_relaxed);
    const auto lastCurrent = LastCurrentFrame.load(std::memory_order_relaxed);
    const auto remaining = static_cast<std::int64_t>(lastExpire) - static_cast<std::int64_t>(lastCurrent);
    std::snprintf(
        line,
        sizeof(line),
        "BuffPanel BuffTracker last semantic witness: flags=0x%08X state=%u skill=%u slvl=%u expire=%u current=%u remaining=%lld frames (%.2fs).",
        LastFlags.load(std::memory_order_relaxed),
        LastState.load(std::memory_order_relaxed),
        LastSkill.load(std::memory_order_relaxed),
        LastSkillLevel.load(std::memory_order_relaxed),
        lastExpire,
        lastCurrent,
        static_cast<long long>(remaining),
        static_cast<double>(remaining) / FramesPerSecond);
    command->plugin->WriteConsoleMessage(line);
    std::snprintf(
        line,
        sizeof(line),
        "BuffPanel BuffTracker last resource witness: current=%d maximum=%d.",
        LastResourceCurrent.load(std::memory_order_relaxed),
        LastResourceMaximum.load(std::memory_order_relaxed));
    command->plugin->WriteConsoleMessage(line);
    std::snprintf(
        line,
        sizeof(line),
        "BuffPanel BuffTracker last premature timer removal: state=%u skill=%u expectedExpire=%u removedAt=%u remaining=%lld frames (%.2fs).",
        LastPrematureRemovedState.load(std::memory_order_relaxed),
        LastPrematureRemovedSkill.load(std::memory_order_relaxed),
        LastPrematureRemovedExpire.load(std::memory_order_relaxed),
        LastPrematureRemovedFrame.load(std::memory_order_relaxed),
        static_cast<long long>(static_cast<std::int64_t>(LastPrematureRemovedExpire.load(std::memory_order_relaxed))
            - static_cast<std::int64_t>(LastPrematureRemovedFrame.load(std::memory_order_relaxed))),
        static_cast<double>(static_cast<std::int64_t>(LastPrematureRemovedExpire.load(std::memory_order_relaxed))
            - static_cast<std::int64_t>(LastPrematureRemovedFrame.load(std::memory_order_relaxed))) / FramesPerSecond);
    command->plugin->WriteConsoleMessage(line);
    std::snprintf(line, sizeof(line),
        "BuffPanel buff-hud.txt source=%s (restart D2R after editing the loose TXT).",
        TableSource == LooseBuffHud::Source::ActiveMod ? "active-mod" : "embedded");
    command->plugin->WriteConsoleMessage(line);
    if (TableSource == LooseBuffHud::Source::ActiveMod) {
        command->plugin->WriteConsoleMessage(TableSourcePath.c_str());
    }
    command->plugin->WriteConsoleMessage(
        "Detection policy: timer rows are matched by whitelisted state on authoritative UNIT_PLAYER StatLists and require native skill/level plus finite future expiry. Subsequent game-thread polling extends an existing HUD timer only for a validated later attached-state expiry; failed metadata reads have no effect on the countdown. Early native state removal retires it. Resource rows poll configured current/max stats. STATLIST_BUFF is ignored; STATLIST_CURSE remains excluded.");
    return D2RL::ConsoleCommandResult::Handled;
}

[[nodiscard]] bool RegisterTable() noexcept {
    if (Context == nullptr || Resources == nullptr || CustomTables == nullptr) return false;
    // Do not depend on opaque resource-overlay priority. Explicitly select the
    // active mod's loose file BEFORE registering a single in-memory resource.
    // D2RLoader copies the selected bytes during registerResource().
    const auto selection = LooseBuffHud::Select(
        Context->modDirectory, Context->activeMod,
        std::string_view(DefaultTables::BuffHud, sizeof(DefaultTables::BuffHud) - 1));
    if (selection.source == LooseBuffHud::Source::InvalidOverride) {
        char line[512]{};
        std::snprintf(line, sizeof(line),
            "Buff Panel: external buff-hud.txt rejected: %s. Correct or delete it; no silent fallback.",
            selection.error.c_str());
        Context->LogError(line);
        return false;
    }
    TableSource = selection.source;
    TableSourcePath.clear();
    if (TableSource == LooseBuffHud::Source::ActiveMod) {
        try {
            const auto utf8 = selection.path.u8string();
            TableSourcePath.assign(utf8.begin(), utf8.end());
        } catch (...) {
            TableSourcePath = "<active mod file>";
        }
    }
    const D2RL::Resources::ResourceRegistration resource{
        .structSize = D2RL::Resources::ResourceRegistrationSize,
        .flags = 0,
        .path = "data/global/excel/d2rloader/buff-panel/buff-hud.txt",
        .bytes = selection.bytes.data(),
        .byteCount = selection.bytes.size(),
    };
    D2RL::Resources::RegistrationHandle resourceHandle{D2RL::Resources::InvalidHandle};
    if (Resources->registerResource(Context, &resource, &resourceHandle)
            != D2RL::Resources::Result::Success
        || resourceHandle == D2RL::Resources::InvalidHandle) {
        Context->LogError("Buff Panel: unable to register selected buff-hud.txt as a loader resource.");
        return false;
    }
    const D2RL::CustomTables::TableRegistration registration{
        .structSize = D2RL::CustomTables::TableRegistrationSize,
        .name = "buff-hud",
        .banks = D2RL::CustomTables::TableBank::Rotw,
        .rowSize = sizeof(BuffHudRow),
        .columns = BuffHudColumns.data(),
        .columnCount = static_cast<std::uint32_t>(BuffHudColumns.size()),
        .columnStride = D2RL::CustomTables::ColumnDefinitionSize,
    };
    if (CustomTables->registerTable(Context, &registration, &BuffHudTable)
            != D2RL::CustomTables::Result::Success
        || BuffHudTable == D2RL::CustomTables::InvalidHandle) {
        Context->LogError("Buff Panel: unable to register selected buff-hud.txt custom table.");
        return false;
    }
    char line[768]{};
    if (TableSource == LooseBuffHud::Source::ActiveMod) {
        std::snprintf(line, sizeof(line),
            "Buff Panel: buff-hud.txt source=active-mod file=%s; override loaded; restart D2R after edits.",
            TableSourcePath.c_str());
    } else {
        std::snprintf(line, sizeof(line),
            "Buff Panel: buff-hud.txt source=embedded (active mod has no loose override)."
            " No loose file is required; restart D2R after adding one.");
    }
    Context->LogInfo(line);
    return true;
}

[[nodiscard]] bool RegisterLifecycle() noexcept {
    if (Context == nullptr || Lifecycle == nullptr) return false;
    const D2RL::Lifecycle::DataTablesLoadedListener tableListener{
        .structSize = D2RL::Lifecycle::DataTablesLoadedListenerSize,
        .flags = 0,
        .callback = &OnTablesLoaded,
        .userData = nullptr,
    };
    if (Lifecycle->registerDataTablesLoadedListener(Context, &tableListener, &DataTablesListener)
            != D2RL::Lifecycle::Result::Success
        || DataTablesListener == D2RL::Lifecycle::InvalidHandle) {
        return false;
    }
    constexpr std::array kinds{
        D2RL::Lifecycle::GameplayEventKind::GameJoined,
        D2RL::Lifecycle::GameplayEventKind::LocalPlayerReady,
        D2RL::Lifecycle::GameplayEventKind::GameLeft,
    };
    for (std::size_t i = 0; i < kinds.size(); ++i) {
        const D2RL::Lifecycle::GameplayEventListener listener{
            .structSize = D2RL::Lifecycle::GameplayEventListenerSize,
            .flags = 0,
            .kind = kinds[i],
            .reserved = 0,
            .callback = &OnGameplayEvent,
            .userData = nullptr,
        };
        if (Lifecycle->registerGameplayEventListener(Context, &listener, &GameplayListeners[i])
            != D2RL::Lifecycle::Result::Success) {
            return false;
        }
    }
    return true;
}

void ResetDiagnostics() noexcept {
    PlayerPosts.store(0, std::memory_order_relaxed);
    AuthoritativePosts.store(0, std::memory_order_relaxed);
    BuffPostsAccepted.store(0, std::memory_order_relaxed);
    RejectedNotWhitelisted.store(0, std::memory_order_relaxed);
    RejectedCurse.store(0, std::memory_order_relaxed);
    RejectedSharedStashProxy.store(0, std::memory_order_relaxed);
    RejectedNoExpiry.store(0, std::memory_order_relaxed);
    RejectedResourceUnavailable.store(0, std::memory_order_relaxed);
    RejectedInvalidMetadata.store(0, std::memory_order_relaxed);
    PublishFailures.store(0, std::memory_order_relaxed);
    ResourcePublishes.store(0, std::memory_order_relaxed);
    ResourceRefreshes.store(0, std::memory_order_relaxed);
    ResourceRemovals.store(0, std::memory_order_relaxed);
    TimerPresenceChecks.store(0, std::memory_order_relaxed);
    TimerPresenceHits.store(0, std::memory_order_relaxed);
    TimerPresenceMisses.store(0, std::memory_order_relaxed);
    TimerExpiryRefreshes.store(0, std::memory_order_relaxed);
    TimerExpiryReadRejects.store(0, std::memory_order_relaxed);
    TimerDiscoveryFrames.store(0, std::memory_order_relaxed);
    TimerDiscoveryStateHits.store(0, std::memory_order_relaxed);
    TimerDiscoveryInvalid.store(0, std::memory_order_relaxed);
    TimerDiscoveryPublishes.store(0, std::memory_order_relaxed);
    TimerDiscoveryPublishFailures.store(0, std::memory_order_relaxed);
    LastDiscoveryState.store(0, std::memory_order_relaxed);
    LastDiscoveryResult.store(0, std::memory_order_relaxed);
    LastDiscoverySkill.store(0, std::memory_order_relaxed);
    LastDiscoveryExpiry.store(0, std::memory_order_relaxed);
    LastTimerDiscoveryFrame.store(0, std::memory_order_relaxed);
    PrematureTimerRemovals.store(0, std::memory_order_relaxed);
    PrematureTimerRemoveMisses.store(0, std::memory_order_relaxed);
    FramePublishes.store(0, std::memory_order_relaxed);
    FramePumpRuns.store(0, std::memory_order_relaxed);
    FramePumpQueueFailures.store(0, std::memory_order_relaxed);
    TableLoads.store(0, std::memory_order_relaxed);
    TableLoadFailures.store(0, std::memory_order_relaxed);
    LastFlags.store(0, std::memory_order_relaxed);
    LastState.store(0, std::memory_order_relaxed);
    LastSkill.store(0, std::memory_order_relaxed);
    LastSkillLevel.store(0, std::memory_order_relaxed);
    LastExpireFrame.store(0, std::memory_order_relaxed);
    LastCurrentFrame.store(0, std::memory_order_relaxed);
    LastResourceCurrent.store(0, std::memory_order_relaxed);
    LastResourceMaximum.store(0, std::memory_order_relaxed);
    LastTimerRefreshState.store(0, std::memory_order_relaxed);
    LastTimerRefreshSkill.store(0, std::memory_order_relaxed);
    LastTimerRefreshOldExpire.store(0, std::memory_order_relaxed);
    LastTimerRefreshNewExpire.store(0, std::memory_order_relaxed);
    LastTimerRefreshFrame.store(0, std::memory_order_relaxed);
    LastPrematureRemovedState.store(0, std::memory_order_relaxed);
    LastPrematureRemovedSkill.store(0, std::memory_order_relaxed);
    LastPrematureRemovedExpire.store(0, std::memory_order_relaxed);
    LastPrematureRemovedFrame.store(0, std::memory_order_relaxed);
    ClearTimerPresenceRecords();
}

} // namespace

bool Initialize(const D2RL::PluginContext* context) noexcept {
    Shutdown();
    if (context == nullptr || context->exeBase == 0) return false;
    Context = context;
    const auto& services = Core::Services();
    Resources = services.resources;
    CustomTables = services.customTables;
    Threads = services.threads;
    Lifecycle = services.lifecycle;
    if (Resources == nullptr || CustomTables == nullptr || Threads == nullptr || Lifecycle == nullptr
        || !D2RL::HasThreadServiceField(Threads, D2RL::ThreadServiceRequiredSize)
        || !D2RL::HasLifecycleServiceField(Lifecycle, D2RL::LifecycleServiceRequiredSize)) {
        Context->LogError("BuffPanel BuffTracker: required Resource/CustomTable/Thread/Lifecycle service unavailable.");
        Shutdown();
        return false;
    }

    if (!Context->CheckExpectedBytes(
            Native::Contract::GetGameFromUnitRva,
            Native::Contract::GetGameFromUnitExpected.data(),
            static_cast<std::uint32_t>(Native::Contract::GetGameFromUnitExpected.size()))) {
        Context->LogError("BuffPanel BuffTracker: UNITS_GetGame native fingerprint mismatch; refusing whitelist-driven buff tracking.");
        Shutdown();
        return false;
    }
    GetGameFromUnit = reinterpret_cast<GetGameFromUnitFn>(
        context->exeBase + Native::Contract::GetGameFromUnitRva);

    if (!Context->CheckExpectedBytes(
            Native::Contract::GenericCurseStateLookupCallRva,
            Native::Contract::GenericCurseStateLookupCallExpected.data(),
            static_cast<std::uint32_t>(Native::Contract::GenericCurseStateLookupCallExpected.size()))) {
        Context->LogError("BuffPanel BuffTracker: exact-state StatList lookup native fingerprint mismatch; refusing premature timer-removal tracking.");
        Shutdown();
        return false;
    }
    GetStatListFromUnitAndState = reinterpret_cast<GetStatListFromUnitAndStateFn>(
        context->exeBase + Native::Contract::GetStatListFromUnitAndStateRva);

    if (Core::StatReads().RawGetter() == nullptr) {
        Context->LogError("BuffPanel BuffTracker: shared Core unit-stat reader is unavailable; resource-mode BuffHud entries cannot be supported.");
        Shutdown();
        return false;
    }

    if (!Core::StatListPosts().EnsureInstalled(
            Native::Contract::PostStatListRva,
            Native::Contract::PostStatListExpected,
            Native::Contract::PostStatListBridgeSlotRva)
        || !Core::StatListPosts().Register({
            .owner = "buff-tracker.player-buffs",
            .priority = 50,
            .callback = &OnStatListPost,
            .userData = nullptr,
        })) {
        Context->LogError("BuffPanel BuffTracker: failed to register with the shared Core STATLIST_PostStatList bus.");
        Shutdown();
        return false;
    }

    if (!RegisterTable() || !RegisterLifecycle()) {
        Context->LogError("BuffPanel BuffTracker: failed to register buff-hud table/lifecycle listeners.");
        Shutdown();
        return false;
    }

    if (!Context->RegisterConsoleCommand(
            "buff-panel-tracker",
            &TrackerCommand,
            "Show whitelist-driven player BuffHud tracking diagnostics.")) {
        Context->LogWarn("BuffPanel BuffTracker: console command 'buff-panel-tracker' could not be registered.");
    }

    ResetDiagnostics();
    Context->LogInfo(
        "Buff Panel 1.0.6 BuffTracker initialized: whitelisted timer states are discovered from attached native player states once per game frame even without STATLIST_PostStatList; qualified expiry renewals, early removal and resource-mode polling remain supported.");
    return true;
}

void Shutdown() noexcept {
    CurrentSessionGeneration.store(0, std::memory_order_release);
    AuthoritativeGame.store(nullptr, std::memory_order_release);
    AuthoritativePlayer.store(nullptr, std::memory_order_release);
    FramePumpScheduled.store(false, std::memory_order_release);
    Whitelist.store({}, std::memory_order_release);
    BuffHudTable = D2RL::CustomTables::InvalidHandle;
    TableSource = LooseBuffHud::Source::Embedded;
    TableSourcePath.clear();
    DataTablesListener = D2RL::Lifecycle::InvalidHandle;
    GameplayListeners = {};
    ClearTimerPresenceRecords();
    GetGameFromUnit = nullptr;
    GetStatListFromUnitAndState = nullptr;
    Resources = nullptr;
    CustomTables = nullptr;
    Threads = nullptr;
    Lifecycle = nullptr;
    Context = nullptr;
}

} // namespace BuffPanel::Systems::BuffTracker
