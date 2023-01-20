#include "driver/driver.hpp"
#include "utils/compiler.hpp"

namespace sim {

Line::Line(std::vector<paddress_t>& add, std::vector<Instruction>& inst) {
  address = add;
  instr = inst;
}

Line::Line() {
  std::vector<Instruction> t1;
  std::vector<paddress_t> t2{0};
  address = t2;
  instr = t1;
}

int Line::count(paddress_t tmp_pc) const {
  int sizeLine = 4;
  for(int i = 0; i < sizeLine; ++i) {
    if (tmp_pc == address[i]) {
      return i;
    }
  }
  return -1;
}

void Line::push(paddress_t add, Instruction inst) {
  address.push_back(add);
  instr.push_back(inst);
}

Line::Line(paddress_t add, Instruction inst) {
  address.push_back(add);
  instr.push_back(inst);
}

Driver::Driver(const std::string &path, const std::string &path_trace, const uint32_t size) {
  ElfLoader loader{path};
  choiceCache(size);
  loader.load(m_hart.mmu().getMemory());
  //
  trace_out.open(path_trace);
  if (trace_out.is_open()) {
    m_hart.trace_out = &(trace_out);
  }
  //
  m_hart.pc = loader.get_entry();
}
/*
Driver::Driver(const std::string &path, const std::string &path_trace, uint32_t size) {
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
*/

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

void Driver::runCache() {
  m_instCounter = 0;
  Cache.resize(segment);
  while (!is_terminate()) {
    auto&& t = lookupDM(m_hart.pc);
    //std::cout << "size: " << t.instr.size() << std::endl;
    int test = t.count(m_hart.pc);
    //while(test !=  -1) {
    //uint32_t inv_begin = (m_hart.pc & ~(maskOff));
    //uint32_t inv = inv_begin;
    //int test = ((m_hart.pc & maskOff) / 4);
    //while(inv == inv_begin) {
    while(test != -1) {
      paddress_t tmp_pc = m_hart.pc;
      //std::cout << "RUn: " << tmp_pc << " tag: "<< getTag(tmp_pc) << " inv: " << inv << std::endl;
    // execute instr
      //std::cout << "TEST: " << test << std::endl;
      Instruction instr = t.instr[test]; 
      m_executor.exec(instr, m_hart);
      trace_out << "*************************************************************"
                  "******************"
                << std::endl;
      trace_out << std::hex << "0x" << m_hart.pc << ": " << InstrId2str[instr.id]
                << std::dec << " rd = " << instr.rd << ", rs1 = " << instr.rs1
                << ", rs2 = " << instr.rs2 << ", rs3 = " << instr.rs3 << std::hex
                << ", imm = 0x" << instr.imm << std::dec << std::endl;

      m_instCounter++;
      if(is_terminate()) {
        break;
      }
      test = t.count(m_hart.pc);
      //inv = (m_hart.pc & ~(maskOff));
      //std::cout << "next inv: " << inv << std::endl;
      //test = ((m_hart.pc & maskOff) / 4);
      //std::cout << "TESTIK: " << test << std::endl;
    } 
  }
}

bool Driver::getBBCache() const {
  return BBCache;
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
  // case InstrId::PAUSE:
  // case InstrId::SBREAK:
  // case InstrId::SCALL:
    return true;
    break;
  default:
    // Disabling warning
    return false;
    break;
  }
  return false;
}

Line Driver::lookupDM(paddress_t addr) {
  //std::cout << "entry" << std::endl;
  uint32_t tag = getTag(addr);
  //std::cout <<"cache.size: " << Cache.size() << " tag: " << tag << std::endl;
  //std::cout << "add[0]: " << Cache[tag].address[0] << " addres: "<< addr << std::endl; 
  if ( Cache[tag].address[0] == addr ) {
    //std::cout << "HIT" << std::endl;
    m_hitCache += 4;
    return Cache[tag];
  }
  uint32_t cur_addr = addr;
  word_t cur_word = m_hart.mmu().read<word_t>(cur_addr);
  Instruction instr = m_decoder.decode(cur_word);
  Line t(cur_addr, instr);
  for(int i = 1; i < sizeLine; ++i) {
    word_t cur_word = m_hart.mmu().read<word_t>(cur_addr);
    Instruction instr = m_decoder.decode(cur_word);
    t.push(cur_addr, instr);
    cur_addr += 4;
  }
  Cache[tag] = t;
  //std::cout << "test" <<std::endl;
  return t;
} 

uint32_t Driver::getTag(paddress_t address) const {
  return (maskTag & address) >> 4;
}

void Driver::choiceCache(uint32_t size) {
  if ( size == 0 ) {
    std::cout << "Using Basic Block Cache" << std::endl;
  } else {
    BBCache = false;
    std::cout << "Using Direct-Mapped cache with size = " << size << std::endl;
  }
}

void Driver::printCoverageCache() const {
  std::cout << "Hits: " << m_hitCache << std::endl;
}

} // namespace sim