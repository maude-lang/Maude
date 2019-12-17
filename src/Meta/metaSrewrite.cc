/*

    This file is part of the Maude 3 interpreter.

    Copyright 2019 SRI International, Menlo Park, CA 94025, USA.

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

bool
MetaLevelOpSymbol::metaSrewrite(FreeDagNode* subject,
				RewritingContext& context)
{
  if (MetaModule* m = metaLevel->downModule(subject->getArgument(0)))
    {
      bool depthFirst;
      Int64 solutionNr;
      if (metaLevel->downSrewriteOption(subject->getArgument(3), depthFirst) &&
	  metaLevel->downSaturate64(subject->getArgument(4), solutionNr) &&
	  solutionNr >= 0)
	{
	  StrategicSearch* state;
	  Int64 lastSolutionNr;
	  if (m->getCachedStateObject(subject, context, solutionNr, state, lastSolutionNr))
	    m->protect();  // Use cached state
	  else if ((state = makeStrategicSearch(m, subject, context, depthFirst)))
	    lastSolutionNr = -1;
	  else
	    return false;

	  DagNode* result = state->lastSolution();
	  while (lastSolutionNr < solutionNr)
	    {
	      result = state->findNextSolution();
	      if (result == 0)
		{
		  delete state;
		  break;
		}
	      ++lastSolutionNr;
	    }
	  if (result != 0)
	    {
	      m->insert(subject, state, solutionNr);
	      result = metaLevel->upResultPair(result, m);
	    }
	  else
	    result = metaLevel->upFailurePair();
	  (void) m->unprotect();
	  return context.builtInReplace(subject, result);
	}
      else
	{
	  DebugAdvisory("bad bound " << QUOTE(subject->getArgument(3)));
	}
    }
  else
    {
      DebugAdvisory("bad metamodule " << QUOTE(subject->getArgument(0)));
    }

  return false;
}

StrategicSearch*
MetaLevelOpSymbol::makeStrategicSearch(MetaModule* m,
				       FreeDagNode* subject,
				       RewritingContext& context,
				       bool depthFirst) const
{
  if (Term* t = metaLevel->downTerm(subject->getArgument(1), m))
    {
      if (StrategyExpression* s = metaLevel->downStratExpr(subject->getArgument(2), m))
	{
	  // Prepares the strategy
	  TermSet boundVars;
	  VariableInfo vinfo;
	  if (s->check(vinfo, boundVars))
	    {
	      m->protect();
	      s->process();

	      // Prepares the term
	      RewritingContext* objectContext = term2RewritingContext(t, context);
	      objectContext->reduce();
	      if (depthFirst)
		return new DepthFirstStrategicSearch(objectContext, s);
	      else
		return new FairStrategicSearch(objectContext, s);
	    }
	  else
	    {
	      DebugAdvisory("bad metastrategy (check)" << QUOTE(s));
	      delete s;
	      t->deepSelfDestruct();
	    }
	}
      else
	{
	  DebugAdvisory("bad metastrategy " << QUOTE(subject->getArgument(2)));
	  t->deepSelfDestruct();
	}
    }
  else
    {
      DebugAdvisory("bad metaterm " << QUOTE(subject->getArgument(1)));
    }
  return 0;
}
