/*

    This file is part of the Maude 3 interpreter.

    Copyright 1997-2021 SRI International, Menlo Park, CA 94025, USA.

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
//      Class for modules that can be printed out.
//
#ifndef _visibleModule_hh_
#define _visibleModule_hh_
#include "importModule.hh"

class VisibleModule : public ImportModule
{
  NO_COPYING(VisibleModule);

public:
  VisibleModule(int name, ModuleType moduleType, Interpreter* owner);

  Interpreter* getOwner() const;

  void showSummary(ostream& s);
  void showKinds(ostream& s) const;
  void showSortsAndSubsorts(ostream& s) const;
  void showModule(ostream& s, bool all) const;
  void showPolymorphs(ostream& s, bool indent, bool all) const;
  void showOps(ostream& s, bool indent, bool all) const;
  void showVars(ostream& s, bool indent) const;
  void showMbs(ostream& s, bool indent, bool all) const;
  void showEqs(ostream& s, bool indent, bool all) const;
  void showRls(ostream& s, bool indent, bool all) const;
  void showStrats(ostream& s, bool indent, bool all) const;
  void showSds(ostream& s, bool indent, bool all) const;

private:
  void showImports(ostream& s) const;
  void showSorts1(ostream& s, bool indent, bool all) const;
  void showSubsorts(ostream& s, bool indent, bool all) const;
  void showPolymorphDecl(ostream& s, bool indent, int index) const;
  void showPolymorphAttributes(ostream& s, int index) const;
  void showDecls(ostream& s, bool indent, int index, bool all) const;
  void showAttributes(ostream& s, Symbol* symbol, int opDeclIndex) const;

  Interpreter* const owner;
};

inline Interpreter*
VisibleModule::getOwner() const
{
  return owner;
}

#endif
