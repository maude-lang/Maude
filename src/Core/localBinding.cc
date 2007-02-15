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
//      Implementation for class Substitution
//

//	utility stuff
#include "macros.hh"
#include "vector.hh"
#include "indent.hh"
//#include "variable.hh"

//      forward declarations
#include "interface.hh"
#include "core.hh"

//      interface class definitions
#include "symbol.hh"
#include "dagNode.hh"

//      core class definitions
#include "variableInfo.hh"
#include "substitution.hh"
#include "subproblemAccumulator.hh"
#include "localBinding.hh"

LocalBinding::LocalBinding(int maxSize) : bindings(0, maxSize)
{
}

void
LocalBinding::markReachableNodes()
{
  int nrBindings = bindings.length();
  for (int i = 0; i < nrBindings; i++)
    {
      Assert(bindings[i].value != 0, "null local binding at index " << i);
      bindings[i].value->mark();
    }
}

bool
LocalBinding::assert(Substitution& substitution)
{
  const Vector<Binding>::iterator b = bindings.begin();
  const Vector<Binding>::iterator e = bindings.end();
  for (Vector<Binding>::iterator i = b; i != e; ++i)
    {
      DagNode* d = substitution.value(i->variableIndex);
      if (d != 0 && !(d->equal(i->value)))
        return false;
    }
  for (Vector<Binding>::iterator i = b; i != e; ++i)
    {
      int index = i->variableIndex;
      if (substitution.value(index) == 0)
	{
	  substitution.bind(index, i->value);
	  i->active = true;
	}
    }
  return true;
}

void
LocalBinding::retract(Substitution& substitution)
{
  const Vector<Binding>::iterator e = bindings.end();
  for (Vector<Binding>::iterator i = bindings.begin(); i != e; ++i)
    {
      if (i->active)
        {
          i->active = false;
          substitution.bind(i->variableIndex, 0);
        }
    }
}

bool
LocalBinding::unificationAssert(Substitution& substitution, Subproblem*& returnedSubproblem)
{
  SubproblemAccumulator subproblems;
  int nrBindings = bindings.size();
  for (int i = 0; i < nrBindings; ++i)
    {
      int index = bindings[i].variableIndex;
      DagNode* newValue = bindings[i].value;
      DagNode* originalValue = substitution.value(index);
      if (originalValue == 0)
	substitution.unificationBind(index, 0, newValue);
      else
	{
	  if (!(originalValue->unify(newValue, substitution, returnedSubproblem, 0)))
	    return false;
	  subproblems.add(returnedSubproblem);
	}
      returnedSubproblem = subproblems.extractSubproblem();
    }
  return true;
}

#ifdef DUMP
void
LocalBinding::dump(ostream& s, const VariableInfo& variableInfo, int indentLevel) const
{
  s << Indent(indentLevel) << "Begin{LocalBinding}\n";
  ++indentLevel;
  //  s << Indent(indentLevel) << "this = " << this << "\tprev = " << prev <<
  //  "\tnext = " << next << '\n';
  int nrBindings = bindings.length();
  for (int i = 0; i < nrBindings; i++)
    {
      int index = bindings[i].variableIndex;
      s << Indent(indentLevel) << "index = " << index <<
	" \"" << variableInfo.index2Variable(index) <<
	"\"\tvalue = " << bindings[i].value << "\t(";
      if (!(bindings[i].active))
	s << "not ";
      s << "active)\n";
    }
  s << Indent(indentLevel - 1) << "End{LocalBinding}\n";
}
	
 
void
LocalBinding::dump(ostream& s, const VariableInfo* variableInfo) const
{
  s << "Dumping LocalBinding (" << this << ")\n";
  //  s << "prev = " << prev << "\tnext = " << next << '\n';
  int nrBindings = bindings.length();
  for (int i = 0; i < nrBindings; i++)
    {
      if (variableInfo != 0)
	s << variableInfo->index2Variable(bindings[i].variableIndex);
      else
	s << "index:" << bindings[i].variableIndex;
      s << "\t=\t" << bindings[i].value << "\t(";
      if (!(bindings[i].active))
	s << "not ";
      s << "active)\n";
    }
}
#endif
