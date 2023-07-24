#include <cstdio>
#include <filesystem>
#include <fmt/args.h>
#include <fmt/format.h>
#include <vector>

#include "orikami.h"
#include "oriken.h"

#include <cmark-gfm.h>

extern "C" {
  #include <parser.h>
  #include <../extensions/cmark-gfm-core-extensions.h>
  // #include <../extensions/table.h>
}


namespace fs = std::filesystem;


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


struct Arg {
  template <typename _T>
  Arg(std::string const &in_name, _T const &in_value) : name(in_name), value(fmt::format("{}", in_value)) {}

  std::string name;
  std::string value;
};

using ArgStore = std::vector<Arg>;


std::string fopenErrnoName() {
  switch (errno) {
  case EACCES: return "EACCES";
  case EINTR: return "EINTR";
  case EINVAL: return "EINVAL";
  case EISDIR: return "EISDIR";
  case ELOOP: return "ELOOP";
  case EMFILE: return "EMFILE";
  case ENAMETOOLONG: return "ENAMETOOLONG";
  case ENFILE: return "ENFILE";
  case ENOENT: return "ENOENT";
  case ENOMEM: return "ENOMEM";
  case ENOSPC: return "ENOSPC";
  case ENOTDIR: return "ENOTDIR";
  case ENXIO: return "ENXIO";
  case EOVERFLOW: return "EOVERFLOW";
  case EROFS: return "EROFS";
  case ETXTBSY: return "ETXTBSY";
  default: return fmt::format("E?({})", errno);
  }
}


std::string getFormattedString(char const *string, ArgStore const &args) {
  auto store = fmt::dynamic_format_arg_store<fmt::format_context>();
  for (auto const &arg : args) {
    store.push_back(fmt::arg(arg.name.c_str(), arg.value));
  }
  return fmt::vformat(string, store);
}


std::string getFormattedFile(fs::path const &path, ArgStore const &args) {
  fmt::print(stderr, "Formatting '{}'\n", path.c_str());
  auto file = fopen(path.c_str(), "r");
  if (file) {
    std::vector<char> buf(fs::file_size(path) + 1);
    std::size_t chars_read = fread(buf.data(), sizeof(char), buf.size(), file);
    if (chars_read) {
      buf[chars_read] = '\0';
      return getFormattedString(buf.data(), args);
    }
  } else {
    fmt::print(stderr, "Error [{}] opening file '{}'\n", fopenErrnoName(), path.c_str());
    throw "";
  }
  return "";
}


template <typename _Archetype>
ArgStore const &getBaseArgStore() {
  static ArgStore store = {
    {"table", formatTable<_Archetype>()},
    {"spell_list", _Archetype::spell_list},
    {"hit_die", _Archetype::hit_die},
    {"hit_die_ev", _Archetype::hit_die / 2 + 1},
    {"skills", "Animal Handling, Nature, Sleight of Hand, Survival"},
    {"Class", _Archetype::name},
    {"class", toLowerCase(_Archetype::name)},
    {"quick_build_ability_2", _Archetype::quick_build_ability_2},
  };
  return store;
}


template <typename _Archetype>
std::string getFormattedFeature(std::string const &feature) {
  // Search the archetype's directory first.
  // If nothing is found, check the default directory.
  // If nothing is found, error?
  constexpr auto feature_format = "<a id=\"{}-{}\"></a>\n\n{}";
  auto archetype_path = fs::path{"archetypes"} / toLowerCase(_Archetype::name) / fmt::format("{}.md", toTokenName(feature));
  if (fs::is_regular_file(archetype_path)) {
    return fmt::format(
      feature_format,
      toLowerCase(_Archetype::name),
      slugify(feature),
      getFormattedFile(archetype_path, getBaseArgStore<_Archetype>())
    );
  }
  auto default_path = fs::path{"archetypes"} / "default" / fmt::format("{}.md", toTokenName(feature));
  if (fs::is_regular_file(default_path)) {
    return fmt::format(
      feature_format,
      toLowerCase(_Archetype::name),
      slugify(feature),
      getFormattedFile(default_path, getBaseArgStore<_Archetype>())
    );
  }
  fmt::print(stderr, "Feature-file for '{}' not found.  Please create one of the following files:\n", toTokenName(feature));
  fmt::print(stderr, "    {}\n", archetype_path.c_str());
  fmt::print(stderr, "    {}\n", default_path.c_str());
  throw "";
}


template <typename _Archetype>
ArgStore makeArgStore() {
  ArgStore args = getBaseArgStore<_Archetype>();

  { std::vector<std::string> feature_names;
    for (uint8_t level = 1; level < 20; ++level) {
      for (auto const &feature : _Archetype::levelFeatures(level)) {
        if (std::find(feature_names.begin(), feature_names.end(), feature) == feature_names.end()) {
          feature_names.push_back(feature);
        }
      }
    }

    std::string features {};
    for (auto const &feature : feature_names) {
      features = fmt::format("{}\n{}", features, getFormattedFeature<_Archetype>(feature));
    }
    args.push_back({"features", features});
  }

  { auto path = fs::path{"archetypes"} / toLowerCase(_Archetype::name) / "Flavor.md";
    if (fs::is_regular_file(path)) {
      args.push_back({"flavor", getFormattedFile(path, getBaseArgStore<_Archetype>())});
    } else {
      fmt::print(stderr, "{} flavor file for not found.  Please create {}\n", _Archetype::name, path.c_str());
    }
  }

  return args;
}


template <typename _Archetype>
void addToOutput(std::string &toc, std::string &body) {
  // Append this archetype's TOC entry.
  toc = fmt::format("{}{}\n", toc, getFormattedString("- [{Class}](#{class})", getBaseArgStore<_Archetype>()));

  // Specialize the archetype file.
  auto md = getFormattedFile("templates/archetype.md", makeArgStore<_Archetype>());
  // Parse the archetype Markdown into HTML.
  auto parser = cmark_parser_new_with_mem(CMARK_OPT_UNSAFE, cmark_get_arena_mem_allocator());
  cmark_parser_attach_syntax_extension(parser, cmark_find_syntax_extension("table"));
  cmark_parser_feed(parser, md.c_str(), md.size());
  auto doc = cmark_parser_finish(parser);
  auto html = cmark_render_html(doc, CMARK_OPT_UNSAFE, parser->syntax_extensions);

  body = fmt::format("{}{}\n", body, html);
  cmark_arena_reset();
}


int main() {
  cmark_gfm_core_extensions_ensure_registered();

  std::string toc {};
  std::string body {};

  addToOutput<Orikami>(toc, body);
  addToOutput<Oriken>(toc, body);

  { auto parser = cmark_parser_new_with_mem(CMARK_OPT_UNSAFE, cmark_get_arena_mem_allocator());
    cmark_parser_attach_syntax_extension(parser, cmark_find_syntax_extension("table"));
    cmark_parser_feed(parser, toc.c_str(), toc.size());
    auto doc = cmark_parser_finish(parser);
    toc = cmark_render_html(doc, CMARK_OPT_UNSAFE, parser->syntax_extensions);
    cmark_arena_reset();
  }

  fmt::print(
    "{}",
    getFormattedFile(
      "templates/index.html",
      { { "toc", toc }
      , { "body", body }
      }
    )
  );

  return 0;
}
