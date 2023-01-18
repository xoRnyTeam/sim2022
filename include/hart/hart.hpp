#ifndef HART_HPP
#define HART_HPP

#include "csr.hpp"
#include "memory/memory.hpp"
#include "support.hpp"
#include "utils/compiler.hpp"

#include <fstream>

/**
 * @brief
 *
 */
namespace sim {

/**
 * @brief
 *
 */
struct Hart {
  paddress_t pc{};
  std::array<reg_val_t, RegNum> regs{};
  CSRRegfile CSRegs;
  bool terminate = false;
  Memory memory{};
  //
  std::ofstream *trace_out = nullptr;
  //

public:
  void setPC(paddress_t val) { pc = val; }
  void addToPC(paddress_t offset) { setPC(getPC() + offset); }
  paddress_t getPC() const { return pc; }

  void setReg(reg_id_t reg, reg_val_t val) { regs[reg] = val; }
  reg_val_t getReg(reg_id_t reg) const { return regs[reg]; }
  /// @brief Set terminate flag true
  void setTerminate() { terminate = true; }
};

} // namespace sim

#endif //! HART_HPP