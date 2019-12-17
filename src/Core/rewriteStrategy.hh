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
//		Class for strategies which control the rewriting process.
//		(not to be confused with those about argument evaluation)
//
#ifndef _rewriteStrategy_hh_
#define _rewriteStrategy_hh_
#include "macros.hh"
#include "sort.hh"

class RewriteStrategy : public NamedEntity, public LineNumber, public ModuleItem
{
  NO_COPYING(RewriteStrategy);

public:
  RewriteStrategy(int id, const Vector<Sort*>& domain, Sort* subjectSort, Symbol* auxSymbol);

  //
  // Get the auxiliary tuple symbol which is used for matching strategy calls
  // in definitions.
  //
  // Explicit access to this symbol is not needed but it is used for eficency
  // when strategies are created or imported.
  //
  Symbol* getSymbol() const;

  //
  // The strategy is intented to rewrite terms of the this sort.
  // This information is not used and no checks are done.
  //
  Sort* getSubjectSort() const;

  const Vector<Sort*>& getDomain() const;
  int arity() const;

  //
  // Functions for the auxiliary term
  //
  Term* makeAuxiliaryTerm(const Vector<Term*>& args) const;
  Term* copyAuxiliaryTerm(Term* term, SymbolMap* translation) const;

  //
  // Functions for a table of definitions
  //
  void addDefinition(StrategyDefinition* sdef);
  const Vector<StrategyDefinition*>& getDefinitions() const;

  //
  // A strategy is simple if it has no arguments and is defined by a single
  // unconditional definition.
  //
  bool isSimple() const;

private:
  Vector<Sort*> domain;
  Sort* subjectSort;

  Symbol* symbol;
  Vector<StrategyDefinition*> definitions;
  bool simple;
};

inline Symbol*
RewriteStrategy::getSymbol() const
{
  return symbol;
}

inline Sort*
RewriteStrategy::getSubjectSort() const
{
  return subjectSort;
}

inline Term*
RewriteStrategy::makeAuxiliaryTerm(const Vector<Term*>& args) const
{
  return symbol->makeTerm(args);
}

inline int
RewriteStrategy::arity() const
{
  return domain.length();
}

inline const Vector<Sort*>&
RewriteStrategy::getDomain() const
{
  return domain;
}

inline const Vector<StrategyDefinition*>&
RewriteStrategy::getDefinitions() const
{
  return definitions;
}

inline bool
RewriteStrategy::isSimple() const
{
  return simple;
}

//
//      Output function for RewriteStrategy must be defined by library user.
//
ostream& operator<<(ostream& s, const RewriteStrategy* rs);

#endif
