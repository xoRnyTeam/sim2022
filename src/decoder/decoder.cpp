
#include "isa/instr.hpp"


#include "decoder/decoder.hpp"


namespace sim {


Instruction Decoder::decode(word_t word) {

  Instruction instr{};
switch ((word >> 0) & 0b1111111)
{
case 0b11:
{
switch ((word >> 12) & 0b0)
{
case 0b0:
{
//! LB
//! xxxxxxxxxxxxxxxxx000xxxxx0000011
instr.id = InstrId::LB;
instr.rd = slice<11, 7>(word);
instr.rs1 = slice<19, 15>(word);
instr.imm |= slice<31, 20>(word);
return instr;
}
case 0b1:
{
//! LH
//! xxxxxxxxxxxxxxxxx001xxxxx0000011
instr.id = InstrId::LH;
instr.rd = slice<11, 7>(word);
instr.rs1 = slice<19, 15>(word);
instr.imm |= slice<31, 20>(word);
return instr;
}
case 0b10:
{
//! LW
//! xxxxxxxxxxxxxxxxx010xxxxx0000011
instr.id = InstrId::LW;
instr.rd = slice<11, 7>(word);
instr.rs1 = slice<19, 15>(word);
instr.imm |= slice<31, 20>(word);
return instr;
}
case 0b100:
{
//! LBU
//! xxxxxxxxxxxxxxxxx100xxxxx0000011
instr.id = InstrId::LBU;
instr.rd = slice<11, 7>(word);
instr.rs1 = slice<19, 15>(word);
instr.imm |= slice<31, 20>(word);
return instr;
}
case 0b101:
{
//! LHU
//! xxxxxxxxxxxxxxxxx101xxxxx0000011
instr.id = InstrId::LHU;
instr.rd = slice<11, 7>(word);
instr.rs1 = slice<19, 15>(word);
instr.imm |= slice<31, 20>(word);
return instr;
}
}
}
case 0b1111:
{
switch ((word >> 12) & 0b0)
{
case 0b0:
{
//! FENCE
//! xxxxxxxxxxxxxxxxx000xxxxx0001111
instr.id = InstrId::FENCE;
instr.imm |= slice<31, 28>(word);
instr.imm |= slice<27, 24>(word);
instr.imm |= slice<23, 20>(word);
instr.rs1 = slice<19, 15>(word);
instr.rd = slice<11, 7>(word);
return instr;
}
}
}
case 0b10011:
{
switch ((word >> 12) & 0b0)
{
case 0b0:
{
//! ADDI
//! xxxxxxxxxxxxxxxxx000xxxxx0010011
instr.id = InstrId::ADDI;
instr.rd = slice<11, 7>(word);
instr.rs1 = slice<19, 15>(word);
instr.imm |= slice<31, 20>(word);
return instr;
}
case 0b10:
{
//! SLTI
//! xxxxxxxxxxxxxxxxx010xxxxx0010011
instr.id = InstrId::SLTI;
instr.rd = slice<11, 7>(word);
instr.rs1 = slice<19, 15>(word);
instr.imm |= slice<31, 20>(word);
return instr;
}
case 0b11:
{
//! SLTIU
//! xxxxxxxxxxxxxxxxx011xxxxx0010011
instr.id = InstrId::SLTIU;
instr.rd = slice<11, 7>(word);
instr.rs1 = slice<19, 15>(word);
instr.imm |= slice<31, 20>(word);
return instr;
}
case 0b100:
{
//! XORI
//! xxxxxxxxxxxxxxxxx100xxxxx0010011
instr.id = InstrId::XORI;
instr.rd = slice<11, 7>(word);
instr.rs1 = slice<19, 15>(word);
instr.imm |= slice<31, 20>(word);
return instr;
}
case 0b110:
{
//! ORI
//! xxxxxxxxxxxxxxxxx110xxxxx0010011
instr.id = InstrId::ORI;
instr.rd = slice<11, 7>(word);
instr.rs1 = slice<19, 15>(word);
instr.imm |= slice<31, 20>(word);
return instr;
}
case 0b111:
{
//! ANDI
//! xxxxxxxxxxxxxxxxx111xxxxx0010011
instr.id = InstrId::ANDI;
instr.rd = slice<11, 7>(word);
instr.rs1 = slice<19, 15>(word);
instr.imm |= slice<31, 20>(word);
return instr;
}
}
}
case 0b10111:
{
//! AUIPC
//! xxxxxxxxxxxxxxxxxxxxxxxxx0010111
instr.id = InstrId::AUIPC;
instr.rd = slice<11, 7>(word);
instr.imm |= slice<31, 12>(word);
return instr;
}
case 0b100011:
{
switch ((word >> 12) & 0b0)
{
case 0b0:
{
//! SB
//! xxxxxxxxxxxxxxxxx000xxxxx0100011
instr.id = InstrId::SB;
instr.imm |= slice<31, 25>(word);
instr.rs1 = slice<19, 15>(word);
instr.rs2 = slice<24, 20>(word);
instr.imm |= slice<11, 7>(word);
return instr;
}
case 0b1:
{
//! SH
//! xxxxxxxxxxxxxxxxx001xxxxx0100011
instr.id = InstrId::SH;
instr.imm |= slice<31, 25>(word);
instr.rs1 = slice<19, 15>(word);
instr.rs2 = slice<24, 20>(word);
instr.imm |= slice<11, 7>(word);
return instr;
}
case 0b10:
{
//! SW
//! xxxxxxxxxxxxxxxxx010xxxxx0100011
instr.id = InstrId::SW;
instr.imm |= slice<31, 25>(word);
instr.rs1 = slice<19, 15>(word);
instr.rs2 = slice<24, 20>(word);
instr.imm |= slice<11, 7>(word);
return instr;
}
}
}
case 0b110011:
{
switch ((word >> 25) & 0b0)
{
case 0b0:
{
switch ((word >> 12) & 0b0)
{
case 0b0:
{
//! ADD
//! 0000000xxxxxxxxxx000xxxxx0110011
instr.id = InstrId::ADD;
instr.rd = slice<11, 7>(word);
instr.rs1 = slice<19, 15>(word);
instr.rs2 = slice<24, 20>(word);
return instr;
}
case 0b1:
{
//! SLL
//! 0000000xxxxxxxxxx001xxxxx0110011
instr.id = InstrId::SLL;
instr.rd = slice<11, 7>(word);
instr.rs1 = slice<19, 15>(word);
instr.rs2 = slice<24, 20>(word);
return instr;
}
case 0b10:
{
//! SLT
//! 0000000xxxxxxxxxx010xxxxx0110011
instr.id = InstrId::SLT;
instr.rd = slice<11, 7>(word);
instr.rs1 = slice<19, 15>(word);
instr.rs2 = slice<24, 20>(word);
return instr;
}
case 0b11:
{
//! SLTU
//! 0000000xxxxxxxxxx011xxxxx0110011
instr.id = InstrId::SLTU;
instr.rd = slice<11, 7>(word);
instr.rs1 = slice<19, 15>(word);
instr.rs2 = slice<24, 20>(word);
return instr;
}
case 0b100:
{
//! XOR
//! 0000000xxxxxxxxxx100xxxxx0110011
instr.id = InstrId::XOR;
instr.rd = slice<11, 7>(word);
instr.rs1 = slice<19, 15>(word);
instr.rs2 = slice<24, 20>(word);
return instr;
}
case 0b101:
{
//! SRL
//! 0000000xxxxxxxxxx101xxxxx0110011
instr.id = InstrId::SRL;
instr.rd = slice<11, 7>(word);
instr.rs1 = slice<19, 15>(word);
instr.rs2 = slice<24, 20>(word);
return instr;
}
case 0b110:
{
//! OR
//! 0000000xxxxxxxxxx110xxxxx0110011
instr.id = InstrId::OR;
instr.rd = slice<11, 7>(word);
instr.rs1 = slice<19, 15>(word);
instr.rs2 = slice<24, 20>(word);
return instr;
}
case 0b111:
{
//! AND
//! 0000000xxxxxxxxxx111xxxxx0110011
instr.id = InstrId::AND;
instr.rd = slice<11, 7>(word);
instr.rs1 = slice<19, 15>(word);
instr.rs2 = slice<24, 20>(word);
return instr;
}
}
}
case 0b100000:
{
switch ((word >> 12) & 0b0)
{
case 0b0:
{
//! SUB
//! 0100000xxxxxxxxxx000xxxxx0110011
instr.id = InstrId::SUB;
instr.rd = slice<11, 7>(word);
instr.rs1 = slice<19, 15>(word);
instr.rs2 = slice<24, 20>(word);
return instr;
}
case 0b101:
{
//! SRA
//! 0100000xxxxxxxxxx101xxxxx0110011
instr.id = InstrId::SRA;
instr.rd = slice<11, 7>(word);
instr.rs1 = slice<19, 15>(word);
instr.rs2 = slice<24, 20>(word);
return instr;
}
}
}
}
}
case 0b110111:
{
//! LUI
//! xxxxxxxxxxxxxxxxxxxxxxxxx0110111
instr.id = InstrId::LUI;
instr.rd = slice<11, 7>(word);
instr.imm |= slice<31, 12>(word);
return instr;
}
case 0b1100011:
{
switch ((word >> 12) & 0b0)
{
case 0b0:
{
//! BEQ
//! xxxxxxxxxxxxxxxxx000xxxxx1100011
instr.id = InstrId::BEQ;
instr.imm |= slice<31, 31>(word);
instr.imm |= slice<30, 25>(word);
instr.rs1 = slice<19, 15>(word);
instr.rs2 = slice<24, 20>(word);
instr.imm |= slice<11, 8>(word);
instr.imm |= slice<7, 7>(word);
return instr;
}
case 0b1:
{
//! BNE
//! xxxxxxxxxxxxxxxxx001xxxxx1100011
instr.id = InstrId::BNE;
instr.imm |= slice<31, 31>(word);
instr.imm |= slice<30, 25>(word);
instr.rs1 = slice<19, 15>(word);
instr.rs2 = slice<24, 20>(word);
instr.imm |= slice<11, 8>(word);
instr.imm |= slice<7, 7>(word);
return instr;
}
case 0b100:
{
//! BLT
//! xxxxxxxxxxxxxxxxx100xxxxx1100011
instr.id = InstrId::BLT;
instr.imm |= slice<31, 31>(word);
instr.imm |= slice<30, 25>(word);
instr.rs1 = slice<19, 15>(word);
instr.rs2 = slice<24, 20>(word);
instr.imm |= slice<11, 8>(word);
instr.imm |= slice<7, 7>(word);
return instr;
}
case 0b101:
{
//! BGE
//! xxxxxxxxxxxxxxxxx101xxxxx1100011
instr.id = InstrId::BGE;
instr.imm |= slice<31, 31>(word);
instr.imm |= slice<30, 25>(word);
instr.rs1 = slice<19, 15>(word);
instr.rs2 = slice<24, 20>(word);
instr.imm |= slice<11, 8>(word);
instr.imm |= slice<7, 7>(word);
return instr;
}
case 0b110:
{
//! BLTU
//! xxxxxxxxxxxxxxxxx110xxxxx1100011
instr.id = InstrId::BLTU;
instr.imm |= slice<31, 31>(word);
instr.imm |= slice<30, 25>(word);
instr.rs1 = slice<19, 15>(word);
instr.rs2 = slice<24, 20>(word);
instr.imm |= slice<11, 8>(word);
instr.imm |= slice<7, 7>(word);
return instr;
}
case 0b111:
{
//! BGEU
//! xxxxxxxxxxxxxxxxx111xxxxx1100011
instr.id = InstrId::BGEU;
instr.imm |= slice<31, 31>(word);
instr.imm |= slice<30, 25>(word);
instr.rs1 = slice<19, 15>(word);
instr.rs2 = slice<24, 20>(word);
instr.imm |= slice<11, 8>(word);
instr.imm |= slice<7, 7>(word);
return instr;
}
}
}
case 0b1100111:
{
//! JALR
//! xxxxxxxxxxxxxxxxx000xxxxx1100111
instr.id = InstrId::JALR;
instr.rd = slice<11, 7>(word);
instr.rs1 = slice<19, 15>(word);
instr.imm |= slice<31, 20>(word);
return instr;
}
case 0b1101111:
{
//! JAL
//! xxxxxxxxxxxxxxxxxxxxxxxxx1101111
instr.id = InstrId::JAL;
instr.rd = slice<11, 7>(word);
instr.imm |= slice<31, 31>(word);
instr.imm |= slice<30, 21>(word);
instr.imm |= slice<20, 20>(word);
instr.imm |= slice<19, 12>(word);
return instr;
}
case 0b1110011:
{
switch ((word >> 7) & 0b111111111111111111)
{
case 0b0:
{
//! ECALL
//! 00000000000000000000000001110011
instr.id = InstrId::ECALL;
return instr;
}
case 0b10000000000000:
{
//! EBREAK
//! 00000000000100000000000001110011
instr.id = InstrId::EBREAK;
return instr;
}
}
}
}

} //! 


} //! 

