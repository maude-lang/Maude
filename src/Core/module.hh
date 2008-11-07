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
//	Class for modules
//
#ifndef _module_hh_
#define _module_hh_
#include "namedEntity.hh"
#include "lineNumber.hh"
#include "badFlag.hh"
#include "sort.hh"
#include "connectedComponent.hh"
#include "symbol.hh"

class Module : public NamedEntity, public LineNumber, public BadFlag
{
  NO_COPYING(Module);

public:
  enum Status
  {
    OPEN,
    SORT_SET_CLOSED,
    SIGNATURE_CLOSED,
    FIX_UPS_CLOSED,
    THEORY_CLOSED
  };

  Module(int id);
  virtual ~Module();

  virtual void closeSortSet();
  virtual void closeSignature();
  virtual void closeFixUps();
  virtual void closeTheory();
  Status getStatus() const;
  const Vector<Sort*>& getSorts() const;
  const Vector<Symbol*>& getSymbols() const;
  const Vector<ConnectedComponent*>& getConnectedComponents() const;
  const Vector<SortConstraint*>& getSortConstraints() const;
  const Vector<Equation*>& getEquations() const;
  const Vector<Rule*>& getRules() const;
  const SortBdds* getSortBdds();
  void insertSort(Sort* sort);
  void insertSymbol(Symbol* symbol);
  void insertSortConstraint(SortConstraint* sortConstraint);
  void insertEquation(Equation* equation);
  void insertRule(Rule* rule);
  void insertLateSymbol(Symbol*s);
  int getMinimumSubstitutionSize() const;
  void notifySubstitutionSize(int minimumSize);
  //
  //	Call the appropriate function on each symbol.
  //
  virtual void reset();  // clear misc caches for each symbol
  void clearMemo();  // clear memo table for each symbol
  void resetRules();  // clear rule hidden state
  void saveHiddenState();  // save rule hidden state
  void restoreHiddenState();  // restore rule hidden state

#ifdef DUMP
  void dump(ostream& s);
#endif

private:
  void indexSortConstraints();
  void indexEquation(Equation* eq);
  void indexEquations();
  void indexRules();

  Status status;
  Vector<Sort*> sorts;
  Vector<ConnectedComponent*> connectedComponents;
  Vector<Symbol*> symbols;
  Vector<SortConstraint*> sortConstraints;
  Vector<Equation*> equations;
  Vector<Rule*> rules;
  SortBdds* sortBdds;
  int minimumSubstitutionSize;
};

inline Module::Status
Module::getStatus() const
{
  return status;
}

inline const Vector<Sort*>&
Module::getSorts() const
{
  return sorts;
}

inline const Vector<ConnectedComponent*>&
Module::getConnectedComponents() const
{
  return connectedComponents;
}

inline const Vector<Symbol*>&
Module::getSymbols() const
{
  return symbols;
}

inline const Vector<SortConstraint*>&
Module::getSortConstraints() const
{
  return sortConstraints;
}

inline const Vector<Equation*>&
Module::getEquations() const
{
  return equations;
}

inline const Vector<Rule*>&
Module::getRules() const
{
  return rules;
}

inline void
Module::insertSort(Sort* sort)
{
  Assert(status == OPEN, cerr << "bad status");
  sort->setModuleInfo(this, sorts.length());
  sorts.append(sort);
}

inline void
Module::insertSymbol(Symbol* symbol)
{
  Assert(status < SIGNATURE_CLOSED, cerr << "bad status");
  symbol->setModuleInfo(this, symbols.length());
  symbols.append(symbol);
}

inline int
Module::getMinimumSubstitutionSize() const
{
  return minimumSubstitutionSize;
}

inline void
Module::notifySubstitutionSize(int minimumSize)
{
  if (minimumSize > minimumSubstitutionSize)
    {
      DebugAdvisory("minimumSubstitutionSize for " << this << " increased from " <<
		    minimumSubstitutionSize << " to " << minimumSize);
      minimumSubstitutionSize = minimumSize;
    }
}

#endif
