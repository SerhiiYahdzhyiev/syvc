#include "headers/Utils.h"

std::string getHashFrom(const std::string& input) {
  std::size_t hash = Constants::FNV_OFFSET_BASIS;

  for (char ch : input) {
      hash ^= static_cast<std::size_t>(ch);
      hash *= Constants::FNV_PRIME;
  }

  std::stringstream stream;

  stream << hash;

  return stream.str();
}
