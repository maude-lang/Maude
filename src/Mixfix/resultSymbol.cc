/*

    This file is part of the Maude 3 interpreter.

    Copyright 2026 SRI International, Menlo Park, CA 94025, USA.

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
//      Implementation for class ViewResultSymbol.
//

//      utility stuff
#include "macros.hh"
#include "vector.hh"
#include "pointerMap.hh"

//      forward declarations
#include "interface.hh"
#include "core.hh"
#include "freeTheory.hh"
#include "AU_Theory.hh"
#include "mixfix.hh"
#include "higher.hh"
#include "SMT.hh"
#include "meta.hh"

//      interface class definitions
#include "symbol.hh"
#include "dagNode.hh"

//	core class definitions
#include "symbolMap.hh"
#include "dagArgumentIterator.hh"

//      free class definitions
#include "freeDagNode.hh"

//      AU class definitions
#include "AU_Symbol.hh"

//	built-in macros
#include "bindingMacros.hh"

//      MetaLevel class definitions
#include "metaLevelOpSymbol.hh"
#include "metaLevel.hh"

//	mixfix class definitions
#include "resultSymbol.hh"

ResultSymbol::ResultSymbol(int id, int nrArgs)
  : FreeSymbol(id, nrArgs)
{
}

bool
ResultSymbol::attachSymbol(const char* purpose, Symbol* symbol)
{
  Assert(symbol != nullptr, "null symbol for " << purpose);
#define MACRO(SymbolName, SymbolClass) \
  BIND_SYMBOL(purpose, symbol, SymbolName, SymbolClass*)
#include "resultSignature.cc"
#undef MACRO
  return FreeSymbol::attachSymbol(purpose, symbol);
}

void
ResultSymbol::getSymbolAttachments(Vector<const char*>& purposes,
					    Vector<Symbol*>& symbols)
{
#define MACRO(SymbolName, SymbolClass) \
  APPEND_SYMBOL(purposes, symbols, SymbolName)
#include "resultSignature.cc"
#undef MACRO
  FreeSymbol::getSymbolAttachments(purposes, symbols);
}

void
ResultSymbol::copyAttachments(Symbol* original, SymbolMap* map)
{
  ResultSymbol* orig = safeCastNonNull<ResultSymbol*>(original);
#define MACRO(SymbolName, SymbolClass) \
  COPY_SYMBOL(orig, SymbolName, map, SymbolClass*)
#include "resultSignature.cc"
#undef MACRO
  FreeSymbol::copyAttachments(original, map);
}

bool
ResultSymbol::hasTransformFlag(Symbol* symbol)
{
  MixfixModule* ourModule = safeCastNonNull<MixfixModule*>(getModule());
  Index nrOpDecls = symbol->getOpDeclarations().size();
  for (Index i = 0; i < nrOpDecls; ++i)
    {
      int metadata = ourModule->getMetadata(symbol, i);
      if (metadata != NONE && strcmp(Token::name(metadata), "\"transformer\"") == 0)
	return true;
    }
  return false;
}

bool
ResultSymbol::hasGoodDomain(Symbol* symbol, Vector<const ConnectedComponent*>& domain)
{
  Index nrArgs = symbol->arity();
  if (nrArgs > 6)
    return false;
  for (Index i = 0; i < nrArgs; ++i)
    {
      if (symbol->domainComponent(i) != domain[i])
	return false;
    }
  return true;
}

Symbol*
ResultSymbol::getTransformOp(const ConnectedComponent* qidKind,
			      const ConnectedComponent* rangeKind)
{
  if (cachedTransformOp == nullptr)
    {
      //
      //	Need to search symbols for a suitable operator.
      //	Full generality is:
      //	  structured module list
      //	  argument list
      //	  module expression list
      //	  flattened module list
      //	  view list
      //	  view expression list
      //	But we allow any initial segment with the correct range.
      //
      Vector<const ConnectedComponent*> domain(6);
      domain[0] = nilModuleListSymbol->rangeComponent();
      domain[1] = qidKind;
      domain[2] = qidKind;
      domain[3] = nilModuleListSymbol->rangeComponent();
      domain[4] = nilViewListSymbol->rangeComponent();
      domain[5] = qidKind;

      bool cachedHasTransformFlag = false;
      Symbol* conflictOp = nullptr;
      for (Symbol* s : getModule()->getSymbols())
	{
	  if (s->rangeComponent() == rangeKind && hasGoodDomain(s, domain))
	    {
	      bool transformFlag = hasTransformFlag(s);
	      if (cachedTransformOp == nullptr)
		{
		  cachedTransformOp = s;
		  cachedHasTransformFlag = transformFlag;
		}
	      else
		{
		  if (cachedHasTransformFlag)
		    {
		      if (transformFlag)
			{
			  //
			  //	Equal claim to be the transform op and no
			  //	resolution possible.
			  //
			  conflictOp = s;
			  break;
			}
		    }
		  else
		    {
		      if (transformFlag)
			{
			  //
			  //	Quietly displace old candidate and resolve
			  //	any conflict.
			  //
			  cachedTransformOp = s;
			  cachedHasTransformFlag = true;
			  conflictOp = nullptr;
			}
		      else
			{
			  //
			  //	Equal claim to be the transform op and but
			  //	conflict might be resolved by another symbol.
			  //
			  conflictOp = s;
			}
		    }
		}

	    }
	}
      if (conflictOp != nullptr)
	{
	  IssueWarning(*(getModule()) << ": unable to decide between transformer operators " <<
		       QUOTE(cachedTransformOp) << " and " <<
		       QUOTE(conflictOp) << " in module " << QUOTE(getModule()) << ".");
	  cachedTransformOp = nullptr;
	}
      else if (cachedTransformOp == nullptr)
	{
	  IssueWarning(*(getModule()) <<
		       ": didn't find suitable transformer operator in module " <<
		       QUOTE(getModule()) << ".");
	}
    }
  return cachedTransformOp;
}

MetaLevel*
ResultSymbol::getMetaLevel() const
{
  Assert(shareWith, "no metalevel attached");
  return shareWith->getMetaLevel();
}

const ConnectedComponent*
ResultSymbol::getModuleKind() const
{
  Assert(shareWith, "no metalevel attached");
  //
  //	We require that we share the metalevel with an operator
  //	that takes a module as its first argument, which is most
  //	descent functions apart from the up*() family and downTerm().
  //
  return shareWith->domainComponent(0);
}

void
ResultSymbol::handleMessageList(DagNode* messages, LineNumber lineNumber) const
{
  if (messages->symbol() == systemMsgListSymbol)
    {
      for (DagArgumentIterator i(messages); i.valid(); i.next())
	handleMessage(i.argument(), lineNumber);
    }
  else if (messages->symbol() != nilSystemMsgListSymbol)
    handleMessage(messages, lineNumber);
}

void
ResultSymbol::handleMessage(DagNode* message, LineNumber lineNumber) const
{
  Symbol* s = message->symbol();
  DagArgumentIterator i(message);
  if (i.valid())
    {
      Rope text;
      if (getMetaLevel()->downString(i.argument(), text))
	{
	  if (s == advisorySymbol)
	    {
	      IssueAdvisory(lineNumber << ": " << colorize(text));
	      return;
	    }
	  else if (s == warningSymbol)
	    {
	      IssueWarning(lineNumber << ": " << colorize(text));
	      return;
	    }
	  else if (s == verboseSymbol)
	    {
	      Verbose(text);
	      return;
	    }
	}
    }
  IssueWarning(*this << ": " << QUOTE(this) << " returned bad system message: " << QUOTE(message));
}

DagNode*
ResultSymbol::upModuleList(bool flat,
			   const Vector<ImportModule*>& inputModules,
			   PointerMap& qidMap) const
{
  if (inputModules.empty())
    return nilModuleListSymbol->makeDagNode();
  Index index = 0;
  Vector<DagNode*> metaModules(inputModules.size());
  for (ImportModule* m : inputModules)
    {
      m->finishFlattening();
      metaModules[index] = getMetaLevel()->upModule(flat, m, qidMap, Token::addQuotes(m->id()));
      ++index;
    }
  return (inputModules.size() == 1) ? metaModules[0] :
    moduleListSymbol->makeDagNode(metaModules);
}

Rope
ResultSymbol::colorize(const Rope& text)
{
  Rope result;
  bool quote = false;
  for (const char c : text)
    {
      if (c == '\036')  // Record Separator
	{
	  if (!quote)
	    {
	      result += BEGIN_QUOTE.ctrlSequence();
	      quote = true;
	    }
	}
      else if (c == '\037')  // Unit Separator
	{
	  if (quote)
	    {
	      result += END_QUOTE.ctrlSequence();
	      quote = false;
	    }
	}
      else
	result += c;
    }
  if (quote)
    result += END_QUOTE.ctrlSequence();  // don't leave hanging color
  return result;
}
