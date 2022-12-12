#ifndef HART_HPP
#define HART_HPP

#include "memory/memory.hpp"
#include "support.hpp"

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
  bool terminate = false;
  Memory memory{};

  void setReg(reg_id_t reg, reg_val_t val);
  reg_val_t getReg(reg_id_t reg) const;
  /// @brief Set terminate flag true
  void setTerminate() { terminate = true; }
};

} // namespace sim

#endif //! HART_HPP