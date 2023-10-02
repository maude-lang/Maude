/*

    This file is part of the Maude 3 interpreter.

    Copyright 2017-2023 SRI International, Menlo Park, CA 94025, USA.

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
  bool showCommand = getFlag(SHOW_COMMAND);
  if (showCommand)
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
	  for (const Term* t : constraint)
	    {
	      cout << sep << t;
	      sep = ", ";
	    }
	  cout << " irreducible ." << endl;
	}
    }
  if (latexBuffer != 0)
    {
      latexBuffer->generateGetVariants(showCommand,
				       irredundant,
				       d,
				       constraint,
				       limit,
				       debug);
    }

  startUsingModule(fm);
  QUANTIFY_START();
  Timer timer(getFlag(SHOW_TIMING));

  FreshVariableGenerator* freshVariableGenerator = new FreshVariableSource(fm);
  UserLevelRewritingContext* context = new UserLevelRewritingContext(d);
  if (debug)
    UserLevelRewritingContext::setDebug();

  Vector<DagNode*> blockerDags;
  for (Term* t : constraint)
    {
      t = t->normalize(true);  // we don't really need to normalize but we do need to set hash values
      blockerDags.append(t->term2Dag());
      t->deepSelfDestruct();
    }
  //
  //	Responsibility for deleting context and freshVariableGenerator is passed to ~VariantSearch().
  //
  VariantSearch* vs = new VariantSearch(context,
					blockerDags,
					freshVariableGenerator,
					VariantSearch::DELETE_FRESH_VARIABLE_GENERATOR |
					VariantSearch::CHECK_VARIABLE_NAMES |
					(irredundant ? VariantSearch::IRREDUNDANT_MODE : 0));
  if (vs->problemOK())
    {
      if (irredundant)
	{
	  //
	  //	All computation is done upfront so there is only one time value.
	  //
	  printStats(timer, *context);
	}
      doGetVariants(timer, fm, vs, 0, limit);
    }
  else
    {
      delete vs;
      fm->unprotect();
    }
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
      bool anotherVariant = state->findNextVariant();
      //
      //	If we did all the narrowing up front, there are no narrowing or rewriting
      //	steps to catch a ^C so we need to check here and treat it as an abort.
      //
      if ((irredundant && UserLevelRewritingContext::interrupted()) ||
	  UserLevelRewritingContext::aborted())
	break;
      
     if (!anotherVariant)
	{
	  const char* message = (solutionCount == 0) ? "No variants." : "No more variants.";
	  cout << '\n' <<  message << '\n';
	  if (latexBuffer)
	    latexBuffer->generateNonResult(message);
	  if (!irredundant)
	    printStats(timer, *context);
	  if (state->isIncomplete())
	    {
	      const char* message = "Some variants may have been missed due to incomplete unification algorithm(s).";
	      IssueWarning(message);
	      if (latexBuffer)
		latexBuffer->generateWarning(message);
	    }
	  break;
	}
     
      ++solutionCount;
      cout << "\nVariant " << solutionCount << endl;
      if (latexBuffer)
	latexBuffer->generateResult("Variant", solutionCount);

      if (!irredundant)
	printStats(timer, *context);

      int nrFreeVariables;  // dummy
      int variableFamily;  // dummy
      const Vector<DagNode*>& variant = state->getCurrentVariant(nrFreeVariables, variableFamily);

      int nrVariables = variant.size() - 1;
      DagNode* d = variant[nrVariables];
      cout << d->getSort() << ": " << d << '\n';
      for (int i = 0; i < nrVariables; ++i)
	{
	  DagNode* v = variableInfo.index2Variable(i);
	  cout << v << " --> " << variant[i] << endl;
	}
      if (latexBuffer)
	latexBuffer->generateVariant(variant, variableInfo);
    }
  QUANTIFY_STOP();

  if (latexBuffer)
    latexBuffer->cleanUp();
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
  if (latexBuffer)
    latexBuffer->generateContinue(getFlag(SHOW_COMMAND), limit, debug);
  if (debug)
    UserLevelRewritingContext::setDebug();
  QUANTIFY_START();
  Timer timer(getFlag(SHOW_TIMING));
  doGetVariants(timer, fm, state, savedSolutionCount, limit);
}
