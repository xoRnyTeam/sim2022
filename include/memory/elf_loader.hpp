#ifndef ELF_LOADER_H
#define ELF_LOADER

#include "memory.hpp"
#include "utils.hpp"

#include <elfio/elfio.hpp>

namespace sim {

struct ElfLoader final {
  static paddress_t loadELF(Memory *memory, const std::string &filepath);
};

}; // namespace sim

#endif