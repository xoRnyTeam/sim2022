#ifndef DRIVER_HPP
#define DRIVER_HPP

#include "decoder/decoder.hpp"
#include "executor/executor.hpp"
#include "hart/hart.hpp"
#include "jit/rvjit.hpp"
#include "loader/loader.hpp"
#include "memory/memory.hpp"
#include "support.hpp"

#include <fstream>

namespace sim {

/**
 * @brief
 *
 */
class Driver final {
  //
  Hart m_hart;
  Executor m_executor{};
  Decoder m_decoder{};
  //
  // optimizations
  // bt
  RVJit m_jit;
  // <function, translated instructions>
  std::unordered_map<paddress_t, std::pair<RVJit::FunctionTy, size_t>>
      m_translated;
  // cache line block
  std::unordered_map<paddress_t, std::vector<Instruction>> m_nativeBBChache;
  // TODO: impl log class
  std::ofstream trace_out;
  //
  // statistic
  size_t m_instCounter = 0;
  //
public:
  Driver(const std::string &path, const std::string &path_trace = {});
  //
  Driver(const Driver &) = delete;
  Driver(Driver &&) = delete;
  Driver &operator=(const Driver &) = delete;
  Driver &operator=(Driver &&) = delete;
  //
  void run();
  //
  size_t getInstCounter() const { return m_instCounter; }

private:
  //
  paddress_t &pc() { return m_hart.pc; }
  bool is_terminate() { return m_hart.terminate; }
  void dumpRegFile(std::ostream &outs) const;
  //
  std::vector<Instruction> lookupBB(paddress_t addr);
  static bool is_terminate(InstrId id);
};
} // namespace sim

#endif //! DRIVER_HPP