#include "executor/executor.hpp"

#include "gl/gl.hpp"
#include "isa/instr.hpp"

#include <bit>

namespace sim {

void Executor::exec(Instruction &instr, Hart &hart) const {
  m_exec_instr[static_cast<uint8_t>(instr.id)](instr, hart);
  hart.setReg(0, 0);
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
  hart.setReg(instr.rd,
              hart.getReg(instr.rs1) & std::bit_cast<uint32_t>(instr.imm));
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
  bool take = std::bit_cast<int32_t>(hart.getReg(instr.rs1)) >=
              std::bit_cast<int32_t>(hart.getReg(instr.rs2));
  hart.addToPC((take) ? instr.imm : 4);
} //!

void exec_BGEU(Instruction &instr, Hart &hart) {
  bool take = std::bit_cast<uint32_t>(hart.getReg(instr.rs1)) >=
              std::bit_cast<uint32_t>(hart.getReg(instr.rs2));
  hart.addToPC((take) ? instr.imm : 4);
} //!

void exec_BLT(Instruction &instr, Hart &hart) {
  bool take = std::bit_cast<int32_t>(hart.getReg(instr.rs1)) <
              std::bit_cast<int32_t>(hart.getReg(instr.rs2));
  hart.addToPC((take) ? instr.imm : 4);
} //!

void exec_BLTU(Instruction &instr, Hart &hart) {
  bool take = std::bit_cast<uint32_t>(hart.getReg(instr.rs1)) <
              std::bit_cast<uint32_t>(hart.getReg(instr.rs2));
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
  word_t value = hart.mmu().read<bword_t>(address);
  //                    sign extend value
  hart.setReg(instr.rd, std::bit_cast<int32_t>(value << 24) >> 24);
  hart.addToPC(4);
} //!

void exec_LBU(Instruction &instr, Hart &hart) {
  vaddress_t address = hart.getReg(instr.rs1) + instr.imm;
  word_t value = 0xFF & hart.mmu().read<bword_t>(address);
  hart.setReg(instr.rd, value);
  hart.addToPC(4);
} //!

void exec_LH(Instruction &instr, Hart &hart) {
  vaddress_t address = hart.getReg(instr.rs1) + instr.imm;
  word_t value = hart.mmu().read<hword_t>(address);
  //                    sign extend value
  hart.setReg(instr.rd, std::bit_cast<int32_t>(value << 16) >> 16);
  hart.addToPC(4);
} //!

void exec_LHU(Instruction &instr, Hart &hart) {
  vaddress_t address = hart.getReg(instr.rs1) + instr.imm;
  word_t value = 0xFFFF & hart.mmu().read<hword_t>(address);
  hart.setReg(instr.rd, value);
  hart.addToPC(4);
} //!

void exec_LUI(Instruction &instr, Hart &hart) {
  hart.setReg(instr.rd, std::bit_cast<uint32_t>(instr.imm));
  hart.addToPC(4);
} //!

void exec_LW(Instruction &instr, Hart &hart) {
  vaddress_t address = hart.getReg(instr.rs1) + instr.imm;
  word_t value = hart.mmu().read<word_t>(address);
  hart.setReg(instr.rd, value);
  hart.addToPC(4);
} //!

void exec_OR(Instruction &instr, Hart &hart) {
  hart.setReg(instr.rd, hart.getReg(instr.rs1) | hart.getReg(instr.rs2));
  hart.addToPC(4);
} //!

void exec_ORI(Instruction &instr, Hart &hart) {
  hart.setReg(instr.rd,
              hart.getReg(instr.rs1) | std::bit_cast<uint32_t>(instr.imm));
  hart.addToPC(4);
} //!

void exec_PAUSE(Instruction &instr, Hart &hart) { hart.setTerminate(); } //!

void exec_SB(Instruction &instr, Hart &hart) {
  vaddress_t address = hart.getReg(instr.rs1) + instr.imm;
  bword_t value = static_cast<bword_t>(hart.getReg(instr.rs2) & 0xFF);
  hart.mmu().write<bword_t>(address, value);
  hart.addToPC(4);
} //!

void exec_SBREAK(Instruction &instr, Hart &hart) { hart.setTerminate(); } //!

void exec_SCALL(Instruction &instr, Hart &hart) { hart.setTerminate(); } //!

void exec_SH(Instruction &instr, Hart &hart) {
  vaddress_t address = hart.getReg(instr.rs1) + instr.imm;
  hword_t value = static_cast<hword_t>(hart.getReg(instr.rs2) & 0xFFFF);
  hart.mmu().write<hword_t>(address, value);
  hart.addToPC(4);
} //!

void exec_SLL(Instruction &instr, Hart &hart) {
  // shifts on the value in register rs1 by the shift amount held in the lower 5
  // bits of register rs2
  hart.setReg(instr.rd, hart.getReg(instr.rs1)
                            << (hart.getReg(instr.rs2) & 0b011111));
  hart.addToPC(4);
} //!

void exec_SLT(Instruction &instr, Hart &hart) {
  // sign compare
  hart.setReg(instr.rd, std::bit_cast<int32_t>(hart.getReg(instr.rs1)) <
                            std::bit_cast<int32_t>(hart.getReg(instr.rs2)));
  hart.addToPC(4);
} //!

void exec_SLTI(Instruction &instr, Hart &hart) {
  hart.setReg(instr.rd, std::bit_cast<int32_t>(hart.getReg(instr.rs1)) <
                            std::bit_cast<int32_t>(instr.imm));
  hart.addToPC(4);
} //!

void exec_SLTIU(Instruction &instr, Hart &hart) {
  hart.setReg(instr.rd, std::bit_cast<uint32_t>(hart.getReg(instr.rs1)) <
                            std::bit_cast<uint32_t>(instr.imm));
  hart.addToPC(4);
} //!

void exec_SLTU(Instruction &instr, Hart &hart) {
  // unsign cmp
  hart.setReg(instr.rd, hart.getReg(instr.rs1) < hart.getReg(instr.rs2));
  hart.addToPC(4);
} //!

void exec_SRA(Instruction &instr, Hart &hart) {
  // arifmetic shifts on the value in register rs1 by the shift amount held in
  // the lower 5 bits of register rs2
  hart.setReg(instr.rd, ((int)hart.getReg(instr.rs1)) >>
                            (hart.getReg(instr.rs2) & 0b011111));
  hart.addToPC(4);
} //!

void exec_SRL(Instruction &instr, Hart &hart) {
  // shifts on the value in register rs1 by the shift amount held in the lower 5
  // bits of register rs2
  hart.setReg(instr.rd,
              hart.getReg(instr.rs1) >> (hart.getReg(instr.rs2) & 0b011111));
  hart.addToPC(4);
} //!

void exec_SUB(Instruction &instr, Hart &hart) {
  hart.setReg(instr.rd, hart.getReg(instr.rs1) - hart.getReg(instr.rs2));
  hart.addToPC(4);
} //!

void exec_SW(Instruction &instr, Hart &hart) {
  vaddress_t address = hart.getReg(instr.rs1) + instr.imm;
  word_t value = hart.getReg(instr.rs2);
  hart.mmu().write<word_t>(address, value);
  hart.addToPC(4);
} //!

void exec_XOR(Instruction &instr, Hart &hart) {
  hart.setReg(instr.rd, hart.getReg(instr.rs1) ^ hart.getReg(instr.rs2));
  hart.addToPC(4);
} //!

void exec_XORI(Instruction &instr, Hart &hart) {
  hart.setReg(instr.rd,
              hart.getReg(instr.rs1) ^ std::bit_cast<uint32_t>(instr.imm));
  hart.addToPC(4);
} //!

//---------------------------------------------------------------------------

void exec_gl_graph_init(Instruction &instr, Hart &hart) {
  //           a0             , a1
  gl_graph_init(hart.getReg(10), hart.getReg(11));
}

//
void exec_gl_set_pixel(Instruction &instr, Hart &hart) {
  //        a0             , a1             , a2
  gl_set_pixel(hart.getReg(10), hart.getReg(11), hart.getReg(12));
}

//
void exec_gl_window_is_open(Instruction &instr, Hart &hart) {
  hart.setReg(15, gl_window_is_open());
}

//
void exec_gl_get_msecs(Instruction &instr, Hart &hart) {
  hart.setReg(15, gl_get_msecs());
}

//
void exec_gl_flush(Instruction &instr, Hart &hart) { gl_flush(); }

//
void exec_gl_rand(Instruction &instr, Hart &hart) {
  hart.setReg(15, gl_rand());
}

//
void exec_gl_clear(Instruction &instr, Hart &hart) { gl_clear(); }

//---------------------------------------------------------------------------

void exec_SRAI(Instruction &instr, Hart &hart) {
  // arifmetic shifts on the value in register rs1 by the shift amount held in
  // the lower 5 bits of register rs2
  hart.setReg(instr.rd,
              ((int)hart.getReg(instr.rs1)) >> (instr.imm & 0b011111));
  hart.addToPC(4);
} //!

void exec_SRLI(Instruction &instr, Hart &hart) {
  // shifts on the value in register rs1 by the shift amount held in the lower 5
  // bits of register rs2
  hart.setReg(instr.rd, hart.getReg(instr.rs1) >> (instr.imm & 0b011111));
  hart.addToPC(4);
} //!

void exec_SLLI(Instruction &instr, Hart &hart) {
  // shifts on the value in register rs1 by the shift amount held in the lower 5
  // bits of register rs2
  hart.setReg(instr.rd, hart.getReg(instr.rs1) << (instr.imm & 0b011111));
  hart.addToPC(4);
} //!

void exec_DIV(Instruction &instr, Hart &hart) {
  hart.setReg(instr.rd,
              (int)hart.getReg(instr.rs1) / (int)hart.getReg(instr.rs2));
  hart.addToPC(4);
} //!

void exec_DIVU(Instruction &instr, Hart &hart) {
  hart.setReg(instr.rd, hart.getReg(instr.rs1) / hart.getReg(instr.rs2));
  hart.addToPC(4);
} //!

void exec_REM(Instruction &instr, Hart &hart) {
  hart.setReg(instr.rd,
              (int)hart.getReg(instr.rs1) % (int)hart.getReg(instr.rs2));
  hart.addToPC(4);
} //!

void exec_REMU(Instruction &instr, Hart &hart) {
  hart.setReg(instr.rd, hart.getReg(instr.rs1) % hart.getReg(instr.rs2));
  hart.addToPC(4);
} //!

void exec_MUL(Instruction &instr, Hart &hart) {
  hart.setReg(instr.rd, hart.getReg(instr.rs1) * hart.getReg(instr.rs2));
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
  m_exec_instr[static_cast<uint8_t>(InstrId::GL_INIT)] = exec_gl_graph_init;
  m_exec_instr[static_cast<uint8_t>(InstrId::GL_PUT_PIXEL)] = exec_gl_set_pixel;
  m_exec_instr[static_cast<uint8_t>(InstrId::GL_WINDOW_IS_OPEN)] =
      exec_gl_window_is_open;
  m_exec_instr[static_cast<uint8_t>(InstrId::GL_TIME)] = exec_gl_get_msecs;
  m_exec_instr[static_cast<uint8_t>(InstrId::GL_FLUSH)] = exec_gl_flush;
  m_exec_instr[static_cast<uint8_t>(InstrId::GL_RAND)] = exec_gl_rand;
  m_exec_instr[static_cast<uint8_t>(InstrId::GL_CLEAR)] = exec_gl_clear;
  m_exec_instr[static_cast<uint8_t>(InstrId::MUL)] = exec_MUL;
  m_exec_instr[static_cast<uint8_t>(InstrId::DIV)] = exec_DIV;
  m_exec_instr[static_cast<uint8_t>(InstrId::DIVU)] = exec_DIVU;
  m_exec_instr[static_cast<uint8_t>(InstrId::REM)] = exec_REM;
  m_exec_instr[static_cast<uint8_t>(InstrId::REMU)] = exec_REMU;
  m_exec_instr[static_cast<uint8_t>(InstrId::SLLI)] = exec_SLLI;
  m_exec_instr[static_cast<uint8_t>(InstrId::SRAI)] = exec_SRAI;
  m_exec_instr[static_cast<uint8_t>(InstrId::SRLI)] = exec_SRLI;
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