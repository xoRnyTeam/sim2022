#include "memory/mmu.hpp"
#include "hart/hart.hpp"

namespace sim {

MMU::MMU(Hart &hart) : m_hart(&hart) {
  hart.csr().setReg(CSR_SATP, 0);
  hart.csr().setReg(CSR_SATP, TranslationModes::Bare << 31);
}

paddress_t MMU::translate(vaddress_t address) {
  paddress_t result = 0;
  switch (getTranslationMode()) {
  case Bare:
    result = translateBare(address);
    break;
  default:
    assert(0 && "Unknown translation mode");
    break;
  }
  // Address bound check.
  // It could be more complicated in case of nonlinear physical adress space.
  if (result > getPhysMemSize()) {
    std::cerr << "Illegal phys. memory access at " << result << "\n";
    abort();
  }
  return result;
}

MMU::TranslationModes MMU::getTranslationMode() const {
  return static_cast<MMU::TranslationModes>(
      slice<31, 31>(m_hart->csr().getReg(CSR_SATP)));
}

size_t MMU::getPhysMemSize() const {
    return Memory::MEM_SIZE;
}

} // namespace sim