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
//	Implementation for class Rule.
//

//	utility stuff
#include "macros.hh"
#include "vector.hh"

//	forward declarations
#include "interface.hh"
#include "core.hh"
#include "variable.hh"

//	interface class definitions
//#include "symbol.hh"
//#include "dagNode.hh"
#include "term.hh"
#include "extensionInfo.hh"
#include "lhsAutomaton.hh"
#include "rhsAutomaton.hh"
#include "subproblem.hh"

//	core class definitions
#include "rewritingContext.hh"
#include "termBag.hh"
#include "rule.hh"

Rule::Rule(int label, Term* lhs, Term* rhs, const Vector<ConditionFragment*>& condition)
  : PreEquation(label, lhs, condition),
    rhs(rhs)
{
  Assert(rhs != 0, "null rhs");
  nonExtLhsAutomaton = 0;
  extLhsAutomaton = 0;
}

Rule::~Rule()
{
  delete nonExtLhsAutomaton;
  delete extLhsAutomaton;
  rhs->deepSelfDestruct();
}

void
Rule::check()
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
		   " is used before it is bound in rule:\n" <<
		   this);
      //markAsBad();
      setNonexec();
    }
}

void
Rule::preprocess()
{
  PreEquation::preprocess();
  rhs->symbol()->fillInSortInfo(rhs);
  Assert(getLhs()->getComponent() == rhs->getComponent(), "connected component clash");
}

LhsAutomaton*
Rule::getNonExtLhsAutomaton()
{
  if (nonExtLhsAutomaton == 0)
    {
      NatSet boundUniquely;
      bool subproblemLikely;
      nonExtLhsAutomaton = getLhs()->compileLhs(false, *this, boundUniquely, subproblemLikely);
    }
  return nonExtLhsAutomaton;
}

LhsAutomaton*
Rule::getExtLhsAutomaton()
{
  if (extLhsAutomaton == 0)
    {
      NatSet boundUniquely;
      bool subproblemLikely;
      extLhsAutomaton = getLhs()->compileLhs(true, *this, boundUniquely, subproblemLikely);
    }
  return extLhsAutomaton;
}

void
Rule::compile(bool compileLhs)
{
  if (isCompiled())
    return;
  setCompiled();
  TermBag availableTerms;  // terms available for reuse
  //
  //	Since rules can be applied in non-eager subterms, if we have
  //	a condition we must consider all variables to be non-eager
  //	to avoid having a condition reduce a lazy subterm.
  //
  compileBuild(availableTerms, !hasCondition());
  //
  //	HACK: we pessimize the compilation of unconditional rules to avoid
  //	left->right subterm sharing that would break narrowing.
  //
  if (!hasCondition())
    {
      TermBag dummy;
      rhs->compileTopRhs(builder, *this, dummy);
    }
  else
    rhs->compileTopRhs(builder, *this, availableTerms);  // original code

  //  builder.dump(cout, *this);
  compileMatch(compileLhs, true);
  builder.remapIndices(*this);
  //
  //	Make all variables in a rules lhs into condition variables so that
  //	if we compile lhs again in getNonExtLhsAutomaton() or getExtLhsAutomaton()
  //	it will be compiled to generate all matchers rather than just those
  //	that differ on variables in the condition.
  //
  addConditionVariables(getLhs()->occursBelow());
}

int
Rule::traceBeginTrial(DagNode* subject, RewritingContext& context) const
{
  return context.traceBeginRuleTrial(subject, this);
}

void
Rule::print(ostream& s) const
{
  s << this;
}
