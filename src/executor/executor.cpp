
#include "executor/executor.hpp"
#include "isa/instr.hpp"

namespace sim {

void exec_ADD(Instruction &instr) {} //!

void exec_ADDI(Instruction &instr) {} //!

void exec_AND(Instruction &instr) {} //!

void exec_ANDI(Instruction &instr) {} //!

void exec_AUIPC(Instruction &instr) {} //!

void exec_BEQ(Instruction &instr) {} //!

void exec_BGE(Instruction &instr) {} //!

void exec_BGEU(Instruction &instr) {} //!

void exec_BLT(Instruction &instr) {} //!

void exec_BLTU(Instruction &instr) {} //!

void exec_BNE(Instruction &instr) {} //!

void exec_EBREAK(Instruction &instr) {} //!

void exec_ECALL(Instruction &instr) {} //!

void exec_FENCE(Instruction &instr) {} //!

void exec_FENCE_TSO(Instruction &instr) {} //!

void exec_JAL(Instruction &instr) {} //!

void exec_JALR(Instruction &instr) {} //!

void exec_LB(Instruction &instr) {} //!

void exec_LBU(Instruction &instr) {} //!

void exec_LH(Instruction &instr) {} //!

void exec_LHU(Instruction &instr) {} //!

void exec_LUI(Instruction &instr) {} //!

void exec_LW(Instruction &instr) {} //!

void exec_OR(Instruction &instr) {} //!

void exec_ORI(Instruction &instr) {} //!

void exec_PAUSE(Instruction &instr) {} //!

void exec_SB(Instruction &instr) {} //!

void exec_SBREAK(Instruction &instr) {} //!

void exec_SCALL(Instruction &instr) {} //!

void exec_SH(Instruction &instr) {} //!

void exec_SLL(Instruction &instr) {} //!

void exec_SLT(Instruction &instr) {} //!

void exec_SLTI(Instruction &instr) {} //!

void exec_SLTIU(Instruction &instr) {} //!

void exec_SLTU(Instruction &instr) {} //!

void exec_SRA(Instruction &instr) {} //!

void exec_SRL(Instruction &instr) {} //!

void exec_SUB(Instruction &instr) {} //!

void exec_SW(Instruction &instr) {} //!

void exec_XOR(Instruction &instr) {} //!

void exec_XORI(Instruction &instr) {} //!

Executor::Executor()
    : m_exec_instr{
          {InstrId::ADD, exec_ADD},
          {InstrId::ADDI, exec_ADDI},
          {InstrId::AND, exec_AND},
          {InstrId::ANDI, exec_ANDI},
          {InstrId::AUIPC, exec_AUIPC},
          {InstrId::BEQ, exec_BEQ},
          {InstrId::BGE, exec_BGE},
          {InstrId::BGEU, exec_BGEU},
          {InstrId::BLT, exec_BLT},
          {InstrId::BLTU, exec_BLTU},
          {InstrId::BNE, exec_BNE},
          {InstrId::EBREAK, exec_EBREAK},
          {InstrId::ECALL, exec_ECALL},
          {InstrId::FENCE, exec_FENCE},
          {InstrId::FENCE_TSO, exec_FENCE_TSO},
          {InstrId::JAL, exec_JAL},
          {InstrId::JALR, exec_JALR},
          {InstrId::LB, exec_LB},
          {InstrId::LBU, exec_LBU},
          {InstrId::LH, exec_LH},
          {InstrId::LHU, exec_LHU},
          {InstrId::LUI, exec_LUI},
          {InstrId::LW, exec_LW},
          {InstrId::OR, exec_OR},
          {InstrId::ORI, exec_ORI},
          {InstrId::PAUSE, exec_PAUSE},
          {InstrId::SB, exec_SB},
          {InstrId::SBREAK, exec_SBREAK},
          {InstrId::SCALL, exec_SCALL},
          {InstrId::SH, exec_SH},
          {InstrId::SLL, exec_SLL},
          {InstrId::SLT, exec_SLT},
          {InstrId::SLTI, exec_SLTI},
          {InstrId::SLTIU, exec_SLTIU},
          {InstrId::SLTU, exec_SLTU},
          {InstrId::SRA, exec_SRA},
          {InstrId::SRL, exec_SRL},
          {InstrId::SUB, exec_SUB},
          {InstrId::SW, exec_SW},
          {InstrId::XOR, exec_XOR},
          {InstrId::XORI, exec_XORI},
      } {}

} // namespace sim
