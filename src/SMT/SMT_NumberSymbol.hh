/*

    This file is part of the Maude 3 interpreter.

    Copyright 2014 SRI International, Menlo Park, CA 94025, USA.

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
//      Class for symbols representing SMT numeric constants.
//
#ifndef _SMT_NumberSymbol_hh_
#define _SMT_NumberSymbol_hh_
#include "NA_Symbol.hh"
#include "SMT_Base.hh"

class SMT_NumberSymbol : public NA_Symbol, public SMT_Base
{
public:
  SMT_NumberSymbol(int id);
  //
  //	Standard functionality for managing hooks.
  //
  bool attachData(const Vector<Sort*>& opDeclaration,
		  const char* purpose,
		  const Vector<const char*>& data);
  void copyAttachments(Symbol* original, SymbolMap* map);
  void getDataAttachments(const Vector<Sort*>& opDeclaration,
			  Vector<const char*>& purposes,
			  Vector<Vector<const char*> >& data);

  //
  //	For NA_Symbol base.
  //
  Term* termify(DagNode* dagNode);
  //
  //	For SMT_Base.
  //
  void fillOutSMT_Info(SMT_Info& info);

private:
  int numberSystem;
};

#endif
