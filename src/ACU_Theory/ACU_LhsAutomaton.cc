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
//      Implementation for class ACU_LhsAutomaton.
//

//	utility stuff
#include "macros.hh"
#include "indent.hh"
#include "vector.hh"

//      forward declarations
#include "interface.hh"
#include "core.hh"
#include "ACU_Persistent.hh"
#include "ACU_Theory.hh"

//      interface class definitions
#include "argVec.hh"
#include "associativeSymbol.hh"
#include "dagNode.hh"
#include "term.hh"
#include "subproblem.hh"
#include "lhsAutomaton.hh"
#include "rhsAutomaton.hh"

//      core class definitions
#include "variableInfo.hh"
#include "subproblemSequence.hh"
#include "subproblemDisjunction.hh"
#include "subproblemAccumulator.hh"
#include "disjunctiveSubproblemAccumulator.hh"
#include "substitution.hh"
#include "localBinding.hh"
#include "variableAbstractionSubproblem.hh"
#include "equalitySubproblem.hh"

//      variable class definitions
#include "variableSymbol.hh"
#include "variableTerm.hh"

//	ACU Red-Black class definitions
#include "ACU_SlowIter.hh"

//	ACU theory class definitions
#include "ACU_Symbol.hh"
#include "ACU_DagNode.hh"
#include "ACU_TreeDagNode.hh"
#include "ACU_LhsAutomaton.hh"
#include "ACU_Subproblem.hh"
#include "ACU_LazySubproblem.hh"
#include "ACU_TreeVariableSubproblem.hh"
#include "ACU_ExtensionInfo.hh"

//	our stuff
#include "ACU_TreeMatcher.cc"
#include "ACU_CollapseMatcher.cc"
#include "ACU_Matcher.cc"
#include "ACU_GreedyMatcher.cc"

ACU_LhsAutomaton::ACU_LhsAutomaton(ACU_Symbol* symbol,
				   bool matchAtTop,
				   bool collapsePossible,
				   int nrVariables)
  : topSymbol(symbol),
    matchAtTop(matchAtTop),
    collapsePossible(collapsePossible),
    local(nrVariables),
    scratch(nrVariables)
{
  totalLowerBound = 0;
  totalUpperBound = matchAtTop ? UNBOUNDED : 0;
  maxPatternMultiplicity = 0;
  totalNonGroundAliensMultiplicity = 0;
  uniqueCollapseAutomaton = 0;
  treeMatchOK = true;
  collectorSeen = matchAtTop;  // consider extension as collector
  stripperVariableSeen = false;
  nrExpectedUnboundVariables = 0;
} 

ACU_LhsAutomaton::~ACU_LhsAutomaton()
{
  delete uniqueCollapseAutomaton;
  int nrTopVariables = topVariables.length();
  for (int i = 0; i < nrTopVariables; i++)
    delete topVariables[i].abstracted;
  int nrGroundedOutAliens = groundedOutAliens.length();
  for (int i = 0; i < nrGroundedOutAliens; i++)
    delete groundedOutAliens[i].automaton;
  int nrGroundAliens = nonGroundAliens.length();
  for (int i = 0; i < nrGroundAliens; i++)
    delete nonGroundAliens[i].automaton;
}

void
ACU_LhsAutomaton::updateTotals(int min, int max)
{
  totalLowerBound += min;
  if (min > maxPatternMultiplicity)
    maxPatternMultiplicity = min;
  if (totalUpperBound != UNBOUNDED && max != UNBOUNDED)
    totalUpperBound += max;
  else
    totalUpperBound = UNBOUNDED;
}

void
ACU_LhsAutomaton::addTopVariable(const VariableTerm* variable,
				 int multiplicity,
				 bool willBeBound)
{
  Sort* s = variable->getSort();
  int bound = topSymbol->sortBound(s);
  collectorSeen = collectorSeen || (!willBeBound && bound == UNBOUNDED && multiplicity == 1);  // can it serve as colletor?
  if (!willBeBound)
    ++nrExpectedUnboundVariables;
  bool takeIdentity = topSymbol->takeIdentity(s);
  if (!willBeBound && !takeIdentity && bound == 1)
    stripperVariableSeen = true;
  int nrTopVariables = topVariables.length();
  topVariables.expandBy(1);
  TopVariable& tv = topVariables[nrTopVariables];
  tv.index = variable->getIndex();
  tv.multiplicity = multiplicity;
  tv.sort = s;
  tv.upperBound = bound;
  tv.structure = topSymbol->sortStructure(s);
  tv.takeIdentity = takeIdentity;
  tv.abstracted = 0;
  updateTotals(takeIdentity ? 0 : multiplicity,
	       (bound == UNBOUNDED) ? UNBOUNDED : (bound * multiplicity));
}

void
ACU_LhsAutomaton::addAbstractionVariable(int index,
					 Sort* sort,
					 int upperBound,
					 bool takeIdentity,
					 LhsAutomaton* abstracted,
					 int multiplicity)
{
  int nrTopVariables = topVariables.length();
  topVariables.expandBy(1);
  TopVariable& tv = topVariables[nrTopVariables];
  tv.index = index;
  tv.multiplicity = multiplicity;
  tv.sort = sort;
  tv.upperBound = upperBound;
  tv.structure = AssociativeSymbol::UNSTRUCTURED;
  tv.takeIdentity = takeIdentity;
  tv.abstracted = abstracted;
  updateTotals(takeIdentity ? 0 : multiplicity,
	       (upperBound == UNBOUNDED) ? UNBOUNDED : (upperBound * multiplicity));
  treeMatchOK = false;
}

void
ACU_LhsAutomaton::addGroundAlien(Term* alien, int multiplicity)
{
  updateTotals(multiplicity, multiplicity);
  int nrGroundAliens = groundAliens.length();
  groundAliens.expandBy(1);
  groundAliens[nrGroundAliens].term = alien;
  groundAliens[nrGroundAliens].multiplicity = multiplicity;
}

void
ACU_LhsAutomaton::addGroundedOutAlien(Term* alien, LhsAutomaton* automaton, int multiplicity)
{
  updateTotals(multiplicity, multiplicity);
  int nrGroundedOutAliens = groundedOutAliens.length();
  groundedOutAliens.expandBy(1);
  groundedOutAliens[nrGroundedOutAliens].term = alien->stable() ? alien : 0;
  groundedOutAliens[nrGroundedOutAliens].automaton = automaton;
  groundedOutAliens[nrGroundedOutAliens].multiplicity = multiplicity;
  treeMatchOK = treeMatchOK && alien->stable();
}

void
ACU_LhsAutomaton::addNonGroundAlien(Term* alien, LhsAutomaton* automaton, int multiplicity)
{
  updateTotals(multiplicity, multiplicity);
  totalNonGroundAliensMultiplicity += multiplicity;
  int nrNonGroundAliens = nonGroundAliens.length();
  nonGroundAliens.expandBy(1);
  nonGroundAliens[nrNonGroundAliens].term = alien->stable() ? alien : 0;
  nonGroundAliens[nrNonGroundAliens].automaton = automaton;
  nonGroundAliens[nrNonGroundAliens].multiplicity = multiplicity;
  treeMatchOK = treeMatchOK && alien->stable();
}

local_inline bool
ACU_LhsAutomaton::topVariableLt(const TopVariable& t1, const TopVariable& t2)
{
  //
  //	Variables with high multiplicity go to the begining of the vector.
  //	Between variables of equal multiplicity, variables of high sort
  //	index (low sort) come first.
  //
  int r = t2.multiplicity - t1.multiplicity;
  return (r != 0) ? (r < 0) : (t2.sort->index() < t1.sort->index());
}

void
ACU_LhsAutomaton::complete(MatchStrategy strategy,
			   int nrIndependent)
{
  //
  //	For red-black greedy matching to be correct we require that
  //	(1) unbound top variables are quasi-linear and don't occur in the
  //	    condition;
  //	(2) unbound variables in a nonground alien don't occur in other
  //	    aliens, in our context or in the condition;
  //	(3) there are no abstraction variables;
  //	(4) all groundout aliens and nonground aliens are stable; and
  //	(5) - obsolete - (was SAT_MULT related)
  //
  //	For red-black greedy matching to be a win we require that
  //	(6) subproblems must be unlikely; and
  //	(7) there is a unbound variable or extension that can take almost
  //	    everything.
  //
  //	(1) and (2) are satisfied if strategy is LONE_VARIABLE or GREEDY.
  //	(6) is satisfied if strategy is  GREEDY or strategy is LONE_VARIABLE
  //	and (3) holds.
  //	treeMatchOK will have already been falsified if (3) or (4) fail to
  //	hold. (7) is recorded by the collectorSeen flag.
  //
  if (treeMatchOK)
    {
      if (strategy == LONE_VARIABLE || strategy == GREEDY)
	treeMatchOK = collectorSeen;
      else if (strategy == FULL)
	{
	  //
	  //	We now allow full tree matching in two common special cases.
	  //
	  //	We potentially have an variable stripper-collector situation if we have two unbound
	  //	variables and one is element variable.
	  //
	  bool varStripper = nrExpectedUnboundVariables == 2 && stripperVariableSeen;
	  //
	  //	We potentially have an nga stripper-collector situation if we have one unbound variable
	  //	and a single nga which must have multiplicity 1.
	  bool ngaStripper = nrExpectedUnboundVariables == 1 && nonGroundAliens.length() == 1 && nonGroundAliens[0].multiplicity == 1;
	  //
	  //	In either situation, we need a collector variable and we cannot be at the top (since
	  //	extension would provided multiple ways of splitting the remaining subterm arguments between
	  //	the collector variable and extension).
	  //
	  treeMatchOK = collectorSeen && !matchAtTop && (varStripper || ngaStripper);
	}
      else
	treeMatchOK = false;
    }

  matchStrategy = strategy;
  Assert(nrIndependent <= nonGroundAliens.length(), "too many independent");
  nrIndependentAliens = nrIndependent;
  sort(topVariables.begin(), topVariables.end(), topVariableLt);
}

#ifdef DUMP
void
ACU_LhsAutomaton::dump(ostream& s, const VariableInfo& variableInfo, int indentLevel)
{
  s << Indent(indentLevel) << "Begin{ACU_LhsAutomaton}\n";
  ++indentLevel;
  s << Indent(indentLevel) << "topSymbol = \"" << topSymbol <<
    "\"\tmatchAtTop = " << static_cast<bool>(matchAtTop) <<
    "\tcollapsePossible = " << static_cast<bool>(collapsePossible) << '\n';
  s << Indent(indentLevel) << "treeMatchOK = " << static_cast<bool>(treeMatchOK) <<
    "\tcollectorSeen = " << static_cast<bool>(collectorSeen) <<
    "\tmatchStrategy = " << static_cast<MatchStrategy>(matchStrategy) << '\n';
  if (uniqueCollapseAutomaton != 0)
    {
      s << Indent(indentLevel) << "uniqueCollapseAutomaton =\n";
      uniqueCollapseAutomaton->dump(s, variableInfo, indentLevel + 1);
    }
  s << Indent(indentLevel) << "totalLowerBound = " << totalLowerBound  <<
    "\tmaxPatternMultiplicity = " << maxPatternMultiplicity <<
    "\ttotalUpperBound = " << totalUpperBound << '\n';
  s << Indent(indentLevel) <<
    "totalNonGroundAliensMultiplicity = " << totalNonGroundAliensMultiplicity <<
    "\tnrIndependentAliens = " << nrIndependentAliens << '\n';

  s << Indent(indentLevel) << "topVariables:\n";
  ++indentLevel;
  for (int i = 0; i < topVariables.length(); i++)
    {
      TopVariable& tv = topVariables[i];
      s << Indent(indentLevel) << "index = " << tv.index;
      if (tv.abstracted == 0)
	s << " \"" << variableInfo.index2Variable(tv.index) << '"';
      s << "\tmultiplicity = " << tv.multiplicity <<
	"\tsort = \"" << tv.sort << 
	"\"\tupperBound = " << tv.upperBound <<
	"\tstructure = " << tv.structure <<
	"\ttakeIdentity = " << tv.takeIdentity << '\n';
      if (tv.abstracted != 0)
	{
	  s << Indent(indentLevel) << "abstracted = " << '\n';
	  tv.abstracted->dump(s, variableInfo, indentLevel + 1);
	} 
    }

  s << Indent(indentLevel - 1) << "groundAliens:\n";
  for (int i = 0; i < groundAliens.length(); i++)
    {
      s << Indent(indentLevel) << "term = \"" << groundAliens[i].term <<
	"\"\tmultiplicity = " << groundAliens[i].multiplicity << '\n';
    }

  s << Indent(indentLevel - 1) << "groundedOutAliens:\n";
  for (int i = 0; i < groundedOutAliens.length(); i++)
    {
      s << Indent(indentLevel) << "multiplicity = " << groundedOutAliens[i].multiplicity <<
	"\tautomaton =\n";
      groundedOutAliens[i].automaton->dump(s, variableInfo, indentLevel + 1);
    }

  s << Indent(indentLevel - 1) << "nonGroundAliens:\n";
  for (int i = 0; i < nonGroundAliens.length(); i++)
    {
      s << Indent(indentLevel) << "multiplicity = " << nonGroundAliens[i].multiplicity <<
	"\tautomaton =\n";
      nonGroundAliens[i].automaton->dump(s, variableInfo, indentLevel + 1);
    }

  s << Indent(indentLevel - 2) << "End{ACU_LhsAutomaton}\n";
}


ostream& operator<<(ostream& s, ACU_LhsAutomaton::MatchStrategy strategy)
{
  static const char* const names[] = {"GROUND_OUT",
                                      "LONE_VARIABLE",
				      "ALIENS_ONLY",
				      "GREEDY",
                                      "FULL"};
  s << names[strategy];
  return s;
}
#endif
