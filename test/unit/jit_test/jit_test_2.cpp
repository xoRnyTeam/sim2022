#include "executor/executor.hpp"
#include "jit/rvjit.hpp"
//
#include "header.hpp"

#include <vector>

using namespace sim;

TEST(Jit_2, add) {
  sim::RVJit jit;
  sim::Hart hart;

  hart.regs[1] = 0x1;
  hart.regs[2] = 0x5;
  hart.pc = 0xf0;
  std::vector<Instruction> instrs = {
      Instruction{1, 2, 0, 0, 2, 0, InstrId::ADD}};

  auto fn = jit.translate_2(instrs.begin(), instrs.end(), &hart);
  ASSERT_NE(fn, nullptr);

  fn();
  EXPECT_EQ(hart.regs[1], 0x1);
  EXPECT_EQ(hart.regs[2], 0x6);
  EXPECT_EQ(hart.pc, 0xf4);

  fn();
  EXPECT_EQ(hart.regs[1], 0x1);
  EXPECT_EQ(hart.regs[2], 0x7);
  EXPECT_EQ(hart.pc, 0xf8);
}

TEST(Jit_2, shifts) {
  sim::RVJit jit;
  sim::Hart hart;

  { // SLL
    hart.regs[1] = 0xf0015;
    hart.regs[2] = 0x5;
    hart.pc = 0xf0;
    std::vector<Instruction> instrs = {
        Instruction{1, 2, 0, 0, 2, 0, InstrId::SLL}};

    auto fn = jit.translate_2(instrs.begin(), instrs.end(), &hart);
    ASSERT_NE(fn, nullptr);

    fn();
    EXPECT_EQ(hart.regs[1], 0xf0015);
    EXPECT_EQ(hart.regs[2], 0xf0015 << 0x5);
    EXPECT_EQ(hart.pc, 0xf4);
  }
  { // SRA
    hart.regs[1] = 0xf0015;
    hart.regs[2] = 0x5;
    hart.pc = 0xf0;
    std::vector<Instruction> instrs = {
        Instruction{1, 2, 0, 0, 2, 0, InstrId::SRA}};

    auto fn = jit.translate_2(instrs.begin(), instrs.end(), &hart);
    ASSERT_NE(fn, nullptr);

    fn();
    EXPECT_EQ(hart.regs[1], 0xf0015);
    EXPECT_EQ(hart.regs[2], (int)0xf0015 >> 0x5);
    EXPECT_EQ(hart.pc, 0xf4);
  }
  { // SRL
    hart.regs[1] = 0xf0015;
    hart.regs[2] = 0x5;
    hart.pc = 0xf0;
    std::vector<Instruction> instrs = {
        Instruction{1, 2, 0, 0, 2, 0, InstrId::SRL}};

    auto fn = jit.translate_2(instrs.begin(), instrs.end(), &hart);
    ASSERT_NE(fn, nullptr);

    fn();
    EXPECT_EQ(hart.regs[1], 0xf0015);
    EXPECT_EQ(hart.regs[2], (unsigned)0xf0015 >> 0x5);
    EXPECT_EQ(hart.pc, 0xf4);
  }
}

TEST(Jit_2, addi) {
  sim::RVJit jit;
  sim::Hart hart;

  hart.regs[1] = 0xaf32;
  hart.pc = 0xf0;
  std::vector<Instruction> instrs = {
      Instruction{1, 0, 0, 0, 2, 0xfa, InstrId::ADDI}};

  auto fn = jit.translate_2(instrs.begin(), instrs.end(), &hart);
  ASSERT_NE(fn, nullptr);

  fn();
  EXPECT_EQ(hart.regs[1], 0xaf32);
  EXPECT_EQ(hart.regs[2], 0xaf32 + 0xfa);
  EXPECT_EQ(hart.pc, 0xf4);
}

TEST(Jit_2, and) {
  sim::RVJit jit;
  sim::Hart hart;

  hart.regs[1] = 0xf1;
  hart.regs[2] = 0xa5;
  hart.regs[3] = 0xbd;
  hart.regs[4] = 0xff3412;
  hart.pc = 0xf0;
  std::vector<Instruction> instrs = {
      Instruction{1, 2, 0, 0, 2, 0, InstrId::AND},
      Instruction{3, 0, 0, 0, 4, 0xb7, InstrId::ANDI}};

  auto fn = jit.translate_2(instrs.begin(), instrs.end(), &hart);
  ASSERT_NE(fn, nullptr);

  fn();
  EXPECT_EQ(hart.regs[1], 0xf1);
  EXPECT_EQ(hart.regs[2], 0xf1 & 0xa5);
  EXPECT_EQ(hart.regs[3], 0xbd);
  EXPECT_EQ(hart.regs[4], 0xbd & 0xb7);
  EXPECT_EQ(hart.pc, 0xf8);
}

TEST(Jit_2, branches) {
  sim::RVJit jit;
  sim::Hart hart;

  // BEQ
  { // false
    hart.regs[1] = 0xf1;
    hart.regs[2] = 0xa5;
    hart.pc = 0xf0;
    std::vector<Instruction> instrs = {
        Instruction{1, 2, 0, 0, 0, 0xfa, InstrId::BEQ}};

    auto fn = jit.translate_2(instrs.begin(), instrs.end(), &hart);
    ASSERT_NE(fn, nullptr);

    fn();
    EXPECT_EQ(hart.regs[1], 0xf1);
    EXPECT_EQ(hart.regs[2], 0xa5);
    EXPECT_EQ(hart.pc, 0xf0 + 0x4);
  }
  { // true
    hart.regs[1] = 0xf1;
    hart.regs[2] = 0xa5;
    hart.pc = 0xf0;
    std::vector<Instruction> instrs = {
        Instruction{2, 0, 0, 0, 2, 0x4c, InstrId::ADDI},
        Instruction{1, 2, 0, 0, 0, 0xfa, InstrId::BEQ}};

    auto fn = jit.translate_2(instrs.begin(), instrs.end(), &hart);
    ASSERT_NE(fn, nullptr);

    fn();
    EXPECT_EQ(hart.regs[1], 0xf1);
    EXPECT_EQ(hart.regs[2], 0xf1);
    EXPECT_EQ(hart.pc, 0xf0 + 0x4 + 0xfa);
  }

  // BGE
  { // false
    hart.regs[1] = 0xa4;
    hart.regs[2] = 0xa5;
    hart.pc = 0xf0;
    std::vector<Instruction> instrs = {
        Instruction{1, 2, 0, 0, 0, 0xfa, InstrId::BGE}};

    auto fn = jit.translate_2(instrs.begin(), instrs.end(), &hart);
    ASSERT_NE(fn, nullptr);

    fn();
    EXPECT_EQ(hart.regs[1], 0xa4);
    EXPECT_EQ(hart.regs[2], 0xa5);
    EXPECT_EQ(hart.pc, 0xf0 + 0x4);
  }
  { // true
    hart.regs[1] = 0xf1;
    hart.regs[2] = 0xa5;
    hart.pc = 0xf0;
    std::vector<Instruction> instrs = {
        Instruction{2, 0, 0, 0, 2, 0x4c, InstrId::ADDI},
        Instruction{1, 2, 0, 0, 0, 0xfa, InstrId::BGE}};

    auto fn = jit.translate_2(instrs.begin(), instrs.end(), &hart);
    ASSERT_NE(fn, nullptr);

    fn();
    EXPECT_EQ(hart.regs[1], 0xf1);
    EXPECT_EQ(hart.regs[2], 0xf1);
    EXPECT_EQ(hart.pc, 0xf0 + 0x4 + 0xfa);
  }
  { // true
    hart.regs[1] = 0xfff1;
    hart.regs[2] = 0xa5;
    hart.pc = 0xf0;
    std::vector<Instruction> instrs = {
        Instruction{1, 2, 0, 0, 0, 0xfa, InstrId::BGE}};

    auto fn = jit.translate_2(instrs.begin(), instrs.end(), &hart);
    ASSERT_NE(fn, nullptr);

    fn();
    EXPECT_EQ(hart.regs[1], 0xfff1);
    EXPECT_EQ(hart.regs[2], 0xa5);
    EXPECT_EQ(hart.pc, 0xf0 + 0xfa);
  }

  // BGEU
  { // false
    hart.regs[1] = 0xa4;
    hart.regs[2] = 0xa5;
    hart.pc = 0xf0;
    std::vector<Instruction> instrs = {
        Instruction{1, 2, 0, 0, 0, 0xfa, InstrId::BGEU}};

    auto fn = jit.translate_2(instrs.begin(), instrs.end(), &hart);
    ASSERT_NE(fn, nullptr);

    fn();
    EXPECT_EQ(hart.regs[1], 0xa4);
    EXPECT_EQ(hart.regs[2], 0xa5);
    EXPECT_EQ(hart.pc, 0xf0 + 0x4);
  }
  { // true
    hart.regs[1] = 0xf1;
    hart.regs[2] = 0xf1;
    hart.pc = 0xf0;
    std::vector<Instruction> instrs = {
        Instruction{1, 2, 0, 0, 0, 0xfa, InstrId::BGEU}};

    auto fn = jit.translate_2(instrs.begin(), instrs.end(), &hart);
    ASSERT_NE(fn, nullptr);

    fn();
    EXPECT_EQ(hart.regs[1], 0xf1);
    EXPECT_EQ(hart.regs[2], 0xf1);
    EXPECT_EQ(hart.pc, 0xf0 + 0xfa);
  }
  { // true
    hart.regs[1] = 0xf1;
    hart.regs[2] = 0x5;
    hart.pc = 0xf0;
    std::vector<Instruction> instrs = {
        Instruction{2, 0, 0, 0, 2, (int)0xfffffffc, InstrId::ADDI},
        Instruction{1, 2, 0, 0, 0, 0xfa, InstrId::BGEU}};

    auto fn = jit.translate_2(instrs.begin(), instrs.end(), &hart);
    ASSERT_NE(fn, nullptr);

    fn();
    EXPECT_EQ(hart.regs[1], 0xf1);
    EXPECT_EQ(hart.regs[2], 0x5 + 0xfffffffc);
    EXPECT_EQ(hart.pc, 0xf0 + 0x4 + 0xfa);
  }

  // BLT
  { // false
    hart.regs[1] = 0xffffffa6;
    hart.regs[2] = 0xffffffa5;
    hart.pc = 0xf0;
    std::vector<Instruction> instrs = {
        Instruction{1, 2, 0, 0, 0, 0xfa, InstrId::BLT}};

    auto fn = jit.translate_2(instrs.begin(), instrs.end(), &hart);
    ASSERT_NE(fn, nullptr);

    fn();
    EXPECT_EQ(hart.regs[1], 0xffffffa6);
    EXPECT_EQ(hart.regs[2], 0xffffffa5);
    EXPECT_EQ(hart.pc, 0xf0 + 0x4);
  }
  { // false
    hart.regs[1] = 0xffffffa4;
    hart.regs[2] = 0xffffffa4;
    hart.pc = 0xf0;
    std::vector<Instruction> instrs = {
        Instruction{1, 2, 0, 0, 0, 0xfa, InstrId::BLT}};

    auto fn = jit.translate_2(instrs.begin(), instrs.end(), &hart);
    ASSERT_NE(fn, nullptr);

    fn();
    EXPECT_EQ(hart.regs[1], 0xffffffa4);
    EXPECT_EQ(hart.regs[2], 0xffffffa4);
    EXPECT_EQ(hart.pc, 0xf0 + 0x4);
  }
  { // true
    hart.regs[1] = 0x1;
    hart.regs[2] = 0x5;
    hart.pc = 0xf0;
    std::vector<Instruction> instrs = {
        Instruction{1, 2, 0, 0, 0, 0xfa, InstrId::BLT}};

    auto fn = jit.translate_2(instrs.begin(), instrs.end(), &hart);
    ASSERT_NE(fn, nullptr);

    fn();
    EXPECT_EQ(hart.regs[1], 0x1);
    EXPECT_EQ(hart.regs[2], 0x5);
    EXPECT_EQ(hart.pc, 0xf0 + 0xfa);
  }

  // BLTU
  { // true
    hart.regs[1] = 0xffffffa4;
    hart.regs[2] = 0xffffffa5;
    hart.pc = 0xf0;
    std::vector<Instruction> instrs = {
        Instruction{1, 2, 0, 0, 0, 0xfa, InstrId::BLTU}};

    auto fn = jit.translate_2(instrs.begin(), instrs.end(), &hart);
    ASSERT_NE(fn, nullptr);

    fn();
    EXPECT_EQ(hart.regs[1], 0xffffffa4);
    EXPECT_EQ(hart.regs[2], 0xffffffa5);
    EXPECT_EQ(hart.pc, 0xf0 + 0xfa);
  }
  { // false
    hart.regs[1] = 0xffffffa4;
    hart.regs[2] = 0xffffffa4;
    hart.pc = 0xf0;
    std::vector<Instruction> instrs = {
        Instruction{1, 2, 0, 0, 0, 0xfa, InstrId::BLTU}};

    auto fn = jit.translate_2(instrs.begin(), instrs.end(), &hart);
    ASSERT_NE(fn, nullptr);

    fn();
    EXPECT_EQ(hart.regs[1], 0xffffffa4);
    EXPECT_EQ(hart.regs[2], 0xffffffa4);
    EXPECT_EQ(hart.pc, 0xf0 + 0x4);
  }
  { // true
    hart.regs[1] = 0x1;
    hart.regs[2] = 0x5;
    hart.pc = 0xf0;
    std::vector<Instruction> instrs = {
        Instruction{1, 2, 0, 0, 0, 0xfa, InstrId::BLTU}};

    auto fn = jit.translate_2(instrs.begin(), instrs.end(), &hart);
    ASSERT_NE(fn, nullptr);

    fn();
    EXPECT_EQ(hart.regs[1], 0x1);
    EXPECT_EQ(hart.regs[2], 0x5);
    EXPECT_EQ(hart.pc, 0xf0 + 0xfa);
  }

  // BNE
  { // true
    hart.regs[1] = 0xffffffa4;
    hart.regs[2] = 0xffffffa5;
    hart.pc = 0xf0;
    std::vector<Instruction> instrs = {
        Instruction{1, 2, 0, 0, 0, 0xfa, InstrId::BNE}};

    auto fn = jit.translate_2(instrs.begin(), instrs.end(), &hart);
    ASSERT_NE(fn, nullptr);

    fn();
    EXPECT_EQ(hart.regs[1], 0xffffffa4);
    EXPECT_EQ(hart.regs[2], 0xffffffa5);
    EXPECT_EQ(hart.pc, 0xf0 + 0xfa);
  }
  { // false
    hart.regs[1] = 0xffffffa4;
    hart.regs[2] = 0xffffffa4;
    hart.pc = 0xf0;
    std::vector<Instruction> instrs = {
        Instruction{1, 2, 0, 0, 0, 0xfa, InstrId::BNE}};

    auto fn = jit.translate_2(instrs.begin(), instrs.end(), &hart);
    ASSERT_NE(fn, nullptr);

    fn();
    EXPECT_EQ(hart.regs[1], 0xffffffa4);
    EXPECT_EQ(hart.regs[2], 0xffffffa4);
    EXPECT_EQ(hart.pc, 0xf0 + 0x4);
  }
}

TEST(Jit_2, jal_jalr) {
  sim::RVJit jit;
  sim::Hart hart;

  { // jal
    hart.regs[2] = 0xa5;
    hart.pc = 0xf0;
    std::vector<Instruction> instrs = {
        Instruction{0, 0, 0, 0, 2, 0xdf0, InstrId::JAL}};

    auto fn = jit.translate_2(instrs.begin(), instrs.end(), &hart);
    ASSERT_NE(fn, nullptr);

    fn();
    EXPECT_EQ(hart.regs[2], 0xf0 + 0x4);
    EXPECT_EQ(hart.pc, 0xf0 + 0xdf0);
  }

  { // jalr
    hart.regs[1] = 0x76;
    hart.regs[2] = 0xa5;
    hart.pc = 0xf0;
    std::vector<Instruction> instrs = {
        Instruction{1, 0, 0, 0, 2, 0xdf1, InstrId::JALR}};

    auto fn = jit.translate_2(instrs.begin(), instrs.end(), &hart);
    ASSERT_NE(fn, nullptr);

    fn();
    EXPECT_EQ(hart.regs[2], 0xf0 + 0x4);
    EXPECT_EQ(hart.pc, (0x76 + 0xdf1) & (~0b01));
  }
}

TEST(Jit_2, slt) {
  sim::RVJit jit;
  sim::Hart hart;

  // slt
  { // 1
    hart.regs[1] = 0xa3;
    hart.regs[2] = 0xa5;
    hart.pc = 0xf0;
    std::vector<Instruction> instrs = {
        Instruction{1, 2, 0, 0, 3, 0, InstrId::SLT}};

    auto fn = jit.translate_2(instrs.begin(), instrs.end(), &hart);
    ASSERT_NE(fn, nullptr);

    fn();
    EXPECT_EQ(hart.regs[1], 0xa3);
    EXPECT_EQ(hart.regs[2], 0xa5);
    EXPECT_EQ(hart.regs[3], 0x1);
    EXPECT_EQ(hart.pc, 0xf0 + 0x4);
  }

  { // 0
    hart.regs[1] = 0xa7;
    hart.regs[2] = 0xa5;
    hart.regs[3] = 0x1;
    hart.regs[4] = 0xff;
    hart.regs[5] = 0xff;
    hart.regs[6] = 0x1;
    hart.pc = 0xf0;
    std::vector<Instruction> instrs = {
        Instruction{1, 2, 0, 0, 3, 0, InstrId::SLT},
        Instruction{4, 5, 0, 0, 6, 0, InstrId::SLT}};

    auto fn = jit.translate_2(instrs.begin(), instrs.end(), &hart);
    ASSERT_NE(fn, nullptr);

    fn();
    EXPECT_EQ(hart.regs[1], 0xa7);
    EXPECT_EQ(hart.regs[2], 0xa5);
    EXPECT_EQ(hart.regs[3], 0x0);
    EXPECT_EQ(hart.regs[4], 0xff);
    EXPECT_EQ(hart.regs[5], 0xff);
    EXPECT_EQ(hart.regs[6], 0x0);
    EXPECT_EQ(hart.pc, 0xf0 + 0x4 + 0x4);
  }

  // sltu
  { // 1
    hart.regs[1] = -0xa5;
    hart.regs[2] = -0xa3;
    hart.regs[3] = 0;
    hart.pc = 0xf0;
    std::vector<Instruction> instrs = {
        Instruction{1, 2, 0, 0, 3, 0, InstrId::SLTU}};

    auto fn = jit.translate_2(instrs.begin(), instrs.end(), &hart);
    ASSERT_NE(fn, nullptr);

    fn();
    EXPECT_EQ(hart.regs[1], -0xa5);
    EXPECT_EQ(hart.regs[2], -0xa3);
    EXPECT_EQ(hart.regs[3], 0x1);
    EXPECT_EQ(hart.pc, 0xf0 + 0x4);
  }

  { // 0
    hart.regs[1] = 0xa7;
    hart.regs[2] = 0xa5;
    hart.regs[3] = 0x1;
    hart.regs[4] = 0xff;
    hart.regs[5] = 0xff;
    hart.regs[6] = 0x1;
    hart.pc = 0xf0;
    std::vector<Instruction> instrs = {
        Instruction{1, 2, 0, 0, 3, 0, InstrId::SLTU},
        Instruction{4, 5, 0, 0, 6, 0, InstrId::SLTU}};

    auto fn = jit.translate_2(instrs.begin(), instrs.end(), &hart);
    ASSERT_NE(fn, nullptr);

    fn();
    EXPECT_EQ(hart.regs[1], 0xa7);
    EXPECT_EQ(hart.regs[2], 0xa5);
    EXPECT_EQ(hart.regs[3], 0x0);
    EXPECT_EQ(hart.regs[4], 0xff);
    EXPECT_EQ(hart.regs[5], 0xff);
    EXPECT_EQ(hart.regs[6], 0x0);
    EXPECT_EQ(hart.pc, 0xf0 + 0x4 + 0x4);
  }
}

TEST(Jit_2, store) {
  sim::RVJit jit;
  sim::Hart hart;

  { // SW
    hart.regs[1] = 0xa5;
    hart.regs[2] = 0xff2136;
    hart.pc = 0xf0;
    std::vector<Instruction> instrs = {
        Instruction{1, 2, 0, 0, 0, 0x56, InstrId::SW}};

    auto fn = jit.translate_2(instrs.begin(), instrs.end(), &hart);
    ASSERT_NE(fn, nullptr);

    fn();
    EXPECT_EQ(hart.regs[1], 0xa5);
    EXPECT_EQ(hart.regs[2], 0xff2136);
    EXPECT_EQ(hart.mmu().read<word_t>(0xa5 + 0x56), 0xff2136);
    EXPECT_EQ(hart.pc, 0xf0 + 0x4);
  }

  { // SH
    hart.regs[1] = 0xa5;
    hart.regs[2] = 0xff2136;
    hart.pc = 0xf0;
    std::vector<Instruction> instrs = {
        Instruction{1, 2, 0, 0, 0, 0x56, InstrId::SH}};

    auto fn = jit.translate_2(instrs.begin(), instrs.end(), &hart);
    ASSERT_NE(fn, nullptr);

    fn();
    EXPECT_EQ(hart.regs[1], 0xa5);
    EXPECT_EQ(hart.regs[2], 0xff2136);
    EXPECT_EQ(hart.mmu().read<hword_t>(0xa5 + 0x56), 0x2136);
    EXPECT_EQ(hart.pc, 0xf0 + 0x4);
  }
  { // SH
    hart.regs[1] = 0xa5;
    hart.regs[2] = 0xff2136;
    hart.pc = 0xf0;
    std::vector<Instruction> instrs = {
        Instruction{1, 2, 0, 0, 0, 0x56, InstrId::SB}};

    auto fn = jit.translate_2(instrs.begin(), instrs.end(), &hart);
    ASSERT_NE(fn, nullptr);

    fn();
    EXPECT_EQ(hart.regs[1], 0xa5);
    EXPECT_EQ(hart.regs[2], 0xff2136);
    EXPECT_EQ(hart.mmu().read<bword_t>(0xa5 + 0x56), 0x36);
    EXPECT_EQ(hart.pc, 0xf0 + 0x4);
  }
}

TEST(Jit_2, load) {
  sim::RVJit jit;
  sim::Hart hart;

  { // LW
    hart.regs[1] = 0xa5;
    hart.regs[2] = 0xff2136;
    hart.pc = 0xf0;
    std::vector<Instruction> instrs = {
        Instruction{1, 2, 0, 0, 3, 0x56, InstrId::LW}};

    auto fn = jit.translate_2(instrs.begin(), instrs.end(), &hart);
    ASSERT_NE(fn, nullptr);

    hart.mmu().write<word_t>(0x56 + 0xa5, 0xfa67c39f);

    fn();
    EXPECT_EQ(hart.regs[1], 0xa5);
    EXPECT_EQ(hart.regs[2], 0xff2136);
    EXPECT_EQ(hart.regs[3], 0xfa67c39f);
    EXPECT_EQ(hart.pc, 0xf0 + 0x4);
  }

  { // LH
    hart.regs[1] = 0xa5;
    hart.regs[2] = 0xff2136;
    hart.regs[3] = 0xffff2136;
    hart.pc = 0xf0;
    std::vector<Instruction> instrs = {
        Instruction{1, 2, 0, 0, 3, 0x56, InstrId::LH}};

    auto fn = jit.translate_2(instrs.begin(), instrs.end(), &hart);
    ASSERT_NE(fn, nullptr);

    hart.mmu().write<word_t>(0x56 + 0xa5, 0xfa67f39f);

    fn();
    EXPECT_EQ(hart.regs[1], 0xa5);
    EXPECT_EQ(hart.regs[2], 0xff2136);
    EXPECT_EQ(hart.regs[3], ((int)0xf39f << 16) >> 16);
    EXPECT_EQ(hart.pc, 0xf0 + 0x4);
  }
  { // LHU
    hart.regs[1] = 0xa5;
    hart.regs[2] = 0xff2136;
    hart.regs[3] = 0xffff2136;
    hart.pc = 0xf0;
    std::vector<Instruction> instrs = {
        Instruction{1, 2, 0, 0, 3, 0x56, InstrId::LHU}};

    auto fn = jit.translate_2(instrs.begin(), instrs.end(), &hart);
    ASSERT_NE(fn, nullptr);

    hart.mmu().write<word_t>(0x56 + 0xa5, 0xfa67c39f);

    fn();
    EXPECT_EQ(hart.regs[1], 0xa5);
    EXPECT_EQ(hart.regs[2], 0xff2136);
    EXPECT_EQ(hart.regs[3], 0xc39f);
    EXPECT_EQ(hart.pc, 0xf0 + 0x4);
  }

  { // LB
    hart.regs[1] = 0xa5;
    hart.regs[2] = 0xff2136;
    hart.regs[3] = 0xffff2136;
    hart.pc = 0xf0;
    std::vector<Instruction> instrs = {
        Instruction{1, 2, 0, 0, 3, 0x56, InstrId::LB}};

    auto fn = jit.translate_2(instrs.begin(), instrs.end(), &hart);
    ASSERT_NE(fn, nullptr);

    hart.mmu().write<word_t>(0x56 + 0xa5, 0xfa67f39f);

    fn();
    EXPECT_EQ(hart.regs[1], 0xa5);
    EXPECT_EQ(hart.regs[2], 0xff2136);
    EXPECT_EQ(hart.regs[3], ((int)0x9f << 24) >> 24);
    EXPECT_EQ(hart.pc, 0xf0 + 0x4);
  }
  { // LBU
    hart.regs[1] = 0xa5;
    hart.regs[2] = 0xff2136;
    hart.regs[3] = 0xffff2136;
    hart.pc = 0xf0;
    std::vector<Instruction> instrs = {
        Instruction{1, 2, 0, 0, 3, 0x56, InstrId::LBU}};

    auto fn = jit.translate_2(instrs.begin(), instrs.end(), &hart);
    ASSERT_NE(fn, nullptr);

    hart.mmu().write<word_t>(0x56 + 0xa5, 0xfa67c39f);

    fn();
    EXPECT_EQ(hart.regs[1], 0xa5);
    EXPECT_EQ(hart.regs[2], 0xff2136);
    EXPECT_EQ(hart.regs[3], 0x9f);
    EXPECT_EQ(hart.pc, 0xf0 + 0x4);
  }
}