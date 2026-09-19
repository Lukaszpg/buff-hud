# Extraction audit — Buff Panel 1.0.0

Source baseline: `soe-0.18.148-charsi.zip` / `plugins/soe/`. Original source remains separate and unchanged.

| Copied modules | Standalone modules |
| --- | --- |
| `src/systems/buff_hud/{buff_hud,icon_frame_backend,skill_icon_resolver}.*` | Same relative paths under `plugins/buff-panel` |
| `src/systems/buff_tracker/buff_tracker.*` | Same relative paths under `plugins/buff-panel` |
| `src/core/buff_display_bus.*` | Same relative paths |
| `src/core/stat_list_post_bus.*` | Same relative paths |
| `src/core/stat_read_bus.*` | Same relative paths; extended with non-hooking raw getter bridge resolver |
| `src/core/{services,hook_registry}.*`, `src/core/rip_indirect_bridge.hpp` | Same relative paths; standalone copy |
| `src/native/native_contract.hpp` | **Only the 49 required build-93847 constants** from SoE (including GetUnitStat); no unrelated map/curse/flask addresses |
| `runtime-data/data/global/excel/d2rloader/soe/buff-hud.txt` | `runtime-data/data/global/excel/d2rloader/buff-panel/buff-hud.txt` |
| `runtime-data/data/global/ui/layouts/soe/BuffHudhd.json` | `runtime-data/data/global/ui/layouts/buff-panel/BuffHudhd.json` |

New `src/plugin.cpp`, `src/plugin.rc`, and `CMakeLists.txt` are standalone wrappers, not SoE core entry points. Other SoE systems are intentionally not present.

Adaptations: `SoE::` -> `BuffPanel::`, independent panel ID/resource paths, diagnostics and commands namespaced to `buff-panel`, plugin ID `buff-panel`, output `d2rl-buff-panel.dll`, version 1.0.0, global-capable plugin flags, read-only raw stat getter resolver, and self-contained embedded default JSON/TXT generation.

Original SoE version remains 0.18.148. Do not remove the original files to install this package.

## 0.1.1 build compatibility
The Windows build log for 0.1.0 showed C4003/C2589 originating in `PluginSDK/include/D2RLPlugin/item.h` after including `Windows.h`, followed by errors in `rip_indirect_bridge.hpp` and `buff_hud.cpp`. This is the Windows `min`/`max` macro collision. The `buff_panel` CMake target now defines `NOMINMAX` and `WIN32_LEAN_AND_MEAN` on Windows. SoE sources are not touched.
