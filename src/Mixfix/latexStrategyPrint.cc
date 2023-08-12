/*

    This file is part of the Maude 3 interpreter.

    Copyright 2023 SRI International, Menlo Park, CA 94025, USA.

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
//	Strategy* -> ostream& LaTeX pretty printer.
//

bool
VisibleModule::latexPrintStrategy(ostream& s, StrategyExpression* strategy, int requiredPrec) const
{
  bool needParen = false;

  if (TrivialStrategy* t = dynamic_cast<TrivialStrategy*>(strategy))
    s << "\\maudeKeyword{" << (t->getResult() ? "idle" : "fail") << "}";
  else if (OneStrategy* o = dynamic_cast<OneStrategy*>(strategy))
    {
      s << "\\maudeKeyword{one}\\maudeLeftParen(";
      (void) latexPrintStrategy(s, o->getStrategy());
      s << "\\maudeRightParen";
    }
  else if (ApplicationStrategy* a = dynamic_cast<ApplicationStrategy*>(strategy))
    {
      bool top = a->getTop();
      if (top)
	s << "\\maudeKeyword{top}\\maudeLeftParen(";
      int label = a->getLabel();
      if (label == NONE)
	s << "\\maudeKeyword{all}";
      else
	{
	  s <<"\\maudeLabel{" << Token::latexName(label) << "}";
	  const Vector<Term*>& variables = a->getVariables();
	  if (!variables.empty())
	    {
	      const Vector<CachedDag>& values = a->getValues();
	      s << "\\maudeLeftBracket";
	      Index nrAssignments = variables.size();
	      for (Index i = 0; i < nrAssignments; ++i)
		{
		  if (i != 0)
		    s << "\\maudeComma\\maudeSpace";
		  latexPrettyPrint(s, variables[i]);
		  s << "\\maudeStratAssign";
		  latexPrettyPrint(s, values[i].getTerm());
		}
	      s << "\\maudeRightBracket";
	    }
	  const Vector<StrategyExpression*>& strategies = a->getStrategies();
	  if (!strategies.empty())
	    {
	      s << "\\maudeLeftBrace";
	      Index nrStrategies = strategies.size();
	      for (Index i = 0; i < nrStrategies; ++i)
		{
		  if (i != 0)
		    s << "\\maudeComma\\maudeSpace";
		  (void) latexPrintStrategy(s, strategies[i]);
		}
	      s << "\\maudeRightBrace";
	    }
	}
      if (top)
	s << "\\maudeRightParen";
    }
  else if (ConcatenationStrategy* c = dynamic_cast<ConcatenationStrategy*>(strategy))
    {
      needParen = requiredPrec < STRAT_SEQ_PREC;
      if (needParen)
	s << "\\maudeLeftParen";
      const Vector<StrategyExpression*>& strategies = c->getStrategies();
      Index nrStrategies = strategies.size();
      for (Index i = 0; i < nrStrategies; ++i)
	{
	  if (i != 0)
	    s << "\\maudeStratConcat";
	  (void) latexPrintStrategy(s, strategies[i], STRAT_SEQ_PREC);
	}
    }
  else if (UnionStrategy* c = dynamic_cast<UnionStrategy*>(strategy))
    {
      needParen = requiredPrec < STRAT_UNION_PREC;
      if (needParen)
	s << "\\maudeLeftParen";
      const Vector<StrategyExpression*>& strategies = c->getStrategies();
      Index nrStrategies = strategies.size();
      for (Index i = 0; i < nrStrategies; ++i)
	{
	  if (i != 0)
	    s << "\\maudeStratUnion";
	  (void) latexPrintStrategy(s, strategies[i], STRAT_UNION_PREC);
	}
    }
  else if (IterationStrategy* i = dynamic_cast<IterationStrategy*>(strategy))
    {
      if (latexPrintStrategy(s, i->getStrategy(), 0))
	s << "\\maudeSpace";
      s << (i->getZeroAllowed() ? "\\maudeStratStar" : "\\maudeStratPlus");
    }


  else if (BranchStrategy* b = dynamic_cast<BranchStrategy*>(strategy))
    {
      switch (b->getSuccessAction())
	{
	case BranchStrategy::FAIL:
	  {
	    Assert(b->getFailureAction() == BranchStrategy::IDLE &&
		   b->getSuccessStrategy() == 0 &&
		   b->getFailureStrategy() == 0, "unknown branch strategy");
	    s << "\\maudeKeyword{not}\\maudeLeftParen";
	    (void) latexPrintStrategy(s,  b->getInitialStrategy());
	    s << "\\maudeRightParen";
	    break;
	  }
	case BranchStrategy::IDLE:
	  {
	    Assert(b->getFailureAction() == BranchStrategy::FAIL &&
		   b->getSuccessStrategy() == 0 &&
		   b->getFailureStrategy() == 0, "unknown branch strategy");
	    s << "\\maudeKeyword{test}\\maudeLeftParen";
	    (void) latexPrintStrategy(s,  b->getInitialStrategy());
	    s << "\\maudeRightParen";
	    break;
	  }
	case BranchStrategy::PASS_THRU:
	  {
	    if (b->getFailureAction() == BranchStrategy::IDLE)
	      {
		Assert(b->getSuccessStrategy() == 0 && b->getFailureStrategy() == 0, "unknown branch strategy");
		s << "\\maudeKeyword{try}\\maudeLeftParen";
		(void) latexPrintStrategy(s,  b->getInitialStrategy());
		s << "\\maudeRightParen";
	      }
	    else
	      {
		Assert(b->getFailureAction() == BranchStrategy::NEW_STRATEGY &&
		       b->getSuccessStrategy() == 0, "unknown branch strategy");
		needParen = requiredPrec < STRAT_ORELSE_PREC;
		if (needParen)
		  s << "\\maudeLeftParen";
		(void) latexPrintStrategy(s, b->getInitialStrategy(), STRAT_ORELSE_PREC);
		s << "\\maudeSpace\\maudeKeyword{or-else}\\maudeSpace";
		(void) latexPrintStrategy(s, b->getFailureStrategy(), STRAT_ORELSE_PREC);
	      }
	    break;
	  }
	case BranchStrategy::NEW_STRATEGY:
	  {
	    Assert(b->getFailureAction() == BranchStrategy::NEW_STRATEGY, "unknown branch strategy");
	    needParen = requiredPrec < STRAT_BRANCH_PREC;
	    if (needParen)
	      s << "\\maudeLeftParen";
	    (void) latexPrintStrategy(s, b->getInitialStrategy(), STRAT_BRANCH_PREC);
	    s << "\\maudeStratQuery";
	    (void) latexPrintStrategy(s, b->getSuccessStrategy(), STRAT_BRANCH_PREC);
	    s << "\\maudeStratColon";
	    (void) latexPrintStrategy(s, b->getFailureStrategy(), STRAT_BRANCH_PREC);
	    break;
	  }
	case BranchStrategy::ITERATE:
	  {
	    Assert(b->getFailureAction() == BranchStrategy::IDLE &&
		   b->getSuccessStrategy() == 0 &&
		   b->getFailureStrategy() == 0, "unknown branch strategy");
	    if (latexPrintStrategy(s, b->getInitialStrategy(), 0))
	      s << "\\maudeSpace";
	    s << "\\maudeStratBang";
	    break;
	  }
	default:
	  CantHappen("bad success action");
	}
    }
   else if (TestStrategy* t = dynamic_cast<TestStrategy*>(strategy))
    {
      needParen = requiredPrec < STRAT_TEST_PREC;
      if (needParen)
	s << "\\maudeLeftParen";
      s << "\\maudeKeyword{";
      int depth = t->getDepth();
      if (depth >= 0)
	s << (depth == 0 ? 'x' : 'a');
      s << "match}\\maudeSpace";
      latexPrettyPrint(s, t->getPatternTerm());
      const Vector<ConditionFragment*>& condition = t->getCondition();
      if (!condition.empty())
	{
	  s << "\\maudeSpace\\maudeKeyword{such that}\\maudeSpace";
	  latexPrintCondition(s, condition);
	}
    }
  else if (SubtermStrategy* t = dynamic_cast<SubtermStrategy*>(strategy))
    {
      needParen = requiredPrec < STRAT_REW_PREC;
      if (needParen)
	s << "\\maudeLeftParen";
      s << "\\maudeKeyword{";
      int depth = t->getDepth();
      if (depth >= 0)
	s << (depth == 0 ? 'x' : 'a');
      s << "matchrew}\\maudeSpace";
      latexPrettyPrint(s, t->getPatternTerm());
      const Vector<ConditionFragment*>& condition = t->getCondition();
      if (!condition.empty())
	{
	  s << "\\maudeSpace\\maudeKeyword{such that}\\maudeSpace";
	  latexPrintCondition(s, condition);
	}
      const Vector<Term*>& subterms = t->getSubterms();
      const Vector<StrategyExpression*>& strategies = t->getStrategies();
      Index nrSubterms = subterms.size();
      for (Index i = 0; i < nrSubterms; ++i)
	{
	  s << ((i == 0) ? "\\maudeSpace\\maudeKeyword{by}\\maudeSpace" : "\\maudeComma");
	  latexPrettyPrint(s, subterms[i]);
	  s << "\\maudeSpace\\maudeKeyword{using}\\maudeSpace";
	  (void) latexPrintStrategy(s, strategies[i], STRAT_USING_PREC - 1);
	}
    }
  else if (CallStrategy* t = dynamic_cast<CallStrategy*>(strategy))
    latexPrintStrategyTerm(s, t->getStrategy(), t->getTerm());
  if (needParen)
    {
      s << "\\maudeRightParen";
      return false;
    }
  return true;
}
