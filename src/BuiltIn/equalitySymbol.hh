/*

    This file is part of the Maude 3 interpreter.

    Copyright 1997-2003 SRI International, Menlo Park, CA 94025, USA.

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
//      Class for symbols for built in equality operations.
//
#ifndef _equalitySymbol_hh_
#define _equalitySymbol_hh_
#include "freeSymbol.hh"
#include "cachedDag.hh"
#include "fullCompiler.hh"

class EqualitySymbol : public FreeSymbol
{
public:
  EqualitySymbol(int id, const Vector<int>& strategy);

  bool attachData(const Vector<Sort*>& opDeclaration,
		  const char* purpose,
		  const Vector<const char*>& data);
  bool attachTerm(const char* purpose, Term* term);
  void copyAttachments(Symbol* original, SymbolMap* map);
  void getDataAttachments(const Vector<Sort*>& opDeclaration,
			  Vector<const char*>& purposes,
			  Vector<Vector<const char*> >& data);
  void getTermAttachments(Vector<const char*>& purposes,
			  Vector<Term*>& terms);

  void postInterSymbolPass();
  void reset();
  bool eqRewrite(DagNode* subject, RewritingContext& context);
  //
  //	We don't accept or compile any equations.
  //
  bool acceptEquation(Equation* equation);
  void compileEquations();
  bool domainSortAlwaysLeqThan(Sort* sort, int argNr);
  //
  //	MVM stuff.
  //
  Instruction* generateFinalInstruction(const Vector<int>& argumentSlots);
  Instruction* generateInstruction(int destination, const Vector<int>& argumentSlots, Instruction* nextInstruction);
  void stackMachineCompile();
  Instruction* getEqualInstructionSequence() const;
  Instruction* getNotEqualInstructionSequence() const;

#ifdef COMPILER
  void generateCode(CompilationContext& context) const;
#endif

private:
  CachedDag equalTerm;
  CachedDag notEqualTerm;
};

inline void
EqualitySymbol::stackMachineCompile()
{
  if (equalTerm.getInstructionSequence() == 0)
    {
      equalTerm.generateInstructionSequence();
      notEqualTerm.generateInstructionSequence();
    }
}

inline Instruction*
EqualitySymbol::getEqualInstructionSequence() const
{
  return equalTerm.getInstructionSequence();
}

inline Instruction*
EqualitySymbol::getNotEqualInstructionSequence() const
{
  return notEqualTerm.getInstructionSequence();
}

#endif
