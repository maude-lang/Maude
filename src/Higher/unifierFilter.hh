/*

    This file is part of the Maude 3 interpreter.

    Copyright 2016 SRI International, Menlo Park, CA 94025, USA.

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
//	Class for filtering unifiers, keeping a minimal set that are most general on a range of variables:
//	  firstInterestingVariable,..., firstInterestingVariable + nrInterestingVariables -1
//
#ifndef _unifierFilter_hh_
#define _unifierFilter_hh_
#include <list>
#include "simpleRootContainer.hh"

class UnifierFilter : private SimpleRootContainer
{
  NO_COPYING(UnifierFilter);

public:
  UnifierFilter(int firstInterestingVariable, int nrInterestingVariables);
  ~UnifierFilter();

  void insertUnifier(const Substitution& unifier, int positionIndex, int equationIndex);
  bool getNextSurvivingUnifier(Substitution*& unifier, int& positionIndex, int& equationIndex);

private:
  struct RetainedUnifier
  {
    RetainedUnifier(const Substitution& original,
		    int firstInterestingVariable,
		    int nrInterestingVariables);
    ~RetainedUnifier();

    Substitution unifier;
    Vector<Term*> interestingBindings;
    Vector<LhsAutomaton*> matchingAutomata;
    int nrVariablesInBindings;
    int positionIndex;
    int equationIndex;
  };

  typedef list<RetainedUnifier*> RetainedUnifierList;

  void markReachableNodes();
  bool subsumes(const RetainedUnifier* retainedUnifier, const Substitution& unifier);


  const int firstInterestingVariable;
  const int nrInterestingVariables;
  RetainedUnifierList mostGeneralSoFar;   // on interestingVariables;

  bool startedExtractingUnifiers;
  RetainedUnifierList::const_iterator nextUnifier;
};


#endif
