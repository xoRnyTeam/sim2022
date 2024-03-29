#ifndef ELF_LOADER_H
#define ELF_LOADER_H

#include "memory/memory.hpp"
#include "support.hpp"

#include <elfio/elfio.hpp>

namespace sim {

struct ElfLoader final {

  ELFIO::elfio m_reader{};

  ElfLoader(const std::string &filepath);
  //
public:
  void load(Memory &memory);
  //
  paddress_t get_entry() const { return m_reader.get_entry(); }
};

}; // namespace sim

#endif