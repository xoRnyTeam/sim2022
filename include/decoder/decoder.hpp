#ifndef DECODE_HPP
#define DECODE_HPP

#include "isa/bits.hpp"
#include "isa/instr.hpp"

namespace sim {

class Decoder final {

public:
  Instruction decode(word_t word);
  //
  Decoder();
  Decoder(const Decoder &) = delete;
  Decoder(Decoder &&) = delete;
  Decoder &operator=(const Decoder &) = delete;
  Decoder &operator=(Decoder &&) = delete;
};

} // namespace sim

#endif //! DECODE_HPP