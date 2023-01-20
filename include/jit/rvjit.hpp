#ifndef RVJIT_HPP
#define RVJIT_HPP

#include "hart/hart.hpp"
#include "isa/instr.hpp"

#include <array>
#include <asmjit/asmjit.h>
#include <concepts>
#include <iterator>
#include <unordered_map>

namespace sim {

#if defined(__x86_64__)
constexpr bool jit_is_supported = true;
#else
constexpr bool jit_is_supported = false;
#endif

/// @brief Defines the requirements for the structure that stores instructions
template <typename ItT>
concept instruction_iterator = requires(ItT it) {
  { it->id } -> std::convertible_to<InstrId>;
  { it->rd } -> std::convertible_to<op_t>;
  { it->rs1 } -> std::convertible_to<op_t>;
  { it->rs2 } -> std::convertible_to<op_t>;
  { it->rs3 } -> std::convertible_to<op_t>;
  { it->imm } -> std::convertible_to<imm_t>;
}
&&std::input_iterator<ItT>;

/// @brief Runtime for jit
class RVJit {
  asmjit::JitRuntime m_rt;
  size_t m_lastTranslated = 0;

public:
  using FunctionTy = void (*)(void);

public:
  /// @brief Translates a range of decoded instructions into native code.
  /// @tparam InputIt - see std::input_iterator
  /// @param begin - first instruction to translate
  /// @param end - not inclusive ( (begin, end] )
  /// @param hart -
  /// @note the hart address will be sewn into the function code.
  ///       If the hart address changes or is destroyed, then calling
  ///       the translated function will result in undefined behavior.
  /// @note Not the entire range can be translated.
  ///       This may happen due to the presence of jump instructions (in this
  ///       case, jump will be the last instruction translated).
  ///       To find out the number of translated functions, call 'translated'.
  /// @return - translated function
  template <instruction_iterator InputIt>
  FunctionTy translate(InputIt begin, InputIt end, Hart *hart);

  template <instruction_iterator InputIt>
  FunctionTy translate_2(InputIt begin, InputIt end, Hart *hart);

  /// @brief frees resources related to translated function
  /// @param ptr - function to release
  /// @return true if successful
  bool release(FunctionTy ptr) {
    return m_rt.release(ptr) == asmjit::kErrorOk;
  };

  /// @brief get count of translated instructions at last call
  /// 'RVJit::translate'
  size_t translated() const { return m_lastTranslated; }

private:
  // used as a wrapper for calling a non-static class member from an assembler
  template <class T>
  static void write_helper(Hart *hart, paddress_t addr, T value) {
    hart->mmu().write<T>(addr, value);
  }
  template <class T> static T read_helper(Hart *hart, paddress_t addr) {
    return hart->mmu().read<T>(addr);
  }
}; // end class RVJit

template <instruction_iterator InputIt>
RVJit::FunctionTy RVJit::translate(InputIt begin, InputIt end, Hart *hart) {
  if constexpr (!jit_is_supported) {
    return nullptr;
  }
  m_lastTranslated = 0;

  // hold cods and relocation information
  asmjit::CodeHolder code;
  code.init(m_rt.environment());

  asmjit::FileLogger logger(stdout);
  // code.setLogger(&logger);

  asmjit::x86::Compiler cc(&code);

  asmjit::FuncNode *funcNode = cc.addFunc(asmjit::FuncSignatureT<void>());

  // virtual registers for rs1, rs2, rs3, rd
  // NOTE: created 32-int (d)
  asmjit::x86::Gp gp_rs1 = cc.newGpd();
  asmjit::x86::Gp gp_rs2 = cc.newGpd();
  asmjit::x86::Gp gp_rs3 = cc.newGpd();
  asmjit::x86::Gp gp_rd = cc.newGpd();
  asmjit::x86::Gp gp_pc = cc.newGpd();

  // translate code
  size_t translated = 0;
  for (auto it = begin; it != end; ++it) {
    int64_t pc_shift = 4;

    switch (it->id) {
    // instructions with: pc += 4
    // and rd = gp_rs1
    case InstrId::ADD:
      cc.mov(gp_rs1, asmjit::x86::dword_ptr((size_t)(&(hart->regs[it->rs1]))));
      cc.mov(gp_rs2, asmjit::x86::dword_ptr((size_t)(&(hart->regs[it->rs2]))));
      cc.add(gp_rs1, gp_rs2);
      cc.mov(asmjit::x86::dword_ptr((size_t)(&(hart->regs[it->rd]))), gp_rs1);
      break;
    case InstrId::ADDI:
      cc.mov(gp_rs1, asmjit::x86::dword_ptr((size_t)(&(hart->regs[it->rs1]))));
      cc.add(gp_rs1, it->imm);
      cc.mov(asmjit::x86::dword_ptr((size_t)(&(hart->regs[it->rd]))), gp_rs1);
      break;
    case InstrId::SUB:
      cc.mov(gp_rs1, asmjit::x86::dword_ptr((size_t)(&(hart->regs[it->rs1]))));
      cc.sub(gp_rs1, asmjit::x86::dword_ptr((size_t)(&(hart->regs[it->rs2]))));
      cc.mov(asmjit::x86::dword_ptr((size_t)(&(hart->regs[it->rd]))), gp_rs1);
      break;

    // bit operations
    case InstrId::AND:
      cc.mov(gp_rs1, asmjit::x86::dword_ptr((size_t)(&(hart->regs[it->rs1]))));
      cc.mov(gp_rs2, asmjit::x86::dword_ptr((size_t)(&(hart->regs[it->rs2]))));
      cc.and_(gp_rs1, gp_rs2);
      cc.mov(asmjit::x86::dword_ptr((size_t)(&(hart->regs[it->rd]))), gp_rs1);
      break;
    case InstrId::ANDI:
      cc.mov(gp_rs1, asmjit::x86::dword_ptr((size_t)(&(hart->regs[it->rs1]))));
      cc.and_(gp_rs1, it->imm);
      cc.mov(asmjit::x86::dword_ptr((size_t)(&(hart->regs[it->rd]))), gp_rs1);
      break;
    case InstrId::OR:
      cc.mov(gp_rd, asmjit::x86::dword_ptr((size_t)(&(hart->regs[it->rs1]))));
      cc.or_(gp_rd, asmjit::x86::dword_ptr((size_t)(&(hart->regs[it->rs2]))));
      cc.mov(asmjit::x86::dword_ptr((size_t)(&(hart->regs[it->rd]))), gp_rd);
      break;
    case InstrId::ORI:
      cc.mov(gp_rd, asmjit::x86::dword_ptr((size_t)(&(hart->regs[it->rs1]))));
      cc.or_(gp_rd, it->imm);
      cc.mov(asmjit::x86::dword_ptr((size_t)(&(hart->regs[it->rd]))), gp_rd);
      break;
    case InstrId::XOR:
      cc.mov(gp_rd, asmjit::x86::dword_ptr((size_t)(&(hart->regs[it->rs1]))));
      cc.xor_(gp_rd, asmjit::x86::dword_ptr((size_t)(&(hart->regs[it->rs2]))));
      cc.mov(asmjit::x86::dword_ptr((size_t)(&(hart->regs[it->rd]))), gp_rd);
      break;
    case InstrId::XORI:
      cc.mov(gp_rd, asmjit::x86::dword_ptr((size_t)(&(hart->regs[it->rs1]))));
      cc.xor_(gp_rd, it->imm);
      cc.mov(asmjit::x86::dword_ptr((size_t)(&(hart->regs[it->rd]))), gp_rd);
      break;
    case InstrId::AUIPC:
      cc.mov(gp_rs1, asmjit::x86::dword_ptr((size_t)(&(hart->pc))));
      cc.add(gp_rs1, it->imm);
      cc.mov(asmjit::x86::dword_ptr((size_t)(&(hart->regs[it->rd]))), gp_rs1);
      break;
    case InstrId::SLT: // FIXME
      cc.mov(gp_rs1, asmjit::x86::dword_ptr((size_t)(&(hart->regs[it->rs1]))));
      cc.mov(gp_rs2, asmjit::x86::dword_ptr((size_t)(&(hart->regs[it->rs2]))));
      cc.cmp(gp_rs2, gp_rs1);
      cc.setg(gp_rd);
      cc.movzx(gp_rd, gp_rd.r8());
      cc.mov(asmjit::x86::dword_ptr((size_t)(&(hart->regs[it->rd]))), gp_rd);
      break;
    case InstrId::SLTI:
      cc.mov(gp_rs1, asmjit::x86::dword_ptr((size_t)(&(hart->regs[it->rs1]))));
      cc.cmp(gp_rs1, it->imm);
      cc.setg(gp_rd);
      cc.movzx(gp_rd, gp_rd.r8());
      cc.mov(asmjit::x86::dword_ptr((size_t)(&(hart->regs[it->rd]))), gp_rd);
      break;
    case InstrId::SLTU: // FIXME
      cc.mov(gp_rs1, asmjit::x86::dword_ptr((size_t)(&(hart->regs[it->rs2]))));
      cc.mov(gp_rs2, asmjit::x86::dword_ptr((size_t)(&(hart->regs[it->rs1]))));
      cc.cmp(gp_rs1, gp_rs2);
      cc.seta(gp_rd);
      cc.movzx(gp_rd, gp_rd.r8());
      cc.mov(asmjit::x86::dword_ptr((size_t)(&(hart->regs[it->rd]))), gp_rd);
      break;
    case InstrId::SLTIU:
      cc.mov(gp_rs1, asmjit::x86::dword_ptr((size_t)(&(hart->regs[it->rs1]))));
      cc.cmp(gp_rs1, it->imm);
      cc.seta(gp_rd);
      cc.movzx(gp_rd, gp_rd.r8());
      cc.mov(asmjit::x86::dword_ptr((size_t)(&(hart->regs[it->rd]))), gp_rd);
      break;
    case InstrId::LUI:
      cc.mov(asmjit::x86::dword_ptr((size_t)(&(hart->regs[it->rd]))), it->imm);
      break;

    // shifts
    case InstrId::SLL:
      cc.mov(gp_rs1, asmjit::x86::dword_ptr((size_t)(&(hart->regs[it->rs1]))));
      cc.mov(gp_rs2, asmjit::x86::dword_ptr((size_t)(&(hart->regs[it->rs2]))));
      cc.shl(gp_rs1, gp_rs2);
      cc.mov(asmjit::x86::dword_ptr((size_t)(&(hart->regs[it->rd]))), gp_rs1);
      break;
    case InstrId::SRA:
      cc.mov(gp_rs1, asmjit::x86::dword_ptr((size_t)(&(hart->regs[it->rs1]))));
      cc.mov(gp_rs2, asmjit::x86::dword_ptr((size_t)(&(hart->regs[it->rs2]))));
      cc.sar(gp_rs1, gp_rs2);
      cc.mov(asmjit::x86::dword_ptr((size_t)(&(hart->regs[it->rd]))), gp_rs1);
      break;
    case InstrId::SRL:
      cc.mov(gp_rs1, asmjit::x86::dword_ptr((size_t)(&(hart->regs[it->rs1]))));
      cc.mov(gp_rs2, asmjit::x86::dword_ptr((size_t)(&(hart->regs[it->rs2]))));
      cc.shr(gp_rs1, gp_rs2);
      cc.mov(asmjit::x86::dword_ptr((size_t)(&(hart->regs[it->rd]))), gp_rs1);
      break;

    // instructions with jump
    case InstrId::BEQ: {
      asmjit::Label l_end = cc.newLabel();
      asmjit::Label lf = cc.newLabel();
      cc.mov(gp_rs1, asmjit::x86::dword_ptr((size_t)(&(hart->regs[it->rs1]))));
      cc.mov(gp_rs2, asmjit::x86::dword_ptr((size_t)(&(hart->regs[it->rs2]))));
      cc.cmp(gp_rs1, gp_rs2);
      cc.jne(lf);
      // shift pc with imm
      cc.mov(gp_pc, asmjit::x86::dword_ptr((size_t)(&(hart->pc))));
      cc.add(gp_pc, it->imm);
      cc.jmp(l_end);
      // shift pc with 4
      cc.bind(lf);
      cc.mov(gp_pc, asmjit::x86::dword_ptr((size_t)(&(hart->pc))));
      cc.add(gp_pc, pc_shift);
      cc.bind(l_end);
      cc.mov(asmjit::x86::dword_ptr((size_t)(&(hart->pc))), gp_pc);
      translated++;
      pc_shift = 0;
      break;
    }
    case InstrId::BGE: {
      asmjit::Label l_end = cc.newLabel();
      asmjit::Label lf = cc.newLabel();
      cc.mov(gp_rs1, asmjit::x86::dword_ptr((size_t)(&(hart->regs[it->rs1]))));
      cc.mov(gp_rs2, asmjit::x86::dword_ptr((size_t)(&(hart->regs[it->rs2]))));
      cc.cmp(gp_rs1, gp_rs2);
      cc.jl(lf);
      // shift pc with imm
      cc.mov(gp_pc, asmjit::x86::dword_ptr((size_t)(&(hart->pc))));
      cc.add(gp_pc, it->imm);
      cc.jmp(l_end);
      // shift pc with 4
      cc.bind(lf);
      cc.mov(gp_pc, asmjit::x86::dword_ptr((size_t)(&(hart->pc))));
      cc.add(gp_pc, pc_shift);
      cc.bind(l_end);
      cc.mov(asmjit::x86::dword_ptr((size_t)(&(hart->pc))), gp_pc);
      translated++;
      pc_shift = 0;
      break;
    }
    case InstrId::BGEU: {
      asmjit::Label l_end = cc.newLabel();
      asmjit::Label lf = cc.newLabel();
      cc.mov(gp_rs1, asmjit::x86::dword_ptr((size_t)(&(hart->regs[it->rs1]))));
      cc.mov(gp_rs2, asmjit::x86::dword_ptr((size_t)(&(hart->regs[it->rs2]))));
      cc.cmp(gp_rs1, gp_rs2);
      cc.jb(lf);
      // shift pc with imm
      cc.mov(gp_pc, asmjit::x86::dword_ptr((size_t)(&(hart->pc))));
      cc.add(gp_pc, it->imm);
      cc.jmp(l_end);
      // shift pc with 4
      cc.bind(lf);
      cc.mov(gp_pc, asmjit::x86::dword_ptr((size_t)(&(hart->pc))));
      cc.add(gp_pc, pc_shift);
      cc.bind(l_end);
      cc.mov(asmjit::x86::dword_ptr((size_t)(&(hart->pc))), gp_pc);
      translated++;
      pc_shift = 0;
      break;
    }
    case InstrId::BLT: {
      asmjit::Label l_end = cc.newLabel();
      asmjit::Label lf = cc.newLabel();
      cc.mov(gp_rs1, asmjit::x86::dword_ptr((size_t)(&(hart->regs[it->rs1]))));
      cc.mov(gp_rs2, asmjit::x86::dword_ptr((size_t)(&(hart->regs[it->rs2]))));
      cc.cmp(gp_rs1, gp_rs2);
      cc.jge(lf);
      // shift pc with imm
      cc.mov(gp_pc, asmjit::x86::dword_ptr((size_t)(&(hart->pc))));
      cc.add(gp_pc, it->imm);
      cc.jmp(l_end);
      // shift pc with 4
      cc.bind(lf);
      cc.mov(gp_pc, asmjit::x86::dword_ptr((size_t)(&(hart->pc))));
      cc.add(gp_pc, pc_shift);
      cc.bind(l_end);
      cc.mov(asmjit::x86::dword_ptr((size_t)(&(hart->pc))), gp_pc);
      translated++;
      pc_shift = 0;
      break;
    }
    case InstrId::BLTU: {
      asmjit::Label l_end = cc.newLabel();
      asmjit::Label lf = cc.newLabel();
      cc.mov(gp_rs1, asmjit::x86::dword_ptr((size_t)(&(hart->regs[it->rs1]))));
      cc.mov(gp_rs2, asmjit::x86::dword_ptr((size_t)(&(hart->regs[it->rs2]))));
      cc.cmp(gp_rs1, gp_rs2);
      cc.jae(lf);
      // shift pc with imm
      cc.mov(gp_pc, asmjit::x86::dword_ptr((size_t)(&(hart->pc))));
      cc.add(gp_pc, it->imm);
      cc.jmp(l_end);
      // shift pc with 4
      cc.bind(lf);
      cc.mov(gp_pc, asmjit::x86::dword_ptr((size_t)(&(hart->pc))));
      cc.add(gp_pc, pc_shift);
      cc.bind(l_end);
      cc.mov(asmjit::x86::dword_ptr((size_t)(&(hart->pc))), gp_pc);
      translated++;
      pc_shift = 0;
      break;
    }
    case InstrId::BNE: {
      asmjit::Label l_end = cc.newLabel();
      asmjit::Label lf = cc.newLabel();
      cc.mov(gp_rs1, asmjit::x86::dword_ptr((size_t)(&(hart->regs[it->rs1]))));
      cc.mov(gp_rs2, asmjit::x86::dword_ptr((size_t)(&(hart->regs[it->rs2]))));
      cc.cmp(gp_rs1, gp_rs2);
      cc.je(lf);
      // shift pc with imm
      cc.mov(gp_pc, asmjit::x86::dword_ptr((size_t)(&(hart->pc))));
      cc.add(gp_pc, it->imm);
      cc.jmp(l_end);
      // shift pc with 4
      cc.bind(lf);
      cc.mov(gp_pc, asmjit::x86::dword_ptr((size_t)(&(hart->pc))));
      cc.add(gp_pc, pc_shift);
      cc.bind(l_end);
      cc.mov(asmjit::x86::dword_ptr((size_t)(&(hart->pc))), gp_pc);
      translated++;
      pc_shift = 0;
      break;
    }

    case InstrId::JAL:
      cc.mov(gp_rd, asmjit::x86::dword_ptr((size_t)(&(hart->pc))));
      cc.mov(gp_pc, gp_rd);
      cc.add(gp_rd, 0x4);
      cc.mov(asmjit::x86::dword_ptr((size_t)(&(hart->regs[it->rd]))), gp_rd);
      cc.add(gp_pc, it->imm);
      cc.mov(asmjit::x86::dword_ptr((size_t)(&(hart->pc))), gp_pc);
      translated++;
      pc_shift = 0;
      break;
    case InstrId::JALR:
      cc.mov(gp_rd, asmjit::x86::dword_ptr((size_t)(&(hart->pc))));
      cc.add(gp_rd, 0x4);
      cc.mov(gp_pc, asmjit::x86::dword_ptr((size_t)(&(hart->regs[it->rs1]))));
      cc.add(gp_pc, it->imm);
      cc.and_(gp_pc, ~0b1);
      cc.mov(asmjit::x86::dword_ptr((size_t)(&(hart->regs[it->rd]))), gp_rd);
      cc.mov(asmjit::x86::dword_ptr((size_t)(&(hart->pc))), gp_pc);
      translated++;
      pc_shift = 0;
      break;

    // loads
    case InstrId::LB: {
      cc.mov(gp_rs1, asmjit::x86::dword_ptr((size_t)(&(hart->regs[it->rs1]))));
      cc.add(gp_rs1, it->imm);
      // call mmu load
      asmjit::InvokeNode *invoke = nullptr;
      cc.invoke(&invoke, reinterpret_cast<size_t>(read_helper<bword_t>),
                asmjit::FuncSignatureT<bword_t, Hart *, paddress_t>());
      invoke->setArg(0, hart);
      invoke->setArg(1, gp_rs1);
      invoke->setRet(0, gp_rd);
      // sign extend result
      cc.shl(gp_rd, 24);
      cc.sar(gp_rd, 24);
      cc.mov(asmjit::x86::dword_ptr((size_t)(&(hart->regs[it->rd]))), gp_rd);
      break;
    }
    case InstrId::LBU: {
      cc.mov(gp_rs1, asmjit::x86::dword_ptr((size_t)(&(hart->regs[it->rs1]))));
      cc.add(gp_rs1, it->imm);
      cc.mov(gp_rd, 0);
      // call mmu load
      asmjit::InvokeNode *invoke = nullptr;
      cc.invoke(&invoke, reinterpret_cast<size_t>(read_helper<bword_t>),
                asmjit::FuncSignatureT<bword_t, Hart *, paddress_t>());
      invoke->setArg(0, hart);
      invoke->setArg(1, gp_rs1);
      invoke->setRet(0, gp_rd);
      // sign extend result
      cc.mov(asmjit::x86::dword_ptr((size_t)(&(hart->regs[it->rd]))), gp_rd);
      break;
    }
    case InstrId::LH: {
      cc.mov(gp_rs1, asmjit::x86::dword_ptr((size_t)(&(hart->regs[it->rs1]))));
      cc.add(gp_rs1, it->imm);
      // call mmu load
      asmjit::InvokeNode *invoke = nullptr;
      cc.invoke(&invoke, reinterpret_cast<size_t>(read_helper<hword_t>),
                asmjit::FuncSignatureT<hword_t, Hart *, paddress_t>());
      invoke->setArg(0, hart);
      invoke->setArg(1, gp_rs1);
      invoke->setRet(0, gp_rd);
      // sign extend result
      cc.shl(gp_rd, 16);
      cc.sar(gp_rd, 16);
      cc.mov(asmjit::x86::dword_ptr((size_t)(&(hart->regs[it->rd]))), gp_rd);
      break;
    }
    case InstrId::LHU: {
      cc.mov(gp_rs1, asmjit::x86::dword_ptr((size_t)(&(hart->regs[it->rs1]))));
      cc.add(gp_rs1, it->imm);
      cc.mov(gp_rd, 0);
      // call mmu load
      asmjit::InvokeNode *invoke = nullptr;
      cc.invoke(&invoke, reinterpret_cast<size_t>(read_helper<hword_t>),
                asmjit::FuncSignatureT<hword_t, Hart *, paddress_t>());
      invoke->setArg(0, hart);
      invoke->setArg(1, gp_rs1);
      invoke->setRet(0, gp_rd);
      // sign extend result
      cc.mov(asmjit::x86::dword_ptr((size_t)(&(hart->regs[it->rd]))), gp_rd);
      break;
    }
    case InstrId::LW: {
      cc.mov(gp_rs1, asmjit::x86::dword_ptr((size_t)(&(hart->regs[it->rs1]))));
      cc.add(gp_rs1, it->imm);
      cc.mov(gp_rd, 0);
      // call mmu load
      asmjit::InvokeNode *invoke = nullptr;
      cc.invoke(&invoke, reinterpret_cast<size_t>(read_helper<word_t>),
                asmjit::FuncSignatureT<word_t, Hart *, paddress_t>());
      invoke->setArg(0, hart);
      invoke->setArg(1, gp_rs1);
      invoke->setRet(0, gp_rd);
      // sign extend result
      cc.mov(asmjit::x86::dword_ptr((size_t)(&(hart->regs[it->rd]))), gp_rd);
      break;
    }

    case InstrId::SB: {
      // calc addr
      cc.mov(gp_rs1, asmjit::x86::dword_ptr((size_t)(&(hart->regs[it->rs1]))));
      cc.mov(gp_rs2, asmjit::x86::dword_ptr((size_t)(&(hart->regs[it->rs2]))));
      cc.add(gp_rs1, it->imm);
      // call mmu store
      asmjit::InvokeNode *invoke = nullptr;
      cc.invoke(&invoke, reinterpret_cast<size_t>(write_helper<bword_t>),
                asmjit::FuncSignatureT<void, Hart *, paddress_t, bword_t>());
      invoke->setArg(0, hart);
      invoke->setArg(1, gp_rs1);
      invoke->setArg(2, gp_rs2);
      break;
    }
    case InstrId::SH: {
      // calc addr
      cc.mov(gp_rs1, asmjit::x86::dword_ptr((size_t)(&(hart->regs[it->rs1]))));
      cc.mov(gp_rs2, asmjit::x86::dword_ptr((size_t)(&(hart->regs[it->rs2]))));
      cc.add(gp_rs1, it->imm);
      // call mmu store
      asmjit::InvokeNode *invoke = nullptr;
      cc.invoke(&invoke, reinterpret_cast<size_t>(write_helper<hword_t>),
                asmjit::FuncSignatureT<void, Hart *, paddress_t, hword_t>());
      invoke->setArg(0, hart);
      invoke->setArg(1, gp_rs1);
      invoke->setArg(2, gp_rs2);
      break;
    }
    case InstrId::SW: {
      // calc addr
      cc.mov(gp_rs1, asmjit::x86::dword_ptr((size_t)(&(hart->regs[it->rs1]))));
      cc.mov(gp_rs2, asmjit::x86::dword_ptr((size_t)(&(hart->regs[it->rs2]))));
      cc.add(gp_rs1, it->imm);
      // call mmu store
      asmjit::InvokeNode *invoke = nullptr;
      cc.invoke(&invoke, reinterpret_cast<size_t>(write_helper<word_t>),
                asmjit::FuncSignatureT<void, Hart *, paddress_t, word_t>());
      invoke->setArg(0, hart);
      invoke->setArg(1, gp_rs1);
      invoke->setArg(2, gp_rs2);
      break;
    }
    // stop translate
    case InstrId::EBREAK:
    case InstrId::ECALL:
    case InstrId::FENCE:
    default:
      pc_shift = 0;
      break;
    }
    // shift pc += 4
    if (pc_shift) {
      cc.mov(gp_pc, asmjit::x86::dword_ptr((size_t)(&(hart->pc))));
      cc.add(gp_pc, pc_shift);
      cc.mov(asmjit::x86::dword_ptr((size_t)(&(hart->pc))), gp_pc);
      translated++;
    } else {
      break;
    }
  }

  if (translated == 0) {
    return nullptr;
  }

  // end function generation
  cc.endFunc();
  cc.finalize();

  FunctionTy res = {};
  asmjit::Error error = m_rt.add(&res, &code);
  if (error)
    return nullptr;

  m_lastTranslated = translated;
  return res;
} // end RVJit::translate

template <instruction_iterator InputIt>
RVJit::FunctionTy RVJit::translate_2(InputIt begin, InputIt end, Hart *hart) {
  if constexpr (!jit_is_supported) {
    return nullptr;
  }
  m_lastTranslated = 0;

  // hold cods and relocation information
  asmjit::CodeHolder code;
  code.init(m_rt.environment());

  asmjit::FileLogger logger(stdout);
  //code.setLogger(&logger);

  asmjit::x86::Compiler cc(&code);

  asmjit::FuncNode *funcNode = cc.addFunc(asmjit::FuncSignatureT<void>());

  struct RegInfo final {
    asmjit::x86::Gp reg;
    bool loaded = false;
    bool changed = false;
  };
  // virtual reg file
  std::array<RegInfo, 32> regFile = {};
  for (auto &&reg : regFile) {
    reg.reg = cc.newGpd();
  }
  asmjit::x86::Gp pc = cc.newGpd();
  asmjit::x86::Gp tmp = cc.newGpd();

  // translate code
  cc.mov(pc, asmjit::x86::dword_ptr((size_t)(&(hart->pc))));

  size_t translated = 0;
  bool cont_translate = true;
  for (auto it = begin; it != end && cont_translate; ++it) {
    asmjit::x86::Gp rs1 = regFile[it->rs1].reg;
    asmjit::x86::Gp rs2 = regFile[it->rs2].reg;
    asmjit::x86::Gp rs3 = regFile[it->rs3].reg;
    asmjit::x86::Gp rd = regFile[it->rd].reg;

    // load the register once and only if it has not been overwritten yet
    if (!regFile[it->rs1].loaded && !regFile[it->rs1].changed) {
      cc.mov(rs1, asmjit::x86::dword_ptr((size_t)(&(hart->regs[it->rs1]))));
      regFile[it->rs1].loaded = true;
    }
    if (!regFile[it->rs2].loaded && !regFile[it->rs2].changed) {
      cc.mov(rs2, asmjit::x86::dword_ptr((size_t)(&(hart->regs[it->rs2]))));
      regFile[it->rs2].loaded = true;
    }
    if (!regFile[it->rs3].loaded && !regFile[it->rs3].changed) {
      cc.mov(rs3, asmjit::x86::dword_ptr((size_t)(&(hart->regs[it->rs3]))));
      regFile[it->rs3].loaded = true;
    }

    bool go_end_immediate = false;
    bool rd_changed = true;
    size_t pc_change = 0x4;
    switch (it->id) {
    case InstrId::ADD:
      cc.mov(tmp, rs2);
      cc.mov(rd, rs1);
      cc.add(rd, tmp);
      break;
    case InstrId::ADDI:
      cc.mov(rd, rs1);
      cc.add(rd, it->imm);
      break;
    case InstrId::SUB:
      cc.mov(tmp, rs2);
      cc.mov(rd, rs1);
      cc.sub(rd, tmp);
      break;

    // bit operations
    case InstrId::AND:
      cc.mov(tmp, rs2);
      cc.mov(rd, rs1);
      cc.and_(rd, tmp);
      break;
    case InstrId::ANDI:
      cc.mov(rd, rs1);
      cc.and_(rd, it->imm);
      break;
    case InstrId::OR:
      cc.mov(tmp, rs2);
      cc.mov(rd, rs1);
      cc.or_(rd, tmp);
      break;
    case InstrId::ORI:
      cc.mov(rd, rs1);
      cc.and_(rd, it->imm);
      break;
    case InstrId::XOR:
      cc.mov(tmp, rs2);
      cc.mov(rd, rs1);
      cc.xor_(rd, tmp);
      break;
    case InstrId::XORI:
      cc.mov(rd, rs1);
      cc.xor_(rd, it->imm);
      break;

    case InstrId::AUIPC:
      cc.mov(rd, pc);
      cc.add(rd, it->imm);
      break;

    case InstrId::SLT:
      cc.xor_(tmp, tmp);
      cc.cmp(rs2, rs1);
      cc.setg(tmp);
      cc.mov(rd, tmp);
      break;
    case InstrId::SLTI:
      cc.xor_(tmp, tmp);
      cc.cmp(rs1, it->imm);
      cc.setg(tmp);
      cc.mov(rd, tmp);
      break;
    case InstrId::SLTU:
      cc.xor_(tmp, tmp);
      cc.cmp(rs2, rs1);
      cc.seta(tmp);
      cc.mov(rd, tmp);
      break;
    case InstrId::SLTIU:
      cc.xor_(tmp, tmp);
      cc.cmp(rs1, it->imm);
      cc.seta(tmp);
      cc.mov(rd, tmp);
      break;

    // shifts
    case InstrId::SLL:
      cc.mov(tmp, rs2);
      cc.mov(rd, rs1);
      cc.shl(rd, tmp);
      break;
    case InstrId::SLLI:
      cc.mov(rd, rs1);
      cc.shl(rd, it->imm);
      break;
    case InstrId::SRA:
      cc.mov(tmp, rs2);
      cc.mov(rd, rs1);
      cc.sar(rd, tmp);
      break;
    case InstrId::SRAI:
      cc.mov(rd, rs1);
      cc.sar(rd, it->imm);
      break;
    case InstrId::SRL:
      cc.mov(tmp, rs2);
      cc.mov(rd, rs1);
      cc.shr(rd, tmp);
      break;
    case InstrId::SRLI:
      cc.mov(rd, rs1);
      cc.shr(rd, it->imm);
      break;

    case InstrId::LUI:
      cc.mov(rd, it->imm);
      break;

    // instructions with jump
    case InstrId::BEQ: {
      asmjit::Label l_end = cc.newLabel();
      asmjit::Label lf = cc.newLabel();
      cc.cmp(rs1, rs2);
      cc.jne(lf);
      // shift pc with imm
      cc.add(pc, it->imm);
      cc.jmp(l_end);
      // shift pc with 4
      cc.bind(lf);
      cc.add(pc, 0x4);
      cc.bind(l_end);
      translated++;
      go_end_immediate = true;
      break;
    }
    case InstrId::BGE: {
      asmjit::Label l_end = cc.newLabel();
      asmjit::Label lf = cc.newLabel();
      cc.cmp(rs1, rs2);
      cc.jl(lf);
      // shift pc with imm
      cc.add(pc, it->imm);
      cc.jmp(l_end);
      // shift pc with 4
      cc.bind(lf);
      cc.add(pc, 0x4);
      cc.bind(l_end);
      translated++;
      go_end_immediate = true;
      break;
    }
    case InstrId::BGEU: {
      asmjit::Label l_end = cc.newLabel();
      asmjit::Label lf = cc.newLabel();
      cc.cmp(rs1, rs2);
      cc.jb(lf);
      // shift pc with imm
      cc.add(pc, it->imm);
      cc.jmp(l_end);
      // shift pc with 4
      cc.bind(lf);
      cc.add(pc, 0x4);
      cc.bind(l_end);
      translated++;
      go_end_immediate = true;
      break;
    }
    case InstrId::BLT: {
      asmjit::Label l_end = cc.newLabel();
      asmjit::Label lf = cc.newLabel();
      cc.cmp(rs1, rs2);
      cc.jge(lf);
      // shift pc with imm
      cc.add(pc, it->imm);
      cc.jmp(l_end);
      // shift pc with 4
      cc.bind(lf);
      cc.add(pc, 0x4);
      cc.bind(l_end);
      translated++;
      go_end_immediate = true;
      break;
    }
    case InstrId::BLTU: {
      asmjit::Label l_end = cc.newLabel();
      asmjit::Label lf = cc.newLabel();
      cc.cmp(rs1, rs2);
      cc.jae(lf);
      // shift pc with imm
      cc.add(pc, it->imm);
      cc.jmp(l_end);
      // shift pc with 4
      cc.bind(lf);
      cc.add(pc, 0x4);
      cc.bind(l_end);
      translated++;
      go_end_immediate = true;
      break;
    }
    case InstrId::BNE: {
      asmjit::Label l_end = cc.newLabel();
      asmjit::Label lf = cc.newLabel();
      cc.cmp(rs1, rs2);
      cc.je(lf);
      // shift pc with imm
      cc.add(pc, it->imm);
      cc.jmp(l_end);
      // shift pc with 4
      cc.bind(lf);
      cc.add(pc, 0x4);
      cc.bind(l_end);
      translated++;
      go_end_immediate = true;
      break;
    }

    case InstrId::JAL:
      cc.mov(rd, pc);
      cc.add(rd, 0x4);
      cc.add(pc, it->imm);
      cont_translate = false;
      pc_change = 0;
      break;
    case InstrId::JALR:
      cc.mov(rd, pc);
      cc.add(rd, 0x4);
      cc.mov(pc, rs1);
      cc.add(pc, it->imm);
      cc.and_(pc, ~0b1);
      cont_translate = false;
      pc_change = 0;
      break;

    // loads
    case InstrId::LB: {
      // calc addr
      cc.mov(tmp, rs1);
      cc.add(tmp, it->imm);
      // call mmu load
      asmjit::InvokeNode *invoke = nullptr;
      cc.invoke(&invoke, reinterpret_cast<size_t>(read_helper<bword_t>),
                asmjit::FuncSignatureT<bword_t, Hart *, paddress_t>());
      invoke->setArg(0, hart);
      invoke->setArg(1, tmp);
      invoke->setRet(0, rd);
      // sign extend result
      cc.shl(rd, 24);
      cc.sar(rd, 24);
      break;
    }
    case InstrId::LBU: {
      // calc addr
      cc.mov(tmp, rs1);
      cc.add(tmp, it->imm);
      cc.mov(rd, 0);
      // call mmu load
      asmjit::InvokeNode *invoke = nullptr;
      cc.invoke(&invoke, reinterpret_cast<size_t>(read_helper<bword_t>),
                asmjit::FuncSignatureT<bword_t, Hart *, paddress_t>());
      invoke->setArg(0, hart);
      invoke->setArg(1, tmp);
      invoke->setRet(0, rd);
      break;
    }
    case InstrId::LH: {
      // calc addr
      cc.mov(tmp, rs1);
      cc.add(tmp, it->imm);
      // call mmu load
      asmjit::InvokeNode *invoke = nullptr;
      cc.invoke(&invoke, reinterpret_cast<size_t>(read_helper<hword_t>),
                asmjit::FuncSignatureT<hword_t, Hart *, paddress_t>());
      invoke->setArg(0, hart);
      invoke->setArg(1, tmp);
      invoke->setRet(0, rd);
      // sign extend result
      cc.shl(rd, 16);
      cc.sar(rd, 16);
      break;
    }
    case InstrId::LHU: {
      // calc addr
      cc.mov(tmp, rs1);
      cc.add(tmp, it->imm);
      cc.xor_(rd, rd);
      // call mmu load
      asmjit::InvokeNode *invoke = nullptr;
      cc.invoke(&invoke, reinterpret_cast<size_t>(read_helper<hword_t>),
                asmjit::FuncSignatureT<hword_t, Hart *, paddress_t>());
      invoke->setArg(0, hart);
      invoke->setArg(1, tmp);
      invoke->setRet(0, rd);
      break;
    }
    case InstrId::LW: {
      // calc addr
      cc.mov(tmp, rs1);
      cc.add(tmp, it->imm);
      // call mmu load
      asmjit::InvokeNode *invoke = nullptr;
      cc.invoke(&invoke, reinterpret_cast<size_t>(read_helper<word_t>),
                asmjit::FuncSignatureT<word_t, Hart *, paddress_t>());
      invoke->setArg(0, hart);
      invoke->setArg(1, tmp);
      invoke->setRet(0, rd);
      break;
    }

    // stores
    // TODO: check changes at code section
    case InstrId::SB: {
      // calc addr
      cc.mov(tmp, rs1);
      cc.add(tmp, it->imm);
      // call mmu store
      asmjit::InvokeNode *invoke = nullptr;
      cc.invoke(&invoke, reinterpret_cast<size_t>(write_helper<bword_t>),
                asmjit::FuncSignatureT<void, Hart *, paddress_t, bword_t>());
      invoke->setArg(0, hart);
      invoke->setArg(1, tmp);
      invoke->setArg(2, rs2);
      break;
    }
    case InstrId::SH: {
      // calc addr
      cc.mov(tmp, rs1);
      cc.add(tmp, it->imm);
      // call mmu store
      asmjit::InvokeNode *invoke = nullptr;
      cc.invoke(&invoke, reinterpret_cast<size_t>(write_helper<hword_t>),
                asmjit::FuncSignatureT<void, Hart *, paddress_t, hword_t>());
      invoke->setArg(0, hart);
      invoke->setArg(1, tmp);
      invoke->setArg(2, rs2);
      break;
    }
    case InstrId::SW: {
      // calc addr
      cc.mov(tmp, rs1);
      cc.add(tmp, it->imm);
      // call mmu store
      asmjit::InvokeNode *invoke = nullptr;
      cc.invoke(&invoke, reinterpret_cast<size_t>(write_helper<word_t>),
                asmjit::FuncSignatureT<void, Hart *, paddress_t, word_t>());
      invoke->setArg(0, hart);
      invoke->setArg(1, tmp);
      invoke->setArg(2, rs2);
      break;
    }
    // stop translate
    case InstrId::EBREAK:
    case InstrId::ECALL:
    case InstrId::FENCE:
    default:
      go_end_immediate = true;
      break;
    } // end switch

    if (go_end_immediate)
      break;

    if (pc_change != 0)
      cc.add(pc, pc_change);

    // mark rd as changed register (need store it back)
    regFile[it->rd].changed |= rd_changed;
    if (it->rd == 0 && rd_changed) {
      cc.xor_(rd, rd);
    }
    translated++;
  } // end for it

  // empty function
  if (translated == 0) {
    return nullptr;
  }

  // Save registers.
  // Only the changed registers will be saved.
  cc.mov(asmjit::x86::dword_ptr((size_t)(&(hart->pc))), pc);
  for (int i = 1; i < 32; ++i) {
    if (regFile[i].changed) {
      cc.mov(asmjit::x86::dword_ptr((size_t)(&(hart->regs[i]))),
             regFile[i].reg);
    }
  }

  // End function generation
  cc.endFunc();
  cc.finalize();

  FunctionTy res = {};
  asmjit::Error error = m_rt.add(&res, &code);
  if (error)
    return nullptr;

  m_lastTranslated = translated;
  return res;
} // end RVJit::translate_2

} // namespace sim

#endif