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
//	Implementation for class VariantNarrowingSearchState.
//

//	utility stuff
#include "macros.hh"
#include "vector.hh"

//	forward declarations
#include "interface.hh"
#include "core.hh"
#include "variable.hh"

//	interface class definitions
#include "symbol.hh"
#include "dagNode.hh"
#include "rhsAutomaton.hh"
#include "lhsAutomaton.hh"
#include "subproblem.hh"

//	core class definitions
#include "rewritingContext.hh"
#include "equation.hh"
#include "freshVariableGenerator.hh"
#include "dagArgumentIterator.hh"
#include "extensionInfo.hh"

//	variable class definitions
#include "variableDagNode.hh"

//	higher class definitions
#include "narrowingUnificationProblem.hh"
#include "variantNarrowingSearchState.hh"

VariantNarrowingSearchState::VariantNarrowingSearchState(RewritingContext* context,
							 const Vector<DagNode*>& variantSubstitution,
							 const Vector<DagNode*>& blockerDags,
							 FreshVariableGenerator* freshVariableGenerator,
							 int variableFamily,
							 const NarrowingVariableInfo& originalVariables,
							 bool unificationMode)
  :  PositionState(context->root(), 0, 0, UNBOUNDED),
     context(context),
     variantSubstitution(variantSubstitution),
     blockerDags(blockerDags),
     //freshVariableGenerator(freshVariableGenerator),
     originalVariables(originalVariables),
     module(context->root()->symbol()->getModule()),
     blockerSubstitution(originalVariables.getNrVariables())
{
  //DebugAlways("variant term = " << context->root());
  //
  //	The variant term is contained in the context.
  //	The variant substitution could be empty vector if we don't care about it.
  //	The blocker substitution can have entries for variables that are peculiar to the blocker dags.
  //
  incompleteFlag = false;
  if (originalVariables.getNrVariables() > 0)
    {
      //
      //	Make sure that all variables that appeared in the original problem, including
      //	those that only appear in blocker dags are cleared.
      //
      blockerSubstitution.clear(originalVariables.getNrVariables());
    }
  //
  //	Index all variables occuring in the variant term and the variant substitution.
  //	These VariableDagNodes are coming from dags that are assumed to be protected by the
  //	caller and thus we assume are safe from garbage collection.
  //
  //	Indexing the variables will convert any persistent representations into
  //	regular representations suitable for unification and instantiation.
  //
  //	We allocate indices above any that may be used by variant equations in the module.
  //
  int firstTargetSlot = module->getMinimumSubstitutionSize();
  context->root()->indexVariables(variableInfo, firstTargetSlot);
  //
  //	Only variables that occur in the term part of the variant are considered interesting.
  //
  int nrInterestingVariables = variableInfo.getNrVariables();
  //
  //	Now we can index the variant substitution.
  //
  for (DagNode* d : variantSubstitution)
    d->indexVariables(variableInfo, firstTargetSlot);
  //
  //	We filter unifiers by subsumption on interesting variables.
  //
  unifiers = new UnifierFilter(firstTargetSlot, nrInterestingVariables);
  //
  //	Generate all unifiers between non-variable subterms in our variant term and variant
  //	equations in our module and insert them in our unifier filter.
  //
  if (unificationMode)
    {
      //
      //	If we are in unification mode, get the lhs and rhs from under the pairing
      //	symbol and try to unify them.
      //
      DagArgumentIterator a(context->root());
      Assert(a.valid(), "bad 1st argument in unification mode");
      DagNode* lhs = a.argument();
      a.next();
      Assert(a.valid(), "bad 2nd argument in unification mode");

      //for (int i = 0; i < variableInfo.getNrVariables(); ++i)
      //  cout << "var index " << i << " is " << (DagNode*) variableInfo.index2Variable(i) << endl;
      NarrowingUnificationProblem* unificationProblem =
	new NarrowingUnificationProblem(lhs,
					a.argument(),
					variableInfo,
					freshVariableGenerator,
					variableFamily);
      collectUnifiers(unificationProblem, 0, NONE);
      incompleteFlag |= unificationProblem->isIncomplete();
      delete unificationProblem;
    }

  while (findNextPosition())
    {
      DagNode* d = getDagNode();
      //cout << "looking at subterm: " << d << endl;

      if (dynamic_cast<VariableDagNode*>(d) == 0)  // only consider non-variable positions
	{
	  int positionIndex = getPositionIndex();
	  //cout << "has position " << positionIndex << endl;
	  //
	  //	If the top symbol is unstable, we need to consider all the equations,
	  //	rather than just the ones indexed under the top symbol.
	  //
	  Symbol* s = d->symbol();
	  const Vector<Equation*>& equations = s->isStable() ?
	    s->getEquations() :
	    module->getEquations();
	  const ConnectedComponent* topComponent = s->rangeComponent();

	  for (Equation* eq : equations)
	    {
	      if (eq->isVariant() &&  // only consider equations with the variant attribute
		  eq->getLhs()->getComponent() == topComponent)  // and with lhs in the correct component
		{
		  NarrowingUnificationProblem* unificationProblem =
		    new NarrowingUnificationProblem(eq,
						    d,
						    variableInfo,
						    freshVariableGenerator,
						    variableFamily);
		  collectUnifiers(unificationProblem, positionIndex, eq->getIndexWithinModule());
		  incompleteFlag |= unificationProblem->isIncomplete();
		  delete unificationProblem;
		}
	    }
	}
    }
}

void
VariantNarrowingSearchState::collectUnifiers(NarrowingUnificationProblem* unificationProblem,
					     int positionIndex,
					     int equationIndex)
{
  int firstTargetSlot = module->getMinimumSubstitutionSize();
  int nrInterestingVariables = variableInfo.getNrVariables();
  long nrUnifiersFound = 0;
  while (unificationProblem->findNextUnifier())
    {
      ++nrUnifiersFound;
      if (globalVerboseFlag)
	{
	  //
	  //	Because this can take a very long time, if the user
	  //	has switch on the verbose flag, we print a message
	  //	every thousand unifiers we look at, so they know something
	  //	is happening.
	  //
	  if (nrUnifiersFound >= 1000 && (nrUnifiersFound % 1000) == 0)
	    {
	      cout << "Variant Narrowing, term = " << context->root();
	      if (equationIndex == NONE)
		cout << "(unifying of subterms) for variant unfication\n";
	      else
		cout << ", subterm = " <<  getDagNode() <<
		  " equation = " << module->getEquations()[equationIndex] << '\n';
	      cout << "number of unifiers seen = " << nrUnifiersFound << endl;
	    }
	}
      const Substitution& solution = unificationProblem->getSolution();
      //
      //	Check for reducibility on interesting variables.
      //
      for (int j = 0; j < nrInterestingVariables; ++j)
	{
	  DagNode* d = solution.value(firstTargetSlot + j);
	  d->computeTrueSort(*context);  // needed for matching
	  //
	  //	If a unifier fails this check, since anything it might have subsumed will be
	  //	an instance of it and also fail the check, we lose nothing by tossing it now.
	  //
	  if (d->reducibleByVariantEquation(*context))
	    goto nextUnifier;
	}
      unifiers->insertUnifier(solution, positionIndex, equationIndex);
    nextUnifier:
      ;
    }
}

VariantNarrowingSearchState::~VariantNarrowingSearchState()
{
  delete unifiers;
}

bool
VariantNarrowingSearchState::findNextVariant(DagNode*& newVariantTerm, Vector<DagNode*>& newVariantSubstitution)
{
  int variantSubstitutionSize = variantSubstitution.size();
  newVariantSubstitution.resize(variantSubstitutionSize);

  Substitution* survivor;
  int positionIndex;
  int equationIndex;
  while (unifiers->getNextSurvivingUnifier(survivor, positionIndex, equationIndex))
    {
      //DebugAlways("trying unifier");
      //
      //	Compute accumulated substitution and check for reducibility.
      //
      for (int i = 0; i < variantSubstitutionSize; ++i)
	{
	  DagNode* d = variantSubstitution[i]->instantiate(*survivor, false);
	  //DebugAlways("considering entry " << i << " which is\n  " << variantSubstitution[i] << "\ninstantiated to\n  " << d);
	  if (d == 0)
	    d = variantSubstitution[i];  // no change
	  d->computeTrueSort(*context);  // also handles theory normalization
	  if (d->reducibleByVariantEquation(*context))
	    {
	      //DebugAlways("killed by entry " << i << " of " << variantSubstitutionSize);
	      goto nextUnifier;
	    }
	  newVariantSubstitution[i] = d;
	  blockerSubstitution.bind(i, d);
	}
      //DebugAlways("survived reducibilitity check");
      //
      //	Check if this variant causes any of the blocker dags to become reducible.
      //
      {
	for (DagNode* b : blockerDags)
	  {
	    DebugInfo("checking blocker dag " << b);
	    DagNode* d = b->instantiate(blockerSubstitution, false);
	    if (d != 0)
	      {
		DebugInfo("instantiated to " << d);
		d->computeTrueSort(*context);  // also handles theory normalization
		if (d->reducibleByVariantEquation(*context))
		  {
		    DebugInfo("blocked");
		    goto nextUnifier;
		  }
	      }
	    DebugInfo("irreducible");
	  }
      }
      //DebugAlways("survived blocker dags");
      if (equationIndex == NONE)
	{
	  //
	  //	Virtual rewrite; this means we're in unificationMode and just found a
	  //	solution to the variant unification problem.
	  //
	  newVariantTerm = 0;
	  return true;
	}
      {
	Equation* eq = module->getEquations()[equationIndex];
	//
	//	Variant equations are compiled in such a way that all variable bindings
	//	are copied upto eager.
	//
	DagNode* replacement = eq->getRhsBuilder().construct(*survivor);
	//
	//	Any slots we touched while building at right hand instance we don't care
	//	about; they occur after the binding to the variables in the equation and
	//	before the bindings to the variables in the original variant.
	//
	int firstVariantVariable = module->getMinimumSubstitutionSize();
	int lastVariantVariable = firstVariantVariable + variableInfo.getNrVariables() - 1;
	//
	//	rebuildAndInstantiateDag() will copy upto eager the unifier bindings.
	//
	newVariantTerm = rebuildAndInstantiateDag(replacement,
						  *survivor,
						  firstVariantVariable,
						  lastVariantVariable,
						  positionIndex);
	//
	//	However we still need to clear those slots because the unifier belongs to
	//	the UnifierFilter that will do GC protection on it.
	//
	//	The variables belonging to the equation are in slots
	//	  0,..., eq->getNrRealVariables() - 1
	//	Protected variables needed for matching are in slots
	//	  eq->getNrRealVariables(),..., eq->getNrProtectedVariables() - 1
	//	Construction slots start from eq->getNrProtectedVariables() and must end
	//	by firstVariantVariable - 1
	//
	for (int i = eq->getNrProtectedVariables(); i < firstVariantVariable; ++i)
	  survivor->bind(i, 0);

	if (RewritingContext::getTraceStatus())
	  {
	    context->traceVariantNarrowingStep(eq,
					       variantSubstitution,
					       getDagNode(positionIndex),
					       replacement,
					       variableInfo,
					       survivor,
					       newVariantTerm,
					       newVariantSubstitution,
					       originalVariables);
	    if (context->traceAbort())
	      return false;
	  }
	context->incrementVariantNarrowingCount();

	return true;
      }
    nextUnifier:
      ;
    }
  return false;
}
