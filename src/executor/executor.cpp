
#include "executor/executor.hpp"

#include "isa/instr.hpp"

#include <bit>

namespace sim {

void Executor::exec(Instruction &instr, Hart &hart) const {
  m_exec_instr[static_cast<uint8_t>(instr.id)](instr, hart);
}

void exec_ADD(Instruction &instr, Hart &hart) {
  hart.setReg(instr.rd, hart.getReg(instr.rs1) + hart.getReg(instr.rs2));
  hart.addToPC(4);
} //!

void exec_ADDI(Instruction &instr, Hart &hart) {
  hart.setReg(instr.rd, instr.imm + hart.getReg(instr.rs1));
  hart.addToPC(4);
} //!

void exec_AND(Instruction &instr, Hart &hart) {
  hart.setReg(instr.rd, hart.getReg(instr.rs1) & hart.getReg(instr.rs2));
  hart.addToPC(4);
} //!

void exec_ANDI(Instruction &instr, Hart &hart) {
  hart.setReg(instr.rd, hart.getReg(instr.rs1) & std::bit_cast<uint32_t>(instr.imm));
  hart.addToPC(4);
} //!

void exec_AUIPC(Instruction &instr, Hart &hart) {
  hart.setReg(instr.rd, instr.imm + hart.getPC());
  hart.addToPC(4);
} //!

void exec_BEQ(Instruction &instr, Hart &hart) {
  bool take = hart.getReg(instr.rs1) == hart.getReg(instr.rs2);
  hart.addToPC((take) ? instr.imm : 4);
} //!

void exec_BGE(Instruction &instr, Hart &hart) {
  bool take = std::bit_cast<int32_t>(hart.getReg(instr.rs1)) >= std::bit_cast<int32_t>(hart.getReg(instr.rs2));
  hart.addToPC((take) ? instr.imm : 4);
} //!

void exec_BGEU(Instruction &instr, Hart &hart) {
  bool take = std::bit_cast<uint32_t>(hart.getReg(instr.rs1)) >= std::bit_cast<uint32_t>(hart.getReg(instr.rs2));
  hart.addToPC((take) ? instr.imm : 4);
} //!

void exec_BLT(Instruction &instr, Hart &hart) {
  bool take = std::bit_cast<int32_t>(hart.getReg(instr.rs1)) < std::bit_cast<int32_t>(hart.getReg(instr.rs2));
  hart.addToPC((take) ? instr.imm : 4);
} //!

void exec_BLTU(Instruction &instr, Hart &hart) {
  bool take = std::bit_cast<uint32_t>(hart.getReg(instr.rs1)) < std::bit_cast<uint32_t>(hart.getReg(instr.rs2));
  hart.addToPC((take) ? instr.imm : 4);
} //!

void exec_BNE(Instruction &instr, Hart &hart) {
  bool take = hart.getReg(instr.rs1) != hart.getReg(instr.rs2);
  hart.addToPC((take) ? instr.imm : 4);
} //!

void exec_EBREAK(Instruction &instr, Hart &hart) { hart.setTerminate(); } //!

void exec_ECALL(Instruction &instr, Hart &hart) { hart.setTerminate(); } //!

void exec_FENCE(Instruction &instr, Hart &hart) { hart.addToPC(4); } //!

void exec_FENCE_TSO(Instruction &instr, Hart &hart) { hart.addToPC(4); } //!

void exec_JAL(Instruction &instr, Hart &hart) {
  auto pc_next = hart.pc + 4;
  hart.addToPC(instr.imm);
  hart.setReg(instr.rd, pc_next);
} //!

void exec_JALR(Instruction &instr, Hart &hart) {
  auto pc_next = hart.pc + 4;
  hart.setPC((std::bit_cast<int32_t>(hart.getReg(instr.rs1)) + instr.imm) &
             (~0b01));
  hart.setReg(instr.rd, pc_next);
} //!

void exec_LB(Instruction &instr, Hart &hart) {
  vaddress_t address = hart.getReg(instr.rs1) + instr.imm;
  word_t value = hart.memory.readBWord(address);
  //                    sign extend value
  hart.setReg(instr.rd, std::bit_cast<int32_t>(value << 24) >> 24);
  hart.addToPC(4);
} //!

void exec_LBU(Instruction &instr, Hart &hart) {
  vaddress_t address = hart.getReg(instr.rs1) + instr.imm;
  word_t value = 0xFF & hart.memory.readBWord(address);
  hart.setReg(instr.rd, value);
  hart.addToPC(4);
} //!

void exec_LH(Instruction &instr, Hart &hart) {
  vaddress_t address = hart.getReg(instr.rs1) + instr.imm;
  word_t value = hart.memory.readHWord(address);
  //                    sign extend value
  hart.setReg(instr.rd, std::bit_cast<int32_t>(value << 16) >> 16);
  hart.addToPC(4);
} //!

void exec_LHU(Instruction &instr, Hart &hart) {
  vaddress_t address = hart.getReg(instr.rs1) + instr.imm;
  word_t value = 0xFFFF & hart.memory.readHWord(address);
  hart.setReg(instr.rd, value);
  hart.addToPC(4);
} //!

void exec_LUI(Instruction &instr, Hart &hart) {
  hart.setReg(instr.rd, std::bit_cast<uint32_t>(instr.imm));
  hart.addToPC(4);
} //!

void exec_LW(Instruction &instr, Hart &hart) {
  vaddress_t address = hart.getReg(instr.rs1) + instr.imm;
  word_t value = hart.memory.readWord(address);
  hart.setReg(instr.rd, value);
  hart.addToPC(4);
} //!

void exec_OR(Instruction &instr, Hart &hart) {
  hart.setReg(instr.rd, hart.getReg(instr.rs1) | hart.getReg(instr.rs2));
  hart.addToPC(4);
} //!

void exec_ORI(Instruction &instr, Hart &hart) {
  hart.setReg(instr.rd, hart.getReg(instr.rs1) | std::bit_cast<uint32_t>(instr.imm));
  hart.addToPC(4);
} //!

void exec_PAUSE(Instruction &instr, Hart &hart) { hart.setTerminate(); } //!

void exec_SB(Instruction &instr, Hart &hart) {
  vaddress_t address = hart.getReg(instr.rs1) + instr.imm;
  bword_t value = static_cast<bword_t>(hart.getReg(instr.rs2) & 0xFF);
  hart.memory.writeBWord(address, value);
  hart.addToPC(4);
} //!

void exec_SBREAK(Instruction &instr, Hart &hart) { hart.setTerminate(); } //!

void exec_SCALL(Instruction &instr, Hart &hart) { hart.setTerminate(); } //!

void exec_SH(Instruction &instr, Hart &hart) {
  vaddress_t address = hart.getReg(instr.rs1) + instr.imm;
  hword_t value = static_cast<hword_t>(hart.getReg(instr.rs2) & 0xFFFF);
  hart.memory.writeHWord(address, value);
  hart.addToPC(4);
} //!

void exec_SLL(Instruction &instr, Hart &hart) {
  //shifts on the value in register rs1 by the shift amount held in the lower 5 bits of register rs2
  hart.setReg(instr.rd, hart.getReg(instr.rs1) << (hart.getReg(instr.rs2) & 0b011111));
  hart.addToPC(4);
} //!

void exec_SLT(Instruction &instr, Hart &hart) {
  // sign compare
  hart.setReg(instr.rd, std::bit_cast<int32_t>(hart.getReg(instr.rs1)) < std::bit_cast<int32_t>(hart.getReg(instr.rs2)));
  hart.addToPC(4);
} //!

void exec_SLTI(Instruction &instr, Hart &hart) {
  hart.setReg(instr.rd, std::bit_cast<int32_t>(hart.getReg(instr.rs1)) < std::bit_cast<int32_t>(instr.imm));
  hart.addToPC(4);
} //!

void exec_SLTIU(Instruction &instr, Hart &hart) {
  hart.setReg(instr.rd, std::bit_cast<uint32_t>(hart.getReg(instr.rs1)) < std::bit_cast<uint32_t>(instr.imm));
  hart.addToPC(4);
} //!

void exec_SLTU(Instruction &instr, Hart &hart) {
  // unsign cmp
  hart.setReg(instr.rd, hart.getReg(instr.rs1) < hart.getReg(instr.rs2));
  hart.addToPC(4);
} //!

void exec_SRA(Instruction &instr, Hart &hart) {
  // arifmetic shifts on the value in register rs1 by the shift amount held in the lower 5 bits of register rs2
  hart.setReg(instr.rd, ((int)hart.getReg(instr.rs1)) >> (hart.getReg(instr.rs2) & 0b011111));
  hart.addToPC(4);
} //!

void exec_SRL(Instruction &instr, Hart &hart) {
  //shifts on the value in register rs1 by the shift amount held in the lower 5 bits of register rs2
  hart.setReg(instr.rd, hart.getReg(instr.rs1) >> (hart.getReg(instr.rs2) & 0b011111));
  hart.addToPC(4);
} //!

void exec_SUB(Instruction &instr, Hart &hart) {
  hart.setReg(instr.rd, hart.getReg(instr.rs1) - hart.getReg(instr.rs2));
  hart.addToPC(4);
} //!

void exec_SW(Instruction &instr, Hart &hart) {
  vaddress_t address = hart.getReg(instr.rs1) + instr.imm;
  word_t value = hart.getReg(instr.rs2);
  hart.memory.writeWord(address, value);
  hart.addToPC(4);
} //!

void exec_XOR(Instruction &instr, Hart &hart) {
  hart.setReg(instr.rd, hart.getReg(instr.rs1) ^ hart.getReg(instr.rs2));
  hart.addToPC(4);
} //!

void exec_XORI(Instruction &instr, Hart &hart) {
  hart.setReg(instr.rd, hart.getReg(instr.rs1) ^ std::bit_cast<uint32_t>(instr.imm));
  hart.addToPC(4);
} //!

Executor::Executor() {
  m_exec_instr[static_cast<uint8_t>(InstrId::ADD)] = exec_ADD;
  m_exec_instr[static_cast<uint8_t>(InstrId::ADDI)] = exec_ADDI;
  m_exec_instr[static_cast<uint8_t>(InstrId::AND)] = exec_AND;
  m_exec_instr[static_cast<uint8_t>(InstrId::ANDI)] = exec_ANDI;
  m_exec_instr[static_cast<uint8_t>(InstrId::AUIPC)] = exec_AUIPC;
  m_exec_instr[static_cast<uint8_t>(InstrId::BEQ)] = exec_BEQ;
  m_exec_instr[static_cast<uint8_t>(InstrId::BGE)] = exec_BGE;
  m_exec_instr[static_cast<uint8_t>(InstrId::BGEU)] = exec_BGEU;
  m_exec_instr[static_cast<uint8_t>(InstrId::BLT)] = exec_BLT;
  m_exec_instr[static_cast<uint8_t>(InstrId::BLTU)] = exec_BLTU;
  m_exec_instr[static_cast<uint8_t>(InstrId::BNE)] = exec_BNE;
  m_exec_instr[static_cast<uint8_t>(InstrId::EBREAK)] = exec_EBREAK;
  m_exec_instr[static_cast<uint8_t>(InstrId::ECALL)] = exec_ECALL;
  m_exec_instr[static_cast<uint8_t>(InstrId::FENCE)] = exec_FENCE;
  // m_exec_instr[static_cast<uint8_t>(InstrId::FENCE_TSO)] = exec_FENCE_TSO;
  m_exec_instr[static_cast<uint8_t>(InstrId::JAL)] = exec_JAL;
  m_exec_instr[static_cast<uint8_t>(InstrId::JALR)] = exec_JALR;
  m_exec_instr[static_cast<uint8_t>(InstrId::LB)] = exec_LB;
  m_exec_instr[static_cast<uint8_t>(InstrId::LBU)] = exec_LBU;
  m_exec_instr[static_cast<uint8_t>(InstrId::LH)] = exec_LH;
  m_exec_instr[static_cast<uint8_t>(InstrId::LHU)] = exec_LHU;
  m_exec_instr[static_cast<uint8_t>(InstrId::LUI)] = exec_LUI;
  m_exec_instr[static_cast<uint8_t>(InstrId::LW)] = exec_LW;
  m_exec_instr[static_cast<uint8_t>(InstrId::OR)] = exec_OR;
  m_exec_instr[static_cast<uint8_t>(InstrId::ORI)] = exec_ORI;
  // m_exec_instr[static_cast<uint8_t>(InstrId::PAUSE)] = exec_PAUSE;
  m_exec_instr[static_cast<uint8_t>(InstrId::SB)] = exec_SB;
  // m_exec_instr[static_cast<uint8_t>(InstrId::SBREAK)] = exec_SBREAK;
  // m_exec_instr[static_cast<uint8_t>(InstrId::SCALL)] = exec_SCALL;
  m_exec_instr[static_cast<uint8_t>(InstrId::SH)] = exec_SH;
  m_exec_instr[static_cast<uint8_t>(InstrId::SLL)] = exec_SLL;
  m_exec_instr[static_cast<uint8_t>(InstrId::SLT)] = exec_SLT;
  m_exec_instr[static_cast<uint8_t>(InstrId::SLTI)] = exec_SLTI;
  m_exec_instr[static_cast<uint8_t>(InstrId::SLTIU)] = exec_SLTIU;
  m_exec_instr[static_cast<uint8_t>(InstrId::SLTU)] = exec_SLTU;
  m_exec_instr[static_cast<uint8_t>(InstrId::SRA)] = exec_SRA;
  m_exec_instr[static_cast<uint8_t>(InstrId::SRL)] = exec_SRL;
  m_exec_instr[static_cast<uint8_t>(InstrId::SUB)] = exec_SUB;
  m_exec_instr[static_cast<uint8_t>(InstrId::SW)] = exec_SW;
  m_exec_instr[static_cast<uint8_t>(InstrId::XOR)] = exec_XOR;
  m_exec_instr[static_cast<uint8_t>(InstrId::XORI)] = exec_XORI;
}

} // namespace sim
