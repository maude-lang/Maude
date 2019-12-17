/*

    This file is part of the Maude 3 interpreter.

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
//	Class to translate symbols from imported module to importing module.
//
#ifndef _importTranslation_hh_
#define _importTranslation_hh_
#include <list>
#include "strategyLanguage.hh"
#include "symbolMap.hh"
#include "pointerMap.hh"

class ImportTranslation : public SymbolMap
{
  NO_COPYING(ImportTranslation);

public:
  ImportTranslation(ImportModule* target, Renaming* renaming = 0);
  void push(Renaming* renaming, ImportModule* target);
  //
  //	These three functions are required by our base class.
  //
  Symbol* translate(Symbol* symbol);  // returns 0 to indicate op->term mapping in play
  Term* translateTerm(const Term* term);  // handles op->term mappings on a whole term basis
  Symbol* findTargetVersionOfSymbol(Symbol* symbol);
  //
  //	Other public functions that we provide.
  //
  Sort* translate(const Sort* sort);
  ConnectedComponent* translate(const ConnectedComponent* component);
  int translateLabel(int id);
  RewriteStrategy* translate(RewriteStrategy* strat);
  StrategyExpression* translateExpr(const CallStrategy* cs);

private:
  ImportTranslation();
  //
  //	Typically we have a list of renamings that move stuff from module to module
  //	until we arrive at the final target module.
  //
  typedef list<Renaming*> RenamingList;
  typedef list<ImportModule*> ModuleList;

  static ConnectedComponent* translate(Renaming* renaming,
				       ImportModule* target,
				       const ConnectedComponent* old);
  Symbol* translateRegularSymbol(Symbol* symbol,
				 RenamingList::const_iterator& opToTerm,
				 int& opToTermIndex) const;
  RewriteStrategy* translateStrategy(RewriteStrategy* strat,
				     RenamingList::const_iterator& stratToExpr,
				     int& stratToExprIndex) const;

  void splitTranslation(RenamingList::const_iterator firstMapping,
			ImportTranslation*& prefix,
			ImportTranslation*& suffix);

  RenamingList renamings;
  //
  //	Usually we only need the last target module because that's the one
  //	we're translating into. But in the case of an op->term mapping
  //	we may need to split the ImportTranslation and to do this we
  //	need an intermediate target.
  //
  ModuleList targets;
  //
  //	Because translating symbols is the most frequent operation and also
  //	fairly expensive, we cache translations here.
  //
  PointerMap directMap;
};

inline
ImportTranslation::ImportTranslation()
{
}

inline ConnectedComponent*
ImportTranslation::translate(const ConnectedComponent* component)
{
  return translate(component->sort(1))->component();
}

inline void
ImportTranslation::push(Renaming* renaming, ImportModule* target)
{
  renamings.push_front(renaming);
  targets.push_front(target);
}

#endif
