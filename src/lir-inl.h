#ifndef _SRC_LIR_INL_H_
#define _SRC_LIR_INL_H_

#include "hir.h"

#if CANDOR_ARCH_x64
#include "x64/lir-instructions-x64.h"
#elif CANDOR_ARCH_ia32
#include "ia32/lir-instructions-ia32.h"
#endif

#include <sys/types.h> // off_t

namespace candor {
namespace internal {

inline LIROperand* LIR::GetSpill() {
  int spill_index;
  if (spills()->IsEmpty()) {
    spill_index = get_new_spill();
  } else {
    spill_index = spills()->Get();
  }

  return new LIROperand(LIROperand::kSpill, spill_index);
}


inline LIRInstruction* LIR::Cast(HIRInstruction* instr) {
#define LIR_GEN_CAST(V)\
  case HIRInstruction::k##V:\
    result = new LIR##V();\
    result->hir(instr);\
    result->id(instr->id());\
    break;

  LIRInstruction* result = NULL;
  switch (instr->type()) {
   LIR_ENUM_INSTRUCTIONS(LIR_GEN_CAST)
   default: UNEXPECTED break;
  }

  return result;
#undef LIR_GEN_CAST
}


} // namespace internal
} // namespace candor

#endif // _SRC_LIR_INL_H_