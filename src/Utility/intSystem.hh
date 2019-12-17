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
//	Class to solve Diophantine systems associated with AC and ACU unification, using
//	machine integers. This version is optimized for the small multiplicities found in
//	typical unification problems and only implements the Contejean-Devie algorithm.
//
#include <list>
#include "natSet.hh"

#ifndef _intSystem_hh_
#define _intSystem_hh_

class IntSystem
{
public:  
  IntSystem(int nrVariables);

  typedef Vector<int> IntVec;

  void insertEqn(const IntVec& eqn);
  void setUpperBounds(const IntVec& bounds);  // pass UNBOUNDED to bound by INT_MAX
  bool findNextMinimalSolution(IntVec& solution);

private:
  //
  //	General data types, functions and data.
  //
  typedef list<IntVec> VecList;

  static bool greaterEqual(const IntVec& arg1, const IntVec& arg2);
  bool minimal(const IntVec& arg);
  void initializeUpperBounds();
#ifndef NO_ASSERT
  void dumpEqns();
#endif

  const int nrVariables;
  VecList eqns;			// Diophantine system
  IntVec upperBounds;		// upper bounds for each variable
  VecList solutions;		// minimal solutions found so far
  //
  //	Contejean-Devie specific stuff.
  //
  struct State
  {
    IntVec assignment;	// current assignment to each variable
    IntVec residue;	// residue for each equation
    NatSet frozen;	// indicies of variables that can no longer be incremented
  };

  typedef Vector<State> StateStack;

  static bool isZero(const IntVec& arg);
  void initialize();
  int scalerProduct(const IntVec& arg, int columnNr);

  StateStack states;		// stack of search tree states
  int stackPointer;
  State current;		// pre-allocated temporary storage
};

#endif
