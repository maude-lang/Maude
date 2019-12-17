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
//      Implementation for class AU_Layer.
//

//	utility stuff
#include "macros.hh"
#include "vector.hh"
#include "sequencePartition.hh"

//      forward declarations
#include "interface.hh"
#include "core.hh"
#include "AU_Theory.hh"

//      interface class definitions
#include "symbol.hh"
#include "term.hh"
#include "subproblem.hh"

//      core class definitions
#include "rewritingContext.hh"
#include "localBinding.hh"

//	AU theory class definitions
#include "AU_Symbol.hh"
// HACK
#undef assert
#include "AU_DagNode.hh"
#include "AU_ExtensionInfo.hh"
#include "AU_Layer.hh"

AU_Layer::AU_Layer()
{
  leftExtend = false;
  rightExtend = false;
  totalLowerBound = 0;
  totalUpperBound = 0;
  partition = 0;
}

AU_Layer::~AU_Layer()
{
  int nrNodes = nodes.length();
  for (int i = 0; i < nrNodes; i++)
    {
      Node& n = nodes[i];
      delete n.difference;
      delete n.subproblem;
    }
  delete partition;
}

void
AU_Layer::initialize(AU_DagNode* subjectDag)
{
  subject = subjectDag;
  lastSubjectSubterm = subjectDag->argArray.length() - 1;
  AU_Symbol* s = subjectDag->symbol();
  oneSidedId = s->oneSidedId();
  leftId = s->leftId();
}

void
AU_Layer::initializeFirst(int first, AU_ExtensionInfo* extension)
{
  firstSubterm = first;
  if (extension != 0)
    {
      extensionInfo = extension;
      leftExtend = true;
      totalUpperBound = UNBOUNDED;
    }
}

void
AU_Layer::initializeLast(int last, AU_ExtensionInfo* extension)
{
  lastSubterm = last;
  if (extension != 0)
    {
      extensionInfo = extension;
      rightExtend = true;
      totalUpperBound = UNBOUNDED;
    }
}

void
AU_Layer::addTopVariable(int index, int lowerBound, int upperBound, Sort* sort)
{
  int nrVariables = prevVariables.length();
  prevVariables.expandBy(1);
  TopVariable& tv = prevVariables[nrVariables];
  tv.index = index;
  tv.lowerBound = lowerBound;
  tv.upperBound = upperBound;
  tv.sort = sort;
  totalLowerBound += lowerBound;
  totalUpperBound = uplus(totalUpperBound, upperBound);
}

void
AU_Layer::addNode(LocalBinding* difference,
		  Subproblem* subproblem,
		  int firstSubterm,
		  int lastSubterm)
{
  int nrNodes = nodes.length();
  nodes.expandBy(1);
  Node& n = nodes[nrNodes];
  n.difference = difference;
  n.subproblem = subproblem;
  n.firstSubterm = firstSubterm;
  n.lastSubterm = lastSubterm;
  n.downTarget = UNDEFINED;
}

void
AU_Layer::link(AU_Layer& nextLayer)
{
  int nrNodes = nodes.length();
  int j = 0;
  int nextStart = nextLayer.nodes[j].firstSubterm - nextLayer.totalLowerBound;

  for (int i = 0; i < nrNodes; i++)
    {
      int lastSubterm = nodes[i].lastSubterm;
      while (nextStart <= lastSubterm)
	{
	  ++j;
	  Assert(j < nextLayer.nodes.length(), 
		 "could not find a valid target for node " << i);
	  nextStart = nextLayer.nodes[j].firstSubterm - nextLayer.totalLowerBound;
	}
      nodes[i].downTarget = j;
    }
}

bool
AU_Layer::solvePatterns(bool findFirst,
			RewritingContext& solution,
			AU_Layer& nextLayer)
{
  if (solvePatterns2(findFirst, solution))
    {
      Node& n = nodes[selectedNode];
      lastSubterm = n.firstSubterm - 1;
      nextLayer.firstSubterm = n.lastSubterm + 1;
      nextLayer.selectedNode = n.downTarget;
      return true;
    }
  return false;
}

bool
AU_Layer::solvePatterns2(bool findFirst, RewritingContext& solution)
{
  int nrNodes = nodes.length();
  for(; selectedNode < nrNodes; selectedNode++)
    {
      Node& n = nodes[selectedNode];
      if (findFirst)
	{
	  int d = n.firstSubterm - firstSubterm;
	  Assert(d >= totalLowerBound,
		 "insufficient subterms for previous variables");
	  if (d > totalUpperBound)
	    break;
	  if (n.difference != 0 && !(n.difference->assert(solution)))
	    continue;
	}
      if (n.subproblem == 0)
        {
          if (findFirst)
            return true;
        }
      else
	{
	  if (n.subproblem->solve(findFirst, solution))
            return true;
        }
      if (n.difference != 0)
        n.difference->retract(solution);
      findFirst = true;
    }
  return false;
}

bool
AU_Layer::solveVariables(bool findFirst, RewritingContext& solution)
{
  if (findFirst)
    partition = buildPartition(solution);
  while (solvePartition(findFirst))
    {
      if (bindVariables(solution))
	return true;
      MemoryCell::okToCollectGarbage();  // to avoid build up of failed solutions
      findFirst = false;
    }
  unbindVariables(solution);
  delete partition;
  partition = 0;
  return false;
}

bool
AU_Layer::solvePartition(bool findFirst)
{
  if (oneSidedId)
    {
      int nrVariables = prevVariables.length();
      if (!findFirst)
	{
	  //
	  //	See if we can change existing solution by messing with extra
	  //	identities.
	  //
	  int nrVariables = prevVariables.length();
	  for (int i = nrVariables - 1; i >= 0; i--)
	    {
	      TopVariable& tv = prevVariables[i];
	      if (tv.boundByUs && !(tv.extraId))
		{
		  int start = firstSubterm + partition->start(i + leftExtend);
		  int end = firstSubterm + partition->end(i + leftExtend);
		  int nrSubterms = end - start + 1;
		  if (nrSubterms > 0 && nrSubterms < tv.upperBound &&
		      (leftId ? (end < lastSubjectSubterm) : (start > 0)))
		    {
		      tv.extraId = true;
		      for (i++; i < nrVariables; i++)
			{
			  TopVariable& v = prevVariables[i];
			  if (v.boundByUs)
			    v.extraId = false;
			}
		      return true;
		    }
		}
	    }
	}
      for (int i = 0; i < nrVariables; i++)
	{
	  TopVariable& tv = prevVariables[i];
	  if (tv.boundByUs)
	    tv.extraId = false;
	}
    }
  return partition->solve();
}

SequencePartition*
AU_Layer::buildPartition(const Substitution& solution)
{
  AU_Symbol* s = subject->symbol();
  int nrVariables = prevVariables.length();
  int nrParts = nrVariables + leftExtend + rightExtend;
  Assert(nrParts > 0, "no parts");
  SequencePartition* p = new SequencePartition(lastSubterm - firstSubterm + 1, nrParts);

  if (leftExtend)
    p->insertPart(0, UNBOUNDED);
  for (int i = 0; i < nrVariables; i++)
    {
      TopVariable& v = prevVariables[i];
      int min = v.lowerBound;
      int max = v.upperBound;
      DagNode* d = solution.value(v.index);
      if (d != 0)
	{
	  v.boundByUs = false;
	  bool leftExtreme = (i == 0 && firstSubterm == 0);
	  bool rightExtreme = (i == nrVariables - 1 && lastSubterm == lastSubjectSubterm);
	  bool nasty;
	  min = max = s->calculateNrSubjectsMatched(d, leftExtreme, rightExtreme, nasty);
	  DebugAdvisoryCheck(!nasty, "nasty binding of " << d <<
			     " to variable with index " << v.index <<
			     " detected in solve phase");
	  if (nasty && (leftId ? rightExtend : leftExtend))
	    {
	      //
	      //	Because extension may take extreme end, matching one
	      //	less subterm is possible.
	      //
	      --min;
	    }
	}
      else
	{
	  v.boundByUs = true;
	  //
	  //	Check for early occurrence of same variable in this block.
	  //
	  for (int j = 0; j < i; j++)
	    {
	      if (prevVariables[j].index == v.index)
		{
		  v.boundByUs = false;
		  break;
		}
	    }
	}
      p->insertPart(min, max);
    }
  if (rightExtend)
    p->insertPart(0, UNBOUNDED);
  return p;
}

bool
AU_Layer::bindVariables(RewritingContext& solution)
{
  bool buildReducedNodes = subject->isReduced();
  AU_Symbol* topSymbol = subject->symbol();
  int nrVariables = prevVariables.length();
  for (int i = 0; i < nrVariables; i++)
    {
      int start = firstSubterm + partition->start(i + leftExtend);
      int end = firstSubterm + partition->end(i + leftExtend);
      TopVariable& tv = prevVariables[i];
      if (tv.boundByUs)
	{
	  int nrSubterms = end - start + 1;
	  if (nrSubterms == 0)
	    {
	      if (oneSidedId)
		{
		  if (leftId ? (end == lastSubjectSubterm) : (start == 0))
		    return false;  // can't assign one sided identity
		}
	      solution.bind(tv.index, topSymbol->getIdentityDag());
	    }
	  else
	    {
	      DagNode* d = subject->makeFragment(start, nrSubterms,
						 oneSidedId && prevVariables[i].extraId);
	      if (!(d->checkSort(tv.sort, solution)))
		return false;
	      if (d->symbol() == topSymbol)
		{
		  //
		  //	makeFragment() will have produced an AU_DagNode*
		  //
		  safeCast(AU_DagNode*, d)->setProducedByAssignment();
		  if (buildReducedNodes && d->getSortIndex() != Sort::SORT_UNKNOWN)
		    d->setReduced();
		}
	      solution.bind(tv.index, d);
	    }
	}
      else
	{
	  DagNode* d = solution.value(tv.index);
	  Assert(d != 0, "variable should be bound");
	  if (!(subject->eliminateForward(d, start, end) && start == end + 1))
	    return false;
	}
    }
  if (leftExtend)
    {
      if (!leftId)
	extensionInfo->setExtraIdentity(false);
      Assert(firstSubterm == 0, "non zero firstSubterm in left extension case");
      int first = partition->end(0) + 1;
      extensionInfo->setFirstMatched(first);
      if (oneSidedId && !leftId && first != 0 && nrVariables > 0)
	{
	  int nrMatched = partition->end(1) - first + 1;
	  DagNode* d = solution.value(prevVariables[0].index);
	  if (nrMatched == 0 ||
	      (d->symbol() == topSymbol &&
	       static_cast<AU_DagNode*>(d)->argArray.length() > nrMatched))  // FIX
	    extensionInfo->setExtraIdentity(true);
	}
    }
  if (rightExtend)
    {
      if (leftId)
	extensionInfo->setExtraIdentity(false);
      int last = firstSubterm + partition->start(nrVariables + leftExtend) - 1;
      extensionInfo->setLastMatched(last);
      if (oneSidedId && leftId && last != lastSubjectSubterm && nrVariables > 0)
	{
	  int nrMatched = partition->start(nrVariables + leftExtend) -
	    partition->start(nrVariables - 1 + leftExtend);
	  DagNode* d = solution.value(prevVariables[0].index);
	  if (nrMatched == 0 ||
	      (d->symbol() == topSymbol &&
	       static_cast<AU_DagNode*>(d)->argArray.length() > nrMatched))  // FIX
	    extensionInfo->setExtraIdentity(true);
	}
    }
  return true;
}

void
AU_Layer::unbindVariables(RewritingContext& solution)
{
  int nrVariables = prevVariables.length();
  for (int i = 0; i < nrVariables; i++)
    {
      TopVariable& v = prevVariables[i];
      if (v.boundByUs)
	solution.bind(v.index, 0);
    }
}
