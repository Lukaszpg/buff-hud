# Buff Panel 1.0.0 — D2RLoader plugin

A standalone 3×7 temporary-buff panel and countdown tracker.

## What it provides

- 21 reusable display-only buff slots in a 3×7, lower-left-fill panel.
- Timed entries with countdowns based on the game's authoritative 25-frame-per-second clock.
- Resource entries with an optional current/max stat counter (e.g. Bone Armor), independent of time remaining.
- Automatic icon and localized name discovery using game Skills/SkillDesc and original skill icon atlases (Amazon through Warlock and Global).
- Table-driven whitelist from `buff-hud.txt`: `name`, `state_id`, `display_type`, `value_stat`, `max_stat`, `skill_id`, `value_shift`, `enabled`.
- Excludes curses and shared-stash proxy states; built-in semantic checks on native buff metadata.

### Default table

Timed self-skill, shrine, temporary potion and resource candidates are included; auras/permanent passives are recorded with `enabled=0` because they are **not** finite countdowns. A row's presence does **not** guarantee that the game emits the metadata necessary for a HUD countdown. In particular, shrine/potion rows can be enabled but remain invisible if `skill=0`. Absorb pools have independent resource presentation and state disambiguation.

`buff-hud.txt` with tabs:

```text
name\tstate_id\tdisplay_type\tvalue_stat\tmax_stat\tskill_id\tvalue_shift\tenabled
fade\t159\ttimer\t0\t0\t0\t0\t1
bone_armor\t14\tresource\t132\t133\t68\t8\t1
```

The `\t` escapes above indicate tab separators; use actual tabs in the text file. For resource entries, `value_stat` and `max_stat` are live stat IDs, `skill_id` supplies the icon/name, and `value_shift` scales the displayed value.

## Installation

Either head to [releases](https://github.com/Lukaszpg/buff-hud/releases) and download the newest version of the plugin or [build](https://github.com/Lukaszpg/buff-hud/edit/main/README.md#building) it from source.

Copy `d2rl-buff-panel.dll` to **one** of these paths (create the `plugins` directory if necessary):

```text
<Diablo II Resurrected>/d2rloader/plugins/d2rl-buff-panel.dll
<Diablo II Resurrected>/mods/<mod-name>/d2rloader/plugins/d2rl-buff-panel.dll
```

Use the first for a global installation, or the second to scope it to one mod. The default `BuffHudhd.json` and `buff-hud.txt` are embedded during the build; shipping their source copies separately is not required for the default behavior. To change the **embedded** default list/layout, edit the matching file under `runtime-data/`, **reconfigure CMake**, and rebuild. Loose mod-specific overrides are an advanced option; validate resource precedence for the selected loader/mod configuration before documenting them for users.

## Building

Buff Panel is a standalone D2RLoader plugin.

### Requirements

- Windows x64, with a C++20-capable MSVC toolchain and Windows SDK (Visual Studio with **Desktop development with C++** is sufficient).
- CMake 3.29 or newer.
- Official [D2RLoader PluginSDK](https://github.com/D2RLoader/PluginSDK), release **0.2.x**, plugin ABI **4**.
- A D2RLoader runtime supporting plugin ABI 4 and the services used by Buff Panel.

**Compatibility:** Buff Panel 1.0.0's native addresses and byte contracts are qualified only for D2R build **93847**. Building successfully does not qualify it for other game builds. The plugin is not a universal-build release.

### Option A — Build Buff Panel by itself (recommended for distribution)

1. Extract `buff-panel-1.0.0-source.zip`. The project root is `plugins/buff-panel/`, the directory containing `CMakeLists.txt`.
2. Open **Developer PowerShell for Visual Studio** (x64 environment) and change directory to the extracted project root.
3. Download the official PluginSDK into `third_party/PluginSDK` beneath that project root:

   ```powershell
   git clone --depth 1 https://github.com/D2RLoader/PluginSDK.git third_party/PluginSDK
   ```

   The expected header path is `third_party/PluginSDK/include/D2RLPlugin/version.h`. Check it defines `D2RL_SDK_VERSION "0.2.x"` and `D2RL_PLUGIN_ABI_VERSION 4`. Do not replace just `version.h`: the whole SDK must match.

4. Configure and build with the Visual Studio generator:

   ```powershell
   cmake -S . -B build -A x64
   cmake --build build --config Release --target buff_panel --parallel
   ```

   If CMake selects a non-Visual-Studio generator, specify an installed generator with `-G`, e.g. `-G "Visual Studio 17 2022" -A x64` for Visual Studio 2022. A developer using Ninja may instead run `cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release` followed by `cmake --build build --target buff_panel --parallel` from an MSVC developer environment. Do not reuse one build directory when switching generators.

5. Find `d2rl-buff-panel.dll` in the build tree. Visual Studio multi-configuration builds normally place it at `build/Release/d2rl-buff-panel.dll`; Ninja normally places it at `build/d2rl-buff-panel.dll`. The exact output directory can differ if a parent build overrides it.

**Already installed PluginSDK as a CMake package?** Skip cloning when CMake can resolve `find_package(D2RLPlugin 0.2.0 CONFIG)` through `CMAKE_PREFIX_PATH` or `D2RLPlugin_DIR`.

### Option B — Integrate with an existing D2RLoader CMake workspace

Copy the project folder to `<workspace>/plugins/buff-panel/`. Ensure the parent CMake project includes the plugin, for example:

```cmake
add_subdirectory(plugins/buff-panel)
```

If the parent already provides `D2RLPlugin::D2RLPlugin` (or a compatible ABI-4 SDK target), Buff Panel reuses it. Otherwise, place the official PluginSDK at `<workspace>/third_party/PluginSDK`, install it as a CMake package, or use the plugin-local SDK location described in Option A.

From the **workspace root**, run:

```powershell
cmake -S . -B build
cmake --build build --config Release --target buff_panel --parallel
```

For a new workspace using a Visual Studio generator, add `-A x64` to the initial configure command. The parent project may set a different DLL output directory.

### Verify and troubleshoot

Start a compatible game build and inspect D2RLoader logs. While a listed buff is active, use:

```text
buff-panel status
buff-panel-tracker
```

Try Fade (`state_id=159`) or another enabled native timer state. The 1.0.0 expanded catalog contains candidates that still require runtime qualification; see `BUFF-CATALOG-REPORT.md` and `buff-hud-audit.tsv`.

- **CMake cannot find PluginSDK:** verify the location of its `CMakeLists.txt` and `include/D2RLPlugin/version.h`, or set `CMAKE_PREFIX_PATH` for an installed package.
- **`min`/`max` macro errors (`C4003`, `C2589`, `C2059`):** use Buff Panel 1.0.0's included CMake configuration, which defines `NOMINMAX` and `WIN32_LEAN_AND_MEAN` on the plugin target. Reconfigure after modifying CMake.
- **Wrong compiler/generator or architecture:** open a Visual Studio C++ developer shell and select x64. Use a fresh build directory when switching generators.
- **The DLL builds but refuses to initialize:** inspect build qualification / service availability. Do not bypass native-byte safety checks or assume an unrelated D2R build is supported.
- **No buff appears:** confirm `buff-hud.txt` has the state enabled, the corresponding state is actually active, and `buff-panel-tracker` reports successful publication. Not every enabled catalog entry guarantees a usable native timer.

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

- It is not a generic arbitrary-D2R-build plugin: qualified low-level offsets and bridge signatures are used and fail closed on mismatch.
- `buff-panel` owns native patching; do not run it alongside another uncoordinated plugin that changes the same bridge without safe chaining.

## Source provenance

`src/systems/buff_hud/`, `src/systems/buff_tracker/`, the relevant shared display/stat buses, the native qualification constants. The copy was renamed and isolated under `BuffPanel::`, given its own plugin entry point/CMake/resource paths/console commands, and the *standalone-only* read-only getter resolver was added.

## Distributing

Publish the built DLL with the matching version, supported-game-build information, installation instructions, and a license covering the Buff Panel source you are distributing. PluginSDK is a separate dependency with its own license; it is not included in this source archive.
