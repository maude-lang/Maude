/*

    This file is part of the Maude 3 interpreter.

    Copyright 2017 SRI International, Menlo Park, CA 94025, USA.

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
//      Class to handle standard stream manager symbols.
//
#ifndef _streamManagerSymbol_hh_
#define _streamManagerSymbol_hh_
#include "externalObjectManagerSymbol.hh"

class StreamManagerSymbol : public ExternalObjectManagerSymbol
{
  NO_COPYING(StreamManagerSymbol);

public:
  StreamManagerSymbol(int id);
  //~StreamManagerSymbol();

  bool attachData(const Vector<Sort*>& opDeclaration,
		  const char* purpose,
		  const Vector<const char*>& data);
  bool attachSymbol(const char* purpose, Symbol* symbol);

  void copyAttachments(Symbol* original, SymbolMap* map);

  void getDataAttachments(const Vector<Sort*>& opDeclaration,
			  Vector<const char*>& purposes,
			  Vector<Vector<const char*> >& data);
  void getSymbolAttachments(Vector<const char*>& purposes, Vector<Symbol*>& symbols);
  //
  //	Overridden methods from ExternalObjectManagerSymbol.
  //
  bool handleManagerMessage(DagNode* message, ObjectSystemRewritingContext& context);
  bool handleMessage(DagNode* message, ObjectSystemRewritingContext& context);
  void cleanUp(DagNode* objectId);

private:
  //
  //	Actions.
  //
  bool write(FreeDagNode* message, ObjectSystemRewritingContext& context);
  bool getLine(FreeDagNode* message, ObjectSystemRewritingContext& context);
  //
  //	Reply.
  //
  void gotLineReply(const Rope& line,
		    FreeDagNode* originalMessage,
		    ObjectSystemRewritingContext& context);

#define MACRO(SymbolName, SymbolClass, NrArgs) \
  SymbolClass* SymbolName;
#include "streamSignature.cc"
#undef MACRO
  int streamNr;
};

#endif
