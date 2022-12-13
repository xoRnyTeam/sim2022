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

word_t Memory::readWord(paddress_t addr) const {
  word_t cur_word = *reinterpret_cast<const word_t *>(m_mem.data() + addr);
  return cur_word;
}

void Memory::memfill(paddress_t addr, byte_t val, size_t nbytes) {
  std::memset(m_mem.data() + addr, val, nbytes);
}

void Memory::writeWord(paddress_t addr, word_t value) {
  *reinterpret_cast<word_t *>(m_mem.data() + addr) = value;
}

}; // namespace sim
