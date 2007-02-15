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
//      Implementation for class PreModule.
//

//      utility stuff
#include "macros.hh"
#include "vector.hh"
#include "timer.hh"

//      forward declarations
#include "interface.hh"
#include "core.hh"
#include "variable.hh"
#include "builtIn.hh"
#include "freeTheory.hh"
#include "AU_Theory.hh"
#include "higher.hh"
#include "strategyLanguage.hh"
#include "mixfix.hh"

//      interface class definitions
#include "term.hh"
#include "lhsAutomaton.hh"
#include "extensionInfo.hh"
#include "subproblem.hh"
#include "binarySymbol.hh"
 
//      core class definitions
#include "lineNumber.hh"
#include "equation.hh"
#include "rule.hh"
#include "sortConstraint.hh"
#include "module.hh"
#include "rewritingContext.hh"
#include "argumentIterator.hh"
#include "dagArgumentIterator.hh"
//#include "dagNodeSet.hh"

//	front end class definitions
#include "moduleExpression.hh"
#include "fileTable.hh"
#include "userLevelRewritingContext.hh"
#include "quotedIdentifierSymbol.hh"
#include "token.hh"
#include "loopSymbol.hh"
#include "visibleModule.hh"
#include "view.hh"
#include "preModule.hh"
#include "interpreter.hh"
#include "maudemlBuffer.hh"
#include "global.hh"  // HACK shouldn't be accessing global variables

#ifdef QUANTIFY_PROCESSING
#include "quantify.h"
#endif

//	our stuff
#include "import.cc"
#include "process.cc"
#include "fixUp.cc"
#include "ops.cc"
#include "command.cc"

PreModule::PreModule(Token startToken, Token moduleName)
  : NamedEntity(moduleName.code()),
    LineNumber(moduleName.lineNumber()),
    startTokenCode(startToken.code())
{
  if (startTokenCode == th)
    moduleType = MixfixModule::SYSTEM_THEORY;
  else if (startTokenCode == fth)
    moduleType = MixfixModule::FUNCTIONAL_THEORY;
  else if (startTokenCode == mod || startTokenCode == omod || startTokenCode == smod)
    moduleType = MixfixModule::SYSTEM_MODULE;
  else
    moduleType = MixfixModule::FUNCTIONAL_MODULE;

  lastSawOpDecl = false;
  isCompleteFlag = false;
  flatModule = 0;
}

PreModule::~PreModule()
{
  if (flatModule != 0)
    flatModule->deepSelfDestruct();
  {
    FOR_EACH_CONST(i, Vector<Import>, imports)
      i->expr->deepSelfDestruct();
  }
  {
    FOR_EACH_CONST(i, Vector<Parameter>, parameters)
      i->theory->deepSelfDestruct();
  }
}

void
PreModule::regretToInform(Entity* doomedEntity)
{
  Assert(doomedEntity == flatModule, "module pointer error");
  flatModule = 0;
#ifdef COMPILER
  interpreter.invalidate(this);
#endif
}

VisibleModule*
PreModule::getFlatModule()
{
  VisibleModule* m = getFlatSignature();
  if (m->getStatus() != Module::THEORY_CLOSED)
    {
      //
      //	Need to flatten in statements and compile.
      //
      m->importStatements();
      if (m->isBad())
	{
	  IssueWarning(*m <<
		       ": this module contains one or more errors that \
could not be patched up and thus it cannot be used or imported.");
	}
      else
	m->closeTheory();
      m->resetImports();
    }
  return m;
}

VisibleModule*
PreModule::getFlatSignature() 
{
  if (flatModule == 0)
    {
      IssueAdvisory("reparsing module " << QUOTE(this) <<
		    " due to changes in imported modules.");
      process();
    }
  else if (flatModule->getStatus() == Module::OPEN)
    return 0;  // we must already be in the middle of processing this module
  return flatModule;
}

bool
PreModule::compatible(int endTokenCode)
{
  if (startTokenCode == th)
    return endTokenCode == endth;
  if (startTokenCode == fth)
    return endTokenCode == endfth;
  if (startTokenCode == mod)
    return endTokenCode == endm;
  if (startTokenCode == fmod)
    return endTokenCode == endfm;
  if (startTokenCode == smod)
    return endTokenCode == endsm;
  if (startTokenCode == omod)
    return endTokenCode == endom;
  //
  //	OBJ backward compatibility.
  //
  return endTokenCode == endo || endTokenCode == jbo;
}

void
PreModule::finishModule(Token endToken)
{
  if (!compatible(endToken.code()))
    {
      IssueWarning(LineNumber(endToken.lineNumber()) << ": module started with " <<
		   QUOTE(Token::name(startTokenCode)) << " ends with "
		   << QUOTE(endToken) << '.');
    }
  autoImports = interpreter.getAutoImports(); // deep copy
  isCompleteFlag = true;
  interpreter.insertModule(id(), this);
  process();
}

PreModule::OpDef::OpDef()
{
  prec = DEFAULT;
  metadata = NONE;
}

void
PreModule::addParameter(Token name, ModuleExpression* theory)
{
  if (MixfixModule::isTheory(moduleType))
    {
      IssueWarning(LineNumber(name.lineNumber()) <<
		   ": parmaeterized theories are not supported; recovering by ignoring parameter " <<
		   QUOTE(name) << '.');
      delete theory;
      return;
    }
  int nrParameters = parameters.length();
  parameters.resize(nrParameters + 1);
  parameters[nrParameters].name = name;
  parameters[nrParameters].theory = theory;
}

void
PreModule::addImport(Token mode, ModuleExpression* expr)
{
  int nrImports = imports.length();
  imports.resize(nrImports + 1);
  imports[nrImports].mode = mode;
  imports[nrImports].expr = expr;
}

void
PreModule::addStatement(const Vector<Token>& statement)
{
  int keywordCode = statement[0].code();
  if (keywordCode == rl || keywordCode == crl)
    {
      if (moduleType == MixfixModule::FUNCTIONAL_THEORY)
	{
	  IssueWarning(LineNumber(statement[0].lineNumber()) <<
		       ": rule not allowed in a functional theory.");
	}
      else if (moduleType == MixfixModule::FUNCTIONAL_MODULE)
	{
	  IssueWarning(LineNumber(statement[0].lineNumber()) <<
		       ": rule not allowed in a functional module.");
	}
    }

  if (statement[1].code() == leftBracket &&
      statement[3].code() == rightBracket &&
      statement[4].code() == colon)
    (void) potentialLabels.insert(statement[2].code());

  int i = statement.length() - 1;
  if (statement[i].code() == rightBracket)
    {
      int bracketCount = 1;
      for (i -= 2; i >= 4; i--)
	{
	  int t = statement[i].code();
	  if (t == leftBracket)
	    {
	      --bracketCount;
	      if (bracketCount == 0)
		break;
	    }
	  else if (t == label)
	    potentialLabels.insert(statement[i+1].code());
	  else if (t == rightBracket)
	    ++bracketCount;
	}
    }
  statements.append(statement);
}
