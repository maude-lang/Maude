/*

    This file is part of the Maude 3 interpreter.

    Copyright 1997-2024 SRI International, Menlo Park, CA 94025, USA.

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
//      Implementation for base class Symbol.
//

//	utility stuff
#include "macros.hh"
#include "vector.hh"
#include "indent.hh"
 
//      forward declarations
#include "interface.hh"
#include "core.hh"
#include "variable.hh"
#include "fullCompiler.hh"

//      interface class definitions
#include "symbol.hh"
#include "dagNode.hh"
#include "term.hh"

//      core class definitions
#include "argumentIterator.hh"
#include "dagArgumentIterator.hh"
#include "equation.hh"
#include "rule.hh"
#include "sortConstraint.hh"

//	variable class definitions
#include "variableSymbol.hh"

//	full compiler class definitions
#include "compilationContext.hh"

int Symbol::symbolCount = 0;
const Vector<DagNode*> Symbol::noArgs;  // constant empty vector

Symbol::Symbol(int id, int arity, bool memoFlag)
  : NamedEntity(id),
    SortTable(arity),
    MemoTable(memoFlag),
    orderInt(symbolCount++ | (arity << 24))
{
  uniqueSortIndex = 0;
  matchIndex = 1;  // default match index
}

Symbol::~Symbol()
{
}

void
Symbol::slowComputeTrueSort(DagNode* subject, RewritingContext& context)
{
  if (subject->getSortIndex() == Sort::SORT_UNKNOWN)
    {
      computeBaseSort(subject);
      constrainToSmallerSort(subject, context); 
    }
}

bool
Symbol::mightMatchPattern(Term* pattern)
{
  if (mightMatchSymbol(pattern->symbol()))
    return true;
  const PointerSet& cs = pattern->collapseSymbols();
  int cardinality = cs.cardinality();
  for (int i = 0; i < cardinality; i++)
    {
      if (mightMatchSymbol(static_cast<Symbol*>(cs.index2Pointer(i))))
	return true;
    }
  return false;
}

bool
Symbol::mightMatchSymbol(Symbol* symbol)
{
  if (symbol == this)
    return true;
  if (VariableSymbol* vs = dynamic_cast<VariableSymbol*>(symbol))
    {
      Sort* s = vs->getSort();
      if (rangeComponent() == s->component())
	{
	  if (specialSortHandling())
	    return true;  // can't tell anything about a symbol with special sort handling
	  if (!safeToInspectSortConstraints())
	    return true;  // uninspected sort constraint may lower our symbols lowest sort
	  const Vector<OpDeclaration>& opDeclarations = getOpDeclarations();
	  int nrDeclarations = opDeclarations.length();
	  int nrArgs = arity();
	  for (int i = 0; i < nrDeclarations; i++)
	    {
	      if (leq(opDeclarations[i].getDomainAndRange()[nrArgs], s))
		return true;
	    }
	  const Vector<SortConstraint*>& sortConstraints = getSortConstraints();
	  int nrSortConstraints = sortConstraints.length();
	  for (int i = 0; i < nrSortConstraints; i++)
	    {
	      if (leq(sortConstraints[i]->getSort(), s))
		return true;
	    }
	}
    }
  return false;
}

bool
Symbol::acceptSortConstraint(SortConstraint* sortConstraint)
{
  return mightMatchPattern(sortConstraint->getLhs());
}

bool
Symbol::acceptEquation(Equation* equation)
{
  return mightMatchPattern(equation->getLhs());
}

bool
Symbol::acceptRule(Rule* rule)
{
  return mightMatchPattern(rule->getLhs());
}

void
Symbol::fillInSortInfo(Term* subject)
{
  Assert(this == subject->symbol(), "Bad Symbol");
  ConnectedComponent* component = rangeComponent();  // should be const
  Assert(component != 0, "couldn't get component");

  int nrArgs = arity();
  if (nrArgs == 0)
    {
      subject->setSortInfo(component, traverse(0, 0));  // HACK
      return;
    }

  int step = 0;
#ifndef NO_ASSERT
  int nrArgsSeen = 0;
#endif
  for (ArgumentIterator a(*subject); a.valid(); a.next())
    {
      Term* t = a.argument();
      t->symbol()->fillInSortInfo(t);
      Assert(t->getComponent() == domainComponent(nrArgsSeen),
	     "component error on arg " << nrArgsSeen <<
	     " while computing sort of " << subject);
      step = traverse(step, t->getSortIndex());
#ifndef NO_ASSERT
      ++nrArgsSeen;
#endif
    }
  Assert(nrArgsSeen == nrArgs, "bad # of args for op");
  subject->setSortInfo(component, step);
}

void
Symbol::stackArguments(DagNode* /* subject */,
		       Vector<RedexPosition>& /* stack */,
		       int /* parentIndex */,
		       bool /* respectFrozen */,
		       bool /* respectUnstackable*/,
		       bool /* eagerContext */)
{
  //
  //	Default version does nothing and can be used for symbols that have no arguments.
  //
}

void
Symbol::stackPhysicalArguments(DagNode* subject,
			       Vector<RedexPosition>& stack,
			       int parentIndex,
			       bool respectFrozen,
			       bool respectUnstackable,
			       bool eagerContext)
{
  //
  //	Default version assumes that physical arguments correspond to notional arguments.
  //
  stackArguments(subject, stack, parentIndex, respectFrozen, respectUnstackable, eagerContext);
}

bool
Symbol::interSymbolPass()
{
  return false;
}

void
Symbol::postInterSymbolPass()
{
}

void
Symbol::postOpDeclarationPass()
{
}

void
Symbol::finalizeSortInfo()
{
  //
  //	If a symbol has sort constraints we leave uniqueSortIndex = 0 for slowest, most general handling.
  //	Otherwise we check if it can produces a unique sort and set uniqueSortIndex to the index of this sort if so.
  //	If neither case applies we set uniqueSortIndex = -1, to run fast, theory dependent case.
  //
  if (sortConstraintFree())
    {
      Sort* s = getSingleNonErrorSort();
      uniqueSortIndex = (s != 0 && !(canProduceErrorSort())) ? s->index() : -1;
    }
}

Instruction*
Symbol::generateFinalInstruction(const Vector<int>& /* argumentSlots */)
{
  return 0;
}

Instruction*
Symbol::generateInstruction(int /* destination */,  const Vector<int>& /* argumentSlots */, Instruction* /* nextInstruction */)
{
  return 0;
}

void
Symbol::stackMachinePostProcess()
{
}

void
Symbol::reset()
{
  resetEachEquation();
  resetEachRule();
}

bool
Symbol::rangeSortNeverLeqThan(Sort* sort)
{
  //
  //	Check to see that our range sort will never be less than or equal to
  //	given sort; Equivalently a variable with given sort can never take a
  //	term headed by us.
  //
  int nrArgs = arity();
  const Vector<OpDeclaration>& opDecls = getOpDeclarations();
  int nrOpDecls = opDecls.length();
  for (int i = 0; i < nrOpDecls; i++)
    {
      if (leq(opDecls[i].getDomainAndRange()[nrArgs], sort))
	return false;
    }
  const Vector<SortConstraint*>& sortConstraints = getSortConstraints();
  int nrSortConstraints = sortConstraints.length();
  for (int i = 0; i < nrSortConstraints; i++)
    {
      if (leq(sortConstraints[i]->getSort(), sort))
	return false;
    }
  return true;
}

bool
Symbol::rangeSortAlwaysLeqThan(Sort* sort)
{
  //
  //	Check to see that our range sort will always be less than or equal to
  //	given sort (except if it is in the error sort); Equivalently a variable
  //	with given sort can always take a term headed by us unless it is in the
  //	error sort.
  //
  int nrArgs = arity();
  const Vector<OpDeclaration>& opDecls = getOpDeclarations();
  int nrOpDecls = opDecls.length();
  for (int i = 0; i < nrOpDecls; i++)
    {
      if (!leq(opDecls[i].getDomainAndRange()[nrArgs], sort))
	return false;
    }
  return true;
}

bool
Symbol::domainSortAlwaysLeqThan(Sort* sort, int argNr)
{
  //
  //	Check that any sort we could take in our argNr position and not end up
  //	in the error sort is less or equal to the given sort.
  //	The current definition will be seriously incorrect if sort constraints
  //	are allowed to terms out of the error sort in the future.
  //
  const Vector<OpDeclaration>& opDecls = getOpDeclarations();
  int nrOpDecls = opDecls.length();
  for (int i = 0; i < nrOpDecls; i++)
    {
      if (!leq(opDecls[i].getDomainAndRange()[argNr], sort))
	return false;
    }
  return true;
}

void
Symbol::computePossibleDomainSorts(const NatSet& rangeSorts,
				   Vector<NatSet>& domainSorts)
{
  Assert(!(rangeSorts.empty()), "shouldn't be empty");
  //
  //	We compute the set of sorts that the specified argument can possibly have
  //	given that the range sort must be <= one of the specified range sorts.
  //	We are conservative in that we may generate a larger set of domain sorts
  //	than strictly necessary.
  //
  int nrArgs = arity();
  domainSorts.resize(nrArgs);
  if (specialSortHandling() ||
      !safeToInspectSortConstraints() ||
      rangeSorts.contains(Sort::KIND))
    {
      //
      //	We can't safely compute anything in these cases so
      //	return the complete set of domain sorts.
      //
      for (int i = 0; i < nrArgs; i++)
	domainSorts[i] = domainComponent(i)->sort(Sort::KIND)->getLeqSorts();
      return;
    }
  NatSet allPossibleRangeSorts;
  {
    //
    //	First throw in all sorts that are less or equal to given range sorts.
    //
    const ConnectedComponent* rc = rangeComponent();
    const NatSet::const_iterator e = rangeSorts.end();
    for (NatSet::const_iterator i = rangeSorts.begin(); i != e; ++i)
      allPossibleRangeSorts.insert(rc->sort(*i)->getLeqSorts());
  }
  {
    //
    //	Now do a fixed point computation to see if we can get down from
    //	other sorts to or below our given range sorts via a sort constraint.
    //
    const Vector<SortConstraint*>& sortConstraints = getSortConstraints();
    int nrSortConstraints = sortConstraints.length();
    bool changed;
    do
      {
	changed = false;
	for (int i = 0; i < nrSortConstraints; i++)
	  {
	    SortConstraint* sc = sortConstraints[i];
	    if (allPossibleRangeSorts.contains(sc->getSort()->index()))
	      {
		const NatSet& fromSorts = sc->getLhs()->getSort()->getLeqSorts();
		if (fromSorts.contains(Sort::KIND))
		  {
		    //
		    //	A sort constraint can pull us out of the kind!
		    //
		    for (int i = 0; i < nrArgs; i++)
		      domainSorts[i] = domainComponent(i)->sort(Sort::KIND)->getLeqSorts();
		    return;
		  }
		if (!(allPossibleRangeSorts.contains(fromSorts)))
		  {
		    allPossibleRangeSorts.insert(fromSorts);
		    changed = true;
		  }
	      }
	  }
      }
    while (changed);
  }
  Assert(!(allPossibleRangeSorts.contains(Sort::KIND)),
	 "shouldn't contain kind");
  {
    //
    //	Now examine our declarations to see what domain sorts are possible
    //	in each argument position.
    //
    for (int i = 0; i < nrArgs; i++)
      domainSorts[i].clear();

    const Vector<OpDeclaration>& opDeclarations = getOpDeclarations();
    int nrDeclarations = opDeclarations.length();
    for (int i = 0; i < nrDeclarations; i++)
      {
	const Vector<Sort*>& domainAndRange = opDeclarations[i].getDomainAndRange();
	if (allPossibleRangeSorts.contains(domainAndRange[nrArgs]->index()))
	  {
	    for (int j = 0; j < nrArgs; j++)
	      domainSorts[j].insert(domainAndRange[j]->getLeqSorts());
	  }
      }
  }
}

bool
Symbol::attachData(const Vector<Sort*>& /* opDeclaration */,
		   const char* purpose,
		   const Vector<const char*>& /* data */)
{
  IssueWarning(*this <<": failed to attach id-hook " << QUOTE(purpose) <<
	       " to " << QUOTE(this) << '.');
  return false;
}

bool
Symbol::attachSymbol(const char* purpose, Symbol* symbol)
{
  IssueWarning(*this <<": failed to attach op-hook " << QUOTE(purpose) <<
	       ' ' << QUOTE(symbol) << " to " << QUOTE(this) << '.');
  return false;
}

bool
Symbol::attachTerm(const char* purpose, Term* term)
{
  IssueWarning(*this <<": failed to attach term-hook " << QUOTE(purpose) <<
	       ' ' << QUOTE(term) << " to " << QUOTE(this) << '.');
  term->deepSelfDestruct();
  return false;
}

void
Symbol::copyAttachments(Symbol* /* original */, SymbolMap* /* map */)
{
}

void
Symbol::getDataAttachments(const Vector<Sort*>& /* opDeclaration */,
			   Vector<const char*>& /* purposes */,
			   Vector<Vector<const char*> >& /* data */)
{
}

void
Symbol::getSymbolAttachments(Vector<const char*>& /* purposes */,
			     Vector<Symbol*>& /* symbols */)
{
}

void
Symbol::getTermAttachments(Vector<const char*>& /* purposes */,
			   Vector<Term*>& /* terms */)
{
}

bool
Symbol::isConstructor(DagNode* subject)
{
  if (specialSortHandling())
    return false;  // HACK
  
  switch (getCtorStatus())
    {
    case SortTable::IS_CTOR:
      return true;
    case SortTable::IS_COMPLEX:
      {
	if (arity() == 0)
	  return ctorTraverse(0, 0);  // HACK
	
	int state = 0;
	for (DagArgumentIterator a(*subject); a.valid(); a.next())
	  {
	    int t = a.argument()->getSortIndex();
	    Assert(t != Sort::SORT_UNKNOWN, "Unknown sort");
	    state = ctorTraverse(state, t);
	  }
	return state;
      }
    default:
      break;
    }
  return false;
}

//
//	Putting this stuff here rather than the header file avoids gcc creating separate instances
//	for derived classes tht don't define their own versions.
//

bool
Symbol::canResolveTheoryClash()
{
  //
  //	The default is that we can't resolve theory clashes.
  //
  return false;
}

UnificationSubproblem*
Symbol::makeUnificationSubproblem()
{
  CantHappen("Not implemented"); return 0;
}

int 
Symbol::unificationPriority() const
{
  return 100;
}

#ifdef COMPILER
void
Symbol::fullCompile(CompilationContext& context, bool inLine) const
{
  context.beginNewFunction();
#ifdef ANNOTATE
  context.body() << "// " << this << '\n';
#endif
  int nrArgs = arity();
  int index = getIndexWithinModule();
  //
  //	Generate forward decl for C++ function.
  //
  context.head() << "Node* f" << index << '(';
  for (int i = 0; i < nrArgs; i++)
    {
      if (i != 0)
	context.head() << ", ";
      context.head() << "Node*";
    }
  context.head() << ");\n";
  //
  //	Generate C++ function.
  //
  if (inLine)
    context.body() << "inline ";
  context.body() << "Node*\nf" << index << '(';
  for (int i = 0; i < nrArgs; i++)
    {
      if (i != 0)
	context.body() << ", ";
      context.body() << "Node* a" << i;
    }
  context.body() << ")\n{\n  F" << index << ";\n";
  generateCode(context);
  context.body() << "}\n\n";
  //
  //	Generate #define for start label and/or safe slots if needed.
  //
  context.head() << "#define F" << getIndexWithinModule();
  if (context.isTailRecursive())
    context.head() << "\tstart:";
  int maxSafe = context.getNrSafeSlots();
  if (maxSafe > 0)
    context.head() << "\tLink safe[" << maxSafe + 2 << ']';
  context.head() << '\n';
}

void
Symbol::generateCode(CompilationContext& context) const
{
  context.body() << "  // not implemented\n  return 0;\n";
}
#endif

#ifdef DUMP
void
Symbol::dump(ostream& s, int indentLevel)
{
  s << Indent(indentLevel) << "Dumping Symbol: \"" << this << "\"\n";
  dumpSortDiagram(s, indentLevel);
  dumpEquationTable(s, indentLevel);
}
#endif
