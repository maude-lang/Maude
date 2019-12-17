/*

    This file is part of the Maude 3 interpreter.

    Copyright 2019 SRI International, Menlo Park, CA 94025, USA.

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
//	Code for printTerm() and parseTerm() messages.
//

bool
InterpreterManagerSymbol::printTerm(FreeDagNode* message, ObjectSystemRewritingContext& context)
{
  //
  //	  op printTerm : Oid Oid Qid VariableSet Term PrintOptionSet -> Msg .
  //	                  0   1   2      3        4         5
  int printFlags;
  if (metaLevel->downPrintOptionSet(message->getArgument(5), printFlags))
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
		      MixfixModule::AliasMap aliasMap;
		      if (metaLevel->downVariableDeclSet(message->getArgument(3), aliasMap, m))
			{
			  if (Term* t = metaLevel->downTerm(message->getArgument(4), m))
			    {
			      //
			      //	Swap in our alias map and a null parser.
			      //
			      MixfixParser* parser = 0;
			      m->swapVariableAliasMap(aliasMap, parser);
			      //
			      //	Do the pretty print.
			      //
			      Vector<int> buffer;
			      m->bufferPrint(buffer, t, printFlags);
			      t->deepSelfDestruct();
			      //
			      //	Restore original alias map and parser.
			      //
			      m->swapVariableAliasMap(aliasMap, parser);
			      //
			      //	Assemble the reply.
			      //
			      Vector<DagNode*> reply(3);
			      DagNode* target = message->getArgument(1);
			      reply[0] = target;
			      reply[1] = message->getArgument(0);
			      reply[2] = metaLevel->upQidList(buffer);
			      context.bufferMessage(target, printedTermMsg->makeDagNode(reply));
			      return true;
			    }
			}
		    }
		}
	    }
	}
    }
  return false;
}

#include "metaLevelOpSymbol.hh"

bool
InterpreterManagerSymbol::parseTerm(FreeDagNode* message, ObjectSystemRewritingContext& context)
{
  //
  //	op parseTerm : Oid Oid Qid VariableSet QidList Type? -> Msg [ctor msg format (b o)] .
  //	                0   1   2       3         4      5
  //
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
		  ConnectedComponent* component;
		  Vector<int> metaTokens;
		  if (metaLevel->downComponent(message->getArgument(5), m, component) &&
		      metaLevel->downQidList(message->getArgument(4), metaTokens))
		    {
		      //
		      //	Check if we're already got an (AliasMap, MixfixParser) pair
		      //	the MetaOpCache associated with our module.
		      //
		      MetaLevelOpSymbol::AliasMapParserPair* pair;
		      CacheableState* cachedPair;
		      Int64 dummy;
		      MetaModule* mm = safeCast(MetaModule*, m);  // metaInterpreter can only store MetaModules
		      if (mm->remove(message, cachedPair, dummy, 2 /* ignore last two arguments */))
			{
			  DebugInfo("cache hit for " << message);
			  pair = safeCast(MetaLevelOpSymbol::AliasMapParserPair*, cachedPair);
			}
		      else
			{
			  //
			  //	We don't have a cached (AliasMap, MixfixParser) pair
			  //	for (Module, VariableSet) so we need to make one.
			  //
			  //	We don't have special treatment for the empty AliasMap.
			  //	MixfixParsers are only made by makeGrammar() in response
			  //	to a parse request so we don't expect to have a default
			  //	parser that we could use.
			  //
			  pair = new MetaLevelOpSymbol::AliasMapParserPair();  // empty map, null parser
			  if (!(metaLevel->downVariableDeclSet(message->getArgument(3), pair->aliasMap, m)))
			    {
			      delete pair;
			      return false;
			    }
			}
		      //
		      //	Swap our (AliasMap, MixfixParser) pair with the AliasMap
		      //	and Parser in m.
		      //	
		      m->swapVariableAliasMap(pair->aliasMap, pair->parser);
		      //
		      //	Turn meta-tokens into tokens.
		      //
		      int nrTokens = metaTokens.length();
		      Vector<Token> tokens(nrTokens);
		      for (int i = 0; i < nrTokens; i++)
			tokens[i].tokenize(metaTokens[i], FileTable::META_LEVEL_CREATED);
		      //
		      //	We got this far so we are going to produce a reply,
		      //	even if it's a parse error.
		      //
		      Vector<DagNode*> reply(3);
		      DagNode* target = message->getArgument(1);
		      reply[0] = target;
		      reply[1] = message->getArgument(0);
		      //
		      //	Do the parse.
		      //
		      Term* parse1;
		      Term* parse2;
		      int firstBad;
		      switch(m->parseTerm2(tokens, component, parse1, parse2, firstBad))
			{
			case -1:	// bad token
			case 0:		// no parse
			  {
			    reply[2] = metaLevel->upNoParse(firstBad);
			    break;
			  }
			case 1:		// 1 parse
			  {
			    parse1->symbol()->fillInSortInfo(parse1);
			    reply[2] = metaLevel->upResultPair(parse1, m);
			    parse1->deepSelfDestruct();
			    break;
			  }
			default:	// more than 1 parse
			  {
			    parse1->symbol()->fillInSortInfo(parse1);
			    parse2->symbol()->fillInSortInfo(parse2);
			    reply[2] = metaLevel->upAmbiguity(parse1, parse2, m);
			    parse1->deepSelfDestruct();
			    parse2->deepSelfDestruct();
			  }
			}
		      //
		      //	Restore AliasMaps and Parsers.
		      //
		      m->swapVariableAliasMap(pair->aliasMap, pair->parser);
		      //
		      //	Cache the AliasMap and Parser we just used.
		      //
		      mm->insert(message, pair, 0 /* dummy value */);
		      
		      context.bufferMessage(target, parsedTermMsg->makeDagNode(reply));
		      return true;
		    }
		}
	    }
	}
    }
  return false;
}
