#ifndef DRIVER_HPP
#define DRIVER_HPP

#include "decoder/decoder.hpp"
#include "executor/executor.hpp"
#include "hart/hart.hpp"
#include "loader/loader.hpp"
#include "memory/memory.hpp"
#include "support.hpp"

#include <fstream>

namespace sim {

struct Line final {
  std::vector<Instruction> instr;
  std::vector<paddress_t> address;
  Line(std::vector<paddress_t>& add, std::vector<Instruction>& inst);
  Line();
  Line(paddress_t add, Instruction inst);
  void push(paddress_t addr, Instruction inst);
  int count(paddress_t addr) const;
};

/**
 * @brief
 *
 */
class Driver final {
  //
  Hart m_hart{};
  Executor m_executor{};
  Decoder m_decoder{};
  //
  bool BBCache = true;
  std::unordered_map<paddress_t, std::vector<Instruction>> m_nativeBBChache;
  uint32_t sizeLine = 4; //number instructions in line
  uint32_t segment = 32; //number line in the cache
  uint32_t maskTag = 0b111100000;
  uint32_t maskOff = 0b01111;
  std::vector<Line> Cache;

  // TODO: impl log class
  std::ofstream trace_out;
  //
  // statistic
  size_t m_instCounter = 0;
  size_t m_hitCache = 0;
  //
public:
  Driver(const std::string &path, const std::string &path_trace = {}, const uint32_t size = 0);
  //
  Driver(const Driver &) = delete;
  Driver(Driver &&) = delete;
  Driver &operator=(const Driver &) = delete;
  Driver &operator=(Driver &&) = delete;
  //
  void run();
  void runCache();
  //
  size_t getInstCounter() const { return m_instCounter; }
  bool getBBCache() const;
  void printCoverageCache() const;

private:
  //
  paddress_t &pc() { return m_hart.pc; }
  bool is_terminate() { return m_hart.terminate; }
  void dumpRegFile(std::ostream &outs) const;
  //
  uint32_t getTag(paddress_t addres) const;
  void choiceCache(uint32_t size);
  std::vector<Instruction> lookupBB(paddress_t addr);
  Line lookupDM(paddress_t addr);
  static bool is_terminate(InstrId id);
};
} // namespace sim

#endif //! DRIVER_HPP