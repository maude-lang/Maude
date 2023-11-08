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
//      Class for modules with mixfix syntax.
//
#ifndef _mixfixModule_hh_
#define _mixfixModule_hh_
#include <gmpxx.h>
#include <set>
#include <map>
#include <list>
#include "profileModule.hh"
#include "metadataStore.hh"
#include "sharedTokens.hh"
#include "freeTheory.hh"
#include "builtIn.hh"
#include "token.hh"
#include "pointerSet.hh"
#include "symbolType.hh"
#include "SMT_Info.hh"
#include "SMT_NumberSymbol.hh"
#include "statementTransformer.hh"

class MixfixModule : public ProfileModule, public MetadataStore, protected SharedTokens
{
  NO_COPYING(MixfixModule);

  enum Bits
  {
    SYSTEM = 1,
    THEORY = 2,
    STRATEGY = 4,
    OBJECT_ORIENTED = 8  // a fiction for SyntacticPreModule and for us during construction
  };

public:
  enum ModuleType
  {
    FUNCTIONAL_MODULE = 0,
    SYSTEM_MODULE = SYSTEM,
    STRATEGY_MODULE = SYSTEM | STRATEGY,
    FUNCTIONAL_THEORY = THEORY,
    SYSTEM_THEORY = SYSTEM | THEORY,
    STRATEGY_THEORY = SYSTEM | STRATEGY | THEORY,
    //
    //	These types only exist during construction; once the module is finished
    //	the object oriented flag has to be removed.
    //
    OBJECT_ORIENTED_MODULE = SYSTEM | OBJECT_ORIENTED,
    OBJECT_ORIENTED_THEORY = SYSTEM | OBJECT_ORIENTED | THEORY
  };

  enum GatherSymbols
  {
    GATHER_e = -1,
    GATHER_E = 0,
    GATHER_AMP = 1
  };

  typedef map<int, Sort*> AliasMap;

  MixfixModule(int name, ModuleType moduleType);
  ~MixfixModule();

  void closeSortSet();
  void closeSignature();
  void economize();
  void processingComplete();
  //
  //	Functions to insert stuff.
  //
  void insertPotentialLabels(const set<int>& l);
  void insertPotentialRuleLabels(const set<int>& rls);
  Sort* addSort(int name);
  Symbol* addOpDeclaration(Token prefixName,
			   const Vector<Sort*>& domainAndRange,
			   SymbolType symbolType,
			   const Vector<int>& strategy,
			   const NatSet& frozen,
			   int prec,
			   const Vector<int>& gather,
			   const Vector<int>& format,
			   int metadata,
			   bool& firstDecl);
  void addVariableAlias(Token name, Sort* sort);
  int addPolymorph(Token prefixName,
		   const Vector<Sort*>& domainAndRange,
		   SymbolType symbolType,
		   const Vector<int>& strategy,
		   const NatSet& frozen,
		   int prec,
		   const Vector<int>& gather,
		   const Vector<int>& format,
		   int metadata);
  int addStrategy(Token name,
		  const Vector<Sort*>& domainSorts,
		  Sort* subjectSort,
		  int metadata = NONE,
		  bool imported = false);
  void addIdentityToPolymorph(int polymorphIndex,
			      Term* identity);
  void addIdHookToPolymorph(int polymorphIndex,
			    int purpose,
			    const Vector<int>& data);
  void addOpHookToPolymorph(int polymorphIndex,
			    int purpose,
			    Symbol* symbol);
  void addTermHookToPolymorph(int polymorphIndex,
			      int purpose,
			      Term* term);
  //
  //	Allow a temporary change op variable aliases for the benefit of processing
  //	op->term mappings in views.
  //
  void swapVariableAliasMap(AliasMap& other, MixfixParser*& otherParser);
  //
  //	Functions to make things.
  //
  Symbol* instantiateSortTest(Sort* sort, bool eager);
  Symbol* instantiateVariable(Sort* sort);
  Symbol* instantiatePolymorph(int polymorphIndex, int kindIndex);
  Term* makeTrueTerm();
  Term* makeBubble(int bubbleSpecIndex, const Vector<Token>& tokens, int first, int last);
  DagNode* makeUnificationProblemDag(Vector<Term*>& lhs, Vector<Term*>& rhs);
  pair<DagNode*, DagNode*> makeMatchProblemDags(Vector<Term*>& lhs, Vector<Term*>& rhs);
  
  static void printCondition(ostream& s, const Vector<ConditionFragment*>& condition);
  static void printCondition(ostream& s, const PreEquation* pe);
  void printAttributes(ostream& s, const PreEquation* pe, ItemType itemType);
  void printStrategyTerm(ostream& s, RewriteStrategy* strat, Term* term);
  //
  //	Parsing functions.
  //
  Term* parseTerm(const Vector<Token>& bubble,
		  ConnectedComponent* component = 0,
		  int begin = 0,
		  int end = DEFAULT);
  int parseTerm2(const Vector<Token>& bubble,
		 ConnectedComponent* component,
		 Term*& parse1,
		 Term*& parse2,
		 int& firstBad);
  StrategyExpression* parseStrategyExpr(const Vector<Token>& bubble,
					int begin = 0,
					int end = DEFAULT);
  int parseStrategyExpr2(const Vector<Token>& bubble,
			 StrategyExpression*& parse1,
			 StrategyExpression*& parse2,
			 int& firstBad);
  void parseStatement(const Vector<Token>& bubble);
  bool parseSearchCommand(const Vector<Token>& bubble,
			  Term*& initial,
			  int& searchType,
			  Term*& target,
			  Vector<ConditionFragment*>& condition);
  bool parseMatchCommand(const Vector<Token>& bubble,
			 Term*& pattern,
			 Term*& subject,
			 Vector<ConditionFragment*>& condition);
  bool parseUnifyCommand(const Vector<Token>& bubble,
			 Vector<Term*>& lhs,
			 Vector<Term*>& rhs);
  bool parseStrategyCommand(const Vector<Token>& bubble,
			    Term*& subject,
			    StrategyExpression*& strategy);
  bool parseGetVariantsCommand(const Vector<Token>& bubble,
			       Term*& initial,
			       Vector<Term*>& constraints);
  bool parseVariantUnifyCommand(const Vector<Token>& bubble,
				Vector<Term*>& lhs,
				Vector<Term*>& rhs,
				Vector<Term*>& constraints);
  bool parseVariantMatchCommand(const Vector<Token>& bubble,
				Vector<Term*>& lhs,
				Vector<Term*>& rhs,
				Vector<Term*>& constraints);
  //
  //	Get functions.
  //
  ModuleType getModuleType() const;
  static const char* moduleTypeString(ModuleType type);
  static const char* moduleEndString(ModuleType type);
  SymbolType getSymbolType(const Symbol* symbol) const;
  int getPolymorphIndex(const Symbol* symbol) const;
  int getPrec(const Symbol* symbol) const;
  void getGather(const Symbol* symbol, Vector<int>& gather) const;
  const Vector<int>& getFormat(const Symbol* symbol) const;
  const AliasMap& getVariableAliases() const;
  void getParserStats(int& nrNonterminals, int& nrTerminals, int& nrProductions);
  void getDataAttachments(Symbol* symbol,
			  const Vector<Sort*>& opDeclaration,
			  Vector<const char*>& purposes,
			  Vector<Vector<const char*> >& data) const;
  void getSymbolAttachments(Symbol* symbol,
			    Vector<const char*>& purposes,
			    Vector<Symbol*>& symbols) const;
  void getTermAttachments(Symbol* symbol,
			  Vector<const char*>& purposes,
			  Vector<Term*>& terms) const;
  Sort* getStrategyRangeSort() const;
  const NatSet& getObjectSymbols() const;
  //
  //	Find functions.
  //
  Sort* findSort(int name) const;
  RewriteStrategy* findStrategy(int name,
				const Vector<ConnectedComponent*>& domainComponents) const;
  
  Symbol* findFirstUnarySymbol(int name, const ConnectedComponent* rangeComponent) const;
  Symbol* findNextUnarySymbol(Symbol* previous, const ConnectedComponent* rangeComponent) const;
  Symbol* findSymbol(int name,
		     const Vector<ConnectedComponent*>& domainComponents,
		     ConnectedComponent* rangeComponent);
  
  QuotedIdentifierSymbol* findQuotedIdentifierSymbol(const ConnectedComponent* component) const;
  StringSymbol* findStringSymbol(const ConnectedComponent* component) const;
  FloatSymbol* findFloatSymbol(const ConnectedComponent* component) const;
  SMT_NumberSymbol* findSMT_NumberSymbol(const ConnectedComponent* component, SMT_Info::SMT_Type type);
  int findPolymorphIndex(int polymorphName, const Vector<Sort*>& domainAndRange) const;
  //
  //	Polymorph functions.
  //
  void copyFixUpPolymorph(int polymorphIndex,
			  const MixfixModule* originalModule,
			  int originalPolymorphIndex,
			  SymbolMap* map);
  int getNrPolymorphs() const;
  Token getPolymorphName(int index) const;
  SymbolType getPolymorphType(int index) const;
  const Vector<Sort*>& getPolymorphDomainAndRange(int index) const;
  Term* getPolymorphIdentity(int index) const;
  const Vector<int>& getPolymorphStrategy(int index) const;
  const NatSet& getPolymorphFrozen(int index) const;
  int getPolymorphPrec(int index) const;
  void getPolymorphGather(int index, Vector<int>& gather) const;
  const Vector<int>& getPolymorphFormat(int index) const;
  int getPolymorphMetadata(int index) const;
  bool getPolymorphDataAttachment(int index, int nr, int& purpose, Vector<int>& items) const;
  bool getPolymorphSymbolAttachment(int index, int nr, int& purpose, Symbol*& op) const;
  bool getPolymorphTermAttachment(int index, int nr, int& purpose, Term*& term) const;
  //
  //	Bubble functions.
  //
  int addBubbleSpec(Symbol* topSymbol,
		    int lowerBound,
		    int upperBound,
		    int leftParenToken,
		    int rightParenToken,
		    const Vector<int>& excludedTokens);
  void copyBubbleSpec(Symbol* originalSymbol, Symbol* newSymbol);
  void copyFixUpBubbleSpec(Symbol* originalSymbol, SymbolMap* map);
  void fixUpBubbleSpec(int bubbleSpecIndex,
		       Symbol* qidSymbol,
		       Symbol* nilQidListSymbol,
		       Symbol* qidListSymbol);
  //
  //	Pretty print functions.
  //
  void bufferPrint(Vector<int>& buffer, Term* term, int printFlags);
  void bufferPrint(Vector<int>& buffer, StrategyExpression* term, int printFlags);
  static Sort* disambiguatorSort(const Term* term);
  int getSMT_NumberToken(const mpq_class& value, Sort* sort);
  void printModifiers(ostream& s, Int64 number = NONE, Int64 number2 = NONE);
  static void prettyPrint(ostream& s, const Term* term, bool rangeKnown = false);
  //
  //	LaTeX conversion functions.
  //
  static string latexStructuredName(const Vector<int>& codes, const Module* m);
  static string latexSort(int code, const Module* module);
  static string latexSort(const Sort* sort);
  static string latexType(const Sort* sort);
  string latexStructuredConstant(int code) const;
  static string latexPrettyOp(int code);
  static string latexConstant(int code, const Module* module);
  static string latexTokenVector(const Vector<Token>& tokens, Index first, Index last);

  //
  //	Latex pretty print functions.
  //
  static void latexPrettyPrint(ostream& s, DagNode* dagNode);
  static void latexPrettyPrint(ostream& s, Term* term, bool rangeKnown = false);
  static void latexPrintDagNode(ostream& s, DagNode* dagNode);
  static void latexPrintGather(ostream& s, const Vector<int>& gather);
  static void latexPrintFormat(ostream& s, const Vector<int>& format);
  //
  //	Misc.
  //
  static Sort* hookSort(Sort* sort);
  static ModuleType join(ModuleType t1, ModuleType t2);
  static bool isObjectOriented(ModuleType t);
  bool isObjectOriented() const;
  static bool isTheory(ModuleType t);
  bool isTheory() const;
  static bool isStrategic(ModuleType t);
  bool isSystem() const;
  static bool isSystem(ModuleType t);
  bool isStrategic() const;
  static bool canImport(ModuleType t1, ModuleType t2);
  static bool canHaveAsParameter(ModuleType t1, ModuleType t2);
  const SMT_Info& getSMT_Info();
  bool validForSMT_Rewriting();
  void checkFreshVariableNames();
  static Term* findNonlinearVariable(Term* term, VariableInfo& variableInfo);
  Symbol* findSMT_Symbol(Term* term);

  Rope serialize(DagNode* dagNode);
  DagNode* deserialize(const Rope& encoding);

  //
  //	This functionality is to enable statements to be transformed after they
  //	are generated by the parser, but before they are entered into the module.
  //
  void handleSortConstraint(SortConstraint* sortConstraint, bool dnt);
  void handleEquation(Equation* equation, bool dnt);
  void handleRule(Rule* rule, bool dnt);
  void installStatementTransformer(StatementTransformer* st);
  
protected:
  static int findMatchingParen(const Vector<Token>& tokens, int pos);

  // We need to account rule labels here (before importing statements) because
  // strategy statements can use them
  set<int> ruleLabels;

public:  // HACK
  enum Precedence
  {
    MIN_PREC = 0,
    MAX_PREC = 127,
    ANY = 127,
    PREFIX_PREC = 0,
    PREFIX_GATHER = 95,		// to allow _,_ to work correctly
    UNARY_PREC = 15,		// backward compatibility with OBJ3 defaults
    INFIX_PREC = 41,		// backward compatibility with OBJ3 defaults
    //
    //	Precedences for complex syntax
    //
    ASSIGNMENT_PREC = 75,
    //
    //	Precedences for strategy language.
    //
    STRAT_BASIC_PREC = 0,
    STRAT_TEST_PREC = 21,
    STRAT_REW_PREC = 21,
    STRAT_SEQ_PREC = 39,
    STRAT_UNION_PREC = 41,
    STRAT_ORELSE_PREC = 43,
    STRAT_BRANCH_PREC = 55,
    STRAT_USING_LIST_PREC = 61,
    STRAT_USING_PREC = 21
  };

private: 
  enum InternalFlags
  {
    LEFT_BARE = 0x1,		// operator name has leading argument underscore
    RIGHT_BARE = 0x2,		// operator name has trailing argument underscore
    ADHOC_OVERLOADED = 0x4,	// another operator with the same name exists
    DOMAIN_OVERLOADED = 0x8,	// another operator with the same name and domain kinds exists
    RANGE_OVERLOADED = 0x10,	// another operator with the same name and range kind exists
    
    PSEUDO_STRING = 0x40,	// nullary operator looks like a built-in string
    PSEUDO_QUOTED_IDENTIFIER = 0x80,	// nullary operator looks like a built-in quoted identifier
    PSEUDO_FLOAT = 0x100,	// nullary operator looks like a built-in float
    PSEUDO_VARIABLE = 0x200,	// nullary operator looks like a explicit variable (i.e. X:Foo)

    PSEUDO_NAT = 0x1000,	// nullary operator looks like a positive integer
    PSEUDO_NEG = 0x2000,	// nullary operator looks like a negative integer
    PSEUDO_RAT = 0x4000,	// nullary operator looks like a rational
    PSEUDO_ZERO = 0x8000,	// nullary operator looks like zero integer
    
    PSEUDOS = PSEUDO_VARIABLE | PSEUDO_FLOAT | PSEUDO_STRING | PSEUDO_QUOTED_IDENTIFIER |
      PSEUDO_NAT | PSEUDO_NEG | PSEUDO_RAT | PSEUDO_ZERO
  };

  enum NonTerminal
  {
    //
    //	Nonterminals needed to parse modules and simple commands.
    //
    TERM = -1,
    SMALL_NAT = -2,
    SMALL_NEG = -3,
    QUOTED_ID = -4,
    STRING_NT = -5,
    FLOAT_NT = -6,
    ENDS_IN_COLON_NT = -7,
    
    COLON_PAIR = -8,
    COLON2_PAIR = -9,
    COLON3_PAIR = -10,
    EQUALITY_PAIR = -11,
    ARROW_PAIR = -12,
    ASSIGN_PAIR = -13,

    LABEL = -14,
    RULE_LABEL = -15,
    CONDITION_FRAGMENT = -16,
    RULE_CONDITION_FRAGMENT = -17,
    CONDITION = -18,
    RULE_CONDITION = -19,

    MB_BODY = -20,
    EQ_BODY = -21,
    RL_BODY = -22,
    
    STATEMENT_PART = -23,
    ATTRIBUTE_PART = -24,
    ATTRIBUTE_LIST = -25,
    ATTRIBUTE = -26,
    STATEMENT = -27,

    ZERO = -28,
    RATIONAL = -29,

    VARIABLE = -30,
    PRINT_ITEM = -31,
    PRINT_LIST = -32,

    SIMPLE_BASE = -33,
    //
    //	Extra nonterminals needed to parse complex commands.
    //
    SEARCH_CONNECTIVE = SIMPLE_BASE,
    MATCH_PAIR = SIMPLE_BASE - 1,
    SEARCH_PAIR = SIMPLE_BASE - 2,
    SUCH_THAT = SIMPLE_BASE - 3,
    STRATEGY_EXPRESSION = SIMPLE_BASE - 4,
    STRATEGY_CALL_EXPRESSION = SIMPLE_BASE - 5,

    MATCH_COMMAND = SIMPLE_BASE - 6,
    SEARCH_COMMAND = SIMPLE_BASE - 7,
    STRATEGY_COMMAND = SIMPLE_BASE - 8,

    ASSIGNMENT = SIMPLE_BASE - 9,
    SUBSTITUTION = SIMPLE_BASE - 10,

    STRATEGY_LIST = SIMPLE_BASE - 11,

    UNIFY_PAIR = SIMPLE_BASE - 12,
    UNIFY_COMMAND = SIMPLE_BASE - 13,

    GET_VARIANTS_COMMAND = SIMPLE_BASE - 14,
    TERM_LIST = SIMPLE_BASE - 15,  // for command separated list of hetrogeneous terms

    VARIANT_UNIFY_COMMAND = SIMPLE_BASE - 16,
    VARIANT_MATCH_COMMAND = SIMPLE_BASE - 17,
    MULTI_MATCH_COMMAND = SIMPLE_BASE - 18,

    USING_PAIR = SIMPLE_BASE - 19,
    USING_LIST = SIMPLE_BASE - 20,

    SD_BODY = SIMPLE_BASE - 21,
    STRATEGY_PAIR = SIMPLE_BASE - 22,

    COMPLEX_BASE = SIMPLE_BASE - 23
  };

  enum NonTerminalType
  {
    NUMBER_OF_TYPES = 5,
    TERM_TYPE = 0,
    SORT_TYPE = 1,
    DOT_SORT_TYPE = 2,
    ASSOC_LIST_TYPE = 3,
    SORT_LIST_TYPE = 4
  };

  enum SMT_Status
    {
      UNCHECKED = -1,
      BAD = 0,
      GOOD = 1
    };

  struct SymbolInfo
  {
    void revertGather(Vector<int>& gatherSymbols) const;

    Vector<int> mixfixSyntax;
    Vector<int> gather;
    Vector<int> format;
    short prec;
    short polymorphIndex;  // for polymorphs and polymorph instances only
    SymbolType symbolType;
    int iflags;  // internal flags; mostly information about potential overloading
    int next;
  };

  struct IdHook
  {
    int purpose;
    Vector<int> data;
  };

  struct OpHook
  {
    int purpose;
    Symbol* symbol;
  };

  struct TermHook
  {
    int purpose;
    Term* term;
  };

  struct Polymorph
  {
    Token name;
    Vector<Sort*> domainAndRange;
    Vector<int> strategy;
    NatSet frozen;
    Term* identity;
    int metadata;
    Vector<IdHook> idHooks;
    Vector<OpHook> opHooks;
    Vector<TermHook> termHooks;
    Vector<Symbol*> instantiations;
    SymbolInfo symbolInfo;
  };

  //
  //	Information needed for node coloring in pretty printer.
  //
  enum DagNodeStatus
  {
    CONSTRUCTOR = 1,
    STRANGENESS_BELOW = 2
  };

  struct ColoringInfo
  {
    PointerSet visited;		// set of all nodes visited on 1st pass
    Vector<int> statusVec;	// status computed on 1st pass
    bool reducedAbove;		// updated on 2nd pass
    bool reducedDirectlyAbove;  // updated on 2nd pass
  };

  int nonTerminal(int componentIndex, NonTerminalType type);
  int nonTerminal(const Sort* sort, NonTerminalType type);

  static int domainComponentIndex(const Symbol* symbol, int argNr);
  static int mayAssoc(Symbol* symbol, int argNr);

  Symbol* newFancySymbol(Token prefixName,
			 const Vector<Sort*>& domainAndRange,
			 SymbolType symbolType,
			 const Vector<int>& strategy);
  Symbol* newAssociativeSymbol(int name,
			       const Vector<Sort*>& domainAndRange,
			       SymbolType symbolType,
			       const Vector<int>& strategy);
  Symbol* newAxiomSymbol(int name,
			 const Vector<Sort*>& domainAndRange,
			 SymbolType symbolType,
			 const Vector<int>& strategy);
  void validateAttributes(Token prefixName,
			  const Vector<Sort*>& domainAndRange,
			  SymbolType& symbolType);

  void makeGrammar(bool complexFlag = false);
  void makeComplexProductions();
  void makeStrategyLanguageProductions();
  void makeStatementProductions();
  void makeConditionProductions();
  void makeAttributeProductions();
  void makeParameterizedSortProductions();
  void makeComponentProductions();
  void makeSymbolProductions();
  void makeVariableProductions();
  void makeBoolProductions();
  void makeLabelProductions();
  void makeSpecialProductions();
  void makePolymorphProductions();
  void makeBubbleProductions();
  void computePrecAndGather(int nrArgs, SymbolInfo& si, Symbol* symbol = 0);

  static Term* findNonlinearVariable(Term* term, NatSet& seenIndices);

  void printVariable(ostream& s, int name, Sort* sort) const;
  void graphPrint(ostream& s, DagNode* dagNode);

  static void printPrefixName(ostream& s, const char* prefixName, SymbolInfo& si);
  static int printTokens(ostream& s,
			 const SymbolInfo& si,
			 int pos,
			 const char* color = 0);
  static void printTails(ostream& s,
			 const SymbolInfo& si,
			 int pos,
			 int nrTails,
			 bool needAssocParen,
			 bool checkForInterrupt = false,
			 const char* color = 0);
  static bool fancySpace(ostream& s, int spaceToken);
  static int chooseDisambiguator(Symbol* s);
  static void graphCount(DagNode* dagNode, PointerSet& visited, Vector<mpz_class>& counts);
  static void makeIterName(string& name, int id, const mpz_class& number);
  static int computeGraphStatus(DagNode* dagNode,
				PointerSet& visited,
				Vector<int>& statusVec);
  //
  //	Member functions for Term* -> Vector<int>&  pretty printer.
  //
  static void prefix(Vector<int>& buffer, bool needDisambig);
  static void suffix(Vector<int>& buffer, Term* term, bool needDisambig, int printFlags);
  void prettyPrint(Vector<int>& buffer,
		   Term* term,
		   int requiredPrec,
		   int leftCapture,
		   const ConnectedComponent* leftCaptureComponent,
		   int rightCapture,
		   const ConnectedComponent* rightCaptureComponent,
		   bool rangeKnown,
		   int printFlags);

  void handleVariable(Vector<int>& buffer, Term* term, int printFlags);
  bool handleIter(Vector<int>& buffer, Term* term, SymbolInfo& si, bool rangeKnown, int printFlags);
  bool handleMinus(Vector<int>& buffer, Term* term, bool rangeKnown, int printFlags);
  bool handleDivision(Vector<int>& buffer, Term* term, bool rangeKnown, int printFlags);
  void handleFloat(Vector<int>& buffer, Term* term, bool rangeKnown, int printFlags);
  void handleString(Vector<int>& buffer, Term* term, bool rangeKnown, int printFlags);
  void handleQuotedIdentifier(Vector<int>& buffer, Term* term, bool rangeKnown, int printFlags);
  void handleSMT_NumberSymbol(Vector<int>& buffer, Term* term, bool rangeKnown, int printFlags);

  static void printKind(Vector<int>& buffer, const Sort* kind, int printFlags);
  static void printSort(Vector<int>& buffer, const Sort* sort, int printFlags);
  static void printDotSort(Vector<int>& buffer, const Sort* sort, int printFlags);
  static void printVarSort(Vector<int>& buffer, string& fullName, const Sort* sort, int printFlags);

  int printTokens(Vector<int>& buffer, const SymbolInfo& si, int pos, int printFlags);
  void printTails(Vector<int>& buffer,
		  const SymbolInfo& si,
		  int pos,
		  int nrTails,
		  bool needAssocParen,
		  int printFlags);
  void printPrefixName(Vector<int>& buffer, int prefixName, SymbolInfo& si, int printFlags);
  void handleFormat(Vector<int>& buffer, int spaceToken);

  //
  //	Member functions for StrategyExpression* -> Vector<int>&  pretty printer.
  //
  void prettyPrint(Vector<int>& buffer,
		   StrategyExpression* term,
		   int requiredPrec,
		   int printFlags);
  void prettyPrint(Vector<int>& buffer,
		   const Vector<ConditionFragment*>& condition,
		   int printFlags);
  void prettyPrint(Vector<int>& buffer,
		   const ConditionFragment* fragment,
		   int printFlags);

  static Vector<int> emptyGather;
  static Vector<int> gatherAny;
  static Vector<int> gatherAnyAny;
  static Vector<int> gatherAnyAnyAny;
  static Vector<int> gatherAny4;
  static Vector<int> gatherPrefix;
  static Vector<int> gatherPrefixPrefix;
  static Vector<int> gatherAny0;
  static int globalIndent;
  static bool attributeUsed;

  ModuleType moduleType;
  Sort* boolSort;
  Sort* strategyRangeSort;
  FreeSymbol* trueSymbol;
  FreeSymbol* falseSymbol;

  Vector<Symbol*> variableBase;  // for each sort
  Vector<Symbol*> eagerSortTests;
  Vector<Symbol*> lazySortTests;

  AliasMap variableAliases;

  Vector<SymbolInfo> symbolInfo;
  Vector<Polymorph> polymorphs;
  //
  //	Mixfix parsing stuff.
  //
  int parseSentence(const Vector<Token>& bubble,
		    int root,
		    int begin = 0,
		    int end = DEFAULT);
  int parseSentence2(const Vector<Token>& bubble,
		     int root,
		     int& firstBad,
		     int begin,
		     int nrTokens);
  Term* makeTerm(int node);
  void makeStatement(int node);
  void makeCondition(int node, Vector<ConditionFragment*>& condition);
  ConditionFragment* makeConditionFragment(int node);
  void makeCommand(int node, Vector<Term*>& terms);
  MixfixParser* parser;
 //
  //	These two data structures are filled out during the construction
  //	of a grammar and cleared immediately after. Thus they need not
  //	be preserved during parser swapping.
  //
  typedef map<int, int> IntMap;
  IntMap iterSymbols;  // maps from name code to unique nonterminals
  IntMap leadTokens;  // maps from lead tokens of structured sort to unique nonterminals

  typedef map<int, Sort*> SortMap;
  SortMap sortNames;  // maps from name codes to sort pointers

  IntMap firstSymbols;  // names from name codes to index of first symbol with that name

  struct BubbleSpec
  {
    QuotedIdentifierSymbol* qidSymbol;
    Symbol* nilQidListSymbol;
    Symbol* qidListSymbol;
    Symbol* topSymbol;
    int lowerBound;
    int upperBound;
    int leftParenToken;
    int rightParenToken;
    int componentIndex;
    Vector<int> excludedTokens;
  };

  int findBubbleSpecIndex(Symbol* topSymbol) const;

  set<int> potentialLabels;
  set<int> bubbleComponents;
  Vector<BubbleSpec> bubbleSpecs;

  //
  //	These sets and maps are needed to decided when to
  //	disambiguate by sort in the pretty printer and to
  //	find builtin symbols from their kind index.
  //
  set<double> overloadedFloats;
  map<int, Symbol*> floatSymbols;
  set<string> overloadedStrings;
  map<int, Symbol*> stringSymbols;
  set<int> overloadedQuotedIdentifiers;
  map<int, Symbol*> quotedIdentifierSymbols;
  map<int, Symbol*> SMT_NumberSymbols;
  //
  //	We keep sets of integers and rationals whose decimal i/o
  //	representation has been used for user symbols of arity 0.
  //
  set<mpz_class> overloadedIntegers;
  set<pair<mpz_class, mpz_class> > overloadedRationals;
  //
  //	We also keep sets of kinds that have succ, minus and division
  //	symbols defined on them, or that have an non-explicit zero.
  //
  set<int> kindsWithSucc;  // also holds kinds with built-in constants that look like positive integers
  set<int> kindsWithMinus;  // also holds kinds with built-in constants that look like negative integers
  set<int> kindsWithDivision;  // also holds kinds with built-in that look like rationals
  set<int> kindsWithZero;   // holds kinds with a built-in constnat that look like zero

  set<pair<int, int> > overloadedVariables;

  //
  //	We keep track of symbols whose name looks like iterated notation
  //
  typedef multimap<mpz_class, Symbol*> NumberToSymbolMap;
  typedef map<int, NumberToSymbolMap> PseudoIteratedMap;
  PseudoIteratedMap pseudoIteratedMap;
  //
  //	We also keep track of iterated symbols.
  //
  typedef multimap<int, Symbol*> IteratedMap;
  IteratedMap iteratedMap;

  static bool hasSameDomain(const Vector<Sort*>& domainAndRange1,
			    bool assoc1,
			    const Vector<Sort*>& domainAndRange2,
			    bool assoc2);
  static bool domainAndRangeMatch(const Vector<Sort*>& domainAndRange1,
				  const Vector<Sort*>& domainAndRange2);

  int visit(DagNode* dagNode, PointerSet& visited, Rope& accumulator);

  bool ambiguous(int iflags) const;
  static bool rangeOfArgumentsKnown(int iflags, bool rangeKnown, bool rangeDisambiguated);
  void decideIteratedAmbiguity(bool rangeKnown,
			       Symbol* symbol,
			       const mpz_class& number,
			       bool& needToDisambiguate,
			       bool& argumentRangeKnown) const;
  int checkPseudoIterated(Symbol* symbol, const Vector<Sort*>& domainAndRange);
  void checkIterated(Symbol* symbol, const Vector<Sort*>& domainAndRange);
  //
  //	Member functions for DagNode* -> ostream& pretty printer.
  //
  const char* computeColor(ColoringInfo& coloringInfo, DagNode* dagNode);
  static void prefix(ostream& s, bool needDisambig, const char* color);
  static void suffix(ostream& s, DagNode* dagNode, bool needDisambig, const char* color);
  void handleFloat(ostream& s,
		   DagNode* dagNode,
		   bool rangeKnown,
		   const char* color);
  void handleString(ostream& s,
		    DagNode* dagNode,
		    bool rangeKnown,
		    const char* color);
  void handleQuotedIdentifier(ostream& s,
			      DagNode* dagNode,
			      bool rangeKnown,
			      const char* color);
  void handleVariable(ostream& s,
		      DagNode* dagNode,
		      bool rangeKnown,
		      const char* color);
  bool handleIter(ostream& s,
		  ColoringInfo& coloringInfo,
		  DagNode* dagNode,
		  SymbolInfo& si,
		  bool rangeKnown,
		  const char* color);
  bool handleMinus(ostream& s,
		   DagNode* dagNode,
		   bool rangeKnown,
		   const char* color);
  bool handleDivision(ostream& s,
		      DagNode* dagNode,
		      bool rangeKnown,
		      const char* color);
  void handleSMT_Number(ostream& s,
			DagNode* dagNode,
			bool rangeKnown,
			const char* color);
  void prettyPrint(ostream& s,
		   ColoringInfo& coloringInfo,
		   DagNode* dagNode,
		   int requiredPrec,
		   int leftCapture,
		   const ConnectedComponent* leftCaptureComponent,
		   int rightCapture,
		   const ConnectedComponent* rightCaptureComponent,
		   bool rangeKnown);
  //
  //	Member functions for Term* -> ostream& pretty printer.
  //
  static const char* computeColor(SymbolType st);
  static void suffix(ostream& s, Term* term, bool needDisambig, const char* color);
  bool handleIter(ostream& s,
		  Term* term,
		  SymbolInfo& si,
		  bool rangeKnown,
		  const char* color);
  bool handleMinus(ostream& s, Term* term,
		   bool rangeKnown,
		   const char* color);
  bool handleDivision(ostream& s,
		      Term* term,
		      bool rangeKnown,
		      const char* color);
  void handleFloat(ostream& s,
		   Term* term,
		   bool rangeKnown,
		   const char* color);
  void handleString(ostream& s,
		    Term* term,
		    bool rangeKnown,
		    const char* color);
  void handleQuotedIdentifier(ostream& s,
			      Term* term,
			      bool rangeKnown,
			      const char* color);
  void handleVariable(ostream& s,
		      Term* term,
		      bool rangeKnown,
		      const char* color);
  void handleSMT_Number(ostream& s,
			Term* term,
			bool rangeKnown,
			const char* color);
  void prettyPrint(ostream& s,
		   Term* term,
		   int requiredPrec,
		   int leftCapture,
		   const ConnectedComponent* leftCaptureComponent,
		   int rightCapture,
		   const ConnectedComponent* rightCaptureComponent,
		   bool rangeKnown);

  //
  //	Common LaTeX functions.
  //
  static void latexPrintPrefixName(ostream& s, const char* prefixName, const SymbolInfo& si);
  static int latexPrintTokens(ostream& s, const SymbolInfo& si, int pos, const char* color);
  static void latexPrefix(ostream& s, bool needDisambig, const char* color);
  static void latexPrintTails(ostream& s,
			      const SymbolInfo& si,
			      int pos,
			      int nrTails,
			      bool needAssocParen,
			      bool checkForInterrupt,
			      const char* color);
  static bool latexFancySpace(ostream& s, int spaceToken);
  //
  //	Member functions for Term* -> LaTeX pretty printer.
  //
  static const char* latexComputeColor(SymbolType st);
  static void latexSuffix(ostream& s, Term* term, bool needDisambig, const char* color);
  bool latexHandleIter(ostream& s, Term* term, const SymbolInfo& si, bool rangeKnown, const char* color);
  bool latexHandleMinus(ostream& s, Term* term, bool rangeKnown, const char* color) const;
  bool latexHandleDivision(ostream& s, Term* term, bool rangeKnown, const char* color) const;
  void latexHandleFloat(ostream& s, Term* term, bool rangeKnown, const char* color) const;
  void latexHandleString(ostream& s, Term* term, bool rangeKnown, const char* color) const;
  void latexHandleQuotedIdentifier(ostream& s, Term* term, bool rangeKnown, const char* color) const;
  void latexHandleVariable(ostream& s, Term* term, bool rangeKnown, const char* color) const;
  void latexHandleSMT_Number(ostream& s, Term* term, bool rangeKnown, const char* color);

protected:
  void latexPrettyPrint(ostream& s,
			Term* term,
			int requiredPrec,
			int leftCapture,
			const ConnectedComponent* leftCaptureComponent,
			int rightCapture,
			const ConnectedComponent* rightCaptureComponent,
			bool rangeKnown);
  void latexPrintStrategyTerm(ostream& s, RewriteStrategy* rs, Term* term) const;

private:
  //
  //	Member functions for DagNode* -> LaTeX pretty printer.
  //
  static const char* latexComputeColor(ColoringInfo& coloringInfo, DagNode* dagNode);
  static void latexSuffix(ostream& s, DagNode* dagNode, bool needDisambig, const char* color);
  bool latexHandleIter(ostream& s,
		       ColoringInfo& coloringInfo,
		       DagNode* dagNode,
		       SymbolInfo& si,
		       bool rangeKnown,
		       const char* color);
  bool latexHandleMinus(ostream& s, DagNode* dagNode, bool rangeKnown, const char* color);
  bool latexHandleDivision(ostream& s, DagNode* dagNode, bool rangeKnown, const char* color);
  void latexHandleFloat(ostream& s, DagNode* dagNode, bool rangeKnown, const char* color);
  void latexHandleString(ostream& s, DagNode* dagNode, bool rangeKnown, const char* color);
  void latexHandleQuotedIdentifier(ostream& s, DagNode* dagNode, bool rangeKnown, const char* color);
  void latexHandleVariable(ostream& s, DagNode* dagNode, bool rangeKnown, const char* color);
  void latexHandleSMT_Number(ostream& s, DagNode* dagNode, bool rangeKnown, const char* color);
  void latexPrettyPrint(ostream& s,
			ColoringInfo& coloringInfo,
			DagNode* dagNode,
			int requiredPrec,
			int leftCapture,
			const ConnectedComponent* leftCaptureComponent,
			int rightCapture,
			const ConnectedComponent* rightCaptureComponent,
			bool rangeKnown);

  static bool prettyPrint(ostream& s,
			  StrategyExpression* strategy,
			  int requiredPrec);
  NatSet objectSymbols;
  NatSet messageSymbols;
  NatSet portalSymbols;
  StatementTransformer* statementTransformer = 0;

  //
  //	Stuff for internal tuples.
  //
  Symbol* createInternalTupleSymbol(const Vector<ConnectedComponent*>& domain, ConnectedComponent* range);
  typedef list<int> IntList;
  typedef map<IntList, Symbol*> InternalTupleMap;
  InternalTupleMap tupleSymbols;

  SMT_Info smtInfo;
  SMT_Status smtStatus;

  static const char* latexRed;
  static const char* latexGreen;
  static const char* latexBlue;
  static const char* latexCyan;
  static const char* latexMagenta;
  static const char* latexYellow;
  static const char* latexResetColor;

  friend ostream& operator<<(ostream& s, DagNode* dagNode);
  friend ostream& operator<<(ostream& s, StrategyExpression* strategy);
};

inline SymbolType
MixfixModule::getSymbolType(const Symbol* symbol) const
{
  Assert(symbol->getModule() == this, "symbol " << symbol <<
	 " belongs to " << symbol->getModule() << " and not " << this);
  return symbolInfo[symbol->getIndexWithinModule()].symbolType;
}

inline int
MixfixModule::getPolymorphIndex(const Symbol* symbol) const
{
  //
  //	If symbol is an instantiation of a polymorph, return its index or else return NONE.
  //
  Assert(symbol->getModule() == this, "symbol " << symbol <<
	 " belongs to " << symbol->getModule() << " and not " << this);
  return symbolInfo[symbol->getIndexWithinModule()].polymorphIndex;
}

inline Token
MixfixModule::getPolymorphName(int index) const
{
  return polymorphs[index].name;
}

inline SymbolType
MixfixModule::getPolymorphType(int index) const
{
  return polymorphs[index].symbolInfo.symbolType;
}

inline const Vector<Sort*>&
MixfixModule::getPolymorphDomainAndRange(int index) const
{
  return polymorphs[index].domainAndRange;
}

inline Term*
MixfixModule::getPolymorphIdentity(int index) const
{
  return polymorphs[index].identity;
}

inline const Vector<int>&
MixfixModule::getPolymorphStrategy(int index) const
{
  return polymorphs[index].strategy;
}

inline const NatSet&
MixfixModule::getPolymorphFrozen(int index) const
{
  return polymorphs[index].frozen;
}

inline int
MixfixModule::getPolymorphPrec(int index) const
{
  return polymorphs[index].symbolInfo.prec;
}

inline void
MixfixModule::getPolymorphGather(int index, Vector<int>& gather) const
{
  polymorphs[index].symbolInfo.revertGather(gather);
}

inline const Vector<int>&
MixfixModule::getPolymorphFormat(int index) const
{
  return polymorphs[index].symbolInfo.format;
}

inline int
MixfixModule::getPolymorphMetadata(int index) const
{
  return polymorphs[index].metadata;
}

inline Sort*
MixfixModule::getStrategyRangeSort() const
{
  return strategyRangeSort;
}

inline int
MixfixModule::getPrec(const Symbol* symbol) const
{
  return symbolInfo[symbol->getIndexWithinModule()].prec;
}

inline void
MixfixModule::getGather(const Symbol* symbol, Vector<int>& gather) const
{
  symbolInfo[symbol->getIndexWithinModule()].revertGather(gather);
}

inline const Vector<int>&
MixfixModule::getFormat(const Symbol* symbol) const
{
  return symbolInfo[symbol->getIndexWithinModule()].format;
}

inline MixfixModule::ModuleType
MixfixModule::getModuleType() const
{
  return moduleType;
}

inline const MixfixModule::AliasMap&
MixfixModule::getVariableAliases() const
{
  return variableAliases;
}

inline const NatSet&
MixfixModule::getObjectSymbols() const
{
  return objectSymbols;
}

inline void
MixfixModule::insertPotentialLabels(const set<int>& l)
{
  potentialLabels = l;
}

inline void
MixfixModule::insertPotentialRuleLabels(const set<int>& rls)
{
  ruleLabels.insert(rls.begin(), rls.end());
}

inline int
MixfixModule::getNrPolymorphs() const
{
  return polymorphs.length();
}

inline Sort*
MixfixModule::hookSort(Sort* sort)
{
  return (sort->index() == Sort::KIND) ? sort->component()->sort(1) : sort;
}

inline MixfixModule::ModuleType
MixfixModule::join(ModuleType t1, ModuleType t2)
{
  return static_cast<ModuleType>(t1 | t2);
}

inline bool
MixfixModule::isTheory(ModuleType t)
{
  return t & THEORY;
}

inline bool
MixfixModule::isTheory() const
{
  return isTheory(getModuleType()); 
}

inline bool
MixfixModule::isStrategic(ModuleType t)
{
  return t & STRATEGY;
}

inline bool
MixfixModule::isStrategic() const
{
  return isStrategic(getModuleType());
}

inline bool
MixfixModule::isSystem(ModuleType t)
{
  return t & SYSTEM;
}

inline bool
MixfixModule::isSystem() const
{
  return isSystem(getModuleType());
}

inline bool
MixfixModule::isObjectOriented(ModuleType t)
{
  return t & OBJECT_ORIENTED;
}

inline bool
MixfixModule::isObjectOriented() const
{
  return isObjectOriented(getModuleType()); 
}

inline bool
MixfixModule::canImport(ModuleType t1, ModuleType t2)
{
  //
  //	System can import anything; funtional can only import functional.
  //	Theory can import anything; module can only import module.
  //	And then, strategy can import anything; but others cannot import them.
  //
  return (t1 | t2) == t1;
}

inline bool
MixfixModule::canHaveAsParameter(ModuleType t1, ModuleType t2)
{
  //
  //	System can be parameterized by anything; functional can only be parameterized by functional.
  //	Only theories can be parameters.
  //
  return isTheory(t2) && (((t1 | t2) & (SYSTEM | STRATEGY)) == (t1 & (SYSTEM | STRATEGY)));
}

inline DagNode*
MixfixModule::deserialize(const Rope& encoding)
{
  extern DagNode* deserializeRope(MixfixModule*, const Rope&);
  
  return deserializeRope(this, encoding);
}

inline void
MixfixModule::processingComplete()
{
  //
  //	Desugaring complete - ditch the OBJECT_ORIENTED fiction.
  //
  moduleType = static_cast<ModuleType>(moduleType &~OBJECT_ORIENTED);
}

inline void
MixfixModule::installStatementTransformer(StatementTransformer* st)
{
  statementTransformer = st;
}

#endif
