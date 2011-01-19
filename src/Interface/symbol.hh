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
//	Abstract base class for function/variable symbol definitions.
//
#ifndef _symbol_hh_
#define _symbol_hh_
#include "fullCompiler.hh"

#include "namedEntity.hh"
#include "lineNumber.hh"
#include "moduleItem.hh"
#include "sort.hh"
#include "connectedComponent.hh"
#include "sortTable.hh"
#include "sortConstraintTable.hh"
#include "equationTable.hh"
#include "ruleTable.hh"
#include "strategy.hh"
#include "memoTable.hh"

class Symbol
  : public RuleTable,
    public NamedEntity,
    public LineNumber,
    public SortTable,
    public SortConstraintTable,
    public EquationTable,
    public Strategy,
    public MemoTable
{
  NO_COPYING(Symbol);

  static const Vector<DagNode*> noArgs;

public:
  Symbol(int id, int arity, bool memoFlag = false);
  virtual ~Symbol();

  unsigned int getHashValue();
  int compare(const Symbol* other) const;
  bool mightMatchPattern(Term* pattern);
  void fastComputeTrueSort(DagNode* subject, RewritingContext& context);  // maybe should be const?
  int getUniqueSortIndex();
  //
  //	Functions needed for sophisticated sort analysis.
  //
  virtual bool rangeSortNeverLeqThan(Sort* sort);
  virtual bool rangeSortAlwaysLeqThan(Sort* sort);
  virtual bool domainSortAlwaysLeqThan(Sort* sort, int argNr);
  void computePossibleDomainSorts(const NatSet& rangeSorts,
				  Vector<NatSet>& domainSorts);
  //
  //	These functions must be defined for each derived class.
  //
  virtual bool eqRewrite(DagNode* subject, RewritingContext& context) = 0;
  virtual void computeBaseSort(DagNode* subject) = 0;
  virtual void normalizeAndComputeTrueSort(DagNode* subject, RewritingContext& context) = 0;
  virtual Term* makeTerm(const Vector<Term*>& args) = 0;
  virtual DagNode* makeDagNode(const Vector<DagNode*>& args = noArgs) = 0;
  virtual void stackArguments(DagNode* subject,
			      Vector<RedexPosition>& stack,
			      int parentIndex) = 0;
  
  //
  //	These functions may be redefined for each derived class.
  //
  //	interSymbolPass() is called on each symbol in turn until all
  //	return false. It is use for resolving interdependencies.
  //
  //	postInterSymbolPass() is called on each symbol after the above
  //	completes.
  //
  //	postOpDeclarationPass() is called on each symbol after its sort
  //	table has been calculated.
  //
  virtual bool interSymbolPass();
  virtual void postInterSymbolPass();
  virtual void postOpDeclarationPass();
  //
  //	Clear state information such as caches that affects performance but not result.
  //
  virtual void reset();
  void finalizeSortInfo();  // virtual in base class SortTable
  void fillInSortInfo(Term* subject);  // virtual in base class SortTable
  //
  //	These functions may be redefined to attach data/symbols/terms to
  //	support "special" operators.
  //
  virtual bool attachData(const Vector<Sort*>& opDeclaration,
			  const char* purpose,
			  const Vector<const char*>& data);
  virtual bool attachSymbol(const char* purpose, Symbol* symbol);
  virtual bool attachTerm(const char* purpose, Term* term);
  virtual void copyAttachments(Symbol* original, SymbolMap* map);
  virtual void getDataAttachments(const Vector<Sort*>& opDeclaration,
				  Vector<const char*>& purposes,
				  Vector<Vector<const char*> >& data);
  virtual void getSymbolAttachments(Vector<const char*>& purposes,
				    Vector<Symbol*>& symbols);
  virtual void getTermAttachments(Vector<const char*>& purposes,
				  Vector<Term*>& terms);
  //
  //	This function might be redefined to provide better performance
  //	or special constructor semantics.
  //
  virtual bool isConstructor(DagNode* subject);

  //
  //	Interface for unification.
  //
  virtual UnificationSubproblem* makeUnificationSubproblem();
  virtual int unificationPriority() const;
  //
  //	Symbols that can disappear under substitution must rdefine this to return false.
  //
  virtual bool isStable() const = 0;
  //
  //	Symbols that can resolved theory clashes (typically by collapsing) should
  //	redefined this to return true and be ready to handle alien right hand sides
  //	by restricted unification in unification problem passed to their unification
  //	subproblems.
  //
  virtual bool canResolveTheoryClash();
  //
  //	Interface for hash consing.
  //
  virtual DagNode* makeCanonical(DagNode* original, HashConsSet* hcs) = 0;
  //
  //	Same as above but copying is forced - original can never be the canonical dagnode.
  //
  virtual DagNode* makeCanonicalCopy(DagNode* original, HashConsSet* hcs) = 0;

#ifdef COMPILER
  void fullCompile(CompilationContext& context, bool inLine) const;
  virtual void generateCode(CompilationContext& context) const;
#endif

#ifdef DUMP
  virtual void dump(ostream& s, int indentLevel = 0);
#endif

protected:
  bool acceptSortConstraint(SortConstraint* sortConstraint);
  bool acceptEquation(Equation* equation);
  bool acceptRule(Rule* rule);

private:
  bool mightMatchSymbol(Symbol* symbol);
  void slowComputeTrueSort(DagNode* subject, RewritingContext& context);

  static int symbolCount;

  const int orderInt;
  int uniqueSortIndex;
};

inline unsigned int
Symbol::getHashValue()
{
  return orderInt;
}

inline int
Symbol::compare(const Symbol* other) const
{
  return orderInt - other->orderInt;
}

inline int
Symbol::getUniqueSortIndex()
{
  return uniqueSortIndex;
}

#endif
