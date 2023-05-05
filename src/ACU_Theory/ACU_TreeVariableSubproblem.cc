/*

    This file is part of the Maude 3 interpreter.

    Copyright 2023 SRI International, Menlo Park, CA 94025, USA.

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
//      Implementation for class ACU_TreeVariableSubproblem.
//

//	utility stuff
#include "macros.hh"
#include "vector.hh"
#include "indent.hh"

//      forward declarations
#include "interface.hh"
#include "core.hh"
#include "ACU_RedBlack.hh"
#include "ACU_Theory.hh"

//      interface class definitions
#include "argVec.hh"
#include "associativeSymbol.hh"
#include "dagNode.hh"
#include "lhsAutomaton.hh"
#include "subproblem.hh"

//      core class definitions
#include "variableInfo.hh"
#include "substitution.hh"
#include "rewritingContext.hh"
#include "localBinding.hh"
#include "dagArgumentIterator.hh"

//	ACU theory class definitions
#include "ACU_Symbol.hh"
#include "ACU_TreeDagNode.hh"
#include "ACU_TreeVariableSubproblem.hh"

ACU_TreeVariableSubproblem::ACU_TreeVariableSubproblem(const ACU_TreeDagNode* subject,
						       const ACU_Tree& remaining,
						       int stripperVarIndex,
						       Sort* stripperSort,
						       int collectorVarIndex,
						       Sort* collectorSort)
  : subject(subject),
    remaining(remaining),
    stripperVarIndex(stripperVarIndex),
    stripperSort(stripperSort),
    collectorVarIndex(collectorVarIndex),
    collectorSort(collectorSort)
{
  Assert(remaining.getSize() != 0, "no remaining subject arguments");
  Assert(remaining.getSize() > 1 || remaining.getMaxMult() > 1, "only one subject argument remaining");
}

bool
ACU_TreeVariableSubproblem::solve(bool findFirst, RewritingContext& solution)
{
  if (findFirst)
    {
      //
      //	Record state of stripper and collector variables on entry so we can restore
      //	them on backtracking.
      //
      stripperAlreadyBound = (solution.value(stripperVarIndex) != nullptr);
      collectorAlreadyBound = (solution.value(collectorVarIndex) != nullptr);
      if (collectorAlreadyBound)
	{
	  //
	  //	Having the collector variable bound, whether or not the stripper variable
	  //	is bound is an unusual situation. It must have been bound by a sub-match
	  //	after the sub-match that created this problem, or by a subproblem
	  //	created for a sub-match before the sub-match that created this problem.
	  //	There can be at most one solution.
	  //
	  return handleBoundCollector(solution);
	}
      if (stripperAlreadyBound)
	{
	  //
	  //	The stripper variable can be bound by the same means as described above.
	  //	For some reason this is quite a common occurrence, and we need to handle
	  //	it efficiently. There can be at most one solution.
	  //
	  return handleBoundStripper(solution);
	}
      //
      //	Stripper and collect are both free - start a systematic search for subject arguments
      //	to bind stripper variable to.
      //
      currentPath.reset(remaining);
    }
  else
    {
      //
      //	Restore solution to state before last solve() by clearing any bindings we created.
      //
      if (!stripperAlreadyBound)
	solution.bind(stripperVarIndex, nullptr);
      if (!collectorAlreadyBound)
	solution.bind(collectorVarIndex, nullptr);
      //
      //	If either variable was bound before we found a first solution, this implies the
      //	solution was unique.
      //
      if (stripperAlreadyBound || collectorAlreadyBound)
	return false;
    }
  //
  //
  //	Try next alternative for the stripper variable.
  //
  for (; currentPath.valid(); currentPath.next())
    {
      DagNode* sVal = currentPath.getDagNode();
      if (leq(sVal->getSortIndex(), stripperSort) && bindCollector(solution))  // can't update currentPath before bindCollector()
	{
	  //
	  //	Got a match.
	  //
	  solution.bind(stripperVarIndex, sVal);
	  currentPath.next();  // must always move to the next alternative
	  return true;
	}
    }
  return false;
}

bool
ACU_TreeVariableSubproblem::handleBoundCollector(RewritingContext& solution)
{
  DagNode* cVal = solution.value(collectorVarIndex);
  Term* identity = subject->symbol()->getIdentity();
  if (identity != nullptr && identity->equal(cVal))
    {
      //
      //	We have at least 2 subject arguments remaining and the stripper variable can take at most 1
      //	so if the collector takes the identity, no match is possible.
      //
      return false;
    }
  //
  //	We need exactly one argument left over from remaining after cVal is subtracted.
  //
  DagNode* leftOver = nullptr;
 
  if (cVal->symbol() == subject->symbol())
    {
      //
      //	Collector variable is bound to a term headed by the same ACU symbol.
      //	It could be in tree or argVec form so we use a slow general purpose iterator
      //	to inspect its arguments.
      //
      currentPath.reset(remaining);
      Assert(currentPath.valid(), "no remaining arguments");
      DagNode* d = currentPath.getDagNode();
      int multiplicity = currentPath.getMultiplicity();
      for (DagArgumentIterator i(cVal); i.valid(); i.next())
	{
	  DagNode* c = i.argument();
	  //
	  //	Try count off 1 copy of c.
	  //
	  for (;;)
	    {
	      if (c->equal(d))
		{
		  if (multiplicity == 0)
		    return false;  // multiplicity exhausted
		  --multiplicity;
		  break;
		}
	      else
		{
		  //
		  //	Deal with left over multiplicity of d.
		  //
		  if (multiplicity > 1)
		    return false;  // more than copy of d left over
		  if (multiplicity == 1)
		    {
		      if (leftOver != nullptr)
			return false;  // leftOver already full
		      leftOver = d;
		    }
		  //
		  //	Move to next remaining argument to try and find c.
		  //
		  currentPath.next();
		  if (!currentPath.valid())
		    return false;  // no more remaining arguments
		  d = currentPath.getDagNode();
		  multiplicity = currentPath.getMultiplicity();
		}
	    }
	}
      //
      //	We have now seen everything in cVal. Need to check what is left for stripper variable.
      //
      if (leftOver == nullptr)
	{
	  if (multiplicity > 1)
	    return false;  // too much
	  if (multiplicity == 1)
	    leftOver = d;  // just right
	  else
	    {
	      currentPath.next();
	      if (!currentPath.valid() || currentPath.getMultiplicity() > 1)
		return false;  // nothing or too much
	      leftOver = currentPath.getDagNode();  // just right
	    }
	}
      else if (multiplicity > 0)
	return false;  // leftOver already full
      //
      //	We have a dagNode for stripper variable.
      //	Need to check if there in anything left in remaining.
      //
      currentPath.next();
      if (currentPath.valid())
	return false; // leftOver already full
    }
  else
    {
      //
      //	Collector variable is bound to an alien.
      //	We check that this alien appears in remaining and there is only one other argument left.
      //	We are going to examine at most 2 arguments of remaining so we don't bother with find().
      //
      for (currentPath.reset(remaining); currentPath.valid(); currentPath.next())
	{
	  DagNode* d = currentPath.getDagNode();
	  int multiplicity = currentPath.getMultiplicity();
	  if (d->equal(cVal))
	    {
	      if (multiplicity > 2)
		return false;  // stripper can take only one copy
	      if (multiplicity == 2)
		{
		  if (leftOver != nullptr)
		    return false;
		  leftOver = d;
		}
	    }
	  else
	    {
	      if (leftOver != nullptr || multiplicity > 1)
		return false;  // stripper can take only one argument
	      leftOver = d;
	    }
	}
      //
      //	Because we have at least two arguments remaining, if we haven't returned false,
      //	then it is because we had exactly two arguments remaining, at least one of them
      //	is equal to cVal and leftOver points to the other (which could be equal to cVal also).
      //
      Assert(leftOver != nullptr, "nothing left over with collector = " << cVal);
    }
  //
  //	We've seen cVal and there is one argument left over; check whether it agrees with
  //	or can be assigned to stripper variable.
  //
  DagNode* sVal = solution.value(stripperVarIndex);
  if (sVal != nullptr)
    return sVal->equal(leftOver);
  if (leq(leftOver->getSortIndex(), stripperSort))
    {
      solution.bind(stripperVarIndex, leftOver);
      return true;
    }
  return false;
}

bool
ACU_TreeVariableSubproblem::handleBoundStripper(RewritingContext& solution)
{
  Assert(solution.value(collectorVarIndex) == nullptr, "collector bound");
  DagNode* sVal = solution.value(stripperVarIndex);
  Assert(sVal != nullptr, "stripper not bound");
  Assert(sVal->symbol() != subject->symbol(), "stripper bound to term headed by top symbol " << sVal);
  Assert(subject->symbol()->getIdentity() == nullptr || !(subject->symbol()->getIdentity()->equal(sVal)),
	 "stripper bound to identity " << sVal);
  //
  //	Stripper variable is bound to an alien. We need to see if we can remove it from remaining
  //	subject arguments. At most one solution is possible.
  //
  currentPath.clear();  // might have had previous solve() calls with other solutions
  return remaining.find(sVal, currentPath) && bindCollector(solution);
}

bool
ACU_TreeVariableSubproblem::bindCollector(RewritingContext& solution)
{
  ACU_Tree remainingCopy(remaining);
  remainingCopy.deleteMult2(currentPath, 1);  // preserves currentPath
  Assert(remainingCopy.getSize() != 0, "no arguments left");

  DagNode* cVal;
  if (remainingCopy.getSize() == 1 && remainingCopy.getMaxMult() == 1)
    {
      //
      //	Just one subject argument left to bind the collector variable to.
      //	It will already have its correct sort.
      //
      cVal = remainingCopy.getSoleDagNode();
      if (!leq(cVal->getSortIndex(), collectorSort))
	return false;
    }
  else
    {
      //
      //	More than one subject argument left; need to make a new ACU_TreeDagNode
      //	headed by our top symbol.
      //
      cVal = new ACU_TreeDagNode(subject->symbol(), remainingCopy);
      //
      //	checkSort() could run the garbage collector if sort constraints are executed
      //	but in this case it will protect cVal in a new context.
      //
      if (!(cVal->checkSort(collectorSort, solution)))
	return false;
      //
      //	If the parent dagnode was reduced and checkSort() filled in the sort index we
      //	can mark our binding as reduced.
      //
      if (subject->isReduced() && cVal->getSortIndex() != Sort::SORT_UNKNOWN)
	cVal->setReduced();
    }
  solution.bind(collectorVarIndex, cVal);
  return true;
}

void
ACU_TreeVariableSubproblem::markReachableNodes()
{
  //
  //	Because remaining may have been made using new dagnodes before it was passed to us
  //	and the GC can run during the solve phase, we're responsible for protecting it.
  //
  remaining.mark();
}

#ifdef DUMP

void
ACU_TreeVariableSubproblem::dump(ostream& s, const VariableInfo& variableInfo, int indentLevel)
{
  s << "begin{ACU_TreeVariableSubproblem}\n";
  s << "end{ACU_TreeVariableSubproblem}\n";
}

#endif
