/*

    This file is part of the Maude 2 interpreter.

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
//      Implementation for class UserLevelRewritingContext
//

//#include <unistd.h>  // HACK

//      utility stuff
#include "macros.hh"
#include "vector.hh"
#include "pointerSet.hh"
 
//      forward declarations
#include "interface.hh"
#include "core.hh"
#include "variable.hh"
#include "mixfix.hh"

//      interface class definitions
#include "symbol.hh"
#include "dagNode.hh"
#include "higher.hh"
 
//      core class definitions
#include "redexPosition.hh"
#include "variableSymbol.hh"
#include "rewritingContext.hh"
#include "conditionFragment.hh"
#include "sortConstraint.hh"
#include "equation.hh"
#include "rule.hh"

//      variable class definitions
#include "variableTerm.hh"

//	front end class definitions
#include "token.hh"
#include "userLevelRewritingContext.hh"
#include "preModule.hh"  // HACK
#include "autoWrapBuffer.hh"

#include "interpreter.hh"  // HACK
#include "main.hh"  // HACK shouldn't be accessing global variables

//	our stuff
#include "interact.cc"
#include "trial.cc"

bool UserLevelRewritingContext::tracePostFlag = false;
const char UserLevelRewritingContext::header[] = "*********** ";

UserLevelRewritingContext::UserLevelRewritingContext(DagNode* root)
  : ObjectSystemRewritingContext(root),
    parent(0),
    purpose(TOP_LEVEL_EVAL),
    localTraceFlag(true)
{
}

UserLevelRewritingContext::UserLevelRewritingContext(DagNode* root,
						     UserLevelRewritingContext* parent,
						     int purpose,
						     bool localTraceFlag)
  : ObjectSystemRewritingContext(root),
    parent(parent),
    purpose(purpose),
    localTraceFlag(localTraceFlag)
{
}

RewritingContext*
UserLevelRewritingContext::makeSubcontext(DagNode* root, int purpose)
{
  return new UserLevelRewritingContext(root, this, purpose,
				       localTraceFlag &&
				       (purpose != CONDITION_EVAL || interpreter.getFlag(Interpreter::TRACE_EQ)));
}

bool
UserLevelRewritingContext::dontTrace(const DagNode* redex, const PreEquation* pe)
{
  Symbol* symbol = redex->symbol();
  return (interpreter.getFlag(Interpreter::TRACE_SELECT) &&
	  !(interpreter.traceId(symbol->id()) ||
	    (pe != 0 && interpreter.traceId(pe->getLabel().id())))) ||
    interpreter.excludedModule(symbol->getModule()->id());
}

void
UserLevelRewritingContext::tracePreEqRewrite(DagNode* redex,
					     const Equation* equation,
					     int type)
{
  if (interpreter.getFlag(Interpreter::PROFILE))
    {
      safeCast(ProfileModule*, root()->symbol()->getModule())->
	profileEqRewrite(redex, equation, type);
    }
  if (handleDebug(redex, equation) ||
      !localTraceFlag ||
      !(interpreter.getFlag(Interpreter::TRACE_EQ)) ||
      dontTrace(redex, equation))
    {
      tracePostFlag = false;
      return;
    }
  tracePostFlag = true;

  if (interpreter.getFlag(Interpreter::TRACE_BODY))
    cout << header << "equation\n";
  if (equation == 0)
    {
      if (type == RewritingContext::BUILTIN)
	cout << "(built-in equation for symbol " << redex->symbol() << ")\n";
      else if (type == RewritingContext::MEMOIZED)
	cout << "(memo table lookup for symbol " << redex->symbol() << ")\n";
    }
  else
    {
      if (interpreter.getFlag(Interpreter::TRACE_BODY))
	{
	  cout << equation << '\n';
	  if (interpreter.getFlag(Interpreter::TRACE_SUBSTITUTION))
	    printSubstitution(*this, *equation);
	}
      else
	{
	  const Label& label = equation->getLabel();
	  if (label.id() == NONE)
	    cout << "(unlabeled equation)\n";
	  else
	    cout << &label << '\n';
	}
    }
  if (interpreter.getFlag(Interpreter::TRACE_WHOLE))
    cout << "Old: " << root() << '\n';
  if (interpreter.getFlag(Interpreter::TRACE_REWRITE))
    cout << redex << "\n--->\n";
}

void
UserLevelRewritingContext::tracePostEqRewrite(DagNode* replacement)
{
  if (tracePostFlag)
    {
      Assert(!abortFlag, "abort flag set");
      if (interpreter.getFlag(Interpreter::TRACE_REWRITE))
	cout << replacement << '\n';
      if (interpreter.getFlag(Interpreter::TRACE_WHOLE))
	cout << "New: " << root() << '\n';
    }
}

void
UserLevelRewritingContext::tracePreRuleRewrite(DagNode* redex, const Rule* rule)
{
  if (interpreter.getFlag(Interpreter::PROFILE))
    {
      safeCast(ProfileModule*, root()->symbol()->getModule())->
	profileRlRewrite(redex, rule);
    }
  if (handleDebug(redex, rule) ||
      !localTraceFlag ||
      !(interpreter.getFlag(Interpreter::TRACE_RL)) ||
      dontTrace(redex, rule))
    {
      tracePostFlag = false;
      return;
    }
  tracePostFlag = true;
  if (interpreter.getFlag(Interpreter::TRACE_BODY))
    cout << header << "rule\n";
  if (rule == 0)
    cout << "(built-in rule for symbol " << redex->symbol() << ")\n";
  else
    {
      if (interpreter.getFlag(Interpreter::TRACE_BODY))
	{
	  cout << rule << '\n';
	  if (interpreter.getFlag(Interpreter::TRACE_SUBSTITUTION))
	    printSubstitution(*this, *rule);
	}
      else
	{
	  const Label& label = rule->getLabel();
	  if (label.id() == NONE)
	    cout << "(unlabeled rule)\n";
	  else
	    cout << &label << '\n';
	}
    }
  if (interpreter.getFlag(Interpreter::TRACE_WHOLE))
    cout << "Old: " << root() << '\n';
  if (interpreter.getFlag(Interpreter::TRACE_REWRITE))
    cout << redex << "\n--->\n";
}

void
UserLevelRewritingContext::tracePostRuleRewrite(DagNode* replacement)
{
  if (tracePostFlag)
    {
      if (interpreter.getFlag(Interpreter::TRACE_REWRITE))
	cout << replacement << '\n';
      if (interpreter.getFlag(Interpreter::TRACE_WHOLE))
	cout << "New: " << root() << '\n';
    }
}

void
UserLevelRewritingContext::tracePreScApplication(DagNode* subject, const SortConstraint* sc)
{
  if (interpreter.getFlag(Interpreter::PROFILE))
    {
      safeCast(ProfileModule*, root()->symbol()->getModule())->
	profileMbRewrite(subject, sc);
    }
  if (handleDebug(subject, sc) ||
      !localTraceFlag ||
      !(interpreter.getFlag(Interpreter::TRACE_MB)) ||
      dontTrace(subject, sc))
    return;
  if (interpreter.getFlag(Interpreter::TRACE_BODY))
    cout << header << "membership axiom\n";
  if (sc == 0)
    cout << "(built-in membership axiom for symbol " << subject->symbol() << ")\n";
  else
    {
      if (interpreter.getFlag(Interpreter::TRACE_BODY))
	{
	  cout << sc << '\n';
	  if (interpreter.getFlag(Interpreter::TRACE_SUBSTITUTION))
	    printSubstitution(*this, *sc);
	}
      else
	{
	  const Label& label = sc->getLabel();
	  if (label.id() == NONE)
	    cout << "(unlabeled membership axiom)\n";
	  else
	    cout << &label << '\n';
	}
    }
  if (interpreter.getFlag(Interpreter::TRACE_WHOLE))
    cout << "Whole: " << root() << '\n';
  if (interpreter.getFlag(Interpreter::TRACE_REWRITE))
    cout << subject->getSort() << ": " << subject << " becomes " << sc->getSort() << '\n';
}

void
UserLevelRewritingContext::printSubstitution(const Substitution& substitution,
					     const VariableInfo& varInfo)
{
  int nrVars = varInfo.getNrRealVariables();
  if (nrVars == 0)
    cout << "empty substitution\n";
  else
    {
      for (int i = 0; i < nrVars; i++)
	{
	  Term* v = varInfo.index2Variable(i);
	  DagNode* d = substitution.value(i);
	  //DebugAdvisory(static_cast<void*>(v) << " --> " <<
	  //	static_cast<void*>(d) << " / " <<
	  //	static_cast<void*>(d->symbol()));
	  Assert(v != 0, "null variable");
	  cout << v << " --> ";
	  if (d == 0)
	    cout << "(unbound)\n";
	  else
	    cout << d << '\n';
	}
    }
}
