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

  void addImport(const Vector<Token>& import);
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
  void setLatexMacro(const string& latexMacro);
  void addHook(HookType type, Token name, const Vector<Token>& details);
  void addVarDecl(Token varName);
  void addStatement(const Vector<Token>& statement);
  VisibleModule* getFlatSignature();
  VisibleModule* getFlatModule();

  void process();

  void dump();
  void showModule(ostream& s = cout);

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

  void regretToInform(ImportModule* doomedModule);
  int findHook(const Vector<Hook>& hookList, HookType type, int name);

  Symbol* findHookSymbol(const Vector<Token>& fullName);
  void printOpDef(ostream&s, int defIndex);
  bool defaultFixUp(OpDef& opDef, Symbol* symbol);
  void extractSpecialTerms(const Vector<Token>& bubble,
			   int begin,
			   ConnectedComponent* component,
			   Vector<Term*>& terms);
  Symbol* extractSpecialSymbol(const Vector<Token>& bubble, int& pos);
  void processImports();
  void importModule(int name, const LineNumber& linenumber);
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
  Vector<Vector<Token> > imports;
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

inline void
PreModule::addImport(const Vector<Token>& import)
{
  imports.append(import);
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

#endif
