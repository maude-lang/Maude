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
MetaLevel::upQid(int id, PointerMap& qidMap)
{
  void* p = const_cast<void*>(static_cast<const void*>(Token::name(id)));
  DagNode *d = static_cast<DagNode*>(qidMap.getMap(p));
  if (d == 0)
    {
      d = new QuotedIdentifierDagNode(qidSymbol, Token::backQuoteSpecials(id));
      (void) qidMap.setMap(p, d);
    }
  return d;
}

DagNode*
MetaLevel::upType(Sort* sort, PointerMap& qidMap)
{
  int id = sort->id();
  if (sort->index() == Sort::KIND)
    {
      string fullName("`[");
      ConnectedComponent* component = sort->component();
      int nrMaxSorts = component->nrMaximalSorts();
      for (int i = 1; i <= nrMaxSorts; i++)
	{
	  fullName += Token::name(component->sort(i)->id());
	  fullName += (i == nrMaxSorts) ? "`]" : "`,";
	}
      id = Token::encode(fullName.c_str());
    }
  return upQid(id, qidMap);
}

DagNode*
MetaLevel::upJoin(int id, Sort* sort, char sep, PointerMap& qidMap)
{
  Assert(sort != 0, "null sort");
  string fullName(Token::name(id));
  fullName += sep;
  if (sort->index() == Sort::KIND)
    {
      fullName += "`[";
      ConnectedComponent* component = sort->component();
      int nrMaxSorts = component->nrMaximalSorts();
      for (int i = 1; i <= nrMaxSorts; i++)
	{
	  fullName += Token::name(component->sort(i)->id());
	  fullName += (i == nrMaxSorts) ? "`]" : "`,";
	}
    }
  else
    fullName += Token::name(sort->id());
  return upQid(Token::encode(fullName.c_str()), qidMap);
}

local_inline DagNode*
MetaLevel::upConstant(int id, Sort* sort, PointerMap& qidMap)
{
  return upJoin(id, sort, '.', qidMap);
}

DagNode*
MetaLevel::upConstant(int id, DagNode* d, PointerMap& qidMap)
{
  Sort* sort = d->getSort();
  if (sort == 0)
    {
      //
      //	Can happen since sorts are not calculated in unifiers.
      //	Fortunately there shouldn't be any membership axioms involved
      //	in a unification problem and the precise sorts of constants is
      //	non-critical for disambiguation.
      //	So we temporarily compute a base sort.
      //
      Symbol* s = d->symbol();
      s->computeBaseSort(d);
      sort = s->rangeComponent()->sort(d->getSortIndex());
      d->setSortIndex(Sort::SORT_UNKNOWN);
    }
  return upJoin(id, sort, '.', qidMap);
}

inline DagNode*
MetaLevel::upVariable(int id, Sort* sort, PointerMap& qidMap)
{
  //
  //	id might be flagged (in the disjoint unification case) so we always unflag it.
  //
  return upJoin(Token::unflaggedCode(id), sort, ':', qidMap);
}

DagNode*
MetaLevel::upDagNode(DagNode* dagNode,
		     MixfixModule* m,
		     PointerMap& qidMap,
		     PointerMap& dagNodeMap)
{
  Assert(dagNode != 0, "null dagNode");
  Vector<DagNode*> args(2);  // can't be static!
  //
  //	See if we have already up'd this dag node via a different path.
  //
  DagNode *d = static_cast<DagNode*>(dagNodeMap.getMap(dagNode));
  if (d != 0)
    return d;
  //
  //	Examine symbol to see if we have a special representation with its own
  //	conversion scheme.
  //
  Symbol* s = dagNode->symbol();
  switch (m->getSymbolType(s).getBasicType())
    {
    case SymbolType::QUOTED_IDENTIFIER:
      {
	int id = static_cast<QuotedIdentifierDagNode*>(dagNode)->getIdIndex();
	d = upConstant(Token::quoteNameCode(id), dagNode, qidMap);
	break;
      }
    case SymbolType::STRING:
      {
	string result;
	Token::ropeToString(static_cast<StringDagNode*>(dagNode)->getValue(), result);
	d = upConstant(Token::encode(result.c_str()), dagNode, qidMap);
	break;
      }
    case SymbolType::FLOAT:
      {
	double mf = static_cast<FloatDagNode*>(dagNode)->getValue();
	d = upConstant(Token::doubleToCode(mf), dagNode, qidMap);
	break;
      }
    case SymbolType::SMT_NUMBER_SYMBOL:
      {
	const mpq_class& value = safeCast(SMT_NumberDagNode*, dagNode)->getValue();
	d = upSMT_Number(value, s, m, qidMap);
	break;
      }
    case SymbolType::VARIABLE:
      {
	VariableDagNode* v = safeCast(VariableDagNode*, dagNode);
	int id = (variableGenerator == 0) ? v->id() :
	  variableGenerator->getFreshVariableName(variableBase + v->getIndex());
	Sort* sort = safeCast(VariableSymbol*, dagNode->symbol())->getSort();
	d = upVariable(id, sort, qidMap);
	break;
      }
    default:
      {
	int nrArgs = s->arity();
	if (nrArgs == 0)
	  d = upConstant(s->id(), dagNode, qidMap);
	else
	  {
	    args[0] = upQid(m->getSymbolType(s).hasFlag(SymbolType::ITER) ?
			    iterToken(dagNode) : s->id(),
			    qidMap);
	    DagArgumentIterator a(*dagNode);
	    if (nrArgs == 1)
	      args[1] = upDagNode(a.argument(), m, qidMap, dagNodeMap);
	    else
	      {
		//
		//	nrArgs is only a suggestion - flattened assoc operators may have more args.
		//
		Vector<DagNode*> args2(0, nrArgs);  // can't be static!
		for (; a.valid(); a.next())
		  args2.append(upDagNode(a.argument(), m, qidMap, dagNodeMap));
		args[1] = metaArgSymbol->makeDagNode(args2);
	      }
	    d = metaTermSymbol->makeDagNode(args);
	  }
	break;
      }
    }
  (void) dagNodeMap.setMap(dagNode, d);
  return d;
}

DagNode*
MetaLevel::upTerm(const Term* term, MixfixModule* m, PointerMap& qidMap)
{
  Assert(term != 0, "null term");
  Vector<DagNode*> args(2);  // can't be static!
  //
  //	Examine symbol to see if we have a special representation with its own
  //	conversion scheme.
  //

  Symbol* s = term->symbol();
  switch (m->getSymbolType(s).getBasicType())
    {
    case SymbolType::QUOTED_IDENTIFIER:
      {
	int id = static_cast<const QuotedIdentifierTerm*>(term)->getIdIndex();
	return upConstant(Token::quoteNameCode(id), MixfixModule::disambiguatorSort(term), qidMap);
      }
    case SymbolType::STRING:
      {
	string result;
	Token::ropeToString(static_cast<const StringTerm*>(term)->getValue(), result);
	return upConstant(Token::encode(result.c_str()), MixfixModule::disambiguatorSort(term), qidMap);
      }
    case SymbolType::FLOAT:
      {
	double mf = static_cast<const FloatTerm*>(term)->getValue();
	return upConstant(Token::doubleToCode(mf), MixfixModule::disambiguatorSort(term), qidMap);
      }
    case SymbolType::SMT_NUMBER_SYMBOL:
      {
	const mpq_class& value = safeCast(const SMT_NumberTerm*, term)->getValue();
	return upSMT_Number(value, s, m, qidMap);
      }
    case SymbolType::VARIABLE:
      {
	const VariableTerm* vt = safeCast(const VariableTerm*, term);
	return upVariable(vt->id(), vt->getSort(), qidMap);
      }
    default:
      {
	int nrArgs = s->arity();
	if (nrArgs == 0)
	  return upConstant(s->id(), MixfixModule::disambiguatorSort(term), qidMap);
	else
	  {
	    int id = s->id();
	    if (m->getSymbolType(s).hasFlag(SymbolType::ITER))
	      {
		const mpz_class& number = safeCast(const S_Term*, term)->getNumber();
		if (number > 1)
		  {
		    string tmp(Token::name(id));
		    tmp += '^';
		    char* str = mpz_get_str(0, 10, number.get_mpz_t());
		    tmp += str;
		    free(str);
		    id = Token::encode(tmp.c_str());
		  }
	      }
	    args[0] = upQid(id, qidMap);
	    ArgumentIterator a(*(const_cast<Term*>(term)));
	    if (nrArgs == 1)
	      args[1] = upTerm(a.argument(), m, qidMap);
	    else
	      {
		//
		//	nrArgs is only a suggestion - flattened assoc operators may have more args.
		//
		Vector<DagNode*> args2(0, nrArgs);  // can't be static!
		for (; a.valid(); a.next())
		  args2.append(upTerm(a.argument(), m, qidMap));
		args[1] = metaArgSymbol->makeDagNode(args2);
	      }
	    return metaTermSymbol->makeDagNode(args);
	  }
      }
    }
}

int
MetaLevel::iterToken(DagNode* dagNode)
{
  int id = dagNode->symbol()->id();
  const mpz_class& number = safeCast(S_DagNode*, dagNode)->getNumber();
  if (number == 1)
    return id;
  string tmp(Token::name(id));
  tmp += '^';
  char* str = mpz_get_str(0, 10, number.get_mpz_t());
  tmp += str;
  free(str);
  return Token::encode(tmp.c_str());
}
    
DagNode*
MetaLevel::upContext(DagNode* dagNode,
		     MixfixModule* m,
		     DagNode* hole,
		     PointerMap& qidMap,
		     PointerMap& dagNodeMap)
{
  Assert(dagNode != 0, "null dagNode");
  //
  //	See if we have already up'd this dag node via a different path.
  //
  if (DagNode *d = static_cast<DagNode*>(dagNodeMap.getMap(dagNode)))
    return d;
  //
  //	See if our node is the hole.
  //
  if (dagNode == hole)
    return holeSymbol->makeDagNode();
  //
  //	Constants cannot contain the hole.
  //
  Symbol* s = dagNode->symbol();
  int nrArgs = s->arity();
  if (nrArgs == 0)
    return upDagNode(dagNode, m, qidMap, dagNodeMap);
  //
  //	Usual case.
  //
  Vector<DagNode*> args(2);  // can't be static!
  args[0] = upQid(m->getSymbolType(s).hasFlag(SymbolType::ITER) ?
		  iterToken(dagNode) : s->id(),
		  qidMap);
  DagArgumentIterator a(*dagNode);
  if (nrArgs == 1)
    args[1] = upContext(a.argument(), m, hole, qidMap, dagNodeMap);
  else
    {
      Vector<DagNode*> args2(0, nrArgs);  // can't be static!
      for (; a.valid(); a.next())
	args2.append(upContext(a.argument(), m, hole, qidMap, dagNodeMap));
      args[1] = metaArgSymbol->makeDagNode(args2);
    }
  DagNode* d = metaTermSymbol->makeDagNode(args);
  (void) dagNodeMap.setMap(dagNode, d);
  return d;
}

DagNode*
MetaLevel::upResultPair(DagNode* dagNode, MixfixModule* m)
{
  Assert(dagNode != 0, "null dagNode");
  static Vector<DagNode*> args(2);
  PointerMap qidMap;
  PointerMap dagNodeMap;
  args[0] = upDagNode(dagNode, m, qidMap, dagNodeMap);
  args[1] = upType(dagNode->getSort(), qidMap);
  return resultPairSymbol->makeDagNode(args);
}

DagNode*
MetaLevel::upResultPair(Term* term, MixfixModule* m)
{
  Assert(term != 0, "null term");
  static Vector<DagNode*> args(2);
  PointerMap qidMap;
  args[0] = upTerm(term, m, qidMap);
  args[1] = upType(term->getSort(), qidMap);
  return resultPairSymbol->makeDagNode(args);
}

DagNode*
MetaLevel::upResultTriple(DagNode* dagNode,
			  const Substitution& substitution,
			  const VariableInfo& variableInfo,
			  MixfixModule* m)
{
  Assert(dagNode != 0, "null dagNode");
  static Vector<DagNode*> args(3);
  PointerMap qidMap;
  PointerMap dagNodeMap;
  args[0] = upDagNode(dagNode, m, qidMap, dagNodeMap);
  args[1] = upType(dagNode->getSort(), qidMap);
  args[2] = upSubstitution(substitution, variableInfo, m, qidMap, dagNodeMap);
  return resultTripleSymbol->makeDagNode(args);
}

DagNode*
MetaLevel::upResult4Tuple(DagNode* dagNode,
			  const Substitution& substitution,
			  const VariableInfo& variableInfo,
			  DagNode* metaContext,
			  MixfixModule* m)
{
  Assert(dagNode != 0, "null dagNode");
  static Vector<DagNode*> args(4);
  PointerMap qidMap;
  PointerMap dagNodeMap;
  args[0] = upDagNode(dagNode, m, qidMap, dagNodeMap);
  args[1] = upType(dagNode->getSort(), qidMap);
  args[2] = upSubstitution(substitution, variableInfo, m, qidMap, dagNodeMap);
  args[3] = metaContext;
  return result4TupleSymbol->makeDagNode(args);
}

DagNode*
MetaLevel::upUnificationPair(const Substitution& substitution,
			     const VariableInfo& variableInfo,
			     const mpz_class& variableIndex,
			     MixfixModule* m)
{
  PointerMap qidMap;
  PointerMap dagNodeMap;
  Vector<DagNode*> args(2);
  args[0] = upSubstitution(substitution, variableInfo, m, qidMap, dagNodeMap);
  args[1] = succSymbol->makeNatDag(variableIndex);
  return unificationPairSymbol->makeDagNode(args);
}

DagNode*
MetaLevel::upUnificationTriple(const Substitution& substitution,
			       const VariableInfo& variableInfo,
			       const mpz_class& variableIndex,
			       MixfixModule* m)
{
  PointerMap qidMap;
  PointerMap dagNodeMap;
  Vector<DagNode*> args(3);
  upDisjointSubstitutions(substitution,
			  variableInfo,
			  m,
			  qidMap,
			  dagNodeMap,
			  args[0],
			  args[1]);
  args[2] = succSymbol->makeNatDag(variableIndex);
  return unificationTripleSymbol->makeDagNode(args);
}


void
MetaLevel::upDisjointSubstitutions(const Substitution& substitution,
				   const VariableInfo& variableInfo,
				   MixfixModule* m,
				   PointerMap& qidMap,
				   PointerMap& dagNodeMap,
				   DagNode*& left,
				   DagNode*& right)
{
  int nrVariables = variableInfo.getNrRealVariables();
  Vector<DagNode*> leftArgs(0, nrVariables);
  Vector<DagNode*> rightArgs(0, nrVariables);
  for (int i = 0; i < nrVariables; i++)
    {
      VariableTerm* variable = safeCast(VariableTerm*, variableInfo.index2Variable(i));
      DagNode* d = upAssignment(variable,
				substitution.value(i),
				m,
				qidMap,
				dagNodeMap);
      if (Token::isFlagged(variable->id()))
	rightArgs.append(d);
      else
	leftArgs.append(d);
    }
  int nrLeftArgs = leftArgs.size();
  if (nrLeftArgs == 0)
    left = emptySubstitutionSymbol->makeDagNode();
  else if (nrLeftArgs == 1)
    left = leftArgs[0];
  else
    left = substitutionSymbol->makeDagNode(leftArgs);
  int nrRightArgs = rightArgs.size();
  if (nrRightArgs == 0)
    right = emptySubstitutionSymbol->makeDagNode();
  else if (nrRightArgs == 1)
    right = rightArgs[0];
  else
    right = substitutionSymbol->makeDagNode(rightArgs);
}

DagNode*
MetaLevel::upSubstitution(const Substitution& substitution,
			  const VariableInfo& variableInfo,
			  MixfixModule* m,
			  PointerMap& qidMap,
			  PointerMap& dagNodeMap)
{
  int nrVariables = variableInfo.getNrRealVariables();
  if (nrVariables == 0)
    return emptySubstitutionSymbol->makeDagNode();
  if (nrVariables == 1)
    {
      return upAssignment(variableInfo.index2Variable(0),
			  substitution.value(0),
			  m,
			  qidMap,
			  dagNodeMap);
    }
  Vector<DagNode*> args(nrVariables);
  for (int i = 0; i < nrVariables; i++)
    {
      args[i] = upAssignment(variableInfo.index2Variable(i),
			     substitution.value(i),
			     m,
			     qidMap,
			     dagNodeMap);
    }
  return substitutionSymbol->makeDagNode(args);
}

DagNode*
MetaLevel::upAssignment(const Term* variable,
			DagNode* value,
			MixfixModule* m,
			PointerMap& qidMap,
			PointerMap& dagNodeMap)
{
  static Vector<DagNode*> args(2);
  args[0] = upTerm(variable, m, qidMap);
  args[1] = upDagNode(value, m, qidMap, dagNodeMap);
  return assignmentSymbol->makeDagNode(args);
}

DagNode*
MetaLevel::upAssignment(DagNode* variable,
			DagNode* value,
			MixfixModule* m,
			PointerMap& qidMap,
			PointerMap& dagNodeMap)
{
  static Vector<DagNode*> args(2);
  args[0] = upDagNode(variable, m, qidMap, dagNodeMap);
  args[1] = upDagNode(value, m, qidMap, dagNodeMap);
  return assignmentSymbol->makeDagNode(args);
}

DagNode*
MetaLevel::upSmtSubstitution(const Substitution& substitution,
			     const VariableInfo& variableInfo,
			     const NatSet& smtVariables,
			     MixfixModule* m,
			     PointerMap& qidMap,
			     PointerMap& dagNodeMap)
{
  int nrVariables = variableInfo.getNrRealVariables();
  Vector<DagNode*> args;

  for (int i = 0; i < nrVariables; i++)
    {
      //
      //	SMT variables are constrained rather than bound.
      //
      if (!smtVariables.contains(i))
	{
	  args.append(upAssignment(variableInfo.index2Variable(0),
				   substitution.value(0),
				   m,
				   qidMap,
				   dagNodeMap));
	}
    }

  int nrBindings = args.size();
  if (nrBindings == 0)
    return emptySubstitutionSymbol->makeDagNode();
  if (nrBindings == 1)
    return args[0];
  return substitutionSymbol->makeDagNode(args);
}

DagNode*
MetaLevel::upSmtResult(DagNode* state,
		       const Substitution& substitution,
		       const VariableInfo& variableInfo,
		       const NatSet& smtVariables,
		       DagNode* constraint,
		       const mpz_class& variableNumber,
		       MixfixModule* m)
{
  Assert(state != 0, "null state");
  Assert(constraint != 0, "null constraint");
  Vector<DagNode*> args(4);
  PointerMap qidMap;
  PointerMap dagNodeMap;
  args[0] = upDagNode(state, m, qidMap, dagNodeMap);
  args[1] = upSmtSubstitution(substitution,
			     variableInfo,
			     smtVariables,
			     m,
			     qidMap,
			      dagNodeMap);
  args[2] = upDagNode(constraint, m, qidMap, dagNodeMap);
  args[3] = succSymbol->makeNatDag(variableNumber);
  return smtResultSymbol->makeDagNode(args);
}

DagNode*
MetaLevel::upSmtFailure()
{
  return smtFailureSymbol->makeDagNode();
}

DagNode*
MetaLevel::upFailurePair()
{
  return failure2Symbol->makeDagNode();
}

DagNode*
MetaLevel::upFailureTriple()
{
  return failure3Symbol->makeDagNode();
}

DagNode*
MetaLevel::upNoUnifierPair(bool incomplete)
{
  return (incomplete ? noUnifierIncompletePairSymbol : noUnifierPairSymbol)->makeDagNode();
}

DagNode*
MetaLevel::upNoUnifierTriple(bool incomplete)
{
  return (incomplete ? noUnifierIncompleteTripleSymbol : noUnifierTripleSymbol)->makeDagNode();
}

DagNode*
MetaLevel::upNoMatchSubst()
{
  return noMatchSubstSymbol->makeDagNode();
}

DagNode*
MetaLevel::upNoMatchPair()
{
  return noMatchPairSymbol->makeDagNode();
}

DagNode*
MetaLevel::upMatchPair(const Substitution& substitution,
		       const VariableInfo& variableInfo,
		       DagNode* dagNode,
		       DagNode* hole,
		       MixfixModule* m)
{
  PointerMap qidMap;
  PointerMap dagNodeMap;
  static Vector<DagNode*> args(2);
  args[0] = upSubstitution(substitution, variableInfo, m, qidMap, dagNodeMap);
  args[1] = upContext(dagNode, m, hole, qidMap, dagNodeMap);
  return matchPairSymbol->makeDagNode(args);
}

DagNode*
MetaLevel::upFailure4Tuple()
{
  return failure4Symbol->makeDagNode();
}

DagNode*
MetaLevel::upTrace(const RewriteSequenceSearch& state, MixfixModule* m)
{
  Vector<int> steps;
  for (int i = state.getStateNr(); i != 0; i = state.getStateParent(i))
    steps.append(i);

  int nrSteps = steps.size();   
  if (nrSteps == 0)
    return nilTraceSymbol->makeDagNode();

  Vector<DagNode*> args(nrSteps);
  PointerMap qidMap;
  PointerMap dagNodeMap;
  int j = 0;
  for (int i = nrSteps - 1; i >= 0; --i, ++j)
    args[j] = upTraceStep(state, steps[i], m, qidMap, dagNodeMap);
  return (nrSteps == 1) ? args[0] : traceSymbol->makeDagNode(args);
}

DagNode*
MetaLevel::upTraceStep(const RewriteSequenceSearch& state,
		       int stateNr,
		       MixfixModule* m,
		       PointerMap& qidMap,
		       PointerMap& dagNodeMap)
{
  static Vector<DagNode*> args(3);
  int parentNr = state.getStateParent(stateNr);
  DagNode* dagNode = state.getStateDag(parentNr);
  args[0] = upDagNode(dagNode, m, qidMap, dagNodeMap);
  args[1] = upType(dagNode->getSort(), qidMap);
  args[2] = upRl(state.getStateRule(stateNr), m, qidMap);
  return traceStepSymbol->makeDagNode(args);
}

DagNode*
MetaLevel::upFailureTrace()
{
  return failureTraceSymbol->makeDagNode();
}

DagNode*
MetaLevel::upNoParse(int badTokenIndex)
{
  static Vector<DagNode*> args(1);
  args[0] = succSymbol->makeNatDag(badTokenIndex);
  return noParseSymbol->makeDagNode(args);
}

DagNode*
MetaLevel::upAmbiguity(Term* parse1, Term* parse2, MixfixModule* m)
{
  Assert(parse1 != 0 && parse2 != 0, "null term");
  static Vector<DagNode*> args(2);
  static Vector<DagNode*> args2(2);
  PointerMap qidMap;
  args[0] = upTerm(parse1, m, qidMap);
  args[1] = upType(parse1->getSort(), qidMap);
  args2[0] = resultPairSymbol->makeDagNode(args);
  args[0] = upTerm(parse2, m, qidMap);
  args[1] = upType(parse2->getSort(), qidMap);
  args2[1] = resultPairSymbol->makeDagNode(args);
  return ambiguitySymbol->makeDagNode(args2);
}

DagNode*
MetaLevel::upBool(bool value)
{
  return value ? trueTerm.getDag() : falseTerm.getDag();
}

DagNode*
MetaLevel::upKindSet(const Vector<ConnectedComponent*>& kinds)
{
  int nrKinds = kinds.length();
  if (nrKinds == 0)
    return new FreeDagNode(emptySortSetSymbol);
  PointerMap qidMap;
  if (nrKinds == 1)
    return upType(kinds[0]->sort(0), qidMap);
  Vector<DagNode*> args(nrKinds);
  for (int i = 0; i < nrKinds; i++)
    args[i] = upType(kinds[i]->sort(0), qidMap);
  return sortSetSymbol->makeDagNode(args);
}

DagNode*
MetaLevel::upSortSet(const Vector<Sort*>& sorts)
{
  PointerMap qidMap;
  return upSortSet(sorts, 0, sorts.length(), qidMap);
}

DagNode*
MetaLevel::upSortSet(const Vector<Sort*>& sorts,
		     int begin,
		     int nrSorts,
		     PointerMap& qidMap)
{
  if (nrSorts == 0)
    return new FreeDagNode(emptySortSetSymbol);
  if (nrSorts == 1)
    return upType(sorts[begin], qidMap);
  Vector<DagNode*> args(nrSorts);
  for (int i = 0; i < nrSorts; i++, begin++)
    args[i] = upType(sorts[begin], qidMap);
  return sortSetSymbol->makeDagNode(args);
}

DagNode*
MetaLevel::upQidList(const Vector<int>& ids)
{
  PointerMap qidMap;
  return upQidList(ids, qidMap);
}

DagNode*
MetaLevel::upQidList(const Vector<int>& ids, PointerMap& qidMap)
{
  int nrIds = ids.length();
  if (nrIds == 0)
    return new FreeDagNode(nilQidListSymbol);
  if (nrIds == 1)
    return new QuotedIdentifierDagNode(qidSymbol, Token::backQuoteSpecials(ids[0]));
  Vector<DagNode*> args(nrIds);
  for (int i = 0; i < nrIds; i++)
    args[i] = upQid(ids[i], qidMap);
  return qidListSymbol->makeDagNode(args);
}

DagNode*
MetaLevel::upTypeListSet(const Vector<OpDeclaration>& opDecls,
			 const NatSet& chosenDecls,
			 PointerMap& qidMap)
{
  Vector<DagNode*> args;
  FOR_EACH_CONST(i, NatSet, chosenDecls)
    args.append(upTypeList(opDecls[*i].getDomainAndRange(), true, qidMap));
  int nrArgs = args.size();
  if (nrArgs == 0)
    return new FreeDagNode(emptySortSetSymbol);
  if (nrArgs == 1)
    return args[0];
  return sortSetSymbol->makeDagNode(args);
}		     

DagNode*
MetaLevel::upTypeList(const Vector<Sort*>& types,
		      bool omitLast,
		      PointerMap& qidMap)
{
  int nrTypes = types.size();
  if (omitLast)
    --nrTypes;
  if (nrTypes == 0)
    return new FreeDagNode(nilQidListSymbol);
  if (nrTypes == 1)
    return upType(types[0], qidMap);
  Vector<DagNode*> args(nrTypes);
  for (int i = 0; i < nrTypes; ++i)
    args[i] = upType(types[i], qidMap);
  return qidListSymbol->makeDagNode(args);
}

DagNode*
MetaLevel::upSubstitution(const Vector<DagNode*>& substitution,
			  const NarrowingVariableInfo& variableInfo,
			  int nrVariables,
			  MixfixModule* m,
			  PointerMap& qidMap,
			  PointerMap& dagNodeMap)
{
  if (nrVariables == 0)
    return emptySubstitutionSymbol->makeDagNode();
  else if (nrVariables == 1)
    {
      return upAssignment(variableInfo.index2Variable(0),
			  substitution[0],
			  m,
			  qidMap,
			  dagNodeMap);
    }

  Vector<DagNode*> args(nrVariables);
  for (int i = 0; i < nrVariables; ++i)
    {
      args[i] = upAssignment(variableInfo.index2Variable(i),
			     substitution[i],
			     m,
			     qidMap,
			     dagNodeMap);
    }
  return substitutionSymbol->makeDagNode(args);
}

DagNode*
MetaLevel::upUnificationPair(const Vector<DagNode*>& unifier,
			     const NarrowingVariableInfo& variableInfo,
			     const mpz_class& variableIndex,
			     MixfixModule* m)
{
  PointerMap qidMap;
  PointerMap dagNodeMap;
  Vector<DagNode*> args(2);

  args[0] = upSubstitution(unifier, variableInfo, unifier.size(), m, qidMap, dagNodeMap);
  args[1] = succSymbol->makeNatDag(variableIndex);
  return unificationPairSymbol->makeDagNode(args);
}

void
MetaLevel::upDisjointSubstitutions(const Vector<DagNode*>& unifier,
				   const NarrowingVariableInfo& variableInfo,
				   MixfixModule* m,
				   PointerMap& qidMap,
				   PointerMap& dagNodeMap,
				   DagNode*& left,
				   DagNode*& right)
{
  int nrVariables = unifier.size();
  Vector<DagNode*> leftArgs(0, nrVariables);
  Vector<DagNode*> rightArgs(0, nrVariables);
  for (int i = 0; i < nrVariables; i++)
    {
      VariableDagNode* variable = variableInfo.index2Variable(i);
      DagNode* d = upAssignment(variable,
				unifier[i],
				m,
				qidMap,
				dagNodeMap);
      if (Token::isFlagged(variable->id()))
	rightArgs.append(d);
      else
	leftArgs.append(d);
    }
  int nrLeftArgs = leftArgs.size();
  if (nrLeftArgs == 0)
    left = emptySubstitutionSymbol->makeDagNode();
  else if (nrLeftArgs == 1)
    left = leftArgs[0];
  else
    left = substitutionSymbol->makeDagNode(leftArgs);
  int nrRightArgs = rightArgs.size();
  if (nrRightArgs == 0)
    right = emptySubstitutionSymbol->makeDagNode();
  else if (nrRightArgs == 1)
    right = rightArgs[0];
  else
    right = substitutionSymbol->makeDagNode(rightArgs);
}

DagNode*
MetaLevel::upUnificationTriple(const Vector<DagNode*>& unifier,
			       const NarrowingVariableInfo& variableInfo,
			       const mpz_class& variableIndex,
			       MixfixModule* m)
{
  PointerMap qidMap;
  PointerMap dagNodeMap;
  Vector<DagNode*> args(3);

  upDisjointSubstitutions(unifier, variableInfo, m, qidMap, dagNodeMap, args[0], args[1]);
  args[2] = succSymbol->makeNatDag(variableIndex);
  return unificationTripleSymbol->makeDagNode(args);
}

DagNode*
MetaLevel::upVariant(const Vector<DagNode*>& variant, 
		     const NarrowingVariableInfo& variableInfo,
		     const mpz_class& variableIndex,
		     const mpz_class& parentIndex,
		     bool moreInLayer,
		     MixfixModule* m)
{
  PointerMap qidMap;
  PointerMap dagNodeMap;
  Vector<DagNode*> args(5);

  int nrVariables = variant.size() - 1;
  args[0] = upDagNode(variant[nrVariables], m, qidMap, dagNodeMap);
  args[1] = upSubstitution(variant, variableInfo, nrVariables, m, qidMap, dagNodeMap);
  args[2] = succSymbol->makeNatDag(variableIndex);
  args[3] = (parentIndex >= 0) ? succSymbol->makeNatDag(parentIndex) :
    noParentSymbol->makeDagNode();
  args[4] = upBool(moreInLayer);
  return variantSymbol->makeDagNode(args);
}

DagNode*
MetaLevel::upNoVariant(bool incomplete)
{
  return (incomplete ? noVariantIncompleteSymbol : noVariantSymbol)->makeDagNode();
}

DagNode*
MetaLevel::upSMT_Number(const mpq_class& value, Symbol* symbol, MixfixModule* m, PointerMap& qidMap)
{
  Sort* sort = symbol->getRangeSort();
  int id = m->getSMT_NumberToken(value, sort);
  return upJoin(id, sort, '.', qidMap);
}
