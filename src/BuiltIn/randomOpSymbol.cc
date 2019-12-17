/*

    This file is part of the Maude 3 interpreter.

    Copyright 2004 SRI International, Menlo Park, CA 94025, USA.

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
//      Implementation for class NumberOpSymbol.
//

//      utility stuff
#include "macros.hh"
#include "vector.hh"

//      forward declarations
#include "interface.hh"
#include "core.hh"
#include "freeTheory.hh"
#include "builtIn.hh"

//      interface class definitions
#include "term.hh"

//      core class definitions
#include "rewritingContext.hh"

//      free theory class definitions
#include "freeDagNode.hh"

//      built in stuff
#include "succSymbol.hh"
#include "randomOpSymbol.hh"

MTRand::uint32 RandomOpSymbol::globalSeed = 0;

RandomOpSymbol::RandomOpSymbol(int id)
  : NumberOpSymbol(id, 1),
    currentIndex(0),
    currentState(globalSeed)
{
  randomNumber = currentState.randInt();
}

bool
RandomOpSymbol::attachData(const Vector<Sort*>& opDeclaration,
			   const char* purpose,
			   const Vector<const char*>& data)
{
  if (strcmp(purpose, "RandomOpSymbol") == 0)
    return true;
  return NumberOpSymbol::attachData(opDeclaration, purpose, data);
}

void
RandomOpSymbol::getDataAttachments(const Vector<Sort*>& opDeclaration,
				   Vector<const char*>& purposes,
				   Vector<Vector<const char*> >& data)
{
  int nrDataAttachments = purposes.length();
  purposes.resize(nrDataAttachments + 1);
  purposes[nrDataAttachments] = "RandomOpSymbol";
  data.resize(nrDataAttachments + 1);
  NumberOpSymbol::getDataAttachments(opDeclaration, purposes, data);
}

bool
RandomOpSymbol::eqRewrite(DagNode* subject, RewritingContext& context)
{
  FreeDagNode* d = safeCast(FreeDagNode*, subject);
  DagNode* a = d->getArgument(0);
  a->reduce(context);

  SuccSymbol* succSymbol = getSuccSymbol();
  if (succSymbol != 0 && succSymbol->isNat(a))
    {
      const mpz_class& wantedIndex = succSymbol->getNat(a);
      DebugAdvisory("currentIndex = " << currentIndex << "  wantedIndex = " << wantedIndex);
      if (wantedIndex < currentIndex)
	{
	  currentIndex = 0;
	  currentState.seed(globalSeed);
	  randomNumber = currentState.randInt();
	}
      while (currentIndex < wantedIndex)
	{
	  ++currentIndex;
	  randomNumber = currentState.randInt();
	}
      return succSymbol->rewriteToNat(subject, context, randomNumber);
    }
  //
  //	NumberOpSymbol doesn't know how to deal with this.
  //
  return FreeSymbol::eqRewrite(subject, context);
}
