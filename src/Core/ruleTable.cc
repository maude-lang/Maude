/*

    This file is part of the Maude 3 interpreter.

    Copyright 1997-2023 SRI International, Menlo Park, CA 94025, USA.

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
//      Implementation for abstract class RuleTable.
//

//	utility stuff
#include "macros.hh"
#include "vector.hh"

//      forward declarations
#include "interface.hh"
#include "core.hh"

//      interface class definitions
//#include "symbol.hh"
//#include "dagNode.hh"
#include "term.hh"
#include "subproblem.hh"
#include "extensionInfo.hh"

//      core class definitions
#include "rewritingContext.hh"
#include "lhsAutomaton.hh"
#include "rhsAutomaton.hh"
#include "rule.hh"
#include "ruleTable.hh"

RuleTable::RuleTable()
{
  nextRule = 0;
}

void
RuleTable::compileRules()
{
  int nrRules = rules.length();
  for (int i = 0; i < nrRules; i++)
    rules[i]->compile(true);
}

void
RuleTable::resetRules()
{
  nextRule = 0;
}

DagNode*
RuleTable::ruleRewrite(DagNode* subject, RewritingContext& context)
{
  return applyRules(subject, context, 0);
}

DagNode*
RuleTable::applyRules(DagNode* subject,
		      RewritingContext& context,
		      ExtensionInfo* extensionInfo)
{
  Assert(subject->getSortIndex() != Sort::SORT_UNKNOWN, "sort unknown");
  int nrRules = rules.length();
  int n = nextRule;
  for (int i = 0; i < nrRules; i++, n++)
    {
      if (n >= nrRules)
	n = n - nrRules;
      Rule* rl = rules[n];
      if (rl->isNonexec())
	continue;
      int nrVariables = rl->getNrProtectedVariables();
      context.clear(nrVariables);
      Subproblem* sp;
      if (rl->getLhsAutomaton()->match(subject, context, sp, extensionInfo))
	{
	  if (sp == 0 || sp->solve(true, context))
	    {
	      if (!(rl->hasCondition()) || rl->checkCondition(subject, context, sp))
		{
		  DagNode* r;
		  if (RewritingContext::getTraceStatus())
		    {
		      context.tracePreRuleRewrite(subject, rl);
		      if (context.traceAbort())
			{
			  delete sp;
			  context.finished();
			  return subject;
			}
		    }
		  if (extensionInfo == 0 || extensionInfo->matchedWhole())
		    r =  rl->getRhsBuilder().construct(context);
		  else
		    {
		      r = subject->partialConstruct(rl->getRhsBuilder().construct(context),
						    extensionInfo);
		    }
		  context.incrementRlCount();
		  delete sp;
		  context.finished();
		  nextRule = n + 1;
		  return r;
		}
	    }
	  delete sp;
	}
      context.finished();
    }
  subject->setUnrewritable();
  return 0;
}

void
RuleTable::resetEachRule()
{
  for (Rule* r : rules)
    r->reset();
}
