#ifndef UTILS_HPP
#define UTILS_HPP

#include <cstddef>
#include <cstdint>

namespace sim {

using byte_t = std::uint8_t;
using paddress_t = std::uint32_t;
using vaddress_t = std::uint32_t;
//
using reg_val_t = std::uint32_t;
using reg_id_t = std::uint8_t;
//
using word_t = std::uint32_t;
using hword_t = std::uint16_t;
using bword_t = std::uint8_t;
//
using op_t = std::uint8_t;
using imm_t = std::int32_t;
//
constexpr reg_id_t RegNum = 32;

}; // namespace sim

#endif