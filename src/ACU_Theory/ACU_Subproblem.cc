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
//      Implementation for class ACU_Subproblem.
//

//	utility stuff
#include "macros.hh"
#include "vector.hh"
#include "indent.hh"
#include "diophantineSystem.hh"

//      forward declarations
#include "interface.hh"
#include "core.hh"
#include "ACU_Persistent.hh"
#include "ACU_Theory.hh"

//      interface class definitions
#include "argVec.hh"
#include "associativeSymbol.hh"
#include "dagNode.hh"
#include "subproblem.hh"

//      core class definitions
#include "variableInfo.hh"
#include "substitution.hh"
#include "rewritingContext.hh"
#include "localBinding.hh"

//	ACU theory class definitions
#include "ACU_Symbol.hh"
#include "ACU_DagNode.hh"
#include "ACU_ExtensionInfo.hh"
#include "ACU_Subproblem.hh"

ACU_Subproblem::ACU_Subproblem(ACU_DagNode* subject,
			       ACU_ExtensionInfo* extensionInfo)
  : subject(subject),
    extensionInfo(extensionInfo)
{
  //cout << "ACU_Subproblem " << ((void*) this) << " created" << endl;
  system = 0;
}

void
ACU_Subproblem::addSubjects(Vector<int>& multiplicity)
{
  currentMultiplicity = multiplicity;  // deep copy
}

ACU_Subproblem::~ACU_Subproblem()
{
  //cout << "ACU_Subproblem " << ((void*) this) << " destroyed" << endl;
  FOR_EACH_CONST(i, Vector<PatternNode>, patternNodes)
    {
      FOR_EACH_CONST(j, Vector<Edge>, i->edges)
	{
	  delete j->difference;
	  delete j->subproblem;
	}
    }
  delete system;
}

int
ACU_Subproblem::addPatternNode(int multiplicity)
{
  int nrPatternNodes = patternNodes.length();
  patternNodes.expandBy(1);
  patternNodes[nrPatternNodes].multiplicity = multiplicity;
  return nrPatternNodes;
}

void
ACU_Subproblem::removePatternNode(int& uniqueSubject,
				  LocalBinding*& difference,
				  Subproblem*& subproblem)
{
  int newLen = patternNodes.length() - 1;
  PatternNode& p = patternNodes[newLen];
  Assert(p.edges.length() == 1, "can only remove pattern with single edge");
  Edge& e = p.edges[0];
  uniqueSubject = e.target;
  difference = e.difference;
  subproblem = e.subproblem;
  patternNodes.contractTo(newLen);
}

void
ACU_Subproblem::addEdge(int pattern,
			int target,
			LocalBinding* difference,
			Subproblem* subproblem)
{
  PatternNode& p = patternNodes[pattern];
  int nrEdges = p.edges.length();
  p.edges.expandBy(1);
  Edge& edge = p.edges[nrEdges];
  edge.target = target;
  edge.difference = difference;
  edge.subproblem = subproblem;
}

void
ACU_Subproblem::addTopVariable(int index,
			       int multiplicity,
			       int lowerBound,
			       int upperBound,
			       const Sort* sort)
{
  int nrTopVariables = topVariables.length();
  topVariables.expandBy(1);
  TopVariable& tv = topVariables[nrTopVariables];
  tv.index = index;
  tv.multiplicity = multiplicity;
  tv.lowerBound = lowerBound;
  tv.upperBound = upperBound;
  tv.sort = sort;
}

bool
ACU_Subproblem::solve(bool findFirst, RewritingContext& solution)
{
  if (!findFirst && solveVariables(false, solution))
    return true;
  for (;;)
    {
      if (!solvePatterns(findFirst, solution))
        return false;
      if (solveVariables(true, solution))
        return true;
      findFirst = false;
    }
}

bool
ACU_Subproblem::solvePatterns(bool findFirst, RewritingContext& solution)
{
  int nrPatterns = patternNodes.length();
  if (nrPatterns == 0)
    return findFirst;
  int i = findFirst ? 0 : nrPatterns - 1;
  for(;;)
    {
      findFirst = patternNodes[i].solve(findFirst, solution, currentMultiplicity);
      if (findFirst)
	{
	  if (++i == nrPatterns)
	    return true;
	}
      else
	{
	  if (--i < 0)
	    break;
	}
    }
  return false;
}

bool
ACU_Subproblem::PatternNode::solve(bool findFirst,
				   RewritingContext& solution,
				   Vector<int>& currentMultiplicity)
{
  int nrEdges = edges.length();
  if (findFirst)
    selectedEdge = 0;
  for (; selectedEdge < nrEdges; selectedEdge++)
    {
      Edge& e = edges[selectedEdge];
      int target = e.target;
      if (findFirst)
	{
	  if (currentMultiplicity[target] < multiplicity)
	    continue;
	  if (e.difference != 0 && !(e.difference->assert(solution)))
	    continue;
	  currentMultiplicity[target] -= multiplicity;
	}
      Subproblem* subproblem = e.subproblem;
      if (subproblem == 0)
	{
	  if (findFirst)
	    return true;
	}
      else
	{
	  if (subproblem->solve(findFirst, solution))
	    return true;
	}
      currentMultiplicity[target] += multiplicity;
      if (e.difference != 0)
	e.difference->retract(solution);
      findFirst = true;
    }
  return false;
}

bool
ACU_Subproblem::solveVariables(bool findFirst, RewritingContext& solution)
{
  if (findFirst && !extractDiophantineSystem(solution))
    return false;
  int nrVars = variableMap.length();
  ACU_Symbol* topSymbol = subject->symbol();
  if (system == 0)
    {
      //
      //	Deal with trivial Diophantine system - 
      //	we can succeed only once.
      //
      if (findFirst)
	return true;
    }
  else
    {
      bool buildReducedNodes = subject->isReduced();
      while (system->solve())
	{
	  for (int i = 0; i < nrVars; i++)
	    {
	      TopVariable& tv = topVariables[variableMap[i]];
	      DagNode *d = computeAssignment(i);
	      if (d == 0)
		d = topSymbol->getIdentityDag();
	      else
		{
		  if (!(d->checkSort(tv.sort, solution)))
		    goto fail;
		  if (d->symbol() == topSymbol &&
		      buildReducedNodes &&
		      d->getSortIndex() != Sort::SORT_UNKNOWN)
		    d->setReduced();
		}
	      solution.bind(tv.index, d);
	    }
	  if (extensionInfo != 0)
	    fillOutExtensionInfo();
	  return true;
	fail:
	  MemoryCell::okToCollectGarbage();  // to avoid build up of failed solutions
	}
    }
  //
  //	Unbind variables involved in Diophantine system
  //	(Can we guarentee that the same set of variables will be involved
  //	in each Diophantine system for this subproblem and avoid this; and
  //	also reuse variableMap?); Subjects (even number of subjects) will
  //	certainly change for each system.
  //
  FOR_EACH_CONST(i, Vector<int>, variableMap)
    solution.bind(topVariables[*i].index, 0);
  return false;
}

bool
ACU_Subproblem::noVariableCase(const Vector<int>& multVec)
{
  int nrSubjects = multVec.length();
  if (extensionInfo == 0)
    {
      for (int i = 0; i < nrSubjects; i++)
	{
	  if (multVec[i] > 0)
	    return false;
	}
      return true;
    }
  //
  //	Extension takes everything.
  //
  extensionInfo->clear();
  int total = 0;
  for (int i = 0; i < nrSubjects; i++)
    {
      int t = multVec[i];
      if (t > 0)
	{
	  extensionInfo->setUnmatched(i, t);
	  total += t;
	}
    }
  if (total > extensionInfo->getUpperBound())
    return false;  // would give extension too much
  extensionInfo->setMatchedWhole(total == 0);
  return true;
}

bool
ACU_Subproblem::oneVariableCase(const Vector<int>& multVec, RewritingContext& solution)
{
  //
  //	The case where we have a single unbound top variable
  //	left which has multiplicity 1 and there is no extension
  //	is common enough that we handle it with custom code that
  //	avoids the Diophantine system setup and tear-down costs.
  //
  int nrSubjects = multVec.length();
  int nrSubterms = 0;
  int last = NONE;
  for (int i = 0; i < nrSubjects; i++)
    {
      if (multVec[i] > 0)
	{
	  ++nrSubterms;
	  last = i;
	}
    }

  TopVariable& tv = topVariables[variableMap[0]];
  DagNode* d;
  if (nrSubterms == 0)
    {
      if (tv.lowerBound > 0)
	return false;
      d = subject->symbol()->getIdentityDag();
    }
  else if (nrSubterms == 1 && multVec[last] == 1)
    {
      d = (subject->argArray[last]).dagNode;
      if (!(d->leq(tv.sort)))  // d's sort must be known
	return false;
    }
  else
    {
      ACU_DagNode* a = new ACU_DagNode(subject->symbol(), nrSubterms, ACU_DagNode::ASSIGNMENT);
      ArgVec<ACU_DagNode::Pair>::const_iterator source = subject->argArray.begin();
      ArgVec<ACU_DagNode::Pair>::iterator dest = a->argArray.begin();
      for (int i = 0; i <= last; i++, ++source)
	{
	  int m = multVec[i];
	  if (m > 0)
	    {
	      dest->dagNode = source->dagNode;
	      dest->multiplicity = m;
	      ++dest;
	    }
	}
      if (!(a->checkSort(tv.sort, solution)))
	return false;
      if (subject->isReduced() && a->getSortIndex() != Sort::SORT_UNKNOWN)
	a->setReduced();
      d = a;
    }
  solution.bind(tv.index, d);
  return true;
}

bool
ACU_Subproblem::extractDiophantineSystem(RewritingContext& solution)
{
  //
  //	Initialize data structures
  //
  delete system;
  system = 0;
  variableMap.clear();
  //
  //	Check for easy special cases.
  //
  int nrTopVariables = topVariables.length();
  if (nrTopVariables == 1)
    {
      if (extensionInfo == 0)
	{
	  TopVariable& tv = topVariables[0];
	  if (tv.multiplicity == 1 && solution.value(tv.index) == 0)
	    {
	      variableMap.append(0);
	      return oneVariableCase(currentMultiplicity, solution);
	    }
	}
    }
  else if (nrTopVariables == 0)
    return noVariableCase(currentMultiplicity);
  //
  //	General case requires that we build the Diophantine system.
  //
  int nrSubjects = currentMultiplicity.length();
  system = new DiophantineSystem(nrTopVariables, nrSubjects);
  afterMultiplicity = currentMultiplicity;  // deep copy
  //
  //	Find unbound variables (= rows in Diophantine system)
  //
  int nrNonIdentityVariables = 0;
  for (int i = 0; i < nrTopVariables; i++)
    {
      TopVariable& tv = topVariables[i];
      DagNode* d = solution.value(tv.index);
      if (d != 0)
	{
	  if (!(subject->eliminateSubject(d, tv.multiplicity, afterMultiplicity)))
	    return false;
	}
      else
	{
	  variableMap.append(i);
	  if (tv.lowerBound > 0)
	    ++nrNonIdentityVariables;
	  system->insertRow(tv.multiplicity, tv.lowerBound, tv.upperBound);
	}
    }
  //
  //	Check if after handing bound variables we have reached an easy special case.
  //
  int nrVariables = variableMap.length();
  if (nrVariables == 1)
    {
      if (extensionInfo == 0 && topVariables[variableMap[0]].multiplicity == 1)
	{
	  delete system;
	  system = 0;
	  return oneVariableCase(afterMultiplicity, solution);
	}
    }
  else if (nrVariables == 0)
    {
      delete system;
      system = 0;
      return noVariableCase(afterMultiplicity);
    }
  //
  //	Find unused subjects (= columns in Diophantine system)
  //
  subjectMap.clear();
  for (int i = 0; i < nrSubjects; i++)
    {
      int t = afterMultiplicity[i];
      if (t > 0)
	{
	  subjectMap.append(i);
	  system->insertColumn(t);
	}
    }
  //
  //	Check for trivial system.
  //
  if (subjectMap.empty())
    {
      //
      //	No subjects.
      //
      delete system;
      system = 0;
      if (nrNonIdentityVariables > 0)
	return false;
      //
      //	Extension gets nothing and any unbound variables
      //	must be bound to identity.
      //
      if (extensionInfo != 0)
	extensionInfo->setMatchedWhole(true);
      int nrVars = variableMap.length();
      if (nrVars > 0)
	{
	  DagNode* identityDag = subject->symbol()->getIdentityDag();
	  for (int i = 0; i < nrVars; i++)
	    solution.bind(topVariables[variableMap[i]].index, identityDag);
	}
      return true;
    }
  //
  //	Extra row to account for extension.
  //
  if (extensionInfo != 0)
    system->insertRow(1, 0, extensionInfo->getUpperBound());
  return true;
}

DagNode*
ACU_Subproblem::computeAssignment(int row)
{
  int nrColumns = system->columnCount();
  int nrSubterms = 0;
  int totalMultiplicity = 0;
  int col = UNDEFINED;
  for (int i = 0; i < nrColumns; i++)
    {
      int t = system->solution(row, i);
      if (t > 0)
	{
	  ++nrSubterms;
	  totalMultiplicity += t;
	  col = i;
	}
    }
  if (totalMultiplicity == 1)
    return (subject->argArray[subjectMap[col]]).dagNode;
  if (totalMultiplicity == 0)
    return 0;  // identity case
  ACU_Symbol* s = subject->symbol();
  ACU_DagNode* d = new ACU_DagNode(s, nrSubterms, ACU_DagNode::ASSIGNMENT);
  int pos = 0;
  //
  //	Because subject is in normal form and subjectMap is monotonic, we will
  //	construct new ACU_DagNode in normal form.
  //
  for (int i = 0; i <= col; i++)
    {
      int t = system->solution(row, i);
      if (t > 0)
	{
	  d->argArray[pos].dagNode = (subject->argArray[subjectMap[i]]).dagNode;
	  d->argArray[pos].multiplicity = t;
	  ++pos;
	}
    }
  return d;
}

void
ACU_Subproblem::fillOutExtensionInfo()
{
  extensionInfo->setMatchedWhole(true);
  int nrColumns = system->columnCount();
  int extensionRow = system->rowCount() - 1;
  bool partial = false;
  for (int i = 0; i < nrColumns; i++)
    {
      int t = system->solution(extensionRow, i);
      if (t > 0)
	{
	  if (!partial)
	    {
	      extensionInfo->setMatchedWhole(false);
	      extensionInfo->clear();
	      partial = true;
	    }
	  extensionInfo->setUnmatched(subjectMap[i], t);
	}
    }
}

#ifdef DUMP
void
ACU_Subproblem::dump(ostream& s, const VariableInfo& variableInfo, int indentLevel)
{
  s << Indent(indentLevel) << "Begin{ACU_Subproblem}\n";
  ++indentLevel;
  s << Indent(indentLevel) << "subject = " << subject << '\n';
  s << Indent(indentLevel) << "extensionInfo = " << extensionInfo << '\n';
  s << Indent(indentLevel) << "currentMultiplicity:\n";
  ++indentLevel;
  for (int i = 0; i < currentMultiplicity.length(); i++)
    {
      s << Indent(indentLevel) << currentMultiplicity[i] << '\t' <<
	subject->argArray[i].dagNode << '\n';
    }
  s << Indent(indentLevel - 1) << "topVariables:\n";
  for (int i = 0; i < topVariables.length(); i++)
    {
      TopVariable& tv = topVariables[i];
      s << Indent(indentLevel) << "index = " << tv.index <<
	" \"" << variableInfo.index2Variable(tv.index) <<
	"\"\tmultiplicity = " << tv.multiplicity <<
	"\tlowerBound = " << tv.lowerBound <<
	"\tupperBound = " << tv.upperBound <<
	"\tsort = " << tv.sort << '\n';
    }
  s << Indent(indentLevel - 1) << "patternNodes:\n";
  for (int i = 0; i < patternNodes.length(); i++)
    {
      PatternNode& pn = patternNodes[i];
      s << Indent(indentLevel) << "multiplicity = " << pn.multiplicity <<
	"\tedges:\n";
      for (int j = 0; j < pn.edges.length(); j++)
	{
	  Edge& e = pn.edges[j];
	  s << Indent(indentLevel + 1) << "target = " << e.target << '\n';
	  s << Indent(indentLevel + 1) << "difference = ";
	  if (e.difference == 0)
	    s << "0\n";
	  else
	    {
	      s << '\n';
	      e.difference->dump(s, variableInfo, indentLevel + 2);
	    }
	  s << Indent(indentLevel + 1) << "subproblem = ";
	  if (e.subproblem == 0)
	    s << "0\n";
	  else
	    {
	      s << '\n';
	      e.subproblem->dump(s, variableInfo, indentLevel + 2);
	    }
	}
    }
  s << Indent(indentLevel - 2) << "End{ACU_Subproblem}\n";
}
#endif
