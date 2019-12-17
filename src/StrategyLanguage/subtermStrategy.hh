/*

    This file is part of the Maude 3 interpreter.

    Copyright 2017 SRI International, Menlo Park, CA 94025, USA.

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
//      Class of the strategy for rewriting of subterms.
//
#ifndef _subtermStrategy_hh_
#define _subtermStrategy_hh_
#include "strategyExpression.hh"
#include "rhsBuilder.hh"
#include "pattern.hh"

class SubtermStrategy : public StrategyExpression
{
public:
  //
  // Creates a strategy object for rewriting of subterms.
  //
  // matchrew P such that C by P1 using S1, ..., Pn using Sn.
  //
  // @param patternTerm The main pattern term (P).
  // @param depth Whether to match at top without extension (-1), with it (0) or all
  //	the way down to the leaf nodes (UNBOUNDED).
  // @param condition The condition (C).
  // @param patterns Subpattern terms (P1, ..., Pn)
  // @param strategies Strategies to rewrite the matched subterms (S1, ..., Sn)
  //
  SubtermStrategy(Term* patternTerm,
		  int depth,
		  const Vector<ConditionFragment*>& condition,
		  const Vector<Term*>& patterns,
		  const Vector<StrategyExpression*>& strategies);
  ~SubtermStrategy();

  int getDepth() const;
  const Pattern& getPattern() const;
  Term* getPatternTerm() const;
  const Vector<ConditionFragment*>& getCondition() const;
  const Vector<Term*>& getSubterms() const;
  const Vector<StrategyExpression*>& getStrategies() const;

  //
  // Gets the context specification of the matchrew substrategies.
  //
  // The vector includes as many entries as variables in the context of the
  // substrategies S1, ..., Sn. Each entry is an index identifying the source
  // of the value the variable must take. If it is positive, the origin is the
  // matching variable with such an index. If negative, its value comes from
  // the outer context variable at index -(1 + i).
  //
  // This context specification is calculated by check.
  //
  const Vector<int>& getContextSpec() const;
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

  //
  // Rebuild the subject node with modified subterms.
  //
  // @param matchSubst Values of the matching for the main pattern according to its
  // variable information. It may contain additional entries.
  // @param results DAG nodes for the subpatterns after rewriting.
  //
  DagNode* rebuild(const Vector<DagNode*>& matchSubst, const Vector<DagNode*>& results) const;

  StrategicExecution::Survival decompose(StrategicSearch& searchObject, DecompositionProcess* remainder);

private:
  bool checkSubterms();

  Pattern mainPattern;				// P
  Vector<Term*> subpatterns;			// P1, ..., Pn
  RhsBuilder rhsBuilder;			// Term builder for the results
  Vector<StrategyExpression*> subtermStrats;    // S1, ..., Sn
  const int depth;				// Search depth
  Vector<int> variableMapping;			// Variable mapping for rebuild
  Vector<int> contextSpec;			// Variable mapping for creating a subcontext
  Vector<std::pair<int, int> > indexTranslation; // Variable mapping from the context to the pattern
};

inline Term*
SubtermStrategy::getPatternTerm() const
{
  return mainPattern.getLhs();
}

inline int
SubtermStrategy::getDepth() const
{
  return depth;
}

inline const Vector<ConditionFragment*>&
SubtermStrategy::getCondition() const
{
  return mainPattern.getCondition();
}

inline const Pattern&
SubtermStrategy::getPattern() const
{
  return mainPattern;
}

inline const Vector<Term*>&
SubtermStrategy::getSubterms() const
{
  return subpatterns;
}

inline const Vector<StrategyExpression*>&
SubtermStrategy::getStrategies() const
{
  return subtermStrats;
}

inline const Vector<int>&
SubtermStrategy::getContextSpec() const
{
  return contextSpec;
}

inline const Vector<std::pair<int, int> >&
SubtermStrategy::getIndexTranslation() const
{
  return indexTranslation;
}

#endif
