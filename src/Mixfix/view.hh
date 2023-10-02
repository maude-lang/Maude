/*

    This file is part of the Maude 3 interpreter.

    Copyright 1997-2019 SRI International, Menlo Park, CA 94025, USA.

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
#include "argument.hh"
#include "lineNumber.hh"
#include "entity.hh"
#include "renaming.hh"
#include "enclosingObject.hh"
#include "importModule.hh"

class View
  : public Argument,
    public LineNumber,
    public Entity,
    public Entity::User,
    public Renaming,
    public EnclosingObject
{
  NO_COPYING(View);

public:
  enum ReturnValues
  {
    OP_TERM_MAPPING = -2
  };

  struct StratExprInfo
  {
    StratExprInfo(CallStrategy* call, StrategyExpression* value, Vector<int>& contextSpec);

    CallStrategy* call;
    StrategyExpression* value;
    Vector<int> contextSpec;
  };

  typedef multimap<int, pair<Term*, Term*> > OpTermMap;
  typedef multimap<int, StratExprInfo> StratExprMap;

  View(Token viewName, Interpreter* owner);
  View(int viewName,
       int cleanName,
       View* baseView,
       const Vector<Argument*>& arguments,
       Interpreter* owner);
  ~View();

  void addParameter(int name, ModuleExpression* expr);
  void addFrom(ModuleExpression* expr);
  void addTo(ModuleExpression* expr);
  void finishView();
  bool evaluate();
  bool isComplete();
  int getNrParameters() const;
  bool hasFreeParameters() const;

  int getParameterName(int parameterNr) const;
  ModuleExpression* getParameterTheoryExpression(int parameterNr) const;
  ImportModule* getFromTheory() const;
  ImportModule* getToModule() const;
  bool getOpToTermMapping(Symbol* symbol, Term*& fromTerm, Term*& toTerm) const;
  bool getStratToExprMapping(RewriteStrategy* symbol,
			     CallStrategy*& fromCall,
			     StrategyExpression*& toExpr,
			     const Vector<int>*& varIndices) const;
  //
  //	These exist in order print a view at the object level and "up" a view in the metalevel.
  //
  ModuleExpression* getFrom() const;
  ModuleExpression* getTo() const;
  const OpTermMap& getOpTermMap() const;  // maybe these exposes too much but we need it for the metalevel
  const StratExprMap& getStratExprMap() const;
  //
  //	Needed for instantiating sorts, defaults to true name for views that are not
  //	the instantiation of some other view.
  //
  int getCleanName() const;
  //
  //	Needed for EnclosingObject base.
  //
  int findParameterIndex(int name) const;
  ImportModule* getParameterTheory(int index) const;
  const char* getObjectType() const;
  const NamedEntity* getObjectName() const;

  View* makeInstantiation(int viewName,
			  int cleanName,
			  const Vector<Argument*>& arguments);
  View* instantiateBoundParameters(const Vector<Argument*>& arguments);
  //
  //	OO stuff.
  //
  bool getClassSortAndSymbol(Token className,
			     const char* direction,
			     Sort* classIdSort,
			     ImportModule* module,
			     Sort*& classSort,
			     Symbol*& classSymbol);
  bool handleClassMappings();
  bool handleAttrMappings();
  //
  //	So ViewCache can tell if the view it just instantiated is bad.
  //
  bool isBad() const;

  string latexViewExpression(bool parameterBrackets = false) const;

protected:
  void regretToInform(Entity* doomedEntity);
  ConnectedComponent* mapComponent(const ConnectedComponent* component) const;
  //
  //	Op->Term and Strat->Expr mappings need to be handled by a subclass.
  //
  virtual bool handleTermAndExprMappings();
  bool insertOpToTermMapping(Term* fromTerm, Term* toTerm);
  bool insertStratToExprMapping(CallStrategy* fromStrategy,
				StrategyExpression* toExpr,
				ImportModule* targetModule);

private:
  enum Status
    {
      INITIAL,  // unprocessed - needs evalute() called before we can use
      PROCESSING,	// processing in progress - used to catch recursive use
      GOOD,	// processed and good; could become STALE if stuff we rely on changes
      BAD,	// processing halted due to badness; could become STALE if stuff we rely on changes
      STALE	// stuff we relied on changed; need to be reprocessed to GOOD or BAD if needed
    };

  struct ParameterDecl
  {
    int name;
    ModuleExpression* expr;
    ImportModule* theory;  // this is the actual theory and not a parameter copy
  };

  typedef map<int, pair<Sort*, int> > VarMap;

  static bool typeMatch(const ConnectedComponent* c1, const ConnectedComponent* c2);
  static bool typeMatch(const Symbol* s1, const Symbol* s2);
  static bool typeMatch(const RewriteStrategy* s1, const RewriteStrategy* s2);

  void finishModule(ImportModule* module);
  bool checkSorts();
  bool checkOps();
  bool checkPolymorphicOps();
  bool checkStrats();
  void clearOpTermMap();
  void clearStratExprMap();
  Sort* mapSort(const Sort* sort) const;
  bool indexRhsVariables(Term* term, const VarMap& varMap);
  void addParameter(int name, ImportModule* theory);
  void setToModule(ImportModule* target);

  ImportModule* makeToModule(const Vector<Argument*>& arguments) const;
  bool handleInstantiationByTheoryView(View* copy,
				       Renaming* canonicalRenaming,
				       ParameterMap& parameterMap,
				       ParameterSet& extraParameterSet,
				       const Vector<Argument*>& arguments) const;
  void handleInstantiationByParameter(View* copy,
				      Renaming* canonicalRenaming,
				      ParameterMap& parameterMap,
				      NatSet& positionsInstantiatedParameter,
				      const Vector<Argument*>& arguments) const;
  void handleInstantiationByModuleView(View* copy,
				       Renaming* canonicalRenaming,
				       ParameterMap& parameterMap,
				       const NatSet& positionsInstantiatedParameter,
				       const Vector<Argument*>& arguments) const;
  void handleSortMappings(View* copy, const Renaming* canonicalRenaming) const;
  void handleOpMappings(View* copy, const Renaming* canonicalRenaming) const;
  void handleAwkwardCase(View* copy, Symbol* symbol, Term* fromTerm, Term* toTerm) const;
  void handleOpToTermMappings(View* copy, Renaming* canonical) const;
  void handlePolymorphMappings(View* copy, const Renaming* canonicalRenaming) const;
  void handleStratMappings(View* copy, const Renaming* canonicalRenaming) const;
  void handleStratAwkwardCase(View* copy,
			      RewriteStrategy* strat,
			      CallStrategy* fromCall,
			      StrategyExpression* toExpr,
			      const Vector<int>& varIndices) const;
  void handleStratToExprMappings(View* copy, Renaming* canonical) const;

  Interpreter* const owner;
  //
  //	If we are an instantiation of a view we keep a pointer to the
  //	view we are an instantiation of; Otherwise this pointer is 0.
  //
  View* const baseView;
  //
  //	If we are the instantiation of a view, we keep a copy of the view arguments
  //	we were passed so we safely add ourselves as users and be able to remove
  //	ourselves as users in our destructor.
  //
  //	Being a user of each argument view shouldn't be strictly necessary since we expect
  //	our target to be a user; and in the dubious case that our target of our baseView
  //	doesn't depend on one of the parameters of our baseView, we expect a change
  //	to a corresponding view argument to have no noticeable effect.
  //
  //	However, to forstall any unanticipated weirdness from not self-destructing when one
  //	of our argument views changes, we add ourself as a user of each argument view and
  //	thus need a pointer to each argument view so we can remove ourself as a user in
  //	our destructor and avoid an argument view from having a stale user pointer.
  //
  //	If we formed this view by instantiating some baseView with parameters from an enclosing object
  //	then this view will have bound parameters that have to be revisited should the enclosing
  //	object be instantiated. In particular a bound parameter could belong to a view with
  //	bound parameters that was passed as an argument.
  //	In order to revisit the bound parameters we also make use of this saved copy
  //	of our arguments.
  //
  const Vector<Argument*> savedArguments;
  //
  //	An instantiation can have a clean name that is different from its true name.
  //
  const int cleanName;

  Status status;
  Vector<ParameterDecl> parameters;
  ModuleExpression* fromExpr;
  ModuleExpression* toExpr;
  ImportModule* fromTheory;
  ImportModule* toModule;
  //
  //	We store op->term mappings as term pairs so we can print out
  //	the view.
  //
  OpTermMap opTermMap;
  StratExprMap stratExprMap;	// map from op name to strat->expr mapping stored and (id, expr) pairs
};

inline bool
View::isBad() const
{
  return status == BAD;
}

inline void
View::setToModule(ImportModule* target)
{
  toModule = target;
  addInAllConflicts(target);  // we propagate conflicts from our toModule's parameters to our own
  target->addUser(this);  // we become a user of our toModule
}

inline int
View::getCleanName() const
{
  return cleanName;
}

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

inline void
View::finishView()
{
  evaluate();
}

inline bool
View::isComplete()
{
  return status != INITIAL;
}

inline int
View::getNrParameters() const
{
  return parameters.size();
}

inline int
View::getParameterName(int parameterNr) const
{
  return parameters[parameterNr].name;
}

inline ModuleExpression*
View::getParameterTheoryExpression(int parameterNr) const
{
  return parameters[parameterNr].expr;
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

inline ModuleExpression*
View::getFrom() const
{
  return fromExpr;
}

inline ModuleExpression*
View::getTo() const
{
  return toExpr;
}

inline const View::OpTermMap&
View::getOpTermMap() const
{
  return opTermMap;
}

inline const View::StratExprMap&
View::getStratExprMap() const
{
  return stratExprMap;
}


inline
View::StratExprInfo::StratExprInfo(CallStrategy* call,
				   StrategyExpression* value,
				   Vector<int>& contextShape)
  : call(call), value(value)
{
  contextSpec.swap(contextShape);
}

inline ostream&
operator<<(ostream& s, const View* v)
{
  //
  //	Needed to avoid ambiguity.
  //
  s << static_cast<const NamedEntity*>(v);
  return s;
}

inline bool
View::hasFreeParameters() const
{
  return getNrParameters() > 0 && !(hasBoundParameters());
}

#endif
