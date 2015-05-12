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
//	Class for rewrite rules
//
#ifndef _rule_hh_
#define _rule_hh_
#include "preEquation.hh"
#include "rhsBuilder.hh"

class Rule : public PreEquation
{
public:
  Rule(int label,
       Term* lhs,
       Term* rhs,
       const Vector<ConditionFragment*>& condition = noCondition);
  ~Rule();

  Term* getRhs() const;
  void check();
  void preprocess();
  void compile(bool compileLhs);
  const RhsBuilder& getRhsBuilder() const;
  LhsAutomaton* getNonExtLhsAutomaton();
  LhsAutomaton* getExtLhsAutomaton();
  void print(ostream& s) const;

private:
  int traceBeginTrial(DagNode* subject, RewritingContext& context) const;

  Term* rhs;
  RhsBuilder builder;
  LhsAutomaton* nonExtLhsAutomaton;
  LhsAutomaton* extLhsAutomaton;
  DagRoot lhsDag;  // for unification
};

inline Term*
Rule::getRhs() const
{
  return rhs;
}

inline const RhsBuilder&
Rule::getRhsBuilder() const
{
  return builder;
}

//
//      Output function for Rule must be defined by library user
//
ostream& operator<<(ostream& s, const Rule* rule);
 
#endif
