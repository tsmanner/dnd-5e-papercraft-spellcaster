#include <cstdio>
#include <filesystem>
#include <fmt/args.h>
#include <fmt/format.h>
#include <vector>

#include "orikami.h"
#include "oriken.h"


namespace fs = std::filesystem;


template <typename ..._Ts>
int processFileWithArgs(
  fs::path const &in_path,
  fs::path const &out_path,
  std::tuple<_Ts ...> const &args
) {
  if (fs::is_regular_file(in_path)) {
    auto in_f = std::fopen(in_path.c_str(), "r");
    if (in_f) {
      auto out_f = out_path == "-" ? stdout : std::fopen(out_path.c_str(), "w");
      if (out_f) {
        std::vector<char> buf (fs::file_size(in_path));
        if (std::fread(buf.data(), sizeof(buf.front()), buf.size(), in_f)) {
          fmt::print(out_f, buf.data(), fmt::arg(_Ts::name, std::get<_Ts>(args).value)...);
        }
        // Don't try to close stdout
        if (out_path == "-") {
          std::fclose(out_f);
        }
      }
      std::fclose(in_f);
    }
  }
  return 0;
}


template <typename ..._Ts>
std::string formatTable(std::string const &format, _Ts ...fs) {
  std::string result {};
  for (uint8_t lvl = 1; lvl <= 20; ++lvl) {
    fmt::format_to(std::back_inserter(result), format, lvl, fs(lvl)...);
  }
  result.push_back('\0');
  return result;
}


template <std::size_t _i, typename _Iter, typename ..._Ts>
void formatColumnHeaderTo(_Iter head, _Iter div, std::tuple<_Ts...> const &names) {
  fmt::format_to(head, " {} |", std::get<_i>(names));
  fmt::format_to(div, "---|");
  if constexpr (_i < sizeof...(_Ts) - 1) {
    formatColumnHeaderTo<_i + 1>(head, div, names);
  }
}

template <typename _Names>
std::string formatTableHeader(_Names const &names) {
  std::string head {};
  std::string div {};
  fmt::format_to(std::back_inserter(head), "| Level | Proficiency Bonus |");
  fmt::format_to(std::back_inserter(div), "|---|---|");
  formatColumnHeaderTo<0>(std::back_inserter(head), std::back_inserter(div), names);
  head.push_back('\n');
  div.push_back('\n');
  return head + div;
}


template <std::size_t _i, typename _Iter, typename ..._Ts>
void formatColumnsTo(_Iter out, uint8_t level, std::tuple<_Ts...> const &fs) {
  fmt::format_to(out, " {} |", std::get<_i>(fs)(level));
  if constexpr (_i < sizeof...(_Ts) - 1) {
    formatColumnsTo<_i + 1>(out, level, fs);
  }
}

template <typename _LevelFunctions>
std::string formatTable(_LevelFunctions const &fs) {
  std::string result {};
  for (uint8_t level = 1; level <= 20; ++level) {
    fmt::format_to(std::back_inserter(result), "| {} | {:+} |", level, +proficiencyBonus(level));
    formatColumnsTo<0>(std::back_inserter(result), level, fs);
    result.push_back('\n');
  }
  return result;
}


template <typename _Archetype>
std::string formatTable() {
  return
    formatTableHeader(Columns<_Archetype>::names) +
    formatTable(Columns<_Archetype>::columns)
    ;
}


template <typename _Archetype, typename _Format>
void print(_Format const &format) {
  fmt::print(
    format,
    fmt::arg("table", formatTable<_Archetype>()),
    fmt::arg("hit_die", _Archetype::hit_die),
    fmt::arg("hit_die_ev", _Archetype::hit_die / 2 + 1),
    fmt::arg("skills", "Animal Handling, Nature, Sleight of Hand, Survival"),
    fmt::arg("Class", _Archetype::name),
    fmt::arg("class", toLowerCase(_Archetype::name))
  );
}

int main() {
  auto template_path = fs::path("../../template.md");
  if (fs::is_regular_file(template_path)) {
    auto template_file = std::fopen(template_path.c_str(), "r");
    if (template_file) {
      std::vector<char> buf (fs::file_size(template_path));
      if (std::fread(buf.data(), sizeof(char), buf.size(), template_file)) {
        print<Orikami>(buf.data());
        fmt::print("\n");
        print<Oriken>(buf.data());
      }
      fclose(template_file);
    }
  }

  return 0;
}
