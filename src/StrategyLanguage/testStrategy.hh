/*

    This file is part of the Maude 3 interpreter.

    Copyright 1997-2006 SRI International, Menlo Park, CA 94025, USA.

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
//      Class for match/xmatch/amatch strategies.
//
#ifndef _testStrategy_hh_
#define _testStrategy_hh_
#include "strategyExpression.hh"
#include "pattern.hh"

class TestStrategy : public StrategyExpression
{
public:
  //
  //	depth = -1		means at top, no extension
  //	depth = 0		means at top, with extension
  //	depth = UNBOUNDED	means all the way down to the leaf nodes, with extension
  //
  TestStrategy(Term* patternTerm, int depth, const Vector<ConditionFragment*>& condition);

  Term* getPatternTerm() const;
  const Pattern& getPattern() const;
  int getDepth() const;
  const Vector<ConditionFragment*>& getCondition() const;
  //
  // Gets the index translation from the outer context to the matching
  // pattern.
  //
  // The first entry of each pair is the outer context index and
  // the second is the variable index in the pattern.
  //
  // This translation is calculated by check.
  //
  const Vector<std::pair<int, int> >& getIndexTranslation() const;

  bool check(VariableInfo& indices, const TermSet& boundVars);
  void process();

  StrategicExecution::Survival decompose(StrategicSearch& searchObject, DecompositionProcess* remainder);

private:
  Pattern pattern;
  const int depth;

  Vector<std::pair<int, int> > indexTranslation;
};

inline Term*
TestStrategy::getPatternTerm() const
{
  return pattern.getLhs();
}

inline int
TestStrategy::getDepth() const
{
  return depth;
}

inline const Vector<ConditionFragment*>&
TestStrategy::getCondition() const
{
  return pattern.getCondition();
}

inline const Pattern&
TestStrategy::getPattern() const
{
  return pattern;
}

inline const Vector<std::pair<int, int> >&
TestStrategy::getIndexTranslation() const
{
  return indexTranslation;
}

#endif
