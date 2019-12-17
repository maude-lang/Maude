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
#include "counterSymbol.hh"

CounterSymbol::CounterSymbol(int id)
  : NumberOpSymbol(id, 0)
{
  currentValue = 0;
}

bool
CounterSymbol::attachData(const Vector<Sort*>& opDeclaration,
			  const char* purpose,
			  const Vector<const char*>& data)
{
  if (strcmp(purpose, "CounterSymbol") == 0)
    return true;
  return NumberOpSymbol::attachData(opDeclaration, purpose, data);
}

void
CounterSymbol::getDataAttachments(const Vector<Sort*>& opDeclaration,
				  Vector<const char*>& purposes,
				  Vector<Vector<const char*> >& data)
{
  int nrDataAttachments = purposes.length();
  purposes.resize(nrDataAttachments + 1);
  purposes[nrDataAttachments] = "CounterSymbol";
  data.resize(nrDataAttachments + 1);
  NumberOpSymbol::getDataAttachments(opDeclaration, purposes, data);
}

void
CounterSymbol::resetRules()
{
  currentValue = 0;
  NumberOpSymbol::resetRules();
}

/*
void
CounterSymbol::saveHiddenState()
{
  currentValueStack.push(currentValue);
  NumberOpSymbol::saveHiddenState();
}

void
CounterSymbol::restoreHiddenState()
{
  currentValue = currentValueStack.top();
  currentValueStack.pop();
  NumberOpSymbol::restoreHiddenState();
}
*/

bool
CounterSymbol::eqRewrite(DagNode* subject, RewritingContext& context)
{
  //
  //	NumberOpSymbol doesn't know how to deal with this.
  //
  return FreeSymbol::eqRewrite(subject, context);
}

DagNode* 
CounterSymbol::ruleRewrite(DagNode* subject, RewritingContext& context)
{
  if (SuccSymbol* succSymbol = getSuccSymbol())
    {
      if (RewritingContext::getTraceStatus())
	{
	  context.tracePreRuleRewrite(subject, 0);
	  if (context.traceAbort())
	    return 0;
	}
      DagNode* r = succSymbol->makeNatDag(currentValue);
      ++currentValue;
      if (RewritingContext::getTraceStatus())
	context.tracePostRuleRewrite(r);
      context.incrementRlCount();
      return r;
    }
  return NumberOpSymbol::ruleRewrite(subject, context);
}
