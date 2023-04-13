/*

    This file is part of the Maude 3 interpreter.

    Copyright 1997-2023 SRI International, Menlo Park, CA 94025, USA.

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
//	Class for formulae in some propositional logic.
//
#ifndef _logicFormula_hh_
#define _logicFormula_hh_

class LogicFormula
{
public:
  enum Op
  {
    //
    //	Primitive
    //
    PROPOSITION,
    LTL_TRUE,
    LTL_FALSE,
    //
    //	Unary
    //
    NOT,
    NEXT,
    //
    //	Binary
    //
    AND,
    OR,
    UNTIL,
    RELEASE
  };

  int makeProp(int propIndex);
  int makeOp(Op op, int firstArg = NONE, int secondArg = NONE);

  int getProp(int nodeNr) const;
  Op getOp(int nodeNr) const;
  int getArg(int nodeNr, int argNr = 0) const;
  bool isPropositional(int nodeNr) const;
  /*
  bool structuallyImplies(const NatSet& now, const NatSet& next, int candidate) const;
  bool structuallyContradicts(const NatSet& now, const NatSet& next, int candidate) const;
  bool containsNegation(const NatSet& indexSet, int formulaIndex) const;
  bool isNegation(int f1, int f2) const;
  */
  void dump(ostream& s);

private:
  struct Node
  {
    short op;
    bool propositional;
    int args[2];
  };

  Vector<Node> nodes;
};

inline LogicFormula::Op
LogicFormula::getOp(int nodeNr) const
{
  return static_cast<Op>(nodes[nodeNr].op);
}

inline int
LogicFormula::getProp(int nodeNr) const
{
  return nodes[nodeNr].args[0];
}

inline int
LogicFormula::getArg(int nodeNr, int argNr) const
{
  return nodes[nodeNr].args[argNr];
}

inline bool
LogicFormula::isPropositional(int nodeNr) const
{
  return nodes[nodeNr].propositional;
}

#endif
