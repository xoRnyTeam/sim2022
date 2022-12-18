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
  // TODO: impl log class
  std::ofstream trace_out;
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

private:
  //
  paddress_t &pc() { return m_hart.pc; }
  bool is_terminate() { return m_hart.terminate; }
  void dumpRegFile(std::ostream &outs) const;
  //
};
} // namespace sim

#endif //! DRIVER_HPP