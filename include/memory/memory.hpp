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
  std::vector<byte_t> m_mem;

public:
  static constexpr size_t MEM_SIZE = std::numeric_limits<paddress_t>::max();
  Memory() { m_mem.resize(MEM_SIZE + 16); }
  /// @brief Write \p nbytes bytes from \p src to \p addr in physical memory
  void writeRaw(paddress_t addr, const byte_t *src, size_t nbytes);
  /// @brief Read \p nbytes bytes from physical address \p addr to dst pointer
  void readRaw(paddress_t addr, byte_t *dst, size_t nbytes) const;

  template <typename T> T read(paddress_t addr) const {
    static_assert(std::is_integral<T>::value, "Integral type required");
    T cur_val = *reinterpret_cast<const T *>(m_mem.data() + addr);
    return cur_val;
  }

  template <typename T> void write(paddress_t addr, T value) {
    static_assert(std::is_integral<T>::value, "Integral type required");
    *reinterpret_cast<T *>(m_mem.data() + addr) = value;
  }

  /// @brief Fill memory with value
  void memfill(paddress_t addr, byte_t val, size_t nbytes);
};
//
} // namespace sim

#endif //! MEMORY_HPP