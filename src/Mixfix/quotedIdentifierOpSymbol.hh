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
#define MOS 1
//
//      Class for symbols for built in operations on quoted identifiers.
//
#ifndef _quotedIdentifierOpSymbol_hh_
#define _quotedIdentifierOpSymbol_hh_
#include "freeSymbol.hh"

class QuotedIdentifierOpSymbol : public FreeSymbol
{
public:
  QuotedIdentifierOpSymbol(int id, int nrArgs);

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

  bool eqRewrite(DagNode* subject, RewritingContext& context);

private:
  NO_COPYING(QuotedIdentifierOpSymbol);

#ifdef MOS
  static int counter;
#endif

  int op;
  QuotedIdentifierSymbol* quotedIdentifierSymbol;
  StringSymbol* stringSymbol;
};

#endif
