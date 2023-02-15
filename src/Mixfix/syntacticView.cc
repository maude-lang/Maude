/*

    This file is part of the Maude 3 interpreter.

    Copyright 2018-2023 SRI International, Menlo Park, CA 94025, USA.

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
//      Implementation for class SyntacticView.
//

//      utility stuff
#include "macros.hh"
#include "vector.hh"

//      forward declarations
#include "core.hh"
#include "interface.hh"
#include "variable.hh"
#include "higher.hh"
#include "strategyLanguage.hh"
#include "mixfix.hh"

//      interface class definitions
#include "symbol.hh"
#include "dagNode.hh"
#include "term.hh"

//      core class definitions
#include "argumentIterator.hh"

//      variable class definitions
#include "variableTerm.hh"

//	strategy language definitions
#include "strategyExpression.hh"
#include "callStrategy.hh"

//	front end class definitions
#include "token.hh"
#include "moduleExpression.hh"
#include "interpreter.hh"
#include "mixfixParser.hh"
#include "syntacticView.hh"

SyntacticView::SyntacticView(Token viewName, Interpreter* owner)
  : View(viewName, owner)
{
}

void
SyntacticView::addParameter2(Token name, ModuleExpression* theory)
{
  addParameter(name.code(), theory);
}

void
SyntacticView::addVarDecl(Token varName)
{
  varDecls.push_back(VarDecl());
  varDecls.back().varName = varName;
  varDecls.back().lastWithCurrentDef = false;
}

void
SyntacticView::addType(bool kind, const Vector<Token>& tokens)
{
  if (varDecls.empty() || varDecls.back().lastWithCurrentDef)
    Renaming::addType(kind, tokens);  // not ours
  else
    {
      varDecls.back().lastWithCurrentDef = true;
      varDefs.push_back(Type());
      varDefs.back().kind = kind;
      varDefs.back().tokens = tokens;
    }
}

void
SyntacticView::addOpTermMapping(const Vector<Token>& fromOp, const Vector<Token>& toTerm, bool msgFlag)
{
  opTermList.push_back({fromOp, toTerm, msgFlag});
}

void
SyntacticView::addStratExprMapping(const Vector<Token>& fromExpr, const Vector<Token>& toExpr)
{
  stratExprList.push_back(BubblePair());
  stratExprList.back().fromBubble = fromExpr;  // deep copy
  stratExprList.back().toBubble = toExpr;  // deep copy
}

bool
SyntacticView::handleVarDecls()
{
  //
  //	Need to clear any saved information in case we are reexamining this view.
  //
  fromTheoryVariableAliases.clear();
  toModuleVariableAliases.clear();
  ImportModule* fromTheory = getFromTheory();
  ImportModule* toModule = getToModule();
  Sort* fromSort = 0;
  Sort* toSort = 0;
  TypeList::const_iterator j = varDefs.begin();
  FOR_EACH_CONST(i, VarDeclList, varDecls)
    {
      if (fromSort == 0)
	{
	  int code = j->tokens[0].code();
	  fromSort = fromTheory->findSort(code);
	  if (fromSort == 0)
	    {
	      IssueWarning(LineNumber(j->tokens[0].lineNumber()) <<
			   ": failed to find sort " << QUOTE(Token::sortName(code)) <<
			   " in " << QUOTE(fromTheory) << '.');
	      return false;
	    }
	  toSort = toModule->findSort(renameSort(code));
	  Assert(toSort != 0, "couldn't find translation of sort");
	  if (j->kind)
	    {
	      int nrTokens = j->tokens.size();
	      for (int k = 1; k < nrTokens; ++k)
		{
		  int code = j->tokens[k].code();
		  Sort* extraSort = fromTheory->findSort(code);
		  if (extraSort == 0)
		    {
		      IssueWarning(LineNumber(j->tokens[k].lineNumber()) <<
				   ": failed to find sort " << QUOTE(Token::sortName(code)) <<
				   " in " << QUOTE(fromTheory) << '.');
		      return false;
		    }
		  if (extraSort->component() != fromSort->component())
		    {
		      IssueWarning(LineNumber(j->tokens[k].lineNumber()) <<
				   ": sorts " << QUOTE(fromSort) << " and " <<
				   QUOTE(extraSort) << " are in different components.");
		      return false;
		    }
		}
	      fromSort = fromSort->component()->sort(Sort::KIND);
	      toSort = toSort->component()->sort(Sort::KIND);
	    }
	  ++j;
	}
      //
      //	We can add variable aliases to a module at any point before
      //	we first do parsing in the module.
      //
      fromTheoryVariableAliases.insert(MixfixModule::AliasMap::value_type(i->varName.code(), fromSort));
      toModuleVariableAliases.insert(MixfixModule::AliasMap::value_type(i->varName.code(), toSort));
      if (i->lastWithCurrentDef)
	fromSort = 0;
    }
  return true;
}

bool
SyntacticView::handleTermAndExprMappings()
{
  messages.clear();  // clear out any old messages in case we are reevaluating
  if (opTermList.empty() && stratExprList.empty())
    return true;  // nothing to do
  if (!varDecls.empty() && !handleVarDecls())
    return false;  // bad variable declaration

  ImportModule* fromTheory = getFromTheory();
  ImportModule* toModule = getToModule();
  //
  //   We need to swap in our own aliases, even if they're empty
  //   if only to get rid of the modules' own aliases.
  //   We save the original parsers.
  //
  MixfixParser* savedFromTheoryParser = 0;
  fromTheory->swapVariableAliasMap(fromTheoryVariableAliases, savedFromTheoryParser);
  MixfixParser* savedToModuleParser = 0;
  toModule->swapVariableAliasMap(toModuleVariableAliases, savedToModuleParser);
  //
  //	Now deal with op->term mappings.
  //
  bool result = true;
  {
    for (const OpTermMapping& i : opTermList)
      {
	Term* fromTerm = fromTheory->parseTerm(i.fromBubble);
	if (fromTerm == 0)
	  {
	    result = false;
	    break;
	  }

	ConnectedComponent* expectedKind = mapComponent(fromTerm->symbol()->rangeComponent());
	Term* toTerm = toModule->parseTerm(i.toBubble, expectedKind, 1);
	if (toTerm == 0)
	  {
	    fromTerm->deepSelfDestruct();
	    result = false;
	    break;
	  }
	if (!insertOpToTermMapping(fromTerm, toTerm))
	  {
	    fromTerm->deepSelfDestruct();
	    toTerm->deepSelfDestruct();
	    result = false;
	    break;
	  }
	if (i.msg)
	  {
	    //
	    //	Track and check msg->term mappings.
	    //
	    messages.insert(fromTerm);
	    Symbol* fromSymbol = fromTerm->symbol();
	    {
	      SymbolType st = fromTheory->getSymbolType(fromSymbol);
	      AdvisoryCheck(st.hasFlag(SymbolType::MESSAGE), "msg to term mapping for symbol " <<
			    QUOTE(fromSymbol) << " which does not have the msg attribute.");
	    }
	    {
	      Symbol* toSymbol = toTerm->symbol();
	      SymbolType st = toModule->getSymbolType(toSymbol);
	      AdvisoryCheck(st.hasFlag(SymbolType::MESSAGE), "msg to term mapping for symbol " <<
			    QUOTE(fromSymbol) << " maps it to a term headed by " << QUOTE(toSymbol) <<
			    " which does not have the msg attribute.");
	    }
	  }
      }
  }
  //
  //	Now deal with strat->expr mappings.
  //
  if (result)
    {
      for (const BubblePair& bpair : stratExprList)
	{
	  StrategyExpression* from = fromTheory->parseStrategyExpr(bpair.fromBubble);
	  CallStrategy* callFrom = static_cast<CallStrategy*>(from);
	  if (callFrom == 0)
	    {
	      delete from;
	      result = false;
	      break;
	    }
	  StrategyExpression* toExpr = toModule->parseStrategyExpr(bpair.toBubble, 1);
	  if (toExpr == 0)
	    {
	      delete from;
	      result = false;
	      break;
	    }
	  if (!insertStratToExprMapping(callFrom,
					toExpr,
					toModule))
	    {
	      delete from;
	      delete toExpr;
	      result = false;
	      break;
	    }
	}
    }
  //
  //	Restore original variable aliases and parsers. We delete any
  //	parsers that were newly created for our parsing - not worth cacheing.
  //
  fromTheory->swapVariableAliasMap(fromTheoryVariableAliases, savedFromTheoryParser);
  delete savedFromTheoryParser;
  toModule->swapVariableAliasMap(toModuleVariableAliases, savedToModuleParser);
  delete savedToModuleParser;
  return result;
}

void
SyntacticView::showView(ostream& s)
{
  s << "view " << this;
  int nrParameters = getNrParameters();
  if (nrParameters > 0)
    {
      s << '{';
      for (int i = 0;;)
	{
	  s << Token::name(getParameterName(i)) << " :: " << getParameterTheoryExpression(i);
	  ++i;
	  if (i == nrParameters)
	    break;
	  s << ", ";
	}
      s << '}';
    }
  s << " from " << getFrom() << " to " << getTo() << " is\n";
  printRenaming(s, "  ", " .\n  ");
  if (getNrSortMappings() + getNrOpMappings() + getNrClassMappings() > 0)
    s << " .\n";
  if (!varDecls.empty())
    {
      bool startNew = true;
      TypeList::const_iterator j = varDefs.begin();
      for (const VarDecl& i : varDecls)
	{
	  if (startNew)
	    {
	      s << "  var";
	      if (!(i.lastWithCurrentDef))
		s << 's';
	      startNew = false;
	    }
	  s << ' ' << i.varName;
	  if (i.lastWithCurrentDef)
	    {
	      s << " : " << *j << " .\n";
	      ++j;
	      startNew = true;
	    }
	}
    }
  //
  //	We print op->term and  msg->term mappings as unparsed bubbles.
  //
  for (const auto& i : opTermList)
    s << (i.msg ? "  msg " : "  op ") << i.fromBubble << " to " << i.toBubble << " .\n";
  //
  //	We print strat->expr mappings as unparsed bubbles.
  //
  for (const auto& i : stratExprList)
    s << "  strat " << i.fromBubble << " to " << i.toBubble << " .\n";
  s << "endv\n";
}

void
SyntacticView::showProcessedView(ostream& s)
{
  s << "view " << this;
  int nrParameters = getNrParameters();
  if (nrParameters > 0)
    {
      s << '{';
      for (int i = 0;;)
	{
	  s << Token::name(getParameterName(i)) << " :: " << getParameterTheory(i);
	  ++i;
	  if (i == nrParameters)
	    break;
	  s << ", ";
	}
      s << '}';
    }
  ImportModule* fromTheory = getFromTheory();
  ImportModule* toModule = getToModule();
  s << " from " << fromTheory << " to " << Token::removeBoundParameterBrackets(toModule->id()) << " is\n";
  printRenaming(s, "  ", " .\n  ", true);
  if (getNrSortMappings() + getNrOpMappings() > 0)
    s << " .\n";
  //
  //	Print from aliases.
  //
  for (auto& p : fromTheoryVariableAliases)
    s << "  var " << Token::name(p.first) << " : " << p.second << " .\n";
  //
  //	We need to swap in our own aliases, even if they're empty
  //	if only to get rid of the modules' own aliases.
  //
  MixfixParser* savedFromTheoryParser = 0;
  fromTheory->swapVariableAliasMap(fromTheoryVariableAliases, savedFromTheoryParser);
  MixfixParser* savedToModuleParser = 0;
  toModule->swapVariableAliasMap(toModuleVariableAliases, savedToModuleParser);

  for (const auto& i : getOpTermMap())
     s << "  op " << i.second.first << " to term " << i.second.second << " .\n";

  for (const auto& j : getStratExprMap())
    s << "  strat " << j.second.call << " to expr " << j.second.value << " .\n";

  s << "endv\n";
  //
  //	Restore original variable aliases. Check that we didn't generate parsers.
  //
  fromTheory->swapVariableAliasMap(fromTheoryVariableAliases, savedFromTheoryParser);
  Assert(savedFromTheoryParser == 0, "unexpected new from-theory parser");
  toModule->swapVariableAliasMap(toModuleVariableAliases, savedToModuleParser);
  Assert(savedToModuleParser == 0, "unexpected new to module-parser");
}
