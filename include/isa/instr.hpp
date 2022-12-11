#ifndef INSTR_HPP
#define INSTR_HPP

#include "isa/bits.hpp"
#include "isa/isa.hpp"

namespace sim {

struct Instruction final {
  op_t rs1{};
  op_t rs2{};
  op_t rs3{};
  //
  op_t rd{};
  //
  imm_t imm{};

  InstrId id = InstrId::NONE;
};
} // namespace sim

#endif //! INSTR_HPP