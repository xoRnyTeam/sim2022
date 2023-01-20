
#include "executor/executor.hpp"

#include "isa/instr.hpp"

#include <bit>

namespace sim {

//void Executor::exec(const Instruction &instr, Hart &hart) const {
//  m_exec_instr[static_cast<uint8_t>(instr.id)](instr, hart);
//  hart.regs[0] = 0;
//}

void exec_ADD(CallInfo* callee) {
  auto&& instr = *callee->instr; 
  auto&& hart = *callee->hart; 
  hart.setReg(instr.rd, hart.getReg(instr.rs1) + hart.getReg(instr.rs2));
  hart.addToPC(4); hart.regs[0] = 0; callee++; (callee)->exec(callee);
} //!

void exec_ADDI(CallInfo* callee) {
  auto&& instr = *callee->instr; 
  auto&& hart = *callee->hart; 
  hart.setReg(instr.rd, instr.imm + hart.getReg(instr.rs1));
  hart.addToPC(4); hart.regs[0] = 0; callee++; (callee)->exec(callee); 
} //!

void exec_AND(CallInfo* callee) {
  auto&& instr = *callee->instr; 
  auto&& hart = *callee->hart; 
  hart.setReg(instr.rd, hart.getReg(instr.rs1) & hart.getReg(instr.rs2));
  hart.addToPC(4); hart.regs[0] = 0; callee++; (callee)->exec(callee); 
} //!

void exec_ANDI(CallInfo* callee) {
  auto&& instr = *callee->instr; 
  auto&& hart = *callee->hart; 
  hart.setReg(instr.rd, hart.getReg(instr.rs1) & std::bit_cast<uint32_t>(instr.imm));
  hart.addToPC(4); hart.regs[0] = 0; callee++; (callee)->exec(callee); 
} //!

void exec_AUIPC(CallInfo* callee) {
  auto&& instr = *callee->instr; 
  auto&& hart = *callee->hart; 
  hart.setReg(instr.rd, instr.imm + hart.getPC());
  hart.addToPC(4); hart.regs[0] = 0; callee++; (callee)->exec(callee); 
} //!

void exec_BEQ(CallInfo* callee) {
  auto&& instr = *callee->instr; 
  auto&& hart = *callee->hart; 
  bool take = hart.getReg(instr.rs1) == hart.getReg(instr.rs2);
  hart.addToPC((take) ? instr.imm : 4);
   hart.regs[0] = 0; callee++; (callee)->exec(callee); 
} //!

void exec_BGE(CallInfo* callee) {
  auto&& instr = *callee->instr; 
  auto&& hart = *callee->hart; 
  bool take = std::bit_cast<int32_t>(hart.getReg(instr.rs1)) >= std::bit_cast<int32_t>(hart.getReg(instr.rs2));
  hart.addToPC((take) ? instr.imm : 4);
   hart.regs[0] = 0; callee++; (callee)->exec(callee); 
} //!

void exec_BGEU(CallInfo* callee) {
  auto&& instr = *callee->instr; 
  auto&& hart = *callee->hart; 
  bool take = std::bit_cast<uint32_t>(hart.getReg(instr.rs1)) >= std::bit_cast<uint32_t>(hart.getReg(instr.rs2));
  hart.addToPC((take) ? instr.imm : 4);
   hart.regs[0] = 0; callee++; (callee)->exec(callee); 
} //!

void exec_BLT(CallInfo* callee) {
  auto&& instr = *callee->instr; 
  auto&& hart = *callee->hart; 
  bool take = std::bit_cast<int32_t>(hart.getReg(instr.rs1)) < std::bit_cast<int32_t>(hart.getReg(instr.rs2));
  hart.addToPC((take) ? instr.imm : 4);
   hart.regs[0] = 0; callee++; (callee)->exec(callee); 
} //!

void exec_BLTU(CallInfo* callee) {
  auto&& instr = *callee->instr; 
  auto&& hart = *callee->hart; 
  bool take = std::bit_cast<uint32_t>(hart.getReg(instr.rs1)) < std::bit_cast<uint32_t>(hart.getReg(instr.rs2));
  hart.addToPC((take) ? instr.imm : 4);
   hart.regs[0] = 0; callee++; (callee)->exec(callee); 
} //!

void exec_BNE(CallInfo* callee) {
  auto&& instr = *callee->instr; 
  auto&& hart = *callee->hart; 
  bool take = hart.getReg(instr.rs1) != hart.getReg(instr.rs2);
  hart.addToPC((take) ? instr.imm : 4);
   hart.regs[0] = 0; callee++; (callee)->exec(callee); 
} //!

void exec_EBREAK(CallInfo* callee) {
  auto&& instr = *callee->instr; 
  auto&& hart = *callee->hart;  hart.setTerminate(); } //!

void exec_ECALL(CallInfo* callee) {
  auto&& instr = *callee->instr; 
  auto&& hart = *callee->hart;  hart.setTerminate(); } //!

void exec_FENCE(CallInfo* callee) {
  auto&& instr = *callee->instr; 
  auto&& hart = *callee->hart;  hart.addToPC(4);  hart.regs[0] = 0; callee++; (callee)->exec(callee); } //!

void exec_FENCE_TSO(CallInfo* callee) {
  auto&& instr = *callee->instr; 
  auto&& hart = *callee->hart;  hart.addToPC(4);  hart.regs[0] = 0; callee++; (callee)->exec(callee); } //!

void exec_JAL(CallInfo* callee) {
  auto&& instr = *callee->instr; 
  auto&& hart = *callee->hart; 
  auto pc_next = hart.pc + 4;
  hart.addToPC(instr.imm);
  hart.setReg(instr.rd, pc_next); hart.regs[0] = 0; callee++; (callee)->exec(callee); 
} //!

void exec_JALR(CallInfo* callee) {
  auto&& instr = *callee->instr; 
  auto&& hart = *callee->hart; 
  auto pc_next = hart.pc + 4;
  hart.setPC((std::bit_cast<int32_t>(hart.getReg(instr.rs1)) + instr.imm) &
             (~0b01));
  hart.setReg(instr.rd, pc_next); hart.regs[0] = 0; callee++; (callee)->exec(callee); 
} //!

void exec_LB(CallInfo* callee) {
  auto&& instr = *callee->instr; 
  auto&& hart = *callee->hart; 
  vaddress_t address = hart.getReg(instr.rs1) + instr.imm;
  word_t value = hart.mmu().read<bword_t>(address);
  //                    sign extend value
  hart.setReg(instr.rd, std::bit_cast<int32_t>(value << 24) >> 24);
  hart.addToPC(4); hart.regs[0] = 0; callee++; (callee)->exec(callee); 
} //!

void exec_LBU(CallInfo* callee) {
  auto&& instr = *callee->instr; 
  auto&& hart = *callee->hart; 
  vaddress_t address = hart.getReg(instr.rs1) + instr.imm;
  word_t value = 0xFF & hart.mmu().read<bword_t>(address);
  hart.setReg(instr.rd, value);
  hart.addToPC(4); hart.regs[0] = 0; callee++; (callee)->exec(callee); 
} //!

void exec_LH(CallInfo* callee) {
  auto&& instr = *callee->instr; 
  auto&& hart = *callee->hart; 
  vaddress_t address = hart.getReg(instr.rs1) + instr.imm;
  word_t value = hart.mmu().read<hword_t>(address);
  //                    sign extend value
  hart.setReg(instr.rd, std::bit_cast<int32_t>(value << 16) >> 16);
  hart.addToPC(4); hart.regs[0] = 0; callee++; (callee)->exec(callee); 
} //!

void exec_LHU(CallInfo* callee) {
  auto&& instr = *callee->instr; 
  auto&& hart = *callee->hart; 
  vaddress_t address = hart.getReg(instr.rs1) + instr.imm;
  word_t value = 0xFFFF & hart.mmu().read<hword_t>(address);
  hart.setReg(instr.rd, value);
  hart.addToPC(4); hart.regs[0] = 0; callee++; (callee)->exec(callee); 
} //!

void exec_LUI(CallInfo* callee) {
  auto&& instr = *callee->instr; 
  auto&& hart = *callee->hart; 
  hart.setReg(instr.rd, std::bit_cast<uint32_t>(instr.imm));
  hart.addToPC(4); hart.regs[0] = 0; callee++; (callee)->exec(callee); 
} //!

void exec_LW(CallInfo* callee) {
  auto&& instr = *callee->instr; 
  auto&& hart = *callee->hart; 
  vaddress_t address = hart.getReg(instr.rs1) + instr.imm;
  word_t value = hart.mmu().read<word_t>(address);
  hart.setReg(instr.rd, value);
  hart.addToPC(4); hart.regs[0] = 0; callee++; (callee)->exec(callee); 
} //!

void exec_OR(CallInfo* callee) {
  auto&& instr = *callee->instr; 
  auto&& hart = *callee->hart; 
  hart.setReg(instr.rd, hart.getReg(instr.rs1) | hart.getReg(instr.rs2));
  hart.addToPC(4); hart.regs[0] = 0; callee++; (callee)->exec(callee); 
} //!

void exec_ORI(CallInfo* callee) {
  auto&& instr = *callee->instr; 
  auto&& hart = *callee->hart; 
  hart.setReg(instr.rd, hart.getReg(instr.rs1) | std::bit_cast<uint32_t>(instr.imm));
  hart.addToPC(4); hart.regs[0] = 0; callee++; (callee)->exec(callee); 
} //!

void exec_PAUSE(CallInfo* callee) {
  auto&& instr = *callee->instr; 
  auto&& hart = *callee->hart;  hart.setTerminate(); } //!

void exec_SB(CallInfo* callee) {
  auto&& instr = *callee->instr; 
  auto&& hart = *callee->hart; 
  vaddress_t address = hart.getReg(instr.rs1) + instr.imm;
  bword_t value = static_cast<bword_t>(hart.getReg(instr.rs2) & 0xFF);
  hart.mmu().write<bword_t>(address, value);
  hart.addToPC(4); hart.regs[0] = 0; callee++; (callee)->exec(callee); 
} //!

void exec_SBREAK(CallInfo* callee) {
  auto&& instr = *callee->instr; 
  auto&& hart = *callee->hart;  hart.setTerminate(); } //!

void exec_SCALL(CallInfo* callee) {
  auto&& instr = *callee->instr; 
  auto&& hart = *callee->hart;  hart.setTerminate(); } //!

void exec_SH(CallInfo* callee) {
  auto&& instr = *callee->instr; 
  auto&& hart = *callee->hart; 
  vaddress_t address = hart.getReg(instr.rs1) + instr.imm;
  hword_t value = static_cast<hword_t>(hart.getReg(instr.rs2) & 0xFFFF);
  hart.mmu().write<hword_t>(address, value);
  hart.addToPC(4); hart.regs[0] = 0; callee++; (callee)->exec(callee); 
} //!

void exec_SLL(CallInfo* callee) {
  auto&& instr = *callee->instr; 
  auto&& hart = *callee->hart; 
  //shifts on the value in register rs1 by the shift amount held in the lower 5 bits of register rs2
  hart.setReg(instr.rd, hart.getReg(instr.rs1) << (hart.getReg(instr.rs2) & 0b011111));
  hart.addToPC(4); hart.regs[0] = 0; callee++; (callee)->exec(callee); 
} //!

void exec_SLLI(CallInfo* callee) {
  auto&& instr = *callee->instr; 
  auto&& hart = *callee->hart; 
  //shifts on the value in register rs1 by the shift amount held in the lower 5 bits of register rs2
  hart.setReg(instr.rd, hart.getReg(instr.rs1) << (instr.imm & 0b011111));
  hart.addToPC(4); hart.regs[0] = 0; callee++; (callee)->exec(callee); 
} //!

void exec_SLT(CallInfo* callee) {
  auto&& instr = *callee->instr; 
  auto&& hart = *callee->hart; 
  // sign compare
  hart.setReg(instr.rd, std::bit_cast<int32_t>(hart.getReg(instr.rs1)) < std::bit_cast<int32_t>(hart.getReg(instr.rs2)));
  hart.addToPC(4); hart.regs[0] = 0; callee++; (callee)->exec(callee); 
} //!

void exec_SLTI(CallInfo* callee) {
  auto&& instr = *callee->instr; 
  auto&& hart = *callee->hart; 
  hart.setReg(instr.rd, std::bit_cast<int32_t>(hart.getReg(instr.rs1)) < std::bit_cast<int32_t>(instr.imm));
  hart.addToPC(4); hart.regs[0] = 0; callee++; (callee)->exec(callee); 
} //!

void exec_SLTIU(CallInfo* callee) {
  auto&& instr = *callee->instr; 
  auto&& hart = *callee->hart; 
  hart.setReg(instr.rd, std::bit_cast<uint32_t>(hart.getReg(instr.rs1)) < std::bit_cast<uint32_t>(instr.imm));
  hart.addToPC(4); hart.regs[0] = 0; callee++; (callee)->exec(callee); 
} //!

void exec_SLTU(CallInfo* callee) {
  auto&& instr = *callee->instr; 
  auto&& hart = *callee->hart; 
  // unsign cmp
  hart.setReg(instr.rd, hart.getReg(instr.rs1) < hart.getReg(instr.rs2));
  hart.addToPC(4); hart.regs[0] = 0; callee++; (callee)->exec(callee); 
} //!

void exec_SRA(CallInfo* callee) {
  auto&& instr = *callee->instr; 
  auto&& hart = *callee->hart; 
  // arifmetic shifts on the value in register rs1 by the shift amount held in the lower 5 bits of register rs2
  hart.setReg(instr.rd, ((int)hart.getReg(instr.rs1)) >> (hart.getReg(instr.rs2) & 0b011111));
  hart.addToPC(4); hart.regs[0] = 0; callee++; (callee)->exec(callee); 
} //!

void exec_SRAI(CallInfo* callee) {
  auto&& instr = *callee->instr; 
  auto&& hart = *callee->hart; 
  // arifmetic shifts on the value in register rs1 by the shift amount held in the lower 5 bits of register rs2
  hart.setReg(instr.rd, ((int)hart.getReg(instr.rs1)) >> (instr.imm & 0b011111));
  hart.addToPC(4); hart.regs[0] = 0; callee++; (callee)->exec(callee); 
} //!

void exec_SRL(CallInfo* callee) {
  auto&& instr = *callee->instr; 
  auto&& hart = *callee->hart; 
  //shifts on the value in register rs1 by the shift amount held in the lower 5 bits of register rs2
  hart.setReg(instr.rd, hart.getReg(instr.rs1) >> (hart.getReg(instr.rs2) & 0b011111));
  hart.addToPC(4); hart.regs[0] = 0; callee++; (callee)->exec(callee); 
} //!

void exec_SRLI(CallInfo* callee) {
  auto&& instr = *callee->instr; 
  auto&& hart = *callee->hart; 
  //shifts on the value in register rs1 by the shift amount held in the lower 5 bits of register rs2
  hart.setReg(instr.rd, hart.getReg(instr.rs1) >> (instr.imm & 0b011111));
  hart.addToPC(4); hart.regs[0] = 0; callee++; (callee)->exec(callee); 
} //!

void exec_SUB(CallInfo* callee) {
  auto&& instr = *callee->instr; 
  auto&& hart = *callee->hart; 
  hart.setReg(instr.rd, hart.getReg(instr.rs1) - hart.getReg(instr.rs2));
  hart.addToPC(4); hart.regs[0] = 0; callee++; (callee)->exec(callee); 
} //!

void exec_SW(CallInfo* callee) {
  auto&& instr = *callee->instr; 
  auto&& hart = *callee->hart; 
  vaddress_t address = hart.getReg(instr.rs1) + instr.imm;
  word_t value = hart.getReg(instr.rs2);
  hart.mmu().write<word_t>(address, value);
  hart.addToPC(4); hart.regs[0] = 0; callee++; (callee)->exec(callee); 
} //!

void exec_XOR(CallInfo* callee) {
  auto&& instr = *callee->instr; 
  auto&& hart = *callee->hart; 
  hart.setReg(instr.rd, hart.getReg(instr.rs1) ^ hart.getReg(instr.rs2));
  hart.addToPC(4); hart.regs[0] = 0; callee++; (callee)->exec(callee); 
} //!

void exec_XORI(CallInfo* callee) {
  auto&& instr = *callee->instr; 
  auto&& hart = *callee->hart; 
  hart.setReg(instr.rd, hart.getReg(instr.rs1) ^ std::bit_cast<uint32_t>(instr.imm));
  hart.addToPC(4); hart.regs[0] = 0; callee++; (callee)->exec(callee); 
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
  m_exec_instr[static_cast<uint8_t>(InstrId::SLLI)] = exec_SLLI;
  m_exec_instr[static_cast<uint8_t>(InstrId::SLT)] = exec_SLT;
  m_exec_instr[static_cast<uint8_t>(InstrId::SLTI)] = exec_SLTI;
  m_exec_instr[static_cast<uint8_t>(InstrId::SRAI)] = exec_SRAI;
  m_exec_instr[static_cast<uint8_t>(InstrId::SRLI)] = exec_SRLI;
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
