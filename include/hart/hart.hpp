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
  bool termiante = false;
  Memory memory{};
};

} // namespace sim

#endif //! HART_HPP