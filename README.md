# Buff Panel 1.0.0 — D2RLoader plugin

A standalone 3×7 temporary-buff panel and countdown tracker extracted **by copying**, not moving, the BuffHud and BuffTracker systems from Sanctuary of Exile (SoE) 0.18.148. It does not need SoE's Maps, Flasks, Worldstone tree, item tooltip system, or other SoE gameplay features.

**Windows build compatibility:** Windows `min`/`max` macros are disabled for every Buff Panel C++ translation unit via the target-local `NOMINMAX` definition. This also fixes parsing of the official PluginSDK 0.2.x `item.h`; do not modify PluginSDK or SoE to work around it. CMake reconfiguration is required for the changed compile definitions.

**Release status:** The user confirmed that standalone 0.1.1 builds and runs. The expanded 1.0.0 table and resource-pool disambiguation still need Windows compilation and in-game testing. Version 1.0.0 belongs to standalone Buff Panel, not SoE.

## Requirements

- Windows x64, D2R build **93847** (D2R 3.3.0 RC 1) — the native RVAs, widget frame offsets, and byte signatures in the copied implementation are qualified for this build only.
- D2RLoader 1.3.x with Plugin ABI 4 and the Resource, Panel, CustomTable, Lifecycle, Thread, Widget, DataTable, and Localization services. Localization is optional for display; names are retried when tables load.
- Official PluginSDK **0.2.x / ABI 4** to compile; CMake 3.29+ and MSVC.
- For an unrelated D2R build or older D2RLoader, **do not remove the native fingerprint checks**. Requalify the addresses and contracts instead.

## Build in RuffnecKk D2RLoader Suite

Place the included `plugins/buff-panel` folder in:

```text
D:\RuffnecKk-D2RLoader-Suite\plugins\buff-panel
```

Ensure the Suite's top-level CMake registers `buff-panel` in `RUFFNECKK_SUITE_PLUGIN_DIRECTORIES` (if it does not already automatically discover the new plugin). Then:

```powershell
cd D:\RuffnecKk-D2RLoader-Suite
cmake -S . -B build
cmake --build build --config Debug --target buff_panel --parallel
```

Compiled DLL: `build/plugins/buff-panel/Debug/d2rl-buff-panel.dll` (subject to Suite output-dir overrides). You may also point Charsi at the included `plugins/buff-panel` source package after registering the plugin.

For a standalone build outside the Suite, install the official SDK as a CMake package or place its source at `plugins/buff-panel/third_party/PluginSDK`, then run CMake on `plugins/buff-panel`.

## Install for users

Copy the **compiled** `d2rl-buff-panel.dll` into one of these locations:

```text
<Diablo II Resurrected>\d2rloader\plugins\d2rl-buff-panel.dll
<Diablo II Resurrected>\mods\<mod-name>\d2rloader\plugins\d2rl-buff-panel.dll
```

The layout and `buff-hud.txt` are also embedded in the DLL. The two loose files under `runtime-data` are editable authoring/default files, not an additional required DLL dependency. When shipping a customizable mod, place an appropriately modified `buff-hud.txt` in that mod's `data/global/excel/d2rloader/buff-panel/` resource path and test its precedence under your D2RLoader configuration. Edit the **loose** table when present; the compiled fallback is updated by rebuilding the source.

**Do not load this DLL alongside SoE with its internal BuffHud/BuffTracker enabled.** Both need native StatList-post observation and would create two buff panels. This extraction is intended for non-SoE installations; SoE's original system remains in SoE.

## What it provides

- 21 reusable display-only buff slots in a 3×7, lower-left-fill panel.
- Timed entries with countdowns based on the game's authoritative 25-frame-per-second clock.
- Resource entries with an optional current/max stat counter (e.g. Bone Armor), independent of time remaining.
- Automatic icon and localized name discovery using game Skills/SkillDesc and original skill icon atlases (Amazon through Warlock and Global).
- Table-driven whitelist from `buff-hud.txt`: `name`, `state_id`, `display_type`, `value_stat`, `max_stat`, `skill_id`, `value_shift`, `enabled`.
- Excludes curses and shared-stash proxy states; built-in semantic checks on native buff metadata.
- A read-only GetUnitStat bridge resolver (no extra stat-read hook); qualified chain on STATLIST_PostStatList for buff posting, plus attached-state discovery and expiry refresh as present in SoE 0.18.148.

### Default table

The **1.0.0 catalog** expands the SoE 0.18.148 sample table using D2R `states.txt`, `skills.txt`, and `itemstatcost.txt` reference data (see `BUFF-CATALOG-REPORT.md` and `buff-hud-audit.tsv`). Timed self-skill, shrine, temporary potion and resource candidates are included; auras/permanent passives are recorded with `enabled=0` because they are **not** finite countdowns. A row's presence does **not** guarantee that the game emits the metadata necessary for a HUD countdown. In particular, shrine/potion rows can be enabled but remain invisible if `skill=0`. Absorb pools have independent resource presentation and state disambiguation.

`buff-hud.txt` with tabs:

```text
name\tstate_id\tdisplay_type\tvalue_stat\tmax_stat\tskill_id\tvalue_shift\tenabled
fade\t159\ttimer\t0\t0\t0\t0\t1
bone_armor\t14\tresource\t132\t133\t68\t8\t1
```

The `\t` escapes above indicate tab separators; use actual tabs in the text file. For resource entries, `value_stat` and `max_stat` are live stat IDs, `skill_id` supplies the icon/name, and `value_shift` scales the displayed value according to the copied SoE logic.

## Commands

```text
buff-panel status
buff-panel test 15 3
buff-panel clear
buff-panel rebuild-icons
buff-panel-tracker
buff-panel-status
```

Test while a configured buff is **currently active**. `buff-panel-tracker` reports whether the whitelist loaded, whether a timed state was discovered, and whether its live expiry was published; `buff-panel status` reports currently displayed entries and the HUD frame clock.

## Known limitations / verification

- The 1.0.0 source build/runtime is not tested here (the working environment lacks Windows/MSVC/D2RLoader runtime); user confirmed 0.1.1 works. Validate 1.0.0 on a test profile before publishing.
- Fade and timer discovery come from SoE 0.18.148 and were copied; test Fade reapplication. The 1.0.0 resource alias guard is a standalone-only modification.
- It is not a generic arbitrary-D2R-build plugin: qualified low-level offsets and bridge signatures are used and fail closed on mismatch.
- `buff-panel` owns native patching; do not run it alongside another uncoordinated plugin that changes the same bridge without safe chaining. It is not a hot-swappable runtime replacement for SoE's internal buff system.
- No license for these *extracted SoE sources* is asserted by this archive; choose and add your intended distribution license before publishing. D2RLoader PluginSDK itself is a separate dependency with its own terms and is not bundled here.

## Source provenance

`src/systems/buff_hud/`, `src/systems/buff_tracker/`, the relevant shared display/stat buses, the native qualification constants, and the two default assets were **copied** from SoE 0.18.148. The copy was renamed and isolated under `BuffPanel::`, given its own plugin entry point/CMake/resource paths/console commands, and the *standalone-only* read-only getter resolver was added. No original SoE module was removed or overwritten.

See `EXTRACTION.md` for the exact source-file map.

## Buff catalog 1.0.0

See `BUFF-CATALOG-REPORT.md` for counts, qualification warnings and test steps. `buff-hud-audit.tsv` records every included state, game-ID mapping, on/off choice and caveat. The **enabled=0 aura and passive records are documentation**, not working aura widgets. `skill_id` is intentionally zero on every timer row because the timer's actual skill/icon comes from native StatList metadata, *not* that table column.
