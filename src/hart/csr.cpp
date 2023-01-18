#include "hart/csr.hpp"
#include <assert.h>

namespace sim {

CSRRegfile::CSRRegfile() {
  // Fill CSRs according to simulated architecture

  // MISA
  csr_val_t mxl = 1 << (MXLEN - 2);
  // I M Z, shift is index in alphabet
  csr_val_t extensions = (1 << 8) | (1 << 12) | (1 << 25);
  setReg(CSR_MISA, 0 | mxl | extensions);

  // MSTATUS
  
  // MVENDORID

  // MARCHID

  // MIMPID

  // MHARTID
}

}; // namespace sim