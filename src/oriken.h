#pragma once

#include "common.h"


struct Oriken : HalfCaster {
  static constexpr auto name = "Oriken";

  static LevelFeatures const &levelFeatures(uint8_t in_level) {
    static AllFeatures values = {
      { { "Familiar Weapon", "Papercraft Familiar" }
      , { "Fighting Style", "Spellcasting" }
      , { "Enhance Armor" }
      , { "Ability Score Improvement" }
      , { "Extra Attack" }
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

  static constexpr uint8_t hit_die = 10;

  static constexpr auto quick_build_ability_2 = "Constitution";

  static constexpr auto spell_list = "wizard";

  static constexpr auto columns = std::make_tuple(
  );

  static constexpr auto column_names = std::make_tuple(
  );

};
