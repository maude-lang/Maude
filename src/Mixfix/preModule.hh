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
//      Class for surface syntax of modules.
//
#ifndef _preModule_hh_
#define _preModule_hh_
#include "namedEntity.hh"
#include "lineNumber.hh"
#include "commonTokens.hh"
#include "importModule.hh"

class PreModule
  : public NamedEntity,
    public LineNumber,
    private CommonTokens,
    private ImportModule::Parent
{
  NO_COPYING(PreModule);

public:
  enum HookType
  {
    ID_HOOK,
    OP_HOOK,
    TERM_HOOK
  };

  PreModule(Token moduleName, MixfixModule::ModuleType moduleType);
  ~PreModule();

  void loseFocus();
  void finishModule();
  bool isComplete();

  void addImport(Token mode, ModuleExpression* expr);
  void addSortDecl(const Vector<Token>& sortDecl);
  void addSubsortDecl(const Vector<Token>& subsortDecl);
  void addOpDecl(const Vector<Token>& opName);
  void makeOpDeclsConsistent();

  void addType(bool kind, const Vector<Token>& tokens);
  void convertSortsToKinds();
  void setFlag(int flag);
  void setPrec(Token range);
  void setGather(const Vector<Token>& gather);
  void setFormat(const Vector<Token>& format);
  void setIdentity(const Vector<Token>& identity);
  void setStrat(const Vector<Token>& strategy);
  void setFrozen(const Vector<Token>& frozen);
  void setPoly(const Vector<Token>& polyArgs);
  void setLatexMacro(const string& latexMacro);
  void addHook(HookType type, Token name, const Vector<Token>& details);
  void addVarDecl(Token varName);
  void addStatement(const Vector<Token>& statement);
  VisibleModule* getFlatSignature();
  VisibleModule* getFlatModule();

  MixfixModule::ModuleType getModuleType() const;
  int getNrAutoImports() const;
  int getNrImports() const;
  int getAutoImport(int index) const;
  int getImportMode(int index) const;
  const ModuleExpression* getImport(int index) const;

  void process();

  void dump();
  void showModule(ostream& s = cout);

  static ImportModule* makeModule(const ModuleExpression* expr);

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

  struct Type
  {
    bool kind;
    Vector<Token> tokens;
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
    SymbolType symbolType;
    string latexMacro;
    //
    //	Filled out from types after connected components are determined.
    //
    Vector<Sort*> domainAndRange;
  };

  struct Import
  {
    Token mode;
    ModuleExpression* expr;
  };

  static void printAttributes(ostream& s, const OpDef& opDef);
  static ImportModule*  getModule(int name, const LineNumber& lineNumber);

  void regretToInform(ImportModule* doomedModule);
  int findHook(const Vector<Hook>& hookList, HookType type, int name);

  Symbol* findHookSymbol(const Vector<Token>& fullName);
  void printOpDef(ostream&s, int defIndex);
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
  Sort* computeType(const Type& type);
  void processOps();
  void fixUpSymbols();
  void processStatements();
  DagNode* makeDag(Term* subjectTerm);
  DagNode* makeDag(const Vector<Token>& subject);

  MixfixModule::ModuleType moduleType;
  Bool lastSawOpDecl;
  Bool isCompleteFlag;
  Vector<Import> imports;
  Vector<Vector<Token> > sortDecls;
  Vector<Vector<Token> > subsortDecls;
  Vector<OpDecl> opDecls;
  Vector<OpDef> opDefs;
  Vector<Vector<Token> > statements;
  IntSet labels;
  IntSet autoImports;
  VisibleModule* flatModule;

  friend ostream& operator<<(ostream& s, const PreModule::Type& type);
};

inline bool
PreModule::isComplete()
{
  return isCompleteFlag;
}

inline MixfixModule::ModuleType
PreModule::getModuleType() const
{
  return moduleType;
}

inline void
PreModule::addSortDecl(const Vector<Token>& sortDecl)
{
  sortDecls.append(sortDecl);
}

inline void
PreModule::addSubsortDecl(const Vector<Token>& subsortDecl)
{
  subsortDecls.append(subsortDecl);
}

inline int
PreModule::getNrAutoImports() const
{
  return autoImports.cardinality();
}

inline int
PreModule::getNrImports() const
{
  return imports.length();
}

inline int
PreModule::getAutoImport(int index) const
{
  return autoImports.index2Int(index); 
}

inline int
PreModule::getImportMode(int index) const
{
  return imports[index].mode.code();
}

inline const ModuleExpression*
PreModule::getImport(int index) const
{
  return imports[index].expr;
}

#endif
