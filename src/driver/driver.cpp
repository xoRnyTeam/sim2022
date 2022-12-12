#include "driver/driver.hpp"

namespace sim {

Driver::Driver(const std::string &path) {
  ElfLoader loader{path};
  loader.load(m_hart.memory);
  //
  m_hart.pc = loader.get_entry();
}

void Driver::run() {
  while (!is_terminate()) {
    word_t cur_word = m_hart.memory.readWord(m_hart.pc);
    auto instr = m_decoder.decode(cur_word);
    m_executor.exec(instr, m_hart);
    //
  }
}
} // namespace sim