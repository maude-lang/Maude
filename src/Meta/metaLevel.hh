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
//      Class for meta level signature.
//
#ifndef _metaLevel_hh_
#define _metaLevel_hh_
#include "cachedDag.hh"
#include "metaModuleCache.hh"

class MetaLevel
{
  NO_COPYING(MetaLevel);

public:
  MetaLevel();
  MetaLevel(const MetaLevel* original, SymbolMap* map);
  ~MetaLevel();

  bool bind(const char* name, Symbol* symbol);
  bool bind(const char* name, Term* term);
  void getSymbolAttachments(Vector<const char*>& purposes,
			    Vector<Symbol*>& symbols);
  void getTermAttachments(Vector<const char*>& purposes,
			  Vector<Term*>& terms);
  void postInterSymbolPass();
  void reset();

  DagNode* upResultPair(DagNode* dagNode, MixfixModule* m);
  DagNode* upResultPair(Term* term, MixfixModule* m);
  DagNode* upNoParse(int badTokenIndex);
  DagNode* upAmbiguity(Term* parse1, Term* parse2, MixfixModule* m);
  DagNode* upBool(bool value);
  DagNode* upType(Sort* sort, PointerMap& qidMap);
  DagNode* upKindSet(const Vector<ConnectedComponent*>& kinds);
  DagNode* upSortSet(const Vector<Sort*>& sorts);
  DagNode* upQidList(const Vector<int>& ids);
  DagNode* upResultTriple(DagNode* dagNode,
			  const Substitution& substitution,
			  const VariableInfo& variableInfo,
			  MixfixModule* m);
  DagNode* upFailureTriple();
  DagNode* upResult4Tuple(DagNode* dagNode,
			  const Substitution& substitution,
			  const VariableInfo& variableInfo,
			  DagNode* metaContext,
			  MixfixModule* m);
  DagNode* upFailure4Tuple();
  DagNode* upContext(DagNode* dagNode,
		     MixfixModule* m,
		     DagNode* hole,
		     PointerMap& qidMap,
		     PointerMap& dagNodeMap);
  DagNode* upSubstition(const Substitution& substitution,
			const VariableInfo& variableInfo,
			MixfixModule* m,
			PointerMap& qidMap,
			PointerMap& dagNodeMap);
  DagNode* upNoMatchSubst();
  DagNode* upNoMatchPair();
  DagNode* upMatchPair(const Substitution& substitution,
		       const VariableInfo& variableInfo,
		       DagNode* dagNode,
		       DagNode* hole,
		       MixfixModule* m);

  DagNode* upModule(bool flat, PreModule* pm, PointerMap& qidMap);
  DagNode* upImports(PreModule* pm, PointerMap& qidMap);
  DagNode* upSorts(bool flat, ImportModule* m, PointerMap& qidMap);
  DagNode* upSubsortDecls(bool flat, ImportModule* m, PointerMap& qidMap);
  DagNode* upOpDecls(bool flat, ImportModule* m, PointerMap& qidMap);
  DagNode* upMbs(bool flat, ImportModule* m, PointerMap& qidMap);
  DagNode* upEqs(bool flat, ImportModule* m, PointerMap& qidMap);
  DagNode* upRls(bool flat, ImportModule* m, PointerMap& qidMap);
  DagNode* upDagNode(DagNode* dagNode,
		     MixfixModule* m,
		     PointerMap& qidMap,
		     PointerMap& dagNodeMap);

  bool downBound(DagNode* metaBound, int& bound) const;
  bool downSaturate(DagNode* metaBound, int& bound) const;
  bool downBound64(DagNode* metaBound, Int64& bound) const;
  bool downSaturate64(DagNode* metaBound, Int64& bound) const;

  bool downBool(DagNode* metaBool, bool& value);
  bool downQid(DagNode* metaQid, int& id);
  MetaModule* downModule(DagNode* metaModule);
  bool downTermAndSort(DagNode* metaTerm ,
		       DagNode* metaSort , 
		       Term*& term ,
		       Sort*& sort ,
		       MixfixModule* m);
  bool downTermPair(DagNode* metaTerm1,
		    DagNode* metaTerm2, 
		    Term*& term1,
		    Term*& term2,
		    MixfixModule* m);
  Term* downTerm(DagNode* metaTerm, MixfixModule* m);
  bool downCondition(DagNode* metaCondition,
		     MixfixModule* m,
		     Vector<ConditionFragment*>& condition);
  bool downSimpleSort(DagNode* metaSort, MixfixModule* m, Sort*& sort);
  bool downType(DagNode* metaType, MixfixModule* m, Sort*& type);
  bool downQidList(DagNode* metaQidList, Vector<int>& ids);
  bool downComponent(DagNode* metaComponent,
		     MixfixModule* m,
		     ConnectedComponent*& component);
  bool downSubstitution(DagNode* metaSubstitution,
			MixfixModule* m,
			Vector<Term*>& variables,
			Vector<Term*>& values);
  bool downAssignment(DagNode* metaAssignment,
		      MixfixModule* m,
		      Vector<Term*>& variables,
		      Vector<Term*>& values);

private:
  enum Implementation
  {
    nrPreallocatedArgs = 3
  };

  enum ComplexSymbolType
  {
    REGULAR_SYMBOL,
    POLYMORPH,
    BUBBLE
  };

  enum Flags
  {
    NONEXEC = 1,
    OWISE = 2
  };

  struct AttributeInfo
  {
    AttributeInfo();
    
    SymbolType symbolType;
    Vector<int> strategy;
    NatSet frozen;
    NatSet polyArgs;
    int prec;
    Vector<int> gather;
    Vector<int> format;
    DagNode* identity;
    DagNode* fixUpInfo;
  };

  static int iterToken(DagNode* dagNode);
  static DagNode* upGroup(const Vector<DagNode*>& args,
			  Symbol* emptyCase,
			  Symbol* multipleCase);
  static void convertToTokens(const Vector<int>& ids, Vector<Token>& tokens);

  DagNode* upQid(int id, PointerMap& qidMap);
  DagNode* upJoin(int id, Sort* sort, char sep, PointerMap& qidMap);
  DagNode* upConstant(int id, Sort* sort, PointerMap& qidMap);
  DagNode* upVariable(int id, Sort* sort, PointerMap& qidMap);
  DagNode* upTerm(const Term* term, MixfixModule* m, PointerMap& qidMap);

  DagNode* upAssignment(const Term* variable,
			DagNode* value,
			MixfixModule* m,
			PointerMap& qidMap,
			PointerMap& dagNodeMap);

  DagNode* upPolymorphDecl(ImportModule* m, int index, PointerMap& qidMap);
  DagNode* upOpDecl(ImportModule* m, int symbolNr, int declNr, PointerMap& qidMap);
  DagNode* upMb(const SortConstraint* mb, MixfixModule* m, PointerMap& qidMap);
  DagNode* upEq(const Equation* equation, MixfixModule* m, PointerMap& qidMap);
  DagNode* upRl(const Rule* rule, MixfixModule* m, PointerMap& qidMap);
  DagNode* upLabel(const Label& label, PointerMap& qidMap);
  DagNode* upCondition(const Vector<ConditionFragment*>& condition,
		       MixfixModule* m,
		       PointerMap& qidMap);
  DagNode* upConditionFragment(const ConditionFragment* fragment,
			       MixfixModule* m,
			       PointerMap& qidMap);
  DagNode* upStatementAttributes(MixfixModule* m,
				 MixfixModule::ItemType type,
				 const PreEquation *pe,
				 PointerMap& qidMap);
  DagNode* upSortSet(const Vector<Sort*>& sorts,
		     int begin,
		     int nrSorts,
		     PointerMap& qidMap);
  DagNode* upAttributeSet(SymbolType st, Vector<DagNode*>& args);
  DagNode* upStrat(const Vector<int>& strategy);
  DagNode* upFrozen(const NatSet& frozen);
  DagNode* upGather(const Vector<int>& gather, PointerMap& qidMap);
  DagNode* upIdentity(MixfixModule* m,
		      SymbolType st,
		      Term* identity,
		      PointerMap& qidMap);
  DagNode* upQidList(const Vector<int>& ids, PointerMap& qidMap);
  DagNode* upPolymorphSpecial(int index, MixfixModule* m, PointerMap& qidMap);
  DagNode* upIdHook(int purpose, const Vector<int>& items, PointerMap& qidMap);
  DagNode* upOpHook(int purpose, Symbol* op, PointerMap& qidMap);
  DagNode* upTermHook(int purpose, Term* term, MixfixModule* m, PointerMap& qidMap);
  DagNode* upSpecial(Symbol* symbol,
		     const OpDeclaration& decl,
		     MixfixModule* m,
		     PointerMap& qidMap);
  DagNode* upModuleExpression(const ModuleExpression* e, PointerMap& qidMap);
  DagNode* upRenaming(const Renaming* r, PointerMap& qidMap);
  DagNode* upTypeSorts(const set<int>& sorts, PointerMap& qidMap);
  DagNode* upRenamingAttributeSet(const Renaming* r, int index, PointerMap& qidMap);

  bool downModuleExpression(DagNode* metaExpr, ImportModule*& m);
  bool downRenamings(DagNode* metaRenamings, Renaming* renaming);
  bool downRenaming(DagNode* metaRenaming, Renaming* renaming);
  bool downRenamingTypes(DagNode* metaTypes, Renaming* renaming);
  bool downRenamingType(DagNode* metaType, Renaming* renaming);
  bool downRenamingAttributes(DagNode* metaRenamingAttributes, Renaming* renaming);
  bool downRenamingAttribute(DagNode* metaRenamingAttribute, Renaming* renaming);
  
  bool downVariable(DagNode* metaVariable, MixfixModule* m, Symbol*& vs);
  bool downTypeList(DagNode* metaTypeList, MixfixModule* m, Vector<Sort*>& typeList);
  bool downPolymorphTypeList(DagNode* metaTypeList,
			     MixfixModule* m,
			     const NatSet& polyArgs,
			     Vector<Sort*>& typeList);
  bool downType2(int id, MixfixModule* m, Sort*& type);
  bool downSimpleSortList(DagNode* metaSortList, MixfixModule* m, Vector<Sort*>& sortList);
  bool downNatList(DagNode* metaNatList, Vector<int>& intList);
  bool downImports(DagNode* metaImports, MetaModule* m);
  bool downImport(DagNode* metaImport, MetaModule* m);
  bool downSorts(DagNode* metaSorts, MixfixModule* m);
  bool downSort(DagNode* metaSort, MixfixModule* m);
  bool downSubsorts(DagNode* metaSubsorts, MixfixModule* m);
  bool downSubsort(DagNode* metaSubsort, MixfixModule* m);
  bool downOpDecls(DagNode* metaOpDecls, MetaModule* m);
  bool downOpDecl(DagNode* metaOpDecl, MetaModule* m);
  bool downAttrSet(DagNode* metaAttrSet, AttributeInfo& ai);
  bool downAttr(DagNode* metaAttr, AttributeInfo& ai);
  bool downBubbleSpec(DagNode* metaBubbleSpec,
		      MetaModule* m,
		      Symbol* topSymbol,
		      int& bubbleSpecIndex);

  void checkHookList(DagNode* metaHookList, SymbolType& symbolType);
  void checkHook(DagNode* metaIdHook, SymbolType& symbolType);
 
  bool downConditionFragment(DagNode* metaConditionFragment,
			     MixfixModule* m,
			     ConditionFragment*& fragment);

  bool downStatementAttrSet(DagNode* metaAttrSet, int& label, int& metadata, FlagSet& flags);
  bool downStatementAttr(DagNode* metaAttr, int& label, int& metadata, FlagSet& flags);

  bool downMembAxs(DagNode* metaMembAxs, MixfixModule* m);
  bool downMembAx(DagNode* metaMembAx, MixfixModule* m);
  bool downEquations(DagNode* metaEquations, MixfixModule* m);
  bool downEquation(DagNode* metaEquation, MixfixModule* m);
  bool downRules(DagNode* metaRules, MixfixModule* m);
  bool downRule(DagNode* metaRule, MixfixModule* m);
  bool downTermList(DagNode* metaTermList, MixfixModule* m, Vector<Term*>& termList);
  bool downAssignment(DagNode* metaAssignment,
		      MixfixModule* m,
		      Vector<Symbol*>& variables,
		      Vector<Term*>& values);

  bool downFixUps(MetaModule* m);

  bool handleIdentity(DagNode* metaIdentity, MetaModule* m, BinarySymbol* s);
  bool handleSpecial(DagNode* metaHookList,
		     MetaModule* m,
		     Symbol* symbol,
		     const Vector<Sort*>& domainAndRange);
  bool downHook(DagNode* metaHook,
		MetaModule* m,
		Symbol* symbol,
		const Vector<Sort*>& domainAndRange);
  bool downOpHook(DagNode* metaOpHook, MetaModule* m, int& purpose, Symbol*& op);
  bool fixUpBubble(DagNode* metaHookList, MetaModule* m, int polymorphIndex);
  //
  //	Polymorph fixups.
  //
  bool handleIdentity(DagNode* metaIdentity,
		      MetaModule* m,
		      int index,
		      const Vector<Sort*>& domainAndRange);
  bool handleSpecial(DagNode* metaHookList, MetaModule* m, int polymorphIndex);
  bool downHook(DagNode* metaHook, MetaModule* m, int index);
  //
  //	Metalevel signature (generated by macro expansion).
  //
#define MACRO(SymbolName, SymbolClass, RequiredFlags, NrArgs) \
  SymbolClass* SymbolName;
#include "metaLevelSignature.cc"
#undef MACRO

  CachedDag trueTerm;
  CachedDag falseTerm;
  MetaModuleCache cache;
};

inline
MetaLevel::AttributeInfo::AttributeInfo()
{
  prec = DEFAULT;
  identity = 0;
  fixUpInfo = 0;
}

inline DagNode*
MetaLevel::upGroup(const Vector<DagNode*>& args,
		   Symbol* emptyCase,
		   Symbol* multipleCase)
{
  int nrArgs = args.length();
  if (nrArgs == 0)
    return emptyCase->makeDagNode();
  else if (nrArgs == 1)
    return args[0];
  return multipleCase->makeDagNode(args);
}

#endif
