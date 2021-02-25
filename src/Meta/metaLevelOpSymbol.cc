/*

    This file is part of the Maude 3 interpreter.

    Copyright 1997-2020 SRI International, Menlo Park, CA 94025, USA.

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
//      Implementation for class MetaLevelOpSymbol.
//
 
//      utility stuff
#include "macros.hh"
#include "vector.hh"
#include "pointerMap.hh"
#include "meta.hh"
 
//      forward declarations
#include "interface.hh"
#include "core.hh"
#include "variable.hh"
#include "higher.hh"
#include "freeTheory.hh"
#include "AU_Theory.hh"
#include "NA_Theory.hh"
#include "builtIn.hh"
#include "strategyLanguage.hh"
#include "mixfix.hh"
#include "SMT.hh"
 
//      interface class definitions
#include "symbol.hh"
#include "dagNode.hh"
#include "rawDagArgumentIterator.hh"
#include "rawArgumentIterator.hh"
#include "term.hh"
#include "extensionInfo.hh"

//      core class definitions
#include "variableInfo.hh"
#include "variableSymbol.hh"
#include "preEquation.hh"
#include "substitution.hh"
#include "rewritingContext.hh"
#include "module.hh"
#include "label.hh"
#include "rule.hh"
#include "symbolMap.hh"

//	higher class definitions
#include "pattern.hh"
#include "rewriteSearchState.hh"
#include "matchSearchState.hh"
#include "rewriteSequenceSearch.hh"
#include "narrowingSequenceSearch.hh"
#include "unificationProblem.hh"
#include "irredundantUnificationProblem.hh"
#include "variantSearch.hh"
#include "filteredVariantUnifierSearch.hh"
#include "narrowingSearchState2.hh"
#include "narrowingSequenceSearch3.hh"

//      free theory class definitions
#include "freeNet.hh"
#include "freeSymbol.hh"
#include "freeDagNode.hh"

//      variable class definitions
#include "variableDagNode.hh"

//      built in class definitions
#include "succSymbol.hh"
#include "bindingMacros.hh"
 
//      front end class definitions
#include "userLevelRewritingContext.hh"
#include "quotedIdentifierSymbol.hh"
#include "quotedIdentifierDagNode.hh"
#include "quotedIdentifierOpSymbol.hh"
#include "metaModule.hh"
#include "metaLevel.hh"
#include "metaLevelOpSymbol.hh"
#include "fileTable.hh"
#include "syntacticPreModule.hh"
#include "interpreter.hh"
#include "visibleModule.hh"
#include "freshVariableSource.hh"
#include "mixfixParser.hh"

//	strategy language definitions
#include "strategyExpression.hh"
#include "depthFirstStrategicSearch.hh"
#include "fairStrategicSearch.hh"

//	our stuff
#include "descentFunctions.cc"
#include "ascentFunctions.cc"
#include "metaApply.cc"
#include "metaMatch.cc"
#include "metaSearch.cc"
#include "metaSrewrite.cc"
#include "metaUnify.cc"
#include "metaVariant.cc"
#include "metaVariantUnify.cc"
#include "metaVariantMatch.cc"
#include "metaNarrow.cc"
#include "metaNewNarrow.cc"
#include "metaNewNarrow2.cc"
#include "legacyMetaUnify.cc"
#include "legacyMetaVariant.cc"
#include "legacyMetaVariantUnify.cc"

MetaLevelOpSymbol::MetaLevelOpSymbol(int id, int nrArgs, const Vector<int>& strategy)
  : FreeSymbol(id, nrArgs, strategy)
{
  shareWith = 0;
  metaLevel = 0;
}

MetaLevelOpSymbol::~MetaLevelOpSymbol()
{
  if (shareWith == 0)
    delete metaLevel;
}

MetaLevelOpSymbol::AliasMapParserPair::~AliasMapParserPair()
{
  delete parser;
}

bool
MetaLevelOpSymbol::okToBind()
{
  if (shareWith != 0)
    return false;
  if (metaLevel == 0)
    metaLevel = new MetaLevel;
  return true;
}

bool
MetaLevelOpSymbol::attachData(const Vector<Sort*>& opDeclaration,
			      const char* purpose,
			      const Vector<const char*>& data)
{
  if (data.length() == 1)
    {
      const char* opName = data[0];
#define MACRO(SymbolName, NrArgs) \
      if (arity() == NrArgs && strcmp(opName, #SymbolName) == 0) \
	descentFunction = &MetaLevelOpSymbol::SymbolName; else
#include "descentSignature.cc"
#undef MACRO
        return FreeSymbol::attachData(opDeclaration, purpose, data);
      return true;
    }
  return FreeSymbol::attachData(opDeclaration, purpose, data);
}

bool
MetaLevelOpSymbol::attachSymbol(const char* purpose, Symbol* symbol)
{
  if (metaLevel == 0)
    BIND_SYMBOL(purpose, symbol, shareWith, MetaLevelOpSymbol*);
  return (okToBind() && metaLevel->bind(purpose, symbol)) ? true :
    FreeSymbol::attachSymbol(purpose, symbol);
}

bool
MetaLevelOpSymbol::attachTerm(const char* purpose, Term* term)
{
  return (okToBind() && metaLevel->bind(purpose, term)) ? true :
    FreeSymbol::attachTerm(purpose, term);
}

void
MetaLevelOpSymbol::copyAttachments(Symbol* original, SymbolMap* map)
{
  if (shareWith == 0 && metaLevel == 0)
    {
      MetaLevelOpSymbol* orig = safeCast(MetaLevelOpSymbol*, original);
      descentFunction = orig->descentFunction;
      MetaLevelOpSymbol* sw = orig->shareWith;
      if (sw != 0)
	{
	  metaLevel = 0;
	  shareWith = (map == 0) ? sw : safeCast(MetaLevelOpSymbol*, map->translate(sw));
	}
      else
	{
	  metaLevel = new MetaLevel(orig->metaLevel, map);
	  shareWith = 0;
	}
    }
  FreeSymbol::copyAttachments(original, map);
}

void
MetaLevelOpSymbol::getDataAttachments(const Vector<Sort*>& opDeclaration,
				      Vector<const char*>& purposes,
				      Vector<Vector<const char*> >& data)
{
  int nrDataAttachments = purposes.length();
  purposes.resize(nrDataAttachments + 1);
  purposes[nrDataAttachments] = "MetaLevelOpSymbol";
  data.resize(nrDataAttachments + 1);
  data[nrDataAttachments].resize(1);
  const char*& str = data[nrDataAttachments][0];
#define MACRO(SymbolName, NrArgs) \
  if (descentFunction == &MetaLevelOpSymbol::SymbolName) \
    str = #SymbolName; else
#include "descentSignature.cc"
    CantHappen("unrecognized descentFunction");
#undef MACRO
  FreeSymbol::getDataAttachments(opDeclaration, purposes, data);
}

void
MetaLevelOpSymbol::getSymbolAttachments(Vector<const char*>& purposes,
					Vector<Symbol*>& symbols)
{
  if (shareWith == 0)
    metaLevel->getSymbolAttachments(purposes, symbols);
  else
    {
      purposes.append("shareWith");
      symbols.append(shareWith);
    }
  FreeSymbol::getSymbolAttachments(purposes, symbols);
}

void
MetaLevelOpSymbol::getTermAttachments(Vector<const char*>& purposes,
				      Vector<Term*>& terms)
{
  if (shareWith == 0)
    metaLevel->getTermAttachments(purposes, terms);
  FreeSymbol::getTermAttachments(purposes, terms);
}

void
MetaLevelOpSymbol::postInterSymbolPass()
{
  if (shareWith == 0)
    metaLevel->postInterSymbolPass();
  else
    metaLevel = shareWith->metaLevel;
}

void
MetaLevelOpSymbol::reset()
{
  if (shareWith == 0 && metaLevel != 0)
    metaLevel->reset();
}

bool
MetaLevelOpSymbol::eqRewrite(DagNode* subject, RewritingContext& context)
{
  Assert(this == subject->symbol(), "Bad symbol");
  Assert(metaLevel != 0, "metaLevel not set for " << this);
  //if (metaLevel == 0)
  //  metaLevel = shareWith->metaLevel;
  FreeDagNode* d = safeCast(FreeDagNode*, subject);
  int nrArgs = arity();
  if (standardStrategy())
    {
      for (int i = 0; i < nrArgs; i++)
	d->getArgument(i)->reduce(context);
      return (this->*descentFunction)(d, context) || FreeSymbol::eqRewrite(subject, context);
    }
  return complexStrategy(subject, context);
}

bool
MetaLevelOpSymbol::complexStrategy(DagNode* subject, RewritingContext& context)
{
  FreeDagNode* d = safeCast(FreeDagNode*, subject);
  //
  //	Execute user supplied strategy.
  //
  //	We can't deal with multiple zeros in strategy, both because
  //	(1) we have no way to apply user equations at an non-final zero; and
  //	(2) we have no way to replace semi-eager arguments so that they can be evaluated.
  //
  const Vector<int>& userStrategy = getStrategy();
  int stratLen = userStrategy.length();
  for (int i = 0; i < stratLen - 1; i++)
    {
      int a = userStrategy[i];
      if(a == 0)
	{
	  //
	  //	Zero must be the end of the strategy and is treated as such.
	  //
	  IssueWarning("multiple zeros in strategy for MetaLevelOpSymbol " << QUOTE(this) << " not supported.");
	  break;
	}
      else
	d->getArgument(a - 1)->reduce(context);
    }
  return (this->*descentFunction)(d, context) || FreeSymbol::eqRewrite(subject, context);
}
