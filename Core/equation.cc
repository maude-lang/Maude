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
//	Implementation for class Equation.
//

//	utility stuff
#include "macros.hh"
#include "vector.hh"

//	forward declarations
#include "interface.hh"
#include "core.hh"

//	interface class definitions
//#include "symbol.hh"
//#include "dagNode.hh"
#include "term.hh"
#include "rhsAutomaton.hh"

//	core class definitions
#include "rewritingContext.hh"
#include "termBag.hh"
#include "equation.hh"

Equation::Equation(int label,
		   Term* lhs,
		   Term* rhs,
		   bool owise,
		   const Vector<ConditionFragment*>& condition)
  : PreEquation(label, lhs, condition),
    rhs(rhs)
{
  if (owise)
    setFlags(OWISE);
  Assert(rhs != 0, "null rhs");
}

Equation::~Equation()
{
  rhs->deepSelfDestruct();
}

void
Equation::check()
{
  NatSet boundVariables;
  PreEquation::check(boundVariables);
  rhs = rhs->normalize(false);
  rhs->indexVariables(*this);

  NatSet unboundVariables(rhs->occursBelow());
  unboundVariables.subtract(boundVariables);
  addUnboundVariables(unboundVariables);
  if (!isNonexec() && !getUnboundVariables().empty())
    {
      IssueWarning(*this << ": variable " <<
		   QUOTE(index2Variable(getUnboundVariables().min())) <<
		   " is used before it is bound in equation:\n" <<
		   this);
      markAsBad();
    }
}

void
Equation::preprocess()
{
  PreEquation::preprocess();
  rhs->symbol()->fillInSortInfo(rhs);
  Assert(getLhs()->getComponent() == rhs->getComponent(),
	 "connected component clash");
}

void
Equation::compile(bool compileLhs)
{
  if (isCompiled())
    return;
  setCompiled();
  TermBag availableTerms;  // terms available for reuse
  compileBuild(availableTerms, true);
  rhs->compileTopRhs(builder, *this, availableTerms);
  compileMatch(compileLhs, true);
  //builder.dump(cerr, *this);
  builder.remapIndices(*this);
  //builder.dump(cerr, *this);

  fast = hasCondition() ? DEFAULT : getNrProtectedVariables();  // HACK
}

int
Equation::traceBeginTrial(DagNode* subject, RewritingContext& context) const
{
  return context.traceBeginEqTrial(subject, this);
}
