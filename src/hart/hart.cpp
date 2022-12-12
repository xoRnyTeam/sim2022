#include "hart/hart.hpp"

namespace sim {

void Hart::setReg(reg_id_t reg, reg_val_t val) { regs[reg] = val; }

reg_val_t Hart::getReg(reg_id_t reg) const { return regs[reg]; }

} // namespace sim