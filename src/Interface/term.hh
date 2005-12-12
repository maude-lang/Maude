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
//	Abstract base class for terms.
//
#ifndef _term_hh_
#define _term_hh_
#include "lineNumber.hh"
#include "sort.hh"
#include "symbol.hh"
#include "dagNode.hh"
#include "natSet.hh"
#include "pointerSet.hh"
#include "termSet.hh"
#include "variableSymbol.hh"

class Term : public LineNumber
{
  NO_COPYING(Term);

  static bool discard;

public:
  enum ReturnValues
  {
    GREATER = 1,
    LESS = -2,
    EQUAL = 0,
    UNKNOWN = -1
  };

  Term(Symbol* symbol);
  virtual ~Term() {}
  //
  //	These member functions should not be overridden
  //
  Symbol* symbol() const;
  bool ground() const;
  bool stable() const;
  const NatSet& occursBelow() const;
  const NatSet& occursInContext() const;
  const PointerSet& collapseSymbols() const;
  unsigned int getHashValue() const;
  Term* deepCopy(SymbolMap* translation = 0) const;
  Term* instantiate(const Vector<Term*>& varBindings, SymbolMap* translation);
  int compare(const Term* other) const;
  int compare(const DagNode* other) const;
  bool equal(const Term* other) const;
  bool equal(const DagNode* other) const;
  bool leq(const Sort* sort) const;
  DagNode* term2Dag(bool setSortInfo = false);
  DagNode* dagify();
  void indexVariables(VariableInfo& indicies);
  void determineContextVariables();
  void markEager(int nrVariables,
		 const NatSet& eagerVariables,
		 Vector<int>& problemVariables);
  ConnectedComponent* getComponent() const;
  int getSortIndex() const;
  Sort* getSort() const;
  void setSortInfo(ConnectedComponent* component, int index);
  int getSaveIndex() const;
  bool matchIndependent(const Term* other) const;
  bool honorsGroundOutMatch() const;
  void setHonorsGroundOutMatch(bool flag);
  bool willGroundOutMatch(const NatSet& boundUniquely) const;
  void analyseCollapses();
  bool hasEagerContext() const;
  bool greedySafe(const VariableInfo& variableInfo, const NatSet& boundUniquely) const;
  LhsAutomaton* compileLhs(bool matchAtTop,
			   const VariableInfo& variableInfo,
			   NatSet& boundUniquely,
			   bool& subproblemLikely);
  int compileRhs(RhsBuilder& rhsBuilder,
		 VariableInfo& variableInfo,
		 TermBag& availableTerms,
		 bool eagerContext);
  void compileTopRhs(RhsBuilder& rhsBuilder,
		     VariableInfo& variableInfo,
		     TermBag& availableTerms);
  //
  //	These member functions must be defined for each derived class
  //
  virtual RawArgumentIterator* arguments() = 0;
  virtual void deepSelfDestruct() = 0;
  virtual Term* deepCopy2(SymbolMap* translator) const = 0;
  virtual Term* normalize(bool full, bool& changed = discard) = 0;
  virtual int compareArguments(const Term* other) const = 0;
  virtual int compareArguments(const DagNode* other) const = 0;
  virtual void findAvailableTerms(TermBag& availableTerms,
				  bool eagerContext,
				  bool atTop = false) = 0;
  virtual void findEagerVariables(bool atTop, NatSet& eagerVariables) const = 0;
  virtual void analyseConstraintPropagation(NatSet& boundUniquely) const = 0;
  virtual LhsAutomaton* compileLhs2(bool matchAtTop,
				   const VariableInfo& variableInfo,
				   NatSet& boundUniquely,
				   bool& subproblemLikely) = 0;
  virtual void markEagerArguments(int nrVariables,
				  const NatSet& eagerVariables,
				  Vector<int>& problemVariables) = 0;
  virtual int compileRhs2(RhsBuilder& rhsBuilder,
			 VariableInfo& variableInfo,
			 TermBag& availableTerms,
			  bool eagerContext) = 0;
  virtual DagNode* dagify2() = 0;
  //
  //	The following function returns true if the lhs automaton for "this"
  //	is guarenteed to return "false" on any instance of "other" when running
  //	without extension and an empty substitution. It is used to optimize the
  //	compilation of patterns and can be defined conservatively --- returning
  //	false is always OK.
  //
  virtual bool earlyMatchFailOnInstanceOf(const Term* other) const;
  //
  //	These member functions compute approximate relations between terms;
  //	i.e. they can return UNDECIDED whenever the true relation is too
  //	expensive to evaluate (or even undecidable). The default versions
  //	return UNDECIDED most of the time. Theory specific versions may be able
  //	to do better but must be prepared to handle UNDECIDED results from
  //	alien subterms. These functions are intended for use in optimizing
  //	lhs and rhs automata; if a result is UNDECIDED the optimization is quietly
  //	forgotten.
  //
  //	The following function returns true if any subject that matches "other"
  //	is guarenteed to match "this" taking into account nonlinear variables
  //	in the context. It can be defined conservatively --- returning
  //	false is always OK. The sameVariableSet flag indicates whether
  //	variables in this are the same as (and thus trivially subsume) variables
  //	with the same name in "other". When both subterms are from the same
  //	pattern it should be set to true; when they are from different patterns
  //	it should be set to false since f(X, Y) does not subsume f(a, Y) if
  //	Y is already bound in the context of f(X, Y).
  //
  virtual bool subsumes(const Term* other, bool sameVariableSet) const;
  //
  //	This pair of functions does the preprocessing needed to handle collapse
  //	theories correctly. analyseCollapses2() computes the set of symbols that
  //	could be the top symbol of the term following collapses caused by bindings
  //	to its variables. The default version will work for symbols with
  //	collapse-free theories. insertAbstractionVariables() inserts abstraction
  //	variables where needed to help out the matching algorithm by replacing
  //	tricky subpatterns with variables, whose binding will be matched against
  //	the subpattern at solve time. The default version will work for symbols
  //	whose matching algorithm does not need abstraction variables.
  //
  virtual void analyseCollapses2();
  virtual void insertAbstractionVariables(VariableInfo& variableInfo);
  //
  //	Runtime heuristic partial camparison between a term with variables
  //	and a dag, with a substitution that may bind some of the variables.
  //
  int partialCompare(const Substitution& partialSubstitution,
		     DagNode* other) const;
  virtual int partialCompareUnstable(const Substitution& partialSubstitution,
				      DagNode* other) const;
  virtual int partialCompareArguments(const Substitution& partialSubstitution,
				      DagNode* other) const;
  //
  //	The following function should be redefined for any subclass that needs a
  //	non-naive treatment of it's arguments or has hidden data.
  //
  virtual Term* instantiate2(const Vector<Term*>& varBindings, SymbolMap* translator);

#ifdef COMPILER
  void generateRhs(CompilationContext& context,
		   int indentLevel,
		   const Vector<VariableName>& varNames,
		   Symbol* lhsSymbol);
  int gatherPartialResults(int nrVariables,
			   TermSet& compiled,
			   Vector<Symbol*>& symbols,
			   Vector<Vector<int> >& argLists);
#endif

#ifdef DUMP
  //
  //	dump() routine is optional; Default will dump common stuff togther with args
  //	in naive way. Replacement routine should call dumpCommon() to dump stuff in
  //	base class.
  //
  virtual void dump(ostream& s, const VariableInfo& variableInfo, int indentLevel = 0);
  void dumpCommon(ostream& s, const VariableInfo& variableInfo, int indentLevel);

private:
  static void dumpVariableSet(ostream& s,
			      const NatSet& variableSet,
			      const VariableInfo& variableInfo);
  static void dumpSymbolSet(ostream& s, const PointerSet& symbolSet);

#endif

protected:
  static unsigned int hash(unsigned int v1, unsigned int v2);
  static unsigned int hash(unsigned int v1, unsigned int v2, unsigned int v3);
  static void commonSymbols(Vector<Term*>& terms, PointerSet& common);
  void addCollapseSymbol(Symbol* symbol);
  void addCollapseSymbols(const PointerSet& symbols);
  void setHashValue(unsigned int value);
  void setSaveIndex(int index);
  
private:
  enum Flags
  {
    //
    //	A subterm is stable if its top symbol cannot change under instantiation.
    //
    STABLE = 1,
    //
    //	A subterm is in an eager context if the path to its root contains only
    //	eagerly evaluated positions.
    //
    EAGER_CONTEXT = 2,
    //
    //	A subterm "honors ground out match" if its matching algorithm guarantees
    //	never to to return a matching subproblem when all the terms variables
    //	are already bound.
    //
    HONORS_GROUND_OUT_MATCH = 4
  };

  static bool commonWithOtherPatterns(Vector<Term*>& patterns, int excluded, Symbol* symbol);
  static bool hasGeqOrIncomparableVariable(Term* pattern, VariableSymbol* v);

  static Vector<DagNode*> subDags;
  static TermSet converted;
  static bool setSortInfoFlag;

  Symbol* topSymbol;
  NatSet occursSet;
  NatSet contextSet;
  PointerSet collapseSet;
  Ubyte flags;
  short sortIndex;
  ConnectedComponent* connectedComponent;
  int saveIndex;
  unsigned int hashValue;
};

inline
Term::Term(Symbol* symbol)
{
  topSymbol = symbol;
  flags = 0;
  sortIndex = Sort::SORT_UNKNOWN;
  saveIndex = NONE;
}

inline Symbol*
Term::symbol() const
{
  return topSymbol;
}

inline bool
Term::ground() const
{
  return occursSet.empty();
}

inline bool
Term::stable() const
{
  return flags & STABLE;
}

inline const NatSet&
Term::occursBelow() const
{
  return occursSet;
}

inline const NatSet&
Term::occursInContext() const
{
  return contextSet;
}

inline const PointerSet&
Term::collapseSymbols() const
{
  return collapseSet;
}

inline Term*
Term::deepCopy(SymbolMap* translator) const
{
  Term* t = deepCopy2(translator);
  t->setLineNumber(getLineNumber());
  return t;
}

inline Term*
Term::instantiate(const Vector<Term*>& varBindings, SymbolMap* translator)
{
  Term* t = instantiate2(varBindings, translator);
  t->setLineNumber(getLineNumber());
  return t;
}

inline int
Term::compare(const Term* other) const
{
  Symbol* s = other->topSymbol;
  return (topSymbol == s) ? compareArguments(other) : topSymbol->compare(s);
}

inline int
Term::compare(const DagNode* other) const
{
  Symbol* s = other->symbol();
  return (topSymbol == s) ? compareArguments(other) : topSymbol->compare(s);
}

inline int  // inline this because it is heavily used at runtime
Term::partialCompare(const Substitution& partialSubstitution,
		     DagNode* other) const
{
  if (!stable())
    return partialCompareUnstable(partialSubstitution, other);
  Symbol* s = other->symbol();
  if (topSymbol == s)
    return partialCompareArguments(partialSubstitution, other);
  return (topSymbol->compare(s) < 0) ? LESS : GREATER;
}

inline bool
Term::equal(const Term* other) const
{
  return topSymbol == other->topSymbol && compareArguments(other) == 0;
}

inline bool
Term::equal(const DagNode* other) const
{
  return topSymbol == other->symbol() && compareArguments(other) == 0;
}

inline DagNode*
Term::dagify()
{
  Assert(subDags.length() == converted.cardinality(),
	 "length/cardinality mismatch");
  int e = converted.term2Index(this);
  if (e >= 0)
    return subDags[e];
  DagNode* d = dagify2();
  if (setSortInfoFlag)
    {
      Assert(sortIndex != Sort::SORT_UNKNOWN, "missing sort info");
      d->setSortIndex(sortIndex);
      d->setReduced();
    }
  converted.insert(this);
  subDags.append(d);
  return d;
}

inline void
Term::markEager(int nrVariables,
		const NatSet& eagerVariables,
		Vector<int>& problemVariables)
{
  flags |= EAGER_CONTEXT;
  markEagerArguments(nrVariables, eagerVariables, problemVariables);
}

inline ConnectedComponent*
Term::getComponent() const
{
  return connectedComponent;
}

inline int
Term::getSortIndex() const
{
  return sortIndex;
}
inline Sort*
Term::getSort() const
{
  return (sortIndex == Sort::SORT_UNKNOWN) ? 0 : connectedComponent->sort(sortIndex);
}

inline int
Term::getSaveIndex() const
{
  return saveIndex;
}

inline void
Term::setSaveIndex(int index)
{
  saveIndex = index;
}

inline void
Term::setSortInfo(ConnectedComponent* component, int index)
{
  connectedComponent = component;
  sortIndex = index;
}

inline bool
Term::matchIndependent(const Term* other) const
{
  return earlyMatchFailOnInstanceOf(other) && other->earlyMatchFailOnInstanceOf(this);
}

inline void
Term::addCollapseSymbol(Symbol* symbol)
{
  (void) collapseSet.insert(symbol);
}

inline void
Term::addCollapseSymbols(const PointerSet& symbols)
{
  collapseSet.insert(symbols);
}

inline unsigned int
Term::getHashValue() const
{
  return hashValue;
}

inline unsigned int
Term::hash(unsigned int v1, unsigned int v2)
{
  return 3 * v1 + v2;
}

inline unsigned int
Term::hash(unsigned int v1, unsigned int v2, unsigned int v3)
{
  return 3 * v1 + v2 * v3;
}

inline void
Term::setHashValue(unsigned int value)
{
  hashValue = value;
}

inline bool
Term::honorsGroundOutMatch() const
{
  return flags & HONORS_GROUND_OUT_MATCH;
}

inline void
Term::setHonorsGroundOutMatch(bool flag)
{
  if (flag)  // HACK should probably start clear and set as needed
    flags |= HONORS_GROUND_OUT_MATCH;
  else
    flags &= ~HONORS_GROUND_OUT_MATCH;
}

inline bool
Term::willGroundOutMatch(const NatSet& boundUniquely) const
{
  return honorsGroundOutMatch() && boundUniquely.contains(occursSet);
}

inline bool
Term::hasEagerContext() const
{
  return flags & EAGER_CONTEXT;
}

inline bool
Term::leq(const Sort* sort) const
{
  return ::leq(sortIndex, sort);
}

//
//	Output function for Term must be defined by library user
//
ostream& operator<<(ostream& s, const Term* term);

#endif
