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
#include "succSymbol.hh"
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

  bool isNat(const DagNode* dagNode) const;
  const mpz_class& getNat(const DagNode* dagNode) const;

  void startVariableMapping(int varCounter, FreshVariableGenerator* varGenerator);
  void stopVariableMapping();

  DagNode* upNat(const mpz_class& nat);
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
  DagNode* upFailurePair();
  DagNode* upFailureTriple();
  DagNode* upResult4Tuple(DagNode* dagNode,
			  const Substitution& substitution,
			  const VariableInfo& variableInfo,
			  DagNode* metaContext,
			  MixfixModule* m);
  DagNode* upFailure4Tuple();

  DagNode* upUnificationPair(const Substitution& substitution,
			     const VariableInfo& variableInfo,
			     const mpz_class& variableIndex,
			     MixfixModule* m);
  DagNode* upUnificationTriple(const Substitution& substitution,
			       const VariableInfo& variableInfo,
			       const mpz_class& variableIndex,
			       MixfixModule* m);

  DagNode* upVariant(const Vector<DagNode*>& variant, 
		     const NarrowingVariableInfo& variableInfo,
		     const mpz_class& variableIndex,
		     const mpz_class& parentIndex,
		     bool moreInLayer,
		     MixfixModule* m);
  DagNode* upNoVariant(bool incomplete);

  DagNode* upUnificationPair(const Vector<DagNode*>& unifier,
			     const NarrowingVariableInfo& variableInfo,
			     const mpz_class& variableIndex,
			     MixfixModule* m);
  DagNode* upUnificationTriple(const Vector<DagNode*>& unifier,
			       const NarrowingVariableInfo& variableInfo,
			       const mpz_class& variableIndex,
			       MixfixModule* m);

  DagNode* upTrace(const RewriteSequenceSearch& state, MixfixModule* m);
  DagNode* upFailureTrace();

  DagNode* upContext(DagNode* dagNode,
		     MixfixModule* m,
		     DagNode* hole,
		     PointerMap& qidMap,
		     PointerMap& dagNodeMap);
  DagNode* upSubstitution(const Substitution& substitution,
			  const VariableInfo& variableInfo,
			  MixfixModule* m,
			  PointerMap& qidMap,
			  PointerMap& dagNodeMap);
  DagNode* upNoUnifierPair(bool incomplete);
  DagNode* upNoUnifierTriple(bool incomplete);
  DagNode* upNoMatchSubst();
  DagNode* upNoMatchPair();
  DagNode* upMatchPair(const Substitution& substitution,
		       const VariableInfo& variableInfo,
		       DagNode* dagNode,
		       DagNode* hole,
		       MixfixModule* m);
  DagNode* upSmtResult(DagNode* state,
		       const Substitution& substitution,
		       const VariableInfo& variableInfo,
		       const NatSet& smtVariables,
		       DagNode* constraint,
		       const mpz_class& variableNumber,
		       MixfixModule* m);
  DagNode* upSmtFailure();


  DagNode* upView(View* view, PointerMap& qidMap);
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
  DagNode* upTypeListSet(const Vector<OpDeclaration>& opDecls,
			 const NatSet& chosenDecls,
			 PointerMap& qidMap);

  bool downBound(DagNode* metaBound, int& bound) const;
  bool downSaturate(DagNode* metaBound, int& bound) const;
  bool downBound64(DagNode* metaBound, Int64& bound) const;
  bool downSaturate64(DagNode* metaBound, Int64& bound) const;
  bool downPrintOptionSet(DagNode* metaPrintOptionSet, int& printFlags) const;
  bool downBool(DagNode* metaBool, bool& value);
  bool downQid(DagNode* metaQid, int& id);
  bool downOpName(DagNode* metaQid, int& id);
  MetaModule* downModule(DagNode* metaModule, bool cacheMetaModule = true, Interpreter* owner = 0);
  bool downTermAndSort(DagNode* metaTerm ,
		       DagNode* metaSort , 
		       Term*& term ,
		       Sort*& sort ,
		       MixfixModule* m);
  bool downUnificationProblem(DagNode* metaUnificationProblem,
			      Vector<Term*>& leftHandSides,
			      Vector<Term*>& rightHandSides,
			      MixfixModule* m,
			      bool makeDisjoint);
  bool downUnificandPair(DagNode* metaUnificandPair,
			 Term*& lhs,
			 Term*& rhs,
			 MixfixModule* m,
			 bool makeDisjoint);
  bool downTermPair(DagNode* metaTerm1,
		    DagNode* metaTerm2, 
		    Term*& term1,
		    Term*& term2,
		    MixfixModule* m,
		    bool makeDisjoint = false);
  Term* downTerm(DagNode* metaTerm, MixfixModule* m);
  bool downTermList(DagNode* metaTermList, MixfixModule* m, Vector<Term*>& termList);
  bool downCondition(DagNode* metaCondition,
		     MixfixModule* m,
		     Vector<ConditionFragment*>& condition);
  bool downSimpleSort(DagNode* metaSort, MixfixModule* m, Sort*& sort);
  bool downType(DagNode* metaType, MixfixModule* m, Sort*& type);
  bool downQidList(DagNode* metaQidList, Vector<int>& ids);
  bool downTypeList(DagNode* metaTypeList, MixfixModule* m, Vector<Sort*>& typeList);
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
    OWISE = 2,
    PRINT = 4,
    VARIANT = 8
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
    int metadata;
    DagNode* identity;
    DagNode* fixUpInfo;
  };

  struct StatementAttributeInfo
  {
    StatementAttributeInfo();

    FlagSet flags;
    int label;
    int metadata;
    Vector<int> printNames;
    Vector<Sort*> printSorts;
  };

  static int iterToken(DagNode* dagNode);
  static DagNode* upGroup(const Vector<DagNode*>& args,
			  Symbol* emptyCase,
			  Symbol* multipleCase);
  static void convertToTokens(const Vector<int>& ids, Vector<Token>& tokens);

  DagNode* upQid(int id, PointerMap& qidMap);
  DagNode* upJoin(int id, Sort* sort, char sep, PointerMap& qidMap);
  DagNode* upConstant(int id, Sort* sort, PointerMap& qidMap);
  DagNode* upConstant(int id, DagNode* d, PointerMap& qidMap);
  DagNode* upVariable(int id, Sort* sort, PointerMap& qidMap);
  DagNode* upTerm(const Term* term, MixfixModule* m, PointerMap& qidMap);
  DagNode* upSMT_Number(const mpq_class& value, Symbol* symbol, MixfixModule* m, PointerMap& qidMap);

  DagNode* upAssignment(const Term* variable,
			DagNode* value,
			MixfixModule* m,
			PointerMap& qidMap,
			PointerMap& dagNodeMap);

  DagNode* upAssignment(DagNode* variable,
			DagNode* value,
			MixfixModule* m,
			PointerMap& qidMap,
			PointerMap& dagNodeMap);

  DagNode* upHeader(bool flat, PreModule* pm, PointerMap& qidMap);
  DagNode* upParameterDecls(PreModule* pm, PointerMap& qidMap);
  DagNode* upParameterDecl(PreModule* pm, int index, PointerMap& qidMap);
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
  DagNode* upArguments(const Vector<Token>& arguments, PointerMap& qidMap);
  DagNode* upRenaming(const Renaming* r, PointerMap& qidMap);
  DagNode* upTypeSorts(const set<int>& sorts, PointerMap& qidMap);
  DagNode* upRenamingAttributeSet(const Renaming* r, int index, PointerMap& qidMap);
  DagNode* upSortMappings(View* view, PointerMap& qidMap);
  DagNode* upOpMappings(View* view, PointerMap& qidMap);

  DagNode* upTraceStep(const RewriteSequenceSearch& state,
		       int stateNr,
		       MixfixModule* m,
		       PointerMap& qidMap,
		       PointerMap& dagNodeMap);

  DagNode* upTypeList(const Vector<Sort*>& types,
		      bool omitLast,
		      PointerMap& qidMap);

  void upDisjointSubstitutions(const Substitution& substitution,
			       const VariableInfo& variableInfo,
			       MixfixModule* m,
			       PointerMap& qidMap,
			       PointerMap& dagNodeMap,
			       DagNode*& left,
			       DagNode*& right);

  DagNode* upSubstitution(const Vector<DagNode*>& substitution,
			  const NarrowingVariableInfo& variableInfo,
			  int nrVariables,
			  MixfixModule* m,
			  PointerMap& qidMap,
			  PointerMap& dagNodeMap);

  void upDisjointSubstitutions(const Vector<DagNode*>& unifier,
			       const NarrowingVariableInfo& variableInfo,
			       MixfixModule* m,
			       PointerMap& qidMap,
			       PointerMap& dagNodeMap,
			       DagNode*& left,
			       DagNode*& right);

  DagNode* upSmtSubstitution(const Substitution& substitution,
			     const VariableInfo& variableInfo,
			     const NatSet& smtVariables,
			     MixfixModule* m,
			     PointerMap& qidMap,
			     PointerMap& dagNodeMap);

  bool downHeader(DagNode* metaHeader, int& id, DagNode*& metaParameterDeclList);
  bool downParameterDeclList(DagNode* metaParameterDeclList, ImportModule* m);
  bool downParameterDecl(DagNode* metaParameterDecl, ImportModule* m);

  bool downModuleExpression(DagNode* metaExpr, ImportModule* enclosingModule, ImportModule*& m);
  bool downRenamings(DagNode* metaRenamings, Renaming* renaming);
  bool downRenaming(DagNode* metaRenaming, Renaming* renaming);
  bool downRenamingTypes(DagNode* metaTypes, Renaming* renaming);
  bool downRenamingType(DagNode* metaType, Renaming* renaming);
  bool downRenamingAttributes(DagNode* metaRenamingAttributes, Renaming* renaming);
  bool downRenamingAttribute(DagNode* metaRenamingAttribute, Renaming* renaming);
  
  bool downVariable(DagNode* metaVariable, MixfixModule* m, Symbol*& vs);
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

  bool downPrintOption(DagNode* metaPrintOption, int& printFlags) const;

  void checkHookList(DagNode* metaHookList, SymbolType& symbolType);
  void checkHook(DagNode* metaIdHook, SymbolType& symbolType);
 
  bool downConditionFragment(DagNode* metaConditionFragment,
			     MixfixModule* m,
			     ConditionFragment*& fragment);

  bool downStatementAttrSet(DagNode* metaAttrSet, MixfixModule* m, StatementAttributeInfo& ai);
  bool downStatementAttr(DagNode* metaAttr, MixfixModule* m, StatementAttributeInfo& ai);
  bool downPrintList(DagNode* metaPrintList, MixfixModule* m, StatementAttributeInfo& ai);
  bool downPrintListItem(DagNode* metaPrintListItem, MixfixModule* m, StatementAttributeInfo& ai);

  bool downMembAxs(DagNode* metaMembAxs, MixfixModule* m);
  bool downMembAx(DagNode* metaMembAx, MixfixModule* m);
  bool downEquations(DagNode* metaEquations, MixfixModule* m);
  bool downEquation(DagNode* metaEquation, MixfixModule* m);
  bool downRules(DagNode* metaRules, MixfixModule* m);
  bool downRule(DagNode* metaRule, MixfixModule* m);
  bool downInstantiationArguments(DagNode* metaArguments, Vector<int>& arguments);
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
  //
  //	Settings to modify behavior of methods.
  //
  bool flagVariables;
  int variableBase;
  FreshVariableGenerator* variableGenerator;
};

inline
MetaLevel::AttributeInfo::AttributeInfo()
{
  prec = DEFAULT;
  metadata = NONE;
  identity = 0;
  fixUpInfo = 0;
}

inline
MetaLevel::StatementAttributeInfo::StatementAttributeInfo()
{
  label = NONE;
  metadata = NONE;
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

inline bool
MetaLevel::isNat(const DagNode* dagNode) const
{
  return succSymbol->isNat(dagNode);
}

inline const mpz_class&
MetaLevel::getNat(const DagNode* dagNode) const
{
  return succSymbol->getNat(dagNode);
}

inline DagNode*
MetaLevel::upNat(const mpz_class& nat)
{
  return succSymbol->makeNatDag(nat);
}

inline void
MetaLevel::startVariableMapping(int varBase, FreshVariableGenerator* varGenerator)
{
  variableBase = varBase;
  variableGenerator = varGenerator;
}

inline void
MetaLevel::stopVariableMapping()
{
  variableGenerator = 0;
}

#endif
