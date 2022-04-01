#include "llvm/Pass.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/InstrTypes.h"
#include "llvm/IR/LegacyPassManager.h"

#include "llvm/Support/raw_ostream.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"

#include <algorithm>
#include <vector>
#include <string>

using namespace llvm;
using namespace std;

namespace {
  struct BBCounterPass : public FunctionPass  {
    static char ID;

    vector<string> bb_ids;

    int get_index(string &id);

    BBCounterPass() : FunctionPass (ID) {}
    virtual bool runOnFunction(Function &Func);
  };
}

int BBCounterPass::get_index(string &id) {
  vector<string>::iterator itr = find(bb_ids.begin(), bb_ids.end(), id);
  if (itr != bb_ids.end()) {
    return std::distance(bb_ids.begin(), itr);
  }
  else{
    bb_ids.push_back(id);
    return bb_ids.size() - 1;
  }
}

bool BBCounterPass::runOnFunction(Function &Func) {
  // Get the function to call from our runtime library.
  bool modified = false;
  LLVMContext &Ctx = Func.getContext();
  ArrayRef<Type *> paramTypes = {Type::getInt16Ty(Ctx)};
  Type *retType = Type::getVoidTy(Ctx);
  FunctionType *bbCountFuncType = FunctionType::get(retType, paramTypes, false);
  Module *Mod = Func.getParent();
  FunctionCallee countFunc = Mod->getOrInsertFunction("bbCounter", bbCountFuncType);

  for (auto &BB : Func) {
    string bb_id = Mod->getName().str() + ":" + Func.getName().str();
    bb_id = bb_id + ":" + BB.getName().str();
    uint16_t index = get_index(bb_id);
    errs() << "BB: " << bb_id << "\tN. of Inst: " << BB.size() << "\n";

    IRBuilder<> builder(&BB);
    builder.SetInsertPoint(BB.getFirstNonPHI()); // Insert before first non-PHI
    Value *args[] = {builder.getInt16(index)};
    // Value *args = builder.CreateGlobalStringPtr(bb_id.c_str());
    // Value *args[] = {builder.getInt32(100), strVal};
    builder.CreateCall(countFunc, args);
    modified |= true;
  }

  if (Func.getName() == "main") {
    BasicBlock *entryBB = &Func.getEntryBlock();
    errs() << "\n\nBB: " << entryBB->getName() << "\n";
    errs() << "Number of Basic Blocks:  " << bb_ids.size() << "\n";
    FunctionCallee initFunc = Mod->getOrInsertFunction("initCounter", bbCountFuncType);
    FunctionCallee dumpFunc = Mod->getOrInsertFunction("dumpCounts", bbCountFuncType);
    IRBuilder<> builder(entryBB);


    builder.SetInsertPoint(entryBB->getFirstNonPHI());
    Value *initargs[] = {builder.getInt16(bb_ids.size())};
    builder.CreateCall(initFunc, initargs);

    BasicBlock *exitBB = &Func.getBasicBlockList().back();
    builder.SetInsertPoint(exitBB->getTerminator());
    Value *termiargs[] = {builder.getInt16(0)};
    builder.CreateCall(dumpFunc, termiargs);
    modified |= true;
  }
  return modified;
}

char BBCounterPass::ID = 0;

static void registerBBCounterPass(const PassManagerBuilder &, legacy::PassManagerBase &PM)
{
  PM.add(new BBCounterPass());
}
static RegisterStandardPasses
    RegisterMyPass(PassManagerBuilder::EP_EarlyAsPossible,
                   registerBBCounterPass);
