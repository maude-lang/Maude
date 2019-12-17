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
//      Implementation for class FileManagerSymbol.
//

//      utility stuff
#include "macros.hh"
#include "vector.hh"

//      forward declarations
#include "interface.hh"
#include "core.hh"
#include "freeTheory.hh"
#include "NA_Theory.hh"
#include "S_Theory.hh"
#include "builtIn.hh"
#include "objectSystem.hh"

//      interface class definitions
#include "symbol.hh"
#include "dagNode.hh"
#include "term.hh"

//      core class definitions
#include "symbolMap.hh"

//      free theory class definitions
#include "freeDagNode.hh"

//      built in class definitions
#include "succSymbol.hh"
#include "minusSymbol.hh"
#include "stringSymbol.hh"
#include "stringDagNode.hh"
#include "bindingMacros.hh"

//	object system class definitions
#include "objectSystemRewritingContext.hh"
#include "streamManagerSymbol.hh"

#include "../IO_Stuff/IO_Manager.hh"  // HACK
extern IO_Manager ioManager;  // HACK

StreamManagerSymbol::StreamManagerSymbol(int id)
  : ExternalObjectManagerSymbol(id)
{
#define MACRO(SymbolName, SymbolClass, NrArgs) \
  SymbolName = 0;
#include "streamSignature.cc"
#undef MACRO
}

bool
StreamManagerSymbol::attachData(const Vector<Sort*>& opDeclaration,
				const char* purpose,
				const Vector<const char*>& data)
{
  if (data.length() == 1)
    {
      const char* streamName = data[0];
      if (strcmp(streamName, "stdin") == 0)
	streamNr = STDIN_FILENO;
      else if(strcmp(streamName, "stdout") == 0)
	streamNr = STDOUT_FILENO;
      else if(strcmp(streamName, "stderr") == 0)
	streamNr = STDERR_FILENO;
      else
	return ExternalObjectManagerSymbol::attachData(opDeclaration, purpose, data);
      return true;
    }
  return ExternalObjectManagerSymbol::attachData(opDeclaration, purpose, data);
}

bool
StreamManagerSymbol::attachSymbol(const char* purpose, Symbol* symbol)
{
  Assert(symbol != 0, "null symbol for " << purpose);
#define MACRO(SymbolName, SymbolClass, NrArgs) \
  BIND_SYMBOL(purpose, symbol, SymbolName, SymbolClass*)
#include "streamSignature.cc"
#undef MACRO
  return ExternalObjectManagerSymbol::attachSymbol(purpose, symbol);
}

void
StreamManagerSymbol::copyAttachments(Symbol* original, SymbolMap* map)
{
  StreamManagerSymbol* orig = safeCast(StreamManagerSymbol*, original);
  streamNr = orig->streamNr;
#define MACRO(SymbolName, SymbolClass, NrArgs) \
  COPY_SYMBOL(orig, SymbolName, map, SymbolClass*)
#include "streamSignature.cc"
#undef MACRO
  ExternalObjectManagerSymbol::copyAttachments(original, map);
}

void
StreamManagerSymbol::getDataAttachments(const Vector<Sort*>& opDeclaration,
				      Vector<const char*>& purposes,
				      Vector<Vector<const char*> >& data)
{
  int nrDataAttachments = purposes.length();
  purposes.resize(nrDataAttachments + 1);
  purposes[nrDataAttachments] = "StreamManagerSymbol";
  data.resize(nrDataAttachments + 1);
  data[nrDataAttachments].resize(1);
  data[nrDataAttachments][0] = (streamNr == STDIN_FILENO) ? "stdin" :
    ((streamNr == STDOUT_FILENO) ? "stdout" : "stderr");
  ExternalObjectManagerSymbol::getDataAttachments(opDeclaration, purposes, data);
}

void
StreamManagerSymbol::getSymbolAttachments(Vector<const char*>& purposes,
					  Vector<Symbol*>& symbols)
{
#define MACRO(SymbolName, SymbolClass, NrArgs) \
  APPEND_SYMBOL(purposes, symbols, SymbolName)
#include "streamSignature.cc"
#undef MACRO
  ExternalObjectManagerSymbol::getSymbolAttachments(purposes, symbols);
}

bool
StreamManagerSymbol::handleManagerMessage(DagNode* message, ObjectSystemRewritingContext& context)
{
  //cerr << "StreamManagerSymbol::handleManagerMessage(): saw " << message << endl;
  Symbol* s = message->symbol();
  if (s == writeMsg)
    return write(safeCast(FreeDagNode*, message), context);
  if (s == getLineMsg)
    return getLine(safeCast(FreeDagNode*, message), context);
  return false;
}

bool
StreamManagerSymbol::handleMessage(DagNode* message, ObjectSystemRewritingContext& context)
{
  CantHappen("StreamManagerSymbol::handleMessage() : " << message);
  return false;
}

void
StreamManagerSymbol::cleanUp(DagNode* objectId)
{
  CantHappen("StreamManagerSymbol::cleanUp() : " << objectId);
}

bool
StreamManagerSymbol::write(FreeDagNode* message, ObjectSystemRewritingContext& context)
{
  if (streamNr == STDOUT_FILENO || streamNr == STDERR_FILENO)
    {
      DagNode* textArg = message->getArgument(2);
      if (textArg->symbol() == stringSymbol)
	{
	  Rope text = safeCast(StringDagNode*, textArg)->getValue();
	  if (!(text.empty()))
	    {
	      if (streamNr == STDOUT_FILENO)
		cout << text << flush;
	      else
		cerr << text;
	      trivialReply(wroteMsg, message, context);
	      return true;
	    }
	  IssueAdvisory(message->getArgument(0) << " declined message " << message);
	}
    }
  return false;
}

bool
StreamManagerSymbol::getLine(FreeDagNode* message, ObjectSystemRewritingContext& context)
{
  if (streamNr == STDIN_FILENO)
    {
      DagNode* promptArg = message->getArgument(2);
      if (promptArg->symbol() == stringSymbol)
	{
	  //Rope line = ioManager.getLine(safeCast(StringDagNode*, promptArg)->getValue(), stdin);
	  Rope line = ioManager.getLineFromStdin(safeCast(StringDagNode*, promptArg)->getValue());
	  gotLineReply(line, message, context);
	  return true;
	}
      IssueAdvisory(message->getArgument(0) << " declined message " << message);
    }
  return false;
}

void
StreamManagerSymbol::gotLineReply(const Rope& line,
				  FreeDagNode* originalMessage,
				  ObjectSystemRewritingContext& context)
{
  Vector<DagNode*> reply(3);
  reply[1] = originalMessage->getArgument(0);
  reply[2] = new StringDagNode(stringSymbol, line);
  DagNode* target = originalMessage->getArgument(1);
  reply[0] = target;
  context.bufferMessage(target, gotLineMsg->makeDagNode(reply));
}
