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

//	full compiler class definitions
#include "compilationContext.hh"

//	free theory class definitions
#include "freePreNet.hh"
#include "freeNet.hh"
#include "freeSymbol.hh"
#include "freeUnarySymbol.hh"
#include "freeBinarySymbol.hh"
#include "freeTernarySymbol.hh"
#include "freeDagNode.hh"
#include "freeOccurrence.hh"
#include "freeTerm.hh"
#include "freeLhsAutomaton.hh"
#include "freeRemainder.hh"

FreeSymbol*
FreeSymbol::newFreeSymbol(int id, int arity, const Vector<int>& strategy, bool memoFlag)
{
  if (arity >= 1 && arity <= 3)
    {
      if (memoFlag || strategy.length() != 0)
	{
	  FreeSymbol* t = new FreeSymbol(id, arity, strategy, memoFlag);
	  if (!(t->standardStrategy()))
	    return t;
	  delete t;
	}
      if (arity == 1)
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
	  f->setSlotIndex(0);
	  e->compile(false);
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
      return discriminationNet.applyReplace(subject, context);
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
	  if ((i + 1 == stratLen) ? discriminationNet.applyReplace(subject, context) :
	      discriminationNet.applyReplaceNoOwise(subject, context))
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
	  if ((i + 1 == stratLen) ? discriminationNet.applyReplace(subject, context) :
	      discriminationNet.applyReplaceNoOwise(subject, context))
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
      Assert(t != Sort::SORT_UNKNOWN, "unknown sort");
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
