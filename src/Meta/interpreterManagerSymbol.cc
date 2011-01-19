/*

    This file is part of the Maude 2 interpreter.

    Copyright 1997-2009 SRI International, Menlo Park, CA 94025, USA.

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

//      forward declarations
#include "interface.hh"
#include "core.hh"
#include "higher.hh"
#include "freeTheory.hh"
#include "builtIn.hh"
#include "strategyLanguage.hh"
#include "mixfix.hh"
#include "objectSystem.hh"

//      interface class definitions
#include "symbol.hh"
#include "dagNode.hh"
#include "term.hh"

//      core class definitions
#include "symbolMap.hh"

//	free theory class definitions
#include "freeDagNode.hh"

//      built in class definitions
#include "succSymbol.hh"
#include "bindingMacros.hh"

//	object system definitions
#include "objectSystemRewritingContext.hh"

//	mixfix class definitions
#include "interpreter.hh"
#include "userLevelRewritingContext.hh"

//	metalevel class definitions
#include "metaModule.hh"
#include "metaLevel.hh"
#include "metaLevelOpSymbol.hh"
#include "metaPreModule.hh"
#include "interpreterManagerSymbol.hh"

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
#define MACRO(SymbolName, SymbolClass, NrArgs) \
  BIND_SYMBOL(purpose, symbol, SymbolName, SymbolClass*)
#include "interpreterSignature.cc"
#undef MACRO
  if (metaLevel == 0)
    BIND_SYMBOL(purpose, symbol, shareWith, MetaLevelOpSymbol*);  // returns from function if it was a shareWith hook
  if (okToBind() && metaLevel->bind(purpose, symbol))
    return true;
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
    {
      createInterpreter(safeCast(FreeDagNode*, message), context);
      return true;
    }
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
  if (s == insertViewMsg)
    return insertView(safeCast(FreeDagNode*, message), context);
  else if (s == showViewMsg)
    return showView(safeCast(FreeDagNode*, message), context);
  else if (s == reduceTermMsg)
    return reduceTerm(safeCast(FreeDagNode*, message), context);
  else if (s == rewriteTermMsg)
    return rewriteTerm(safeCast(FreeDagNode*, message), context);
  else if (s == frewriteTermMsg)
    return frewriteTerm(safeCast(FreeDagNode*, message), context);
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
      if (MetaModule* m = metaLevel->downModule(message->getArgument(2), false, interpreter))
	{
	  PreModule* pm = new MetaPreModule(m->id(),
					    message->getArgument(2),
					    metaLevel,
					    m,
					    interpreter);
	  m->addUser(pm);
	  interpreter->insertModule(m->id(), pm); 

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
  return false;
}

bool
InterpreterManagerSymbol::showView(FreeDagNode* message, ObjectSystemRewritingContext& context)
{
  return false;
}

bool
InterpreterManagerSymbol::reduceTerm(FreeDagNode* message, ObjectSystemRewritingContext& context)
{
  Interpreter* interpreter;
  if (getInterpreter(message->getArgument(0), interpreter))
    {
      int id;
      if (metaLevel->downQid(message->getArgument(2), id))
	{
	  if (PreModule* pm = interpreter->getModule(id))
	    {
	      if (ImportModule* m = pm->getFlatModule())
		{
		   if (Term* t = metaLevel->downTerm(message->getArgument(3), m))
		     {
		       t = t->normalize(false);
		       DagNode* d = term2Dag(t);
		       t->deepSelfDestruct();
		       RewritingContext* objectContext =
			 context.makeSubcontext(d, UserLevelRewritingContext::META_EVAL);
		       m->protect();
		       objectContext->reduce();
		       context.addInCount(*objectContext);

		       Vector<DagNode*> reply(5);
		       DagNode* target = message->getArgument(1);
		       reply[0] = target;
		       reply[1] = message->getArgument(0);

		       reply[2] = metaLevel->upNat((int) objectContext->getTotalCount());  // int cast is a hack for mac

		       PointerMap qidMap;
		       PointerMap dagNodeMap;
		       DagNode* dagNode = objectContext->root();
		       reply[3] = metaLevel->upDagNode(dagNode, m, qidMap, dagNodeMap);
		       reply[4] = metaLevel->upType(dagNode->getSort(), qidMap);
		       context.bufferMessage(target, reducedTermMsg->makeDagNode(reply));

		       delete objectContext;
		       (void) m->unprotect();
		       return true;
		     }
		}
	    }
	}
    }
  return false;
}

bool
InterpreterManagerSymbol::rewriteTerm(FreeDagNode* message, ObjectSystemRewritingContext& context)
{
  Interpreter* interpreter;
  if (getInterpreter(message->getArgument(0), interpreter))
    {
      Int64 limit;
      if (metaLevel->downBound64(message->getArgument(2), limit))
	{
	  int id;
	  if (metaLevel->downQid(message->getArgument(3), id))
	    {
	      if (PreModule* pm = interpreter->getModule(id))
		{
		  if (ImportModule* m = pm->getFlatModule())
		    {
		      if (Term* t = metaLevel->downTerm(message->getArgument(4), m))
			{
			  t = t->normalize(false);
			  DagNode* d = term2Dag(t);
			  t->deepSelfDestruct();
			  RewritingContext* objectContext =
			    context.makeSubcontext(d, UserLevelRewritingContext::META_EVAL);
			  m->protect();
			  objectContext->ruleRewrite(limit);
			  context.addInCount(*objectContext);

			  Vector<DagNode*> reply(5);
			  DagNode* target = message->getArgument(1);
			  reply[0] = target;
			  reply[1] = message->getArgument(0);
			  reply[2] = metaLevel->upNat((int) objectContext->getTotalCount());  // HACK

			  PointerMap qidMap;
			  PointerMap dagNodeMap;
			  DagNode* dagNode = objectContext->root();
			  reply[3] = metaLevel->upDagNode(dagNode, m, qidMap, dagNodeMap);
			  reply[4] = metaLevel->upType(dagNode->getSort(), qidMap);
			  context.bufferMessage(target, rewroteTermMsg->makeDagNode(reply));
			  //
			  //	We should somehow save state for continuation.
			  //
			  delete objectContext;
			  (void) m->unprotect();
			  return true;
			}
		    }
		}
	    }
	}
    }
  return false;
}

bool
InterpreterManagerSymbol::frewriteTerm(FreeDagNode* message, ObjectSystemRewritingContext& context)
{
  Interpreter* interpreter;
  if (getInterpreter(message->getArgument(0), interpreter))
    {
      Int64 limit;
      Int64 gas;
      if (metaLevel->downBound64(message->getArgument(2), limit) && limit != 0 &&
	  metaLevel->downSaturate64(message->getArgument(3), gas) && gas != 0)
	{
	  int id;
	  if (metaLevel->downQid(message->getArgument(4), id))
	    {
	      if (PreModule* pm = interpreter->getModule(id))
		{
		  if (ImportModule* m = pm->getFlatModule())
		    {
		      if (Term* t = metaLevel->downTerm(message->getArgument(5), m))
			{
			  t = t->normalize(false);
			  DagNode* d = term2Dag(t);
			  t->deepSelfDestruct();
			  RewritingContext* objectContext =
			    context.makeSubcontext(d, UserLevelRewritingContext::META_EVAL);
			  m->protect();
			  objectContext->fairRewrite(limit, gas);
			  objectContext->root()->computeTrueSort(*objectContext);  // needed so we have well defined sorts
			  context.addInCount(*objectContext);

			  Vector<DagNode*> reply(5);
			  DagNode* target = message->getArgument(1);
			  reply[0] = target;
			  reply[1] = message->getArgument(0);
			  reply[2] = metaLevel->upNat((int) objectContext->getTotalCount());  //HACK

			  PointerMap qidMap;
			  PointerMap dagNodeMap;
			  DagNode* dagNode = objectContext->root();
			  reply[3] = metaLevel->upDagNode(dagNode, m, qidMap, dagNodeMap);
			  reply[4] = metaLevel->upType(dagNode->getSort(), qidMap);
			  context.bufferMessage(target, frewroteTermMsg->makeDagNode(reply));
			  //
			  //	We should somehow save state for continuation.
			  //
			  delete objectContext;
			  (void) m->unprotect();
			  return true;
			}
		    }
		}
	    }
	}
    }
  return false;
}

bool
InterpreterManagerSymbol::quit(FreeDagNode* message, ObjectSystemRewritingContext& context)
{
  DagNode* socketName = message->getArgument(0);
  if (deleteInterpreter(socketName))
    {
      context.deleteExternalObject(socketName);
      Vector<DagNode*> reply(2);
      DagNode* target = message->getArgument(1);
      reply[0] = target;
      reply[1] = socketName;
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
      if (succSymbol->getSignedInt(idArg, interpreterId))
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
InterpreterManagerSymbol::deleteInterpreter(DagNode* interpreterArg)
{
  if (interpreterArg->symbol() == interpreterOidSymbol)
    {
      DagNode* idArg = safeCast(FreeDagNode*, interpreterArg)->getArgument(0);
      int interpreterId;
      if (succSymbol->getSignedInt(idArg, interpreterId))
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

void
InterpreterManagerSymbol::createInterpreter(FreeDagNode* originalMessage, ObjectSystemRewritingContext& context)
{
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
  reply[0] = succSymbol->makeNatDag(id);
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
}


DagNode*
InterpreterManagerSymbol::term2Dag(Term* t)  // HACK - should share with MetaLevelOpSymbol
{
  NatSet eagerVariables;
  Vector<int> problemVariables;
  t->markEager(0, eagerVariables, problemVariables);
  DagNode* r = t->term2Dag();
  return r;
}
