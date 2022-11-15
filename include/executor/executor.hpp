#ifndef EXEC_HPP
#define EXEC_HPP

#include "isa/instr.hpp"
//
#include <functional>
#include <unordered_map>
//

namespace sim {

class Executor final {
  //
  std::unordered_map<InstrId, std::function<void(Instruction &instr)>>
      m_exec_instr{};
  //
public:
  Executor();
  Executor(const Executor &) = delete;
  Executor(Executor &&) = delete;
  Executor &operator=(const Executor &) = delete;
  Executor &operator=(Executor &&) = delete;
  //
  void exec(Instruction &instr) const;
};

} // namespace sim

#endif //! EXEC_HPP