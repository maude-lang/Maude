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
//      Class for symbols for built in loop operation.
//
#ifndef _loopSymbol_hh_
#define _loopSymbol_hh_
#include "freeSymbol.hh"

class LoopSymbol : public FreeSymbol
{
public:
  LoopSymbol(int id);

  bool attachData(const Vector<Sort*>& opDeclaration,
		  const char* purpose,
		  const Vector<const char*>& data);
  bool attachSymbol(const char* purpose, Symbol* symbol);
  void copyAttachments(Symbol* original, SymbolMap* map);
  void getDataAttachments(const Vector<Sort*>& opDeclaration,
			  Vector<const char*>& purposes,
			  Vector<Vector<const char*> >& data);
  void getSymbolAttachments(Vector<const char*>& purposes,
			    Vector<Symbol*>& symbols);

  bool extractOutput(DagNode* loopNode, Vector<int>& bubble);
  void injectInput(DagNode* loopNode, const Vector<Token>& bubble);

private:
  NO_COPYING(LoopSymbol);

  DagNode* createQidList(const Vector<Token>& ids);
  bool extractQid(DagNode* metaQid, int& id);
  bool extractQidList(DagNode* metaQidList, Vector<int>& ids);

  static const Vector<int> eagerStrategy;

  QuotedIdentifierSymbol* qidSymbol;
  Symbol* nilQidListSymbol;
  AU_Symbol* qidListSymbol;
};

#endif
