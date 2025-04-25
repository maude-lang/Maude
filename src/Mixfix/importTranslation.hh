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
//	Class to translate symbols from imported module to importing module.
//
#ifndef _importTranslation_hh_
#define _importTranslation_hh_
#include <list>
#include <unordered_map>
#include "strategyLanguage.hh"
#include "symbolMap.hh"

class ImportTranslation : public SymbolMap
{
  NO_COPYING(ImportTranslation);

public:
  ImportTranslation(ImportModule* target,
		    Renaming* renaming = nullptr,
		    bool preserveVariableIndicesFlag = false,
		    bool liftVariablesToKind = false);
  void push(Renaming* renaming, ImportModule* target);
  //
  //	These three functions are required by our base class.
  //
  Symbol* translate(Symbol* symbol) override;  // returns 0 to indicate op->term mapping in play
  Term* translateTerm(const Term* term) override;  // handles op->term mappings on a whole term basis
  Symbol* findTargetVersionOfSymbol(Symbol* symbol) override;
  bool preserveVariableIndices() override;
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
  typedef unordered_map<Symbol*, Symbol*> SymbolPointerMap;
  typedef unordered_map<RewriteStrategy*, RewriteStrategy*> StrategyMap;

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
  SymbolPointerMap symbolMap;
  StrategyMap strategyMap;
  //
  //	When we copy a statement it is important that variable indices are reset
  //	to UNDEFINED so the statement compiles correctly. However if we're copying
  //	the right-hand side of an op to term mapping, we need to preserve the indices
  //	we obtained when analyzing the view.
  //
  const bool preserveVariableIndicesFlag = false;
  const bool liftVariablesToKind = false;
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
