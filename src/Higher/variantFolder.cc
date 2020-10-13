/*

    This file is part of the Maude 3 interpreter.

    Copyright 1997-2020 SRI International, Menlo Park, CA 94025, USA.

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
  for (auto& i : mostGeneralSoFar)
    delete i.second;
}

void
VariantFolder::markReachableNodes()
{
  for (auto& i : mostGeneralSoFar)
    {
      for (auto& j : i.second->variant)
	j->mark();
    }
}

bool
VariantFolder::isSubsumed(const Vector<DagNode*>& variant) const
{
  for (auto& i : mostGeneralSoFar)
    {
      if (subsumes(i.second, variant))
	return true;  
    }
  return false;
}

bool
VariantFolder::insertVariant(const Vector<DagNode*>& variant,
			     int index,
			     int parentIndex,
			     int variableFamily)
{
  //cerr << " i" << index << "p" << parentIndex;
  //
  //	First we check if it is subsumed by one of the existing variants.
  //
  if (isSubsumed(variant))
    {
      DebugAdvisory("new variant subsumed");
      return false;
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
	  if (existingVariantsSubsumed.find(potentialVictim->parentIndex) !=
	      existingVariantsSubsumed.end())
	    {
	      //
	      //	Our parent was subsumed so we are also subsumed.
	      //
	      DebugAdvisory("new variant evicted descendent of an older variant " << i->first);
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
  newVariant->parentIndex = parentIndex;
  newVariant->variableFamily = variableFamily;
  newVariant->layerNumber = 0;
  if (parentIndex != NONE)
    {
      RetainedVariantMap::iterator parentVariant = mostGeneralSoFar.find(parentIndex);
      Assert(parentVariant != mostGeneralSoFar.end(), "parent " << parentIndex <<
	     " of variant " << index << " has been deleted");
      newVariant->layerNumber = parentVariant->second->layerNumber + 1;
    }

  mostGeneralSoFar.insert(RetainedVariantMap::value_type(index, newVariant));
  return true;
}

bool
VariantFolder::subsumes(const RetainedVariant* retainedVariant, const Vector<DagNode*>& variant) const
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
  int nrSlotsToAllocate = nrVariablesToUse;
  if (nrSlotsToAllocate == 0)
    nrSlotsToAllocate = 1;  // substitutions subject to clear() must always have at least one slot
  RewritingContext matcher(nrSlotsToAllocate);
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
int
VariantFolder::findNextSurvivingVariant()
{
  //
  //	Look for the next variant that has an index larger than the current varaint index.
  //
  currentVariant = mostGeneralSoFar.upper_bound(currentVariantIndex);
  if (currentVariant == mostGeneralSoFar.end())
    return NONE;  // no variants available so change nothing
  currentVariantIndex = currentVariant->first;
  return currentVariantIndex;
}

const Vector<DagNode*>&
VariantFolder::getCurrentVariant(int& nrFreeVariables,
				 int& variableFamily,
				 int* parentNumber,
				 bool* moreInLayer)
{
  Assert(mostGeneralSoFar.find(currentVariantIndex) == currentVariant, "current variant purged");

  nrFreeVariables = currentVariant->second->nrFreeVariables;
  variableFamily = currentVariant->second->variableFamily;
  //
  //    Optional information - non-null pointers means caller wants this information
  //    returned.
  //
  if (parentNumber)
    *parentNumber = currentVariant->second->parentIndex;  // variant's parent (or NONE if root)
  if (moreInLayer)
    {
      //
      //        Flag to indicate whether next call to findNextSurvivingVariant()
      //        will find another variant in the same layer.
      //
      RetainedVariantMap::const_iterator nextVariant =
	mostGeneralSoFar.upper_bound(currentVariantIndex);
      *moreInLayer = (nextVariant != mostGeneralSoFar.end() &&
		      nextVariant->second->layerNumber == currentVariant->second->layerNumber);
    }
  return currentVariant->second->variant;
}

void
VariantFolder::prepareForVariantMatching()
{
  //
  //	The VariableDagNodes in RetainedVariant::variant should have been indexed
  //	when we were looking for unifiers to do variant narrowing steps. But now we
  //	want to reindex them using the scheme that was used during the generation of
  //	RetainedVariant::matchingAutomata so that we can compute instantiations
  //	of them by matchers. Because the indexing schemes depend on the order the
  //	variant term and variant substitution were indexed as well as any peculiarities
  //	of indexing dags with NarrowingVariableInfo vs indexing terms with VariableInfo,
  //	we can't rely on the indexing schemes being identical.
  //
  for (auto& i : mostGeneralSoFar)
    {
      //
      //	We don't bother reindexing the term part of the variant,
      //	just the substitution.
      //
      int nrBindings = i.second->variant.size() - 1;
      for (int j = 0; j < nrBindings; ++j)
	i.second->variant[j]->indexVariables(i.second->variableInfo);
    }
}

const Vector<DagNode*>*
VariantFolder::findNextVariantThatMatches(int& indexOfLastUsedVariant,
					  DagNode* subject,
					  const VariableInfo*& variableInfo,
					  RewritingContext*& matcher,
					  Subproblem*& subproblem)
{
  for (RetainedVariantMap::const_iterator i =
	 mostGeneralSoFar.upper_bound(indexOfLastUsedVariant); i != mostGeneralSoFar.end(); ++i)
    {
      const RetainedVariant* retainedVariant = i->second;
      int termPartIndex = retainedVariant->matchingAutomata.size() - 1;
      //
      //	We use the number of variables in the whole variant, even
      //	though we're only going to match the term part. In particular there
      //	may exist variables in the bindings of the substitution part that
      //	don't appear in the term part (because of nonregular equations) and
      //	that won't be bound by this matchings.
      //
      int nrVariablesToUse = retainedVariant->nrVariables;
      int nrSlotsToAllocate = nrVariablesToUse;
      if (nrSlotsToAllocate == 0)
	nrSlotsToAllocate = 1;  // substitutions subject to clear() must always have at least one slot
      matcher = new RewritingContext(nrSlotsToAllocate);
      matcher->clear(nrVariablesToUse);
      //
      //
      //
      if (retainedVariant->matchingAutomata[termPartIndex]->match(subject, *matcher, subproblem))
	{
	  indexOfLastUsedVariant = i->first;
	  variableInfo = &(retainedVariant->variableInfo);
	  //
	  //	Responsibility for deleting matcher and any subproblem generated
	  //	is passed to caller.
	  //
	  return &(retainedVariant->variant);
	}
      delete matcher;
      matcher = 0;
    }
  return 0;
}

VariantFolder::RetainedVariant::RetainedVariant(const Vector<DagNode*> original)
  : variant(original),
    terms(original.size()),
    matchingAutomata(original.size())
{
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
	    {
	      //
	      //	Variables from other terms are in our context
	      //
	      t->addContextVariables(terms[j]->occursBelow());
	    }
	}
      //
      //	Recursively compute the context variables for each subterm.
      //
      t->determineContextVariables();
      //
      //	Insert abstraction variables for subterms that could collapse into
      //	their enclosing theory; such subterms will need to be treated like
      //	variables and then their binding subject to an extra match.
      //
      t->insertAbstractionVariables(variableInfo);
    }
  //
  //	This may also include some abstraction variables.
  //
  nrVariables = variableInfo.getNrProtectedVariables();
  //
  //	Now we know how many variables we will need to save in branching, we
  //	can safely compile.
  //
  NatSet boundUniquely;
  bool subproblemLikely;
  for (int i = nrDags - 1; i >= 0; --i)
    {
      Term* t = terms[i];
      DebugAdvisory("Compiling " << t);
      matchingAutomata[i] = t->compileLhs(false, variableInfo, boundUniquely, subproblemLikely);
      //matchingAutomata[i]->dump(cerr, variableInfo);
    }
  DebugInfo("compiled retained variant has " << variableInfo.getNrRealVariables() <<
	    " real variables");
  DebugInfo("compiled retained variant has " << variableInfo.getNrProtectedVariables() <<
	    " protected variables");
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
