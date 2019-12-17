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
Interpreter::variantUnify(const Vector<Token>& bubble, Int64 limit, bool debug)
{
  VisibleModule* fm = currentModule->getFlatModule();
  Vector<Term*> lhs;
  Vector<Term*> rhs;
  Vector<Term*> constraint;

  if (!(fm->parseVariantUnifyCommand(bubble, lhs, rhs, constraint)))
    return;

  if (getFlag(SHOW_COMMAND))
    {
      UserLevelRewritingContext::beginCommand();
      if (debug)
	cout << "debug ";
      cout << "variant unify ";
      if (limit != NONE)
	cout << '[' << limit << "] ";
      cout << "in " << currentModule << " : ";
      int nrPairs = lhs.size();
      for (int i = 0; i < nrPairs; ++i)
	cout << lhs[i] << " =? " << rhs[i] << ((i == nrPairs - 1) ? " " : " /\\ ");
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
  FreshVariableGenerator* freshVariableGenerator = new FreshVariableSource(fm);
  if (debug)
    UserLevelRewritingContext::setDebug();

  DagNode* d = fm->makeUnificationProblemDag(lhs, rhs);
  UserLevelRewritingContext* context = new UserLevelRewritingContext(d);

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
  VariantSearch* vs = new VariantSearch(context, blockerDags, freshVariableGenerator, true, false);
  Timer timer(getFlag(SHOW_TIMING));
  doVariantUnification(timer, fm, vs, 0, limit);
}

void
Interpreter::doVariantUnification(Timer& timer,
				  VisibleModule* module,
				  VariantSearch* state,
				  Int64 solutionCount,
				  Int64 limit)
{
  RewritingContext* context = state->getContext();
  const NarrowingVariableInfo& variableInfo = state->getVariableInfo();

  Int64 i = 0;
  for (; i != limit; i++)
    {
      int nrFreeVariables;
      int dummy;
      const Vector<DagNode*>* unifier = state->getNextUnifier(nrFreeVariables, dummy);
      if (UserLevelRewritingContext::aborted())
	break;
      if (unifier == 0)
	{
	  cout << ((solutionCount == 0) ? "\nNo unifiers.\n" : "\nNo more unifiers.\n");
	  printStats(timer, *context, getFlag(SHOW_TIMING));
	  if (state->isIncomplete())
	    IssueWarning("Some unifiers may have been missed due to incomplete unification algorithm(s).");
	  break;
	}

      ++solutionCount;
      cout << "\nUnifier #" << solutionCount << endl;
      printStats(timer, *context, getFlag(SHOW_TIMING));
      UserLevelRewritingContext::printSubstitution(*unifier, variableInfo);
    }
 
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
      continueFunc = &Interpreter::variantUnifyCont;
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
Interpreter::variantUnifyCont(Int64 limit, bool debug)
{
  VariantSearch* state = safeCast(VariantSearch*, savedState);
  VisibleModule* fm = savedModule;
  savedState = 0;
  savedModule = 0;
  continueFunc = 0;

  if (debug)
    UserLevelRewritingContext::setDebug();

  Timer timer(getFlag(SHOW_TIMING));
  doVariantUnification(timer, fm, state, savedSolutionCount, limit);
}
