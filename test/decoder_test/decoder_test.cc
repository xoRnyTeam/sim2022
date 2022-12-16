#include "decoder/decoder.hpp"
//
#include "header.hpp"
#include "init.hpp"

TEST(decoder, LB) {
  sim::word_t word = 0b11110000111101010'000'11100'0000011;
  //
  auto instr = sim::Decoder::decode(word);
  ASSERT_EQ(instr.id, sim::InstrId::LB);
}

TEST(decoder, LH) { // xxxxxxxxxxxxxxxxx'001'xxxxx'0000011
  sim::word_t word = 0b11110000111101010'001'11100'0000011;
  //
  auto instr = sim::Decoder::decode(word);
  ASSERT_EQ(instr.id, sim::InstrId::LH);
}

TEST(decoder, LW) {
  sim::word_t word = 0b11110000111101010'010'11100'0000011;
  //
  auto instr = sim::Decoder::decode(word);
  ASSERT_EQ(instr.id, sim::InstrId::LW);
}

TEST(decoder, LBU) {
  sim::word_t word = 0b11110000111101010'100'11100'0000011;
  //
  auto instr = sim::Decoder::decode(word);
  ASSERT_EQ(instr.id, sim::InstrId::LBU);
}

TEST(decoder, LHU) {
  sim::word_t word = 0b11110000111101010'101'11100'0000011;
  //
  auto instr = sim::Decoder::decode(word);
  ASSERT_EQ(instr.id, sim::InstrId::LHU);
}

TEST(decoder, FENCE) {
  sim::word_t word = 0b11110000111101010'000'11100'0001111;
  //
  auto instr = sim::Decoder::decode(word);
  ASSERT_EQ(instr.id, sim::InstrId::FENCE);
}

TEST(decoder, ADDI) {
  sim::word_t word = 0b11110000111101010'000'11100'0010011;
  //
  auto instr = sim::Decoder::decode(word);
  ASSERT_EQ(instr.id, sim::InstrId::ADDI);
}

TEST(decoder, SLTI) {
  sim::word_t word = 0b11110000111101010'010'11100'0010011;
  //
  auto instr = sim::Decoder::decode(word);
  ASSERT_EQ(instr.id, sim::InstrId::SLTI);
}

TEST(decoder, SLTIU) {
  sim::word_t word = 0b11110000111101010'011'11100'0010011;
  //
  auto instr = sim::Decoder::decode(word);
  ASSERT_EQ(instr.id, sim::InstrId::SLTIU);
}

TEST(decoder, XORI) {
  sim::word_t word = 0b11110000111101010'100'11100'0010011;
  //
  auto instr = sim::Decoder::decode(word);
  ASSERT_EQ(instr.id, sim::InstrId::XORI);
}

TEST(decoder, ORI) {
  sim::word_t word = 0b11110000111101010'110'11100'0010011;
  //
  auto instr = sim::Decoder::decode(word);
  ASSERT_EQ(instr.id, sim::InstrId::ORI);
}

TEST(decoder, ANDI) {
  sim::word_t word = 0b11110000111101010'111'11100'0010011;
  //
  auto instr = sim::Decoder::decode(word);
  ASSERT_EQ(instr.id, sim::InstrId::ANDI);
}

TEST(decoder, AUIPC) {
  sim::word_t word = 0b11110000111101010111110100010111;
  //
  auto instr = sim::Decoder::decode(word);
  ASSERT_EQ(instr.id, sim::InstrId::AUIPC);
}

TEST(decoder, SB) {
  sim::word_t word = 0b11110000111101010'000'11100'0100011;
  //
  auto instr = sim::Decoder::decode(word);
  ASSERT_EQ(instr.id, sim::InstrId::SB);
}

TEST(decoder, SH) {
  sim::word_t word = 0b11110000111101010'001'11100'0100011;
  //
  auto instr = sim::Decoder::decode(word);
  ASSERT_EQ(instr.id, sim::InstrId::SH);
}

TEST(decoder, SW) {
  sim::word_t word = 0b11110000111101010'010'11100'0100011;
  //
  auto instr = sim::Decoder::decode(word);
  ASSERT_EQ(instr.id, sim::InstrId::SW);
}

TEST(decoder, ADD) {
  sim::word_t word = 0b0000000'1010101010'000'11100'0110011;
  //
  auto instr = sim::Decoder::decode(word);
  ASSERT_EQ(instr.id, sim::InstrId::ADD);
}

TEST(decoder, SLL) {
  sim::word_t word = 0b0000000'1010101010'001'11100'0110011;
  //
  auto instr = sim::Decoder::decode(word);
  ASSERT_EQ(instr.id, sim::InstrId::SLL);
}

TEST(decoder, SLT) {
  sim::word_t word = 0b0000000'1010101010'010'11100'0110011;
  //
  auto instr = sim::Decoder::decode(word);
  ASSERT_EQ(instr.id, sim::InstrId::SLT);
}

TEST(decoder, SLTU) {
  sim::word_t word = 0b0000000'1010101010'011'11100'0110011;
  //
  auto instr = sim::Decoder::decode(word);
  ASSERT_EQ(instr.id, sim::InstrId::SLTU);
}

TEST(decoder, XOR) {
  sim::word_t word = 0b0000000'1010101010'100'11100'0110011;
  //
  auto instr = sim::Decoder::decode(word);
  ASSERT_EQ(instr.id, sim::InstrId::XOR);
}

TEST(decoder, SRL) {
  sim::word_t word = 0b0000000'1010101010'101'11100'0110011;
  //
  auto instr = sim::Decoder::decode(word);
  ASSERT_EQ(instr.id, sim::InstrId::SRL);
}

TEST(decoder, AND) {
  sim::word_t word = 0b0000000'1010101010'111'11100'0110011;
  //
  auto instr = sim::Decoder::decode(word);
  ASSERT_EQ(instr.id, sim::InstrId::AND);
}

TEST(decoder, SUB) {
  sim::word_t word = 0b0100000'1010101010'000'11100'0110011;
  //
  auto instr = sim::Decoder::decode(word);
  ASSERT_EQ(instr.id, sim::InstrId::SUB);
}

TEST(decoder, SRA) {
  sim::word_t word = 0b0100000'1010101010'101'11100'0110011;
  //
  auto instr = sim::Decoder::decode(word);
  ASSERT_EQ(instr.id, sim::InstrId::SRA);
}

TEST(decoder, LUI) {
  sim::word_t word = 0b1111000011111110000001111'0110111;
  //
  auto instr = sim::Decoder::decode(word);
  ASSERT_EQ(instr.id, sim::InstrId::LUI);
}

TEST(decoder, BEQ) {
  sim::word_t word = 0b11110101010111111'000'11100'1100011;
  //
  auto instr = sim::Decoder::decode(word);
  ASSERT_EQ(instr.id, sim::InstrId::BEQ);
}

TEST(decoder, BNE) {
  sim::word_t word = 0b11110101010111111'001'11100'1100011;
  //
  auto instr = sim::Decoder::decode(word);
  ASSERT_EQ(instr.id, sim::InstrId::BNE);
}

TEST(decoder, BLT) {
  sim::word_t word = 0b11110101010111111'100'11100'1100011;
  //
  auto instr = sim::Decoder::decode(word);
  ASSERT_EQ(instr.id, sim::InstrId::BLT);
}

TEST(decoder, BGE) {
  sim::word_t word = 0b11110101010111111'101'11100'1100011;
  //
  auto instr = sim::Decoder::decode(word);
  ASSERT_EQ(instr.id, sim::InstrId::BGE);
}

TEST(decoder, BLTU) {
  sim::word_t word = 0b11110101010111111'110'11100'1100011;
  //
  auto instr = sim::Decoder::decode(word);
  ASSERT_EQ(instr.id, sim::InstrId::BLTU);
}

TEST(decoder, BGEU) {
  sim::word_t word = 0b11110101010111111'111'11100'1100011;
  //
  auto instr = sim::Decoder::decode(word);
  ASSERT_EQ(instr.id, sim::InstrId::BGEU);
}

TEST(decoder, JALR) {
  sim::word_t word = 0b11110101010111111'000'11100'1100111;
  //
  auto instr = sim::Decoder::decode(word);
  ASSERT_EQ(instr.id, sim::InstrId::JALR);
}

TEST(decoder, JAL) {
  sim::word_t word = 0b1111000011110000111100001'1101111;
  //
  auto instr = sim::Decoder::decode(word);
  ASSERT_EQ(instr.id, sim::InstrId::JAL);
}


// imm tests
//=---------

TEST(decoder, I_imm) {
  sim::word_t word = 0b11110000111101010'111'11100'0010011;
  //
  auto instr = sim::Decoder::decode(word);
  ASSERT_EQ(instr.imm, 0b111111111111111111111'11100001111);
}

TEST(decoder, S_imm) {
  // 7th bit
  sim::word_t word = 0b0000000'0111101010'010'00001'0100011;
  auto instr = sim::Decoder::decode(word);
  ASSERT_EQ(instr.imm, 0b01);
  // 11-8
  word = 0b0000000'0111101010'010'11110'0100011;
  instr = sim::Decoder::decode(word);
  ASSERT_EQ(instr.imm, 0b1111'0);
  //30-25
  word = 0b0111111'0111101010'010'00000'0100011;
  instr = sim::Decoder::decode(word);
  ASSERT_EQ(instr.imm, 0b011111100000);
  //31
  word = 0b1000000'0111101010'010'00000'0100011;
  instr = sim::Decoder::decode(word);
  ASSERT_EQ(instr.imm, (int32_t)0b111111111111111111111'00000000000);
}

TEST(decoder, B_imm) {
  // 7
  sim::word_t word = 0b0000000'1010111111'000'00001'1100011;
  auto instr = sim::Decoder::decode(word);
  ASSERT_EQ(instr.imm, 0b0100000000000);
  // 11-8
  word = 0b0000000'0111101010'000'11110'1100011;
  instr = sim::Decoder::decode(word);
  ASSERT_EQ(instr.imm, 0b1111'0);
  //30-25
  word = 0b0111111'0111101010'000'00000'1100011;
  instr = sim::Decoder::decode(word);
  ASSERT_EQ(instr.imm, 0b011111100000);
  //31
  word = 0b1000000'0111101010'000'00000'1100011;
  instr = sim::Decoder::decode(word);
  ASSERT_EQ(instr.imm, (int32_t)0b11111111111111111111'000000000000);
}

TEST(decoder, U_imm) {
  // 31 - 12
  sim::word_t word = 0b11110000111101010111'11010'0010111;
  auto instr = sim::Decoder::decode(word);
  ASSERT_EQ(instr.imm, 0b11110000111101010111'000000000000);
}

TEST(decoder, J_imm) {
  // 30-21
  sim::word_t word = 0b0'1110000111'0'00000000'00001'1101111;
  auto instr = sim::Decoder::decode(word);
  ASSERT_EQ(instr.imm, 0b011100001110);
  // 20
  word = 0b0'0000000000'1'00000000'00001'1101111;
  instr = sim::Decoder::decode(word);
  ASSERT_EQ(instr.imm, 0b1'00000000000);
  // 19-12
  word = 0b0'0000000000'0'10001111'00001'1101111;
  instr = sim::Decoder::decode(word);
  ASSERT_EQ(instr.imm, 0b10001111'000000000000);
  //31
  word = 0b1'0000000000'0'00000000'00001'1101111;
  instr = sim::Decoder::decode(word);
  ASSERT_EQ(instr.imm, (int32_t)0b111111111111'00000000000000000000);
}
