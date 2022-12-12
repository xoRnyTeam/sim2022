#ifndef DECODE_HPP
#define DECODE_HPP

#include "isa/bits.hpp"
#include "isa/instr.hpp"

namespace sim {

class Decoder final {

public:
  Instruction decode(word_t word);
};

} // namespace sim

#endif //! DECODE_HPP