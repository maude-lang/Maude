/*

    This file is part of the Maude 2 interpreter.

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
//      Implementation for class SortTestSymbol.
//
 
//	utility stuff
#include "macros.hh"
#include "vector.hh"

//      forward declarations
#include "interface.hh"
#include "core.hh"
#include "freeTheory.hh"

//      interface class definitions
#include "symbol.hh"
#include "dagNode.hh"
#include "term.hh"
 
//      core class definitions
#include "argumentIterator.hh"
#include "rewritingContext.hh"

//      free theory class definitions
#include "freeDagNode.hh"
 
//      built in class definitions
#include "sortTestSymbol.hh"

const Vector<int>&
SortTestSymbol::makeLazyStrategy()
{
  static Vector<int> lazyStrategy;
  if (lazyStrategy.length() == 0)
    lazyStrategy.append(0);
  return lazyStrategy;
}

SortTestSymbol::SortTestSymbol(int id,
			       Sort* testSort,
			       FreeSymbol* leq,
			       FreeSymbol* nleq,
			       bool eager)
  : FreeSymbol(id, 1, eager ? standard : makeLazyStrategy())
{
  cmpSort = testSort;
  leqResult = leq;
  notLeqResult = nleq;
  eagerFlag = eager;
}

bool
SortTestSymbol::eqRewrite(DagNode* subject, RewritingContext& context)
{
  Assert(this == subject->symbol(), "bad symbol");
  DagNode *t = static_cast<FreeDagNode*>(subject)->getArgument(0);
  if (eagerFlag)
    t->reduce(context);
  else
    t->computeTrueSort(context);
  t->symbol()->constrainToExactSort(t, context);  // HACK
  FreeSymbol* result = (t->leq(cmpSort)) ? leqResult : notLeqResult;
  bool trace = RewritingContext::getTraceStatus();
  if (trace)
    {
      context.tracePreEqRewrite(subject, 0, RewritingContext::BUILTIN);
      if (context.traceAbort())
	return false;
    }
  (void) new(subject) FreeDagNode(result);
  context.incrementEqCount();
  if (trace)
    context.tracePostEqRewrite(subject);
  return true;
}

bool
SortTestSymbol::acceptEquation(Equation* /* equation */)
{
  return false;
}

void
SortTestSymbol::compileEquations()
{
}
