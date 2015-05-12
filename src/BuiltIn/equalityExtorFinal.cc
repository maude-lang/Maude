//	utility stuff
#include "macros.hh"
#include "vector.hh"
#include "indent.hh"

//	forward declarations
#include "interface.hh"
#include "core.hh"
#include "variable.hh"
#include "freeTheory.hh"

//      core class definitions
#include "frame.hh"
#include "stackMachine.hh"

//      interface class definitions
#include "symbol.hh"
#include "dagNode.hh"
#include "term.hh"

//	free theory class definitions
#include "equalitySymbol.hh"
#include "equalityExtorFinal.hh"

EqualityExtorFinal::EqualityExtorFinal(EqualitySymbol* symbol,  int argIndex0, int argIndex1)
  : symbol(symbol),
    argIndex0(argIndex0),
    argIndex1(argIndex1)
{
  //
  //	Since EqualitySymbols are generated as needed and inserted as late symbols, we compile on demand.
  //	It is safe to call stackMachineCompile() multiple times on the same EqualitySymbol.
  //
  symbol->stackMachineCompile();
}

void
EqualityExtorFinal::execute(StackMachine* machine) const
{
  //
  //    Get the stack frame containing the pointer to us.
  //
  Frame* frame = machine->getTopFrame();
  //
  //	Check equality of arguments.
  //
  bool equal = frame->getSlot(argIndex0)->equal(frame->getSlot(argIndex1));
  //
  //	Run approriate instruction sequence.
  //
  Instruction* ni = equal ? symbol->getEqualInstructionSequence() : symbol->getNotEqualInstructionSequence();
  machine->incrementEqCount();
  ni->execute(machine);  // tail call (should we worry about collecting garbage?)
}
