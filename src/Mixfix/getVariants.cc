/*

    This file is part of the Maude 3 interpreter.

    Copyright 2017 SRI International, Menlo Park, CA 94025, USA.

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

void
Interpreter::getVariants(const Vector<Token>& bubble, Int64 limit, bool irredundant, bool debug)
{
  VisibleModule* fm = currentModule->getFlatModule();
  Term* initial;
  Vector<Term*> constraint;

  if (!(fm->parseGetVariantsCommand(bubble, initial, constraint)))
    return;

  DagNode* d = makeDag(initial);
  if (getFlag(SHOW_COMMAND))
    {
      UserLevelRewritingContext::beginCommand();
      if (debug)
	cout << "debug ";
      cout << "get " <<  (irredundant ? "irredundant variants " : "variants ");
      if (limit != NONE)
	cout  << '[' << limit << "] ";
      cout << "in " << currentModule << " : " << d;
      if (constraint.empty())
	cout << " ." << endl;
      else
	{
	  cout << " such that ";
	  const char* sep = "";
	  FOR_EACH_CONST(i, Vector<Term*>, constraint)
	    {
	      cout << sep << *i;
	      sep = ", ";
	    }
	  cout << " irreducible ." << endl;
	}
    }

  startUsingModule(fm);
  QUANTIFY_START();
  Timer timer(getFlag(SHOW_TIMING));

  FreshVariableGenerator* freshVariableGenerator = new FreshVariableSource(fm);
  UserLevelRewritingContext* context = new UserLevelRewritingContext(d);
  if (debug)
    UserLevelRewritingContext::setDebug();

  Vector<DagNode*> blockerDags;
  FOR_EACH_CONST(i, Vector<Term*>, constraint)
    {
      Term* t = *i;
      t = t->normalize(true);  // we don't really need to normalize but we do need to set hash values
      blockerDags.append(t->term2Dag());
      t->deepSelfDestruct();
    }
  //
  //	Responsibility for deleting context and freshVariableGenerator is passed to ~VariantSearch().
  //
  VariantSearch* vs = new VariantSearch(context, blockerDags, freshVariableGenerator, false, irredundant);
  if (irredundant)
    {
      //
      //	All computation is done up-front so there is only one time value.
      //
      printStats(timer, *context, getFlag(SHOW_TIMING));
    }
  doGetVariants(timer, fm, vs, 0, limit);
}

void
Interpreter::doGetVariants(Timer& timer,
			   VisibleModule* module,
			   VariantSearch* state,
			   Int64 solutionCount,
			   Int64 limit)
{
  bool irredundant = state->getIrredundantMode();
  RewritingContext* context = state->getContext();
  const NarrowingVariableInfo& variableInfo = state->getVariableInfo();

  Int64 i = 0;
  for (; i != limit; i++)
    {
      //
      //	We don't have a use for the extra information returned by getNextVariant().
      //
      int dummy;    // nrFreeVariables;
      int dummy2;   // variableFamily
      int dummy3;   // parentIndex
      bool dummy4;  // moreInLayer
      const Vector<DagNode*>* variant = state->getNextVariant(dummy, dummy2, dummy3, dummy4);
      //
      //	If we did all the narrowing up front, there are no narrowing or rewriting
      //	steps to catch a ^C so we need to check here and treat it as an abort.
      //
      if ((irredundant && UserLevelRewritingContext::interrupted()) ||
	  UserLevelRewritingContext::aborted())
	break;
      if (variant == 0)
	{
	  cout << ((solutionCount == 0) ? "\nNo variants.\n" : "\nNo more variants.\n");
	  if (!irredundant)
	    printStats(timer, *context, getFlag(SHOW_TIMING));
	  if (state->isIncomplete())
	    IssueWarning("Some variants may have been missed due to incomplete unification algorithm(s).");
	  break;
	}
      ++solutionCount;
      cout << "\nVariant #" << solutionCount << endl;
      if (!irredundant)
	printStats(timer, *context, getFlag(SHOW_TIMING));

      int nrVariables = variant->size() - 1;
      DagNode* d = (*variant)[nrVariables];
      cout << d->getSort() << ": " << d << '\n';
      for (int i = 0; i < nrVariables; ++i)
	{
	  DagNode* v = variableInfo.index2Variable(i);
	  cout << v << " --> " << (*variant)[i] << endl;
	}
    }
  QUANTIFY_STOP();

  clearContinueInfo();  // just in case debugger left info
  if (i == limit)  
    {
      //
      //	The loop terminated because we hit user's limit so 
      //	continuation is still possible. We save the state,
      //	solutionCount and module, and set a continutation function.
      //
      context->clearCount();
      savedState = state;
      savedSolutionCount = solutionCount;
      savedModule = module;
      continueFunc = &Interpreter::getVariantsCont;
    }
  else
    {
      //
      //	Either user aborted or we ran out of solutions; either
      //	way we need to tidy up.
      //
      delete state;
      module->unprotect();
    }
  UserLevelRewritingContext::clearDebug();
}

void
Interpreter::getVariantsCont(Int64 limit, bool debug)
{
  VariantSearch* state = safeCast(VariantSearch*, savedState);
  VisibleModule* fm = savedModule;
  savedState = 0;
  savedModule = 0;
  continueFunc = 0;

  if (debug)
    UserLevelRewritingContext::setDebug();

  QUANTIFY_START();
  Timer timer(getFlag(SHOW_TIMING));
  doGetVariants(timer, fm, state, savedSolutionCount, limit);
}
