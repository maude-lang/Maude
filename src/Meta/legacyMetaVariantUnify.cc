/*

    This file is part of the Maude 3 interpreter.

    Copyright 2019-2021 SRI International, Menlo Park, CA 94025, USA.

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
//	Code for legacy variant unification descent functions.
//

bool
MetaLevelOpSymbol::legacyMetaVariantUnify2(FreeDagNode* subject, RewritingContext& context, bool disjoint)
{
  DebugAdvisory(Tty(Tty::CYAN) << "meta variant unify call: " << Tty(Tty::GREEN) <<
		(DagNode*) subject << Tty(Tty::RESET));
  //
  //	We handle both metaVariantUnify() and metaVariantDisjointUnify().
  //
  if (MetaModule* m = metaLevel->downModule(subject->getArgument(0)))
    {
      Int64 solutionNr;
      DagNode* metaVarIndex = subject->getArgument(3);
      if (metaLevel->isNat(metaVarIndex) &&
	  metaLevel->downSaturate64(subject->getArgument(4), solutionNr) && solutionNr >= 0)
	{
	  const mpz_class& varIndex = metaLevel->getNat(metaVarIndex);
	  VariantSearch* vs;
	  Int64 lastSolutionNr;
	  if (m->getCachedStateObject(subject, context, solutionNr, vs, lastSolutionNr))
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
		  for (Term* t : lhs)
		    t->deepSelfDestruct();
		  for (Term* t : rhs)
		    t->deepSelfDestruct();
		  return false;
		}
	      
	      m->protect();
	      DagNode* d = m->makeUnificationProblemDag(lhs, rhs);
	      RewritingContext* startContext = context.makeSubcontext(d, UserLevelRewritingContext::META_EVAL);

	      Vector<DagNode*> blockerDags;
	      for (Term* t : blockerTerms)
		{
		  t = t->normalize(true);  // we don't really need to normalize but we do need to set hash values
		  blockerDags.append(t->term2Dag());
		  t->deepSelfDestruct();
		}
	      vs = new VariantSearch(startContext,
				     blockerDags,
				     new FreshVariableSource(m, varIndex),
				     VariantSearch::UNIFICATION_MODE |
				     VariantSearch::DELETE_FRESH_VARIABLE_GENERATOR |
				     VariantSearch::CHECK_VARIABLE_NAMES);
	      lastSolutionNr = -1;
	    }

	  DagNode* result;
	  for (; lastSolutionNr < solutionNr; ++lastSolutionNr)
	    {
	      if (!(vs->findNextUnifier()))
		{
		  bool incomplete = vs->isIncomplete();
		  delete vs;
		  result = disjoint ? metaLevel->upNoUnifierTriple(incomplete) :
		    metaLevel->upNoUnifierPair(incomplete);
		  goto fail;
		}
	      context.transferCountFrom(*(vs->getContext()));
	    }
	  m->insert(subject, vs, solutionNr);
	  {
	    int nrFreeVariables;
	    int resultVariableFamily;
	    const Vector<DagNode*>& unifier = vs->getCurrentUnifier(nrFreeVariables,
								    resultVariableFamily);
	    mpz_class lastVarIndex = varIndex + nrFreeVariables;
	    result = disjoint ?
	      metaLevel->upUnificationTriple(unifier, vs->getVariableInfo(), lastVarIndex, m) :
	      metaLevel->upUnificationPair(unifier, vs->getVariableInfo(), lastVarIndex, m);
	  }
	fail:
	  (void) m->unprotect();
	  return context.builtInReplace(subject, result);
	}
    }
  return false;
}

bool
MetaLevelOpSymbol::legacyMetaVariantUnify(FreeDagNode* subject, RewritingContext& context)
{
  //
  //	op metaVariantUnify : Module UnificationProblem TermList Nat Nat ~> UnificationPair? .
  //
  return legacyMetaVariantUnify2(subject, context, false);
}

bool
MetaLevelOpSymbol::legacyMetaVariantDisjointUnify(FreeDagNode* subject, RewritingContext& context)
{
  //
  //	op metaVariantDisjointUnify : Module UnificationProblem TermList Nat Nat ~> UnificationTriple? .
  //
  return legacyMetaVariantUnify2(subject, context, true);
}
