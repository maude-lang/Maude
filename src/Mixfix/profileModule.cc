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
//      Implementation for class ProfileModule.
//

//	utility stuff
#include "macros.hh"
#include "vector.hh"

//      forward declarations
#include "interface.hh"
#include "core.hh"
#include "strategyLanguage.hh"
#include "mixfix.hh"

//      interface class definitions
#include "term.hh"

//      core class definitions
#include "sortConstraint.hh"
#include "equation.hh"
#include "rule.hh"
#include "rewritingContext.hh"

//      front end class definitions
#include "profileModule.hh"

ProfileModule::ProfileModule(int name)
  : Module(name)
{
}

ProfileModule::SymbolProfile::SymbolProfile()
{
  nrBuiltinMbRewrites = 0;
  nrBuiltinEqRewrites = 0;
  nrBuiltinRlRewrites = 0;
  nrMemoRewrites = 0;
}

ProfileModule::FragmentProfile::FragmentProfile()
{
  nrSuccesses = 0;
  nrFailures = 0;
}

ProfileModule::StatementProfile::StatementProfile()
{
  nrRewrites = 0;
  nrConditionStarts = 0;
}

void
ProfileModule::StatementProfile::updateFragmentInfo(int index, bool success)
{
  if (index >= fragmentInfo.length())
    fragmentInfo.resize(index + 1);
  if (success)
    ++(fragmentInfo[index].nrSuccesses);
  else
    ++(fragmentInfo[index].nrFailures);
}

void
ProfileModule::clearProfile()
{
  symbolInfo.clear();
  mbInfo.clear();
  eqInfo.clear();
  rlInfo.clear();
}

void
ProfileModule::profileMbRewrite(DagNode* redex, const SortConstraint* mb)
{
  if (mb == 0)
    {
      int index = redex->symbol()->getIndexWithinModule();
      if (index >= symbolInfo.length())
	symbolInfo.resize(index + 1);
      ++(symbolInfo[index].nrBuiltinMbRewrites);
    }
  else
    {
      int index = mb->getIndexWithinModule();
      if (index >= mbInfo.length())
	mbInfo.resize(index + 1);
      ++(mbInfo[index].nrRewrites);
    }
}

void
ProfileModule::profileEqRewrite(DagNode* redex, const Equation* eq, int type)
{
  if (eq == 0)
    {
      int index = redex->symbol()->getIndexWithinModule();
      if (index >= symbolInfo.length())
	symbolInfo.resize(index + 1);
      switch (type)
	{
	case RewritingContext::BUILTIN:
	  {
	    ++(symbolInfo[index].nrBuiltinEqRewrites);
	    break;
	  }
	case RewritingContext::MEMOIZED:
	  {
	    ++(symbolInfo[index].nrMemoRewrites);
	    break;
	  }
	}
    }
  else
    {
      int index = eq->getIndexWithinModule();
      if (index >= eqInfo.length())
	eqInfo.resize(index + 1);
      ++(eqInfo[index].nrRewrites);
    }
}

void
ProfileModule::profileRlRewrite(DagNode* redex, const Rule* rl)
{
  if (rl == 0)
    {
      int index = redex->symbol()->getIndexWithinModule();
      if (index >= symbolInfo.length())
	symbolInfo.resize(index + 1);
      ++(symbolInfo[index].nrBuiltinRlRewrites);
    }
  else
    {
      int index = rl->getIndexWithinModule();
      if (index >= rlInfo.length())
	rlInfo.resize(index + 1);
      ++(rlInfo[index].nrRewrites);
    }
}

//////////////////////

void
ProfileModule::profileMbConditionStart(const SortConstraint* mb)
{
  int index = mb->getIndexWithinModule();
  if (index >= mbInfo.length())
    mbInfo.resize(index + 1);
  ++(mbInfo[index].nrConditionStarts);
}

void
ProfileModule::profileEqConditionStart(const Equation* eq)
{
  int index = eq->getIndexWithinModule();
  if (index >= eqInfo.length())
    eqInfo.resize(index + 1);
  ++(eqInfo[index].nrConditionStarts);
}

void
ProfileModule::profileRlConditionStart(const Rule* rl)
{
  int index = rl->getIndexWithinModule();
  if (index >= rlInfo.length())
    rlInfo.resize(index + 1);
  ++(rlInfo[index].nrConditionStarts);
}

////////////////////////

void
ProfileModule::profileFragment(const PreEquation* preEquation,
			       int fragmentIndex,
			       bool success)
{
  Assert(preEquation->getModule() == this, "module mismatch " <<
	 this << " vs " << preEquation->getModule());
  int index = preEquation->getIndexWithinModule();
  Assert(index != NONE, "index = NONE");

  {
    const Vector<SortConstraint*>& mbs= getSortConstraints();
    if (index < mbs.length() && mbs[index] == preEquation)
      {
	mbInfo[index].updateFragmentInfo(fragmentIndex, success);
	return;
      }
  }
  {
    const Vector<Equation*>& eqs = getEquations();
    if (index < eqs.length() && eqs[index] == preEquation)
      {
	eqInfo[index].updateFragmentInfo(fragmentIndex, success);
	return;
      }
  }
  {
    const Vector<Rule*>& rls = getRules();
    if (index < rls.length() && rls[index] == preEquation)
      {
	rlInfo[index].updateFragmentInfo(fragmentIndex, success);
	return;
      }
  }
  // must be a top level pattern fragment
}

#define PC(n)	(n) << " (" << ((100 * n) / floatTotal) << "%)"

void
ProfileModule::showProfile(ostream& s) const
{
  double floatTotal;
  {
    //
    //	First compute a grand total.
    //
    Int64 total = 0;
    int nrSymbols = symbolInfo.length();
    for (int i = 0; i < nrSymbols; i++)
      {
	const SymbolProfile& p = symbolInfo[i];
	total += p.nrBuiltinMbRewrites;
	total += p.nrBuiltinEqRewrites;
	total += p.nrBuiltinRlRewrites;
	total += p.nrMemoRewrites;
      }
    int nrMbs = mbInfo.length();
    for (int i = 0; i < nrMbs; i++)
      total += mbInfo[i].nrRewrites;
    int nrEqs = eqInfo.length();
    for (int i = 0; i < nrEqs; i++)
      total += eqInfo[i].nrRewrites;
    int nrRls = rlInfo.length();
    for (int i = 0; i < nrRls; i++)
      total += rlInfo[i].nrRewrites;
    floatTotal = total;
  }
  {
    const Vector<Symbol*>& symbols = getSymbols();
    int nrSymbols = symbolInfo.length();
    for (int i = 0; i < nrSymbols; i++)
      {
	const SymbolProfile& p = symbolInfo[i];
	if (p.nrBuiltinMbRewrites + p.nrBuiltinEqRewrites +
	    p.nrBuiltinRlRewrites + p.nrMemoRewrites > 0)
	  {
	    showSymbol(s, symbols[i]);
	    const char* g = "";
	    if (p.nrBuiltinMbRewrites > 0)
	      {
		s << "built-in mb rewrites: " << PC(p.nrBuiltinMbRewrites);
		g = "\t";
	      }
	    if (p.nrBuiltinEqRewrites > 0)
	      {
		s << g << "built-in eq rewrites: " << PC(p.nrBuiltinEqRewrites);
		g = "\t";
	      }
	    if (p.nrBuiltinRlRewrites > 0)
	      {
		s << g << "built-in rl rewrites: " << PC(p.nrBuiltinRlRewrites);
		g = "\t";
	      }
	    if (p.nrMemoRewrites > 0)
	      s << g << "memo rewrites: " << PC(p.nrMemoRewrites);
	    s << "\n\n";
	  }
      }
  }
  {
    const Vector<SortConstraint*>& mbs = getSortConstraints();
    int nrMbs = mbInfo.length();
    for (int i = 0; i < nrMbs; i++)
      {
	const StatementProfile& p = mbInfo[i];
	if (p.nrConditionStarts > 0)
	  {
	    s << mbs[i] << '\n';
	    s << "lhs matches: " << p.nrConditionStarts <<
	      "\trewrites: " << PC(p.nrRewrites) << '\n';
	    showFragmentProfile(s, p.fragmentInfo, p.nrConditionStarts);
	    s << '\n';
	  }
	else if (p.nrRewrites > 0)
	  {
	    s << mbs[i] << '\n';
	    s << "rewrites: " << PC(p.nrRewrites) << "\n\n";
	  }
      }
  }
  {
    const Vector<Equation*>& eqs = getEquations();
    int nrEqs = eqInfo.length();
    for (int i = 0; i < nrEqs; i++)
      {
	const StatementProfile& p = eqInfo[i];
	if (p.nrConditionStarts > 0)
	  {
	    s << eqs[i] << '\n';
	    s << "lhs matches: " << p.nrConditionStarts <<
	      "\trewrites: " << PC(p.nrRewrites) << '\n';
	    showFragmentProfile(s, p.fragmentInfo, p.nrConditionStarts);
	    s << '\n';
	  }
	else if (p.nrRewrites > 0)
	  {
	    s << eqs[i] << '\n';
	    s << "rewrites: " << PC(p.nrRewrites) << "\n\n";
	  }
      }
  }
  {
    const Vector<Rule*>& rls = getRules();
    int nrRls = rlInfo.length();
    for (int i = 0; i < nrRls; i++)
      {
	const StatementProfile& p = rlInfo[i];
	if (p.nrConditionStarts > 0)
	  {
	    s << rls[i] << '\n';
	    s << "lhs matches: " << p.nrConditionStarts <<
	      "\trewrites: " << PC(p.nrRewrites) << '\n';
	    showFragmentProfile(s, p.fragmentInfo, p.nrConditionStarts);
	    s << '\n';
	  }
	else if (p.nrRewrites > 0)
	  {
	    s << rls[i] << '\n';
	    s << "rewrites: " << PC(p.nrRewrites) << "\n\n";
	  }
      }
  }
}

void
ProfileModule::showSymbol(ostream& s, Symbol* op)
{
  s << "op " << op << " : ";
  int nrArgs = op->arity();
  for (int i = 0; i < nrArgs; i++)
    s << op->domainComponent(i)->sort(Sort::KIND) << ' ';
  s << "-> " << op->rangeComponent()->sort(Sort::KIND) << " .\n";
}

void
ProfileModule::showFragmentProfile(ostream& s,
				   const Vector<FragmentProfile>& fragmentInfo,
				   Int64 nrFirsts)
{
  int nrFragments = fragmentInfo.length();
  s << "Fragment\tInitial tries\tResolve tries\tSuccesses\tFailures\n";
  for (int i = 0; i < nrFragments; i++)
    {
      Int64 nrSuccesses = fragmentInfo[i].nrSuccesses;
      Int64 nrFailures = fragmentInfo[i].nrFailures;
      Int64 nrAttempts = nrSuccesses + nrFailures;
      Int64 nrBacktracks = nrAttempts - nrFirsts;
      s << i + 1 << "\t\t" << nrFirsts << "\t\t" << nrBacktracks <<
	"\t\t" << nrSuccesses << "\t\t" << nrFailures << '\n';
      nrFirsts = nrSuccesses;  // for next fragment
    }
}
