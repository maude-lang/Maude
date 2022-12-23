/*

    This file is part of the Maude 3 interpreter.

    Copyright 2022 SRI International, Menlo Park, CA 94025, USA.

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
//	Implementation for class VariantUnifierFilter.
//

//	utility stuff
#include "macros.hh"
#include "vector.hh"

//	forward declarations
#include "interface.hh"
#include "core.hh"
#include "variable.hh"
#include "higher.hh"

//	interface class definitions
#include "symbol.hh"
#include "dagNode.hh"
#include "term.hh"
#include "lhsAutomaton.hh"
#include "subproblem.hh"

//	core class definitions
#include "module.hh"
#include "sort.hh"
#include "connectedComponent.hh"
#include "rewritingContext.hh"
#include "subproblemAccumulator.hh"
#include "variableInfo.hh"
#include "freshVariableGenerator.hh"
#include "dagArgumentIterator.hh"

//	variable class definitions
#include "variableDagNode.hh"

//	higher class definitions
#include "variantSearch.hh"
#include "variantUnifierFilter.hh"

VariantUnifierFilter::VariantUnifierFilter(RewritingContext* context,
					   FreshVariableGenerator* freshVariableGenerator)
  : context(context),
    freshVariableGenerator(freshVariableGenerator)
{
  tupleSymbol = 0;
  startedExtractingUnifiers = false;
  currentUnifierSubsumed = false;
  filteringIncompleteFlag = false;
  currentUnifier = mostGeneralSoFar.end();  // will never compare equal to an actual unifier entry
}

VariantUnifierFilter::~VariantUnifierFilter()
{
  for (RetainedUnifier* i : mostGeneralSoFar)
    delete i;
}

bool
VariantUnifierFilter::findNextSurvivingUnifier()
{
  if (startedExtractingUnifiers)
    {
      RetainedUnifierList::const_iterator nextUnifier = currentUnifier;
      ++nextUnifier;
      if (nextUnifier == mostGeneralSoFar.end())
	return false;  // no unifier past the last one we returned
      if (currentUnifierSubsumed)
	{
	  //
	  //	Current unifier was subsumed, but we didn't delete it,
	  //	because it was the current unifier. So we delete it now.
	  //
	  delete *currentUnifier;
	  mostGeneralSoFar.erase(currentUnifier);
	  currentUnifierSubsumed = false;
	}
      currentUnifier = nextUnifier;
      return true;
    }
  if (mostGeneralSoFar.empty())
    return false;  // can't start extracting unifiers
  
  currentUnifier = mostGeneralSoFar.begin();
  startedExtractingUnifiers = true;
  return true;
}

void
VariantUnifierFilter::insertUnifier(const Vector<DagNode*>& unifier,
				    int nrFreeVariables,
				    int variableFamily)
{
  if (tupleSymbol == 0)
    {
      int nrAssignments = unifier.size();  // could be zero!
      Vector<ConnectedComponent*> domain(nrAssignments);
      for (int i = 0; i < nrAssignments; ++i)
	domain[i] = unifier[i]->symbol()->rangeComponent();
      ConnectedComponent* range = context->root()->symbol()->rangeComponent();  // really connected component will do
      Module* module = range->getModule();
      tupleSymbol = module->createInternalTupleSymbol(domain, range);
    }
  //
  //	We need this to protect the encoded unifier during subsumption checks
  //	because solve phase of matching can call the garbage collector and the
  //	encoded unifier isn't protected by a context until is has survived the
  //	subsumption checks.
  //
  encodedUnifier.setNode(tupleSymbol->makeDagNode(unifier));
  for (RetainedUnifierList::iterator i = mostGeneralSoFar.begin(); i != mostGeneralSoFar.end(); ++i)
    {
      //
      //	If the current unifier has been subsumed, we don't bother with it.
      //
      if (!(currentUnifierSubsumed && i == currentUnifier) && (*i)->subsumes(encodedUnifier.getNode()))
	{
	  encodedUnifier.setNode(0);
	  return;
	}
    }
  //
  //	Not subsumed by any existing variant unifier - need to make a set of
  //	most general variants for it.
  //
  RewritingContext* newContext = context->makeSubcontext(encodedUnifier.getNode());
  Vector<DagNode*> dummy;  // subsumption mode can't use blocker dags because there is no substitution
  VariantSearch* variants = new VariantSearch(newContext,
					      dummy,
					      freshVariableGenerator,
					      VariantSearch::SUBSUMPTION_MODE,
					      variableFamily);
  context->addInCount(*newContext);
  if (variants->isIncomplete())
    filteringIncompleteFlag = true;
  RetainedUnifier* newUnifier = new RetainedUnifier(unifier,
						    nrFreeVariables,
						    variableFamily,
						    encodedUnifier.getNode(),
						    variants);
  encodedUnifier.setNode(0);  // now protected by its context
  //
  //	See if this variant unifier can evict an existing unifier.
  //
  RetainedUnifierList::iterator i = mostGeneralSoFar.begin();
  while (i != mostGeneralSoFar.end())
    {
      RetainedUnifierList::iterator next = i;
      ++next;
      if (!(currentUnifierSubsumed && i == currentUnifier))
	{
	  RetainedUnifier* potentialVictim = *i;
	  if (newUnifier->subsumes(potentialVictim->encodedUnifier))
	    {
	      DebugInfo("new variant unifier evicted an older variant unifier");
	      if (i == currentUnifier)
		{
		  //
		  //	We don't delete the current unifier. This allows new unifiers
		  //	to be inserted even as we extract unifiers.
		  //
		  currentUnifierSubsumed = true;
		}
	      else
		{
		  delete potentialVictim;
		  mostGeneralSoFar.erase(i);
		}
	    }
	}
      i = next;
    }
  //
  //	Add to the mostGeneralSoFar collection of unifiers.
  //
  mostGeneralSoFar.push_back(newUnifier);
}
