#pragma once

#include "common.h"


struct Orikami : FullCaster {
  static constexpr auto name = "Orikami";

  static LevelFeatures const &levelFeatures(uint8_t in_level) {
    static AllFeatures values = {
      { { "Spellcasting" }
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

  static uint8_t familiarSpells(uint8_t in_level) {
    return in_level / 4;
  }

  static constexpr auto columns = std::make_tuple(
    familiarSpells
  );

  static constexpr auto column_names = std::make_tuple(
    "Familiar Spells"
  );

};
