/*

    This file is part of the Maude 2 interpreter.

    Copyright 1997-2005 SRI International, Menlo Park, CA 94025, USA.

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
//	multiple precision integers.
//
#include <list>
#include "gmpxx.h"
#include "natSet.hh"

#ifndef _mpzSystem_hh_
#define _mpzSystem_hh_

class MpzSystem
{
public:  
  typedef Vector<mpz_class> IntVec;

  void insertEqn(const IntVec& eqn);
  void setUpperBounds(const IntVec& bounds);
  bool findNextMinimalSolution(IntVec& solution);
  bool findNextMinimalSolutionGcd(IntVec& solution);

private:
  //
  //	Common data types functions and data.
  //
  typedef list<IntVec> VecList;

  static bool greaterEqual(const IntVec& arg1, const IntVec& arg2);
  bool minimal(const IntVec& arg);
  void initializeUpperBounds();
#ifndef NO_ASSERT
  void dumpEqns();
#endif

  int nrVariables;
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
  mpz_class scalerProduct(const IntVec& arg, int columnNr);

  StateStack states;		// stack of search tree states
  int stackPointer;
  State current;		// pre-allocated temporary storage
  //
  //	Gcd based solver specific stuff.
  //
  struct StackEntry
  {
    mpz_class inc;
    mpz_class bound;
  };

  static bool findConcensus(const mpz_class& a,
			    const mpz_class& b,
			    const mpz_class& u,
			    const mpz_class& c,
			    const mpz_class& d,
			    const mpz_class& v,
			    mpz_class& e,
			    mpz_class& f,
			    mpz_class& w);
  static bool solveTwoVariableProblem(mpz_class a,
				      mpz_class b,
				      mpz_class c,
				      bool y_nonneg,
				      const mpz_class& x_bound,
				      const mpz_class& y_bound,
				      mpz_class& x_base,
				      mpz_class& y_base,
				      mpz_class& x_inc,
				      mpz_class& y_inc,
				      mpz_class& bound);

  void swapVariables(int u, int v);
  void integerGaussianElimination();
  void initializeGcd();
  mpz_class computeSumBound();

  void updateResidues(int varNr, const mpz_class& delta);
  bool nextSolution(bool first);
  bool fillOutStackEntry(int varNr);
  bool fillOutLastEntry();
  bool solveDiagonal();

  int nrFreeVariables;			// free variables are those not on the diagonal
  Vector<StackEntry> stack;		// stack for backtracking
  IntVec solution;			// current partial solution
  IntVec residues;			// residue for each equation with the current partial solution
  mpz_class sumBound;			// bound on sum of components for a minimal solution
  mpz_class leftOver;			// part of sumBound not used by current partial solution
  VecList gcds;
  Vector<int> permutation;		// for full pivoting
  Vector<int> lastPrediagNeg;		// index last prediagonal -ve entry in each equation
  int firstPrunablePrediag;		// prediagonal greater or equal to this can take one of at most 2 values
};

#endif
