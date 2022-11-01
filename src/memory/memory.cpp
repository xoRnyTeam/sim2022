#include "memory/memory.hpp"

#include <assert.h>
#include <cstring>

namespace sim {

void Memory::writeRaw(paddress_t addr, const byte_t *src, size_t nbytes) {
  assert(src && "Invalid src pointer");
  std::memcpy(m_mem.data() + addr, reinterpret_cast<const char *>(src),
              nbytes * sizeof(byte_t));
}

void Memory::readRaw(paddress_t addr, byte_t *dst, size_t nbytes) const {
  assert(dst && "Invalid dst pointer");
  std::memcpy(dst, m_mem.data() + addr, nbytes);
}

void Memory::memfill(paddress_t addr, byte_t val, size_t nbytes) {
  std::memset(m_mem.data() + addr, val, nbytes);
}

}; // namespace sim
