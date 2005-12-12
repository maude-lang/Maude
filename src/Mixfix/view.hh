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
//      Class for views.
//
#ifndef _view_hh_
#define _view_hh_
#include<list>
#include "namedEntity.hh"
#include "lineNumber.hh"
#include "entity.hh"
#include "renaming.hh"

class View
  : public NamedEntity,
    public LineNumber,
    public Entity,
    public Entity::User,
    public Renaming
{
public:
  enum ReturnValues
  {
    OP_TERM_MAPPING = -2
  };

  View(Token viewName);
  ~View();

  void addFrom(ModuleExpression* expr);
  void addTo(ModuleExpression* expr);
  void addOpTermMapping(const Vector<Token>& fromOp, const Vector<Token>& toTerm);
  void addVarDecl(Token varName);
  void addType(bool kind, const Vector<Token>& tokens);
  void finishView();
  bool evaluate();
  bool isComplete();
  void showView(ostream& s);
  ImportModule* getFromTheory() const;
  ImportModule* getToModule() const;
  Term* getOpMapTerm(Symbol* symbol) const;

private:
  enum Status
    {
      INITIAL,
      GOOD,
      BAD,
      STALE
    };

  struct VarDecl
  {
    Token varName;
    bool lastWithCurrentDef;
  };

  struct BubblePair
  {
    Vector<Token> fromBubble;
    Vector<Token> toBubble;
  };

  typedef list<VarDecl> VarDeclList;
  typedef list<Type> TypeList;
  typedef list<BubblePair> OpTermList;
  typedef multimap<int, pair<Term*, Term*> > OpTermMap;
  typedef map<int, pair<Sort*, int> > VarMap;

  static bool typeMatch(const ConnectedComponent* c1, const ConnectedComponent* c2);
  static bool typeMatch(const Symbol* s1, const Symbol* s2);

  ConnectedComponent* mapComponent(const ConnectedComponent* component, ImportModule* module) const;
  Sort* mapSort(const Sort* sort, ImportModule* module) const;
  void regretToInform(Entity* doomedEntity);
  void finishModule1(ImportModule* module);
  void finishModule2(ImportModule* module);
  bool checkSorts();
  bool handleVarDecls();
  bool indexRhsVariables(Term* term, const VarMap& varMap, int lineNr);
  bool handleOpTermMappings();
  bool checkOps();
  bool checkPolymorphicOps();
  void clearOpTermMap();

  Status status;
  ModuleExpression* fromExpr;
  ModuleExpression* toExpr;
  ImportModule* fromTheory;
  ImportModule* toModule;
  ImportModule* newFromTheory;	// copy of fromTheory with different variable aliases
  ImportModule* newToModule;	// copy of toModule with different variable aliases
  VarDeclList varDecls;		// list of variable alias declared
  TypeList varDefs;		// list of sorts used in variable declarations
  OpTermList opTermList;	// list of op->term mappings stored as unparsed bubble pairs
  OpTermMap opTermMap;		// map from op name to op->term mappings stored as term pairs
};

inline void
View::addFrom(ModuleExpression* expr)
{
  fromExpr = expr;
}

inline void
View::addTo(ModuleExpression* expr)
{
  toExpr = expr;
}

inline bool
View::isComplete()
{
  return status != INITIAL;
}

inline ImportModule*
View::getFromTheory() const
{
  Assert(status == GOOD, "view status not good");
  return fromTheory;
}

inline ImportModule*
View::getToModule() const
{
  Assert(status == GOOD, "view status not good");
  return toModule;
}

#ifndef NO_ASSERT
inline ostream&
operator<<(ostream& s, const View* v)
{
  //
  //	Needed to avoid ambiguity.
  //
  s << static_cast<const NamedEntity*>(v);
}
#endif

#endif
