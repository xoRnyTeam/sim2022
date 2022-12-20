#ifndef MEMORY_HPP
#define MEMORY_HPP

#include <array>
#include <limits>
#include <vector>

#include "support.hpp"

namespace sim {

/**
 * @brief Class representing physical memory
 */
class Memory final {
  static constexpr size_t MEM_SIZE = std::numeric_limits<paddress_t>::max();
  std::vector<byte_t> m_mem;

public:
  Memory() { m_mem.resize(MEM_SIZE + 16); }
  /// @brief Write \p nbytes bytes from \p src to \p addr in physical memory
  void writeRaw(paddress_t addr, const byte_t *src, size_t nbytes);
  /// @brief Read \p nbytes bytes from physical address \p addr to dst pointer
  void readRaw(paddress_t addr, byte_t *dst, size_t nbytes) const;

  /// @brief
  /// @param addr
  word_t readWord(paddress_t addr) const;
  hword_t readHWord(paddress_t addr) const;
  bword_t readBWord(paddress_t addr) const;

  void writeWord(paddress_t addr, word_t value);
  void writeHWord(paddress_t addr, hword_t value);
  void writeBWord(paddress_t addr, bword_t value);

  /// @brief Fill memory with value
  void memfill(paddress_t addr, byte_t val, size_t nbytes);
};
//
} // namespace sim

#endif //! MEMORY_HPP