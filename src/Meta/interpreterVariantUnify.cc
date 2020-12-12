/*

    This file is part of the Maude 3 interpreter.

    Copyright 2018-2020 SRI International, Menlo Park, CA 94025, USA.

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
//	Code for getVariantUnifier()/getDisjointVariantUnifier() messages.
//

bool
InterpreterManagerSymbol::getVariantUnifier(FreeDagNode* message,
					    ObjectSystemRewritingContext& context,
					    bool disjoint)
{
  //
  //	op getVariantUnifier : Oid Oid Qid UnificationProblem TermList Qid VariantOptionSet Nat -> Msg .
  //                            0   1   2          3             4      5         6          7
  //
  //	op getDisjointVariantUnifier : Oid Oid Qid UnificationProblem TermList Qid VariantOptionSet Nat -> Msg .
  //                                    0   1   2          3             4      5         6          7
  //
  Interpreter* interpreter;
  if (getInterpreter(message->getArgument(0), interpreter))
    {
      int variantFlags;
      Int64 solutionNr;
      if (metaLevel->downVariantOptionSet(message->getArgument(6), variantFlags) &&
	  (variantFlags & ~(MetaLevel::DELAY | MetaLevel::FILTER)) == 0 &&
	  metaLevel->downSaturate64(message->getArgument(7), solutionNr) &&
	  solutionNr >= 0)
	{
	  int id;
	  if (metaLevel->downQid(message->getArgument(2), id))
	    {
	      if (PreModule* pm = interpreter->getModule(id))
		{
		  if (ImportModule* m = pm->getFlatModule())
		    {
		      VariantSearch* vs;
		      Int64 lastSolutionNr;
		      MetaModule* mm = safeCast(MetaModule*, m);  // metaInterpreter can only store MetaModules
		      if (mm->getCachedStateObject(message, solutionNr, vs, lastSolutionNr))
			m->protect();  // Use cached state
		      else
			{
			  //
			  //	No cached variant search so make one.
			  //
			  int variableFamilyName;
			  if (!(metaLevel->downQid(message->getArgument(5), variableFamilyName)))
			    return false;
			  int variableFamily = FreshVariableSource::getFamily(variableFamilyName);
			  if (variableFamily == NONE)
			    return false;
			  //
			  //	Pull down blocker terms.
			  //
			  Vector<Term*> blockerTerms;
			  if (!(metaLevel->downTermList(message->getArgument(4), m, blockerTerms)))
			    return false;
			  //
			  //	Convert blocker terms to blocker dags.
			  //
			  Vector<DagNode*> blockerDags; 
			  FOR_EACH_CONST(i, Vector<Term*>, blockerTerms)
			    {
			      Term* t = *i;
			      //
			      //	We don't really need to normalize but we do need to
			      //	set hash values.
			      //
			      t = t->normalize(true);
			      blockerDags.append(t->term2Dag());
			      t->deepSelfDestruct();
			    }
			  //
			  //	Pull down unification problem.
			  //
			  Vector<Term*> lhs;
			  Vector<Term*> rhs;
			  if (!(metaLevel->downUnificationProblem(message->getArgument(3), lhs, rhs, m, disjoint)))
			    return false;
			  DagNode* d = m->makeUnificationProblemDag(lhs, rhs);
			  RewritingContext* startContext =
			    context.makeSubcontext(d, UserLevelRewritingContext::META_EVAL);
			  //
			  //	VariantSearch() will do rewriting so we need to protect
			  //	the module from being overwritten in the debugger.
			  //
			  m->protect();
			  FreshVariableGenerator* freshVariableGenerator = new FreshVariableSource(m);
			  vs =  (variantFlags & MetaLevel::FILTER) ?
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

		      DagNode* target = message->getArgument(1);
		      DagNode* reply;
		      //
		      //	Now we compute the requested variant unifier.
		      //
		      for (; lastSolutionNr < solutionNr; ++lastSolutionNr)
			{
			  if (!(vs->findNextUnifier()))
			    {
			      Vector<DagNode*> args(4);
			      args[0] = target;
			      args[1] = message->getArgument(0);
			      args[2] = upRewriteCount(vs->getContext());
			      args[3] = metaLevel->upBool(!(vs->isIncomplete()));
			      reply = noSuchResult3Msg->makeDagNode(args);
			      context.addInCount(*(vs->getContext()));
			      delete vs;
			      goto done;
			    }
			}
		      //
		      //	Got the solution we wanted so cache the state.
		      //
		      mm->insert(message, vs, solutionNr);
		      {
			int nrFreeVariables;
			int resultVariableFamily;
			const Vector<DagNode*>& unifier = vs->getCurrentUnifier(nrFreeVariables,
										resultVariableFamily);
			Vector<DagNode*> args(5 + disjoint);
			args[0] = target;
			args[1] = message->getArgument(0);
			args[2] = upRewriteCount(vs->getContext());
			PointerMap qidMap;
			PointerMap dagNodeMap;
			const NarrowingVariableInfo& variableInfo = vs->getVariableInfo();
			int variableNameId = FreshVariableSource::getBaseName(resultVariableFamily);
			DagNode* variableNameQid = metaLevel->upQid(variableNameId, qidMap);
			if (disjoint)
			  {
			    metaLevel->upDisjointSubstitutions(unifier,
							       variableInfo,
							       m,
							       qidMap,
							       dagNodeMap,
							       args[3],
							       args[4]);
			    args[5] = variableNameQid;
			    reply = gotDisjointVariantUnifierMsg->makeDagNode(args);
			  }
			else
			  {
			    args[3] = metaLevel->upSubstitution(unifier,
								variableInfo,
								unifier.size(),
								m,
								qidMap,
								dagNodeMap);
			    args[4] = variableNameQid;
			    reply = gotVariantUnifierMsg->makeDagNode(args);
			  }
			context.transferCountFrom(*(vs->getContext()));
		      }
		    done:
		      context.bufferMessage(target, reply);
		      (void) m->unprotect();
		      return true;
		    }
		}
	    }
	}
    }
  return false;
}
