/*

    This file is part of the Maude 3 interpreter.

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
#include "freshVariableGenerator.hh"
#include "dagArgumentIterator.hh"

//	variable class definitions
#include "variableDagNode.hh"

//	higher class definitions
#include "variantSearch.hh"
#include "variantNarrowingSearchState.hh"
#include "variantMatchingProblem.hh"

bool
VariantSearch::findNextUnifier()
{
  //
  //	We need to find a surviving variant that is the right length.
  //
  while (!(context->traceAbort()))
    {
      if (variantCollection.findNextSurvivingVariant() == NONE)
	{
	  if (frontier.empty())
	    break;
	  //
	  //	Must be in incremental mode - try exanding current frontier.
	  //
	  expandLayer();
	  if (variantCollection.findNextSurvivingVariant() == NONE)
	    break;  // no new variants immediately following a expandLayer() means we're done
	}
      if (variantCollection.getCurrentVariantSize() == nrVariantVariables)
	return true;
    }
  return false;
}

const Vector<DagNode*>&
VariantSearch::getCurrentUnifier(int& nrFreeVariables, int& variableFamily)
{
  return variantCollection.getCurrentVariant(nrFreeVariables, variableFamily);
}

bool
VariantSearch::findNextVariant()
{
  //
  //	We need to find a surviving variant.
  //
  if (context->traceAbort())
    return false;
  int variantNumber = variantCollection.findNextSurvivingVariant();
  if (variantNumber == NONE)
    {
      if (frontier.empty())
	return false;
      //
      //	Must be in incremental mode - try exanding current frontier.
      //
      expandLayer();
      variantNumber = variantCollection.findNextSurvivingVariant();
      if (variantNumber == NONE)
	return false;  // no new variants immediately following a expandLayer() means we're done
    }
  //
  //	We build a mapping from internal variant indices to the indices of surviving
  //	variants so we can translate parent indices from the internal numbering used for
  //	folding variants to the external numbering used at the metalevel.
  //
  internalIndexToExternalIndex.insert(IntMap::value_type(variantNumber, nrVariantsFound));
  ++nrVariantsFound;
  return true;
}

VariantSearch::VariantSearch(RewritingContext* context,
			     const Vector<DagNode*>& blockerDags,
			     FreshVariableGenerator* freshVariableGenerator,
			     int flags,
			     int incomingVariableFamily)
  : context(context),
    blockerDags(blockerDags),  // shallow copy
    freshVariableGenerator(freshVariableGenerator),
    flags(flags),
    firstVariableFamily((incomingVariableFamily == 0) ? 1 : 0),
    secondVariableFamily((incomingVariableFamily == 2 || incomingVariableFamily == NONE) ? 1 : 2)
{
  problemOkay = false;  // until we have verified it is ok
  incompleteFlag = false;
  nrVariantsFound = 0;  // we only track this in variant mode
  lastVariantMatchingProblem = 0;  // we only use this in matching mode
  //
  //	We make a clean copy of the initial dag for several reasons:
  //	(1) If the variables have aready been indexed by a parent computation, we don't
  //	want to overwrite this indexing.
  //	(2) We don't want to reduce any ground dags that may be lying around.
  //
  targetCopy = context->root()->copyAll();
  context->root()->clearCopyPointers();
  //
  //	Index variables in target dag. We don't want to do reduction on target dag for two reasons:
  //	(1) We rely on variable dags not going away to protect variable dags in variableInfo from GC.
  //	(2) Rewriting could introduce new ground terms that don't have their ground flag set,
  //	breaking instantiation.
  //
  targetCopy->indexVariables(variableInfo, 0);
  nrVariantVariables = variableInfo.getNrVariables();
  if (flags & CHECK_VARIABLE_NAMES)
    {
      //
      //	Check that variable names don't clash with the range we're going to use for
      //	fresh variables. If we are being called under system controlled circumstances
      //	to do variant unfication, variable clashes are avoided using multiple variable
      //	families and this check could cause false errors if the number of variables in
      //	place gets larger that freshVariableGenerator base.
      //
      for (int i = 0; i < nrVariantVariables; ++i)
	{
	  VariableDagNode* v = variableInfo.index2Variable(i);
	  if (freshVariableGenerator->variableNameConflict(v->id(), incomingVariableFamily))
	    {
	      DagNode* d = v;
	      IssueWarning("unsafe variable name " << QUOTE(d) << " in variant " <<
			   ((flags & UNIFICATION_MODE) ? "unification" :
			    ((flags & MATCH_MODE) ? "matching" : "narrowing")) << " problem.");
	      return;
	    }
	}
    }
  {
    //
    //	Index and normalize and compute sorts for blocker dags. They could conceivable have
    //	previously unseen variables, but we don't care about the actual names of such variables
    //	since they will only be used in the subject side of matching and will behave as fresh
    //	constants. If one of the blocker dags is already reducible, we count that as an
    //	immediate fail.
    //
    for (DagNode* d : blockerDags)
      {
	d->indexVariables(variableInfo, 0);
	d->computeTrueSort(*context);  // also handles theory normalization
	if (d->reducibleByVariantEquation(*context))
	  {
	    IssueAdvisory("Irreducibility constraint " << d <<
			  " is itself reducible by a variant equation.");
	    return;
	  }
      }
    if (flags & CHECK_VARIABLE_NAMES)
      {
	int nrVariables = variableInfo.getNrVariables();
	for (int i = nrVariantVariables; i < nrVariables; ++i)
	  {
	    VariableDagNode* v = variableInfo.index2Variable(i);
	    if (freshVariableGenerator->variableNameConflict(v->id()))
	      {
		DagNode* d = v;
		IssueWarning("unsafe variable name " << QUOTE(d) << " in irreducibility constraint.");
		return;
	      }
	  }
      }
  }
  problemOkay = true;
  //
  //	Allocate a fresh firstVariableFamily variable for each original variable.
  //
  protectedVariant.resize(nrVariantVariables);
  Substitution s(nrVariantVariables);
  for (int i = 0; i < nrVariantVariables; ++i)
    {
      Symbol* vs = variableInfo.index2Variable(i)->symbol();
      VariableDagNode* v =
	new VariableDagNode(vs,
			    freshVariableGenerator->getFreshVariableName(i, firstVariableFamily),
			    i);
      protectedVariant[i] = v;
      s.bind(i, v);
    }
  //
  //	Make a copy of the initial dag with the variables replaced by fresh ones.
  //
  DagNode* newDag = targetCopy->instantiate(s);  // indexVariables will have marked any ground dags
  if (newDag == 0)
    newDag = targetCopy;
  //
  //	Now we can safely reduce newDag - we have replaced all the variables and
  //	we no longer care about in place rewriting on ground terms.
  //
  RewritingContext* redContext = context->makeSubcontext(newDag);
  //
  //	Note that reduce() does in-place replacement so subterms marked as ground
  //	could potentially have their own subterms replaced with subterms containing
  //	variables. Thus we can no longer trust the ground flags and indeed
  //	  DagNode::indexVariables(NarrowingVariableInfo& indices, int baseIndex)
  //	which reindexes and sets ground flags prior to narrowing steps makes
  //	a point of distrusting existing ground flags.
  //
  redContext->reduce();

  DagNode* r = redContext->root();
  if (flags & UNIFICATION_MODE)
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
	  //
	  //	We pretend to do a notional narrowing step to true that yields
	  //	the trivial unifier.
	  //
	  variantCollection.insertVariant(protectedVariant, 0, NONE, firstVariableFamily);
	  protectedVariant.clear();  // remove GC protection
	  context->addInCount(*redContext);
	  //context->incrementEqCount();  // notional equational rewrite to true
	  delete redContext;
	  return;  // no point in looking for other unifiers
	}
    }
  if (flags & SUBSUMPTION_MODE)
    {
      //
      //	In subsumption mode we only care about the variant term; we don't
      //	need to drag the variant substitution around, especially since
      //	distinct variants could have the same variant term but different
      //	variants substitutions.
      //
      protectedVariant.clear();
    }
  protectedVariant.append(r);
  context->addInCount(*redContext);
  delete redContext;
  //
  //	Insert this initial variant in to collection, and initialize variables for search.
  //
  variantCollection.insertVariant(protectedVariant, 0, NONE, firstVariableFamily);
  protectedVariant.clear();  // remove GC protection
  frontier.append(0);
  currentIndex = 1;
  //
  //	Breadthfirst search for new variants. Variants indexed by the frontier can
  //	disappear if they become covered by later variants, or were descendents of
  //	variants that became covered.
  //
  useFirstVariableFamily = false;
  if (flags & (IRREDUNDANT_MODE | SUBSUMPTION_MODE | MATCH_MODE))
    {
      //
      //	Since later variants can cover earlier variants on different branches
      //	(not ancestors) if we want to avoid returning redundant variants, we
      //	have to generate them all up front. This is less useful for variant
      //	unification, because even without redundant variants we still expect
      //	redundant unifiers, because variant subsumption is modulo axioms not
      //	axioms plus variant equations.
      //
      //	If we're in subsumption mode, then we are using variant narrowing to
      //	compute subsumption modulo axioms plus variants for some larger
      //	problem. In this case we only have temporary use of blocker dags
      //	since they may be needed for some other subsumption problem so we
      //	want to compute a minimal complete set of variants upfront, and
      //	then we're done variant narrowing.
      //
      //	If we're in match mode we want a minimal complete set of variants and
      //	then we will use the matching automata in VariantFolder to do the
      //	matching.
      //
      do
	expandLayer();
      while (!(frontier.empty()));

      if (flags & MATCH_MODE)
	variantCollection.prepareForVariantMatching();
    }
}

VariantSearch::~VariantSearch()
{
  if (flags & DELETE_FRESH_VARIABLE_GENERATOR)
    delete freshVariableGenerator;
  if (flags & DELETE_LAST_VARIANT_MATCHING_PROBLEM)
    delete lastVariantMatchingProblem;
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
  for (DagNode* d : blockerDags)
    d->mark();
  targetCopy->mark();
}

bool
VariantSearch::isSubsumed(DagNode* target) const
{
  Assert(flags & SUBSUMPTION_MODE, "not in SUBSUMPTION_MODE");
  Vector<DagNode*> variant(1);
  variant[0] = target;
  return variantCollection.isSubsumed(variant);
}

void
VariantSearch::expandLayer()
{
  //
  //	Look at each variant in the current frontier, and if it
  //	still exists, expand it by one step variant narrowing.
  //
  for (int index : frontier)
    {
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
  useFirstVariableFamily = !useFirstVariableFamily;
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
  //	Create a search state for one step variant narrowings. We pass variableInfo because
  //	original variable names are needed for tracing, and total number of variables is
  //	needed for unification.
  //
  int variableFamily = useFirstVariableFamily ? firstVariableFamily : secondVariableFamily;
  VariantNarrowingSearchState vnss(newContext,
				   variantSubstitution,
				   blockerDags,
				   freshVariableGenerator,
				   variableFamily,
				   variableInfo,
				   (flags & UNIFICATION_MODE));
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
	  Assert((flags & UNIFICATION_MODE), "null variant term and we're not in unification mode");
	  variantCollection.insertVariant(protectedVariant, newIndex, index, variableFamily);
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
	  //	Insert new variant in to collection and if it sticks, insert its index
	  //	into the new frontier.
	  //
	  if (variantCollection.insertVariant(protectedVariant, newIndex, index, variableFamily))
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
