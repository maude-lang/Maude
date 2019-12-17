/*

    This file is part of the Maude 3 interpreter.

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
//	Class for accumulating disjunctions of subproblems
//
#ifndef _disjunctiveSubproblemAccumulator_hh_
#define _disjunctiveSubproblemAccumulator_hh_

class DisjunctiveSubproblemAccumulator
{
public:
  DisjunctiveSubproblemAccumulator(Substitution& global);
  ~DisjunctiveSubproblemAccumulator();

  bool empty() const;
  void addOption(Substitution& local,
		 Subproblem* subproblem,
		 ExtensionInfo* extensionInfo);
  bool extract(Substitution& solution,
	       Subproblem*&returnedSubproblem,
	       ExtensionInfo* extensionInfo);

private:
  bool first;
  Substitution& global;
  LocalBinding* firstDifference;
  Subproblem* firstSubproblem;
  ExtensionInfo* firstExtensionInfo;
  SubproblemDisjunction* disjunction;
};

inline
DisjunctiveSubproblemAccumulator::DisjunctiveSubproblemAccumulator(Substitution& global)
  : global(global)
{
  first = true;
}

inline bool
DisjunctiveSubproblemAccumulator::empty() const
{
  return first;
}

#endif
