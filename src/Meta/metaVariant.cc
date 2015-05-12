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
//	Code for variant narrowing descent functions.
//
#include "variantSearch.hh"

local_inline bool
MetaLevelOpSymbol::getCachedVariantSearch(MetaModule* m,
					  FreeDagNode* subject,
					  RewritingContext& context,
					  Int64 solutionNr,
					  VariantSearch*& search,
					  Int64& lastSolutionNr)
{
  if (solutionNr > 0)
    {
      CacheableState* cachedState;
      if (m->remove(subject, cachedState, lastSolutionNr))
	{
	  if (lastSolutionNr <= solutionNr)
	    {
	      search = safeCast(VariantSearch*, cachedState);
	      //
	      //	The parent context pointer of the root context in the VariantSearch is possibly
	      //	stale since it points the context from a different descent function call.
	      //
	      safeCast(UserLevelRewritingContext*, search->getContext())->
		beAdoptedBy(safeCast(UserLevelRewritingContext*, &context));
	      return true;
	    }
	  delete cachedState;
	}
    }
  return false;
}

bool
MetaLevelOpSymbol::metaGetVariant2(FreeDagNode* subject, RewritingContext& context, bool irredundant)
{
  //
  //	We handle both metaGenerateVariant() and metaGenerateIrredundantVariant().
  //
  if (MetaModule* m = metaLevel->downModule(subject->getArgument(0)))
    {
      DagNode* metaVarIndex = subject->getArgument(3);
      Int64 solutionNr;
      if (metaLevel->isNat(metaVarIndex) &&
	  metaLevel->downSaturate64(subject->getArgument(4), solutionNr) &&
	  solutionNr >= 0)
	{
	  const mpz_class& varIndex = metaLevel->getNat(metaVarIndex);
	  VariantSearch* vs;
	  Int64 lastSolutionNr;
	  if (getCachedVariantSearch(m, subject, context, solutionNr, vs, lastSolutionNr))
	    m->protect();  // Use cached state
	  else
	    {
	      Term* start;
	      Vector<Term*> blockerTerms;
	      if ((start = metaLevel->downTerm(subject->getArgument(1),  m)) &&
		  metaLevel->downTermList(subject->getArgument(2), m, blockerTerms))
		{
		  m->protect();
		  RewritingContext* startContext = term2RewritingContext(start, context);

		  Vector<DagNode*> blockerDags; 
		  FOR_EACH_CONST(i, Vector<Term*>, blockerTerms)
		    {
		      Term* t = *i;
		      t = t->normalize(true);  // we don't really need to normalize but we do need to set hash values
		      blockerDags.append(t->term2Dag());
		      t->deepSelfDestruct();
		    }
		  vs = new VariantSearch(startContext, blockerDags, new FreshVariableSource(m, varIndex), irredundant);
		  lastSolutionNr = -1;
		}
	      else
		return false;
	    }

	  DagNode* result;
	  const Vector<DagNode*>* variant;
	  int nrFreeVariables;
	  Assert(lastSolutionNr < solutionNr, "bad solution number");
	  while (lastSolutionNr < solutionNr)
	    {
	      variant = vs->getNextVariant(nrFreeVariables);
	      if (variant == 0)
		{
		  delete vs;
		  result = metaLevel->upNoVariant();
		  goto fail;
		}

	      context.transferCount(*(vs->getContext()));
	      ++lastSolutionNr;
	    }
	  {
	    m->insert(subject, vs, solutionNr);
	    mpz_class lastVarIndex = varIndex + nrFreeVariables;
	    result = metaLevel->upVariant(*variant, vs->getVariableInfo(), lastVarIndex, m);
	  }
	fail:
	  (void) m->unprotect();
	  return context.builtInReplace(subject, result);
	}
    }
  return false;
}

bool
MetaLevelOpSymbol::metaGetVariant(FreeDagNode* subject, RewritingContext& context)
{
  //
  //	op metaGenerateVariant : Module Term TermList Nat Nat ~> Variant? .
  //
  return metaGetVariant2(subject, context, false);
}

bool
MetaLevelOpSymbol::metaGetIrredundantVariant(FreeDagNode* subject, RewritingContext& context)
{
  //
  //	op metaGenerateIrredundantVariant : Module Term TermList Nat Nat ~> Variant? .
  //
  return metaGetVariant2(subject, context, true);
}

bool
MetaLevelOpSymbol::metaVariantUnify2(FreeDagNode* subject, RewritingContext& context, bool disjoint)
{
  //
  //	We handle both metaVariantUnify() and metaVariantDisjointUnify().
  //
  if (MetaModule* m = metaLevel->downModule(subject->getArgument(0)))
    {
      Int64 solutionNr;
      DagNode* metaVarIndex = subject->getArgument(3);
      if (metaLevel->isNat(metaVarIndex) &&
	  metaLevel->downSaturate64(subject->getArgument(4), solutionNr))
	{
	  const mpz_class& varIndex = metaLevel->getNat(metaVarIndex);
	  VariantSearch* vs;
	  Int64 lastSolutionNr;
	  if (getCachedVariantSearch(m, subject, context, solutionNr, vs, lastSolutionNr))
	    m->protect();  // Use cached state
	  else
	    {
	      Vector<Term*> lhs;
	      Vector<Term*> rhs;
	      if (!metaLevel->downUnificationProblem(subject->getArgument(1), lhs, rhs, m, disjoint))
		return false;

	      Vector<Term*> blockerTerms;
	      if (!metaLevel->downTermList(subject->getArgument(2), m, blockerTerms))
		{
		  FOR_EACH_CONST(i, Vector<Term*>, lhs)
		    (*i)->deepSelfDestruct();
		  FOR_EACH_CONST(j, Vector<Term*>, rhs)
		    (*j)->deepSelfDestruct();
		  return false;
		}
	      
	      m->protect();
	      DagNode* d = m->makeUnificationProblemDag(lhs, rhs);
	      RewritingContext* startContext = context.makeSubcontext(d, UserLevelRewritingContext::META_EVAL);

	      Vector<DagNode*> blockerDags; 
	      FOR_EACH_CONST(i, Vector<Term*>, blockerTerms)
		{
		  Term* t = *i;
		  t = t->normalize(true);  // we don't really need to normalize but we do need to set hash values
		  blockerDags.append(t->term2Dag());
		  t->deepSelfDestruct();
		}
	      vs = new VariantSearch(startContext, blockerDags, new FreshVariableSource(m, varIndex), true);
	      lastSolutionNr = -1;
	    }

	  DagNode* result;
	  const Vector<DagNode*>* unifier;
	  int nrFreeVariables;
	  Assert(lastSolutionNr < solutionNr, "bad solution number");
	  while (lastSolutionNr < solutionNr)
	    {
	      unifier = vs->getNextUnifier(nrFreeVariables);
	      if (unifier == 0)
		{
		  delete vs;
		  result = disjoint ? metaLevel->upNoUnifierTriple() : metaLevel->upNoUnifierPair();
		  goto fail;
		}

	      context.transferCount(*(vs->getContext()));
	      ++lastSolutionNr;
	    }
	  {
	    m->insert(subject, vs, solutionNr);
	    mpz_class lastVarIndex = varIndex + nrFreeVariables;
	    result = disjoint ?
	      metaLevel->upUnificationTriple(*unifier, vs->getVariableInfo(), lastVarIndex, m) :
	      metaLevel->upUnificationPair(*unifier, vs->getVariableInfo(), lastVarIndex, m);
	  }
	fail:
	  (void) m->unprotect();
	  return context.builtInReplace(subject, result);
	}
    }
  return false;
}

bool
MetaLevelOpSymbol::metaVariantUnify(FreeDagNode* subject, RewritingContext& context)
{
  //
  //	op metaVariantUnify : Module UnificationProblem TermList Nat Nat ~> UnificationPair? .
  //
  return metaVariantUnify2(subject, context, false);
}

bool
MetaLevelOpSymbol::metaVariantDisjointUnify(FreeDagNode* subject, RewritingContext& context)
{
  //
  //	op metaVariantDisjointUnify : Module UnificationProblem TermList Nat Nat ~> UnificationTriple? .
  //
  return metaVariantUnify2(subject, context, true);
}
