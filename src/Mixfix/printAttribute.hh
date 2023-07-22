/*

    This file is part of the Maude 3 interpreter.

    Copyright 1997-2008 SRI International, Menlo Park, CA 94025, USA.

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
//	Class for holding and printing print attributes.
//
#ifndef _printAttribute_hh_
#define _printAttribute_hh_

class PrintAttribute
{
public:
  void fillOut(const PreEquation& statement,
	       const Vector<int>& names, 
	       const Vector<Sort*>& sorts);
  int getNrItems() const;
  int getTokenCode(int i) const;
  int getVariableIndex(int i) const;

  void print(ostream& s, const VariableInfo& variableInfo) const;  // for show
  void print(ostream& s, const Substitution& substitution) const;  // for runtime
  void latexPrint(ostream& s, const VariableInfo& variableInfo) const;

private:
  static int findVariableIndex(const VariableInfo& variableInfo, int name, Sort* sort);

  Vector<int> items;  // -ve values are variable indices
};

inline int
PrintAttribute::getNrItems() const
{
  return items.size();
}

inline int
PrintAttribute::getTokenCode(int i) const
{
  return items[i] > 0 ? items[i] : NONE;
}

inline int
PrintAttribute::getVariableIndex(int i) const
{
  return items[i] < 0 ? (-1 - items[i]) : NONE;
}

#endif
