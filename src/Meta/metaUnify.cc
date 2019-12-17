/*

    This file is part of the Maude 3 interpreter.

    Copyright 1997-2007 SRI International, Menlo Park, CA 94025, USA.

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
//	Code for unification descent functions.
//

bool
MetaLevelOpSymbol::metaUnify2(FreeDagNode* subject, RewritingContext& context, bool disjoint)
{
  DebugAdvisory(Tty(Tty::CYAN) << "meta unfication call: " << Tty(Tty::GREEN) << (DagNode*) subject << Tty(Tty::RESET));
  //
  //	We handle both metaUnify() and metaDisjointUnify().
  //
  if (MetaModule* m = metaLevel->downModule(subject->getArgument(0)))
    {
      Int64 solutionNr;
      int variableFamilyName;
      int variableFamily;
      if (metaLevel->downQid(subject->getArgument(2), variableFamilyName) &&
	  (variableFamily = FreshVariableSource::getFamily(variableFamilyName)) != NONE &&
	  metaLevel->downSaturate64(subject->getArgument(3), solutionNr) &&
	  solutionNr >= 0)
	{
	  UnificationProblem* unification;
	  Int64 lastSolutionNr;
	  if (!(m->getCachedStateObject(subject, solutionNr, unification, lastSolutionNr)))
	    {
	      Vector<Term*> lhs;
	      Vector<Term*> rhs;
	      if (!metaLevel->downUnificationProblem(subject->getArgument(1), lhs, rhs, m, disjoint))
		return false;
	      DebugAdvisory("metaUnify2() - making unification problem for " << subject);
	      unification = new UnificationProblem(lhs,
						   rhs,
						   new FreshVariableSource(m),
						   variableFamily);
	      if (!(unification->problemOK()))
		{
		  delete unification;
		  return false;
		}
	      lastSolutionNr = -1;
	    }
	  else
	    DebugAdvisory("metaUnify2() - using existing unification problem for " << subject);

	  DagNode* result;
	  m->protect();
	  DebugAdvisoryCheck(solutionNr - lastSolutionNr == 1,
			     "unification jump from " <<  lastSolutionNr << " to " << solutionNr);
	  for (; lastSolutionNr < solutionNr; ++lastSolutionNr)
	    {
	      if (!(unification->findNextUnifier()))
		{
		  bool incomplete = unification->isIncomplete();
		  delete unification;
		  result = disjoint ? metaLevel->upNoUnifierTriple(incomplete) : metaLevel->upNoUnifierPair(incomplete);
		  goto fail;
		}
	    }
	  m->insert(subject, unification, solutionNr);
	  {
	    const Substitution& solution = unification->getSolution();
	    const VariableInfo& variableInfo = unification->getVariableInfo();
	    int variableNameId = FreshVariableSource::getBaseName(unification->getVariableFamily());
	    result = disjoint ?
	      metaLevel->upUnificationTriple(solution, variableInfo, variableNameId, m) :
	      metaLevel->upUnificationPair(solution, variableInfo, variableNameId, m);
	  }
	fail:
	  (void) m->unprotect();
	  return context.builtInReplace(subject, result);
	}
    }
  return false;
}

bool
MetaLevelOpSymbol::metaUnify(FreeDagNode* subject, RewritingContext& context)
{
  //
  //	op metaUnify : Module UnificationProblem Qid Nat ~> UnificationPair? .
  //
  return metaUnify2(subject, context, false);
}

bool
MetaLevelOpSymbol::metaDisjointUnify(FreeDagNode* subject, RewritingContext& context)
{
  //
  //	op metaDisjointUnify : Module UnificationProblem Qid Nat ~> UnificationTriple? .
  //
  return metaUnify2(subject, context, true);
}
