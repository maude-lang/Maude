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
//	Implementation for class LogicFormula.
//

//	utility stuff
#include "macros.hh"
#include "vector.hh"
#include "natSet.hh"

//	temporal class definitions
#include "logicFormula.hh"

int
LogicFormula::makeProp(int propIndex)
{
  int nrNodes = nodes.length();
  for (int i = 0; i < nrNodes; i++)
    {
      Node& n = nodes[i];
      if (n.op == PROPOSITION && n.args[0] == propIndex)
	return i;
    }
  nodes.expandBy(1);
  Node& n = nodes[nrNodes];
  n.op = PROPOSITION;
  n.args[0] = propIndex;
  n.propositional = true;
  return nrNodes;
}

int
LogicFormula::makeOp(Op op, int firstArg, int secondArg)
{
  int nrNodes = nodes.length();
  for (int i = 0; i < nrNodes; i++)
    {
      Node& n = nodes[i];
      if (n.op == op && n.args[0] == firstArg && n.args[1] == secondArg)
	return i;
    }
  nodes.expandBy(1);
  Node& n = nodes[nrNodes];
  n.op = op;
  n.args[0] = firstArg;
  n.args[1] = secondArg;
  n.propositional = false;
  switch (op)
    {
    case AND:
    case OR:
      {
	if (!(nodes[secondArg].propositional))
	  break;
	// fall thru
      }
    case NOT:
      {
	if (!(nodes[firstArg].propositional))
	  break;
	// fall thru
      }
    case LTL_TRUE:
    case LTL_FALSE:
      {
	n.propositional = true;
	break;
      }
    default:
      break;
    }
  return nrNodes;
}

/*
bool
LogicFormula::structuallyImplies(const NatSet& now, const NatSet& next, int candidate) const
{
  if (now.contains(candidate))
    return true;
  switch (getOp(candidate))
    {
    case LTL_TRUE:
      return true;
    case NEXT:
      return next.contains(getArg(candidate, 0));
    case AND:
      {
	return structuallyImplies(now, next, getArg(candidate, 0)) &&
	  structuallyImplies(now, next, getArg(candidate, 1));
      }
    case OR:
      {
	return structuallyImplies(now, next, getArg(candidate, 0)) ||
	  structuallyImplies(now, next, getArg(candidate, 1));
      }
    case UNTIL:
      {
	return structuallyImplies(now, next, getArg(candidate, 1)) ||
	  (structuallyImplies(now, next, getArg(candidate, 0)) && next.contains(candidate));
      }
    case RELEASE:
      {
	return structuallyImplies(now, next, getArg(candidate, 1)) &&
	  (structuallyImplies(now, next, getArg(candidate, 0)) || next.contains(candidate));
      }
    default:
      break;
    }
  return false;
}

bool
LogicFormula::structuallyContradicts(const NatSet& now, const NatSet& next, int candidate) const
{
  if (containsNegation(now, candidate))
    return true;
  switch (getOp(candidate))
    {
    case LTL_FALSE:
      return true;
    case NEXT:
      return containsNegation(next, getArg(candidate, 0));
    case AND:
      {
	return structuallyContradicts(now, next, getArg(candidate, 0)) ||
	  structuallyContradicts(now, next, getArg(candidate, 1));
      }
    case OR:
      {
	return structuallyContradicts(now, next, getArg(candidate, 0)) &&
	  structuallyContradicts(now, next, getArg(candidate, 1));
      }
    case UNTIL:
      {
	return structuallyContradicts(now, next, getArg(candidate, 1)) &&
	  (structuallyContradicts(now, next, getArg(candidate, 0)) ||
	   containsNegation(next, candidate));
      }
    case RELEASE:
      {
	return structuallyContradicts(now, next, getArg(candidate, 1)) ||
	  (structuallyContradicts(now, next, getArg(candidate, 0)) &&
	   containsNegation(next, candidate));
      }
    default:
      break;
    }
  return false;
}

bool
LogicFormula::containsNegation(const NatSet& indexSet, int formulaIndex) const
{
  int max = indexSet.max();
  for (int i = 0; i <= max; i++)
    {
      if (indexSet.contains(i) && isNegation(i, formulaIndex))
	return true;
    }
  return false;
}

bool
LogicFormula::isNegation(int f1, int f2) const
{
  static Op inv[] =
  {
    NOT, LTL_FALSE, LTL_TRUE, PROPOSITION, NEXT, OR, AND, RELEASE, UNTIL
  };

  Op op1 = getOp(f1);
  if (getOp(f2) == inv[op1])
    {
      switch (op1)
	{
	case PROPOSITION:
	  return getArg(f2, 0) == f1;
	case LTL_TRUE:
	case LTL_FALSE:
	  return true;
	case NOT:
	  return getArg(f1, 0) == f2;
	case NEXT:
	  return isNegation(getArg(f1, 0), getArg(f2, 0));
	case AND:
	case OR:
	case UNTIL:
	case RELEASE:
	  {
	    return isNegation(getArg(f1, 0), getArg(f2, 0)) &&
	      isNegation(getArg(f1, 1), getArg(f2, 1));
	  }
	}
    }
  return false;
}
*/

void
LogicFormula::dump(ostream& s)
{
  int nrNodes = nodes.length();
  for (int i = 0; i < nrNodes; i++)
    {
      s << i << '\t';
      Node& n = nodes[i];
      switch (n.op)
	{
	case PROPOSITION:
	  s << "prop" << n.args[0];
	  break;
	case LTL_TRUE:
	  s << "True";
	  break;
	case LTL_FALSE:
	  s << "False";
	  break;
	case NOT:
	  s << "~ " << n.args[0];
	  break;
	case NEXT:
	  s << "@ " << n.args[0];
	  break;
	case AND:
	  s << n.args[0] << " /\\ " << n.args[1];
	  break;
	case OR:
	  s << n.args[0] << " \\/ " << n.args[1];
	  break;
	case UNTIL:
	  s << n.args[0] << " U " << n.args[1];
	  break;
	case RELEASE:
	  s << n.args[0] << " R " << n.args[1];
	  break;
	}
      s << '\n';
    }
}
