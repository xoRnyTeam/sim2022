#ifndef EXEC_HPP
#define EXEC_HPP

#include "isa/instr.hpp"
#include "hart/hart.hpp"
//
#include <functional>
#include <unordered_map>
//

namespace sim {

class Executor final {
  //
  std::unordered_map<InstrId, std::function<void(Instruction &instr, Hart& hart)>>
      m_exec_instr{};
  //
public:
  Executor();
  Executor(const Executor &) = delete;
  Executor(Executor &&) = delete;
  Executor &operator=(const Executor &) = delete;
  Executor &operator=(Executor &&) = delete;
  //
  void exec(Instruction &instr, Hart &hart) const { m_exec_instr.at(instr.id)(instr, hart); }
};

} // namespace sim

#endif //! EXEC_HPP