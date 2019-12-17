/*

    This file is part of the Maude 3 interpreter.

    Copyright 2018 SRI International, Menlo Park, CA 94025, USA.

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
InterpreterManagerSymbol::getVariantUnifier(FreeDagNode* message, ObjectSystemRewritingContext& context, bool disjoint)
{
  //
  //	op getVariantUnifier : Oid Oid Qid UnificationProblem TermList Qid Nat -> Msg .
  //                            0   1   2          3             4      5  6
  //
  //	op getDisjointVariantUnifier : Oid Oid Qid UnificationProblem TermList Qid Nat -> Msg .
  //                                    0   1   2          3             4      5  6
  //
  Interpreter* interpreter;
  if (getInterpreter(message->getArgument(0), interpreter))
    {
      Int64 solutionNr;
      if (metaLevel->downSaturate64(message->getArgument(6), solutionNr) &&
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
			      t = t->normalize(true);  // we don't really need to normalize but we do need to set hash values
			      blockerDags.append(t->term2Dag());
			      t->deepSelfDestruct();
			    }
			  //
			  //	Pull down unification problem.
			  //
			  Vector<Term*> lhs;
			  Vector<Term*> rhs;
			  cerr << "saw " << message->getArgument(3) << endl;
			  if (!(metaLevel->downUnificationProblem(message->getArgument(3), lhs, rhs, m, disjoint)))
			    return false;
			  DagNode* d = m->makeUnificationProblemDag(lhs, rhs);
			  RewritingContext* startContext = context.makeSubcontext(d, UserLevelRewritingContext::META_EVAL);
			  //
			  //	VariantSearch() will do rewriting so we need to protect
			  //	the module from being overwritten in the debugger.
			  //
			  m->protect();
			  vs = new VariantSearch(startContext,
						 blockerDags,
						 new FreshVariableSource(m),
						 true,    // unification mode
						 false,   // not irredundant
						 true,    // delete fresh variable generator
						 variableFamily,
						 true);   // check variable names
			  lastSolutionNr = -1;
			}

		      DagNode* target = message->getArgument(1);
		      DagNode* reply;
		      //
		      //	Now we compute the requested variant unifier.
		      //
		      const Vector<DagNode*>* unifier = 0;  // substitution with variant dag at the end
		      int nrFreeVariables;              // number of free variables used to express the variant
		      int variableFamily;	        // family of fresh variables used to express the variant

		      if (lastSolutionNr == solutionNr)
			{
			  //
			  //	So the user can ask for the same variant unifier over and over
			  //	again without a horrible loss of performance.
			  //
			  unifier = vs->getLastReturnedUnifier(nrFreeVariables, variableFamily);
			}
		      else
			{
			  //
			  //	Keep extracting unifiers until we find the one we want or run out.
			  //
			  while (lastSolutionNr < solutionNr)
			    {
			      unifier = vs->getNextUnifier(nrFreeVariables, variableFamily);
			      if (unifier == 0)
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
			      ++lastSolutionNr;
			    }
			}
		      //
		      //	Got the solution we wanted so cache the state.
		      //
		      mm->insert(message, vs, solutionNr);
		      {
			Vector<DagNode*> args(5 + disjoint);
			args[0] = target;
			args[1] = message->getArgument(0);
			args[2] = upRewriteCount(vs->getContext());
			PointerMap qidMap;
			PointerMap dagNodeMap;
			const NarrowingVariableInfo& variableInfo = vs->getVariableInfo();
			int variableNameId = FreshVariableSource::getBaseName(variableFamily);
			DagNode* variableNameQid = metaLevel->upQid(variableNameId, qidMap);
			if (disjoint)
			  {
			    metaLevel->upDisjointSubstitutions(*unifier,
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
			    args[3] = metaLevel->upSubstitution(*unifier, variableInfo, unifier->size(), m, qidMap, dagNodeMap);
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
