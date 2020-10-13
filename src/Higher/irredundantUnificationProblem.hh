/*

    This file is part of the Maude 3 interpreter.

    Copyright 2020 SRI International, Menlo Park, CA 94025, USA.

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
//	Class for searching for a minimal set complete set of order-sorted unifiers.
//
#ifndef _irredundantUnificationProblem_hh_
#define _irredundantUnificationProblem_hh_
#include "unificationProblem.hh"

class IrredundantUnificationProblem : public UnificationProblem
{
  NO_COPYING(IrredundantUnificationProblem);

public:
  IrredundantUnificationProblem(Vector<Term*>& lhs,
				Vector<Term*>& rhs,
				FreshVariableGenerator* freshVariableGenerator,
				int incomingVariableFamily = NONE);
  ~IrredundantUnificationProblem();
  //
  //	We override these functions from the base class that are specific to
  //	individual unifiers.
  //
  virtual bool findNextUnifier();
  virtual const Substitution& getSolution() const;
  virtual int getNrFreeVariables() const;

protected:
  //void markReachableNodes();

private:
  UnifierFilter* unifierFilter;
  Substitution* currentUnifier;
  int nrFreeVariables;
};

#endif
