#include "driver/driver.hpp"

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
  while (!is_terminate()) {
    word_t cur_word = m_hart.memory.readWord(m_hart.pc);
    auto instr = m_decoder.decode(cur_word);

    trace_out << "*************************************************************"
                 "******************"
              << std::endl;
    trace_out << std::hex << "0x" << m_hart.pc << ": " << InstrId2str[instr.id]
              << std::dec << " rd = " << instr.rd << ", rs1 = " << instr.rs1
              << ", rs2 = " << instr.rs2 << ", rs3 = " << instr.rs3 << std::hex
              << ", imm = 0x" << instr.imm << std::dec << std::endl;

    m_executor.exec(instr, m_hart);
    //
  }
}

void Driver::dumpRegFile(std::ostream &outs) const {
  outs << m_hart.pc;
  for (reg_id_t reg = 0; reg < RegNum; ++reg) {
    outs << "x" << reg << " : " << m_hart.getReg(reg) << "\n";
  }
}

} // namespace sim