#ifndef HART_HPP
#define HART_HPP

#include "memory/memory.hpp"
#include "support.hpp"

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
  bool terminate = false;
  Memory memory{};
  //
  std::ofstream *trace_out = nullptr;
  //

  // private:
  void setPC(paddress_t val);
  void addToPC(paddress_t offset);
  paddress_t getPC() const;

  void setReg(reg_id_t reg, reg_val_t val);
  reg_val_t getReg(reg_id_t reg) const;
  /// @brief Set terminate flag true
  void setTerminate() { terminate = true; }
};

} // namespace sim

#endif //! HART_HPP