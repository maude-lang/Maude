/*

    This file is part of the Maude 3 interpreter.

    Copyright 1997-2023 SRI International, Menlo Park, CA 94025, USA.

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
//	Abstract class for table of rules indexed under some symbol.
//
#ifndef _ruleTable_hh_
#define _ruleTable_hh_
#include <list>
#include <stack>

class RuleTable
{
public:
  RuleTable();
  void offerRule(Rule* rule);
  const Vector<Rule*>& getRules() const;
  bool ruleFree() const;
  virtual void compileRules();
  virtual DagNode* ruleRewrite(DagNode* subject, RewritingContext& context);
  virtual void resetRules();

protected:
  virtual bool acceptRule(Rule* rule) = 0;
  DagNode* applyRules(DagNode* subject,
		      RewritingContext& context,
		      ExtensionInfo* extensionInfo);
  void resetEachRule();

private:
  Vector<Rule*> rules;
  int nextRule;
};

inline const Vector<Rule*>&
RuleTable::getRules() const
{
  return rules;
}

inline void
RuleTable::offerRule(Rule* rule)
{
  if (acceptRule(rule))
    rules.append(rule);
}

inline bool
RuleTable::ruleFree() const
{
  return rules.isNull();
}

#endif
