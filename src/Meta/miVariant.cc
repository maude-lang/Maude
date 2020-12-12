/*

    This file is part of the Maude 3 interpreter.

    Copyright 2020 SRI International, Menlo Park, CA 94025, USA.

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
//	MetaInterpreters: getVariant() message.
//

DagNode*
InterpreterManagerSymbol::getVariant(FreeDagNode* message,
				     ObjectSystemRewritingContext& context,
				     Interpreter* interpreter)
{
  //
  //	op getVariant : Oid Oid Qid Term TermList Bool Qid Nat -> Msg .
  //                     0   1   2   3      4      5    6   7
  //
  Int64 solutionNr;
  if (metaLevel->downSaturate64(message->getArgument(7), solutionNr) && solutionNr >= 0)
    {
      DagNode* errorMessage;
      if (MetaModule* mm = getMetaModule(message, 2, interpreter, errorMessage))
	{
	  VariantSearch* vs;
	  Int64 lastSolutionNr;
	  if (mm->getCachedStateObject(message, solutionNr, vs, lastSolutionNr))
	    mm->protect();  // Use cached state
	  else
	    {
	      //
	      //	No cached variant search so make one.
	      //
	      bool irredundant;
	      if (!(metaLevel->downBool(message->getArgument(5), irredundant)))
		return makeErrorReply("Bad flag.", message);

	      int variableFamilyName;
	      if (!(metaLevel->downQid(message->getArgument(6), variableFamilyName)))
		return makeErrorReply("Bad variable family.", message);
	      int variableFamily = FreshVariableSource::getFamily(variableFamilyName);
	      if (variableFamily == NONE)
		return makeErrorReply("Bad variable family.", message);

	      Term* start = metaLevel->downTerm(message->getArgument(3), mm);
	      if (start == 0)
		return makeErrorReply("Bad term.", message);
	      //
	      //	Pull down blocker terms.
	      //
	      Vector<Term*> blockerTerms;
	      if (!(metaLevel->downTermList(message->getArgument(4), mm, blockerTerms)))
		{
		  start->deepSelfDestruct();
		  return makeErrorReply("Bad reducibility constraint.", message);
		}
	      //
	      //	Convert blocker terms to blocker dags.
	      //
	      Vector<DagNode*> blockerDags;
	      for (Term* t : blockerTerms)
		{
		  //
		  //	We don't really need to normalize but we do need
		  //	to set hash values.
		  //
		  t = t->normalize(true);
		  blockerDags.append(t->term2Dag());
		  t->deepSelfDestruct();
		}
	      //
	      //	VariantSearch() will do rewriting so we need to protect
	      //	the module from being overwritten in the debugger.
	      //
	      mm->protect();
	      vs = new VariantSearch(term2RewritingContext(start, context),
				     blockerDags,
				     new FreshVariableSource(mm),
				     VariantSearch::DELETE_FRESH_VARIABLE_GENERATOR |
				     VariantSearch::CHECK_VARIABLE_NAMES |
				     (irredundant ? VariantSearch::IRREDUNDANT_MODE : 0),
				     variableFamily);
	      lastSolutionNr = -1;
	    }

	  DagNode* target = message->getArgument(1);
	  //
	  //	Now we compute the requested variant.
	  //
	  for (; lastSolutionNr < solutionNr; ++lastSolutionNr)
	    {
	      if (!(vs->findNextVariant()))
		{
		  Vector<DagNode*> args(4);
		  args[0] = target;
		  args[1] = message->getArgument(0);
		  args[2] = upRewriteCount(vs->getContext());
		  args[3] = metaLevel->upBool(!(vs->isIncomplete()));
		  context.addInCount(*(vs->getContext()));
		  delete vs;
		  (void) mm->unprotect();
		  return noSuchResult3Msg->makeDagNode(args);
		}
	    }
	  //
	  //	Got the solution we wanted so cache the state.
	  //
	  mm->insert(message, vs, solutionNr);
	  {
	    int nrFreeVariables;
	    int resultVariableFamily;
	    int parentIndex;
	    bool moreInLayer;
	    const Vector<DagNode*>& variant = vs->getCurrentVariant(nrFreeVariables,
								    resultVariableFamily,
								    &parentIndex,
								    &moreInLayer);
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
	    int nrVariables = variant.size() - 1;
	    int variableNameId = FreshVariableSource::getBaseName(resultVariableFamily);

	    args[3] = metaLevel->upDagNode(variant[nrVariables], mm, qidMap, dagNodeMap);
	    args[4] = metaLevel->upSubstitution(variant,
						variableInfo,
						nrVariables,
						mm,
						qidMap,
						dagNodeMap);
	    args[5] = metaLevel->upQid(variableNameId, qidMap);
	    args[6] = (parentIndex >= 0) ? metaLevel->upNat(parentIndex) :
	      metaLevel->upNoParent();
	    args[7] = metaLevel->upBool(moreInLayer);

	    context.transferCountFrom(*(vs->getContext()));
	    (void) mm->unprotect();
	    return gotVariantMsg->makeDagNode(args);
	  }
	}
      return errorMessage;
    }
  return makeErrorReply("Bad solution number.", message);
}
