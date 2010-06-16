/*

    This file is part of the Maude 2 interpreter.

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
//      Abstract class to manage access to a shared MetaLevel object.
//
#ifndef _interpreterManagerSymbol_hh_
#define _interpreterManagerSymbol_hh_
#include "externalObjectManagerSymbol.hh"

class InterpreterManagerSymbol : public ExternalObjectManagerSymbol
{
  NO_COPYING(InterpreterManagerSymbol);

public:
  InterpreterManagerSymbol(int id);
  ~InterpreterManagerSymbol();

  bool attachData(const Vector<Sort*>& opDeclaration,
		  const char* purpose,
		  const Vector<const char*>& data);
  bool attachSymbol(const char* purpose, Symbol* symbol);
  bool attachTerm(const char* purpose, Term* term);
  void copyAttachments(Symbol* original, SymbolMap* map);

  void getDataAttachments(const Vector<Sort*>& opDeclaration,
			  Vector<const char*>& purposes,
			  Vector<Vector<const char*> >& data);
  void getSymbolAttachments(Vector<const char*>& purposes, Vector<Symbol*>& symbols);
  void getTermAttachments(Vector<const char*>& purposes, Vector<Term*>& terms);

  void postInterSymbolPass();
  void reset();

  bool handleManagerMessage(DagNode* message, ObjectSystemRewritingContext& context);
  bool handleMessage(DagNode* message, ObjectSystemRewritingContext& context);
  void cleanUp(DagNode* objectId);

private:
  bool okToBind();

  bool insertModule(FreeDagNode* message, ObjectSystemRewritingContext& context);
  bool showModule(FreeDagNode* message, ObjectSystemRewritingContext& context);
  bool reduceTerm(FreeDagNode* message, ObjectSystemRewritingContext& context);
  bool rewriteTerm(FreeDagNode* message, ObjectSystemRewritingContext& context);
  bool frewriteTerm(FreeDagNode* message, ObjectSystemRewritingContext& context);
  bool quit(FreeDagNode* message, ObjectSystemRewritingContext& context);

  bool getInterpreter(DagNode* interpreterArg, Interpreter*& interpreter);
  bool deleteInterpreter(DagNode* interpreterArg);
  void createInterpreter(FreeDagNode* originalMessage, ObjectSystemRewritingContext& context);

  static DagNode* term2Dag(Term* t);

  MetaLevel* metaLevel;
  MetaLevelOpSymbol* shareWith;  // if this is nonzero then it points to the true owner of the MetaLevel object

#define MACRO(SymbolName, SymbolClass, NrArgs) \
  SymbolClass* SymbolName;
#include "interpreterSignature.cc"
#undef MACRO

  Vector<Interpreter*> interpreters;
};

#endif
