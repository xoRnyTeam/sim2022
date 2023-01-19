/* This file defines Control and Status Registers.
 *  For more information refer to
 *  "The RISC-V Instruction Set Manual
 *  Volume II: Privileged Architecture"
 */

#ifndef CSR_HPP
#define CSR_HPP

#include "isa/bits.hpp"
#include "riscv-opcodes/encoding.out.h"
#include "support.hpp"
#include <vector>
#include <assert.h>

namespace sim {

constexpr uint16_t CSRegNum = 4096; // 12 bit encoding space
constexpr uint8_t MXLEN = 32;

using csr_num_t = uint16_t;
using csr_val_t = uint32_t;

static_assert(MXLEN == sizeof(csr_val_t) * 8);

class CSRRegfile {
  std::vector<csr_val_t> m_regs;

public:
  CSRRegfile();
  // clang-format off
  enum PrivilegeLvl : uint8_t {
    User       = 0b00,
    Supervisor = 0b01,
    Hypervisor = 0b10,  // Reserved
    Machine    = 0b11
  };
  // clang-format on

  static bool isRdOnly(csr_num_t reg) {
    assert(reg < CSRegNum && "Invalid CSR number");
    // Accroding to SPECs register number encoded in 12 bits.
    // I rely on that, assuming that 13 and higher bits are zeroes
    return reg >> 10 == 0b11;
  }

  static bool isRdWr(csr_num_t reg) { return !isRdOnly(reg); }

  /// @brief Get the lowest privilege level for CSR \p reg
  static uint8_t getRegPrivilegeLvl(csr_num_t reg) { return slice<9, 8>(reg); }

  // Register access should be controlled according to CSR Field Specifications
  // WPRI, WLRL, WARL etc.
  // Currently this is unsupported
  csr_val_t getReg(csr_num_t reg) { return m_regs[reg]; }
  void setReg(csr_num_t reg, csr_val_t) { m_regs[reg]; }

  /// @brief check if reg number is correct CSR address
  bool isValidCSR(csr_num_t reg) {
    // Attempt to access non-existent CSR should produce illegal instruction
    // exception
    return reg < CSRegNum;
  }
};

}; // namespace sim

#endif