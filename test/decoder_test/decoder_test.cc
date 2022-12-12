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