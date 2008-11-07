/*

    This file is part of the Maude 2 interpreter.

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
//      Implementation for class PrintAttribute.
//

//      utility stuff
#include "macros.hh"
#include "vector.hh"
 
//      forward declarations
#include "interface.hh"
#include "core.hh"
#include "variable.hh"

//      interface class definitions
#include "symbol.hh"
#include "dagNode.hh"
 
//      core class definitions
#include "substitution.hh"
#include "preEquation.hh"
#include "conditionFragment.hh"

//      variable class definitions
#include "variableTerm.hh"

//	mixfix class definitions
#include "token.hh"
#include "printAttribute.hh"

void
PrintAttribute::fillOut(const PreEquation& statement,
			const Vector<int>& names, 
			const Vector<Sort*>& sorts)
{
  Assert(names.size() == sorts.size(), "vector size mismatch");
  int nrItems = names.size();
  for (int i = 0; i < nrItems; ++i)
    {
      int name = names[i];
      Sort* sort = sorts[i];
      if (sort == 0)
	items.append(name);  // string case
      else
	{
	  int index = findVariableIndex(statement, name,  sort);
	  if (index == NONE)
	    IssueWarning(statement << ": print attribute variable unbound.");
	  else
	    items.append(-1 - index);
	}
    }
}

int
PrintAttribute::findVariableIndex(const VariableInfo& variableInfo, int name, Sort* sort)
{
  int nrVariables = variableInfo.getNrRealVariables();
  for (int i = 0; i < nrVariables; ++i)
    {
      VariableTerm* v = safeCast(VariableTerm*, variableInfo.index2Variable(i));
      if (v->id() == name && v->getSort() == sort)
	return i;
    }
  return NONE;
}

void
PrintAttribute::print(ostream& s, const VariableInfo& variableInfo) const
{
  s << "print";
  int nrItems = items.size();
  for (int i = 0; i < nrItems; ++i)
    {
      s << ' ';
      int item = items[i];
      if (item >= 0)
	s << Token::name(item);
      else
	s << variableInfo.index2Variable(-1 - item);
    }
}

void
PrintAttribute::print(ostream& s, const Substitution& substitution) const
{
  int nrItems = items.size();
  for (int i = 0; i < nrItems; ++i)
    {
      int item = items[i];
      if (item >= 0)
	s << Token::codeToRope(item);
      else
	s << substitution.value(-1 - item);
    }
}
