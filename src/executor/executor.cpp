
#include "executor/executor.hpp"

#include "isa/instr.hpp"

#include <bit>

namespace sim {

void Executor::exec(Instruction &instr, Hart &hart) const {
  m_exec_instr.at(instr.id)(instr, hart);
}

void exec_ADD(Instruction &instr, Hart &hart) {
  hart.setReg(instr.rd, hart.getReg(instr.rs1) + hart.getReg(instr.rs2));
  hart.pc += 4;
} //!

void exec_ADDI(Instruction &instr, Hart &hart) {
  hart.setReg(instr.rd, instr.imm + hart.getReg(instr.rs1));
  hart.pc += 4;
} //!

void exec_AND(Instruction &instr, Hart &hart) {
  hart.setReg(instr.rd, hart.getReg(instr.rs1) & hart.getReg(instr.rs2));
  hart.pc += 4;
} //!

void exec_ANDI(Instruction &instr, Hart &hart) {
  hart.setReg(instr.rd, hart.getReg(instr.rs1) & std::bit_cast<uint32_t>(instr.imm));
  hart.pc += 4;
} //!

void exec_AUIPC(Instruction &instr, Hart &hart) {
  hart.setReg(instr.rd, instr.imm + hart.pc);
  hart.pc += 4;
} //!

void exec_BEQ(Instruction &instr, Hart &hart) {
  bool take = hart.getReg(instr.rs1) == hart.getReg(instr.rs2);
  hart.pc += (take) ? instr.imm : 4;
} //!

void exec_BGE(Instruction &instr, Hart &hart) {
  bool take = std::bit_cast<int32_t>(hart.getReg(instr.rs1)) >= std::bit_cast<int32_t>(hart.getReg(instr.rs2));
  hart.pc += (take) ? instr.imm : 4;
} //!

void exec_BGEU(Instruction &instr, Hart &hart) {
  bool take = std::bit_cast<uint32_t>(hart.getReg(instr.rs1)) >= std::bit_cast<uint32_t>(hart.getReg(instr.rs2));
  hart.pc += (take) ? instr.imm : 4;
} //!

void exec_BLT(Instruction &instr, Hart &hart) {
  bool take = std::bit_cast<int32_t>(hart.getReg(instr.rs1)) < std::bit_cast<int32_t>(hart.getReg(instr.rs2));
  hart.pc += (take) ? instr.imm : 4;
} //!

void exec_BLTU(Instruction &instr, Hart &hart) {
  bool take = std::bit_cast<uint32_t>(hart.getReg(instr.rs1)) < std::bit_cast<uint32_t>(hart.getReg(instr.rs2));
  hart.pc += (take) ? instr.imm : 4;
} //!

void exec_BNE(Instruction &instr, Hart &hart) {
  bool take = hart.getReg(instr.rs1) != hart.getReg(instr.rs2);
  hart.pc += (take) ? instr.imm : 4;
} //!

void exec_EBREAK(Instruction &instr, Hart &hart) { hart.setTerminate(); } //!

void exec_ECALL(Instruction &instr, Hart &hart) { hart.setTerminate(); } //!

void exec_FENCE(Instruction &instr, Hart &hart) { hart.pc += 4; } //!

void exec_FENCE_TSO(Instruction &instr, Hart &hart) { hart.pc += 4; } //!

void exec_JAL(Instruction &instr, Hart &hart) {
  auto pc_next = hart.pc + 4;
  hart.pc += instr.imm;
  hart.setReg(instr.rd, pc_next);
} //!

void exec_JALR(Instruction &instr, Hart &hart) {
  auto pc_next = hart.pc + 4;
  hart.pc = (hart.getReg(instr.rs1) + instr.imm) & (~0b01);
  hart.setReg(instr.rd, pc_next);
} //!

void exec_LB(Instruction &instr, Hart &hart) {
  vaddress_t address = hart.getReg(instr.rs1) + instr.imm;
  word_t value = hart.memory.readBWord(address);
  //                    sign extend value
  hart.setReg(instr.rd, std::bit_cast<int32_t>(value << 24) >> 24);
  hart.pc += 4;
} //!

void exec_LBU(Instruction &instr, Hart &hart) {
  vaddress_t address = hart.getReg(instr.rs1) + instr.imm;
  word_t value = 0xFF & hart.memory.readBWord(address);
  hart.setReg(instr.rd, value);
  hart.pc += 4;
} //!

void exec_LH(Instruction &instr, Hart &hart) {
  vaddress_t address = hart.getReg(instr.rs1) + instr.imm;
  word_t value = hart.memory.readHWord(address);
  //                    sign extend value
  hart.setReg(instr.rd, std::bit_cast<int32_t>(value << 16) >> 16);
  hart.pc += 4;
} //!

void exec_LHU(Instruction &instr, Hart &hart) {
  vaddress_t address = hart.getReg(instr.rs1) + instr.imm;
  word_t value = 0xFFFF & hart.memory.readHWord(address);
  hart.setReg(instr.rd, value);
  hart.pc += 4;
} //!

void exec_LUI(Instruction &instr, Hart &hart) {
  hart.setReg(instr.rd, std::bit_cast<uint32_t>(instr.imm));
  hart.pc += 4;
} //!

void exec_LW(Instruction &instr, Hart &hart) {
  vaddress_t address = hart.getReg(instr.rs1) + instr.imm;
  word_t value = hart.memory.readWord(address);
  hart.setReg(instr.rd, value);
  hart.pc += 4;
} //!

void exec_OR(Instruction &instr, Hart &hart) {
  hart.setReg(instr.rd, hart.getReg(instr.rs1) | hart.getReg(instr.rs2));
  hart.pc += 4;
} //!

void exec_ORI(Instruction &instr, Hart &hart) {
  hart.setReg(instr.rd, hart.getReg(instr.rs1) | std::bit_cast<uint32_t>(instr.imm));
  hart.pc += 4;
} //!

void exec_PAUSE(Instruction &instr, Hart &hart) { hart.setTerminate(); } //!

void exec_SB(Instruction &instr, Hart &hart) {
  vaddress_t address = hart.getReg(instr.rs1) + instr.imm;
  bword_t value = static_cast<bword_t>(hart.getReg(instr.rs2) & 0xFF);
  hart.memory.writeBWord(address, value);
  hart.pc += 4;
} //!

void exec_SBREAK(Instruction &instr, Hart &hart) { hart.setTerminate(); } //!

void exec_SCALL(Instruction &instr, Hart &hart) { hart.setTerminate(); } //!

void exec_SH(Instruction &instr, Hart &hart) {
  vaddress_t address = hart.getReg(instr.rs1) + instr.imm;
  hword_t value = static_cast<hword_t>(hart.getReg(instr.rs2) & 0xFFFF);
  hart.memory.writeHWord(address, value);
  hart.pc += 4;
} //!

void exec_SLL(Instruction &instr, Hart &hart) {
  //shifts on the value in register rs1 by the shift amount held in the lower 5 bits of register rs2
  hart.setReg(instr.rd, hart.getReg(instr.rs1) << (hart.getReg(instr.rs2) & 0b011111));
  hart.pc += 4;
} //!

void exec_SLT(Instruction &instr, Hart &hart) {
  // sign compare
  hart.setReg(instr.rd, std::bit_cast<int32_t>(hart.getReg(instr.rs1)) < std::bit_cast<int32_t>(hart.getReg(instr.rs2)));
  hart.pc += 4;
} //!

void exec_SLTI(Instruction &instr, Hart &hart) {
  hart.setReg(instr.rd, std::bit_cast<int32_t>(hart.getReg(instr.rs1)) < std::bit_cast<int32_t>(instr.imm));
  hart.pc += 4;
} //!

void exec_SLTIU(Instruction &instr, Hart &hart) {
  hart.setReg(instr.rd, std::bit_cast<uint32_t>(hart.getReg(instr.rs1)) < std::bit_cast<uint32_t>(instr.imm));
  hart.pc += 4;
} //!

void exec_SLTU(Instruction &instr, Hart &hart) {
  // unsign cmp
  hart.setReg(instr.rd, hart.getReg(instr.rs1) < hart.getReg(instr.rs2));
  hart.pc += 4;
} //!

void exec_SRA(Instruction &instr, Hart &hart) {
  // arifmetic shifts on the value in register rs1 by the shift amount held in the lower 5 bits of register rs2
  hart.setReg(instr.rd, ((int)hart.getReg(instr.rs1)) >> (hart.getReg(instr.rs2) & 0b011111));
  hart.pc += 4;
} //!

void exec_SRL(Instruction &instr, Hart &hart) {
  //shifts on the value in register rs1 by the shift amount held in the lower 5 bits of register rs2
  hart.setReg(instr.rd, hart.getReg(instr.rs1) >> (hart.getReg(instr.rs2) & 0b011111));
  hart.pc += 4;
} //!

void exec_SUB(Instruction &instr, Hart &hart) {
  hart.setReg(instr.rd, hart.getReg(instr.rs1) - hart.getReg(instr.rs2));
  hart.pc += 4;
} //!

void exec_SW(Instruction &instr, Hart &hart) {
  vaddress_t address = hart.getReg(instr.rs1) + instr.imm;
  word_t value = hart.getReg(instr.rs2);
  hart.memory.writeWord(address, value);
  hart.pc += 4;
} //!

void exec_XOR(Instruction &instr, Hart &hart) {
  hart.setReg(instr.rd, hart.getReg(instr.rs1) ^ hart.getReg(instr.rs2));
  hart.pc += 4;
} //!

void exec_XORI(Instruction &instr, Hart &hart) {
  hart.setReg(instr.rd, hart.getReg(instr.rs1) ^ std::bit_cast<uint32_t>(instr.imm));
  hart.pc += 4;
} //!

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
