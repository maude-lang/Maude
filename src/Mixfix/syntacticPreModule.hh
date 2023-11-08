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
//      Class for surface syntax of modules.
//
#ifndef _syntacticPreModule_hh_
#define _syntacticPreModule_hh_
#include <set>
#include <map>
#include <list>
#include "term.hh"
#include "preModule.hh"
#include "lineNumber.hh"
#include "syntaxContainer.hh"
#include "sharedTokens.hh"
#include "importModule.hh"
#include "moduleDatabase.hh"
#include "statementTransformer.hh"
#include "variable.hh"

class SyntacticPreModule
  : public PreModule,
    public LineNumber,
    public SyntaxContainer,
    public StatementTransformer,
    private SharedTokens
    
{
  NO_COPYING(SyntacticPreModule);

public:
  enum HookType
  {
    ID_HOOK,
    OP_HOOK,
    TERM_HOOK
  };

  SyntacticPreModule(Token startToken, Token moduleName, Interpreter* owner);
  ~SyntacticPreModule();

  void loseFocus(bool clearCaches);
  void finishModule(Token endToken);
  bool isComplete();

  void addParameter2(Token name, ModuleExpression* theory) override;
  void addImport(Token modeToken, ModuleExpression* expr);

  void addSortDecl(const Vector<Token>& sortDecl);
  void addSubsortDecl(const Vector<Token>& subsortDecl);
  void addOpDecl(const Vector<Token>& opName);
  void addStratDecl(Token opName);
  void makeDeclsConsistent();

  void addType(bool kind, const Vector<Token>& tokens) override;
  void convertSortsToKinds();
  void setFlag(int flag);
  void setPrec(Token range);
  void setGather(const Vector<Token>& gather);
  void setMetadata(Token metaDataTok);
  void setFormat(const Vector<Token>& format);
  void setIdentity(const Vector<Token>& identity);
  void setStrat(const Vector<Token>& strategy);
  void setFrozen(const Vector<Token>& frozen);
  void setPoly(const Vector<Token>& polyArgs);
  void setLatexMacro(const string& latexMacro);
  void addHook(HookType type, Token name, const Vector<Token>& details);
  void addVarDecl(Token varName) override;
  void addStatement(const Vector<Token>& statement);

  void addClassDecl(Token name);
  void addAttributePair(Token attributeName, bool kind, const Vector<Token>& tokens);
  void addAttributePairNoColon(Token attributeName, bool kind, const Vector<Token>& tokens);
  void addSubclassDecl(const Vector<Token>& subclassDecl);
  void endMsg();

  VisibleModule* getFlatSignature() override;
  VisibleModule* getFlatModule() override;

  const ModuleDatabase::ImportMap* getAutoImports() const override;

  void dump();
  void showModule(ostream& s);
  //
  //	Utility functions - maybe they should go elsewhere?
  //
  static void printGather(ostream& s, const Vector<int>& gather);
  static void printFormat(ostream& s, const Vector<int>& format);
  static bool checkFormatString(const char* string);
  static string stripAttributeSuffix(Symbol* attributeSymbol);
  static bool hasAttributeSuffix(Symbol* symbol);

  void latexShowModule(ostream& s);

private:
  //
  //	Functions to transform statements as soon as they are parsed and
  //	before they are inserted into the flat module.
  //
  Outcome transformSortConstraint(SortConstraint* sortConstraint) override;
  Outcome transformEquation(Equation* equation) override;
  Outcome transformRule(Rule* rule) override;
  
  struct Hook
  {
    HookType type;
    int name;
    Vector<Token> details;
  };

  struct OpDecl
  {
    Token prefixName;
    int defIndex;
    union
    {
      Symbol* symbol;
      int polymorphIndex;
    };
    bool originator;  // did we originate this symbol in our flat module?
    int bubbleSpecIndex;
  };

  struct OpDef
  {
    OpDef();

    Vector<Type> types;
    Vector<Token> identity;
    Vector<Hook> special;
    Vector<int> strategy;
    NatSet frozen;
    NatSet polyArgs;
    int prec;
    Vector<int> gather;
    Vector<int> format;
    int metadata;
    SymbolType symbolType;
    string latexMacro;
    //
    //	Filled out from types after connected components are determined.
    //
    Vector<Sort*> domainAndRange;
  };

  struct StratDecl
  {
    StratDecl() : metadata(NONE) {}

    Vector<Token> names;
    Vector<Type> types;
    int metadata;
    //
    // Filled out from types after connected components are determined.
    //
    Vector<Sort*> domainAndSubject;
  };

  //
  //	For omods.
  //
  struct AttributePair
  {
    Token attributeName;
    Type type;
    //
    //	Filled out from type after connected components are determined.
    //
    Sort* sort;
  };

  struct ClassDecl
  {
    Token name;  // must be a valid sort name; no mixfix
    Vector<AttributePair> attributes;
    //
    //	Filled out during processing.
    //
    Sort* classSort;
    //Symbol* classSymbol; // do we need to store this?
  };
  //
  //	For omod statement transformation.
  //
  enum class GatherMode
    {
     PATTERN,
     SUBJECT,
     CONDITION_PATTERN,
     CONDITION_SUBJECT,
    };

  typedef map<Symbol*, Term*, Symbol::LessThan> AttributeMap;

  struct ObjectOccurrence
  {
    Term* objectTerm = 0;			// the object occurrence
    VariableTerm* variableTerm = 0;		// if non-null, an AttributeSet variable appearing under the 3rd argument
    AttributeMap attributeTerms;		// attribute subterms appearing under the 3rd argument, indexed by attribute symbols
  };

  typedef list<ObjectOccurrence> ObjectOccurrences;

  struct ObjectInfo
  {
    //
    //	The first occurrence in the lhs of the statement.
    //	A second occurrence will disqualify the statement
    //	Having no occurrence will disqualify the object but not the statement.
    //
    ObjectOccurrence patternOccurrence;
    //
    //	Having a class argument that is not either a class constant or a
    //	variable of class sort will disqualify the statement.
    //
    Term* classArgument = 0;
    //
    //	The name part of the class variable if there is one.
    //
    int classVariableName = NONE;
    //
    //	The class sort, derived from a class constant or class variable.
    //
    Sort* classSort = 0;
    //
    //	Non-pattern occurrences.
    //	Having a different class argument will disqualify the statement.
    //
    ObjectOccurrences subjectOccurrences;
  };

  typedef map<Term*, ObjectInfo, Term::LessThan> ObjectMap;
  typedef map<pair<int,int>,int> VarCountMap;
  typedef set<int> IdSet;
  typedef set<Symbol*> SymbolSet;
  typedef map<int, SymbolSet> ClassAttrMap;

  struct StatementInfo
  {
    ~StatementInfo();
    int chooseFreshVariableName(const char* base);
    bool checkVariables() const;

    ObjectMap objectMap;
    VarCountMap varCountMap;
    IdSet forbiddenNames;
    bool ignore = false;
  };

  void process();

  void printAttributes(ostream& s, const OpDef& opDef) const;
  static void printAttributes(ostream& s, const StratDecl& stratDecl);
  static void printSortTokenVector(ostream& s, const Vector<Token>& sorts);

  static void insertSubsorts(const Vector<Sort*> smaller, Vector<Sort*> bigger);

  void regretToInform(Entity* doomedEntity) override;
  int findHook(const Vector<Hook>& hookList, HookType type, int name);

  Symbol* findHookSymbol(const Vector<Token>& fullName);
  void printOpDef(ostream& s, int defIndex);
  void printStratDecl(ostream& s, const StratDecl& decl);
  bool defaultFixUp(OpDef& opDef, Symbol* symbol);
  bool defaultFixUp(OpDef& opDef, int index);
  void extractSpecialTerms(const Vector<Token>& bubble,
			   int begin,
			   ConnectedComponent* component,
			   Vector<Term*>& terms);
  Symbol* extractSpecialSymbol(const Vector<Token>& bubble, int& pos);
  void processImports();
  void processSorts();
  void processSubsorts();
  Sort* getSort(Token token);
  void checkOpTypes();
  void checkType(const Type& type);
  void computeOpTypes();
  void computeStrategyTypes();
  Sort* computeType(const Type& type);
  void processOps();
  void fixUpSymbols();
  void fixUpSymbol(const OpDecl& opDecl);
  void fixUpPolymorph(const OpDecl& opDecl);
  void processStrategies();
  void processStatements();
  bool compatible(int endTokenCode);
  //
  //	For omods and oths.
  //
  void addHonoraryClassNames(ImportModule* import, set<int>& classNames) const;
  void addHonoraryClassNames(set<int>& classNames) const;
  void addHonoraryAttributeSymbols();
  void processClassSorts();
  Sort* findClassIdSortName() const;
  Sort* findClassIdSort() const;
  void checkAttributeTypes();
  void purgeImpureClasses();
  void computeAttributeTypes();
  void processClassOps();
  void checkAttributes();

  bool isClassSort(const Sort *s) const;
  Sort* findCorrespondingClassSort(const Symbol *s) const;
  //
  //	Statement analysis pass.
  //
  void gatherObjects(PreEquation* pe, StatementInfo& si) const;
  void gatherObjects(GatherMode mode, Term* term, StatementInfo& si) const;
  bool recordClassArgument(Term* classArgument, ObjectInfo& oi) const;
  bool analyzeAttributeSetArgument(Term* attributeSetArgument, Sort* classSort, ObjectOccurrence& oo) const;
  //
  //	Statement transformation pass.
  //
  bool doTransformation(StatementInfo& si);
  void transformClassArgument(ObjectOccurrence& oo, VariableSymbol* vs, int varName);
  bool transformPatternAttributes(ObjectInfo& oi, StatementInfo& si);
  bool transformSubjectAttributes(ObjectOccurrence& so, ObjectOccurrence& po);
  void garbageCollectAttributeSet(Term* attributeSet, Symbol* attributeSetSymbol) const;
  //
  //	LaTeX support.
  //
  void latexSortTokenVector(ostream& s, const Vector<Token>& sorts);
  void latexTokenVector(ostream& s, const Vector<Token>& tokens, Index first, Index last);
  void latexType(ostream& s, const Type& type);
  void latexOpDef(ostream&s, const OpDef& opDef);
  void latexAttributes(ostream& s, const OpDef& opDef);
  void latexStratDecl(ostream& s, const StratDecl& stratDecl);

  int startTokenCode;
  bool lastSawOpDecl;
  bool isStrategy;
  bool isCompleteFlag;
  Vector<Vector<Token> > sortDecls;
  Vector<Vector<Token> > subsortDecls;
  Vector<OpDecl> opDecls;
  Vector<OpDef> opDefs;
  Vector<StratDecl> stratDecls;
  Vector<Vector<Token> > statements;
  
  Vector<ClassDecl> classDecls;
  Vector<Vector<Token> > subclassDecls;

  set<int> potentialLabels;
  set<int> potentialRuleLabels;
  ModuleDatabase::ImportMap autoImports;
  VisibleModule* flatModule;
  //
  //	Sorts determined for object oriented modules and theories.
  //
  Sort* classIdSort = 0;
  Sort* attributeSort = 0;
  set<int> classNames;
  SymbolSet attributeSymbols;
  //
  //	For classes defined in this PreModule we keep track of attribute symbols, both
  //	declared and inherited.
  //	A local class is pure if it does not inherit (directly or indirectly) from an
  //	imported class and it is not below a stray sort.
  //	We consider the set of attribute symbols to be exact for pure classes and a
  //	lower bound for impure classes.
  //
  ClassAttrMap localClasses;
};

inline bool
SyntacticPreModule::isComplete()
{
  return isCompleteFlag;
}

inline void
SyntacticPreModule::addSortDecl(const Vector<Token>& sortDecl)
{
  sortDecls.append(sortDecl);
}

inline void
SyntacticPreModule::addSubsortDecl(const Vector<Token>& subsortDecl)
{
  subsortDecls.append(subsortDecl);
}

inline const ModuleDatabase::ImportMap*
SyntacticPreModule::getAutoImports() const
{
  return &autoImports;
}

inline
SyntacticPreModule::StatementInfo::~StatementInfo()
{
  //
  //	The key of each objectMap entry is a normalized deep copy of an object name subterm and
  //	needs to be self destructed separately from the statement it was copied from.
  //
  for (auto& i : objectMap)
    i.first->deepSelfDestruct();
}

inline bool
SyntacticPreModule::isClassSort(const Sort *s) const
{
  //
  //	Name must be that of an previously identified class.
  //
  return classNames.find(s->id()) != classNames.end();
}

//
//	Ugly hack to pretty print attributes. Note that it cannot be wrapped in parentheses.
//
#define ATTRIBUTE(symbol) \
  stripAttributeSuffix(symbol) << " : " << ((symbol)->domainComponent(0)->sort(Sort::KIND))

#endif
