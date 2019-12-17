/*

    This file is part of the Maude 3 interpreter.

    Copyright 2019 SRI International, Menlo Park, CA 94025, USA.

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
  DebugAdvisory(Tty(Tty::CYAN) << "meta variant unify call: " << Tty(Tty::GREEN) << (DagNode*) subject << Tty(Tty::RESET));
  //
  //	We handle both metaVariantUnify() and metaVariantDisjointUnify().
  //
  if (MetaModule* m = metaLevel->downModule(subject->getArgument(0)))
    {
      Int64 solutionNr;
      int variableFamilyName;
      int variableFamily;
      if (metaLevel->downQid(subject->getArgument(3), variableFamilyName) &&
	  (variableFamily = FreshVariableSource::getFamily(variableFamilyName)) != NONE &&
	  metaLevel->downSaturate64(subject->getArgument(4), solutionNr) && solutionNr >= 0)
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
	      vs = new VariantSearch(startContext,
				     blockerDags,
				     new FreshVariableSource(m),
				     true,	// unification mode
				     false,	// not irredundant
				     true,	// delete fresh variable generator on destruction
				     variableFamily,
				     true);	// check variable names
	      lastSolutionNr = -1;
	    }

	  DagNode* result;
	  const Vector<DagNode*>* unifier = 0;  // initialization is just to avoid compiler warning
	  int nrFreeVariables;
	  int resultVariableFamily;
	  if (lastSolutionNr == solutionNr)
	    {
	      //
	      //	So the user can ask for the same unifier over and over again without
	      //	a horrible loss of performance.
	      //
	      unifier = vs->getLastReturnedUnifier(nrFreeVariables, resultVariableFamily);
	    }
	  else
	    {
	      while (lastSolutionNr < solutionNr)
		{
		  unifier = vs->getNextUnifier(nrFreeVariables, resultVariableFamily);
		  if (unifier == 0)
		    {
		      bool incomplete = vs->isIncomplete();
		      delete vs;
		      result = disjoint ? metaLevel->upNoUnifierTriple(incomplete) : metaLevel->upNoUnifierPair(incomplete);
		      goto fail;
		    }

		  context.transferCountFrom(*(vs->getContext()));
		  ++lastSolutionNr;
		}
	    }
	  {
	    m->insert(subject, vs, solutionNr);
	    int variableNameId = FreshVariableSource::getBaseName(resultVariableFamily);
	    result = disjoint ?
	      metaLevel->upUnificationTriple(*unifier, vs->getVariableInfo(), variableNameId, m) :
	      metaLevel->upUnificationPair(*unifier, vs->getVariableInfo(), variableNameId, m);
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
  //	op metaVariantUnify : Module UnificationProblem TermList Qid Nat ~> UnificationPair? .
  //
  return metaVariantUnify2(subject, context, false);
}

bool
MetaLevelOpSymbol::metaVariantDisjointUnify(FreeDagNode* subject, RewritingContext& context)
{
  //
  //	op metaVariantDisjointUnify : Module UnificationProblem TermList Qid Nat ~> UnificationTriple? .
  //
  return metaVariantUnify2(subject, context, true);
}
