#include <cstdio>
#include <filesystem>
#include <fmt/format.h>
#include <vector>


namespace fs = std::filesystem;


static constexpr auto files = {
  "../../Spellcasting.md"
};


struct Arg { std::string value; };

struct Class      : public Arg { static constexpr auto name = "Class"; };
struct SpellList  : public Arg { static constexpr auto name = "SpellList"; };
struct SpellScale : public Arg { static constexpr auto name = "SpellScale"; };


template <typename ..._Ts>
int processFileWithArgs(
  fs::path const &in_path,
  // fs::path const &out_path,
  _Ts ...args
) {
  if (fs::is_regular_file(in_path)) {
    auto f = std::fopen(in_path.c_str(), "r");
    if (f) {
      std::vector<char> buf (fs::file_size(in_path));
      if (std::fread(buf.data(), sizeof(buf.front()), buf.size(), f)) {
        fmt::print(buf.data(), fmt::arg(_Ts::name, args.value)...);
      }
      std::fclose(f);
    }
  }
  return 0;
}


int main() {
  processFileWithArgs(
    "../../Spellcasting.md",
    Class{{"orikami"}},
    SpellList{{"druid"}},
    SpellScale{{"2"}}
  );
  return 0;
}
