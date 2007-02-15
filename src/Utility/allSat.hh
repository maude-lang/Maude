/*

    This file is part of the Maude 2 interpreter.

    Copyright 2007 SRI International, Menlo Park, CA 94025, USA.

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
//	Class for finding all satisfying assigns over a set of variables
//	{firstVariable,..., lastVariable} for some formula given by a BDD.
//
#ifndef _allSat_hh_
#define _allSat_hh_
#include "bdd.hh"

class AllSat
{
public:
  AllSat(Bdd formula, int firstVariable, int lastVariable);

  bool nextAssignment();
  const Vector<Byte>& getCurrentAssignment() const;

private:
  void forward(Bdd b);

  const Bdd formula;
  const int firstVariable;
  const int lastVariable;

  Vector<Bdd> nodeStack;	// current path to true through BDD
  Vector<int> dontCareSet;	// set of variables of interest not mentioned on current path
  Vector<Byte> assignment;	// current assignment
};

inline const Vector<Byte>&
AllSat::getCurrentAssignment() const
{
  return assignment;
}

#endif
