#pragma once

#include <array>
#include <string>
#include <tuple>
#include <vector>

#include <fmt/ranges.h>


constexpr char toLowerCase(char c) {
  if ('A' <= c and c <= 'Z') {
    return c - 'A' + 'a';
  }
  return c;
}

std::string toLowerCase(std::string const &s) {
  std::string result {};
  for (auto c : s) {
    result.push_back(toLowerCase(c));
  }
  return result;
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


std::string slugify(std::string const &value) {
  std::string result;
  result.reserve(value.size());
  for (std::size_t read_i = 0; read_i < value.size(); ++read_i) {
    auto c = slugifyChar(value[read_i]);
    if (c) {
      result.push_back(c);
    }
  }
  return result;
}


uint8_t proficiencyBonus(uint8_t in_level) {
  return (in_level + 7) / 4;
}


using LevelFeatures = std::vector<std::string>;
using AllFeatures = std::array<LevelFeatures, 20>;


template <typename _T>
struct Columns {
  static std::string features(uint8_t in_level) {
    LevelFeatures const &values = _T::levelFeatures(in_level);
    std::vector<std::string> links(values.size());
    for (uint8_t i = 0; i < values.size(); ++i) {
      links[i] = fmt::format("[{}](#{})", values[i], slugify(values[i]));
    }
    return fmt::format("{}", fmt::join(links, ", "));
  }

  static constexpr auto columns = std::tuple_cat(
    std::make_tuple(features),
    _T::columns,
    std::make_tuple(_T::origamiCap),
    _T::spell_columns
  );

  static constexpr auto names = std::tuple_cat(
    std::make_tuple("Features"),
    _T::column_names,
    std::make_tuple("Origami Cap"),
    _T::spell_column_names
  );

};


struct FullCaster {
  static uint8_t origamiCap(uint8_t in_level) {
    return 2 * in_level;
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
  static uint8_t spellSlots(uint8_t in_level) {
    return spell_slots[_slot - 1][in_level - 1];
  }

  static constexpr auto spell_columns = std::make_tuple(
    spellSlots<1>,
    spellSlots<2>,
    spellSlots<3>,
    spellSlots<4>,
    spellSlots<5>,
    spellSlots<6>,
    spellSlots<7>,
    spellSlots<8>,
    spellSlots<9>
  );

  static constexpr auto spell_column_names = std::make_tuple(
    "1",
    "2",
    "3",
    "4",
    "5",
    "6",
    "7",
    "8",
    "9"
  );

};


struct HalfCaster {
  static uint8_t origamiCap(uint8_t in_level) {
    return in_level > 1 ? in_level : 0;
  }

  static constexpr uint8_t spell_slots[5][20] =
    { { 0, 2, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4 }
    , { 0, 0, 0, 0, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3 }
    , { 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3 }
    , { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 2, 2, 3, 3, 3, 3 }
    , { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 2, 2 }
    };

  template <uint8_t _slot>
  static uint8_t spellSlots(uint8_t in_level) {
    return spell_slots[_slot - 1][in_level - 1];
  }

  static constexpr auto spell_columns = std::make_tuple(
    spellSlots<1>,
    spellSlots<2>,
    spellSlots<3>,
    spellSlots<4>,
    spellSlots<5>
  );

  static constexpr auto spell_column_names = std::make_tuple(
    "1",
    "2",
    "3",
    "4",
    "5"
  );

};
