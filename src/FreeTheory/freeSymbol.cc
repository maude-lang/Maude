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
//	Implementation for class FreeSymbol.
//

//	utility stuff
#include "macros.hh"
#include "vector.hh"
#include "unionFind.hh"
#include "indent.hh"

//	forward declarations
#include "interface.hh"
#include "core.hh"
#include "variable.hh"
#include "fullCompiler.hh"
#include "freeTheory.hh"

//	interface class definitions
#include "symbol.hh"
#include "dagNode.hh"
#include "term.hh"
#include "lhsAutomaton.hh"
#include "rhsAutomaton.hh"

//      core class definitions
#include "rewritingContext.hh"
#include "equation.hh"
#include "sortBdds.hh"
#include "hashConsSet.hh"

//	full compiler class definitions
#include "compilationContext.hh"

//	free theory class definitions
#include "freePreNet.hh"
#include "freeNet.hh"
#include "freeSymbol.hh"
#include "freeNullarySymbol.hh"
#include "freeUnarySymbol.hh"
#include "freeBinarySymbol.hh"
#include "freeTernarySymbol.hh"
#include "freeDagNode.hh"
#include "freeOccurrence.hh"
#include "freeTerm.hh"
#include "freeLhsAutomaton.hh"
#include "freeRemainder.hh"

#include "freeFastInstruction.hh"
#include "freeGeneralCtor.hh"
#include "freeGeneralCtorFinal.hh"
#include "freeGeneralExtor.hh"
#include "freeGeneralExtorFinal.hh"

FreeSymbol*
FreeSymbol::newFreeSymbol(int id, int arity, const Vector<int>& strategy, bool memoFlag)
{
  if (arity <= 3)
    {
      if (memoFlag || strategy.length() != 0)
	{
	  FreeSymbol* t = new FreeSymbol(id, arity, strategy, memoFlag);
	  if (!(t->standardStrategy()))
	    return t;
	  delete t;
	}
      if (arity == 0)
	return new FreeNullarySymbol(id);
      else if (arity == 1)
	return new FreeUnarySymbol(id);
      else if (arity == 2)
	return new FreeBinarySymbol(id);
      else
	return new FreeTernarySymbol(id);
    }
  return new FreeSymbol(id, arity, strategy, memoFlag);
}
	    
FreeSymbol::FreeSymbol(int id, int arity, const Vector<int>& strategy, bool memoFlag)
  : Symbol(id, arity, memoFlag)
{
  setStrategy(strategy, arity, memoFlag);
}

void
FreeSymbol::compileEquations()
{
  const Vector<Equation*>& equations = getEquations();
  int nrEquations = equations.length();
  for (int i = 0; i < nrEquations; i++)
    {
      Equation* e = equations[i];
      Term* p = e->getLhs();
      if (FreeTerm* f = dynamic_cast<FreeTerm*>(p))
	{
	  e->compile(false);
	  //
	  //	Even though we pass compileLhs as false, if the equation has the variant property it will
	  //	get compiled anyway and we need to reset the slot indices to make it safe to use them
	  //	in constructing a discrimination net.
	  //
	  f->resetSlotIndices();
	  f->setSlotIndex(0);
	}
      else
	e->compile(true);  // foreign equation so compile lhs
    }

  FreePreNet n(false);
  n.buildNet(this);
  n.semiCompile(discriminationNet);
}

Term*
FreeSymbol::makeTerm(const Vector<Term*>& args)
{
  return new FreeTerm(this, args);
}

DagNode*
FreeSymbol::makeDagNode(const Vector<DagNode*>& args)
{
  FreeDagNode* f = new FreeDagNode(this);
  DagNode** args2 = f->argArray();
  for (int i = arity() - 1; i >= 0; i--)
    args2[i] = args[i];
  return f;
}

bool
FreeSymbol::eqRewrite(DagNode* subject, RewritingContext& context)
{
  // cout << "attempting " << this << "\n";
  Assert(this == subject->symbol(), "bad symbol");  
  if (standardStrategy())
    {
      int nrArgs = arity();
      DagNode** args = static_cast<FreeDagNode*>(subject)->argArray();
      for (int i = nrArgs; i > 0; i--, args++)
        (*args)->reduce(context);
      return DISC_NET.applyReplace(subject, context);
    }
  return complexStrategy(subject, context);
}

bool
FreeSymbol::complexStrategy(DagNode* subject, RewritingContext& context)
{
  if (isMemoized())
    {
      MemoTable::SourceSet from;
      memoStrategy(from, subject, context);
      memoEnter(from, subject);
      return false;
    }
  int nrArgs = arity();
  DagNode** args = static_cast<FreeDagNode*>(subject)->argArray();
  //
  //	Execute user supplied strategy.
  //
  const Vector<int>& userStrategy = getStrategy();
  int stratLen = userStrategy.length();
  bool seenZero = false;
  for (int i = 0; i < stratLen; i++)
    {
      int a = userStrategy[i];
      if(a == 0)
	{
	  if (!seenZero)
	    {
	      for (int j = 0; j < nrArgs; j++)
		args[j]->computeTrueSort(context);
	      seenZero = true;
	    }
	  if ((i + 1 == stratLen) ? DISC_NET.applyReplace(subject, context) :
	      DISC_NET.applyReplaceNoOwise(subject, context))
	    return true;
	}
      else
	{
	  --a;  // real arguments start at 0 not 1
	  if (seenZero)
	    {
	      args[a] = args[a]->copyReducible();
	      //
	      //	A previous call to discriminationNet.applyReplace() may have
	      //	computed a true sort for our subject which will be
	      //	invalidated by the reduce we are about to do.
	      //
	      subject->repudiateSortInfo();
	    }
	  args[a]->reduce(context);
	}
    }
  return false;
}

void
FreeSymbol::memoStrategy(MemoTable::SourceSet& from,
			 DagNode* subject,
			 RewritingContext& context)
{
  int nrArgs = arity();
  DagNode** args = static_cast<FreeDagNode*>(subject)->argArray();
  //
  //	Execute user supplied strategy.
  //
  const Vector<int>& userStrategy = getStrategy();
  int stratLen = userStrategy.length();
  bool seenZero = false;
  for (int i = 0; i < stratLen; i++)
    {
      int a = userStrategy[i];
      if(a == 0)
	{
	  if (!seenZero)
	    {
	      for (int j = 0; j < nrArgs; j++)
		args[j]->computeTrueSort(context);
	      seenZero = true;
	    }
	  if (memoRewrite(from, subject, context))
	    return;
	  if ((i + 1 == stratLen) ? DISC_NET.applyReplace(subject, context) :
	      DISC_NET.applyReplaceNoOwise(subject, context))
	    {
	      subject->reduce(context);
	      return;
	    }
	}
      else
	{
	  --a;  // real arguments start at 0 not 1
	  if (seenZero)
	    {
	      args[a] = args[a]->copyReducible();
	      //
	      //	A previous call to discriminationNet.applyReplace() may have
	      //	computed a true sort for our subject which will be
	      //	invalidated by the reduce we are about to do.
	      //
	      subject->repudiateSortInfo();
	    }
	  args[a]->reduce(context);
	}
    }
}

void
FreeSymbol::computeBaseSort(DagNode* subject)
{
  Assert(this == subject->symbol(), "bad symbol");
  int nrArgs = arity();
  if (nrArgs == 0)
    {
      subject->setSortIndex(traverse(0, 0));  // HACK
      return;
    }
  DagNode** args = static_cast<FreeDagNode*>(subject)->argArray();

  int state = 0;
  for (int i = 0; i < nrArgs; i++)
    {
      int t = args[i]->getSortIndex();
#if 0
      DebugAdvisory("FreeSymbol::computeBaseSort() arg " << i <<
		    " is " << args[i] <<
		    " at " << ((void*) args[i]) <<
		    " with sort index " << t);
#endif
      Assert(t != Sort::SORT_UNKNOWN, "unknown sort encounter for arg " << i << " subject = " << subject);
      state = traverse(state, t);
    }
  subject->setSortIndex(state);
}

void
FreeSymbol::normalizeAndComputeTrueSort(DagNode* subject, RewritingContext& context)
{
  Assert(this == subject->symbol(), "bad symbol");
  int nrArgs = arity();
  DagNode** args = static_cast<FreeDagNode*>(subject)->argArray();
  //
  //	First make sure each of our subterms has a sort.
  //
  for (int i = 0; i < nrArgs; i++)
    args[i]->computeTrueSort(context);
  //
  //	Now compute subjects true sort.
  //
  fastComputeTrueSort(subject, context);
}

void
FreeSymbol::stackArguments(DagNode* subject,
			   Vector<RedexPosition>& stack,
			   int parentIndex)
{
  int nrArgs = arity();
  if (nrArgs != 0)
    {
      const NatSet& frozen = getFrozen();
      DagNode** args = static_cast<FreeDagNode*>(subject)->argArray();
      for (int i = 0; i < nrArgs; i++)
	{
	  DagNode* d = args[i];
	  if (!(frozen.contains(i)) && !(d->isUnstackable()))
	    stack.append(RedexPosition(args[i], parentIndex, i, eagerArgument(i)));
	}
    }
}

Term*
FreeSymbol::termify(DagNode* dagNode)
{
  int nrArgs = arity();
  Vector<Term*> args(nrArgs);
  DagNode** dagNodeArgs = safeCast(FreeDagNode*, dagNode)->argArray();
  for (int i = 0; i < nrArgs; i++)
    {
      DagNode* d = dagNodeArgs[i];
      args[i] = d->symbol()->termify(d);
    }
  return new FreeTerm(this, args);
}

//
//	Unification code.
//

void
FreeSymbol::computeGeneralizedSort(const SortBdds& sortBdds,
				   const Vector<int>& realToBdd,
				   DagNode* subject,
				   Vector<Bdd>& generalizedSort)
{
  DebugAdvisory("computeGeneralizedSort() called on symbol " << this << " for dag " << subject);
  int nrArgs = arity();
  Assert(nrArgs > 0, "we shouldn't be called on constants: " << subject);
  //
  //	We need to do this early since it may increase the number of bdd variables and we
  //	will need those variables for our argMap.
  //
  DebugAdvisory("getting sort function for " << this << " " << subject);
  const Vector<Bdd>& sortFunction = sortBdds.getSortFunction(this);

  DagNode** args = safeCast(FreeDagNode*, subject)->argArray();
  int varCounter = 0;
  bddPair* argMap = bdd_newpair();
  for (int i = 0; i < nrArgs; i++)
    {
      Vector<Bdd> argGenSort;
      args[i]->computeGeneralizedSort(sortBdds, realToBdd, argGenSort);
      int nrBdds = argGenSort.size();
      for (int j = 0; j < nrBdds; ++j, ++varCounter)
	bdd_setbddpair(argMap, varCounter, argGenSort[j]);
    }
  int nrBdds = sortFunction.size();
  generalizedSort.resize(nrBdds);
  for (int i = 0; i < nrBdds; ++i)
    generalizedSort[i] = bdd_veccompose(sortFunction[i], argMap);
  bdd_freepair(argMap);
}

bool
FreeSymbol::isStable() const
{
  return true;
}

//
//	Hash cons code.
//

DagNode*
FreeSymbol::makeCanonical(DagNode* original, HashConsSet* hcs)
{
  int nrArgs = arity();
  DagNode** p = safeCast(FreeDagNode*, original)->argArray();
  for (int i = 0; i < nrArgs; i++)
    {
      DagNode* d = p[i];
      DagNode* c = hcs->getCanonical(hcs->insert(d));
      if (c != d)
        {
	  //
	  //	Detected a non-canonical argument so need to make a new node.
	  //
	  FreeDagNode* n = new FreeDagNode(this);
	  n->copySetRewritingFlags(original);
	  n->setSortIndex(original->getSortIndex());
	  DagNode** q = n->argArray();
	  for (int j = 0; j < i; ++j, ++p, ++q)
            *q = *p;
	  *q = c;
	  ++p;
	  ++q;
	  for (int j = i + 1; j < nrArgs;  ++j, ++p, ++q)
	    *q = hcs->getCanonical(hcs->insert(*p));
	  return n;
        }
    }
  return original;  // can use the original dag node as the canonical version
}

DagNode*
FreeSymbol::makeCanonicalCopy(DagNode* original, HashConsSet* hcs)
{
  //
  //	We have a unreduced node - copy forced.
  //
  FreeDagNode* n = new FreeDagNode(this);
  n->copySetRewritingFlags(original);
  n->setSortIndex(original->getSortIndex());
  DagNode** q = n->argArray();

  int nrArgs = arity();
  DagNode** p = safeCast(FreeDagNode*, original)->argArray();
  for (int i = 0; i < nrArgs; ++i, ++p, ++q)
    *q = hcs->getCanonical(hcs->insert(*p));
  return n;
}

Instruction*
FreeSymbol::generateFinalInstruction(const Vector<int>& argumentSlots)
{
  if (!(discriminationNet.fastHandling()))
    return new FreeGeneralExtorFinal(this, argumentSlots);

  int nrArgs = arity();
  if (nrArgs > 3)
    {
      if (equationFree())
	return new FreeGeneralCtorFinal(this, argumentSlots);
      else
	return new FreeGeneralExtorFinal(this, argumentSlots);
    }
  switch (nrArgs)
    {
    case 0:
      return new FreeFastInstruction(this, 0, 0);
    case 1:
      return new FreeFastInstruction(this, argumentSlots[0], 0, 0);
    case 2:
      return new FreeFastInstruction(this, argumentSlots[0], argumentSlots[1], 0, 0);
    case 3:
      return new FreeFastInstruction(this, argumentSlots[0], argumentSlots[1], argumentSlots[2], 0, 0);
    }
  return 0;
}

Instruction*
FreeSymbol::generateInstruction(int destination, const Vector<int>& argumentSlots, Instruction* nextInstruction)
{
  if (!(discriminationNet.fastHandling()))
    return new FreeGeneralExtor(this, argumentSlots, destination, nextInstruction);

  int nrArgs = arity();
  if (nrArgs > 3)
    {
      if (equationFree())
	return new FreeGeneralCtor(this, argumentSlots, destination, nextInstruction);
      else
	return new FreeGeneralExtor(this, argumentSlots, destination, nextInstruction);
    }
  switch (nrArgs)
    {
    case 0:
      return new FreeFastInstruction(this, destination, nextInstruction);
    case 1:
      return new FreeFastInstruction(this, argumentSlots[0], destination, nextInstruction);
    case 2:
      return new FreeFastInstruction(this, argumentSlots[0], argumentSlots[1], destination, nextInstruction);
    case 3:
      return new FreeFastInstruction(this, argumentSlots[0], argumentSlots[1], argumentSlots[2], destination, nextInstruction);
    }
  return 0;
}

#ifdef COMPILER
void
FreeSymbol::generateCons(CompilationContext& context, int indentLevel) const
{
  int nrArgs = arity();
  //
  //	Generate code to create new node with our function symbol in it.
  //
  context.body() << Indent(indentLevel) << "Node* r = reinterpret_cast<Node*>(g.memNext);\n";
  context.body() << Indent(indentLevel) << "char* n = g.memNext + sizeof(Node)";
  if (nrArgs > 0)
    context.body() << " + " << nrArgs << " * sizeof(Node*)";
  context.body() << ";\n";
  context.body() << Indent(indentLevel) << "if (n > g.memEnd)\n";
  //
  //	Out of ephemeral space case.
  //
  context.body() << Indent(indentLevel + 1) << "{\n";
  context.body() << Indent(indentLevel + 2) << "g.nrArgs = " << nrArgs << ";\n";
  for (int i = 0; i < nrArgs; i++)
    context.body() << Indent(indentLevel + 2) << "g.args[" << i << "] = a" << i << ";\n";
  context.body() << Indent(indentLevel + 2) << "collectGarbage();\n";
  context.body() << Indent(indentLevel + 2) << "r = reinterpret_cast<Node*>(g.memNext);\n";
  context.body() << Indent(indentLevel + 2) << "n = g.memNext + sizeof(Node)";
  if (nrArgs > 0)
    context.body() << " + " << nrArgs << " * sizeof(Node*)";
  context.body() << ";\n";

  for (int i = 0; i < nrArgs; i++)
    context.body() << Indent(indentLevel + 2) << 'a' << i << " = g.args[" << i << "];\n";
  context.body() << Indent(indentLevel + 1) << "}\n";
  //
  //	Normal case.
  //
  context.body() << Indent(indentLevel) << "g.memNext = n;\n";
  for (int i = 0; i < nrArgs; i++)
    context.body() << Indent(indentLevel) << "r->args[" << i << "] = a" << i << ";\n";
  //
  //	Generate code to fill in symbol and flags and return node.
  //
  int index = getIndexWithinModule();
  context.body() << Indent(indentLevel) << "r->symbol = " << index << ";\n";
  Sort *s = getSingleNonErrorSort();
  if (s != 0 && !(canProduceErrorSort()))
    {
      context.body() << Indent(indentLevel) << "const Flags c = {" <<
	s->index() << "};\n";
      context.body() << Indent(indentLevel) << "r->flags = c;\n";
    }
  else
    {
      context.body() << Indent(indentLevel) << "r->flags = f" << index << "_0";
      for (int i = 0; i < nrArgs; i++)
	context.body() << "[a" << i << "->flags.sortIndex]";
      context.body() << ";\n";
    }
  context.body() << Indent(indentLevel) << "return r;\n";
}

void
FreeSymbol::generateCode(CompilationContext& context) const
{
  if (!(getSingleNonErrorSort()) || canProduceErrorSort())
    generateSortDiagram(context);
  FreePreNet n(true);
  n.buildNet(const_cast<FreeSymbol*>(this));
  n.generateCode(context);
  // n.dump(cerr);
}
#endif

#ifdef DUMP
void
FreeSymbol::dump(ostream& s, int indentLevel)
{
  Symbol::dump(s, indentLevel);
  discriminationNet.dump(s, indentLevel);
}
#endif
