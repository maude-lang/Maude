/*

    This file is part of the Maude 3 interpreter.

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
//	Initialization code for gcd based solver.
//

void
MpzSystem::swapVariables(int u, int v)
{
  const VecList::iterator e = eqns.end();
  for (VecList::iterator i = eqns.begin(); i != e; ++i)
    swap((*i)[u], (*i)[v]);
  swap(upperBounds[u], upperBounds[v]);
  swap(permutation[u], permutation[v]);
}

void
MpzSystem::integerGaussianElimination()
{
#if !defined(NO_ASSERT) && ANALYZE_GCD
  dumpEqns();
#endif
  //
  //    Diagonalize the rhs as much as possible.
  //
  int varNr = nrVariables - 1;
  const VecList::iterator e = eqns.end();
  for (VecList::iterator i = eqns.begin(); varNr >= 0 && i != e;)
    {
      const mpz_class& diag = (*i)[varNr];
      if (diag == 0)
        {
          //
          //    Permute variables to get a nonzero diagonal element.
          //
          for (int j = varNr - 1; j >= 0; --j)
            {
              if ((*i)[j] != 0)
                {
                  swapVariables(varNr, j);
                  goto eliminate;
                }
            }
          //
          //    Delete an all zero equation.
          //
          i = eqns.erase(i);
          DebugAdvisory("deleted all zero equation");
          continue;
        }
      //
      //        Eliminate variable varNr from all equations other than ith.
      //
    eliminate:
      int diagVar = nrVariables - 1;
      for (VecList::iterator j = eqns.begin(); j != e; ++j, --diagVar)
        {
          if (i != j)
            {
              mpz_class&  entry = (*j)[varNr];
              if (entry != 0)
                {
                  mpz_class g;
                  mpz_gcd(g.get_mpz_t(), diag.get_mpz_t(), entry.get_mpz_t());
                  mpz_class d_mult = entry / g;
                  mpz_class e_mult = diag / g;
                  for (int k = varNr; k >= 0; --k)
                    (*j)[k] = e_mult * (*j)[k] - d_mult * (*i)[k];
                  if (diagVar > varNr)
                    (*j)[diagVar] *= e_mult;
                  entry = 0;
                }
            }
        }
      --varNr;
      ++i;
#if !defined(NO_ASSERT) && ANALYZE_GCD
  dumpEqns();
#endif
    }
  //
  //    Make diagonal +ve.
  //
  varNr = nrVariables - 1;
  int nrEquations = eqns.size();
  nrFreeVariables = nrVariables - nrEquations;
  for (VecList::iterator i = eqns.begin(); varNr >= 0 && i != e; ++i, --varNr)
    {
      mpz_class& diag = (*i)[varNr];
      Assert(diag != 0, "zero diagonal element");
      if (diag < 0)
        {
          diag = -diag;
          for (int j = 0; j < nrFreeVariables; j++)
            (*i)[j] = -(*i)[j];  // probably a faster way to do this
        }
    }
#if !defined(NO_ASSERT) && ANALYZE_GCD
  dumpEqns();
#endif
}

mpz_class
MpzSystem::computeSumBound()
{
  //
  //	Compute a bound on the sum of components for any minimal solution.
  //	This bound is taken from:
  //	  L. Pottier,
  //	  "Minimal solutions of linear diophantine systems: bound and algorithms",
  //	  Proceedings of RTA '91, pages 162-173.
  //
  mpz_class bound = 1;
  FOR_EACH_CONST(i, VecList, eqns)
    {
      mpz_class sum = 1;
      FOR_EACH_CONST(j, IntVec, *i)
	sum += abs(*j);
      bound *= sum;
    }
  return bound;
}

void
MpzSystem::initializeGcd()
{
  Assert(!eqns.empty(), "no equations");

  initializeUpperBounds();
  sumBound = computeSumBound();
#if ANALYZE_GCD
  DebugPrintNL(sumBound);
#endif
  //
  //	Initialize permutation vector and perform Gaussian elimination.
  //
  permutation.resize(nrVariables);
  for (int i = 0; i < nrVariables; ++i)
    permutation[i] = i;
  integerGaussianElimination();
  //
  //	See if we have a better sum bound now.
  //
  mpz_class newBound = computeSumBound();
#if ANALYZE_GCD
  DebugPrintNL(newBound);
#endif
  if (newBound < sumBound)
    sumBound = newBound;
  //
  //	Generate gcd table and divide out any common factors an equation might have.
  //	Also find that last negative coefficient for each equation.
  //
  int nrEquations = nrVariables - nrFreeVariables;
  lastPrediagNeg.resize(nrEquations);
  {
    int varNr = nrVariables - 1;
    int nrGcds = nrFreeVariables - 1;
    int eqnNr = 0;
    const VecList::iterator e = eqns.end();
    for (VecList::iterator i = eqns.begin(); i != e; ++i)
      {
	gcds.push_back(IntVec());
	if (nrGcds > 0)
	  {
	    IntVec& v = gcds.back();
	    v.resize(nrGcds);
	    mpz_gcd(v[nrGcds - 1].get_mpz_t(), (*i)[nrGcds].get_mpz_t(), (*i)[varNr].get_mpz_t());
	    for (int j = nrGcds - 1; j > 0; --j)
	      mpz_gcd(v[j - 1].get_mpz_t(), (*i)[j].get_mpz_t(), v[j].get_mpz_t());
	    mpz_class final;
	    mpz_gcd(final.get_mpz_t(), (*i)[0].get_mpz_t(), v[0].get_mpz_t());
	    if (final > 1)
	      {
		DebugAdvisory("dividing equation by " << final);
		for (int j = 0; j < nrVariables; ++j)
		  (*i)[j] /= final;
		for (int j = nrGcds - 1; j >= 0; --j)
		  v[j] /= final;
	      }
#if ANALYZE_GCD
	    FOR_EACH_CONST(j, IntVec, v)
	      cout << *j << '\t';
	    cout << endl;
#endif
	  }

	for (int j = nrGcds; j >= 0; --j)
	  {
	    if ((*i)[j] < 0)
	      {
		lastPrediagNeg[eqnNr] = j;
		goto ok;
	      }
	  }
	DebugAdvisory("all positive equation " << eqnNr);
	lastPrediagNeg[eqnNr] = NONE;
      ok:
	--varNr;
	++eqnNr;
      }
  }

  {
    //
    //	Determine which variables before the diagonal have limited choices.
    //
    //	A prediagonal variable is prunable if
    //	(1) All equations that have a nonzero coefficient for it
    //	  (a) have -ve coefficient for it; and
    //	  (b) have it as their last prediagonal variable with a nonzero coefficient
    //	(2) All following prediagonal variable are prunable
    //
    //	Intuitively we don't need to seach for larger values for a prunable variable
    //	since any further solutions found will be non-minimal. However we need to
    //	avoid the case where the first solution yields the trivial (all zero) solution
    //	to the system.
    //
    firstPrunablePrediag = nrVariables;  // assume no prunable prediagonal variables
    NatSet used;
    for (int i = nrFreeVariables - 1; i >= 0; i--)
      {
	int eqnNr = 0;
	FOR_EACH_CONST(j, VecList, eqns)
	  {
	    if ((*j)[i] > 0)
	      goto done;
	    if ((*j)[i] < 0)
	      {
		if (used.contains(eqnNr))
		  goto done;
		used.insert(eqnNr);
	      }
	    ++eqnNr;
	  }
	firstPrunablePrediag = i;
      }
  done:
    ;
#if ANALYZE_GCD
    DebugPrintNL(firstPrunablePrediag);
#endif
  }
  //
  //	Initialize residue storage.
  //
  {
    residues.resize(nrEquations);
    const IntVec::iterator e = residues.end();
    for (IntVec::iterator i = residues.begin(); i != e; ++i)
      *i = 0;
  }
  //
  //	Initialize solution storage
  //
  {
    solution.resize(nrVariables);
    const IntVec::iterator e = solution.end();
    for (IntVec::iterator i = solution.begin(); i != e; ++i)
      *i = 0;
  }
  //
  //	The freeVariables constitute the search space and need stack entries.
  //
  stack.resize(nrFreeVariables);
  //
  //	Termination is assured by a bound on the sum of a minimal solutions components.
  //
  leftOver = sumBound;
}
