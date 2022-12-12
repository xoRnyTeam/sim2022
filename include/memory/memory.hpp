#ifndef MEMORY_HPP
#define MEMORY_HPP

#include <array>
#include <limits>

#include "support.hpp"

namespace sim {

/**
 * @brief Class representing physical memory
 */
class Memory final {
  static constexpr uint32_t MEM_SIZE = std::numeric_limits<paddress_t>::max();
  std::array<byte_t, MEM_SIZE> m_mem;

public:
  /// @brief Write \p nbytes bytes from \p src to \p addr in physical memory
  void writeRaw(paddress_t addr, const byte_t *src, size_t nbytes);
  /// @brief Read \p nbytes bytes from physical address \p addr to dst pointer
  void readRaw(paddress_t addr, byte_t *dst, size_t nbytes) const;

  /// @brief 
  /// @param addr 
  /// @param dst 
  /// @param nbytes 
  word_t readWord(paddress_t addr) const;

  /// @brief Fill memory with value
  void memfill(paddress_t addr, byte_t val, size_t nbytes);
};
//
} // namespace sim

#endif //! MEMORY_HPP