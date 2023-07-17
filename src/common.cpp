#include "common.h"


std::string toLowerCase(std::string const &s) {
  std::string result {};
  result.reserve(s.size());
  for (auto c : s) {
    result.push_back(toLowerCase(c));
  }
  return result;
}


std::string toUpperCase(std::string const &s) {
  std::string result {};
  result.reserve(s.size());
  for (auto c : s) {
    result.push_back(toUpperCase(c));
  }
  return result;
}


std::string slugify(std::string const &s) {
  std::string result;
  result.reserve(s.size());
  for (std::size_t read_i = 0; read_i < s.size(); ++read_i) {
    auto c = slugifyChar(s[read_i]);
    if (c) {
      result.push_back(c);
    }
  }
  return result;
}

std::string toTokenName(std::string const &s) {
  std::string result;
  result.reserve(s.size());
  for (auto c : s) {
    result.push_back(toTokenChar(c));
  }
  return result;
}
