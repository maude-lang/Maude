/*

    This file is part of the Maude 3 interpreter.

    Copyright 1997-2013 SRI International, Menlo Park, CA 94025, USA.

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA.

*/

//
//	Implementation for class StackMachineRhsCompiler
//

//	utility stuff
#include "macros.hh"
#include "vector.hh"
#include "indent.hh"

//	forward declarations
#include "interface.hh"
#include "core.hh"

//	interface class definitions
#include "symbol.hh"

//	core class definitions
#include "variableInfo.hh"
#include "returnInstruction.hh"
#include "stackMachineRhsCompiler.hh"

void
StackMachineRhsCompiler::recordFunctionEval(Symbol* symbol, int destination, const Vector<int>& argumentSlots)
{
  int nrFunctionEvaluations = functionEvaluations.size();
  functionEvaluations.expandBy(1);
  FunctionEval& f = functionEvaluations[nrFunctionEvaluations];

  f.symbol = symbol;
  f.destination = destination;
  f.argumentSlots = argumentSlots;  // deep copy
}

Instruction*
StackMachineRhsCompiler::compileInstructionSequence()
{
  //
  //	Deal with degenerate case of bare variable.
  //
  FunctionEval& first = functionEvaluations[0];
  if (first.symbol == 0)
    {
      NatSet activeSlots;
      int source = first.argumentSlots[0];
      activeSlots.insert(source);
      Instruction* nextInstruction = new ReturnInstruction(source);
      nextInstruction->setActiveSlots(activeSlots);
      return nextInstruction;
    }

  NatSet activeSlots;
  Instruction* nextInstruction = 0;
  int nrFunctionEvaluations = functionEvaluations.size();
  for (int i = nrFunctionEvaluations - 1; i >= 0; --i)
    {
      FunctionEval& f = functionEvaluations[i];
      Assert(f.symbol != 0, "null f.symbol when nrFunctionEvaluations = " << nrFunctionEvaluations);

      activeSlots.subtract(f.destination);
      for (int i : f.argumentSlots)
	activeSlots.insert(i);
	    
      Instruction* newInstruction = (nextInstruction == 0) ? 
	f.symbol->generateFinalInstruction(f.argumentSlots) :
	f.symbol->generateInstruction(f.destination, f.argumentSlots, nextInstruction);
      if (newInstruction == 0)
	{
	  //
	  //	Didn't generate an instruction (maybe an unimplemented case).
	  //	Warn, clean up and bail.
	  //
	  IssueWarning("stack machine compilation not supported for " << f.symbol);
	  delete nextInstruction;
	  return 0;
	}
      newInstruction->setActiveSlots(activeSlots);
      nextInstruction = newInstruction;
    }

  return nextInstruction;
}

void
StackMachineRhsCompiler::dump(ostream& s, const VariableInfo& variableInfo, int indentLevel)
{
  s << Indent(indentLevel) << "Begin{StackMachineRhsCompiler}\n";
  for (FunctionEval& i : functionEvaluations)
    {
      s << Indent(indentLevel + 1);
      if (i.symbol == 0)
	s << "(return)\t";
      else
	s << i.symbol << "\t";
      s << "destination = " << i.destination << "\targs = ";
      for (int k : i.argumentSlots)
	s << k << ' ';
    }
  s << Indent(indentLevel) << "End{StackMachineRhsCompiler}\n";
}
