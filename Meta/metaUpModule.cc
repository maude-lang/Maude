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

DagNode*
MetaLevel::upMbs(bool flat, ImportModule* m, PointerMap& qidMap)
{
  const Vector<SortConstraint*>& mbs = m->getSortConstraints();
  int nrMbs = flat ? mbs.length() : m->getNrOriginalMembershipAxioms();

  static Vector<DagNode*> args;
  args.clear();
  for (int i = 0; i < nrMbs; i++)
    {
      const SortConstraint* mb = mbs[i];
      if (!(mb->isBad()))
	args.append(upMb(mb, m, qidMap));
    }

  int nrMetaMbs = args.length();
  if (nrMetaMbs == 0)
    return emptyMembAxSetSymbol->makeDagNode();
  else if (nrMetaMbs == 1)
    return args[0];
  return membAxSetSymbol->makeDagNode(args);
}

DagNode*
MetaLevel::upMb(const SortConstraint* mb, MixfixModule* m, PointerMap& qidMap)
{
  static Vector<DagNode*> args(4);

  args[0] = upTerm(mb->getLhs(), m, qidMap);
  args[1] = upType(mb->getSort(), qidMap);
  const Vector<ConditionFragment*>& condition = mb->getCondition();
  if (condition.empty())
    {
      args.resize(3);
      args[2] = upStatementAttributes(m, MixfixModule::MEMB_AX, mb, qidMap);
      return mbSymbol->makeDagNode(args);
    }
  args.resize(4);
  args[2] = upCondition(condition, m, qidMap);
  args[3] = upStatementAttributes(m, MixfixModule::MEMB_AX, mb, qidMap);
  return cmbSymbol->makeDagNode(args);
}

DagNode*
MetaLevel::upEqs(bool flat, ImportModule* m, PointerMap& qidMap)
{
  const Vector<Equation*>& equations = m->getEquations();
  int nrEquations = flat ? equations.length() : m->getNrOriginalEquations();

  static Vector<DagNode*> args;
  args.clear();
  for (int i = 0; i < nrEquations; i++)
    {
      const Equation* eq = equations[i];
      if (!(eq->isBad()))
	args.append(upEq(eq, m, qidMap));
    }

  int nrMetaEquations = args.length();
  if (nrMetaEquations == 0)
    return emptyEquationSetSymbol->makeDagNode();
  else if (nrMetaEquations == 1)
    return args[0];
  return equationSetSymbol->makeDagNode(args);
}

DagNode*
MetaLevel::upEq(const Equation* equation, MixfixModule* m, PointerMap& qidMap)
{
  static Vector<DagNode*> args(4);

  args[0] = upTerm(equation->getLhs(), m, qidMap);
  args[1] = upTerm(equation->getRhs(), m, qidMap);
  const Vector<ConditionFragment*>& condition = equation->getCondition();
  if (condition.empty())
    {
      args.resize(3);
      args[2] = upStatementAttributes(m, MixfixModule::EQUATION, equation, qidMap);
      return eqSymbol->makeDagNode(args);
    }
  args.resize(4);
  args[2] = upCondition(condition, m, qidMap);
  args[3] = upStatementAttributes(m, MixfixModule::EQUATION, equation, qidMap);
  return ceqSymbol->makeDagNode(args);
}

DagNode*
MetaLevel::upRls(bool flat, ImportModule* m, PointerMap& qidMap)
{
  const Vector<Rule*>& rules = m->getRules();
  int nrRules = flat ? rules.length() : m->getNrOriginalRules();

  static Vector<DagNode*> args;
  args.clear();
  for (int i = 0; i < nrRules; i++)
    {
      const Rule* rl = rules[i];
      if (!(rl->isBad()))
	args.append(upRl(rl, m, qidMap));
    }

  int nrMetaRules = args.length();
  if (nrMetaRules == 0)
    return emptyRuleSetSymbol->makeDagNode();
  else if (nrMetaRules == 1)
    return args[0];
  return ruleSetSymbol->makeDagNode(args);
}

DagNode*
MetaLevel::upRl(const Rule* rule, MixfixModule* m, PointerMap& qidMap)
{
  static Vector<DagNode*> args(4);

  args[0] = upTerm(rule->getLhs(), m, qidMap);
  args[1] = upTerm(rule->getRhs(), m, qidMap);
  const Vector<ConditionFragment*>& condition = rule->getCondition();
  if (condition.empty())
    {
      args.resize(3);
      args[2] = upStatementAttributes(m, MixfixModule::RULE, rule, qidMap);
      return rlSymbol->makeDagNode(args);
    }
  args.resize(4);
  args[2] = upCondition(condition, m, qidMap);
  args[3] = upStatementAttributes(m, MixfixModule::RULE, rule, qidMap);
  return crlSymbol->makeDagNode(args);
}

DagNode*
MetaLevel::upStatementAttributes(MixfixModule* m,
				 MixfixModule::ItemType type,
				 const PreEquation *pe,
				 PointerMap& qidMap)
{
  Vector<DagNode*> args;

  int id = pe->getLabel().id();
  if (id != NONE)
    {
      Vector<DagNode*> args2(1);
      args2[0] = upQid(id, qidMap);
      args.append(labelSymbol->makeDagNode(args2));
    }
  int metadata = m->getMetadata(type, pe);
  if (metadata != NONE)
    {
      Vector<DagNode*> args2(1);
      args2[0] = new StringDagNode(stringSymbol, Token::codeToRope(metadata));
      args.append(metadataSymbol->makeDagNode(args2));
    }
  if (pe->isNonexec())
    {
      Vector<DagNode*> args2;
      args.append(nonexecSymbol->makeDagNode(args2));
    }
  const Equation* eq = dynamic_cast<const Equation*>(pe);
  if (eq != 0 && eq->isOwise())
    {
      Vector<DagNode*> args2;
      args.append(owiseSymbol->makeDagNode(args2));
    }
  switch (args.length())
    {
    case 0:
      return emptyAttrSetSymbol->makeDagNode(args);
    case 1:
      return args[0];
    }
  return attrSetSymbol->makeDagNode(args);
}

DagNode*
MetaLevel::upLabel(const Label& label, PointerMap& qidMap)
{
  static const char emptyString[1] = {'\0'};

  int id = label.id();
  if (id == NONE)
    id = Token::encode(emptyString);
  return upQid(id, qidMap);
}

DagNode*
MetaLevel::upCondition(const Vector<ConditionFragment*>& condition,
		       MixfixModule* m,
		       PointerMap& qidMap)
{
  static Vector<DagNode*> args;
 
  int nrFragments = condition.length();
  if (nrFragments == 1)
    return upConditionFragment(condition[0], m, qidMap);

  args.resize(nrFragments);
  for (int i = 0; i < nrFragments; i++)
    args[i] = upConditionFragment(condition[i], m, qidMap);
  return conjunctionSymbol->makeDagNode(args);
}

DagNode*
MetaLevel::upConditionFragment(const ConditionFragment* fragment,
			       MixfixModule* m,
			       PointerMap& qidMap)
{
  static Vector<DagNode*> args(2);
  Symbol* s;

  if (const EqualityConditionFragment* e =
      dynamic_cast<const EqualityConditionFragment*>(fragment))
    {
      args[0] = upTerm(e->getLhs(), m, qidMap);
      args[1] = upTerm(e->getRhs(), m, qidMap);
      s = equalityConditionSymbol;
    }
  else if (const SortTestConditionFragment* t =
	   dynamic_cast<const SortTestConditionFragment*>(fragment))
    {
      args[0] = upTerm(t->getLhs(), m, qidMap);
      args[1] = upType(t->getSort(), qidMap);
      s = sortTestConditionSymbol;
    }
  else if(const AssignmentConditionFragment* a =
	  dynamic_cast<const AssignmentConditionFragment*>(fragment))
    {
      args[0] = upTerm(a->getLhs(), m, qidMap);
      args[1] = upTerm(a->getRhs(), m, qidMap);
      s = matchConditionSymbol;
    }
  else if(const RewriteConditionFragment* r =
	  dynamic_cast<const RewriteConditionFragment*>(fragment))
    {
      args[0] = upTerm(r->getLhs(), m, qidMap);
      args[1] = upTerm(r->getRhs(), m, qidMap);
      s = rewriteConditionSymbol;
    }
  else
    CantHappen("bad condition fragment");
  return s->makeDagNode(args);
}
