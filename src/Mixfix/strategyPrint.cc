/*

    This file is part of the Maude 3 interpreter.

    Copyright 1997-2003 SRI International, Menlo Park, CA 94025, USA.

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
//	StrategyExpression* -> ostream& pretty printer.
//
//	It might appear desirable to virtualize this in class StrategyExpression but
//	the mapping from StrategyExpressions to the strategy language is partial and
//	it's undesirable for the various classes derived from StrategyExpression to
//	know about these details.
//

ostream&
operator<<(ostream& s, StrategyExpression* strategy)
{
  (void) MixfixModule::prettyPrint(s, strategy, UNBOUNDED);
  return s;
}


bool
MixfixModule::prettyPrint(ostream& s, StrategyExpression* strategy, int requiredPrec)
{
  bool needParen = false;
  bool needSpace = true;

  if (TrivialStrategy* t = dynamic_cast<TrivialStrategy*>(strategy))
    s << (t->getResult() ? "idle" : "fail");
  else if (OneStrategy* o = dynamic_cast<OneStrategy*>(strategy))
    s << "one(" << o->getStrategy() << ")";
  else if (ApplicationStrategy* a = dynamic_cast<ApplicationStrategy*>(strategy))
    {
      bool top = a->getTop();
      if (top)
	s << "top(";
      int label = a->getLabel();
      if (label == NONE)
	s << "all";
      else
	{
	  s << Token::name(label);
	  const Vector<Term*>& variables = a->getVariables();
	  if (!variables.empty())
	    {
	      const Vector<CachedDag>& values = a->getValues();
	      s << '[';
	      int nrAssignments = variables.size();
	      for (int i = 0;;)
		{
		  s << variables[i] << " <- " << values[i].getTerm();
		  if (++i == nrAssignments)
		    break;
	      s << ", ";
		}
	      s << ']';
	    }
	  const Vector<StrategyExpression*>& strategies = a->getStrategies();
	  if (!strategies.empty())
	    {
	      s << '{';
	      int nrStrategies = strategies.size();
	      for (int i = 0;;)
		{
		  s << strategies[i];
		  if (++i == nrStrategies)
		    break;
		  s << ", ";
		}
	      s << '}';
	    }
	}
      if (top)
	s << ')';
    }
  else if (ConcatenationStrategy* c = dynamic_cast<ConcatenationStrategy*>(strategy))
    {
      needParen = requiredPrec < STRAT_SEQ_PREC;
      if (needParen)
	s << '(';
      const Vector<StrategyExpression*>& strategies = c->getStrategies();
      int nrStrategies = strategies.size();
      for (int i = 0;;)
	{
	  (void) prettyPrint(s, strategies[i], STRAT_SEQ_PREC);
	  if (++i == nrStrategies)
	    break;
	  s << " ; ";
	}
    }
  else if (UnionStrategy* c = dynamic_cast<UnionStrategy*>(strategy))
    {
      needParen = requiredPrec < STRAT_UNION_PREC;
      if (needParen)
	s << '(';
      const Vector<StrategyExpression*>& strategies = c->getStrategies();
      int nrStrategies = strategies.size();
      for (int i = 0;;)
	{
	  (void) prettyPrint(s, strategies[i], STRAT_UNION_PREC);
	  if (++i == nrStrategies)
	    break;
	  s << " | ";
	}
    }
  else if (IterationStrategy* i = dynamic_cast<IterationStrategy*>(strategy))
    {
      if (prettyPrint(s, i->getStrategy(), 0))
	s << ' ';
      s << (i->getZeroAllowed() ? '*' : '+');
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
	    s << "not(" << b->getInitialStrategy() << ')';
	    break;
	  }
	case BranchStrategy::IDLE:
	  {
	    Assert(b->getFailureAction() == BranchStrategy::FAIL &&
		   b->getSuccessStrategy() == 0 &&
		   b->getFailureStrategy() == 0, "unknown branch strategy");
	    s << "test(" << b->getInitialStrategy() << ')';
	    break;
	  }
	case BranchStrategy::PASS_THRU:
	  {
	    if (b->getFailureAction() == BranchStrategy::IDLE)
	      {
		Assert(b->getSuccessStrategy() == 0 &&
		       b->getFailureStrategy() == 0, "unknown branch strategy");
		s << "try(" << b->getInitialStrategy() << ')';
	      }
	    else
	      {
		Assert(b->getFailureAction() == BranchStrategy::NEW_STRATEGY &&
		       b->getSuccessStrategy() == 0, "unknown branch strategy");
		needParen = requiredPrec < STRAT_ORELSE_PREC;
		if (needParen)
		  s << '(';
		(void) prettyPrint(s, b->getInitialStrategy(), STRAT_ORELSE_PREC);
		s << " or-else ";
		(void) prettyPrint(s, b->getFailureStrategy(), STRAT_ORELSE_PREC);
	      }
	    break;
	  }
	case BranchStrategy::NEW_STRATEGY:
	  {
	    Assert(b->getFailureAction() == BranchStrategy::NEW_STRATEGY, "unknown branch strategy");
	    needParen = requiredPrec < STRAT_BRANCH_PREC;
	    if (needParen)
	      s << '(';
	    (void) prettyPrint(s, b->getInitialStrategy(), STRAT_BRANCH_PREC);
	    s << " ? ";
	    (void) prettyPrint(s, b->getSuccessStrategy(), STRAT_BRANCH_PREC);
	    s << " : ";
	    (void) prettyPrint(s, b->getFailureStrategy(), STRAT_BRANCH_PREC);
	    break;
	  }
	case BranchStrategy::ITERATE:
	  {
	    Assert(b->getFailureAction() == BranchStrategy::IDLE &&
		   b->getSuccessStrategy() == 0 &&
		   b->getFailureStrategy() == 0, "unknown branch strategy");
	    if (prettyPrint(s, b->getInitialStrategy(), 0))
	      s << ' ';
	    s << '!';
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
	s << '(';
      int depth = t->getDepth();
      if (depth >= 0)
	s << (depth == 0 ? 'x' : 'a');
      s << "match " << t->getPatternTerm();
      const Vector<ConditionFragment*>& condition = t->getCondition();
      if (!condition.empty())
	{
	  s << " such that ";
	  printCondition(s, condition);
	}
    }
  else if (SubtermStrategy* t = dynamic_cast<SubtermStrategy*>(strategy))
    {
      needParen = requiredPrec < STRAT_REW_PREC;
      if (needParen)
	s << '(';
      int depth = t->getDepth();
      if (depth >= 0)
	s << (depth == 0 ? 'x' : 'a');
      s << "matchrew " << t->getPatternTerm();
      const Vector<ConditionFragment*>& condition = t->getCondition();
      if (!condition.empty())
	{
	  s << " such that ";
	  printCondition(s, condition);
	}
      const Vector<Term*>& subterms = t->getSubterms();
      const Vector<StrategyExpression*>& strategies = t->getStrategies();
      int nrSubterms = subterms.size();
      for (int i = 0; i < nrSubterms; ++i)
	{
	  s << ((i == 0) ? " by " : ", ");
	  s << subterms[i] << " using ";
	  (void) prettyPrint(s, strategies[i], STRAT_USING_PREC - 1);
	}
    }
  else if (CallStrategy* t = dynamic_cast<CallStrategy*>(strategy))
    {
      MixfixModule* m = safeCast(MixfixModule*, t->getStrategy()->getModule());
      m->printStrategyTerm(s, t->getStrategy(), t->getTerm());
    }
  if (needParen)
    {
      s << ')';
      needSpace = false;
    }
  return needSpace;
}
