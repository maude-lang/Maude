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

#ifdef QUANTIFY_PROCESSING
#include "quantify.h"
#endif

PreModule::PreModule(int moduleName, Interpreter* owner)
  : NamedEntity(moduleName),
    owner(owner)
{
}

PreModule::~PreModule()
{
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
PreModule::addParameter(Token name, ModuleExpression* theory)
{
  if (isTheory())
    {
      IssueWarning(LineNumber(name.lineNumber()) <<
		   ": parmaeterized theories are not supported; recovering by ignoring parameter " <<
		   QUOTE(name) << '.');
      cout << "preModule = " << this << "  theory = " << theory << endl;
      delete theory;
      return;
    }
  int nrParameters = parameters.length();
  parameters.resize(nrParameters + 1);
  parameters[nrParameters].name = name;
  parameters[nrParameters].theory = theory;
}

void
PreModule::addImport(LineNumber lineNumber, ImportModule::ImportMode mode, ModuleExpression* expr)
{
  int nrImports = imports.length();
  imports.resize(nrImports + 1);
  imports[nrImports].lineNumber = lineNumber;
  imports[nrImports].mode = mode;
  imports[nrImports].expr = expr;
}

void
PreModule::processParameters(ImportModule* flatModule)
{
  FOR_EACH_CONST(i, Vector<Parameter>, parameters)
    {
      if (ImportModule* fm = owner->makeModule(i->theory))
	{
	  if (MixfixModule::canHaveAsParameter(getModuleType(), fm->getModuleType()))
	    {
	      ImportModule* parameterCopy = owner->makeParameterCopy(i->name.code(), fm);
	      if (parameterCopy != 0)
		flatModule->addParameter(i->name, parameterCopy);
	      else
		{
		  flatModule->markAsBad();
		  return;
		}
	    }
	  else
	    {
	      IssueWarning(LineNumber(i->name.lineNumber()) <<
			   ": parameterization of " << 
			   QUOTE(MixfixModule::moduleTypeString(getModuleType())) <<
			   " " << this << " by " <<
			   QUOTE(MixfixModule::moduleTypeString(fm->getModuleType())) <<
			   " " << fm << " is not allowed.");
	      flatModule->markAsBad();
	    }
	}
    }
}

void
PreModule::processExplicitImports(ImportModule* flatModule)
{
  FOR_EACH_CONST(i, Vector<Import>, imports)
    {
      if (ImportModule* fm = owner->makeModule(i->expr, flatModule))
	{
	  if (fm->hasFreeParameters())
	    {
	      IssueWarning(i->lineNumber << ": cannot import module " << fm <<
			   " because it has free parameters.");
	      //
	      //	Mark the module as bad to avoid cascading warnings and potential
	      //	internal errors. But press ahead with remaining imports since
	      //	they should be independent and we might find other errors.
	      //
	      flatModule->markAsBad();
	    }
	  else
	    flatModule->addImport(fm, i->mode, i->lineNumber);
	}
      else
	{
	  //
	  //	Mark the module as bad to avoid cascading warnings and potential
	  //	internal errors. But press ahead with remaining imports since
	  //	they should be independent and we might find other errors.
	  //
	  flatModule->markAsBad();
	}
    }
}
