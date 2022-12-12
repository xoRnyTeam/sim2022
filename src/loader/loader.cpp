#include "loader/loader.hpp"

namespace sim {

ElfLoader::ElfLoader(const std::string &filepath) {
  if (!m_reader.load(filepath))
    throw std::invalid_argument("Bad ELF filename : " + filepath);
}

paddress_t ElfLoader::load(Memory *memory, const std::string &filepath) {
  // check for 32-bit
  if (m_reader.get_class() != ELFIO::ELFCLASS32) {
    throw std::runtime_error("Wrong ELF file class.");
  }
  // Check for little-endian
  if (m_reader.get_encoding() != ELFIO::ELFDATA2LSB) {
    throw std::runtime_error("Wrong ELF encoding.");
  }
  ELFIO::Elf_Half seg_num = m_reader.segments.size();
  //
  for (size_t seg_i = 0; seg_i < seg_num; ++seg_i) {
    const ELFIO::segment *segment = m_reader.segments[seg_i];
    if (segment->get_type() != ELFIO::PT_LOAD) {
      continue;
    }
    paddress_t address = segment->get_virtual_address();
    size_t filesz = static_cast<size_t>(segment->get_file_size());
    size_t memsz = static_cast<size_t>(segment->get_memory_size());
    memory->writeRaw(
        address, reinterpret_cast<const byte_t *>(segment->get_data()), filesz);
  }
}

}; // namespace sim