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