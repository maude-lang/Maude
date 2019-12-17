/*

    This file is part of the Maude 3 interpreter.

    Copyright 2019 SRI International, Menlo Park, CA 94025, USA.

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
//	Code for legacy up functions.
//

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
  return legacyUnificationPairSymbol->makeDagNode(args);
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
  return legacyUnificationPairSymbol->makeDagNode(args);
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
  return legacyUnificationTripleSymbol->makeDagNode(args);
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
  return legacyUnificationTripleSymbol->makeDagNode(args);
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
  return legacyVariantSymbol->makeDagNode(args);
}
