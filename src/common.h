#pragma once

#include <array>
#include <string>
#include <tuple>
#include <vector>

#include <fmt/ranges.h>


std::string toLowerCase(std::string const &);

std::string toUpperCase(std::string const &);

std::string slugify(std::string const &);

std::string toTokenName(std::string const &);


constexpr char toLowerCase(char c) {
  if ('A' <= c and c <= 'Z') {
    return c - 'A' + 'a';
  }
  return c;
}

constexpr char toUpperCase(char c) {
  if ('a' <= c and c <= 'z') {
    return c - 'a' + 'A';
  }
  return c;
}

constexpr char slugifyChar(char c) {
  switch (c) {
  case 'a':
  case 'A':
    return 'a';
  case 'b':
  case 'B':
    return 'b';
  case 'c':
  case 'C':
    return 'c';
  case 'd':
  case 'D':
    return 'd';
  case 'e':
  case 'E':
    return 'e';
  case 'f':
  case 'F':
    return 'f';
  case 'g':
  case 'G':
    return 'g';
  case 'h':
  case 'H':
    return 'h';
  case 'i':
  case 'I':
    return 'i';
  case 'j':
  case 'J':
    return 'j';
  case 'k':
  case 'K':
    return 'k';
  case 'l':
  case 'L':
    return 'l';
  case 'm':
  case 'M':
    return 'm';
  case 'n':
  case 'N':
    return 'n';
  case 'o':
  case 'O':
    return 'o';
  case 'p':
  case 'P':
    return 'p';
  case 'q':
  case 'Q':
    return 'q';
  case 'r':
  case 'R':
    return 'r';
  case 's':
  case 'S':
    return 's';
  case 't':
  case 'T':
    return 't';
  case 'u':
  case 'U':
    return 'u';
  case 'v':
  case 'V':
    return 'v';
  case 'w':
  case 'W':
    return 'w';
  case 'x':
  case 'X':
    return 'x';
  case 'y':
  case 'Y':
    return 'y';
  case 'z':
  case 'Z':
    return 'z';
  case '_':
    return '_';
  case ' ':
    return '-';
  default:
    return '\0';
  }
}

constexpr char toTokenChar(char c) {
  if (
    ('a' <= c and c <= 'z') or
    ('A' <= c and c <= 'Z') or
    ('0' <= c and c <= '9')
  ) {
    return c;
  }
  return '_';
}


template <typename _T, _T (*_level_function)(uint8_t)>
struct Column {
  static constexpr auto format = "{}";
  static constexpr auto attributes = "";
  static constexpr _T (*function)(uint8_t) = _level_function;
};


constexpr uint8_t levelFn(uint8_t level) {
  return level;
}


constexpr uint8_t proficiencyBonus(uint8_t level) {
  return (level + 7) / 4;
}

template <>
struct Column<uint8_t, proficiencyBonus> {
  static constexpr auto format = "+{}";
  static constexpr auto attributes = "";
  static constexpr uint8_t (*function)(uint8_t) = proficiencyBonus;
};


using LevelFeatures = std::vector<std::string>;
using AllFeatures = std::array<LevelFeatures, 20>;


template <typename _T>
struct Columns {
  static std::string features(uint8_t level) {
    LevelFeatures const &values = _T::levelFeatures(level);
    std::vector<std::string> links(values.size());
    for (uint8_t i = 0; i < values.size(); ++i) {
      links[i] = fmt::format("<a href=\"#{}-{}\">{}</a>", toLowerCase(_T::name), slugify(values[i]), values[i]);
    }
    return fmt::format("{}", fmt::join(links, ", "));
  }

  static constexpr auto columns = std::tuple_cat(
    std::make_tuple(
      Column<uint8_t, levelFn>{},
      Column<uint8_t, proficiencyBonus>{},
      Column<std::string, features>{}
    ),
    _T::columns,
    std::make_tuple(Column<uint8_t, _T::origamiCap>{}),
    _T::spell_columns
  );

  static constexpr auto names =
    std::make_tuple(
      std::tuple_cat(
        std::make_tuple("Level", "Proficiency Bonus", "Features"),
        _T::column_names,
        std::make_tuple("Origami Cap")
      ),
      _T::spell_column_names
    );

};


struct FullCaster {
  static uint8_t origamiCap(uint8_t level) {
    return 2 * level;
  }

  static constexpr uint8_t spell_slots[9][20] =
    { { 2, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4 }
    , { 0, 0, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3 }
    , { 0, 0, 0, 0, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3 }
    , { 0, 0, 0, 0, 0, 0, 1, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3 }
    , { 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3 }
    , { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2 }
    , { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 2 }
    , { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1 }
    , { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1 }
    };

  template <uint8_t _slot>
  static uint8_t spellSlots(uint8_t level) {
    return spell_slots[_slot - 1][level - 1];
  }

  static constexpr auto spell_columns = std::make_tuple(
    Column<uint8_t, spellSlots<1>>{},
    Column<uint8_t, spellSlots<2>>{},
    Column<uint8_t, spellSlots<3>>{},
    Column<uint8_t, spellSlots<4>>{},
    Column<uint8_t, spellSlots<5>>{},
    Column<uint8_t, spellSlots<6>>{},
    Column<uint8_t, spellSlots<7>>{},
    Column<uint8_t, spellSlots<8>>{},
    Column<uint8_t, spellSlots<9>>{}
  );

  static constexpr auto spell_column_names = std::make_tuple(
    "1", "2", "3", "4", "5", "6", "7", "8", "9"
  );

};


struct HalfCaster {
  static uint8_t origamiCap(uint8_t level) {
    return level > 1 ? level : 0;
  }

  static constexpr uint8_t spell_slots[5][20] =
    { { 0, 2, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4 }
    , { 0, 0, 0, 0, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3 }
    , { 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3 }
    , { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 2, 2, 3, 3, 3, 3 }
    , { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 2, 2 }
    };

  template <uint8_t _slot>
  static uint8_t spellSlots(uint8_t level) {
    return spell_slots[_slot - 1][level - 1];
  }

  static constexpr auto spell_columns = std::make_tuple(
    Column<uint8_t, spellSlots<1>>{},
    Column<uint8_t, spellSlots<2>>{},
    Column<uint8_t, spellSlots<3>>{},
    Column<uint8_t, spellSlots<4>>{},
    Column<uint8_t, spellSlots<5>>{}
  );

  static constexpr auto spell_column_names = std::make_tuple(
    "1", "2", "3", "4", "5"
  );

};
