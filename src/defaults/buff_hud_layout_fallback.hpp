#pragma once

namespace BuffPanel::Systems::BuffHud::Internal {

// Generated as adjacent raw-string literals. Keep each source literal below
// MSVC's pre-concatenation C2026 limit; the compiler joins them into one array.
inline constexpr char BuffHudLayout[] =
R"buffpaneljson({
  "type": "Panel",
  "name": "buff-panel/BuffHud",
  "fields": {
    "anchor": {
      "x": 0.5,
      "y": 1.0
    },
    "priority": 101
  },
  "children": [
    {
      "type": "Widget",
      "name": "BuffGrid",
      "fields": {
        "rect": {
          "x": 50,
          "y": -515,
          "width": 780,
          "height": 300
        }
      },
      "children": [
        {
          "type": "Widget",
          "name": "BuffSlot00",
          "fields": {
            "visible": false,
            "rect": {
              "x": 0,
              "y": 210,
              "width": 96,
              "height": 96
            }
          },
          "children": [
            {
              "type": "ButtonWidget",
              "name": "IconAmazon",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/amazon/AmSkillicon",
                "normalFrame": 0,
                "pressedFrame": 1,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconSorceress",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/Sorceress/SoSkillicon",
                "normalFrame": 0,
                "pressedFrame": 1,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconNecromancer",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/Necromancer/NeSkillicon",
                "normalFrame": 0,
                "pressedFrame": 1,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconPaladin",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/paladin/PaSkillicon",
                "normalFrame": 0,
                "pressedFrame": 1,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconBarbarian",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/Barbarian/BaSkillicon",
                "normalFrame": 0,
                "pressedFrame": 1,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconDruid",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/Druid/DrSkillicon",
                "normalFrame": 0,
                "pressedFrame": 1,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconAssassin",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/Assassin/AsSkillicon",
                "normalFrame": 0,
                "pressedFrame": 1,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "na)buffpaneljson"
R"buffpaneljson(me": "IconWarlock",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/Warlock/WaSkillicon",
                "normalFrame": 0,
                "pressedFrame": 1,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconGlobal",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/SubMenu/Skillicon",
                "normalFrame": 0,
                "pressedFrame": 1,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "TextBoxWidget",
              "name": "Countdown",
              "fields": {
                "visible": false,
                "text": "__BUFF_PANEL_TIMER_RESERVE_00__",
                "fontType": "16pt",
                "rect": {
                  "x": -16,
                  "y": 52,
                  "width": 112,
                  "height": 42
                },
                "style": {
                  "pointSize": "$MediumFontSize",
                  "fontColor": "$FontColorWhite",
                  "alignment": {
                    "h": "center",
                    "v": "center"
                  },
                  "dropShadow": "$DefaultDropShadow",
                  "options": {
                    "hideOverflow": true
                  }
                }
              }
            },
            {
              "type": "FocusableWidget",
              "name": "Tooltip",
              "fields": {
                "rect": {
                  "x": 0,
                  "y": 0,
                  "width": 96,
                  "height": 96
                },
                "tooltipString": "​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​",
                "tooltipStyle": "$TooltipStyle"
              }
            }
          ]
        },
        {
          "type": "Widget",
          "name": "BuffSlot01",
          "fields": {
            "visible": false,
            "rect": {
              "x": 112,
              "y": 210,
              "width": 96,
              "height": 96
            }
          },
          "children": [
            {
              "type": "ButtonWidget",
              "name": "IconAmazon",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/amazon/AmSkillicon",
                "normalFrame": 2,
                "pressedFrame": 3,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconSorceress",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/Sorceress/SoSkillicon",
                "normalFrame": 2,
                "pressedFrame": 3,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconNecromancer",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/Necromancer/NeSkillicon",
                "normalFrame": 2,
                "pressedFrame": 3,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconPaladin",)buffpaneljson"
R"buffpaneljson(
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/paladin/PaSkillicon",
                "normalFrame": 2,
                "pressedFrame": 3,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconBarbarian",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/Barbarian/BaSkillicon",
                "normalFrame": 2,
                "pressedFrame": 3,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconDruid",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/Druid/DrSkillicon",
                "normalFrame": 2,
                "pressedFrame": 3,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconAssassin",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/Assassin/AsSkillicon",
                "normalFrame": 2,
                "pressedFrame": 3,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconWarlock",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/Warlock/WaSkillicon",
                "normalFrame": 2,
                "pressedFrame": 3,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconGlobal",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/SubMenu/Skillicon",
                "normalFrame": 2,
                "pressedFrame": 3,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "TextBoxWidget",
              "name": "Countdown",
              "fields": {
                "visible": false,
                "text": "__BUFF_PANEL_TIMER_RESERVE_00__",
                "fontType": "16pt",
                "rect": {
                  "x": -16,
                  "y": 52,
                  "width": 112,
                  "height": 42
                },
                "style": {
                  "pointSize": "$MediumFontSize",
                  "fontColor": "$FontColorWhite",
                  "alignment": {
                    "h": "center",
                    "v": "center"
                  },
                  "dropShadow": "$DefaultDropShadow",
                  "options": {
                    "hideOverflow": true
                  }
                }
              }
            },
            {
              "type": "FocusableWidget",
              "name": "Tooltip",
              "fields": {
                "rect": {
                  "x": 0,
                  "y": 0,
                  "width": 96,
                  "height": 96
                },
                "tooltipString": "‌​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​",
                "tooltipStyle": "$To)buffpaneljson"
R"buffpaneljson(oltipStyle"
              }
            }
          ]
        },
        {
          "type": "Widget",
          "name": "BuffSlot02",
          "fields": {
            "visible": false,
            "rect": {
              "x": 224,
              "y": 210,
              "width": 96,
              "height": 96
            }
          },
          "children": [
            {
              "type": "ButtonWidget",
              "name": "IconAmazon",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/amazon/AmSkillicon",
                "normalFrame": 4,
                "pressedFrame": 5,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconSorceress",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/Sorceress/SoSkillicon",
                "normalFrame": 4,
                "pressedFrame": 5,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconNecromancer",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/Necromancer/NeSkillicon",
                "normalFrame": 4,
                "pressedFrame": 5,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconPaladin",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/paladin/PaSkillicon",
                "normalFrame": 4,
                "pressedFrame": 5,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconBarbarian",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/Barbarian/BaSkillicon",
                "normalFrame": 4,
                "pressedFrame": 5,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconDruid",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/Druid/DrSkillicon",
                "normalFrame": 4,
                "pressedFrame": 5,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconAssassin",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/Assassin/AsSkillicon",
                "normalFrame": 4,
                "pressedFrame": 5,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconWarlock",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/Warlock/WaSkillicon",
                "normalFrame": 4,
              )buffpaneljson"
R"buffpaneljson(  "pressedFrame": 5,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconGlobal",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/SubMenu/Skillicon",
                "normalFrame": 4,
                "pressedFrame": 5,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "TextBoxWidget",
              "name": "Countdown",
              "fields": {
                "visible": false,
                "text": "__BUFF_PANEL_TIMER_RESERVE_00__",
                "fontType": "16pt",
                "rect": {
                  "x": -16,
                  "y": 52,
                  "width": 112,
                  "height": 42
                },
                "style": {
                  "pointSize": "$MediumFontSize",
                  "fontColor": "$FontColorWhite",
                  "alignment": {
                    "h": "center",
                    "v": "center"
                  },
                  "dropShadow": "$DefaultDropShadow",
                  "options": {
                    "hideOverflow": true
                  }
                }
              }
            },
            {
              "type": "FocusableWidget",
              "name": "Tooltip",
              "fields": {
                "rect": {
                  "x": 0,
                  "y": 0,
                  "width": 96,
                  "height": 96
                },
                "tooltipString": "​‌​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​",
                "tooltipStyle": "$TooltipStyle"
              }
            }
          ]
        },
        {
          "type": "Widget",
          "name": "BuffSlot03",
          "fields": {
            "visible": false,
            "rect": {
              "x": 336,
              "y": 210,
              "width": 96,
              "height": 96
            }
          },
          "children": [
            {
              "type": "ButtonWidget",
              "name": "IconAmazon",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/amazon/AmSkillicon",
                "normalFrame": 6,
                "pressedFrame": 7,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconSorceress",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/Sorceress/SoSkillicon",
                "normalFrame": 6,
                "pressedFrame": 7,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconNecromancer",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/Necromancer/NeSkillicon",
                "normalFrame": 6,
                "pressedFrame": 7,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconPaladin",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/paladin/PaSkillicon",
                "normalFrame": 6,
                "pressedFrame": 7)buffpaneljson"
R"buffpaneljson(,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconBarbarian",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/Barbarian/BaSkillicon",
                "normalFrame": 6,
                "pressedFrame": 7,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconDruid",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/Druid/DrSkillicon",
                "normalFrame": 6,
                "pressedFrame": 7,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconAssassin",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/Assassin/AsSkillicon",
                "normalFrame": 6,
                "pressedFrame": 7,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconWarlock",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/Warlock/WaSkillicon",
                "normalFrame": 6,
                "pressedFrame": 7,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconGlobal",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/SubMenu/Skillicon",
                "normalFrame": 6,
                "pressedFrame": 7,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "TextBoxWidget",
              "name": "Countdown",
              "fields": {
                "visible": false,
                "text": "__BUFF_PANEL_TIMER_RESERVE_00__",
                "fontType": "16pt",
                "rect": {
                  "x": -16,
                  "y": 52,
                  "width": 112,
                  "height": 42
                },
                "style": {
                  "pointSize": "$MediumFontSize",
                  "fontColor": "$FontColorWhite",
                  "alignment": {
                    "h": "center",
                    "v": "center"
                  },
                  "dropShadow": "$DefaultDropShadow",
                  "options": {
                    "hideOverflow": true
                  }
                }
              }
            },
            {
              "type": "FocusableWidget",
              "name": "Tooltip",
              "fields": {
                "rect": {
                  "x": 0,
                  "y": 0,
                  "width": 96,
                  "height": 96
                },
                "tooltipString": "‌‌​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​",
                "tooltipStyle": "$TooltipStyle"
              }
            }
          ]
        },
        {
          "type": "Widget",
          "name": "BuffSlot04",
          "fields": {
            "visible": false,
            "rect": {
              "x": 448,
              "y": 210,
              "width": 96,
              "height": 96
   )buffpaneljson"
R"buffpaneljson(         }
          },
          "children": [
            {
              "type": "ButtonWidget",
              "name": "IconAmazon",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/amazon/AmSkillicon",
                "normalFrame": 8,
                "pressedFrame": 9,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconSorceress",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/Sorceress/SoSkillicon",
                "normalFrame": 8,
                "pressedFrame": 9,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconNecromancer",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/Necromancer/NeSkillicon",
                "normalFrame": 8,
                "pressedFrame": 9,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconPaladin",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/paladin/PaSkillicon",
                "normalFrame": 8,
                "pressedFrame": 9,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconBarbarian",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/Barbarian/BaSkillicon",
                "normalFrame": 8,
                "pressedFrame": 9,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconDruid",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/Druid/DrSkillicon",
                "normalFrame": 8,
                "pressedFrame": 9,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconAssassin",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/Assassin/AsSkillicon",
                "normalFrame": 8,
                "pressedFrame": 9,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconWarlock",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/Warlock/WaSkillicon",
                "normalFrame": 8,
                "pressedFrame": 9,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconGlobal",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y)buffpaneljson"
R"buffpaneljson(": 0,
                  "scale": 0.6
                },
                "filename": "Spells/SubMenu/Skillicon",
                "normalFrame": 8,
                "pressedFrame": 9,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "TextBoxWidget",
              "name": "Countdown",
              "fields": {
                "visible": false,
                "text": "__BUFF_PANEL_TIMER_RESERVE_00__",
                "fontType": "16pt",
                "rect": {
                  "x": -16,
                  "y": 52,
                  "width": 112,
                  "height": 42
                },
                "style": {
                  "pointSize": "$MediumFontSize",
                  "fontColor": "$FontColorWhite",
                  "alignment": {
                    "h": "center",
                    "v": "center"
                  },
                  "dropShadow": "$DefaultDropShadow",
                  "options": {
                    "hideOverflow": true
                  }
                }
              }
            },
            {
              "type": "FocusableWidget",
              "name": "Tooltip",
              "fields": {
                "rect": {
                  "x": 0,
                  "y": 0,
                  "width": 96,
                  "height": 96
                },
                "tooltipString": "​​‌​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​",
                "tooltipStyle": "$TooltipStyle"
              }
            }
          ]
        },
        {
          "type": "Widget",
          "name": "BuffSlot05",
          "fields": {
            "visible": false,
            "rect": {
              "x": 560,
              "y": 210,
              "width": 96,
              "height": 96
            }
          },
          "children": [
            {
              "type": "ButtonWidget",
              "name": "IconAmazon",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/amazon/AmSkillicon",
                "normalFrame": 10,
                "pressedFrame": 11,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconSorceress",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/Sorceress/SoSkillicon",
                "normalFrame": 10,
                "pressedFrame": 11,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconNecromancer",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/Necromancer/NeSkillicon",
                "normalFrame": 10,
                "pressedFrame": 11,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconPaladin",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/paladin/PaSkillicon",
                "normalFrame": 10,
                "pressedFrame": 11,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconBarbarian",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
  )buffpaneljson"
R"buffpaneljson(                "scale": 0.6
                },
                "filename": "Spells/Barbarian/BaSkillicon",
                "normalFrame": 10,
                "pressedFrame": 11,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconDruid",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/Druid/DrSkillicon",
                "normalFrame": 10,
                "pressedFrame": 11,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconAssassin",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/Assassin/AsSkillicon",
                "normalFrame": 10,
                "pressedFrame": 11,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconWarlock",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/Warlock/WaSkillicon",
                "normalFrame": 10,
                "pressedFrame": 11,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconGlobal",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/SubMenu/Skillicon",
                "normalFrame": 10,
                "pressedFrame": 11,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "TextBoxWidget",
              "name": "Countdown",
              "fields": {
                "visible": false,
                "text": "__BUFF_PANEL_TIMER_RESERVE_00__",
                "fontType": "16pt",
                "rect": {
                  "x": -16,
                  "y": 52,
                  "width": 112,
                  "height": 42
                },
                "style": {
                  "pointSize": "$MediumFontSize",
                  "fontColor": "$FontColorWhite",
                  "alignment": {
                    "h": "center",
                    "v": "center"
                  },
                  "dropShadow": "$DefaultDropShadow",
                  "options": {
                    "hideOverflow": true
                  }
                }
              }
            },
            {
              "type": "FocusableWidget",
              "name": "Tooltip",
              "fields": {
                "rect": {
                  "x": 0,
                  "y": 0,
                  "width": 96,
                  "height": 96
                },
                "tooltipString": "‌​‌​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​",
                "tooltipStyle": "$TooltipStyle"
              }
            }
          ]
        },
        {
          "type": "Widget",
          "name": "BuffSlot06",
          "fields": {
            "visible": false,
            "rect": {
              "x": 672,
              "y": 210,
              "width": 96,
              "height": 96
            }
          },
          "children": [
            {
              "type": "ButtonWidget",
              "name": "IconAmazon",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "sca)buffpaneljson"
R"buffpaneljson(le": 0.6
                },
                "filename": "Spells/amazon/AmSkillicon",
                "normalFrame": 12,
                "pressedFrame": 13,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconSorceress",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/Sorceress/SoSkillicon",
                "normalFrame": 12,
                "pressedFrame": 13,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconNecromancer",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/Necromancer/NeSkillicon",
                "normalFrame": 12,
                "pressedFrame": 13,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconPaladin",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/paladin/PaSkillicon",
                "normalFrame": 12,
                "pressedFrame": 13,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconBarbarian",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/Barbarian/BaSkillicon",
                "normalFrame": 12,
                "pressedFrame": 13,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconDruid",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/Druid/DrSkillicon",
                "normalFrame": 12,
                "pressedFrame": 13,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconAssassin",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/Assassin/AsSkillicon",
                "normalFrame": 12,
                "pressedFrame": 13,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconWarlock",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/Warlock/WaSkillicon",
                "normalFrame": 12,
                "pressedFrame": 13,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconGlobal",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/SubMenu/Skillicon",
                "normalFrame": 12,
                "pressedFrame": 13,
                "focusOnMouseOver": false
              }
            },
            {
          )buffpaneljson"
R"buffpaneljson(    "type": "TextBoxWidget",
              "name": "Countdown",
              "fields": {
                "visible": false,
                "text": "__BUFF_PANEL_TIMER_RESERVE_00__",
                "fontType": "16pt",
                "rect": {
                  "x": -16,
                  "y": 52,
                  "width": 112,
                  "height": 42
                },
                "style": {
                  "pointSize": "$MediumFontSize",
                  "fontColor": "$FontColorWhite",
                  "alignment": {
                    "h": "center",
                    "v": "center"
                  },
                  "dropShadow": "$DefaultDropShadow",
                  "options": {
                    "hideOverflow": true
                  }
                }
              }
            },
            {
              "type": "FocusableWidget",
              "name": "Tooltip",
              "fields": {
                "rect": {
                  "x": 0,
                  "y": 0,
                  "width": 96,
                  "height": 96
                },
                "tooltipString": "​‌‌​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​",
                "tooltipStyle": "$TooltipStyle"
              }
            }
          ]
        },
        {
          "type": "Widget",
          "name": "BuffSlot07",
          "fields": {
            "visible": false,
            "rect": {
              "x": 0,
              "y": 105,
              "width": 96,
              "height": 96
            }
          },
          "children": [
            {
              "type": "ButtonWidget",
              "name": "IconAmazon",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/amazon/AmSkillicon",
                "normalFrame": 14,
                "pressedFrame": 15,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconSorceress",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/Sorceress/SoSkillicon",
                "normalFrame": 14,
                "pressedFrame": 15,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconNecromancer",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/Necromancer/NeSkillicon",
                "normalFrame": 14,
                "pressedFrame": 15,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconPaladin",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/paladin/PaSkillicon",
                "normalFrame": 14,
                "pressedFrame": 15,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconBarbarian",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/Barbarian/BaSkillicon",
                "normalFrame": 14,
                "pressedFrame": 15,
                "focusOnMouseOver": false
              }
            },
            {
              )buffpaneljson"
R"buffpaneljson("type": "ButtonWidget",
              "name": "IconDruid",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/Druid/DrSkillicon",
                "normalFrame": 14,
                "pressedFrame": 15,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconAssassin",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/Assassin/AsSkillicon",
                "normalFrame": 14,
                "pressedFrame": 15,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconWarlock",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/Warlock/WaSkillicon",
                "normalFrame": 14,
                "pressedFrame": 15,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconGlobal",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/SubMenu/Skillicon",
                "normalFrame": 14,
                "pressedFrame": 15,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "TextBoxWidget",
              "name": "Countdown",
              "fields": {
                "visible": false,
                "text": "__BUFF_PANEL_TIMER_RESERVE_00__",
                "fontType": "16pt",
                "rect": {
                  "x": -16,
                  "y": 52,
                  "width": 112,
                  "height": 42
                },
                "style": {
                  "pointSize": "$MediumFontSize",
                  "fontColor": "$FontColorWhite",
                  "alignment": {
                    "h": "center",
                    "v": "center"
                  },
                  "dropShadow": "$DefaultDropShadow",
                  "options": {
                    "hideOverflow": true
                  }
                }
              }
            },
            {
              "type": "FocusableWidget",
              "name": "Tooltip",
              "fields": {
                "rect": {
                  "x": 0,
                  "y": 0,
                  "width": 96,
                  "height": 96
                },
                "tooltipString": "‌‌‌​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​",
                "tooltipStyle": "$TooltipStyle"
              }
            }
          ]
        },
        {
          "type": "Widget",
          "name": "BuffSlot08",
          "fields": {
            "visible": false,
            "rect": {
              "x": 112,
              "y": 105,
              "width": 96,
              "height": 96
            }
          },
          "children": [
            {
              "type": "ButtonWidget",
              "name": "IconAmazon",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/amazon/AmSkillicon",
                "normalFrame": 16,
                "pressedFrame": 17,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",)buffpaneljson"
R"buffpaneljson(
              "name": "IconSorceress",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/Sorceress/SoSkillicon",
                "normalFrame": 16,
                "pressedFrame": 17,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconNecromancer",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/Necromancer/NeSkillicon",
                "normalFrame": 16,
                "pressedFrame": 17,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconPaladin",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/paladin/PaSkillicon",
                "normalFrame": 16,
                "pressedFrame": 17,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconBarbarian",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/Barbarian/BaSkillicon",
                "normalFrame": 16,
                "pressedFrame": 17,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconDruid",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/Druid/DrSkillicon",
                "normalFrame": 16,
                "pressedFrame": 17,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconAssassin",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/Assassin/AsSkillicon",
                "normalFrame": 16,
                "pressedFrame": 17,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconWarlock",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/Warlock/WaSkillicon",
                "normalFrame": 16,
                "pressedFrame": 17,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconGlobal",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/SubMenu/Skillicon",
                "normalFrame": 16,
                "pressedFrame": 17,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "TextBoxWidget",
              "name": "Countdown",
              "fields": {
                "visible": false,
                "text": "__BUFF_PANEL_TIMER_RESERVE_00__",
                "fontType": "16pt",
                "rect": {
                  "x": -16,
       )buffpaneljson"
R"buffpaneljson(           "y": 52,
                  "width": 112,
                  "height": 42
                },
                "style": {
                  "pointSize": "$MediumFontSize",
                  "fontColor": "$FontColorWhite",
                  "alignment": {
                    "h": "center",
                    "v": "center"
                  },
                  "dropShadow": "$DefaultDropShadow",
                  "options": {
                    "hideOverflow": true
                  }
                }
              }
            },
            {
              "type": "FocusableWidget",
              "name": "Tooltip",
              "fields": {
                "rect": {
                  "x": 0,
                  "y": 0,
                  "width": 96,
                  "height": 96
                },
                "tooltipString": "​​​‌​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​",
                "tooltipStyle": "$TooltipStyle"
              }
            }
          ]
        },
        {
          "type": "Widget",
          "name": "BuffSlot09",
          "fields": {
            "visible": false,
            "rect": {
              "x": 224,
              "y": 105,
              "width": 96,
              "height": 96
            }
          },
          "children": [
            {
              "type": "ButtonWidget",
              "name": "IconAmazon",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/amazon/AmSkillicon",
                "normalFrame": 18,
                "pressedFrame": 19,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconSorceress",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/Sorceress/SoSkillicon",
                "normalFrame": 18,
                "pressedFrame": 19,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconNecromancer",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/Necromancer/NeSkillicon",
                "normalFrame": 18,
                "pressedFrame": 19,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconPaladin",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/paladin/PaSkillicon",
                "normalFrame": 18,
                "pressedFrame": 19,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconBarbarian",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/Barbarian/BaSkillicon",
                "normalFrame": 18,
                "pressedFrame": 19,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconDruid",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Sp)buffpaneljson"
R"buffpaneljson(ells/Druid/DrSkillicon",
                "normalFrame": 18,
                "pressedFrame": 19,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconAssassin",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/Assassin/AsSkillicon",
                "normalFrame": 18,
                "pressedFrame": 19,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconWarlock",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/Warlock/WaSkillicon",
                "normalFrame": 18,
                "pressedFrame": 19,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconGlobal",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/SubMenu/Skillicon",
                "normalFrame": 18,
                "pressedFrame": 19,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "TextBoxWidget",
              "name": "Countdown",
              "fields": {
                "visible": false,
                "text": "__BUFF_PANEL_TIMER_RESERVE_00__",
                "fontType": "16pt",
                "rect": {
                  "x": -16,
                  "y": 52,
                  "width": 112,
                  "height": 42
                },
                "style": {
                  "pointSize": "$MediumFontSize",
                  "fontColor": "$FontColorWhite",
                  "alignment": {
                    "h": "center",
                    "v": "center"
                  },
                  "dropShadow": "$DefaultDropShadow",
                  "options": {
                    "hideOverflow": true
                  }
                }
              }
            },
            {
              "type": "FocusableWidget",
              "name": "Tooltip",
              "fields": {
                "rect": {
                  "x": 0,
                  "y": 0,
                  "width": 96,
                  "height": 96
                },
                "tooltipString": "‌​​‌​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​",
                "tooltipStyle": "$TooltipStyle"
              }
            }
          ]
        },
        {
          "type": "Widget",
          "name": "BuffSlot10",
          "fields": {
            "visible": false,
            "rect": {
              "x": 336,
              "y": 105,
              "width": 96,
              "height": 96
            }
          },
          "children": [
            {
              "type": "ButtonWidget",
              "name": "IconAmazon",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/amazon/AmSkillicon",
                "normalFrame": 20,
                "pressedFrame": 21,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconSorceress",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/Sorceress/SoSk)buffpaneljson"
R"buffpaneljson(illicon",
                "normalFrame": 20,
                "pressedFrame": 21,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconNecromancer",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/Necromancer/NeSkillicon",
                "normalFrame": 20,
                "pressedFrame": 21,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconPaladin",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/paladin/PaSkillicon",
                "normalFrame": 20,
                "pressedFrame": 21,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconBarbarian",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/Barbarian/BaSkillicon",
                "normalFrame": 20,
                "pressedFrame": 21,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconDruid",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/Druid/DrSkillicon",
                "normalFrame": 20,
                "pressedFrame": 21,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconAssassin",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/Assassin/AsSkillicon",
                "normalFrame": 20,
                "pressedFrame": 21,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconWarlock",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/Warlock/WaSkillicon",
                "normalFrame": 20,
                "pressedFrame": 21,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconGlobal",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/SubMenu/Skillicon",
                "normalFrame": 20,
                "pressedFrame": 21,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "TextBoxWidget",
              "name": "Countdown",
              "fields": {
                "visible": false,
                "text": "__BUFF_PANEL_TIMER_RESERVE_00__",
                "fontType": "16pt",
                "rect": {
                  "x": -16,
                  "y": 52,
                  "width": 112,
                  "height": 42
                },
                "style": {
                  "pointSize": "$MediumFontSize",
                  "fontColor": "$FontColorWhite",
                  "alignment": {
                )buffpaneljson"
R"buffpaneljson(    "h": "center",
                    "v": "center"
                  },
                  "dropShadow": "$DefaultDropShadow",
                  "options": {
                    "hideOverflow": true
                  }
                }
              }
            },
            {
              "type": "FocusableWidget",
              "name": "Tooltip",
              "fields": {
                "rect": {
                  "x": 0,
                  "y": 0,
                  "width": 96,
                  "height": 96
                },
                "tooltipString": "​‌​‌​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​",
                "tooltipStyle": "$TooltipStyle"
              }
            }
          ]
        },
        {
          "type": "Widget",
          "name": "BuffSlot11",
          "fields": {
            "visible": false,
            "rect": {
              "x": 448,
              "y": 105,
              "width": 96,
              "height": 96
            }
          },
          "children": [
            {
              "type": "ButtonWidget",
              "name": "IconAmazon",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/amazon/AmSkillicon",
                "normalFrame": 22,
                "pressedFrame": 23,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconSorceress",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/Sorceress/SoSkillicon",
                "normalFrame": 22,
                "pressedFrame": 23,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconNecromancer",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/Necromancer/NeSkillicon",
                "normalFrame": 22,
                "pressedFrame": 23,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconPaladin",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/paladin/PaSkillicon",
                "normalFrame": 22,
                "pressedFrame": 23,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconBarbarian",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/Barbarian/BaSkillicon",
                "normalFrame": 22,
                "pressedFrame": 23,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconDruid",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/Druid/DrSkillicon",
                "normalFrame": 22,
                "pressedFrame": 23,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconAssassin",
              "fiel)buffpaneljson"
R"buffpaneljson(ds": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/Assassin/AsSkillicon",
                "normalFrame": 22,
                "pressedFrame": 23,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconWarlock",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/Warlock/WaSkillicon",
                "normalFrame": 22,
                "pressedFrame": 23,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconGlobal",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/SubMenu/Skillicon",
                "normalFrame": 22,
                "pressedFrame": 23,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "TextBoxWidget",
              "name": "Countdown",
              "fields": {
                "visible": false,
                "text": "__BUFF_PANEL_TIMER_RESERVE_00__",
                "fontType": "16pt",
                "rect": {
                  "x": -16,
                  "y": 52,
                  "width": 112,
                  "height": 42
                },
                "style": {
                  "pointSize": "$MediumFontSize",
                  "fontColor": "$FontColorWhite",
                  "alignment": {
                    "h": "center",
                    "v": "center"
                  },
                  "dropShadow": "$DefaultDropShadow",
                  "options": {
                    "hideOverflow": true
                  }
                }
              }
            },
            {
              "type": "FocusableWidget",
              "name": "Tooltip",
              "fields": {
                "rect": {
                  "x": 0,
                  "y": 0,
                  "width": 96,
                  "height": 96
                },
                "tooltipString": "‌‌​‌​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​",
                "tooltipStyle": "$TooltipStyle"
              }
            }
          ]
        },
        {
          "type": "Widget",
          "name": "BuffSlot12",
          "fields": {
            "visible": false,
            "rect": {
              "x": 560,
              "y": 105,
              "width": 96,
              "height": 96
            }
          },
          "children": [
            {
              "type": "ButtonWidget",
              "name": "IconAmazon",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/amazon/AmSkillicon",
                "normalFrame": 24,
                "pressedFrame": 25,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconSorceress",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/Sorceress/SoSkillicon",
                "normalFrame": 24,
                "pressedFrame": 25,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconNecromancer",
              "fields": {
     )buffpaneljson"
R"buffpaneljson(           "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/Necromancer/NeSkillicon",
                "normalFrame": 24,
                "pressedFrame": 25,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconPaladin",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/paladin/PaSkillicon",
                "normalFrame": 24,
                "pressedFrame": 25,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconBarbarian",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/Barbarian/BaSkillicon",
                "normalFrame": 24,
                "pressedFrame": 25,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconDruid",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/Druid/DrSkillicon",
                "normalFrame": 24,
                "pressedFrame": 25,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconAssassin",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/Assassin/AsSkillicon",
                "normalFrame": 24,
                "pressedFrame": 25,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconWarlock",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/Warlock/WaSkillicon",
                "normalFrame": 24,
                "pressedFrame": 25,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconGlobal",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/SubMenu/Skillicon",
                "normalFrame": 24,
                "pressedFrame": 25,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "TextBoxWidget",
              "name": "Countdown",
              "fields": {
                "visible": false,
                "text": "__BUFF_PANEL_TIMER_RESERVE_00__",
                "fontType": "16pt",
                "rect": {
                  "x": -16,
                  "y": 52,
                  "width": 112,
                  "height": 42
                },
                "style": {
                  "pointSize": "$MediumFontSize",
                  "fontColor": "$FontColorWhite",
                  "alignment": {
                    "h": "center",
                    "v": "center"
                  },
                  "dropShadow": "$DefaultDropShadow",
                  "options": {
                    "hideOverflow": true
                  }
                }
              }
            },
         )buffpaneljson"
R"buffpaneljson(   {
              "type": "FocusableWidget",
              "name": "Tooltip",
              "fields": {
                "rect": {
                  "x": 0,
                  "y": 0,
                  "width": 96,
                  "height": 96
                },
                "tooltipString": "​​‌‌​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​",
                "tooltipStyle": "$TooltipStyle"
              }
            }
          ]
        },
        {
          "type": "Widget",
          "name": "BuffSlot13",
          "fields": {
            "visible": false,
            "rect": {
              "x": 672,
              "y": 105,
              "width": 96,
              "height": 96
            }
          },
          "children": [
            {
              "type": "ButtonWidget",
              "name": "IconAmazon",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/amazon/AmSkillicon",
                "normalFrame": 26,
                "pressedFrame": 27,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconSorceress",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/Sorceress/SoSkillicon",
                "normalFrame": 26,
                "pressedFrame": 27,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconNecromancer",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/Necromancer/NeSkillicon",
                "normalFrame": 26,
                "pressedFrame": 27,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconPaladin",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/paladin/PaSkillicon",
                "normalFrame": 26,
                "pressedFrame": 27,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconBarbarian",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/Barbarian/BaSkillicon",
                "normalFrame": 26,
                "pressedFrame": 27,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconDruid",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/Druid/DrSkillicon",
                "normalFrame": 26,
                "pressedFrame": 27,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconAssassin",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/Assassin/AsSkillicon",
                "normalFrame": 26,
               )buffpaneljson"
R"buffpaneljson( "pressedFrame": 27,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconWarlock",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/Warlock/WaSkillicon",
                "normalFrame": 26,
                "pressedFrame": 27,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconGlobal",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/SubMenu/Skillicon",
                "normalFrame": 26,
                "pressedFrame": 27,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "TextBoxWidget",
              "name": "Countdown",
              "fields": {
                "visible": false,
                "text": "__BUFF_PANEL_TIMER_RESERVE_00__",
                "fontType": "16pt",
                "rect": {
                  "x": -16,
                  "y": 52,
                  "width": 112,
                  "height": 42
                },
                "style": {
                  "pointSize": "$MediumFontSize",
                  "fontColor": "$FontColorWhite",
                  "alignment": {
                    "h": "center",
                    "v": "center"
                  },
                  "dropShadow": "$DefaultDropShadow",
                  "options": {
                    "hideOverflow": true
                  }
                }
              }
            },
            {
              "type": "FocusableWidget",
              "name": "Tooltip",
              "fields": {
                "rect": {
                  "x": 0,
                  "y": 0,
                  "width": 96,
                  "height": 96
                },
                "tooltipString": "‌​‌‌​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​",
                "tooltipStyle": "$TooltipStyle"
              }
            }
          ]
        },
        {
          "type": "Widget",
          "name": "BuffSlot14",
          "fields": {
            "visible": false,
            "rect": {
              "x": 0,
              "y": 0,
              "width": 96,
              "height": 96
            }
          },
          "children": [
            {
              "type": "ButtonWidget",
              "name": "IconAmazon",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/amazon/AmSkillicon",
                "normalFrame": 28,
                "pressedFrame": 29,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconSorceress",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/Sorceress/SoSkillicon",
                "normalFrame": 28,
                "pressedFrame": 29,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconNecromancer",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/Necromancer/NeSkillicon",
                "normalFrame": 28,
                "pressedFram)buffpaneljson"
R"buffpaneljson(e": 29,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconPaladin",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/paladin/PaSkillicon",
                "normalFrame": 28,
                "pressedFrame": 29,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconBarbarian",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/Barbarian/BaSkillicon",
                "normalFrame": 28,
                "pressedFrame": 29,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconDruid",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/Druid/DrSkillicon",
                "normalFrame": 28,
                "pressedFrame": 29,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconAssassin",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/Assassin/AsSkillicon",
                "normalFrame": 28,
                "pressedFrame": 29,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconWarlock",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/Warlock/WaSkillicon",
                "normalFrame": 28,
                "pressedFrame": 29,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconGlobal",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/SubMenu/Skillicon",
                "normalFrame": 28,
                "pressedFrame": 29,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "TextBoxWidget",
              "name": "Countdown",
              "fields": {
                "visible": false,
                "text": "__BUFF_PANEL_TIMER_RESERVE_00__",
                "fontType": "16pt",
                "rect": {
                  "x": -16,
                  "y": 52,
                  "width": 112,
                  "height": 42
                },
                "style": {
                  "pointSize": "$MediumFontSize",
                  "fontColor": "$FontColorWhite",
                  "alignment": {
                    "h": "center",
                    "v": "center"
                  },
                  "dropShadow": "$DefaultDropShadow",
                  "options": {
                    "hideOverflow": true
                  }
                }
              }
            },
            {
              "type": "FocusableWidget",
              "name": "Tooltip",
              "fields": {
                "rect": {
                  "x": 0,
                  "y": 0,
                  "width": 96,
                  "height": 96
                },
                "t)buffpaneljson"
R"buffpaneljson(ooltipString": "​‌‌‌​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​",
                "tooltipStyle": "$TooltipStyle"
              }
            }
          ]
        },
        {
          "type": "Widget",
          "name": "BuffSlot15",
          "fields": {
            "visible": false,
            "rect": {
              "x": 112,
              "y": 0,
              "width": 96,
              "height": 96
            }
          },
          "children": [
            {
              "type": "ButtonWidget",
              "name": "IconAmazon",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/amazon/AmSkillicon",
                "normalFrame": 30,
                "pressedFrame": 31,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconSorceress",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/Sorceress/SoSkillicon",
                "normalFrame": 30,
                "pressedFrame": 31,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconNecromancer",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/Necromancer/NeSkillicon",
                "normalFrame": 30,
                "pressedFrame": 31,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconPaladin",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/paladin/PaSkillicon",
                "normalFrame": 30,
                "pressedFrame": 31,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconBarbarian",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/Barbarian/BaSkillicon",
                "normalFrame": 30,
                "pressedFrame": 31,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconDruid",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/Druid/DrSkillicon",
                "normalFrame": 30,
                "pressedFrame": 31,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconAssassin",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/Assassin/AsSkillicon",
                "normalFrame": 30,
                "pressedFrame": 31,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconWarlock",
              "fields": {
                "visible": false,
                "rect": {
               )buffpaneljson"
R"buffpaneljson(   "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/Warlock/WaSkillicon",
                "normalFrame": 30,
                "pressedFrame": 31,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconGlobal",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/SubMenu/Skillicon",
                "normalFrame": 30,
                "pressedFrame": 31,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "TextBoxWidget",
              "name": "Countdown",
              "fields": {
                "visible": false,
                "text": "__BUFF_PANEL_TIMER_RESERVE_00__",
                "fontType": "16pt",
                "rect": {
                  "x": -16,
                  "y": 52,
                  "width": 112,
                  "height": 42
                },
                "style": {
                  "pointSize": "$MediumFontSize",
                  "fontColor": "$FontColorWhite",
                  "alignment": {
                    "h": "center",
                    "v": "center"
                  },
                  "dropShadow": "$DefaultDropShadow",
                  "options": {
                    "hideOverflow": true
                  }
                }
              }
            },
            {
              "type": "FocusableWidget",
              "name": "Tooltip",
              "fields": {
                "rect": {
                  "x": 0,
                  "y": 0,
                  "width": 96,
                  "height": 96
                },
                "tooltipString": "‌‌‌‌​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​",
                "tooltipStyle": "$TooltipStyle"
              }
            }
          ]
        },
        {
          "type": "Widget",
          "name": "BuffSlot16",
          "fields": {
            "visible": false,
            "rect": {
              "x": 224,
              "y": 0,
              "width": 96,
              "height": 96
            }
          },
          "children": [
            {
              "type": "ButtonWidget",
              "name": "IconAmazon",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/amazon/AmSkillicon",
                "normalFrame": 32,
                "pressedFrame": 33,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconSorceress",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/Sorceress/SoSkillicon",
                "normalFrame": 32,
                "pressedFrame": 33,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconNecromancer",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/Necromancer/NeSkillicon",
                "normalFrame": 32,
                "pressedFrame": 33,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconPaladin",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
)buffpaneljson"
R"buffpaneljson(                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/paladin/PaSkillicon",
                "normalFrame": 32,
                "pressedFrame": 33,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconBarbarian",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/Barbarian/BaSkillicon",
                "normalFrame": 32,
                "pressedFrame": 33,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconDruid",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/Druid/DrSkillicon",
                "normalFrame": 32,
                "pressedFrame": 33,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconAssassin",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/Assassin/AsSkillicon",
                "normalFrame": 32,
                "pressedFrame": 33,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconWarlock",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/Warlock/WaSkillicon",
                "normalFrame": 32,
                "pressedFrame": 33,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconGlobal",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/SubMenu/Skillicon",
                "normalFrame": 32,
                "pressedFrame": 33,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "TextBoxWidget",
              "name": "Countdown",
              "fields": {
                "visible": false,
                "text": "__BUFF_PANEL_TIMER_RESERVE_00__",
                "fontType": "16pt",
                "rect": {
                  "x": -16,
                  "y": 52,
                  "width": 112,
                  "height": 42
                },
                "style": {
                  "pointSize": "$MediumFontSize",
                  "fontColor": "$FontColorWhite",
                  "alignment": {
                    "h": "center",
                    "v": "center"
                  },
                  "dropShadow": "$DefaultDropShadow",
                  "options": {
                    "hideOverflow": true
                  }
                }
              }
            },
            {
              "type": "FocusableWidget",
              "name": "Tooltip",
              "fields": {
                "rect": {
                  "x": 0,
                  "y": 0,
                  "width": 96,
                  "height": 96
                },
                "tooltipString": "​​​​‌​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​",
                "tooltipStyle": "$TooltipStyle"
              }
            }
          ]
        },
        {
          "type": "Widget")buffpaneljson"
R"buffpaneljson(,
          "name": "BuffSlot17",
          "fields": {
            "visible": false,
            "rect": {
              "x": 336,
              "y": 0,
              "width": 96,
              "height": 96
            }
          },
          "children": [
            {
              "type": "ButtonWidget",
              "name": "IconAmazon",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/amazon/AmSkillicon",
                "normalFrame": 34,
                "pressedFrame": 35,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconSorceress",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/Sorceress/SoSkillicon",
                "normalFrame": 34,
                "pressedFrame": 35,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconNecromancer",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/Necromancer/NeSkillicon",
                "normalFrame": 34,
                "pressedFrame": 35,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconPaladin",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/paladin/PaSkillicon",
                "normalFrame": 34,
                "pressedFrame": 35,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconBarbarian",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/Barbarian/BaSkillicon",
                "normalFrame": 34,
                "pressedFrame": 35,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconDruid",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/Druid/DrSkillicon",
                "normalFrame": 34,
                "pressedFrame": 35,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconAssassin",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/Assassin/AsSkillicon",
                "normalFrame": 34,
                "pressedFrame": 35,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconWarlock",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/Warlock/WaSkillicon",
                "normalFrame": 34,
                "pressedFrame": 35,
                "focusOnMouseOver": false
              }
        )buffpaneljson"
R"buffpaneljson(    },
            {
              "type": "ButtonWidget",
              "name": "IconGlobal",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/SubMenu/Skillicon",
                "normalFrame": 34,
                "pressedFrame": 35,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "TextBoxWidget",
              "name": "Countdown",
              "fields": {
                "visible": false,
                "text": "__BUFF_PANEL_TIMER_RESERVE_00__",
                "fontType": "16pt",
                "rect": {
                  "x": -16,
                  "y": 52,
                  "width": 112,
                  "height": 42
                },
                "style": {
                  "pointSize": "$MediumFontSize",
                  "fontColor": "$FontColorWhite",
                  "alignment": {
                    "h": "center",
                    "v": "center"
                  },
                  "dropShadow": "$DefaultDropShadow",
                  "options": {
                    "hideOverflow": true
                  }
                }
              }
            },
            {
              "type": "FocusableWidget",
              "name": "Tooltip",
              "fields": {
                "rect": {
                  "x": 0,
                  "y": 0,
                  "width": 96,
                  "height": 96
                },
                "tooltipString": "‌​​​‌​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​",
                "tooltipStyle": "$TooltipStyle"
              }
            }
          ]
        },
        {
          "type": "Widget",
          "name": "BuffSlot18",
          "fields": {
            "visible": false,
            "rect": {
              "x": 448,
              "y": 0,
              "width": 96,
              "height": 96
            }
          },
          "children": [
            {
              "type": "ButtonWidget",
              "name": "IconAmazon",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/amazon/AmSkillicon",
                "normalFrame": 36,
                "pressedFrame": 37,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconSorceress",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/Sorceress/SoSkillicon",
                "normalFrame": 36,
                "pressedFrame": 37,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconNecromancer",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/Necromancer/NeSkillicon",
                "normalFrame": 36,
                "pressedFrame": 37,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconPaladin",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/paladin/PaSkillicon",
                "normalFrame": 36,
                "pressedFrame": 37,
                "focusOnMouseOver": false
              }
            },
    )buffpaneljson"
R"buffpaneljson(        {
              "type": "ButtonWidget",
              "name": "IconBarbarian",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/Barbarian/BaSkillicon",
                "normalFrame": 36,
                "pressedFrame": 37,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconDruid",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/Druid/DrSkillicon",
                "normalFrame": 36,
                "pressedFrame": 37,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconAssassin",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/Assassin/AsSkillicon",
                "normalFrame": 36,
                "pressedFrame": 37,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconWarlock",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/Warlock/WaSkillicon",
                "normalFrame": 36,
                "pressedFrame": 37,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconGlobal",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/SubMenu/Skillicon",
                "normalFrame": 36,
                "pressedFrame": 37,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "TextBoxWidget",
              "name": "Countdown",
              "fields": {
                "visible": false,
                "text": "__BUFF_PANEL_TIMER_RESERVE_00__",
                "fontType": "16pt",
                "rect": {
                  "x": -16,
                  "y": 52,
                  "width": 112,
                  "height": 42
                },
                "style": {
                  "pointSize": "$MediumFontSize",
                  "fontColor": "$FontColorWhite",
                  "alignment": {
                    "h": "center",
                    "v": "center"
                  },
                  "dropShadow": "$DefaultDropShadow",
                  "options": {
                    "hideOverflow": true
                  }
                }
              }
            },
            {
              "type": "FocusableWidget",
              "name": "Tooltip",
              "fields": {
                "rect": {
                  "x": 0,
                  "y": 0,
                  "width": 96,
                  "height": 96
                },
                "tooltipString": "​‌​​‌​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​",
                "tooltipStyle": "$TooltipStyle"
              }
            }
          ]
        },
        {
          "type": "Widget",
          "name": "BuffSlot19",
          "fields": {
            "visible": false,
            "rect": {
              "x": 560,
              "y": 0,
              "width": 96,
              "height": 96
            }
          },
          "children": [
            {
         )buffpaneljson"
R"buffpaneljson(     "type": "ButtonWidget",
              "name": "IconAmazon",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/amazon/AmSkillicon",
                "normalFrame": 38,
                "pressedFrame": 39,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconSorceress",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/Sorceress/SoSkillicon",
                "normalFrame": 38,
                "pressedFrame": 39,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconNecromancer",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/Necromancer/NeSkillicon",
                "normalFrame": 38,
                "pressedFrame": 39,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconPaladin",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/paladin/PaSkillicon",
                "normalFrame": 38,
                "pressedFrame": 39,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconBarbarian",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/Barbarian/BaSkillicon",
                "normalFrame": 38,
                "pressedFrame": 39,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconDruid",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/Druid/DrSkillicon",
                "normalFrame": 38,
                "pressedFrame": 39,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconAssassin",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/Assassin/AsSkillicon",
                "normalFrame": 38,
                "pressedFrame": 39,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconWarlock",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/Warlock/WaSkillicon",
                "normalFrame": 38,
                "pressedFrame": 39,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconGlobal",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },)buffpaneljson"
R"buffpaneljson(
                "filename": "Spells/SubMenu/Skillicon",
                "normalFrame": 38,
                "pressedFrame": 39,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "TextBoxWidget",
              "name": "Countdown",
              "fields": {
                "visible": false,
                "text": "__BUFF_PANEL_TIMER_RESERVE_00__",
                "fontType": "16pt",
                "rect": {
                  "x": -16,
                  "y": 52,
                  "width": 112,
                  "height": 42
                },
                "style": {
                  "pointSize": "$MediumFontSize",
                  "fontColor": "$FontColorWhite",
                  "alignment": {
                    "h": "center",
                    "v": "center"
                  },
                  "dropShadow": "$DefaultDropShadow",
                  "options": {
                    "hideOverflow": true
                  }
                }
              }
            },
            {
              "type": "FocusableWidget",
              "name": "Tooltip",
              "fields": {
                "rect": {
                  "x": 0,
                  "y": 0,
                  "width": 96,
                  "height": 96
                },
                "tooltipString": "‌‌​​‌​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​",
                "tooltipStyle": "$TooltipStyle"
              }
            }
          ]
        },
        {
          "type": "Widget",
          "name": "BuffSlot20",
          "fields": {
            "visible": false,
            "rect": {
              "x": 672,
              "y": 0,
              "width": 96,
              "height": 96
            }
          },
          "children": [
            {
              "type": "ButtonWidget",
              "name": "IconAmazon",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/amazon/AmSkillicon",
                "normalFrame": 40,
                "pressedFrame": 41,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconSorceress",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/Sorceress/SoSkillicon",
                "normalFrame": 40,
                "pressedFrame": 41,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconNecromancer",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/Necromancer/NeSkillicon",
                "normalFrame": 40,
                "pressedFrame": 41,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconPaladin",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/paladin/PaSkillicon",
                "normalFrame": 40,
                "pressedFrame": 41,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconBarbarian",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
       )buffpaneljson"
R"buffpaneljson(         "filename": "Spells/Barbarian/BaSkillicon",
                "normalFrame": 40,
                "pressedFrame": 41,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconDruid",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/Druid/DrSkillicon",
                "normalFrame": 40,
                "pressedFrame": 41,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconAssassin",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/Assassin/AsSkillicon",
                "normalFrame": 40,
                "pressedFrame": 41,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconWarlock",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/Warlock/WaSkillicon",
                "normalFrame": 40,
                "pressedFrame": 41,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "ButtonWidget",
              "name": "IconGlobal",
              "fields": {
                "visible": false,
                "rect": {
                  "x": 0,
                  "y": 0,
                  "scale": 0.6
                },
                "filename": "Spells/SubMenu/Skillicon",
                "normalFrame": 40,
                "pressedFrame": 41,
                "focusOnMouseOver": false
              }
            },
            {
              "type": "TextBoxWidget",
              "name": "Countdown",
              "fields": {
                "visible": false,
                "text": "__BUFF_PANEL_TIMER_RESERVE_00__",
                "fontType": "16pt",
                "rect": {
                  "x": -16,
                  "y": 52,
                  "width": 112,
                  "height": 42
                },
                "style": {
                  "pointSize": "$MediumFontSize",
                  "fontColor": "$FontColorWhite",
                  "alignment": {
                    "h": "center",
                    "v": "center"
                  },
                  "dropShadow": "$DefaultDropShadow",
                  "options": {
                    "hideOverflow": true
                  }
                }
              }
            },
            {
              "type": "FocusableWidget",
              "name": "Tooltip",
              "fields": {
                "rect": {
                  "x": 0,
                  "y": 0,
                  "width": 96,
                  "height": 96
                },
                "tooltipString": "​​‌​‌​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​​",
                "tooltipStyle": "$TooltipStyle"
              }
            }
          ]
        }
      ]
    }
  ]
}
)buffpaneljson"

;

} // namespace BuffPanel::Systems::BuffHud::Internal
