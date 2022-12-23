/*

    This file is part of the Maude 3 interpreter.

    Copyright 1997-2022 SRI International, Menlo Park, CA 94025, USA.

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
#include "preModule.hh"
#include "lineNumber.hh"
#include "syntaxContainer.hh"
#include "sharedTokens.hh"
#include "importModule.hh"
#include "moduleDatabase.hh"

class SyntacticPreModule
  : public PreModule,
    public LineNumber,
    public SyntaxContainer,
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

  void loseFocus();
  void finishModule(Token endToken);
  bool isComplete();

  void addParameter2(Token name, ModuleExpression* theory);
  void addImport(Token modeToken, ModuleExpression* expr);

  void addSortDecl(const Vector<Token>& sortDecl);
  void addSubsortDecl(const Vector<Token>& subsortDecl);
  void addOpDecl(const Vector<Token>& opName);
  void addStratDecl(Token opName);
  void makeDeclsConsistent();

  void addType(bool kind, const Vector<Token>& tokens);
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
  void addVarDecl(Token varName);
  void addStatement(const Vector<Token>& statement);

  void addClassDecl(Token name);
  void addAttributePair(Token attributeName, Token sortName);
  void addSubclassDecl(const Vector<Token>& subclassDecl);

  VisibleModule* getFlatSignature();
  VisibleModule* getFlatModule();

  const ModuleDatabase::ImportMap* getAutoImports() const;

  void dump();
  void showModule(ostream& s = cout);
  //
  //	Utility functions - maybe they should go elsewhere?
  //
  static void printGather(ostream& s, const Vector<int>& gather);
  static void printFormat(ostream& s, const Vector<int>& format);
  static bool checkFormatString(const char* string);

private:
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
    Token sortName;
  };

  struct ClassDecl
  {
    Token name;  // must be a valid sort name; no mixfix
    Vector<AttributePair> attributes;
  };
  
  void process();

  static void printAttributes(ostream& s, const OpDef& opDef);
  static void printAttributes(ostream& s, const StratDecl& stratDecl);
  static void printSortTokenVector(ostream& s, const Vector<Token>& sorts);

  void regretToInform(Entity* doomedEntity);
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

  int startTokenCode;
  Bool lastSawOpDecl;
  Bool isStrategy;
  Bool isCompleteFlag;
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
  ModuleDatabase::ImportSet ooIncludes;
  VisibleModule* flatModule;
};

inline bool
SyntacticPreModule::isComplete()
{
  return isCompleteFlag;
}

inline void
SyntacticPreModule::addSortDecl(const Vector<Token>& sortDecl)
{
  if (sortDecl.empty())
    IssueWarning("skipped empty sort declaration.");  // would be nice to have a line number
  else
    sortDecls.append(sortDecl);
}

inline void
SyntacticPreModule::addSubsortDecl(const Vector<Token>& subsortDecl)
{
  subsortDecls.append(subsortDecl);
}

inline void
SyntacticPreModule::addSubclassDecl(const Vector<Token>& subclassDecl)
{
  subclassDecls.append(subclassDecl);
}

inline const ModuleDatabase::ImportMap*
SyntacticPreModule::getAutoImports() const
{
  return &autoImports;
}

inline void
SyntacticPreModule::addClassDecl(Token name)
{
  int nrClassDecls = classDecls.size();
  classDecls.resize(nrClassDecls + 1);
  classDecls[nrClassDecls].name = name;
}

inline void
SyntacticPreModule::addAttributePair(Token attributeName, Token sortName)
{
  classDecls[classDecls.size() - 1].attributes.append({attributeName, sortName});
}

#endif
