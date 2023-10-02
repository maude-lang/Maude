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
//      Implementation for class View.
//

//      utility stuff
#include "macros.hh"
#include "vector.hh"

//      forward declarations
#include "core.hh"
#include "interface.hh"
#include "variable.hh"
#include "higher.hh"
#include "strategyLanguage.hh"
#include "mixfix.hh"

//      interface class definitions
#include "symbol.hh"
#include "dagNode.hh"
#include "term.hh"

//      core class definitions
#include "argumentIterator.hh"
#include "rewriteStrategy.hh"

//      variable class definitions
#include "variableTerm.hh"

// 	strategy language definitions
#include "badFlag.hh"
#include "callStrategy.hh"

//	front end class definitions
#include "token.hh"
#include "moduleExpression.hh"
#include "interpreter.hh"
#include "parameter.hh"
#include "importTranslation.hh"
#include "view.hh"

//	our stuff
#include "instantiateViewWithFreeParameters.cc"
#include "instantiateViewWithBoundParameters.cc"
#include "ooView.cc"

//
//	Version for original view.
//
View::View(Token viewName, Interpreter* owner)
  : Argument(viewName.code()),
    LineNumber(viewName.lineNumber()),
    owner(owner),
    baseView(0),  // marks this as an original
    cleanName(viewName.code())
{
  fromTheory = 0;
  toModule = 0;
  fromExpr = 0;
  toExpr = 0;
  status = INITIAL;
}

//
//	Version for instantiation of a view.
//
View::View(int viewName,
	   int cleanName,
	   View* baseView,
	   const Vector<Argument*>& arguments,
	   Interpreter* owner)
  : Argument(viewName),
    LineNumber(baseView->getLineNumber()),
    owner(owner),
    baseView(baseView),  // marks this as an instantiation
    savedArguments(arguments),
    cleanName(cleanName)
{
  fromTheory = 0;
  toModule = 0;
  fromExpr = 0;
  toExpr = 0;
  status = INITIAL;
  //
  //	We're a user of our baseView, and need to self-destruct if
  //	it goes away.
  //
  baseView->addUser(this);
  //
  //	We are likewise a user of our view arguments, though this
  //	is probably not strictly necessary - we do this defensively.
  //
  for (Argument* a : arguments)
    {
      if (View* v = dynamic_cast<View*>(a))
	v->addUser(this);
    }
  //
  //	Interpereter has base classes ModuleCache and ViewCache.
  //	We want ViewCache to get the regretToInform() if we self-destuct.
  //
  ViewCache* viewCache = owner;
  addUser(viewCache);
}

View::~View()
{
  clearOpTermMap();
  clearStratExprMap();
  //
  //	Remove ourselves as users of our baseView, fromTheory and toModule and
  //	deepSelfDestruct() from/to expressions.
  //
  if (baseView != 0)
    {
      //
      //	We're an instantiation of a view; need to deal with our
      //	view arugments.
      //
      for (Argument* a : savedArguments)
	{
	  if (View* v = dynamic_cast<View*>(a))
	    v->removeUser(this);
	}
      baseView->removeUser(this);
    }
  if (fromTheory != 0)
    fromTheory->removeUser(this);
  if (toModule != 0)
    toModule->removeUser(this);
  if (fromExpr != 0)
    fromExpr->deepSelfDestruct();
  if (toExpr != 0)
    toExpr->deepSelfDestruct();
  //
  //	Remove ourselves as users of our parameter theories and
  //	deepSelfDestruct() parameter theory expressions.
  //
  for (ParameterDecl& pd : parameters)
    {
      if (pd.theory != 0)
	pd.theory->removeUser(this);
      if (pd.expr != 0)
	pd.expr->deepSelfDestruct();
    }
  //
  //	Warn our users of our impending demise.
  //
  informUsers();
}

//
//	The following four member functions are virtual in base class
//	EnclosingObject and so can't be inline.
//

int
View::findParameterIndex(int name) const
{
  int nrParameters = parameters.size();
  for (int i = 0; i < nrParameters; ++i)
    {
      if (parameters[i].name == name)
	return i;
    }
  return NONE;
}

ImportModule*
View::getParameterTheory(int index) const
{
  return parameters[index].theory;
}

const char*
View::getObjectType() const
{
  return "view";
}

const NamedEntity*
View::getObjectName() const
{
  return this;
}

void
View::addParameter(int name, ModuleExpression* expr)
{
  //
  //	This version is for views created from syntax or meta-syntax
  //	that are subject to evaluation(), perhaps multiple times to
  //	compute the parameter theory from the module expression and
  //	the current state of the module and view databases.
  //
  size_t nrParameters = parameters.size();
  parameters.expandTo(nrParameters + 1);
  parameters[nrParameters].name = name;
  parameters[nrParameters].expr = expr;
  parameters[nrParameters].theory = 0;
}

void
View::addParameter(int name, ImportModule* theory)
{
  //
  //	This version is for view instances that are created internally.
  //	They are created in fully evaluated form and are thrown away
  //	rather than re-evaluated if things change underneath them.
  //	Thus paremeters come with a theory and no module expression.
  //
  size_t nrParameters = parameters.size();
  parameters.expandTo(nrParameters + 1);
  parameters[nrParameters].name = name;
  parameters[nrParameters].expr = 0;
  parameters[nrParameters].theory = theory;
  theory->addUser(this);
}

void
View::clearOpTermMap()
{
  for (auto& i : opTermMap)
    {
      i.second.first->deepSelfDestruct();
      i.second.second->deepSelfDestruct();
    }
  opTermMap.clear();
}

void
View::clearStratExprMap()
{
  for (auto& i : stratExprMap)
    {
      delete i.second.call;
      delete i.second.value;
    }
  stratExprMap.clear();
}

void
View::regretToInform(Entity* doomedEntity)
{
  //
  //	Something we depend on disappeared.
  //
  if (baseView != 0)
    {
      //
      //	We're an instantiation that is generated from a module expression
      //	so we just self-destruct.
      //
      DebugAdvisory("view instatiation " << this << " self-destructs");
      delete this;
      return;
    }
  //
  //	We're an original syntactic or meta-syntactic view.
  //
  if (doomedEntity == fromTheory)
    fromTheory = 0;
  else if(doomedEntity == toModule)
    toModule = 0;
  else
    {
      const Vector<ParameterDecl>::iterator e = parameters.end();
      for (Vector<ParameterDecl>::iterator i = parameters.begin(); i != e; ++i)
	{
	  if (doomedEntity == i->theory)
	    {
	      i->theory = 0;
	      goto ok;
	    }
	}
      CantHappen("unexpected regretToInform() for view " << this);
    }
 ok:
  //
  //	Something we depended on changed so self destruct all calculated stuff.
  //
  clearOpTermMap();
  purgeGeneratedMappings(); // breaks stuff - not sure why
  if (fromTheory != 0)
    {
      fromTheory->removeUser(this);
      fromTheory = 0;
    }
  if (toModule != 0)
    {
      toModule->removeUser(this);
      toModule = 0;
    }
  const Vector<ParameterDecl>::iterator e = parameters.end();
  for (Vector<ParameterDecl>::iterator i = parameters.begin(); i != e; ++i)
    {
      if (i->theory != 0)
	{
	  i->theory->removeUser(this);
	  i->theory = 0;
	}
    }
  DebugAdvisory("view " << this << " becomes stale");
  //
  //	In almost all cases, when an original view becomes stale rather
  //	than self destructing, all modules and views that depend on it,
  //	either as a base-view or as an argument also depend on the
  //	thing that went away to make it stale.
  //
  //	However there is an edge case, for example:
  //	  view V{P1 :: T1, P2 :: T2} from T to M{P1} is ... endv
  //	Here if T2 goes away, M1{P1} may not self-destruct and thus it
  //	may not trigger the invalidation cascade for users of V.
  //	To handle this rather dubious case, if we have parameters,
  //	we pretend like we self-destructed and inform our users.
  //
  //	5/9/19 We no longer allow the dubious case and no longer
  //	do the pretend self-destruct
  //
  //if (getNrParameters() != 0)
  //  informUsers();
  //
  status = STALE;
}

ConnectedComponent*
View::mapComponent(const ConnectedComponent* component) const
{
  Sort* sort = toModule->findSort(renameSort(component->sort(1)->id()));
  Assert(sort != 0, "translation for sort failed");
  return sort->component();
}

bool
View::checkSorts()
{
  //
  //	We check that for each pair of sorts (A, B) lying in the same kind in
  //	fromTheory, there exists under our mapping sorts (A', B') that are in 
  //	the same kind in toModule, and further more if A <= B then A' <= B'.
  //
  //	We examine the sort structure of the from theory one connected component at a time.
  //
  for (ConnectedComponent* kind : fromTheory->getConnectedComponents())
    {
      //
      //	When an strategy view is instantiated to a functional or system
      //	module, the auxiliary sort type must not be defined
      //
      if (fromTheory->isStrategic() && kind == fromTheory->getStrategyRangeSort()->component())
	continue;
      //
      //
      //	For each kind, we examine its sorts.
      //
      int nrSorts = kind->nrSorts();
      for (int j = 1; j < nrSorts; ++j)
	{
	  Sort* jSort = kind->sort(j);
	  int jId = jSort->id();
	  int jTrans = renameSort(jSort->id());
	  if (fromTheory->moduleDeclared(jSort) && jTrans != jId)
	    {
	      IssueAdvisory(*this << ": sort mapping not applied to sort " << QUOTE(jSort) <<
			    " as it was declared in a module rather than in a theory.");
	      jTrans = jId;
	    }
	  Sort* jTransSort = toModule->findSort(jTrans);
	  if (jTransSort == 0)
	    {
	      IssueWarning(*this << ": failed to find sort " << QUOTE(Token::sortName(jTrans)) <<
			   " in " << QUOTE(toModule) << " to represent sort " << QUOTE(jSort) <<
			   " from " << QUOTE(fromTheory) << '.');
	      return false;
	    }
	  if (fromTheory->moduleDeclared(jSort) && !(toModule->moduleDeclared(jTransSort)))
	    {
	      //
	      //	We don't allow a module declared sort to be implicity mapped to
	      //	a sort that was declared in a theory, whether a target theory
	      //	or a parameter theory of a target module.
	      //	Currently theories cannot have parameters so if toModule is a theory
	      //	we can blame it for illegal mapping.
	      //
	      IssueWarning(*this << ": implicit mapping of sort " << QUOTE(jSort) <<
			   " that was declared in a module to sort " << QUOTE(jTransSort) <<
			   (toModule->isTheory() ? " from the target theory" : " derived from a parameter theory") <<
			   " is not allowed.");
	      return false;
	    }
	  ConnectedComponent* transKind = jTransSort->component();
	  //
	  //	Now we compare the sort to earlier sorts which have already been
	  //	check for a valid translation.
	  //
	  for (int k = 1; k < j; ++k)
	    {
	      Sort* kSort = kind->sort(k);
	      int kId = kSort->id();
	      if (!(fromTheory->moduleDeclared(kSort)))
		kId = renameSort(kId);
	      Sort* kTransSort = toModule->findSort(kId);
	      if (kTransSort->component() != transKind)
		{
		  IssueWarning(*this << ": sorts " << QUOTE(jSort) << " and " << QUOTE(kSort) << " from " <<
			       QUOTE(fromTheory) << " are in the same kind but " <<
			       QUOTE(jTransSort) << " and " << QUOTE(kTransSort) << " from " <<
			       QUOTE(toModule) << " are in different kinds.");
		  return false;
		}
	      if (leq(jSort, kSort) && !(leq(jTransSort, kTransSort)))
		{
		  //
		  //	<= is not preserved but this case doesn't lead to serious
		  //	inconsistancies later on so we keep going.
		  //
		  IssueWarning(*this << ": " << QUOTE(jSort) << " is a subsort of " << QUOTE(kSort) <<
			       " in " << QUOTE(fromTheory) << " but " << QUOTE(jTransSort) <<
			       " is not a subsort of " << QUOTE(kTransSort) << " in " <<
			       QUOTE(toModule) << '.');
		}			      
	    }
	}
    }
  //
  //	Check that each our sort mappings maps a theory declared sort.
  //
  int nrSortMappings = getNrSortMappings();
  for (int i = 0; i < nrSortMappings; ++i)
    {
      int fromSortName = getSortFrom(i);
      Sort* fromSort = fromTheory->findSort(fromSortName);
      if (fromSort == 0)
	{
	  IssueAdvisory(*this << ": sort mapping for " << QUOTE(Token::sortName(fromSortName)) <<
			" redundant because there is no such sort in the from-theory.");
	}
    }
  return true;
}

bool
View::typeMatch(const ConnectedComponent* c1, const ConnectedComponent* c2)
{
  //
  //	c1 and c2 are in different theories but they typeMatch if they have sorts with the same name.
  //	Actually we only need check for one regular sort from one component, if a similarly named
  //	sort exists in the other.
  //
  int nrSorts = c1->nrSorts();
  int sortName = c2->sort(1)->id();
  for (int i = 0; i < nrSorts; ++i)
    {
      if (c1->sort(i)->id() == sortName)
	return true;
    }
  return false;
}

bool
View::typeMatch(const Symbol* s1, const Symbol* s2)
{
  //
  //	s1 and s2 are in different theories but they typeMatch if their respective components all typeMatch.
  //
  int nrArgs = s1->arity();
  if (s2->arity() != nrArgs)
    return false;
  for (int i = 0; i < nrArgs; ++i)
    {
      if (!typeMatch(s1->domainComponent(i), s2->domainComponent(i)))
	return false;
    }
  return typeMatch(s1->rangeComponent(), s2->rangeComponent());
}

bool
View::typeMatch(const RewriteStrategy* s1, const RewriteStrategy* s2)
{
  //
  //	Checks whether their domain componets match
  //
  int nrArgs = s1->arity();
  const Vector<Sort*>& domain1 = s1->getDomain();
  const Vector<Sort*>& domain2 = s2->getDomain();
  if (s2->arity() != nrArgs)
    return false;
  for (int i = 0; i < nrArgs; ++i)
    {
      if (!typeMatch(domain1[i]->component(), domain2[i]->component()))
	return false;
    }
  return true;
}

bool
View::getOpToTermMapping(Symbol* symbol, Term*& fromTerm, Term*& toTerm) const
{
  int id = symbol->id();
  const OpTermMap::const_iterator e = opTermMap.end();
  for (OpTermMap::const_iterator i = opTermMap.find(id); i != e && i->first == id; ++i)
    {
      if (typeMatch(i->second.first->symbol(), symbol))
	{
	  fromTerm = i->second.first;
	  toTerm = i->second.second;
	  return true;
	}
    }
  return false;
}

bool
View::getStratToExprMapping(RewriteStrategy* strat,
			    CallStrategy*& fromCall,
			    StrategyExpression*& toExpr,
			    const Vector<int>*& varIndices) const
{
  Vector<Term*> vars;
  int id = strat->id();
  const StratExprMap::const_iterator e = stratExprMap.end();
  for (StratExprMap::const_iterator i = stratExprMap.find(id); i != e && i->first == id; ++i)
    {
      if (typeMatch(i->second.call->getStrategy(), strat))
	{
	  fromCall = i->second.call;
	  toExpr = i->second.value;
	  varIndices = &i->second.contextSpec;
	  return true;
	}
    }
  return false;
}

bool
View::checkOps()
{
  //
  //	Now we check that for each operator
  //	  f : D1 ... Dn -> R
  //	in fromTheory, there exists (or can be instantiated) an operator
  //	  f' : D1' ... Dn' -> R'
  //	in toModule where the prime mappings are obtained from the view.
  //
  const Vector<Symbol*>& symbols = fromTheory->getSymbols();
  int nrUserSymbols =  fromTheory->getNrUserSymbols();
  for (int i = 0; i < nrUserSymbols; ++i)
    {
      Symbol* s = symbols[i];
      Assert(fromTheory->getSymbolType(s).getBasicType() != SymbolType::VARIABLE,
	     "didn't expect variable");
      Assert(fromTheory->getSymbolType(s).getBasicType() != SymbolType::SORT_TEST,
	     "didn't expect sort test");
      //
      //	Translate the name for those operators that have no module declarations.
      //
      int id = s->id();
      Term* dummy1;
      Term* dummy2;
      bool msgMapping = false;
      if (fromTheory->moduleDeclared(s))
	{
	  if (getOpToTermMapping(s, dummy1, dummy2) || renameOp(s) != NONE)
	    {
	      IssueAdvisory(*this << ": op mapping not applied to operator " << QUOTE(s) <<
			    " as it has at least one declaration in a module.");
	    }
	}
      else
	{
	  if (getOpToTermMapping(s, dummy1, dummy2))
	    continue;  // op is being mapped to a term which parses and therefore whose ops must exist
	  int index = renameOp(s);
	  if (index != NONE)
	    {
	      id = getOpTo(index);
	      msgMapping = isMsgMapping(index);
	    }
	}
      //
      //	Translate the domain and range components.
      //
      int nrArgs = s->arity();
      Vector<ConnectedComponent*> domainComponents(nrArgs);
      for (int j = 0; j < nrArgs; ++j)
	domainComponents[j] = mapComponent(s->domainComponent(j));
      ConnectedComponent* rangeComponent = mapComponent(s->rangeComponent());
      //
      //	Check to see that a suitable operator exists or can be made in toModule.
      //
      Symbol* t = toModule->findSymbol(id, domainComponents, rangeComponent);
      if (t == 0)
	{
	  IssueWarning(*this << ": failed to find suitable operator " << QUOTE(Token::name(id)) <<
		       " in " << QUOTE(toModule) << " to represent operator " << QUOTE(s) <<
		       " from " << QUOTE(fromTheory) << '.');
	  return false;
	}
      //
      //	Need to check that t can actually replace s. This is an issue if s or t are special
      //	symbols used to represent non-algebraic data types.
      //
      SymbolType st = fromTheory->getSymbolType(s);
      SymbolType tt = toModule->getSymbolType(t);
      if ((st.nonAlgebraic() || tt.nonAlgebraic()) &&
	  st.getBasicType() != tt.getBasicType())
	{
	  IssueWarning(*this << ": operator " << QUOTE(s) << " from " << QUOTE(fromTheory) <<
		       " maps to incompatible operator " << QUOTE(t) << " in " <<
		       QUOTE(toModule) << '.');
	  return false;
	}
      //
      //	Check that if mapping is via a msg mapping, both from and to symbols have the msg attribute.
      //
      if (msgMapping)
	{
	  AdvisoryCheck(st.hasFlag(SymbolType::MESSAGE), *this << ": theory symbol " << QUOTE(s) <<
			" is mapped by a msg mapping but does not have the msg attribute.");
	  AdvisoryCheck(tt.hasFlag(SymbolType::MESSAGE), *this << ": theory symbol " << QUOTE(s) <<
			" is mapped to " << QUOTE(t) <<
			" by a msg mapping, but the latter symbol does not have the msg attribute.");
	}
    }
  return true;
}

bool
View::checkPolymorphicOps()
{
  //
  //	Now we check that for each polymorphic operator
  //	  f : D1 ... Dn -> R
  //	in fromTheory, there exists (or can be instantiated) an operator
  //	  f' : D1' ... Dn' -> R'
  //	in toModule where the prime mappings are obtained from the view, and
  //	the null sort maps to the null sort.
  //
  int nrPolymorphs = fromTheory->getNrPolymorphs();
  for (int i = 0; i < nrPolymorphs; ++i)
    {
      //
      //	Translate the name.
      //
      int id = fromTheory->getPolymorphName(i).code();
      int index = renamePolymorph(id);
      bool msgMapping = false;
      if (index != NONE)
	{
	  if (fromTheory->moduleDeclaredPolymorph(i))
	    {
	      IssueAdvisory(*this << ": op mapping not applied to polymorphic operator " <<
			    QUOTE(Token::name(id)) << " as it was declared in a module.");
	    }
	  else
	    {
	      id = getOpTo(index);
	      msgMapping = isMsgMapping(index);
	    }
	}
      //
      //	Translate the domain and range components.
      //
      const Vector<Sort*>& domainAndRange = fromTheory->getPolymorphDomainAndRange(i);
      int nrSorts = domainAndRange.size();
      Vector<Sort*> newDomainAndRange(nrSorts);
      for (int j = 0; j < nrSorts; ++j)
	{
	  Sort* s = domainAndRange[j];
	  newDomainAndRange[j] = (s == 0) ? 0 : toModule->findSort(renameSort(s->id()));
	}
      //
      //	Check to see that a suitable polymorphic operator exists in toModule.
      //
      int toPolymorphIndex = toModule->findPolymorphIndex(id, newDomainAndRange);
      if (toPolymorphIndex == NONE)
	{
	  IssueWarning(*this << ": failed to find suitable polymorphic operator " << QUOTE(Token::name(id)) <<
		       " in " << QUOTE(toModule) << " to represent polymorphic operator " <<
		       QUOTE(fromTheory->getPolymorphName(i)) << " from " << QUOTE(fromTheory) << '.');
	  return false;
	}
      //
      //	Check that if mapping is via a msg mapping, both from and to symbols have the msg attribute.
      //
      if (msgMapping)
	{
	  SymbolType st = fromTheory->getPolymorphType(i);
	  AdvisoryCheck(st.hasFlag(SymbolType::MESSAGE), *this << ": theory polymorphic symbol " <<
			QUOTE(fromTheory->getPolymorphName(i)) <<
			" is mapped by a msg mapping but does not have the msg attribute.");

	  SymbolType tt = toModule->getPolymorphType(toPolymorphIndex);
	  AdvisoryCheck(tt.hasFlag(SymbolType::MESSAGE), *this << ": theory polymorphic symbol " <<
			QUOTE(fromTheory->getPolymorphName(i)) <<
			" is mapped to " << QUOTE(toModule->getPolymorphName(toPolymorphIndex)) <<
			" by a msg mapping, but the latter polymorphic symbol does not have the msg attribute.");
	}
    }
  return true;
}

bool
View::checkStrats()
{
  //
  //	Now we check that for each strategy
  //	  s : D1 ... Dn @ R
  //	in fromTheory, there exists (or can be instantiated) an strategy
  //	  s' : D1' ... Dn' @ R'
  //	in toModule where the prime mappings are obtained from the view.
  //
  const Vector<RewriteStrategy*>& strats = fromTheory->getStrategies();
  int nrStrats = strats.length();
  for (int i = 0; i < nrStrats; ++i)
    {
      RewriteStrategy* s = strats[i];
      //
      //	Translate the name for those operators that have no module declarations.
      //
      int id = s->id();
      CallStrategy* dummy1;
      StrategyExpression* dummy2;
      const Vector<int>* dummy3;
      if (fromTheory->moduleDeclared(s))
	{
	  if (getStratToExprMapping(s, dummy1, dummy2, dummy3) || renameStrat(s) != NONE)
	    {
	      IssueAdvisory(*this << ": strat mapping not applied to strategy " << QUOTE(s) <<
	      " as it has at least one declaration in a module.");
	    }
	}
      else
	{
	  if (getStratToExprMapping(s, dummy1, dummy2, dummy3))
	    continue;  // strat is being mapped to an expression which parses

	  int index = renameStrat(s);
	  if (index != NONE)
	    id = getStratTo(index);
	}
      //
      //	Translate the domain and range components.
      //
      int nrArgs = s->arity();
      const Vector<Sort*>& domain = s->getDomain();
      Vector<ConnectedComponent*> domainComponents(nrArgs);
      for (int j = 0; j < nrArgs; ++j)
	domainComponents[j] = mapComponent(domain[j]->component());
      //
      //	Check to see that a suitable strategy exists or can be made in toModule.
      //
      RewriteStrategy* t = toModule->findStrategy(id, domainComponents);
      if (!t)
	{
	  IssueWarning(*this << ": failed to find suitable strategy " << QUOTE(Token::name(id)) <<
	  " in " << QUOTE(toModule) << " to represent strategy " << QUOTE(s) <<
	  " from " << QUOTE(fromTheory) << '.');
	  return false;
	}
    }
  return true;
}

bool
View::evaluate()
{
  //
  //	See if view needs to be evaluated.
  //
  switch (status)
    {
    case INITIAL:
      {
	//
	//	Evaluating a view with OO mapping can add sort and op mappings to the
	//	base renaming. We need to keep track of what was original in case we
	//	need to purge these to re-evaluate.
	//
	recordUserMappings();
	break;
      }
    case PROCESSING:
      {
	IssueWarning(*this << ": recursive use of view " << QUOTE(this) << " is not allowed.");
	return false;
      }
    case GOOD:
      return true;
    case BAD:
      return false;
    case STALE:
      {
	IssueAdvisory("reexamining view " << static_cast<NamedEntity*>(this) << '.');
	break;
      }
    }
  status = PROCESSING;  // to catch reentrant use
  //
  //	Evaluate parameter theories.
  //
  const Vector<ParameterDecl>::iterator e = parameters.end();
  for (Vector<ParameterDecl>::iterator i = parameters.begin(); i != e; ++i)
    {
      if (ImportModule* pt = owner->makeModule(i->expr))
	{
	  i->theory = pt;
	  pt->addUser(this);
	  if (!(pt->isTheory()))
	    {
	      IssueWarning(*this << ": parameter theory " << QUOTE(i->expr) <<
			   " of view " << this << " is not a theory.");
	      status = BAD;
	    }
	}
      else
	{
	  DebugAdvisory(*this << ": view " << this << " couldn't make parameter theory " <<
			QUOTE(i->expr) << ".");
	  status = BAD;
	}
    }
  //
  //	Evaluate "from" part.
  //
  fromTheory = owner->makeModule(fromExpr, this);
  if (fromTheory != 0)
    {
      fromTheory->addUser(this);
      if (!(fromTheory->isTheory()))
	{
	  IssueWarning(*this << ": from part of a view must be a theory.");
	  status = BAD;
	}
      else if (!fromTheory->isStrategic() &&
	    (!stratExprMap.empty() || getNrStratMappings() > 0))
	{
	  IssueWarning(*this << ": view from a non-strategic theory cannot contain strategy mappings. Recovering by ignoring them.");

	  discardStratMappings();
	  stratExprMap.clear();
	}
    }
  else
    {
      DebugAdvisory("view " << this << " couldn't make fromTheory");
      status = BAD;
    }
  //
  //	Evaluate "to" part.
  //
  if (ImportModule* target = owner->makeModule(toExpr, this))
    {
      setToModule(target);
      if (target->hasFreeParameters())
	{
	  IssueWarning(*this << ": target of a view cannot have free parameters.");
	  status = BAD;
	}
      for (Vector<ParameterDecl>::iterator i = parameters.begin(); i != e; ++i)
	{
	  if (target->findParameterIndex(i->name) == NONE)
	    {
	      //
	      //	We refuse to handle parameterized views which have
	      //	parameters not used in the toModule. Such parameters are
	      //	useless: since they don't appear in the toModule their sorts,
	      //	operations and sorts parameterized by them cannot appear
	      //	in the range of the views mappings. They also allow construction
	      //	of hard-to-deal-with pathological cases.
	      //
	      //	Having all the parameters bound in the toModule means we can
	      //	use the toModule's machinery to do a lot of the heavy lifting:
	      //	(1) tracking dependencies on view arguments
	      //	(2) tracking conflicts
	      //	(3) creating the canonical renaming to instantiate the view's guts
	      //
	      IssueWarning(*this << ": parameter " << QUOTE(Token::name(i->name)) <<
			   " of view " << QUOTE(this) << " is not bound in target module " <<
			  QUOTE(toExpr) << ".");
	      status = BAD;
	    }
	}
    }
  else
    {
      DebugAdvisory("view " << this << " couldn't make toModule");
      status = BAD;
    }
  if (status == BAD)
    return false;

  status = GOOD;  // until proven otherwise

  if (!handleClassMappings() ||
      !handleAttrMappings() ||
      !checkSorts() ||
      !handleTermAndExprMappings() ||
      !checkOps() ||
      !checkPolymorphicOps() ||
      !checkStrats())
    {
      status = BAD;
      return false;
    }

  return true;
}

Sort*
View::mapSort(const Sort* sort) const
{
  if (sort->index() == Sort::KIND)
    return mapComponent(sort->component())->sort(Sort::KIND);
  Sort* transSort = toModule->findSort(renameSort(sort->id()));
  Assert(transSort != 0, "translation for sort failed");
  return transSort;
}

bool
View::indexRhsVariables(Term* term, const VarMap& varMap)
{
  //
  //	We recurse through term, looking up each variable we encounter
  //	in varMap, and setting its index to the corresponding argument
  //	index obtained from varMap.
  //
  if (VariableTerm* vt = dynamic_cast<VariableTerm*>(term))
    {
      //
      //	We look the variable up by name.
      //
      const VarMap::const_iterator i = varMap.find(vt->id());
      if (i == varMap.end() || i->second.first != vt->getSort())
	{
	  IssueWarning(*term  << ": right-hand side of operator-to-term mapping contains a variable " <<
		       QUOTE(term) << " which does not correspond to a left-hand side variable.");
	  return false;
	}
      vt->setIndex(i->second.second);
    }
  else
    { 
      for (ArgumentIterator i(*term); i.valid(); i.next())
	{
	  if (!indexRhsVariables(i.argument(), varMap))
	    return false;
	}
    }
  return true;
}

bool
View::insertOpToTermMapping(Term* fromTerm, Term* toTerm)
{
  //
  //	If something goes wrong we return false and caller is responsible
  //	for fromTerm and toTerm.
  //
  VarMap varMap;
  //
  //	First we check the arguments under fromTerm so see that
  //	they are all variables, and for X:Foo at index p, we make an entry
  //	X |-> (Bar, p) in varMap.
  //
  int argNr = 0;
  for (ArgumentIterator i(*fromTerm); i.valid(); i.next())
    {
      VariableTerm* vt = dynamic_cast<VariableTerm*>(i.argument());
      if (vt == 0)
	{
	  IssueWarning(*(i.argument()) << ": left-hand side " << QUOTE(fromTerm) <<
		       " of an operator-to-term mapping has non-variable argument " <<
		       QUOTE(i.argument()) << '.');
	  return false;
	}
      int base = vt->id();
      Sort* sort = mapSort(vt->getSort());
      pair<VarMap::iterator, bool> p = varMap.insert(VarMap::value_type(base, make_pair(sort, argNr)));
      if (!p.second)
	{
	  IssueWarning(*(i.argument()) <<
		       ": variable " << QUOTE(Token::name(base)) <<
		       " used more than once in the left-hand side " << QUOTE(fromTerm) <<
		       " of an operator-to-term mapping.");
	  return false;
	}
      ++argNr;
    }
  //
  //	It could be that fromTerm parses but has to many arguments because of user flattened syntax.
  //
  Symbol* fromSymbol = fromTerm->symbol();
  if (fromSymbol->arity() != argNr)
    {
      Assert(fromSymbol->arity() < argNr, "too few args");
      IssueWarning(*fromTerm << ": left-hand side " << QUOTE(fromTerm) << " of operator-to-term mapping has " <<
		   argNr << " arguments whereas 2 were expected.");
      return false;
    }
  //
  //	Check that each variable in toTerm has a matching entry in varMap. 
  //
  if (!indexRhsVariables(toTerm, varMap))
    return false;
  //
  //	For op f(...) to term g(...) we insert
  //	f |-> (f(...), g(...)) into opTermMap.
  //
  (void) opTermMap.insert(OpTermMap::value_type(fromSymbol->id(), make_pair(fromTerm, toTerm)));
  //
  //	We _do not_ insert this op->term map into our base Renaming. This is because
  //	when the view is re-evaluated we cannot change stuff in Renaming - essentially this
  //	stuff is considered original syntax that has no dependencies.
  //
  //	These op->term maps eventually make their way into canonicalRenamings using for
  //	instantiations of modules and views using this view. But these are more ephemeral
  //	and disappear if something under them changes.
  //
  return true;
}

bool
View::handleTermAndExprMappings()
{
  CantHappen("handleTermAndExprMappings() called on instantiated view " << this);
  return false;
}

bool
View::insertStratToExprMapping(CallStrategy* fromCall,
			       StrategyExpression* toExpr,
			       ImportModule* targetModule)
{
  //
  //	First we check the arguments under fromCall term so see that
  //	they are all variables.
  //
  TermSet lhsVars;
  Vector<Term*> vars;

  int argNr = 0;
  for (ArgumentIterator i(*fromCall->getTerm()); i.valid(); i.next())
    {
      VariableTerm* vt = dynamic_cast<VariableTerm*>(i.argument());
      if (vt == 0)
	{
	  IssueWarning(*(i.argument()) <<
		       ": lhs of strategy mapping has non-variable argument " <<
		       QUOTE(i.argument()) << '.');
	  delete fromCall;
	  delete toExpr;
	  for (Term* t : vars)
	    delete t;
	  return false;
	}
      int base = vt->id();
      Sort* sort = mapSort(vt->getSort());
      Term* toVar = new VariableTerm(safeCast(VariableSymbol*, targetModule->instantiateVariable(sort)), base);
      toVar->normalize(true);
      if (lhsVars.term2Index(toVar) != NONE)
	{
	  IssueWarning(*(i.argument()) <<
		       ": using the same variable base name " << QUOTE(Token::name(base)) <<
		       " for two left hand side variables in an strategy mapping is not allowed.");
	  delete fromCall;
	  delete toExpr;
	  delete toVar;
	  for (Term* t : vars)
	    delete t;
	  return false;
	}
      lhsVars.insert(toVar);
      vars.append(toVar);
      ++argNr;
    }
  //
  //	The number of arguments does not need to be checked because strategy call
  //	symbols are never flattened because they are free.
  //
  //	We check the rhs expression and build the context translation vector
  //
  VariableInfo vinfo;
  if (!toExpr->check(vinfo, lhsVars))
    {
      delete fromCall;
      delete toExpr;
      return false;
    }
  int nrVars = vinfo.getNrRealVariables();
  Vector<int> contextSpec(nrVars);
  for (int i = 0; i < nrVars; i++)
    {
      Term* var = vinfo.index2Variable(i);

      for (int j = 0; j < argNr; j++)
	if (var->equal(vars[j]))
	  contextSpec[i] = j;
    }

  for (Term* t : vars)
    delete t;

  //
  //	For strat s(...) to expr e we insert
  //	s |-> (s(...), e) into stratExprMap.
  //
  stratExprMap.insert(StratExprMap::value_type(fromCall->getStrategy()->id(),
					       StratExprInfo(fromCall, toExpr, contextSpec)));
  return true;
}

string
View::latexViewExpression(bool parameterBrackets) const
{
  if (baseView)
    {
      //
      //	We're an instantiation of a view.
      //
      string result = baseView->latexViewExpression(parameterBrackets);
      const char* sep = "\\maudeLeftBrace";
      for (const Argument* a : savedArguments)
	{
	  result += sep;
	  sep = "\\maudeComma\\maudeSpace";
	  if (const View* v = dynamic_cast<const View*>(a))
	    result += v->latexViewExpression(parameterBrackets);
	  else if (const Parameter* p = dynamic_cast<const Parameter*>(a))
	    {
	      if (parameterBrackets)
		result += "[";
	      result += "\\maudeParameter{";
	      result += Token::latexName(p->id());
	      result += "}";
	      if (parameterBrackets)
		result += "]";
	    }
	}
      result += "\\maudeRightBrace";
      return result;
    }
  string result = "\\maudeView{";
  result += Token::latexName(id());
  result += "}";
  return result;
}
