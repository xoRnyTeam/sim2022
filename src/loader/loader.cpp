#include "loader/loader.hpp"

namespace sim {

paddress_t ElfLoader::loadELF(Memory *memory, const std::string &filepath) {
  ELFIO::elfio reader;
  if (!reader.load(filepath)) {
    std::string msg = "Bad ELF filename.";
    msg += filepath;
    throw std::invalid_argument(msg);
  }
  // check for 32-bit
  if (reader.get_class() != ELFIO::ELFCLASS32) {
    throw std::runtime_error("Wrong ELF file class.");
  }
  // Check for little-endian
  if (reader.get_encoding() != ELFIO::ELFDATA2LSB) {
    throw std::runtime_error("Wrong ELF encoding.");
  }
  ELFIO::Elf_Half seg_num = reader.segments.size();
  for (size_t seg_i = 0; seg_i < seg_num; ++seg_i) {
    const ELFIO::segment *segment = reader.segments[seg_i];
    if (segment->get_type() != ELFIO::PT_LOAD) {
      continue;
    }
    paddress_t address = segment->get_virtual_address();
    size_t filesz = static_cast<size_t>(segment->get_file_size());
    size_t memsz = static_cast<size_t>(segment->get_memory_size());
    memory->writeRaw(
        address, reinterpret_cast<const byte_t *>(segment->get_data()), filesz);
  }
  return reader.get_entry();
}

}; // namespace sim