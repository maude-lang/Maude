/*

    This file is part of the Maude 3 interpreter.

    Copyright 2020-2021 SRI International, Menlo Park, CA 94025, USA.

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
//      MetaInterpreters: getUnifier(), getDisjointUnifier(), getIrredundantUnifier(),
//	getIrredundantDisjointUnifier() messages.
//

DagNode*
InterpreterManagerSymbol::getUnifier(FreeDagNode* message,
				     ObjectSystemRewritingContext& context,
				     bool disjoint,
				     bool irredundant,
				     Interpreter* interpreter)
{
  //
  //	op getUnifier : Oid Oid Qid UnificationProblem Qid Nat -> Msg .
  //                     0   1   2          3           4   5
  //
  //	op getDisjointUnifier : Oid Oid Qid UnificationProblem Qid Nat -> Msg .
  //                             0   1   2          3           4   5
  //
  //	op getIrredundantUnifier : Oid Oid Qid UnificationProblem Qid Nat -> Msg .
  //                                0   1   2          3           4   5
  //
  //	op getIrredundantDisjointUnifier : Oid Oid Qid UnificationProblem Qid Nat -> Msg .
  //                                        0   1   2          3           4   5
  //
  Int64 solutionNr;
  if (metaLevel->downSaturate64(message->getArgument(5), solutionNr) && solutionNr >= 0)
    {
      DagNode* errorMessage;
      if (MetaModule* mm = getMetaModule(message, 2, interpreter, errorMessage))
	{
	  UnificationProblem* unification;
	  Int64 lastSolutionNr;
	  if (!(mm->getCachedStateObject(message, solutionNr, unification, lastSolutionNr)))
	    {
	      //
	      //	No cached unification problem so make one.
	      //
	      int variableFamilyName;
	      if (!(metaLevel->downQid(message->getArgument(4), variableFamilyName)))
		return makeErrorReply("Bad variable family.", message);
	      int variableFamily = FreshVariableSource::getFamily(variableFamilyName);
	      if (variableFamily == NONE)
		return makeErrorReply("Bad variable family.", message);

	      Vector<Term*> lhs;
	      Vector<Term*> rhs;
	      if (!metaLevel->downUnificationProblem(message->getArgument(3),
						     lhs,
						     rhs,
						     mm,
						     disjoint))
		return makeErrorReply("Bad unification problem.", message);

	      FreshVariableSource* freshVariableSource = new FreshVariableSource(mm);
	      unification = irredundant ?
		new IrredundantUnificationProblem(lhs,
						  rhs,
						  freshVariableSource,
						  variableFamily) :
		new UnificationProblem(lhs, rhs, freshVariableSource, variableFamily);
	      if (!(unification->problemOK()))
		{
		  delete unification;
		  return makeErrorReply("Bad unification problem.", message);
		}
	      lastSolutionNr = -1;
	    }
	  //
	  //	Currently we don't really need to protect the module since
	  //	we can't enter the debugger during unification and hence
	  //	there is nothing the user can do to make the module disappear
	  //	from under us. Nevertheless we protect it as defensive programming
	  //	because things may change.
	  //
	  mm->protect();
	      
	  DagNode* target = message->getArgument(1);
	  //
	  //	Find requested solution.
	  //
	  for (; lastSolutionNr < solutionNr; ++lastSolutionNr)
	    {
	      if (!(unification->findNextUnifier()))
		{
		  Vector<DagNode*> args(3);
		  args[0] = target;
		  args[1] = message->getArgument(0);
		  args[2] = metaLevel->upBool(!(unification->isIncomplete()));
		  delete unification;
		  (void) mm->unprotect();
		  return noSuchResult2Msg->makeDagNode(args);
		}
	    }
	  mm->insert(message, unification, solutionNr);
	  {
	    Vector<DagNode*> args(4 + disjoint);
	    args[0] = target;
	    args[1] = message->getArgument(0);

	    PointerMap qidMap;
	    PointerMap dagNodeMap;
	    const Substitution& solution = unification->getSolution();
	    const VariableInfo& variableInfo = unification->getVariableInfo();
	    int variableNameId =
	      FreshVariableSource::getBaseName(unification->getVariableFamily());
	    DagNode* variableNameQid = metaLevel->upQid(variableNameId, qidMap);

	    (void) mm->unprotect();
	    if (disjoint)
	      {
		metaLevel->upDisjointSubstitutions(solution,
						   variableInfo,
						   mm,
						   qidMap,
						   dagNodeMap,
						   args[2],
						   args[3]);
		args[4] = variableNameQid;
		return (irredundant ? gotIrredundantDisjointUnifierMsg :
			gotDisjointUnifierMsg)->makeDagNode(args);
	      }
	    args[2] = metaLevel->upSubstitution(solution,
						variableInfo,
						mm,
						qidMap,
						dagNodeMap);
	    args[3] = variableNameQid;
	    return (irredundant ? gotIrredundantUnifierMsg :
		    gotUnifierMsg)->makeDagNode(args);
	  }
	}
      return errorMessage;
    }
  return makeErrorReply("Bad solution number.", message);
}
