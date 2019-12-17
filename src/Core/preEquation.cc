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
//	Implementation for class PreEquation.
//

//	utility stuff
#include "macros.hh"
#include "vector.hh"
#include "indent.hh"

//	forward declarations
#include "interface.hh"
#include "core.hh"
//#include "variable.hh"

//	interface class definitions
//#include "symbol.hh"
//#include "dagNode.hh"
#include "term.hh"
#include "lhsAutomaton.hh"
#include "rhsAutomaton.hh"
#include "subproblem.hh"

//	core class definitions
#include "rewritingContext.hh"
#include "termBag.hh"
#include "conditionFragment.hh"
#include "preEquation.hh"

const Vector<ConditionFragment*> PreEquation::noCondition;

PreEquation::PreEquation(int label, Term* lhs, const Vector<ConditionFragment*>& cond)
  : label(label),
    lhs(lhs)
{
  Assert(lhs != 0, "null lhs");
  lhsAutomaton = 0;
  if (cond.length() > 0)  // insure that isNull() is true for empty condition
    condition = cond;  // semi-deep copy
}

PreEquation::~PreEquation()
{
  delete lhsAutomaton;
  lhs->deepSelfDestruct();
  int nrFragments = condition.length();
  for (int i = 0; i < nrFragments; i++)
    delete condition[i];
}

void
PreEquation::check(NatSet& boundVariables)
{
  //
  //	Normalize lhs and fill out variable info
  //
  lhs = lhs->normalize(true);
  lhs->indexVariables(*this);
  boundVariables = lhs->occursBelow();  // deep copy
  int nrFragments = condition.length();
  for (int i = 0; i < nrFragments; i++)
    condition[i]->check(*this, boundVariables);
}

void
PreEquation::preprocess()
{
  lhs->symbol()->fillInSortInfo(lhs);
  /*
  AdvisoryCheck(lhs->getSortIndex() != Sort::ERROR_SORT,
		"lhs pattern " << lhs <<
		" has different operational semantics in Maude 2.0.");
  */
  lhs->analyseCollapses();
  int nrFragments = condition.length();
  for (int i = 0; i < nrFragments; i++)
    condition[i]->preprocess();
}

void
PreEquation::compileBuild(TermBag& availableTerms, bool eagerContext)
{
  lhs->findAvailableTerms(availableTerms, eagerContext, true);
  lhs->determineContextVariables();
  lhs->insertAbstractionVariables(*this);
  int nrFragments = condition.length();
  for (int i = 0; i < nrFragments; i++)
    condition[i]->compileBuild(*this, availableTerms);
}

void
PreEquation::compileMatch(bool compileLhs, bool withExtension)
{
  //
  //	We don't assume that our module was set so we look at the module of the lhs top symbol.
  //
  lhs->symbol()->getModule()->notifySubstitutionSize(computeIndexRemapping());
  if (compileLhs)
    {
      NatSet boundUniquely;
      bool subproblemLikely;
      lhsAutomaton = lhs->compileLhs(withExtension, *this, boundUniquely, subproblemLikely);
    }
  NatSet boundUniquely(lhs->occursBelow());
  int nrFragments = condition.length();
  for (int i = 0; i < nrFragments; i++)
    condition[i]->compileMatch(*this, boundUniquely);
}

bool
PreEquation::checkCondition(DagNode* subject,
			    RewritingContext& context,
			    Subproblem* subproblem) const
{
  int trialRef =  UNDEFINED;
  Stack<ConditionState*> state;
  bool result = checkCondition(true, subject, context, subproblem, trialRef, state);
  Assert(result || state.empty(), "non-empty condition state stack");
  while (!state.empty())
    {
      delete state.top();
      state.pop();
    }
  return result;
}

bool
PreEquation::checkCondition(bool findFirst,
			    DagNode* subject,
			    RewritingContext& context,
			    Subproblem* subproblem,
			    int& trialRef,
			    Stack<ConditionState*>& state) const
{
  Assert(condition.length() != 0, "no condition");
  Assert(!findFirst || state.empty(), "non-empty condition state stack");
  if (findFirst)
    trialRef = UNDEFINED;
  do
    {
      if (RewritingContext::getTraceStatus())
	{
	  if (findFirst)
	    trialRef = traceBeginTrial(subject, context);
	  if (context.traceAbort())
	    {
	      cleanStack(state);
	      return false;  // return false since condition variables may be unbound
	    }
	}
      bool success = solveCondition(findFirst, trialRef, context, state);
      if (RewritingContext::getTraceStatus())
	{
	  if (context.traceAbort())
	    {
	      cleanStack(state);
	      return false;  // return false since condition variables may be unbound
	    }
	  if (trialRef != UNDEFINED)
	    context.traceEndTrial(trialRef, success);
	}
      if (success)
	return true;
      Assert(state.empty(), "non-empty condition state stack");
      findFirst = true;
      trialRef = UNDEFINED;
      //
      //	Condition evaluation may create nodes without doing rewrites so...
      //
      MemoryCell::okToCollectGarbage();
    }
  while (subproblem != 0 && subproblem->solve(false, context));
  if (RewritingContext::getTraceStatus() && trialRef != UNDEFINED)
    context.traceExhausted(trialRef);
  return false;
}

bool
PreEquation::solveCondition(bool findFirst,
			    int trialRef,
			    RewritingContext& solution,
			    Stack<ConditionState*>& state) const
{
  int nrFragments = condition.length();
  int i = findFirst ? 0 : nrFragments - 1;
  for(;;)
    {
      if (RewritingContext::getTraceStatus())
	{
	  if (solution.traceAbort())
	    return false;
	  solution.traceBeginFragment(trialRef, this, i, findFirst);
	}
      findFirst = condition[i]->solve(findFirst, solution, state);
      if (RewritingContext::getTraceStatus())
	{
	  if (solution.traceAbort())
	    return false;
	  solution.traceEndFragment(trialRef, this, i, findFirst);
	}
      if (findFirst)
	{
	  if (++i == nrFragments)
	    break;
	}
      else
	{
	  if (--i < 0)
	    break;
	}
    }
  return findFirst;
}

void
PreEquation::cleanStack(Stack<ConditionState*>& conditionStack)
{
  DebugAdvisoryCheck(conditionStack.empty(),
		     "cleaning condition stack because of abort");
  while (!conditionStack.empty())
    {
      delete conditionStack.top();
      conditionStack.pop();
    }
}

DagNode*
PreEquation::getLhsDag()
{
  DagNode* d = lhsDag.getNode();
  if (d == 0)
    {
      d = getLhs()->term2Dag();
      if (d->computeBaseSortForGroundSubterms(true) == DagNode::UNIMPLEMENTED)
	{
	  IssueWarning(*this << ": lefthand side of " << this <<
		       " contains function symbols with nonvariable arguments that are not supported by unification.");
	}
      lhsDag.setNode(d);
    }
  return d;
}

void
PreEquation::reset()
{
  lhsDag.setNode(0);
}

ostream&
operator<<(ostream& s, const PreEquation* pe)
{
  pe->print(s);
  return s;
}

#ifdef DUMP
void
PreEquation::dump(ostream& s, int indentLevel)
{
  s << Indent(indentLevel) << "Begin{PreEquation}\n";
  ++indentLevel;
  s << Indent(indentLevel) << "lhs = " << lhs << '\n';
  s << Indent(indentLevel) << "lhsAutomaton =";
  if (lhsAutomaton == 0)
    s << " 0\n";
  else
    {
      s << '\n';
      lhsAutomaton->dump(s, *this, indentLevel);
    }
  int nrFragments = condition.size();
  if (nrFragments > 0)
    {
      s << Indent(indentLevel) << "Begin{Condition}\n";
      for (int i = 0; i < nrFragments; ++i)
	condition[i]->dump(s, *this, indentLevel + 1);
      s << Indent(indentLevel) << "End{Condition}\n";
    }
  s << Indent(indentLevel - 1) << "End{PreEquation}\n";
}
#endif
