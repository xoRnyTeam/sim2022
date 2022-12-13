
#include "executor/executor.hpp"

#include "isa/instr.hpp"

namespace sim {

void Executor::exec(Instruction &instr, Hart &hart) const {
  m_exec_instr.at(instr.id)(instr, hart);
  // Incorrect for branch instructions
  hart.pc += 4;
}

void exec_ADD(Instruction &instr, Hart &hart) {
  hart.setReg(instr.rd, hart.getReg(instr.rs1) + hart.getReg(instr.rs2));
} //!

void exec_ADDI(Instruction &instr, Hart &hart) {
  hart.setReg(instr.rd, instr.imm + hart.getReg(instr.rs1));
} //!

void exec_AND(Instruction &instr, Hart &hart) {} //!

void exec_ANDI(Instruction &instr, Hart &hart) {} //!

void exec_AUIPC(Instruction &instr, Hart &hart) {} //!

void exec_BEQ(Instruction &instr, Hart &hart) {} //!

void exec_BGE(Instruction &instr, Hart &hart) {} //!

void exec_BGEU(Instruction &instr, Hart &hart) {} //!

void exec_BLT(Instruction &instr, Hart &hart) {} //!

void exec_BLTU(Instruction &instr, Hart &hart) {} //!

void exec_BNE(Instruction &instr, Hart &hart) {} //!

void exec_EBREAK(Instruction &instr, Hart &hart) {} //!

void exec_ECALL(Instruction &instr, Hart &hart) { hart.setTerminate(); } //!

void exec_FENCE(Instruction &instr, Hart &hart) {} //!

void exec_FENCE_TSO(Instruction &instr, Hart &hart) {} //!

void exec_JAL(Instruction &instr, Hart &hart) {} //!

void exec_JALR(Instruction &instr, Hart &hart) {} //!

void exec_LB(Instruction &instr, Hart &hart) {} //!

void exec_LBU(Instruction &instr, Hart &hart) {} //!

void exec_LH(Instruction &instr, Hart &hart) {} //!

void exec_LHU(Instruction &instr, Hart &hart) {} //!

void exec_LUI(Instruction &instr, Hart &hart) {} //!

void exec_LW(Instruction &instr, Hart &hart) {
  vaddress_t address = hart.getReg(instr.rs1) + instr.imm;
  word_t value = hart.memory.readWord(address);
  hart.setReg(instr.rd, value);
} //!

void exec_OR(Instruction &instr, Hart &hart) {} //!

void exec_ORI(Instruction &instr, Hart &hart) {} //!

void exec_PAUSE(Instruction &instr, Hart &hart) {} //!

void exec_SB(Instruction &instr, Hart &hart) {} //!

void exec_SBREAK(Instruction &instr, Hart &hart) {} //!

void exec_SCALL(Instruction &instr, Hart &hart) {} //!

void exec_SH(Instruction &instr, Hart &hart) {} //!

void exec_SLL(Instruction &instr, Hart &hart) {} //!

void exec_SLT(Instruction &instr, Hart &hart) {} //!

void exec_SLTI(Instruction &instr, Hart &hart) {} //!

void exec_SLTIU(Instruction &instr, Hart &hart) {} //!

void exec_SLTU(Instruction &instr, Hart &hart) {} //!

void exec_SRA(Instruction &instr, Hart &hart) {} //!

void exec_SRL(Instruction &instr, Hart &hart) {} //!

void exec_SUB(Instruction &instr, Hart &hart) {
  hart.setReg(instr.rd, hart.getReg(instr.rs1) - hart.getReg(instr.rs2));
} //!

void exec_SW(Instruction &instr, Hart &hart) {
  vaddress_t address = hart.getReg(instr.rs1) + instr.imm;
  word_t value = hart.getReg(instr.rs2);
  hart.memory.writeWord(address, value);
} //!

void exec_XOR(Instruction &instr, Hart &hart) {} //!

void exec_XORI(Instruction &instr, Hart &hart) {} //!

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
