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
//	Code for getVariant() message.
//

bool
InterpreterManagerSymbol::getVariant(FreeDagNode* message, ObjectSystemRewritingContext& context)
{
  //
  //	op getVariant : Oid Oid Qid Term TermList Bool Qid Nat -> Msg .
  //                     0   1   2   3      4      5    6   7
  //
  Interpreter* interpreter;
  if (getInterpreter(message->getArgument(0), interpreter))
    {
      Int64 solutionNr;
      int id;
      if (metaLevel->downSaturate64(message->getArgument(7), solutionNr) &&
	  solutionNr >= 0 &&
	  metaLevel->downQid(message->getArgument(2), id))
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
		      bool irredundant;
		      int variableFamilyName;
		      int variableFamily;
		      Term* start;
		      Vector<Term*> blockerTerms;
		      if (metaLevel->downBool(message->getArgument(5), irredundant) &&
			  metaLevel->downQid(message->getArgument(6), variableFamilyName) &&
			  (variableFamily = FreshVariableSource::getFamily(variableFamilyName)) != NONE &&
			  (start = metaLevel->downTerm(message->getArgument(3),  m)))
			{
			  if (metaLevel->downTermList(message->getArgument(4), m, blockerTerms))
			    {
			      Vector<DagNode*> blockerDags; 
			      FOR_EACH_CONST(i, Vector<Term*>, blockerTerms)
				{
				  Term* t = *i;
				  t = t->normalize(true);  // we don't really need to normalize but we do need to set hash values
				  blockerDags.append(t->term2Dag());
				  t->deepSelfDestruct();
				}
			      //
			      //	VariantSearch() will do rewriting so we need to protect
			      //	the module from being overwritten in the debugger.
			      //
			      m->protect();
			      vs = new VariantSearch(term2RewritingContext(start, context),
						     blockerDags,
						     new FreshVariableSource(m),
						     false,  // not doing unification
						     irredundant,
						     true,  // delete fresh variable source in dtor
						     variableFamily,
						     true  /* check variable in start term */);
			      lastSolutionNr = -1;
			    }
			  else
			    {
			      //
			      //	Got start term, but blocker terms failed so
			      //	need to clean up before we quit.
			      //
			      start->deepSelfDestruct();
			      return false;
			    }
			}
		      else
			return false;
		    }
		  DagNode* target = message->getArgument(1);
		  DagNode* reply;
		  //
		  //	Now we compute the requested variant.
		  //
		  const Vector<DagNode*>* variant = 0;  // substitution with variant dag at the end
		  int nrFreeVariables;              // number of free variables used to express the variant
		  int variableFamily;	            // family of fresh variables used to express the variant
		  int parentIndex;                  // index of parent variant
		  bool moreInLayer;                 // more variants in current layer?

		  if (lastSolutionNr == solutionNr)
		    {
		      //
		      //	So the user can ask for the same variant over and over again without
		      //	a horrible loss of performance.
		      //
		      variant = vs->getLastReturnedVariant(nrFreeVariables, variableFamily, parentIndex, moreInLayer);
		    }
		  else
		    {
		      while (lastSolutionNr < solutionNr)
			{
			  variant = vs->getNextVariant(nrFreeVariables, variableFamily, parentIndex, moreInLayer);
			  if (variant == 0)
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
		    //
		    //	op gotVariant : Oid Oid RewriteCount Term Substitution Qid Parent Bool-> Msg .
		    //                   0   1       2        3        4        5    6     7
		    //
		    Vector<DagNode*> args(8);
		    args[0] = target;
		    args[1] = message->getArgument(0);
		    args[2] = upRewriteCount(vs->getContext());
		    PointerMap qidMap;
		    PointerMap dagNodeMap;

		    const NarrowingVariableInfo& variableInfo = vs->getVariableInfo();
		    int nrVariables = variant->size() - 1;
		    int variableNameId = FreshVariableSource::getBaseName(variableFamily);

		    args[3] = metaLevel->upDagNode((*variant)[nrVariables], m, qidMap, dagNodeMap);
		    args[4] = metaLevel->upSubstitution(*variant, variableInfo, nrVariables, m, qidMap, dagNodeMap);
		    args[5] = metaLevel->upQid(variableNameId, qidMap);
		    args[6] = (parentIndex >= 0) ? metaLevel->upNat(parentIndex) : metaLevel->upNoParent();
		    args[7] = metaLevel->upBool(moreInLayer);

		    reply = gotVariantMsg->makeDagNode(args);
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
  return false;
}
