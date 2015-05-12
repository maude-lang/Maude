/*

    This file is part of the Maude 2 interpreter.

    Copyright 1997-2012 SRI International, Menlo Park, CA 94025, USA.

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
//	Implementation for class VariantFolder.
//

//	utility stuff
#include "macros.hh"
#include "vector.hh"

//	forward declarations
#include "interface.hh"
#include "core.hh"
//#include "variable.hh"

//	interface class definitions
#include "symbol.hh"
#include "dagNode.hh"
#include "term.hh"
#include "lhsAutomaton.hh"
#include "subproblem.hh"

//	core class definitions
#include "rewritingContext.hh"
#include "subproblemAccumulator.hh"
#include "variableInfo.hh"

//	higher class definitions
#include "variantFolder.hh"

VariantFolder::VariantFolder()
{
  currentVariantIndex = -1;
}

VariantFolder::~VariantFolder()
{
  FOR_EACH_CONST(i, RetainedVariantMap, mostGeneralSoFar)
    delete i->second;
}

void
VariantFolder::markReachableNodes()
{
  FOR_EACH_CONST(i, RetainedVariantMap, mostGeneralSoFar)
    {
      Vector<DagNode*>& variant = i->second->variant;
      FOR_EACH_CONST(j, Vector<DagNode*>, variant)
	(*j)->mark();
    }
}

bool
VariantFolder::insertVariant(const Vector<DagNode*>& variant, int index, int parentIndex)
{
  //cerr << " i" << index << "p" << parentIndex;
  //
  //	First we check if it is subsumed by one of the existing variants.
  //
  FOR_EACH_CONST(i, RetainedVariantMap, mostGeneralSoFar)
    {
      if (subsumes(i->second, variant))
	{
	  DebugAdvisory("new variant subsumed");
	  //cout << "!!!!!!!" << index << "subsumed by " << i->first << endl;
	  return false;
	}
    }
  DebugAdvisory("new variant added");
  //
  //	Compile a set of matching automata for this variant.
  //
  RetainedVariant* newVariant = new RetainedVariant(variant);
  //
  //	Compute ancestor set.
  //
  set<int> ancestors;
  for (int i = parentIndex; i != NONE; )
    {
      ancestors.insert(i);
      RetainedVariantMap::const_iterator j = mostGeneralSoFar.find(i);
      Assert(j != mostGeneralSoFar.end(), "couldn't find variant with index " << i);
      i = j->second->parentIndex;
    }
  //
  //	See if this variant can evict an existing variant.
  //
  set<int> existingVariantsSubsumed;
  RetainedVariantMap::iterator i = mostGeneralSoFar.begin();
  while (i != mostGeneralSoFar.end())
    {
      RetainedVariantMap::iterator next = i;
      ++next;

      if (ancestors.find(i->first) == ancestors.end())  // can't mess with ancestors of new variant
	{
	  RetainedVariant* potentialVictim = i->second;
	  if (existingVariantsSubsumed.find(potentialVictim->parentIndex) != existingVariantsSubsumed.end())
	    {
	      //
	      //	Our parent was subsumed so we are also subsumed.
	      //
	      DebugAdvisory("new variant evicted descendent of an older variant " << i->first);
	      /*
	      cerr << "!!!!!!! new variant " << index << endl;
	      FOR_EACH_CONST(k, Vector<DagNode*>, variant)
		cerr << *k << endl;
	      cerr << " evicted " << i->first << endl;
	      potentialVictim->dump();
	      cerr << " via parent " << potentialVictim->parentIndex << endl;
	      */
	      existingVariantsSubsumed.insert(i->first);
	      delete potentialVictim;
	      mostGeneralSoFar.erase(i);
	    }
	  else if (subsumes(newVariant, potentialVictim->variant))
	    {
	      //
	      //	Direct subsumption by new variant.
	      //
	      DebugAdvisory("new variant evicted an older variant " << i->first);
	      /*
	      cerr << "!!!!!!! new variant " << index << endl;
	      FOR_EACH_CONST(k, Vector<DagNode*>, variant)
		cerr << *k << endl;
	      cerr << " evicted " << i->first << endl;
	      potentialVictim->dump();
	      */
	      existingVariantsSubsumed.insert(i->first);
	      delete potentialVictim;
	      mostGeneralSoFar.erase(i);
	    }
	}

      i = next;
    }
  //
  //	Add to the mostGeneralSoFar collection of variants.
  //
  //cerr << "*";
  newVariant->parentIndex = parentIndex;
  mostGeneralSoFar.insert(RetainedVariantMap::value_type(index, newVariant));
  return true;
}

const Vector<DagNode*>* 
VariantFolder::getNextSurvivingVariant(int& nrFreeVariables)
{
  //
  //	We allow variants to be extracted, even though we may not be finished inserting new variants.
  //	This means that some of the variant we return may later be evicted by a subsequent insert().
  //
  RetainedVariantMap::const_iterator nextVariant = mostGeneralSoFar.upper_bound(currentVariantIndex);
  if (nextVariant == mostGeneralSoFar.end())
    return 0;  // no variants available so change nothing

  currentVariantIndex = nextVariant->first;
  nrFreeVariables = nextVariant->second->nrFreeVariables;
  return &(nextVariant->second->variant);
}

bool
VariantFolder::subsumes(const RetainedVariant* retainedVariant, const Vector<DagNode*>& variant)
{
  int nrDagsToCheck = variant.size();
  int nrDagsInRetainedVariant = retainedVariant->matchingAutomata.size();
  if (nrDagsToCheck != nrDagsInRetainedVariant)
    return false;  // different sized variants are trivially incomparable

  MemoryCell::okToCollectGarbage();  // otherwise we have huge accumulation of junk from matching
  //
  //	We check if retained variant is at least as general as a new variant.
  //
  int nrVariablesToUse = retainedVariant->nrVariables;
  if (nrVariablesToUse == 0)
    nrVariablesToUse = 1;  // substitutions always expect to have at least one variable
  RewritingContext matcher(nrVariablesToUse);
  matcher.clear(nrVariablesToUse);
  SubproblemAccumulator subproblems;

  for (int i = nrDagsToCheck - 1; i >= 0; --i)
    {
      Subproblem* subproblem;

      DebugAdvisory("Considering dag " << i);
      DebugAdvisory("Pattern has " << retainedVariant->variant[i]);
      DebugAdvisory("Pattern term is " << retainedVariant->terms[i]);
      DebugAdvisory("Subject has " << variant[i]);

      if (retainedVariant->matchingAutomata[i]->match(variant[i], matcher, subproblem))
	subproblems.add(subproblem);
      else
	{
	  DebugAdvisory("non subsumption declared on dag " << i);
	  return false;
	}
    }
  Subproblem* final = subproblems.extractSubproblem();
  if (final == 0)
    {
      DebugAdvisory("no subproblem so declaring subsumption");
      return true;
    }
  bool result = final->solve(true, matcher);
  DebugAdvisory("solve result = " << result);
  delete final;
  return result;
}

VariantFolder::RetainedVariant::RetainedVariant(const Vector<DagNode*> original)
  : variant(original),
    terms(original.size()),
    matchingAutomata(original.size())
{
  VariableInfo variableInfo;  // does this need to be retained?
  
  int nrDags = original.size();
  for (int i = 0; i < nrDags; ++i)
    {
      DagNode* d = original[i];
      Term* t = d->symbol()->termify(d);
      t = t->normalize(true);  // needed even though we should have a normal form, in order to set hash value
      t->indexVariables(variableInfo); 
      t->symbol()->fillInSortInfo(t);
      t->analyseCollapses();
      terms[i] = t;
    }

  nrFreeVariables = variableInfo.getNrRealVariables();

  NatSet boundUniquely;
  bool subproblemLikely;
  //
  //	Variant dags are compiled and matched in reverse order because the term part of the variant
  //	will be at the end, and it is most likely to cause early match failure.
  //
  for (int i = nrDags - 1; i >= 0; --i)
    {
      Term* t = terms[i];
      //
      //	Accumuate the context variables for this term.
      //
      for (int j = 0; j < nrDags; ++j)
	{
	  if (j != i)
	    t->addContextVariables(terms[j]->occursBelow());  // variables from other terms are in our context
	}
      t->determineContextVariables();
      t->insertAbstractionVariables(variableInfo);
      
      DebugAdvisory("Compiling " << t);
      matchingAutomata[i] = t->compileLhs(false, variableInfo, boundUniquely, subproblemLikely);
      //matchingAutomata[i]->dump(cerr, variableInfo);
    }

  nrVariables = variableInfo.getNrRealVariables();  // may also have some abstraction variables
}

VariantFolder::RetainedVariant::~RetainedVariant()
{
  int nrTerms = terms.size();
  for (int i = 0; i < nrTerms; ++i)
    {
      delete matchingAutomata[i];
      terms[i]->deepSelfDestruct();
    }
}

void
VariantFolder::RetainedVariant::dump()
{
  int nrTerms = terms.size();
  for (int i = 0; i < nrTerms; ++i)
    {
      cerr << terms[i] << endl;
    }
}
