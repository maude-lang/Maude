/*

    This file is part of the Maude 2 interpreter.

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
//	Code for metaUnify() descent function.
//

local_inline bool
MetaLevelOpSymbol::getCachedUnificationProblem(MetaModule* m,
					       FreeDagNode* subject,
					       Int64 solutionNr,
					       UnificationProblem*& unification,
					       Int64& lastSolutionNr)
{
  if (solutionNr > 0)
    {
      if (m->remove(subject, unification, lastSolutionNr))
	{
	  if (lastSolutionNr < solutionNr)
	    return true;
	  delete unification;
	}
    }
  return false;
}

bool
MetaLevelOpSymbol::metaUnify(FreeDagNode* subject, RewritingContext& context)
{
  //
  //	op metaUnify : Module Term Term Nat ~> Substitution? .
  //
  Int64 solutionNr;
  if (MetaModule* m = metaLevel->downModule(subject->getArgument(0)))
    {
      if (metaLevel->downSaturate64(subject->getArgument(3), solutionNr))
	{
	  UnificationProblem* unification;
	  Int64 lastSolutionNr;
	  if (!getCachedUnificationProblem(m, subject, solutionNr, unification, lastSolutionNr))
	    {
	      Term* lhs;
	      Term* rhs;
	      if (!metaLevel->downTermPair(subject->getArgument(1), subject->getArgument(2), lhs, rhs, m))
		return false;
	      unification = new UnificationProblem(lhs, rhs, new FreshVariableSource(m));
	      lastSolutionNr = -1;
	    }
	  m->protect();

	  DagNode* result;
	  while (lastSolutionNr < solutionNr)
	    {
	      if (!(unification->findNextUnifier()))
		{
		  delete unification;
		  result = metaLevel->upNoMatchSubst();
		  goto fail;
		}
	      ++lastSolutionNr;
	    }
	  m->insert(subject, unification, solutionNr);
	  {
	    PointerMap qidMap;
	    PointerMap dagNodeMap;
	    result = metaLevel->upSubstition(unification->getSolution(),
					     *unification,
					     m,
					     qidMap,
					     dagNodeMap);
	  }
	fail:
	  (void) m->unprotect();
	  return context.builtInReplace(subject, result);
	}
    }
  return false;
}
