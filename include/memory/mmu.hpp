#ifndef MEM_MANAGEMENT_UNIT_HPP
#define MEM_MANAGEMENT_UNIT_HPP

#include "memory/memory.hpp"
#include "riscv-opcodes/encoding.out.h"
#include "support.hpp"
#include <type_traits>

namespace sim {

const static size_t PageSize = 4096;

struct Hart;

class MMU {
  Hart *m_hart;
  Memory m_memory;

public:
  enum TranslationModes : uint32_t {
    Bare = SATP_MODE_OFF,
    Sv32 = SATP_MODE_SV32,
    // 64 bits translations, currently unsupported
    Sv39 = SATP_MODE_SV39,
    Sv48 = SATP_MODE_SV48,
    Sv57 = SATP_MODE_SV57,
    Sv64 = SATP_MODE_SV64
  };

  MMU(Hart &hart);
  paddress_t translate(vaddress_t address);
  TranslationModes getTranslationMode() const;
  paddress_t translateBare(vaddress_t address) { return address; }
  size_t getPhysMemSize() const;
  Memory &getMemory() { return m_memory; }

  // This can't be const, becuase reads will affect MMU TLB state
  template <typename T> T read(vaddress_t address) {
    paddress_t paddr = translate(address);
    return m_memory.read<T>(paddr);
  }

  template <typename T> void write(vaddress_t address, T value) {
    paddress_t paddr = translate(address);
    m_memory.write<T>(paddr, value);
  }
};

}; // namespace sim

#endif