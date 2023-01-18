#include "driver/driver.hpp"
#include "utils/compiler.hpp"

namespace sim {

Driver::Driver(const std::string &path, const std::string &path_trace) {
  ElfLoader loader{path};
  loader.load(m_hart.memory);
  //
  trace_out.open(path_trace);
  if (trace_out.is_open()) {
    m_hart.trace_out = &(trace_out);
  }
  //
  m_hart.pc = loader.get_entry();
}

void Driver::run() {
  m_instCounter = 0;

  while (!is_terminate()) {
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
          trace_out << "x" << instr.rd << " = 0x" << std::hex
                    << m_hart.getReg(instr.rd) << '\n';
        }
      }
      m_instCounter++;
    }
    //
  }
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
      word_t cur_word = m_hart.memory.readWord(cur_addr);
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
  // case InstrId::PAUSE:
  // case InstrId::SBREAK:
  // case InstrId::SCALL:
    return true;
    break;
  }
  return false;
}

} // namespace sim