/*

    This file is part of the Maude 3 interpreter.

    Copyright 2019-2020 SRI International, Menlo Park, CA 94025, USA.

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

bool
MetaLevelOpSymbol::metaVariantUnify2(FreeDagNode* subject, RewritingContext& context, bool disjoint)
{
  DebugAdvisory(Tty(Tty::CYAN) << "meta variant unify call: " << Tty(Tty::GREEN) <<
		(DagNode*) subject << Tty(Tty::RESET));
  //
  //	We handle both metaVariantUnify() and metaVariantDisjointUnify().
  //
  int variantFlags;
  if (metaLevel->downVariantOptionSet(subject->getArgument(4), variantFlags) &&
      (variantFlags & ~(MetaLevel::DELAY | MetaLevel::FILTER)) == 0)
    {
      if (MetaModule* m = metaLevel->downModule(subject->getArgument(0)))
	{
	  Int64 solutionNr;
	  int variableFamilyName;
	  int variableFamily;
	  if (metaLevel->downQid(subject->getArgument(3), variableFamilyName) &&
	      (variableFamily = FreshVariableSource::getFamily(variableFamilyName)) != NONE &&
	      metaLevel->downSaturate64(subject->getArgument(5), solutionNr) && solutionNr >= 0)
	    {
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
		  RewritingContext* startContext =
		    context.makeSubcontext(d, UserLevelRewritingContext::META_EVAL);

		  Vector<DagNode*> blockerDags;
		  for (Term* t : blockerTerms)
		    {
		      //
		      //	We don't really need to normalize but we do need to set hash values.
		      //
		      t = t->normalize(true);
		      blockerDags.append(t->term2Dag());
		      t->deepSelfDestruct();
		    }
		  FreshVariableGenerator* freshVariableGenerator = new FreshVariableSource(m);
		  vs = (variantFlags & MetaLevel::FILTER) ?
		    new FilteredVariantUnifierSearch(startContext,
						     blockerDags,
						     freshVariableGenerator,
						     variantFlags |
						     VariantSearch::DELETE_FRESH_VARIABLE_GENERATOR |
						     VariantSearch::CHECK_VARIABLE_NAMES,
						     variableFamily) :
		    new VariantSearch(startContext,
				      blockerDags,
				      freshVariableGenerator,
				      variantFlags |
				      VariantSearch::UNIFICATION_MODE |
				      VariantSearch::DELETE_FRESH_VARIABLE_GENERATOR |
				      VariantSearch::CHECK_VARIABLE_NAMES,
				      variableFamily);
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
		int variableNameId = FreshVariableSource::getBaseName(resultVariableFamily);
		result = disjoint ?
		  metaLevel->upUnificationTriple(unifier, vs->getVariableInfo(), variableNameId, m) :
		  metaLevel->upUnificationPair(unifier, vs->getVariableInfo(), variableNameId, m);
	      }

	    fail:
	      (void) m->unprotect();
	      return context.builtInReplace(subject, result);
	    }
	}
    }
  return false;
}

bool
MetaLevelOpSymbol::metaVariantUnify(FreeDagNode* subject, RewritingContext& context)
{
  //
  //	op metaVariantUnify : Module UnificationProblem TermList Qid
  //	                      VariantOptionSet Nat ~> UnificationPair? .
  //
  return metaVariantUnify2(subject, context, false);
}

bool
MetaLevelOpSymbol::metaVariantDisjointUnify(FreeDagNode* subject, RewritingContext& context)
{
  //
  //	op metaVariantDisjointUnify : Module UnificationProblem TermList Qid
  //	                              VariantOptionSet Nat ~> UnificationTriple? .
  //
  return metaVariantUnify2(subject, context, true);
}
