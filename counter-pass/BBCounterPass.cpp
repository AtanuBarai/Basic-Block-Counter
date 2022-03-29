#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/InstrTypes.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"
#include "llvm/IR/Module.h"

using namespace llvm;

namespace {
  struct BBCounterPass : public FunctionPass  {
    static char ID;
    BBCounterPass() : FunctionPass (ID) {}
    virtual bool runOnFunction(Function &Func);
  };
}

bool BBCounterPass::runOnFunction(Function &Func) {
  // Get the function to call from our runtime library.
  bool modified = false;
  LLVMContext &Ctx = Func.getContext();
  ArrayRef<Type *> paramTypes = {Type::getInt8PtrTy(Ctx)};
  Type *retType = Type::getVoidTy(Ctx);
  FunctionType *bbCountFuncType = FunctionType::get(retType, paramTypes, false);
  Module *Mod = Func.getParent();
  FunctionCallee countFunc = Mod->getOrInsertFunction("bbCounter", bbCountFuncType);

  for (auto &BB : Func) {
    std::string bb_id = Mod->getName().str() + ":" + Func.getName().str();
    bb_id = bb_id + ":" + BB.getName().str();
    errs() << "BB: " << bb_id << "\tN. of Inst: " << BB.size() << "\n";

    IRBuilder<> builder(&BB);
    builder.SetInsertPoint(BB.getFirstNonPHI()); // Insert before first non-PHI

    Value *args = builder.CreateGlobalStringPtr(bb_id.c_str());
    // Value *args[] = {builder.getInt32(100), strVal};
    // Insert a call to our bbCounter function.
    builder.CreateCall(countFunc, args);
    modified |= true;
  }
  return modified;
}

char BBCounterPass::ID = 0;

// Automatically enable the pass.
// http://adriansampson.net/blog/clangpass.html
static void registerBBCounterPass(const PassManagerBuilder &, legacy::PassManagerBase &PM)
{
  PM.add(new BBCounterPass());
}
static RegisterStandardPasses
    RegisterMyPass(PassManagerBuilder::EP_EarlyAsPossible,
                   registerBBCounterPass);
