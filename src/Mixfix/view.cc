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
//      Implementation for class View.
//

//      utility stuff
#include "macros.hh"
#include "vector.hh"

//      forward declarations
#include "core.hh"
#include "interface.hh"
#include "variable.hh"
#include "strategyLanguage.hh"
#include "mixfix.hh"

//      interface class definitions
#include "symbol.hh"
#include "dagNode.hh"
#include "term.hh"

//      core class definitions
#include "argumentIterator.hh"

//      variable class definitions
#include "variableTerm.hh"

//	front end class definitions
#include "token.hh"
#include "moduleExpression.hh"
#include "preModule.hh"
#include "view.hh"

View::View(Token viewName)
  : NamedEntity(viewName.code()),
    LineNumber(viewName.lineNumber())
{
  fromTheory = 0;
  toModule = 0;
  newFromTheory = 0;
  newToModule = 0;
  status = INITIAL;
}

View::~View()
{
  clearOpTermMap();
  if (fromTheory != 0)
    fromTheory->removeUser(this);
  if (toModule != 0)
    toModule->removeUser(this);
  if (newFromTheory != 0)
    newFromTheory->deepSelfDestruct();
  if (newToModule != 0)
    newToModule->deepSelfDestruct();
  fromExpr->deepSelfDestruct();
  toExpr->deepSelfDestruct();
  informUsers();
}

void
View::clearOpTermMap()
{
  FOR_EACH_CONST(i, OpTermMap, opTermMap)
    {
      i->second.first->deepSelfDestruct();
      i->second.second->deepSelfDestruct();
    }
  opTermMap.clear();
}

void
View::addVarDecl(Token varName)
{
  varDecls.push_back(VarDecl());
  varDecls.back().varName = varName;
  varDecls.back().lastWithCurrentDef = false;
}

void
View::addType(bool kind, const Vector<Token>& tokens)
{
  if (varDecls.empty() || varDecls.back().lastWithCurrentDef)
    Renaming::addType(kind, tokens);  // not ours
  else
    {
      varDecls.back().lastWithCurrentDef = true;
      varDefs.push_back(Type());
      varDefs.back().kind = kind;
      varDefs.back().tokens = tokens;
    }
}

void
View::addOpTermMapping(const Vector<Token>& fromOp, const Vector<Token>& toTerm)
{
  opTermList.push_back(BubblePair());
  opTermList.back().fromBubble = fromOp;  // deep copy
  opTermList.back().toBubble = toTerm;  // deep copy
}

void
View::regretToInform(Entity* doomedEntity)
{
  if (doomedEntity == fromTheory)
    fromTheory = 0;
  else if (doomedEntity == newFromTheory)
    newFromTheory = 0;
  else if(doomedEntity == toModule)
    toModule = 0;
  else if(doomedEntity == newToModule)
    newToModule = 0;
  else
    CantHappen("unexpected regretToInform()");
  //
  //	Something we depended on changed so self destruct all calculated stuff.
  //
  clearOpTermMap();
  if (fromTheory != 0)
    {
      fromTheory->removeUser(this);
      fromTheory = 0;
    }
  if (newFromTheory != 0)
    {
      newFromTheory->deepSelfDestruct();
      newFromTheory = 0;
    }
  if (toModule != 0)
    {
      toModule->removeUser(this);
      toModule = 0;
    }
  if (newToModule != 0)
    {
      newToModule->deepSelfDestruct();
      newToModule = 0;
    }
  status = STALE;
}

ConnectedComponent*
View::mapComponent(const ConnectedComponent* component, ImportModule* module) const
{
  Sort* sort = module->findSort(renameSort(component->sort(1)->id()));
  Assert(sort != 0, "translation for sort failed");
  return sort->component();
}

Sort*
View::mapSort(const Sort* sort, ImportModule* module) const
{
  if (sort->index() == Sort::KIND)
    return mapComponent(sort->component(), module)->sort(Sort::KIND);
  Sort* transSort = module->findSort(renameSort(sort->id()));
  Assert(transSort != 0, "translation for sort failed");
  return transSort;
}

void
View::finishView()
{
  evaluate();
}

void
View::finishModule1(ImportModule* module)
{
  module->importSorts();
  module->closeSortSet();
  module->importOps();
}

void
View::finishModule2(ImportModule* module)
{
  module->closeSignature();
  module->fixUpImportedOps();
  module->closeFixUps();
  module->localStatementsComplete();
  module->resetImports();
}

bool
View::checkSorts()
{
  //
  //	We check that for each pair of sorts (A, B) lying in the same kind in
  //	toTheory, there exists under our mapping sorts (A', B') that are in 
  //	the same kind in toModule, and further more if A <= B then A' <= B'.
  //
  const Vector<ConnectedComponent*> kinds = fromTheory->getConnectedComponents();
  FOR_EACH_CONST(i, Vector<ConnectedComponent*>, kinds)
    {
      ConnectedComponent* kind = (*i);
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
	  ConnectedComponent* transKind = jTransSort->component();
	  for (int k = 1; k < j; ++k)
	    {
	      Sort* kSort = kind->sort(k);
	      int kId = kSort->id();
	      if (!(fromTheory->moduleDeclared(kSort)))
		kId = renameSort(kId);
	      Sort* kTransSort = toModule->findSort(kId);
	      if (jTransSort->component() != kTransSort->component())
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
  return true;
}

bool
View::handleVarDecls()
{
  Sort* fromSort = 0;
  Sort* toSort = 0;
  TypeList::const_iterator j = varDefs.begin();
  FOR_EACH_CONST(i, VarDeclList, varDecls)
    {
      if (fromSort == 0)
	{
	  int code = j->tokens[0].code();
	  fromSort = newFromTheory->findSort(code);
	  if (fromSort == 0)
	    {
	      IssueWarning(LineNumber(j->tokens[0].lineNumber()) <<
			   ": failed to find sort " << QUOTE(Token::sortName(code)) <<
			   " in " << QUOTE(newFromTheory) << '.');
	      return false;
	    }
	  toSort = newToModule->findSort(renameSort(code));
	  Assert(toSort != 0, "couldn't find translation of sort");
	  if (j->kind)
	    {
	      int nrTokens = j->tokens.size();
	      for (int k = 1; k < nrTokens; ++k)
		{
		  int code = j->tokens[k].code();
		  Sort* extraSort = newFromTheory->findSort(code);
		  if (extraSort == 0)
		    {
		      IssueWarning(LineNumber(j->tokens[k].lineNumber()) <<
				   ": failed to find sort " << QUOTE(Token::sortName(code)) <<
				   " in " << QUOTE(newFromTheory) << '.');
		      return false;
		    }
		  if (extraSort->component() != fromSort->component())
		    {
		      IssueWarning(LineNumber(j->tokens[k].lineNumber()) <<
				   ": sorts " << QUOTE(fromSort) << " and " <<
				   QUOTE(extraSort) << " are in different components.");
		      return false;
		    }
		}
	      fromSort = fromSort->component()->sort(Sort::KIND);
	      toSort = toSort->component()->sort(Sort::KIND);
	    }
	  ++j;
	}
      newFromTheory->addVariableAlias(i->varName, fromSort);
      newToModule->addVariableAlias(i->varName, toSort);
      if (i->lastWithCurrentDef)
	fromSort = 0;
    }
  return true;
}

bool
View::indexRhsVariables(Term* term, const VarMap& varMap, int lineNr)
{
  if (VariableTerm* vt = dynamic_cast<VariableTerm*>(term))
    {
      const VarMap::const_iterator i = varMap.find(vt->id());
      if (i == varMap.end() || i->second.first != vt->getSort())
	{
	  IssueWarning(LineNumber(lineNr) <<
		       ": rhs of operator mapping contains a variable " <<
		       QUOTE(term) << " which is not the mapping of a lhs variable.");
	  return false;
	}
      vt->setIndex(i->second.second);
    }
  else
    { 
      for (ArgumentIterator i(*term); i.valid(); i.next())
	{
	  if (!indexRhsVariables(i.argument(), varMap, lineNr))
	    return false;
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

Term*
View::getOpMapTerm(Symbol* symbol) const
{
  int id = symbol->id();
  const OpTermMap::const_iterator e = opTermMap.end();
  for (OpTermMap::const_iterator i = opTermMap.find(id); i != e && i->first == id; ++i)
    {
      if (typeMatch(i->second.first->symbol(), symbol))
	return i->second.second;
    }
  return 0;
}

bool
View::handleOpTermMappings()
{
  LineNumber lineNumber(FileTable::AUTOMATIC);
  //
  //	Because we have op->term mappings we need to make new modules to parse
  //	these mappings in, in order to exclude any existing variable aliases and
  //	add any new ones.
  //
  newFromTheory = new ImportModule(fromTheory->id(),
				   fromTheory->getModuleType(),
				   ImportModule::VIEW_LOCAL,
				   this);
  newFromTheory->addImport(fromTheory, ImportModule::INCLUDING, lineNumber);
  finishModule1(newFromTheory);
  
  newToModule = new ImportModule(toModule->id(),
				 toModule->getModuleType(),
				 ImportModule::VIEW_LOCAL,
				 this);
  newToModule->addImport(toModule, ImportModule::INCLUDING, lineNumber);
  finishModule1(newToModule);
  
  if (!varDecls.empty() && !handleVarDecls())
    return false;
  finishModule2(newFromTheory);
  finishModule2(newToModule);
  //
  //
  //	Now deal with op->term mappings.
  //
  FOR_EACH_CONST(i, OpTermList, opTermList)
    {
      Term* from = newFromTheory->parseTerm(i->fromBubble);
      if (from == 0)
	return false;
      int argNr = 0;
      VarMap varMap;
      for (ArgumentIterator j(*from); j.valid(); j.next())
	{
	  VariableTerm* vt = dynamic_cast<VariableTerm*>(j.argument());
	  if (vt == 0)
	    {
	      IssueWarning(LineNumber(i->fromBubble[0].lineNumber()) <<
			   ": lhs of operator mapping has non-variable argument " <<
			   QUOTE(j.argument()) << '.');
	      from->deepSelfDestruct();
	      return false;
	    }
	  int base = vt->id();
	  Sort* sort = mapSort(vt->getSort(), newToModule);
	  pair<VarMap::iterator, bool> p = varMap.insert(VarMap::value_type(base, make_pair(sort, argNr)));
	  if (!p.second)
	    {
	      IssueWarning(LineNumber(i->fromBubble[0].lineNumber()) <<
			   ": using the same variable name " << QUOTE(Token::name(base)) <<
			   " for two lhs variables in an operator mapping is not allowed.");
	      from->deepSelfDestruct();
	      return false;
	    }
	  ++argNr;
	}
      Symbol* fromSymbol = from->symbol();
      if (fromSymbol->arity() != argNr)
	{
	  Assert(fromSymbol->arity() < argNr, "too few args");
	  IssueWarning(LineNumber(i->fromBubble[0].lineNumber()) <<
		       ": lhs of operator mapping has too many arguments.");
	  from->deepSelfDestruct();
	  return false;
	}
      
     Term* to = newToModule->parseTerm(i->toBubble, mapComponent(fromSymbol->rangeComponent(), newToModule), 1);
      if (to == 0)
	{
	  from->deepSelfDestruct();
	  return false;
	}
      if (!indexRhsVariables(to, varMap, i->toBubble[1].lineNumber()))
	{
	  from->deepSelfDestruct();
	  to->deepSelfDestruct();
	  return false;
	}
      opTermMap.insert(OpTermMap::value_type(from->symbol()->id(), make_pair(from, to)));
    }
  //
  //	Dispense with bulky parsers.
  //
  newFromTheory->economize();
  newToModule->economize();
  return true;
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
      if (fromTheory->moduleDeclared(s))
	{
	  if (getOpMapTerm(s) != 0 || renameOp(s) != NONE)
	    {
	      IssueAdvisory(*this << ": op mapping not applied to operator " << QUOTE(s) <<
			    " as it has at least one declaration in a module.");
	    }
	}
      else
	{
	  if (getOpMapTerm(s) != 0)
	    continue;  // op is being mapped to a term which parses and therefore whose ops must exist
	  int index = renameOp(s);
	  if (index != NONE)
	    id = getOpTo(index);
	}
      //
      //	Translate the domain and range components.
      //
      int nrArgs = s->arity();
      Vector<ConnectedComponent*> domainComponents(nrArgs);
      for (int j = 0; j < nrArgs; ++j)
	domainComponents[j] = mapComponent(s->domainComponent(j), toModule);
      ConnectedComponent* rangeComponent = mapComponent(s->rangeComponent(), toModule);
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
      if (index != NONE)
	{
	  if (fromTheory->moduleDeclaredPolymorph(i))
	    {
	      IssueAdvisory(*this << ": op mapping not applied to polymorphic operator " <<
			    QUOTE(Token::name(id)) << " as it was declared in a module.");
	    }
	  else
	    id = getOpTo(index);
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
      if (toModule->findPolymorphIndex(id, newDomainAndRange) == NONE)
	{
	  IssueWarning(*this << ": failed to find suitable polymorphic operator " << QUOTE(Token::name(id)) <<
		       " in " << QUOTE(toModule) << " to represent polymorphic operator " <<
		       QUOTE(fromTheory->getPolymorphName(i)) << " from " << QUOTE(fromTheory) << '.');
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
      break;
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
  //
  //	Evaluate from part.
  //
  fromTheory = PreModule::makeModule(fromExpr);
  if (fromTheory != 0)
    {
      fromTheory->addUser(this);
      if (!(fromTheory->isTheory()))
	{
	  IssueWarning(*this << ": from part of a view must be a theory.");
	  status = BAD;
	}
    }
  else
    {
      DebugAdvisory("view " << this << " couldn't make fromTheory");
      status = BAD;
    }
  //
  //	Evaluate to part.
  //
  toModule = PreModule::makeModule(toExpr);
  if (toModule != 0)
    {
      toModule->addUser(this);
      if (toModule->getNrParameters() > 0)
	{
	  IssueWarning(*this << ": target of a view cannot have free parameters.");
	  status = BAD;
	}
    }
  else
    {
      DebugAdvisory("view " << this << " couldn't make toModule");
      status = BAD;
    }
  if (status == BAD)
    return false;

  if (!checkSorts() ||
      (!opTermList.empty() && !handleOpTermMappings()) ||
      !checkOps() ||
      !checkPolymorphicOps())
    {
      status = BAD;
      return false;
    }
  status = GOOD;
  return true;
}

void
View::showView(ostream& s)
{
  s << "view " << static_cast<NamedEntity*>(this) << " from " <<
    fromExpr << " to " << toExpr << " is\n";
  printRenaming(s, "  ", " .\n  ");
  if (getNrSortMappings() > 0 || getNrOpMappings() > 0)
    s << " .\n";
  if (!varDecls.empty())
    {
      bool startNew = true;
      TypeList::const_iterator j = varDefs.begin();
      FOR_EACH_CONST(i, VarDeclList, varDecls)
	{
	  if (startNew)
	    {
	      s << "  var";
	      if (!(i->lastWithCurrentDef))
		s << 's';
	      startNew = false;
	    }
	  s << ' ' << i->varName;
	  if (i->lastWithCurrentDef)
	    {
	      s << " : " << *j << " .\n";
	      ++j;
	      startNew = true;
	    }
	}
    }
  FOR_EACH_CONST(i, OpTermMap, opTermMap)
    s << "  op " << i->second.first << " to term " << i->second.second << " .\n";
  s << "endv\n";
}
