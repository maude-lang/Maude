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
//      Implementation for class SyntacticPreModule.
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
#include "ACU_Theory.hh"
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
#include "rewriteStrategy.hh"

//	free theory class definitions
#include "freeSymbol.hh"
#include "freeTerm.hh"

//	ACU theory class definitions
#include "ACU_Symbol.hh"

//	higher class definitions
#include "equalityConditionFragment.hh"
#include "sortTestConditionFragment.hh"
#include "assignmentConditionFragment.hh"
#include "rewriteConditionFragment.hh"

//	front end class definitions
#include "moduleExpression.hh"
#include "fileTable.hh"
#include "userLevelRewritingContext.hh"
#include "quotedIdentifierSymbol.hh"
#include "token.hh"
#include "loopSymbol.hh"
#include "visibleModule.hh"
#include "view.hh"
#include "syntacticPreModule.hh"
#include "interpreter.hh"
#include "maudemlBuffer.hh"
#include "mixfixParser.hh"
#include "objectConstructorSymbol.hh"

#ifdef QUANTIFY_PROCESSING
#include "quantify.h"
#endif

//	our stuff
#include "ooTransform.cc"
#include "ooProcess.cc"
#include "process.cc"
#include "fixUp.cc"
#include "ops.cc"
#include "command.cc"

SyntacticPreModule::SyntacticPreModule(Token startToken, Token moduleName, Interpreter* owner)
  : PreModule(moduleName.code(), owner),
    LineNumber(moduleName.lineNumber()),
    startTokenCode(startToken.code())
{
  MixfixModule::ModuleType moduleType = MixfixModule::FUNCTIONAL_MODULE;
  if (startTokenCode == th)
    moduleType = MixfixModule::SYSTEM_THEORY;
  else if (startTokenCode == fth)
    moduleType = MixfixModule::FUNCTIONAL_THEORY;
  else if (startTokenCode == sth)
    moduleType = MixfixModule::STRATEGY_THEORY;
  else if (startTokenCode == oth)
    moduleType = MixfixModule::OBJECT_ORIENTED_THEORY;
  else if (startTokenCode == mod)
    moduleType = MixfixModule::SYSTEM_MODULE;
  else if (startTokenCode == smod)
    moduleType = MixfixModule::STRATEGY_MODULE;
  else if (startTokenCode == omod)
    moduleType = MixfixModule::OBJECT_ORIENTED_MODULE;
  setModuleType(moduleType);

  lastSawOpDecl = false;
  isStrategy = false;
  isCompleteFlag = false;
  flatModule = 0;
}

SyntacticPreModule::~SyntacticPreModule()
{
  if (flatModule != 0)
    flatModule->deepSelfDestruct();
}

void
SyntacticPreModule::regretToInform(Entity* doomedEntity)
{
  Assert(doomedEntity == flatModule, "module pointer error");
  flatModule = 0;
#ifdef COMPILER
  getOwner()->invalidate(this);
#endif
}

VisibleModule*
SyntacticPreModule::getFlatModule()
{
  VisibleModule* m = getFlatSignature();
  //
  //	getFlatSignature() returns a module with its bad flag
  //	set if anything went wrong.
  //
  if (!(m->isBad()) && m->getStatus() < Module::THEORY_CLOSED)
    {
      //
      //	Need to flatten in statements and compile.
      //
      m->importStatements();
      Assert(!(m->isBad()), "importStatements() unexpectedly set bad flag in " << *m);
      m->resetImports();
      //
      //	Compile  module.
      //
      m->closeTheory();
      //
      //	We don't allow reserved fresh variable names in variant
      //	equations or narrowing rules. We can't do this until statements
      //	have been compiled since it relied on VariableInfo being filled out.
      //
      m->checkFreshVariableNames();
    }
  return m;
}

VisibleModule*
SyntacticPreModule::getFlatSignature() 
{
  if (flatModule == 0)
    {
      IssueAdvisory("reparsing module " << QUOTE(this) <<
		    " due to changes in imported modules.");
      process();
    }
  else if (flatModule->getStatus() == Module::OPEN)
    {
      DebugNew("module " << this << " had flatModule status open");
      return 0;  // we must already be in the middle of processing this module
    }
  return flatModule;
}

bool
SyntacticPreModule::compatible(int endTokenCode)
{
  if (startTokenCode == th)
    return endTokenCode == endth;
  if (startTokenCode == fth)
    return endTokenCode == endfth;
  if (startTokenCode == sth)
    return endTokenCode == endsth;
  if (startTokenCode == oth)
    return endTokenCode == endoth;
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
SyntacticPreModule::finishModule(Token endToken)
{
  if (!compatible(endToken.code()))
    {
      IssueWarning(LineNumber(endToken.lineNumber()) << ": module started with " <<
		   QUOTE(Token::name(startTokenCode)) << " ends with "
		   << QUOTE(endToken) << '.');
    }
  if (!isTheory())
    autoImports = getOwner()->getAutoImports(); // deep copy
  if (MixfixModule::isObjectOriented(getModuleType()))
    ooIncludes = getOwner()->getOoIncludes(); // deep copy
  isCompleteFlag = true;
  getOwner()->insertModule(id(), this);
  process();
  //
  //	House keeping.
  //
  getOwner()->destructUnusedModules();
}

void
SyntacticPreModule::addParameter2(Token name, ModuleExpression* theory)
{
  PreModule::addParameter(name, theory);
}

void
SyntacticPreModule::addImport(Token modeToken, ModuleExpression* expr)
{
  ImportModule::ImportMode mode;
  LineNumber lineNumber(modeToken.lineNumber());
  int code = modeToken.code();
  if (code == pr || code == protecting)
    mode = ImportModule::PROTECTING;
  else if (code == ex || code == extending)
    mode = ImportModule::EXTENDING;
  else if (code == inc || code == including)
    mode = ImportModule::INCLUDING;
  else if (code == gb || code == generatedBy)
    mode = ImportModule::GENERATED_BY;
  else
    {
      Assert(code == us || code == usingToken, "unknown importation mode");
      IssueWarning(lineNumber <<
		   ": importation mode " << QUOTE("using") <<
		   " not supported - treating it like " <<
		   QUOTE("including") << '.');
      mode = ImportModule::INCLUDING;
    }
  PreModule::addImport(lineNumber, mode, expr);
}

SyntacticPreModule::OpDef::OpDef()
{
  prec = DEFAULT;
  metadata = NONE;
}

void
SyntacticPreModule::addStatement(const Vector<Token>& statement)
{
  //
  // Checks if this module admits the statement and issues a warning if
  // it does not.
  //
  int keywordCode = statement[0].code();
  {
    MixfixModule::ModuleType moduleType = getModuleType();
    bool isStrategic = MixfixModule::isStrategic(moduleType);
    const char* modorth = MixfixModule::isTheory(moduleType) ? "theory." : "module.";

    if (keywordCode == rl || keywordCode == crl)
      {
	if (moduleType == MixfixModule::FUNCTIONAL_MODULE ||
	    moduleType == MixfixModule::FUNCTIONAL_THEORY)
	  IssueWarning(LineNumber(statement[0].lineNumber()) <<
		       ": rule not allowed in a functional " << modorth);
      }
    else if ((keywordCode == sd || keywordCode == csd) && !isStrategic)
	IssueWarning(LineNumber(statement[0].lineNumber()) <<
	  ": strategy definition only allowed in a strategy module or theory.");
  }

  if (statement[1].code() == leftBracket &&
      statement[3].code() == rightBracket &&
      statement[4].code() == colon)
    {
      (void) potentialLabels.insert(statement[2].code());
      if (keywordCode == rl || keywordCode == crl)
	(void) potentialRuleLabels.insert(statement[2].code());
    }

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
	    {
	      potentialLabels.insert(statement[i+1].code());
	      if (keywordCode == rl || keywordCode == crl)
		(void) potentialRuleLabels.insert(statement[i+1].code());
	    }
	  else if (t == rightBracket)
	    ++bracketCount;
	}
    }
  statements.append(statement);
}

void
SyntacticPreModule::addClassDecl(Token name)
{
  if (MixfixModule::isObjectOriented(getModuleType()))
    {
      int nrClassDecls = classDecls.size();
      classDecls.resize(nrClassDecls + 1);
      classDecls[nrClassDecls].name = name;
    }
  else
    {
      IssueWarning(LineNumber(name.lineNumber()) <<
		   ": class declaration only allowed in object-oriented modules and theories.");
    }
}

void
SyntacticPreModule::addAttributePair(Token attributeName, bool kind, const Vector<Token>& tokens)
{
  //
  //	If we're not in an omod/oth then we already reported the error and we silently
  //	ignore the attributes.
  //
  if (MixfixModule::isObjectOriented(getModuleType()))
    {
      classDecls[classDecls.size() - 1].attributes.append({attributeName, {kind, tokens}, 0});
    }
}

void
SyntacticPreModule::addAttributePairNoColon(Token attributeName, bool kind, const Vector<Token>& tokens)
{
  //
  //	If we're not in an omod/oth then we already reported the error and we silently
  //	ignore the attributes.
  //
  if (MixfixModule::isObjectOriented(getModuleType()))
    {
      //
      //	We didn't see a colon; maybe it is stuck on the end of the attribute name.
      //
      const char* name = attributeName.name();
      Index len = strlen(name);
      if (name[len - 1] == ':')
	{
	  string removeColon(name, len - 1);
	  int lineNumber = attributeName.lineNumber();
	  attributeName.tokenize(removeColon.c_str(), lineNumber);
	  IssueWarning(LineNumber(lineNumber) << ": missing space before " << QUOTE(":") <<
		       " in declaration of attribute " << QUOTE(attributeName) << ".");
	}
      else
	{
	  IssueWarning(LineNumber(attributeName.lineNumber()) << ": missing " << QUOTE(":") <<
		       " in declaration of attribute " << QUOTE(attributeName) << ".");
	}
      classDecls[classDecls.size() - 1].attributes.append({attributeName, {kind, tokens}, 0});
    }
}

void
SyntacticPreModule::addSubclassDecl(const Vector<Token>& subclassDecl)
{
  if (MixfixModule::isObjectOriented(getModuleType()))
    subclassDecls.append(subclassDecl);
  else
    {
      IssueWarning(LineNumber(subclassDecl[0].lineNumber()) <<
		   ": subclass declaration only allowed in object-oriented modules and theories.");
    }
}
