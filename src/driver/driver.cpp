#include "driver/driver.hpp"
#include "utils/compiler.hpp"
#include <iomanip>
#include <sstream>

namespace sim {

Driver::Driver(const std::string &path, const std::string &path_trace) {
  ElfLoader loader{path};
  loader.load(m_hart.mmu().getMemory());
  //
  trace_out.open(path_trace);
  if (trace_out.is_open()) {
    m_hart.trace_out = &(trace_out);
  }
  //
  m_hart.pc = loader.get_entry();
}

std::string RegfileStr(const Hart &hart) {
  std::stringstream ss{};
  ss << std::setfill('0');
  constexpr std::size_t lineNum = 8;

  for (std::size_t i = 0; i < lineNum; ++i) {
    for (std::size_t j = 0; j < 32 / lineNum; ++j) {
      auto regIdx = j * lineNum + i;
      auto &reg = hart.regs[regIdx];
      ss << "  [" << std::dec << std::setw(2) << regIdx << "] ";
      ss << "0x" << std::hex << std::setw(sizeof(reg) * 2) << reg;
    }
    ss << std::endl;
  }

  return ss.str();
}

void Driver::run() {
  m_instCounter = 0;

  while (!is_terminate()) {
    // try bt
#if 0
    auto &&translatedIt = m_translated.find(m_hart.pc);
    if (translatedIt != m_translated.end()) {
      m_instCounter += translatedIt->second.second;
      translatedIt->second.first();
      continue;
    } else if (m_nativeBBChache.count(m_hart.pc)) {
      auto &&BB = m_nativeBBChache[m_hart.pc];
      if (BB.size() > 3) {
        auto fn = m_jit.translate_2(BB.begin(), BB.end(), &m_hart);
        if (fn) {
          m_translated[m_hart.pc] = std::make_pair(fn, m_jit.translated());
          fn();
          continue;
        }
      }
    }
#endif

    // interpret
    auto &&BB = lookupBB(m_hart.pc);
    for (Instruction &instr : BB) {
      // save old state
      paddress_t tmp_pc = m_hart.pc;

      // execute instr
      m_executor.exec(instr, m_hart);

      // print trace with using old state
      if (unlikely(trace_out.is_open())) {
        trace_out
            << "*************************************************************"
               "******************"
            << std::endl;
        trace_out << std::hex << "0x" << tmp_pc << ": " << InstrId2str[instr.id]
                  << std::dec << " rd = " << (int)instr.rd
                  << ", rs1 = " << (int)instr.rs1
                  << ", rs2 = " << (int)instr.rs2
                  << ", rs3 = " << (int)instr.rs3 << std::hex << ", imm = 0x"
                  << instr.imm << std::dec << std::endl;

        // trace PC change
        trace_out << "PC = 0x" << std::hex << m_hart.getPC() << '\n';

        // it is assumed that the result is always rd.
        if (instr.rd != 0) {
          trace_out << "x" << std::dec << (int)instr.rd << " = 0x" << std::hex
                    << m_hart.getReg(instr.rd) << '\n';
        }
      }
      m_instCounter++;
    }
    //
  }
  std::cout << "Regs: " << std::endl;
  std::cout << RegfileStr(m_hart) << std::endl;
}

void Driver::dumpRegFile(std::ostream &outs) const {
  outs << m_hart.pc;
  for (reg_id_t reg = 0; reg < RegNum; ++reg) {
    outs << "x" << reg << " : " << m_hart.getReg(reg) << "\n";
  }
}

std::vector<Instruction> Driver::lookupBB(paddress_t addr) {
  if (!m_nativeBBChache.count(addr)) {
    Instruction instr;
    paddress_t cur_addr = addr;
    std::vector<Instruction> res;
    do {
      word_t cur_word = m_hart.mmu().read<word_t>(cur_addr);
      instr = m_decoder.decode(cur_word);
      res.push_back(instr);
      cur_addr += 4;
    } while (!is_terminate(instr.id));

    m_nativeBBChache.emplace(addr, res);
  }
  return m_nativeBBChache.at(addr);
}

bool Driver::is_terminate(InstrId id) {
  switch (id) {
  case InstrId::BEQ:
  case InstrId::BGE:
  case InstrId::BGEU:
  case InstrId::BLT:
  case InstrId::BLTU:
  case InstrId::BNE:
  case InstrId::EBREAK:
  case InstrId::ECALL:
  case InstrId::JAL:
  case InstrId::JALR:
    return true;
    break;
  default:
    // Disabling warning
    return false;
    break;
  }
  return false;
}

} // namespace sim