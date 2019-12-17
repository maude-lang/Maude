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
//	Solver based on repeated use of the extended gcd calculation, with
//	an integer gaussian elimination preprocessing step.
//

void
MpzSystem::updateResidues(int varNr, const mpz_class& delta)
{
  int eqnNr = 0;
  FOR_EACH_CONST(i, VecList, eqns)
    {
      residues[eqnNr] += (*i)[varNr] * delta;
      ++eqnNr;
    }
}

bool
MpzSystem::findNextMinimalSolutionGcd(IntVec& sol)
{
  bool first = false;
  if (permutation.empty())
    {
      initializeGcd();
      first = true;
    }
  if (nextSolution(first))
    {
      //
      //	Need to permute solution into original variable order.
      //
      solution.resize(nrVariables);
      for (int i = 0; i < nrVariables; ++i)
	sol[permutation[i]] = solution[i];
      return true;
    }
  return false;
}

//	Iterative GCD method.

bool
MpzSystem::nextSolution(bool first)
{
  int sp;

  if (first)
    {
      sp = 0;
    forwards:
      for (; sp < nrFreeVariables; ++sp)
	{
	  if (!((sp == nrFreeVariables - 1) ? fillOutLastEntry() : fillOutStackEntry(sp)))
	    goto backwards;
	}
      if (solveDiagonal())
	return true;
      goto backwards;
    }
  else
    {
      sp = nrFreeVariables;
    backwards:
      while (--sp >= 0)
	{
	  StackEntry& st = stack[sp];
	  if (solution[sp] < st.bound)
	    {
	      //
	      //	Update leftover, residues and solution.
	      //
	      leftOver -= st.inc;
	      updateResidues(sp, st.inc);
	      solution[sp] += st.inc;
	      ++sp;
	      goto forwards;
	    }
	  leftOver += solution[sp];
	  updateResidues(sp, - solution[sp]);
	  solution[sp]= 0;
	}
    }
  return false;
}

bool
MpzSystem::fillOutStackEntry(int varNr)
{
  TRACE(" solving x" << varNr);
  //
  //	Default values for if we encounter an all zero coefficient variable.
  //
  mpz_class base = 0;
  mpz_class inc = 1;
  mpz_class bound = 1;
  mpz_class u = upperBounds[varNr];
  if (u < 0 || leftOver < u)
    u = leftOver;
  //
  //	Find a concensus solution for variable.
  //
  bool first = true;
  int row = 0;
  VecList::const_iterator j = gcds.begin();
  FOR_EACH_CONST(i, VecList, eqns)
    {
      const mpz_class& a = (*i)[varNr];
      if (a != 0)
	{
	  const mpz_class& residue = residues[row];
	  const mpz_class& g = (*j)[varNr];
	  mpz_class x_base;
	  mpz_class y_base;
	  mpz_class x_inc;
	  mpz_class y_inc;
	  mpz_class k_bound;
	  Assert(g != 0, "zero gcd");
	  //
	  //	The idea is that if the current variable is X, 
	  //	  aX + (...) = -residue
	  //	but we know that all the coefficient in (...) are divisable
	  //	by g so we abstract them as gY and solve a two variable problem.
	  //	If a is the last -ve coefficient in our equation we know that Y
	  //	will be non-negative since it abtracts a sum of products where
	  //	everything (variables and divided coefficients) is non-negative.
	  //	
	  if (!solveTwoVariableProblem(a, g, -residue,
				       varNr == lastPrediagNeg[row], u, NONE,
				       x_base, y_base, x_inc, y_inc, k_bound))
	    {
	      TRACE("no solution");
	      return false;
	    }
	  TRACE("x" << varNr << " = " << x_base << " + " << x_inc << "k for k = 0,..., " << k_bound);

	  if (first)
	    {
	      base = x_base;
	      inc = x_inc;
	      bound = k_bound;
	      first = false;
	    }
	  else
	    {
	      //
	      //	If we already have a set of values to search for variable
	      //	we must compute a consensus (intersection).
	      //
	      mpz_class c_base;
	      mpz_class c_inc;
	      mpz_class c_bound;
	      if (findConcensus(base, inc, bound,
				x_base, x_inc, k_bound,
				c_base, c_inc, c_bound))
		{
		  base = c_base;
		  inc = c_inc;
		  bound = c_bound;
		  TRACE("concensus: x" << varNr << " = " << c_base << " + " << c_inc << "k for k = 0,..., " << c_bound);
		}
	      else
		{
		  TRACE("no concensus");
		  return false;
		}
	    }
	}
      else
	{
	  //
	  //	Since coefficient is zero in this equation it provides us with no
	  //	constraint on variable.
	  //
	  TRACE("no info");
	}
      ++j;
      ++row;
    }
  //
  //	Correct bound for zero inc, prunable variable and zero so far situations.
  //
  //	If the increment is 0, we set the bound to 0.
  //	If our variable is prunable, we can also set the bound to 0,
  //	since any increments will produce a nonminimal solution; except if
  //	all the variables in our current assignment are zero and the base is also
  //	zero, in which case we set the bound to 1.
  //
  StackEntry& st = stack[varNr];
  if (inc == 0)
    bound = 0;
  else
    {
      if (varNr >= firstPrunablePrediag)
	{
	  bound = 0;
	  if (leftOver == sumBound && base == 0)
	    bound = 1;
	}
    }
  TRACE("corrected: x" << varNr << " = " << base << " + " << inc << "k for k = 0,..., " << bound);
  //
  //	Set up stack entry and give variable it's first value;
  //	update leftOver and residues.
  //
  st.inc = inc;
  st.bound = base + bound * inc;
  if (base != 0)
    {
      leftOver -= base;
      updateResidues(varNr, base);
      solution[varNr] = base;
    }
  return true;
}

bool
MpzSystem::fillOutLastEntry()
{
  TRACE("solving prediagonal");
  //
  //	We solve each equation for the last prediagonal variable
  //	and its diagonal variable and try to find a consensus on the
  //	last prediagonal variable.
  //
  int prediag = nrFreeVariables - 1;
  //
  //	Default values for if we encounter an all zero coefficient variable.
  //
  mpz_class base = 0;
  mpz_class inc = 1;
  mpz_class bound = 1;
  mpz_class u = upperBounds[prediag];
  if (u < 0 || leftOver < u)
    u = leftOver;
  //
  //	Find a concensus solution for last prediagonal variable.
  //
  bool first = true;
  int row = 0;
  FOR_EACH_CONST(i, VecList, eqns)
    {
      const mpz_class& residue = residues[row];
      ++row;
      const mpz_class& a = (*i)[prediag];
      int diag = nrVariables - row;
      const mpz_class& b = (*i)[diag];
      if (a != 0)
	{
	  mpz_class x_base;
	  mpz_class y_base;
	  mpz_class x_inc;
	  mpz_class y_inc;
	  mpz_class k_bound;
	  Assert(b != 0, "zero diag");
	  mpz_class u2 = upperBounds[diag];
	  if (u2 < 0 || leftOver < u2)
	    u2 = leftOver;
	  if (!solveTwoVariableProblem(a, b, -residue,
				       true, u, u2,
				       x_base, y_base, x_inc, y_inc, k_bound))
	    {
	      TRACE("no solution");
	      return false;
	    }
	  TRACE("x = " << x_base << " + " << x_inc << "k for k = 0,..., " << k_bound);

	  if (first)
	    {
	      base = x_base;
	      inc = x_inc;
	      bound = k_bound;
	      first = false;
	    }
	  else
	    {
	      mpz_class c_base;
	      mpz_class c_inc;
	      mpz_class c_bound;
	      if (findConcensus(base, inc, bound,
				x_base, x_inc, k_bound,
				c_base, c_inc, c_bound))
		{
		  base = c_base;
		  inc = c_inc;
		  bound = c_bound;
		  TRACE("concensus: x = " << c_base << " + " << c_inc << "k for k = 0,..., " << c_bound);
		}
	      else
		{
		  TRACE("no concensus");
		  return false;
		}
	    }
	}
      else
	{
	  //
	  //	Since coefficient is zero in this equation it provides us with no
	  //	constraint on variable.
	  //
	  TRACE("no info");
	}
    }
  //
  //	Correct start bound for zero inc, prunable variable and zero so far situations.
  //
  StackEntry& st = stack[prediag];
  mpz_class start = 0;
  if (leftOver == sumBound && base == 0)
    {
      //	Current variables are all 0, as is our base. Thus if we set start
      //	to 0, solving the diagonal would lead to the trivial solution.
      //
      if (bound == 0)
	return false;  
      start = 1;
    }

  if (prediag >= firstPrunablePrediag)
    bound = start;  // only first solution can be minimal
  //
  //	Set up stack entry and give variable it's first value;
  //	update leftOver, residues
  //
  st.inc = inc;
  st.bound = base + bound * inc;
  solution[prediag] = base + start * inc;

  if (solution[prediag] != 0)
    {
      leftOver -= solution[prediag];
      updateResidues(prediag, solution[prediag]);
    }
  return true;
}

bool
MpzSystem::solveDiagonal()
{
  //
  //	For each variable on the diagonal we compute its value by
  //	dividing the (negated) residue of the one equation in which
  //	it has a nonzero coefficient by that coefficient.
  //	We will always get a valid solution however it can fail to
  //	be minimal; it can also fail to be nontrivial
  //
  bool allZero = true;
  {
    int eqnNr = 0;
    int varNr = nrVariables;
    FOR_EACH_CONST(i, VecList, eqns)
      {
	--varNr;
	mpz_class q;
	mpz_class r;
	mpz_class res = - residues[eqnNr];
	mpz_fdiv_qr(q.get_mpz_t(), r.get_mpz_t(), res.get_mpz_t(), (*i)[varNr].get_mpz_t());
	Assert(r == 0, "divisability error " << res << ' ' << (*i)[varNr]);
	Assert(upperBounds[varNr] < 0 || q <= upperBounds[varNr],
	       "bounds error" << upperBounds[varNr] << ' ' << q);
	Assert(q >= 0, "sign error " << q);
	if (q != 0)
	  allZero = false;
	solution[varNr] = q;
	++eqnNr;
      }
  }
  if (allZero && leftOver == sumBound)
    return false;  // trivial solution
  if (minimal(solution))
    {
#if ANALYZE_GCD
      cout << "*** Solution " << solutions.size() + 1 << " ***\n";
      FOR_EACH_CONST(j, IntVec, solution)
	cout << *j << '\t';
      cout << endl;
#endif
      solutions.push_back(solution);

      if (allZero)
	{
	  //
	  //	Diagonal variables were all zero; hence we can terminate
	  //	some of the search ranges because they cannnot produce a
	  //	solution not subsumed by current one. This optimization
	  //	is critical for pruning after finding a solution like:
	  //	(1 0 0 0 ... 0)
	  //
	  for (int i = nrFreeVariables - 1; i >= 0; --i)
	    {
	      stack[i].bound = solution[i];
	      if (solution[i] != 0)
		break;
	    }
	}
      return true;
    }
  return false;
}
