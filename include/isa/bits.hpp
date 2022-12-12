#ifndef BITS_HPP
#define BITS_HPP

#include <cstdint>
#include <iostream>
#include "support.hpp"

namespace sim {

constexpr word_t ones(word_t num) { return (1 << num) - 1; }

template <std::size_t msb, std::size_t lsb> constexpr word_t get_mask() {
  static_assert(msb >= lsb, "Error : illegal bits range");
  return ones(msb - lsb + 1) << lsb;
}

template <std::size_t msb, std::size_t lsb>
constexpr word_t slice(word_t word) {
  static_assert(msb >= lsb, "Error : illegal bits range");
  return (word & get_mask<msb, lsb>()) >> lsb;
}

} // namespace sim

#endif //! BITS_HPP