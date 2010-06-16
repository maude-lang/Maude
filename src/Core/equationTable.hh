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
//	Abstract class for table of equations indexed under some symbol.
//
#ifndef _equationTable_hh_
#define _equationTable_hh_

class EquationTable
{
public:
  void offerEquation(Equation* equation);
  const Vector<Equation*>& getEquations() const;
  bool equationFree() const;
  virtual void compileEquations();

protected:
  virtual bool acceptEquation(Equation* equation) = 0;
  bool applyReplace(DagNode* subject,
		    RewritingContext& context,
		    ExtensionInfo* extensionInfo = 0);
  bool applyReplaceNoOwise(DagNode* subject,
			   RewritingContext& context,
			   ExtensionInfo* extensionInfo = 0);

#ifdef DUMP
  virtual void dumpEquationTable(ostream& s, int indentLevel);
#endif

private:
  Vector<Equation*> equations;
};

inline void
EquationTable::offerEquation(Equation* equation)
{
   if (acceptEquation(equation))
     equations.append(equation);
}

inline const Vector<Equation*>&
EquationTable::getEquations() const
{
  return equations;
}

inline bool
EquationTable::equationFree() const
{
  return equations.isNull();
}

#endif
