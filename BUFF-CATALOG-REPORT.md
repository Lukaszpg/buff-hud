# Buff Panel 1.0.0 — catalog report, warnings and errors

## Scope and sources

This catalog is for **D2R build 93847 / 3.3.0 RC1**, including Warlock reference rows, and the **standalone Buff Panel** copied from SoE 0.18.148. It is a documented inventory of identifiable player-useful native states **within the plugin's existing timer/resource model**; it is NOT a proof that every state can render on every character, item-triggered spell, summon, mod or future D2R build.

Community D2R data mirror inspected on 2026-09-19:
- `states.txt` — 232 states, IDs 0..231, including Warlock states 190..217: https://github.com/pinkufairy/D2R-Excel/blob/main/states.txt (blob SHA 724947692a60c05cc73f28a67a25e947c10c339f).
- `skills.txt` — 429 rows, checked player `aurastate`, `auratargetstate`, `passivestate`, `auralencalc` and `aurastat` columns: https://github.com/pinkufairy/D2R-Excel/blob/main/skills.txt (blob SHA e92c871b9a649a9ed72e8f9d62b3f2759ffeff7b).
- `itemstatcost.txt` — verified 132 `bonearmor`, 133 `bonearmormax`, 361 `psychicward`, 362 `psychicwardmax`: https://github.com/pinkufairy/D2R-Excel/blob/main/itemstatcost.txt.

These are publicly available data references, not an official Blizzard guarantee of active runtime state details. No claim is made that all 232 `states.txt` rows are buffs; most are debuffs, internal state, rendering markers, attack-phase markers, summon-only effects, etc.

## Included table totals

- **117 unique cataloged player-related state IDs**; **53 enabled** and **64 intentionally disabled**.
- Enabled: **50 timer candidates** and **3 resource pools**.
- Disabled: **64 explanatory entries** (auras, passives or unqualified states). `enabled=0` is skipped before validation by `BuildWhitelist`.
- Category breakdown:
  - Absorb-pool resource: 3
  - Aura / party-wide non-timed state: 23
  - NPC-applied enchant candidate: 2
  - Permanent passive skill state: 32
  - Shrine: 10
  - Temporary potion effect: 3
  - Timed martial-arts charge: 6
  - Timed self skill / transformation: 31
  - Transient action state: 2
  - Transient/mixed action state: 1
  - Unqualified Warlock protection: 1
  - Unqualified Warlock state: 2
  - Unqualified temporary state: 1


## What actually displays

- A `timer` row is not a duration definition. It only authorizes a native **player** state with matching ID, **nonzero skill ID and level**, no CURSE bit, and a **finite future expiry**. The live skill ID selects the icon and name; `skill_id=0` in timer table rows is mandatory by the plugin parser. The program will not invent durations for auras, shrine effects, potions, or passives.
- A `resource` row requires valid positive current and maximum stats. Bone Armor (14) and Cyclone Armor (151) both use raw stats **132/133**; Psychic Ward (206) uses **361/362**. Skills.txt multiplies pool amounts by **256**, so all three rows use `value_shift=8`.
- 1.0.0 adds **state disambiguation for enabled resource rows that share the same stat pair**, to prevent Bone Armor and Cyclone Armor from both appearing when only one is active. If a single resource row is enabled for a stat pair, legacy stat-only behavior remains. If the native attached-state lookup fails for an overlapping pair, that icon will not appear (safe failure; see test matrix below).
- The HUD has **21 slots**. The catalog contains more enabled entries than display capacity; the plugin chooses its existing priority/order logic and may omit some concurrent effects. This release does not add paging or a 22nd slot.

## Warnings / limitations

1. **Shrine & temporary potion states:** ID mapping is verified from `states.txt` but no player skill/level metadata for their StatLists was proven. Their enabled rows may **never render** under the current strict timer policy (`skill == 0` or `skillLevel == 0` -> reject). This is not a build error; use `buff-panel-tracker` during effects to determine what is emitted.
2. **Auras / spirits / permanent passives:** These are present in `buff-hud.txt` as **disabled** reference rows. Enabling them does not create permanent icons; timer mode requires native expiry. A dedicated `persistent` presentation mode would be a future feature.
3. **Martial-arts charge states:** IDs 122..127 are enabled but current HUD uses `stacks=1` and does **not show actual charges**. Their expiry and icon need live testing.
4. **Warlock:** IDs 198, 200, 202, 208, 211 and resource 206 are mapped from the new skills/states tables. These were **not qualified through an in-game Warlock test** here. Enemy-facing Hex debuffs (199, 201, 217) are deliberately omitted. State 203 Mind Barrier and generic 205 Sigil remain disabled and flagged unqualified.
5. **Native field and version constraint:** build-93847 addresses, member offsets, and bridge contracts are still hard-qualified. A newer D2R build requires reverse engineering/qualification, not bypassing checks.
6. **Resource stat aliases:** Bone Armor and Cyclone Armor share stats 132/133; native states distinguish them only when both are enabled. A rare case with both source skills/states simultaneously present would need further attribution work; both would read the same aggregate pool.
7. **No runtime source here:** The previous 0.1.1 builds and runs per user report, but 1.0.0 is **not** MSVC-compiled or game-tested in this environment.
8. **Not every recognizable state is beneficial:** Curse states, damaging ailments, potion healing/mana pulses, internal pet markers, attack-phase flags and visuals are excluded from the default whitelist. `buff-hud-audit.tsv` records included states and why disabled cases stay disabled.
9. **Loose TXT vs embedded TXT:** Rebuild the DLL to update its compiled-in fallback. If distributing a loose TXT, place it at `data/global/excel/d2rloader/buff-panel/buff-hud.txt` in the correct active mod/resource path and confirm precedence. Check the `ready; enabledDefinitions=` startup log to verify the actual file being read.
10. **Public redistribution:** No standalone source license was granted or added by this task. Choose a suitable license for your original SoE-derived source before publishing, and respect any third-party SDK/asset licenses.
11. **Separate standalone only:** Do not enable standalone Buff Panel and SoE internal BuffHud/Tracker simultaneously. SoE's original source remains untouched.

## Error report

**Table/schema errors found: 0** in the generated 1.0.0 catalog (unique valid state IDs, 8 exact columns, valid row modes, resource stat IDs and skill IDs, no resource/timer field violations, version metadata coherence). This is *static validation only*; it does not certify the runtime rows or produce a Windows binary.

## In-game acceptance tests

1. Load standalone 1.0.0 **without SoE's internal Buff HUD**; confirm logs say `enabledDefinitions=53` and no `buff-hud.txt rejected`.
2. Cast Fade (159), Venom (31), Shiver Armor (88), Shout (26): confirm icons + correct countdown. Re-cast Fade/Venom: countdown refreshes.
3. Cast **Bone Armor (14)** and **Cyclone Armor (151)** on suitable characters (charges/Oskills may be needed to make both available): confirm exactly one matching resource icon, current/max updates, and removal at zero. Cast Warlock Psychic Ward (206): verify stats 361/362 and `value_shift=8` produce expected display.
4. Activate a shrine and an Antidote/Thawing potion; if no icon, inspect `buff-panel-tracker` `invalid` / `noExpiry` / `last state/skill` rather than weakening the filter.
5. Test martial-arts skills and Warlock Hex self states. Verify no enemy debuff icons. Saturate the HUD with >21 simultaneous buffs and confirm clean capped behavior.
6. Check SoE independently; it was **not** modified or rebuilt for this release.
