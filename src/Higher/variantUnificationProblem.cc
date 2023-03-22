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
//	Implementation for class NarrowingUnificationProblem.
//

//	utility stuff
#include "macros.hh"
#include "vector.hh"
#include "allSat.hh"
#include "bddUser.hh"

//	forward declarations
#include "interface.hh"
#include "core.hh"
#include "variable.hh"
#include "higher.hh"

//	interface class definitions
#include "symbol.hh"
#include "dagNode.hh"
#include "extensionInfo.hh"

//	core class definitions
#include "module.hh"
#include "sort.hh"
#include "freshVariableGenerator.hh"
#include "preEquation.hh"
#include "substitution.hh"
#include "narrowingVariableInfo.hh"
#include "rewritingContext.hh"

//	variable class definitions
#include "variableDagNode.hh"
#include "variableTerm.hh"

//	higher class definitions
#include "variantSearch.hh"
#include "filteredVariantUnifierSearch.hh"
#include "variantUnificationProblem.hh"

VariantUnificationProblem::VariantUnificationProblem(RewritingContext* context,
						     const Vector<DagNode*>& blockerDags,
						     PreEquation* preEquation,
						     DagNode* target,
						     const NarrowingVariableInfo& variableInfo,
						     FreshVariableGenerator* freshVariableGenerator,
						     int disallowedVariableFamily,
						     int variantFlags)
  : context(context),
    preEquation(preEquation),
    variableInfo(variableInfo),
    freshVariableGenerator(freshVariableGenerator)
{
  DebugInfo("variantFlags = " << variantFlags);
  DebugAdvisory(Tty(Tty::RED) <<
		"VariantUnificationProblem(): lhsDag = " << preEquation->getLhsDag() <<
		"  target = " << target << Tty(Tty::RESET));
  //
  //	We assume that the variables in the target term have been indexed using variableInfo
  //	such that the indices start above those used in preEquation.
  //
  //	The unifiers we want to generate will be substitutions using this scheme into
  //	dags over fresh variables from a family other than disallowedVariableFamily.
  //	
  Module* module = preEquation->getModule();
  //
  //	Create a dag correspond to < preEquation lhs, target > tuple.
  //
  Vector<DagNode*> args(2);
  args[0] = preEquation->getLhsDag();
  args[1] = target;
  ConnectedComponent* range = target->symbol()->rangeComponent();
  Assert(range == args[0]->symbol()->rangeComponent(), "kind mismatch");

  Vector<ConnectedComponent*> domain(2);
  domain[0] = range;
  domain[1] = range;
  Symbol* tupleSymbol = module->createInternalTupleSymbol(domain, range);
  DagNode* unificationDag = tupleSymbol->makeDagNode(args);
  //
  //	VariantSearch is responsible for deleting this new rewriting context.
  //
  newContext = context->makeSubcontext(unificationDag);
  //
  //	VariantSearch actually does the work. It can trigger garbage collection
  //	via reduce so we need to set substitutionSize to ensure solution won't
  //	be read.
  //
  substitutionSize = 0;
  variantSearch = (variantFlags & FILTER_VARIANT_UNIFIERS) ?
    new FilteredVariantUnifierSearch(newContext,
				     blockerDags,
				     freshVariableGenerator,
				     variantFlags,
				     disallowedVariableFamily) :
    new VariantSearch(newContext,
		      blockerDags,
		      freshVariableGenerator,
		      variantFlags | VariantSearch::UNIFICATION_MODE,
		      disallowedVariableFamily);

  firstTargetSlot = module->getMinimumSubstitutionSize();  // first slot after variables reserved for preEquation
  substitutionSize = firstTargetSlot + variableInfo.getNrVariables();  // add the number of variables in target
  solution = new Substitution(substitutionSize);
  solution->clear(substitutionSize);  // so GC doesn't barf

  nrPreEquationVariables = preEquation->getNrRealVariables();
}

VariantUnificationProblem::~VariantUnificationProblem()
{
  delete solution;
  delete variantSearch;  // will delete newContext
}

void
VariantUnificationProblem::markReachableNodes()
{
  //
  //	Protect our current solution from garbage collection.
  //
  for (int i = 0; i < substitutionSize; i++)
    {
      DagNode* d = solution->value(i);
      if (d != 0)
	d->mark();
    }
}

bool
VariantUnificationProblem::findNextUnifier()
{
  bool moreUnifiers = variantSearch->findNextUnifier();
  context->transferCountFrom(*newContext);
  if (!moreUnifiers)
    return false;
  //
  //	We got a unifier; we need to translate it into the indices used in preEquations and
  //	variableInfo. Also we generate fresh variables from the appropriate family with
  //	suitable variable indices for any variables mentioned in preEquation or variableInfo
  //	but not mentioned in the unifier.
  //
  const Vector<DagNode*>& unifier = variantSearch->getCurrentUnifier(nrFreeVariables, variableFamily);
  //
  //	Start by zeroing out any old substitution.
  //
  solution->clear(substitutionSize);
  //
  //	Go through bindings in unifier, inserting them into substitution.
  //
  const NarrowingVariableInfo& unifierVariableInfo = variantSearch->getVariableInfo();
  //
  //	We can't take the number of unifier variables from unifierVariableInfo because
  //	this also includes variables from blocker terms that may not have bindings.
  //
  int nrUnifierVariables = unifier.size();
  DebugAdvisory("variant unifier:\n");

  for (int i = 0; i < nrUnifierVariables; ++i)
    {
      //
      //	Get the variable that has been bound.
      //
      VariableDagNode* variable = unifierVariableInfo.index2Variable(i);
      //
      //	See if it belongs to the target's variableInfo.
      //
      int indexInOriginalProblem = variableInfo.variable2IndexNoAdd(variable);
      if (indexInOriginalProblem == NONE)
	indexInOriginalProblem = preEquation->variable2Index(variable);  // from preEquation
      else
	indexInOriginalProblem += firstTargetSlot;  // from target; move to post preEquation indices
      solution->bind(indexInOriginalProblem, unifier[i]);

      DebugAdvisory("  " << (DagNode*) variable <<
		    " |-> " << unifier[i] <<
		    "   using index " << indexInOriginalProblem);
    }
  //
  //	Go through the substitution, binding any unbound variables to fresh variables.
  //
  for (int i = 0; i < substitutionSize; ++i)
    {
      //
      //	Either a slot belongs to the preEquation, or to the target's variableInfo
      //	(whether or not the corresponding variable actually appears in the target), or
      //	to no man's land in between these two blocks of slots. No man's land is
      //	left zero'd out.
      //
      if (i < nrPreEquationVariables)
	{
	  //
	  //	Variable belongs to the preEquation.
	  //
	  if (solution->value(i) == 0)
	    {
	      //
	      //	Not bound by VariantSearch so it must be a preEquation variable
	      //	that doesn't occur in the lhs.
	      //
	      Symbol* baseSymbol = preEquation->index2Variable(i)->symbol();
	      int name = freshVariableGenerator->getFreshVariableName(nrFreeVariables, variableFamily);
	      VariableDagNode* v = new VariableDagNode(baseSymbol, name, nrFreeVariables);
	      solution->bind(i, v);
	      ++nrFreeVariables;

	    }
	}
      else if (i >= firstTargetSlot)
	{
	  //
	  //	Variable belongs to dag being narrowed.
	  //
	  if (solution->value(i) == 0)
	    {
	      //
	      //	Not bound by VariantSearch so it must exist only in a larger dag being
	      //	narrowed and not in the subdag being unified.
	      //
	      int indexInTarget = i - firstTargetSlot;
	      Symbol* baseSymbol = variableInfo.index2Variable(indexInTarget)->symbol();
	      int name = freshVariableGenerator->getFreshVariableName(nrFreeVariables, variableFamily);
	      VariableDagNode* v = new VariableDagNode(baseSymbol, name, nrFreeVariables);
	      solution->bind(i, v);
	      ++nrFreeVariables;
	    }
	}
    }
  return true;
}
