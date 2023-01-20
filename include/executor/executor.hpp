#ifndef EXEC_HPP
#define EXEC_HPP

#include "hart/hart.hpp"
#include "isa/instr.hpp"
//
#include <array>
#include <functional>
#include <unordered_map>
//

namespace sim {

struct CallInfo final {
  void (*exec)(CallInfo *) = nullptr;
  const Instruction *instr = nullptr;
  Hart *hart = nullptr;
};

class Executor final {
public:
  //
  std::array<void(*)(CallInfo *), 0xFF>
      m_exec_instr{};
  //
  Executor();
  Executor(const Executor &) = delete;
  Executor(Executor &&) = delete;
  Executor &operator=(const Executor &) = delete;
  Executor &operator=(Executor &&) = delete;
  //
  void exec(const Instruction &instr, Hart &hart) const;
};

} // namespace sim

#endif //! EXEC_HPP