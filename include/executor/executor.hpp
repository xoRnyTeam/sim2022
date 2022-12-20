#ifndef EXEC_HPP
#define EXEC_HPP

#include "isa/instr.hpp"
#include "hart/hart.hpp"
//
#include <array>
#include <functional>
#include <unordered_map>
//

namespace sim {

class Executor final {
  //
  std::array<std::function<void(Instruction &instr, Hart &hart)>, 0xFF>
      m_exec_instr{};
  //
public:
  Executor();
  Executor(const Executor &) = delete;
  Executor(Executor &&) = delete;
  Executor &operator=(const Executor &) = delete;
  Executor &operator=(Executor &&) = delete;
  //
  void exec(Instruction &instr, Hart &hart) const;
};

} // namespace sim

#endif //! EXEC_HPP