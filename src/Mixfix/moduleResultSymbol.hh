/*

    This file is part of the Maude 3 interpreter.

    Copyright 2026 SRI International, Menlo Park, CA 94025, USA.

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
//      Class for module result symbols.
//
//	They don't have any special operational semantics but serve to
//	identify kinds used for user-defined module operations and to
//	hold related data.
//
#ifndef _moduleResultSymbol_hh_
#define _moduleResultSymbol_hh_
#include "resultSymbol.hh"

class ModuleResultSymbol : public ResultSymbol
{
  NO_COPYING(ModuleResultSymbol);

public:
  ModuleResultSymbol(int id);
  
  ImportModule* makeTransformation(int newModuleName,
				   const Vector<ImportModule*>& inputModules,
				   const Vector<int>& optionVec,
				   Interpreter* owner,
				   LineNumber lineNumber);

private:
  bool attachData(const Vector<Sort*>& opDeclaration,
		  const char* purpose,
		  const Vector<const char*>& data);
  void getDataAttachments(const Vector<Sort*>& opDeclaration,
			  Vector<const char*>& purposes,
			  Vector<Vector<const char*>>& data);
  
  void flattenModule(ImportModule* module);  // maybe move this to ImportModule
};

#endif
