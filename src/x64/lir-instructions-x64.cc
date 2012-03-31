#include "lir-instructions-x64.h"
#include "hir.h"
#include "lir.h"
#include "macroassembler.h"

namespace candor {
namespace internal {

#define __ masm()->

void LIRParallelMove::Generate() {
}


void LIREntry::Generate() {
  __ push(rbp);
  __ movq(rbp, rsp);
}


void LIRReturn::Generate() {
  if (inputs[0]->is_immediate()) {
    __ movq(rax, Immediate(inputs[0]->value()));
  }

  __ movq(rsp, rbp);
  __ pop(rbp);
  __ ret(0);
}


void LIRGoto::Generate() {
}


void LIRStoreLocal::Generate() {
}


void LIRStoreContext::Generate() {
}


void LIRStoreProperty::Generate() {
}


void LIRLoadRoot::Generate() {
}


void LIRLoadLocal::Generate() {
}


void LIRLoadContext::Generate() {
}


void LIRBranchBool::Generate() {
}


void LIRAllocateObject::Generate() {
}

} // namespace internal
} // namespace candor
