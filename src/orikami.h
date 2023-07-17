#pragma once

#include "common.h"


struct Orikami : FullCaster {
  static constexpr auto name = "Orikami";

  static LevelFeatures const &levelFeatures(uint8_t in_level) {
    static AllFeatures values = {
      { { "Papercraft Familiar", "Spellcasting" }
      , { "Improved Familiar" }
      , {}
      , { "Familiar Spells", "Ability Score Improvement" }
      , {}
      , {}
      , {}
      , { "Ability Score Improvement" }
      , {}
      , {}
      , {}
      , { "Ability Score Improvement" }
      , {}
      , {}
      , {}
      , { "Ability Score Improvement" }
      , {}
      , {}
      , { "Ability Score Improvement" }
      , {}
      }
    };
    return values[in_level - 1];
  }

  static constexpr uint8_t hit_die = 8;

  static constexpr auto quick_build_ability_2 = "Wisdom";

  static constexpr auto spell_list = "druid";

  static uint8_t cantripsKnown(uint8_t in_level) {
    static constexpr uint8_t values[20] = { 2, 2, 2, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4 };
    return values[in_level - 1];
  }

  static uint8_t familiarCantrips(uint8_t in_level) {
    static constexpr uint8_t values[20] = { 0, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2 };
    return values[in_level - 1];
  }

  static uint8_t familiarSpells(uint8_t in_level) {
    return in_level / 4;
  }

  static constexpr auto columns = std::make_tuple(
    familiarCantrips,
    familiarSpells,
    cantripsKnown
  );

  static constexpr auto column_names = std::make_tuple(
    "Familiar Cantrips",
    "Familiar Spells",
    "Cantrips Known"
  );

};
