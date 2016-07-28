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
//	Implementation for class VariantSearch.
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
#include "rewritingContext.hh"
#include "subproblemAccumulator.hh"
#include "variableInfo.hh"
#include "freshVariableGenerator.hh"
#include "dagArgumentIterator.hh"

//	variable class definitions
#include "variableDagNode.hh"

//	higher class definitions
#include "variantSearch.hh"
#include "variantNarrowingSearchState.hh"

VariantSearch::VariantSearch(RewritingContext* context,
			     const Vector<DagNode*>& blockerDags,
			     FreshVariableGenerator* freshVariableGenerator,
			     bool unificationMode,
			     bool irredundantMode)
  : context(context),
    blockerDags(blockerDags),  // shallow copy
    freshVariableGenerator(freshVariableGenerator),
    unificationMode(unificationMode)
{
  incompleteFlag = false;
  nrVariantsReturned = 0;  // we only track this is variant mode
  //
  //	Index variables in initial dag. We don't want to do reduction on initial dag for two reasons:
  //	(1) We rely on variable dags not going away to protect variable dags in variableInfo from GC.
  //	(2) Rewriting could introduce new ground terms that don't have their ground flag set, breaking instantiation.
  //
  context->root()->indexVariables(variableInfo, 0);
  //
  //	Check that variable names don't clash with the range we're going to use for fresh variables.
  //
  nrVariantVariables = variableInfo.getNrVariables();
  for (int i = 0; i < nrVariantVariables; ++i)
    {
      VariableDagNode* v = variableInfo.index2Variable(i);
      if (freshVariableGenerator->variableNameConflict(v->id()))
	{
	  DagNode* d = v;
	  IssueWarning("unsafe variable name " << QUOTE(d) << " in variant " <<
		       (unificationMode ? "unification" : "narrowing") << " problem.");
	  return;
	}
    }
  {
    //
    //	Index and normalize and compute sorts for blocker dags. They could conceivable have previously unseen variables,
    //	but we don't care about the actual names of such variables since they will only be used in the subject side of
    //	matching and will behave as fresh constants.
    //	If one of the blocker dags is already reducible, we count that as an immediate fail.
    //
    FOR_EACH_CONST(i, Vector<DagNode*>, blockerDags)
      {
	DagNode* d = *i;
	d->indexVariables(variableInfo, 0);
	d->computeTrueSort(*context);  // also handles theory normalization
	if (d->reducibleByVariantEquation(*context))
	  {
	    IssueAdvisory("Irreducibility constraint " << d << " is itself reducible by a variant equation.");
	    return;
	  }
      }
  }
  //
  //	Allocate a fresh even variable for each original variable.
  //
  protectedVariant.resize(nrVariantVariables);
  Substitution s(nrVariantVariables);
  for (int i = 0; i < nrVariantVariables; ++i)
    {
      Sort* sort = safeCast(VariableSymbol*, variableInfo.index2Variable(i)->symbol())->getSort();
      VariableDagNode* v = new VariableDagNode(freshVariableGenerator->getBaseVariableSymbol(sort),
					       freshVariableGenerator->getFreshVariableName(i, false),
					       i);
      protectedVariant[i] = v;
      s.bind(i, v);
    }
  //
  //	Make a copy of the initial dag with the variables replaced by fresh ones.
  //
  DagNode* newDag = context->root()->instantiate(s);  // not safe if we haven't determined ground terms in context->root()
  if (newDag == 0)
    newDag = context->root();
  //
  //	Now we can safely reduce newDag - we have replaced all the variables and
  //	we no longer care about in place rewriting on ground terms.
  //
  RewritingContext* redContext = context->makeSubcontext(newDag);
  redContext->reduce();

  DagNode* r = redContext->root();
  if (unificationMode)
    {
      //
      //	Check for the identity unification problem, t =? t
      //
      DagArgumentIterator a(r);
      Assert(a.valid(), "bad 1st argument in unification mode");
      DagNode* lhs = a.argument();
      a.next();
      Assert(a.valid(), "bad 2nd argument in unification mode");
      if (lhs->equal(a.argument()))
	{
	  variantCollection.insertVariant(protectedVariant, 0, NONE);
	  protectedVariant.clear();  // remove GC protection
	  context->addInCount(*redContext);
	  //context->incrementEqCount();  // notional equational rewrite to true
	  delete redContext;
	  return;  // no point in looking for other unifiers
	}
    }
  protectedVariant.append(r);
  context->addInCount(*redContext);
  delete redContext;
  //
  //	Insert this initial variant in to collection, and initialize variables for search.
  //
  variantCollection.insertVariant(protectedVariant, 0, NONE);
  protectedVariant.clear();  // remove GC protection
  frontier.append(0);
  currentIndex = 1;
  //
  //	Breadthfirst search for new variants. Variants indexed by the frontier can disappear if they become covered by
  //	later variants, or were descendents of variants that became covered.
  //
  odd = true;
  if (irredundantMode)
    {
      //
      //	Since later variants can cover earlier variants on different branches (not ancestors)
      //	if we want to avoid returning redundant variants, we have to generate them all up front.
      //	This is less useful for variant unification, because even without redundant variants
      //	we still expect redundant unifiers.
      //
      do
	expandLayer();
      while (!(frontier.empty()));
    }
}

VariantSearch::~VariantSearch()
{
  delete freshVariableGenerator;
  delete context;
}

void
VariantSearch::markReachableNodes()
{
  //
  //	We don't mark the variable dag nodes in variableInfo - we rely on these existing in the
  //	original dag protected by the original context.
  //
  int substSize = protectedVariant.size();
  for (int i = 0; i < substSize; ++i)
    protectedVariant[i]->mark();
  FOR_EACH_CONST(i, Vector<DagNode*>, blockerDags)
    (*i)->mark();
}

const Vector<DagNode*>*
VariantSearch::getNextVariant(int& nrFreeVariables, int& parentIndex, bool& moreInLayer)
{
  if (context->traceAbort())
    return 0;

  int variantNumber;
  int parentNumber;

  const Vector<DagNode*>* v =
    variantCollection.getNextSurvivingVariant(nrFreeVariables, &variantNumber, &parentNumber, &moreInLayer);
  if (v == 0 && !(frontier.empty()))
    {
      //
      //	Must be in incremental mode - try expanding current frontier.
      //
      expandLayer();
      v = variantCollection.getNextSurvivingVariant(nrFreeVariables, &variantNumber, &parentNumber, &moreInLayer);
    }
  if (v != 0)
    {
      //
      //	We found a variant - need to keep track a mapping from its internal index
      //	to its external index to we can convert the internal parent index of
      //	its future children.
      //
      internalIndexToExternalIndex.insert(IntMap::value_type(variantNumber, nrVariantsReturned));
      ++nrVariantsReturned;
      parentIndex = (parentNumber == NONE) ? NONE : internalIndexToExternalIndex[parentNumber];
    }
  return v;
}

const Vector<DagNode*>*
VariantSearch::getLastReturnedVariant(int& nrFreeVariables, int& parentIndex, bool& moreInLayer)
{
  int parentNumber;
  const Vector<DagNode*>* v = variantCollection.getLastReturnedVariant(nrFreeVariables, &parentNumber, &moreInLayer);
  Assert(v != 0, "shouldn't be asked for last returned variant, if last call didn't return a variant");
  parentIndex = (parentNumber == NONE) ? NONE : internalIndexToExternalIndex[parentNumber];
  return v;
}

const Vector<DagNode*>*
VariantSearch::getNextUnifier(int& nrFreeVariables)
{
  while (!(context->traceAbort()))
    {
      const Vector<DagNode*>* v = variantCollection.getNextSurvivingVariant(nrFreeVariables);
      if (v == 0)
	{
	  if (frontier.empty())
	    break;
	  //
	  //	Must be in incremental mode - try exanding current frontier.
	  //
	  expandLayer();
	  v = variantCollection.getNextSurvivingVariant(nrFreeVariables);
	  if (v == 0)
	    break;  // no new variants immediately following a expandLayer() means we're done
	}
      int variantSize = v->size();
      if (variantSize == nrVariantVariables)
	return v;
      Assert(variantSize == nrVariantVariables + 1, "non unifier variant should have an extra dag");
    }
  return 0;
}

void
VariantSearch::expandLayer()
{
  //
  //	Look at each variant in the current frontier, and if it
  //	still exists, expand it by one step variant narrowing.
  //
  FOR_EACH_CONST(i, VariantIndexVec, frontier)
    {
      int index = *i;
      const Vector<DagNode*>* variant = variantCollection.getVariant(index);
      if (variant != 0)
	{
	  expandVariant(*variant, index);
	  if (context->traceAbort())
	    {
	      frontier.clear();
	      newFrontier.clear();
	      return;
	    }
	}
    }
  frontier.swap(newFrontier);
  newFrontier.clear();
  odd = !odd;
}

void
VariantSearch::expandVariant(const Vector<DagNode*>& variant, int index)
{
  //
  //	The last member of variant is the variant term and not part of the variant substitution.
  //
  int substSize = variant.size() - 1;
  RewritingContext* newContext = context->makeSubcontext(variant[substSize]);
  //
  //	We don't need to protect the DagNodes in this local copy since the are already protected
  //	by the VariantCollection.
  //
  Vector<DagNode*> variantSubstitution(substSize);
  for (int i = 0; i < substSize; ++i)
    variantSubstitution[i] = variant[i];
  //
  //	Create a search state for one step variant narrowings. We pass variableInfo because original
  //	variable names are needed for tracing, and total number of variables is needed for unification.
  //
  VariantNarrowingSearchState vnss(newContext, variantSubstitution, blockerDags, freshVariableGenerator, odd, variableInfo, unificationMode);
  //
  //	Extract each new variant.
  //
  DagNode* variantTerm;
  Vector<DagNode*> newVariantSubstitution;
  while (vnss.findNextVariant(variantTerm, newVariantSubstitution))
    {
      //
      //	Deep copy to protect variant substitution from GC during the folding and reduction.
      //
      protectedVariant = newVariantSubstitution;
      //
      //	Allocate a unique index for the new variant.
      //
      int newIndex = currentIndex;
      ++currentIndex;
      //
      //	Check to see if we generated a unifier.
      //
      if (variantTerm == 0)
	{
	  Assert(unificationMode, "null variant term and we're not in unification mode");
	  variantCollection.insertVariant(protectedVariant, newIndex, index);
	}
      else
	{
	  //
	  //	Reduce dag part of new variant, and append it the the substitution.
	  //
	  RewritingContext* redContext = context->makeSubcontext(variantTerm);
	  redContext->reduce();
	  protectedVariant.append(redContext->root());
	  //
	  //	Insert new variant in to collection and if it sticks, insert its index into the new frontier.
	  //
	  if (variantCollection.insertVariant(protectedVariant, newIndex, index))
	    newFrontier.append(newIndex);
	  //
	  //	Move rewrite count from reduction context to original context.
	  //
	  context->addInCount(*redContext);
	  delete redContext;
	}
      //
      //	Removed protection from variant substitution.
      //
      protectedVariant.clear();
    }
  incompleteFlag |= vnss.isIncomplete();
  //
  //	Move rewrite count from narrowing context to original context.
  //
  context->addInCount(*newContext);
  delete newContext;
}

#ifdef DUMP
void
VariantSearch::dumpVariant(const Vector<DagNode*>& variant, int index,  int parentIndex)
{
  int nrVariables = variant.size() - 1;
  cout << "\nInserted variant\n";

  cout << "index = " << index << endl;
  cout << "parentIndex = " << parentIndex << endl;
  cout << "term = " << variant[nrVariables] << endl;

  for (int i = 0; i < nrVariables; ++i)
    {
      DagNode* v = variableInfo.index2Variable(i);
      cout << v << " |-> " << variant[i] << endl;
    }
  cout << endl;
}
#endif
