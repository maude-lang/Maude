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
//      Implementation for class InterpreterManagerSymbol.
//
 //      utility stuff
#include "macros.hh"
#include "vector.hh"
#include "pointerMap.hh"
#include "meta.hh"

//      forward declarations
#include "interface.hh"
#include "core.hh"
#include "higher.hh"
#include "freeTheory.hh"
#include "builtIn.hh"
#include "strategyLanguage.hh"
#include "mixfix.hh"
#include "objectSystem.hh"
#include "SMT.hh"

//      interface class definitions
#include "symbol.hh"
#include "dagNode.hh"
#include "term.hh"

//      core class definitions
#include "symbolMap.hh"

//	higher class definitions
#include "pattern.hh"
#include "rewriteSearchState.hh"
#include "matchSearchState.hh"
#include "rewriteSequenceSearch.hh"
#include "unificationProblem.hh"
#include "irredundantUnificationProblem.hh"
#include "variantSearch.hh"
#include "filteredVariantUnifierSearch.hh"
#include "narrowingSearchState2.hh"
#include "narrowingSequenceSearch3.hh"

//	variable class definitions
#include "variableDagNode.hh"

//	free theory class definitions
#include "freeDagNode.hh"

//      built in class definitions
#include "bindingMacros.hh"

//	object system definitions
#include "objectSystemRewritingContext.hh"

//	strategy lanuage
#include "strategyExpression.hh"
#include "depthFirstStrategicSearch.hh"
#include "fairStrategicSearch.hh"

//	mixfix class definitions
#include "interpreter.hh"
#include "userLevelRewritingContext.hh"
#include "view.hh"
#include "freshVariableSource.hh"
#include "mixfixModule.hh"

//	metalevel class definitions
#include "metaModule.hh"
#include "metaLevel.hh"
#include "metaLevelOpSymbol.hh"
#include "metaPreModule.hh"
#include "interpreterManagerSymbol.hh"

//	our stuff
#include "interpreterApply.cc"
#include "interpreterPrint.cc"
#include "interpreterRewrite.cc"
#include "interpreterSearch.cc"
#include "interpreterMatch.cc"
#include "interpreterUnify.cc"
#include "interpreterVariant.cc"
#include "interpreterVariantUnify.cc"
#include "interpreterVariantMatch.cc"
#include "interpreterSort.cc"
#include "interpreterNewNarrow.cc"
#include "interpreterNewNarrowSearch.cc"

InterpreterManagerSymbol::InterpreterManagerSymbol(int id)
  : ExternalObjectManagerSymbol(id)
{
  shareWith = 0;
  metaLevel = 0;
#define MACRO(SymbolName, SymbolClass, NrArgs) \
  SymbolName = 0;
#include "interpreterSignature.cc"
#undef MACRO
}

InterpreterManagerSymbol::~InterpreterManagerSymbol()
{
  if (shareWith == 0)
    delete metaLevel;
}

bool
InterpreterManagerSymbol::okToBind()
{
  if (shareWith != 0)
    return false;
  if (metaLevel == 0)
    metaLevel = new MetaLevel;
  return true;
}

bool
InterpreterManagerSymbol::attachData(const Vector<Sort*>& opDeclaration,
				     const char* purpose,
				     const Vector<const char*>& data)
{
  NULL_DATA(purpose, InterpreterManagerSymbol, data);
  return ExternalObjectManagerSymbol::attachData(opDeclaration, purpose, data);
}

bool
InterpreterManagerSymbol::attachSymbol(const char* purpose, Symbol* symbol)
{
  //
  //	First we check if it is from our signature.
  //
#define MACRO(SymbolName, SymbolClass, NrArgs) \
  BIND_SYMBOL2(purpose, symbol, SymbolName, SymbolClass*, NrArgs)
#include "interpreterSignature.cc"
#undef MACRO
  //
  //	Next we check if it a symbol we are going to share a MetaLevel object with.
  //
  if (metaLevel == 0)
    BIND_SYMBOL(purpose, symbol, shareWith, MetaLevelOpSymbol*);  // returns from function if it was a shareWith hook
  //
  //	If we are not sharing a MetaLevel object, we see if our MetaLevel object will handle it.
  //
  if (okToBind() && metaLevel->bind(purpose, symbol))
    return true;
  //
  //	Finally pass it to our parent class.
  //
  return ExternalObjectManagerSymbol::attachSymbol(purpose, symbol);
}

bool
InterpreterManagerSymbol::attachTerm(const char* purpose, Term* term)
{
  return okToBind() && metaLevel->bind(purpose, term);
}

void
InterpreterManagerSymbol::copyAttachments(Symbol* original, SymbolMap* map)
{
  InterpreterManagerSymbol* orig = safeCast(InterpreterManagerSymbol*, original);
#define MACRO(SymbolName, SymbolClass, NrArgs) \
  COPY_SYMBOL(orig, SymbolName, map, SymbolClass*)
#include "interpreterSignature.cc"
#undef MACRO

  if (shareWith == 0 && metaLevel == 0)
    {
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
  ExternalObjectManagerSymbol::copyAttachments(original, map);
}

void
InterpreterManagerSymbol::getDataAttachments(const Vector<Sort*>& opDeclaration,
					     Vector<const char*>& purposes,
					     Vector<Vector<const char*> >& data)
{
  int nrDataAttachments = purposes.length();
  purposes.resize(nrDataAttachments + 1);
  purposes[nrDataAttachments] = "InterpreterManagerSymbol";
  data.resize(nrDataAttachments + 1);
  ExternalObjectManagerSymbol::getDataAttachments(opDeclaration, purposes, data);
}

void
InterpreterManagerSymbol::getSymbolAttachments(Vector<const char*>& purposes, Vector<Symbol*>& symbols)
{
#define MACRO(SymbolName, SymbolClass, NrArgs) \
  APPEND_SYMBOL(purposes, symbols, SymbolName)
#include "interpreterSignature.cc"
#undef MACRO

  if (shareWith == 0)
    metaLevel->getSymbolAttachments(purposes, symbols);
  else
    {
      purposes.append("shareWith");
      symbols.append(shareWith);
    }
}

void
InterpreterManagerSymbol::getTermAttachments(Vector<const char*>& purposes, Vector<Term*>& terms)
{
  if (shareWith == 0)
    metaLevel->getTermAttachments(purposes, terms);
}

void
InterpreterManagerSymbol::postInterSymbolPass()
{
  if (shareWith == 0)
    metaLevel->postInterSymbolPass();
  else
    metaLevel = shareWith->getMetaLevel();
}

void
InterpreterManagerSymbol::reset()
{
  if (shareWith == 0 && metaLevel != 0)
    metaLevel->reset();
}

bool
InterpreterManagerSymbol::handleManagerMessage(DagNode* message, ObjectSystemRewritingContext& context)
{
  DebugAdvisory("handleManagerMessage() saw " << message);
  Symbol* s = message->symbol();
  if (s == createInterpreterMsg)
    return createInterpreter(safeCast(FreeDagNode*, message), context);
  return false;
}

bool
InterpreterManagerSymbol::handleMessage(DagNode* message, ObjectSystemRewritingContext& context)
{
  DebugAdvisory("handleMessage(): saw " << message);

  Symbol* s = message->symbol();
  if (s == insertModuleMsg)
    return insertModule(safeCast(FreeDagNode*, message), context);
  else if (s == showModuleMsg)
    return showModule(safeCast(FreeDagNode*, message), context);
  else if (s == insertViewMsg)
    return insertView(safeCast(FreeDagNode*, message), context);
  else if (s == showViewMsg)
    return showView(safeCast(FreeDagNode*, message), context);

  else if (s == reduceTermMsg)
    return reduceTerm(safeCast(FreeDagNode*, message), context);
  else if (s == rewriteTermMsg)
    return rewriteTerm(safeCast(FreeDagNode*, message), context);
  else if (s == frewriteTermMsg)
    return frewriteTerm(safeCast(FreeDagNode*, message), context);
  else if (s == erewriteTermMsg)
    return erewriteTerm(safeCast(FreeDagNode*, message), context);
  else if (s == srewriteTermMsg)
    return srewriteTerm(safeCast(FreeDagNode*, message), context);
  else if (s == getSearchResultMsg || s == getSearchResultAndPathMsg)
    return getSearchResult(safeCast(FreeDagNode*, message), context);

  else if (s == getUnifierMsg)
    return getUnifier(safeCast(FreeDagNode*, message), context, false, false);
  else if (s == getDisjointUnifierMsg)
    return getUnifier(safeCast(FreeDagNode*, message), context, true, false);
  else if (s == getIrredundantUnifierMsg)
    return getUnifier(safeCast(FreeDagNode*, message), context, false, true);
  else if (s == getIrredundantDisjointUnifierMsg)
    return getUnifier(safeCast(FreeDagNode*, message), context, true, true);

  else if (s == getVariantMsg)
    return getVariant(safeCast(FreeDagNode*, message), context);
  else if (s == getVariantUnifierMsg)
    return getVariantUnifier(safeCast(FreeDagNode*, message), context, false);
  else if (s == getDisjointVariantUnifierMsg)
    return getVariantUnifier(safeCast(FreeDagNode*, message), context, true);
  else if (s == getVariantMatcherMsg)
    return getVariantMatcher(safeCast(FreeDagNode*, message), context);
  else if (s == getMatchMsg)
    return getMatch(safeCast(FreeDagNode*, message), context);
  else if (s == getXmatchMsg)
    return getXmatch(safeCast(FreeDagNode*, message), context);

  else if (s == printTermMsg)
    return printTerm(safeCast(FreeDagNode*, message), context);
  else if (s == parseTermMsg)
    return parseTerm(safeCast(FreeDagNode*, message), context);

  else if (s == applyRuleMsg)
    return applyRule(safeCast(FreeDagNode*, message), context, true);
  else if (s == applyRule2Msg)
    return applyRule(safeCast(FreeDagNode*, message), context, false);

  else if (s == getOneStepNarrowingMsg)
    return getOneStepNarrowing(safeCast(FreeDagNode*, message), context);
  else if (s == getNarrowingSearchResultMsg)
    return getNarrowingSearchResult(safeCast(FreeDagNode*, message), context, false);
  else if (s == getNarrowingSearchResultAndPathMsg)
    return getNarrowingSearchResult(safeCast(FreeDagNode*, message), context, true);

  else if (s == getLesserSortsMsg)
    return getLesserSorts(safeCast(FreeDagNode*, message), context);
  else if (s == getMaximalSortsMsg)
    return getMaximalSorts(safeCast(FreeDagNode*, message), context);
  else if (s == getMinimalSortsMsg)
    return getMinimalSorts(safeCast(FreeDagNode*, message), context);
  else if (s == compareTypesMsg)
    return compareTypes(safeCast(FreeDagNode*, message), context);
  else if (s == getKindMsg)
    return getKind(safeCast(FreeDagNode*, message), context);
  else if (s == getKindsMsg)
    return getKinds(safeCast(FreeDagNode*, message), context);
  else if (s == getGlbTypesMsg)
    return getGlbTypes(safeCast(FreeDagNode*, message), context);
  else if (s == getMaximalAritySetMsg)
    return getMaximalAritySet(safeCast(FreeDagNode*, message), context);
  else if (s == normalizeTermMsg)
    return normalizeTerm(safeCast(FreeDagNode*, message), context);
  else if (s == quitMsg)
    return quit(safeCast(FreeDagNode*, message), context);
  return false;
}

bool
InterpreterManagerSymbol::insertModule(FreeDagNode* message, ObjectSystemRewritingContext& context)
{
  Interpreter* interpreter;
  if (getInterpreter(message->getArgument(0), interpreter))
    {
      DagNode* metaModule = message->getArgument(2);
      //
      //	We create the flattened module first. Only if it is
      //	successful do we create the MetaPreModule and insert it in
      //	the interpreters module database.
      //
      if (MetaModule* m = metaLevel->downSignature(metaModule, interpreter))
	{
	  //
	  //	We need to get object level versions of header and imports
	  //	inserted into a MetaPreModule.
	  //
	  FreeDagNode* f = safeCast(FreeDagNode*, metaModule);
	  int id;
	  DagNode* metaParameterDeclList;
	  metaLevel->downHeader(f->getArgument(0), id, metaParameterDeclList);

	  MetaPreModule* pm = new MetaPreModule(id,
						metaModule,
						metaLevel,
						m,
						interpreter);

	  metaLevel->downParameterDeclList2(metaParameterDeclList, pm);
	  metaLevel->downImports2(f->getArgument(1), pm);
	  interpreter->insertModule(id, pm); 

	  Vector<DagNode*> reply(2);
	  DagNode* target = message->getArgument(1);
	  reply[0] = target;
	  reply[1] = message->getArgument(0);
	  context.bufferMessage(target, insertedModuleMsg->makeDagNode(reply));
	  return true;
	}
    }
  return false;
}

bool
InterpreterManagerSymbol::showModule(FreeDagNode* message, ObjectSystemRewritingContext& context)
{
  Interpreter* interpreter;
  if (getInterpreter(message->getArgument(0), interpreter))
    {
      int id;
      if (metaLevel->downQid(message->getArgument(2), id))
	{
	  bool flat;
	  if (metaLevel->downBool(message->getArgument(3), flat))
	    {
	      if (PreModule* pm = interpreter->getModule(id))
		{
		  Vector<DagNode*> reply(3);
		  DagNode* target = message->getArgument(1);
		  reply[0] = target;
		  reply[1] = message->getArgument(0);
		  PointerMap qidMap;
		  reply[2] = metaLevel->upModule(flat, pm, qidMap);
		  context.bufferMessage(target, showingModuleMsg->makeDagNode(reply));
		  return true;
		}
	    }
	}
    }
  return false;
}

bool
InterpreterManagerSymbol::insertView(FreeDagNode* message, ObjectSystemRewritingContext& context)
{
  Interpreter* interpreter;
  if (getInterpreter(message->getArgument(0), interpreter))
    {
      if (View* v = metaLevel->downView(message->getArgument(2), interpreter))
	{
	  // FIXME: need to sanity check v before inserting it
	  interpreter->insertView(v->id(), v);
	  
	  Vector<DagNode*> reply(2);
	  DagNode* target = message->getArgument(1);
	  reply[0] = target;
	  reply[1] = message->getArgument(0);
	  context.bufferMessage(target, insertedViewMsg->makeDagNode(reply));
	  return true;
	}
    }
  return false;
}

bool
InterpreterManagerSymbol::showView(FreeDagNode* message, ObjectSystemRewritingContext& context)
{
  Interpreter* interpreter;
  if (getInterpreter(message->getArgument(0), interpreter))
    {
      int id;
      if (metaLevel->downQid(message->getArgument(2), id))
	{
	  if (View* v = interpreter->getView(id))
	    {
	      Vector<DagNode*> reply(3);
	      DagNode* target = message->getArgument(1);
	      reply[0] = target;
	      reply[1] = message->getArgument(0);
	      PointerMap qidMap;
	      reply[2] = metaLevel->upView(v, qidMap);
	      context.bufferMessage(target, showingViewMsg->makeDagNode(reply));
	      return true;
	    }
	}
    }
  return false;
}

bool
InterpreterManagerSymbol::quit(FreeDagNode* message, ObjectSystemRewritingContext& context)
{
  DagNode* interpreterName = message->getArgument(0);
  if (deleteInterpreter(interpreterName))
    {
      context.deleteExternalObject(interpreterName);
      Vector<DagNode*> reply(2);
      DagNode* target = message->getArgument(1);
      reply[0] = target;
      reply[1] = interpreterName;
      context.bufferMessage(target, byeMsg->makeDagNode(reply));
      return true;
    }
  return false;
}

void
InterpreterManagerSymbol::cleanUp(DagNode* objectId)
{
  //
  //	Need to dispose of the interpreter describe by objectId.
  //
  if (!deleteInterpreter(objectId))
    CantHappen("Couldn't delete " << objectId);
}

bool
InterpreterManagerSymbol::getInterpreter(DagNode* interpreterArg, Interpreter*& interpreter)
{
  if (interpreterArg->symbol() == interpreterOidSymbol)
    {
      DagNode* idArg = safeCast(FreeDagNode*, interpreterArg)->getArgument(0);
      int interpreterId;
      if (metaLevel->downSignedInt(idArg, interpreterId))
	{
	  int nrIds = interpreters.size();
	  if (interpreterId < nrIds)
	    {
	      interpreter = interpreters[interpreterId];
	      if (interpreter != 0)
		return true;
	    }
	}
    }
  return false;
}

bool
InterpreterManagerSymbol::getInterpreterAndModule(FreeDagNode* message,
						  Interpreter*& interpreter,
						  ImportModule*& module)
{
  if (getInterpreter(message->getArgument(0), interpreter))
    {
      int id;
      if (metaLevel->downQid(message->getArgument(2), id))
	{
	  if (PreModule* pm = interpreter->getModule(id))
	    {
	      if ((module = pm->getFlatModule()))
		return true;
	    }
	}
    }
  return false;
}

bool
InterpreterManagerSymbol::deleteInterpreter(DagNode* interpreterArg)
{
  if (interpreterArg->symbol() == interpreterOidSymbol)
    {
      DagNode* idArg = safeCast(FreeDagNode*, interpreterArg)->getArgument(0);
      int interpreterId;
      if (metaLevel->downSignedInt(idArg, interpreterId))
	{
	  int nrIds = interpreters.size();
	  if (interpreterId < nrIds)
	    {
	      Interpreter* interpreter = interpreters[interpreterId];
	      if (interpreter != 0)
		{
		  interpreters[interpreterId] = 0;
		  delete interpreter;
		  return true;
		}
	    }
	}
    }
  return false;
}

bool
InterpreterManagerSymbol::createInterpreter(FreeDagNode* originalMessage, ObjectSystemRewritingContext& context)
{
  if (originalMessage->getArgument(2)->symbol() != emptyInterpereterOptionSetSymbol)
    return false;  // we don't currently support options

  int nrIds = interpreters.size();
  int id = 0;
  for (; id < nrIds; ++id)
    {
      if (interpreters[id] == 0)
	goto foundSlot;
    }
  interpreters.resize(nrIds + 1);
 foundSlot:
  interpreters[id] = new Interpreter;

  Vector<DagNode*> reply(1, 3);
  reply[0] = metaLevel->upNat(id);
  DagNode* interpreterName = interpreterOidSymbol->makeDagNode(reply);
  context.addExternalObject(interpreterName, this);
  reply.resize(3);
  reply[2] = interpreterName;
  reply[1] = originalMessage->getArgument(0);
  DagNode* target = originalMessage->getArgument(1);
  reply[0] = target;
  DagNode* response = createdInterpreterMsg->makeDagNode(reply);
  DebugAdvisory("createInterpreter() response: " << response);
  context.bufferMessage(target, response);
  return true;
}

DagNode*
InterpreterManagerSymbol::upRewriteCount(const RewritingContext* context)
{
  mpz_class totalCount(context->getTotalCount());
  return metaLevel->upNat(totalCount);
}
