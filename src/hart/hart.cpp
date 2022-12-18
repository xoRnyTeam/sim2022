#include "hart/hart.hpp"

#include <fstream>
#include <ios>

namespace sim {

void Hart::setReg(reg_id_t reg, reg_val_t val) {
  regs[reg] = val;
  if (trace_out) {
    (*trace_out) << "x" << reg << " = 0x" << std::hex << val << '\n';
  }
}

reg_val_t Hart::getReg(reg_id_t reg) const { return regs[reg]; }

void Hart::setPC(paddress_t val) {
  pc = val;
  if (trace_out) {
    (*trace_out) << "PC = 0x" << std::hex << pc << '\n';
  }
}

void Hart::addToPC(paddress_t offset) { setPC(getPC() + offset); }

paddress_t Hart::getPC() const { return pc; }

} // namespace sim