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

#include <unistd.h>  // HACK

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
bool UserLevelRewritingContext::traceConditionFlag = true;
bool UserLevelRewritingContext::traceWholeFlag = false;
bool UserLevelRewritingContext::traceSubstitutionFlag = true;
bool UserLevelRewritingContext::traceSelectFlag = false;
bool UserLevelRewritingContext::traceScFlag = true;
bool UserLevelRewritingContext::traceEqFlag = true;
bool UserLevelRewritingContext::traceRuleFlag = true;
const char UserLevelRewritingContext::header[] = "*********** ";
IntSet UserLevelRewritingContext::selected;
IntSet UserLevelRewritingContext::excluded;

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
				       (purpose != CONDITION_EVAL || traceConditionFlag));
}

void
UserLevelRewritingContext::selectSymbols(const IntSet& symbols, bool add)
{
  if (add)
    selected.insert(symbols);
  else
    selected.subtract(symbols);
}

void
UserLevelRewritingContext::excludeModules(const IntSet& modules, bool add)
{
  if (add)
    excluded.insert(modules);
  else
    excluded.subtract(modules);
}

bool
UserLevelRewritingContext::dontTrace(const DagNode* redex, const PreEquation* pe)
{
  //  if (!traceRuleFlag)
  //    return true;
  Symbol* symbol = redex->symbol();
  return (traceSelectFlag &&
	  !(selected.contains(symbol->id()) ||
	    (pe != 0 && selected.contains(pe->getLabel().id())))) ||
    excluded.contains(symbol->getModule()->id());
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
  if (handleDebug(redex, equation) || !localTraceFlag || !traceEqFlag || dontTrace(redex, equation))
    {
      tracePostFlag = false;
      return;
    }
  tracePostFlag = true;
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
      cout << equation << '\n';
      if (traceSubstitutionFlag)
	printSubstitution(*this, *equation);
    }
  if (traceWholeFlag)
    cout << "Old: " << root() << '\n';
  cout << redex << "\n--->\n";
}

void
UserLevelRewritingContext::tracePostEqRewrite(DagNode* replacement)
{
  if (tracePostFlag)
    {
      Assert(!abortFlag, "abort flag set");
      cout << replacement << '\n';
      if (traceWholeFlag)
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
  if (handleDebug(redex, rule) || !localTraceFlag || !traceRuleFlag || dontTrace(redex, rule))
    {
      tracePostFlag = false;
      return;
    }
  tracePostFlag = true;
  cout << header << "rule\n";
  if (rule == 0)
    cout << "(built-in rule for symbol " << redex->symbol() << ")\n";
  else
    {
      cout << rule << '\n';
      if (traceSubstitutionFlag)
	printSubstitution(*this, *rule);
    }
  if (traceWholeFlag)
    cout << "Old: " << root() << '\n';
  cout << redex << "\n--->\n";
}

void
UserLevelRewritingContext::tracePostRuleRewrite(DagNode* replacement)
{
  if (tracePostFlag)
    {
      cout << replacement << '\n';
      if (traceWholeFlag)
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
  if (handleDebug(subject, sc) || !localTraceFlag || !traceScFlag || dontTrace(subject, sc))
    return;
  cout << header << "membership axiom\n";
  if (sc == 0)
    cout << "(built-in membership axiom for symbol " << subject->symbol() << ")\n";
  else
    {
      cout << sc << '\n';
      if (traceSubstitutionFlag)
	printSubstitution(*this, *sc);
    }
  if (traceWholeFlag)
    cout << "Whole: " << root() << '\n';
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
	  Assert(v != 0, "null variable");
	  cout << v << " --> ";
	  DagNode* d = substitution.value(i);
	  if (d == 0)
	    cout << "(unbound)\n";
	  else
	    cout << d << '\n';
	}
    }
}
