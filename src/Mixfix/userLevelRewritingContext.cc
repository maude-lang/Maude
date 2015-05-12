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
#include "bddUser.hh"
 
//      forward declarations
#include "interface.hh"
#include "core.hh"
#include "variable.hh"
#include "strategyLanguage.hh"
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
#include "narrowingVariableInfo.hh"

//      variable class definitions
#include "variableTerm.hh"
#include "variableDagNode.hh"

//	front end class definitions
#include "token.hh"
#include "userLevelRewritingContext.hh"
//#include "preModule.hh"  // HACK
#include "autoWrapBuffer.hh"

#include "interpreter.hh"  // HACK
#include "global.hh"  // HACK shouldn't be accessing global variables

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
				       (purpose != CONDITION_EVAL || interpreter.getFlag(Interpreter::TRACE_CONDITION)));
}

bool
UserLevelRewritingContext::dontTrace(const DagNode* redex, const PreEquation* pe)
{
  Symbol* symbol = redex->symbol();
  return (interpreter.getFlag(Interpreter::TRACE_SELECT) &&
	  !(interpreter.traceId(symbol->id()) ||
	    (pe != 0 && interpreter.traceId(pe->getLabel().id())))) ||
    interpreter.excludedModule(symbol->getModule()->id()) ||
    (pe == 0 && !interpreter.getFlag(Interpreter::TRACE_BUILTIN));
}

void
UserLevelRewritingContext::checkForPrintAttribute(MixfixModule::ItemType itemType, const PreEquation* item)
{
  if (item != 0)
    {
      MixfixModule* m = safeCast(MixfixModule*, item->getModule());
      const PrintAttribute* pa = m->getPrintAttribute(itemType, item);
      if (pa != 0)
	{
	  pa->print(cout, *this);
	  if (interpreter.getFlag(Interpreter::PRINT_ATTRIBUTE_NEWLINE))
	    cout << '\n';
	  cout.flush();
	}
    }
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
  if (interpreter.getFlag(Interpreter::PRINT_ATTRIBUTE))
    checkForPrintAttribute(MetadataStore::EQUATION, equation);

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
  DebugAdvisory(static_cast<void*>(redex));
}

void
UserLevelRewritingContext::tracePostEqRewrite(DagNode* replacement)
{
  if (tracePostFlag)
    {
      Assert(!abortFlag, "abort flag set");
      if (interpreter.getFlag(Interpreter::TRACE_REWRITE))
	cout << replacement << '\n';
      DebugAdvisory(static_cast<void*>(replacement));
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
  if (interpreter.getFlag(Interpreter::PRINT_ATTRIBUTE))
    checkForPrintAttribute(MetadataStore::RULE, rule);

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
UserLevelRewritingContext::traceNarrowingStep(Rule* rule,
					      DagNode* redex,
					      DagNode* replacement,
					      const NarrowingVariableInfo* variableInfo,
					      const Substitution* substitution,
					      DagNode* newState)
{
  if (handleDebug(redex, rule) ||
      !localTraceFlag ||
      !(interpreter.getFlag(Interpreter::TRACE_RL)) ||
      dontTrace(redex, rule))
    return;

  if (interpreter.getFlag(Interpreter::TRACE_BODY))
    {
      cout << Tty(Tty::MAGENTA) << header << "narrowing step\n" << Tty(Tty::RESET) << rule << '\n';
      if (interpreter.getFlag(Interpreter::TRACE_SUBSTITUTION))
	{
	  cout << "Rule variable bindings:\n";
	  printSubstitution(*substitution, *rule);

	  cout << "Subject variable bindings:\n";
	  int nrSubjectVariables = variableInfo->getNrVariables();
	  if (nrSubjectVariables == 0)
	    cout << "empty substitution\n";
	  else
	    {
	      int variableBase = rule->getModule()->getMinimumSubstitutionSize();
	      for (int i = 0; i < nrSubjectVariables; ++i)
		{
		  DagNode* v = variableInfo->index2Variable(i);
		  DagNode* d = substitution->value(variableBase + i);
		  Assert(v != 0, "null variable");
		  cout << v << " --> ";
		  if (d == 0)
		    cout << "(unbound)\n";
		  else
		    cout << d << '\n';
		}
	    }
	}
    }
  if (interpreter.getFlag(Interpreter::TRACE_WHOLE))
    cout << "Old: " << root() << '\n';
  if (interpreter.getFlag(Interpreter::TRACE_REWRITE))
    cout << redex << "\n--->\n" << replacement << '\n';
  if (interpreter.getFlag(Interpreter::TRACE_WHOLE))
    cout << "New: " << newState << '\n';
}

void
UserLevelRewritingContext::traceVariantNarrowingStep(Equation* equation,
						     const Vector<DagNode*>& oldVariantSubstitution,
						     DagNode* redex,
						     DagNode* replacement,
						     const NarrowingVariableInfo& variableInfo,
						     const Substitution* substitution,
						     DagNode* newState,
						     const Vector<DagNode*>& newVariantSubstitution,
						     const NarrowingVariableInfo& originalVariables)
{
  if (handleDebug(redex, equation) ||
      !localTraceFlag ||
      !(interpreter.getFlag(Interpreter::TRACE_EQ)) ||
      dontTrace(redex, equation))
    return;

  if (interpreter.getFlag(Interpreter::TRACE_BODY))
    {
      cout << Tty(Tty::CYAN) << header << "variant narrowing step\n" << Tty(Tty::RESET) << equation << '\n';
      if (interpreter.getFlag(Interpreter::TRACE_SUBSTITUTION))
	{
	  cout << "Equation variable bindings:\n";
	  printSubstitution(*substitution, *equation);

	  cout << "Old variant variable bindings:\n";
	  int nrSubjectVariables = variableInfo.getNrVariables();
	  if (nrSubjectVariables == 0)
	    cout << "empty substitution\n";
	  else
	    {
	      int variableBase = equation->getModule()->getMinimumSubstitutionSize();
	      for (int i = 0; i < nrSubjectVariables; ++i)
		{
		  DagNode* v = variableInfo.index2Variable(i);
		  DagNode* d = substitution->value(variableBase + i);
		  Assert(v != 0, "null variable");
		  /*
		  DebugAdvisory(static_cast<void*>(v) << " --> " <<
				static_cast<void*>(d) << " / " <<
				static_cast<void*>(d->symbol()));
		  */

		  cout << v << " --> ";
		  if (d == 0)
		    cout << "(unbound)\n";
		  else
		    cout << d << '\n';
		}
	    }
	}
    }
  if (interpreter.getFlag(Interpreter::TRACE_WHOLE))
    {
      cout << "\nOld variant: " << root() << '\n';
      int nrBindings = oldVariantSubstitution.size();
      for (int i = 0; i < nrBindings; ++i)
	{
	  DagNode* v = originalVariables.index2Variable(i);
	  DagNode* d = oldVariantSubstitution[i];
	  cout << v << " --> " << d << '\n';
	}
      cout << '\n';
    }
  if (interpreter.getFlag(Interpreter::TRACE_REWRITE))
    cout << redex << "\n--->\n" << replacement << '\n';
  if (interpreter.getFlag(Interpreter::TRACE_WHOLE))
    {
      cout << "\nNew variant: " << newState << '\n';
      int nrBindings = newVariantSubstitution.size();
      for (int i = 0; i < nrBindings; ++i)
	{
	  DagNode* v = originalVariables.index2Variable(i);
	  DagNode* d = newVariantSubstitution[i];
	  cout << v << " --> " << d << '\n';
	}
      cout << '\n';
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
  if (interpreter.getFlag(Interpreter::PRINT_ATTRIBUTE))
    checkForPrintAttribute(MetadataStore::MEMB_AX, sc);

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
  //
  //	Coverity discovered bug - sc could be 0.
  //
  if (interpreter.getFlag(Interpreter::TRACE_REWRITE))
    cout << subject->getSort() << ": " << subject << " becomes " << sc->getSort() << '\n';  // BUG
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
	  DebugAdvisory(static_cast<void*>(v) << " --> " <<
			static_cast<void*>(d) << " / " <<
			static_cast<void*>(d->symbol()));
	  Assert(v != 0, "null variable");
	  cout << v << " --> ";
	  if (d == 0)
	    cout << "(unbound)\n";
	  else
	    cout << d << '\n';
	}
    }
}
