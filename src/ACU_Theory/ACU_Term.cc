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
//	Implementation for class ACU_Term.
//

//	utility stuff
#include "macros.hh"
#include "vector.hh"
#include "graph.hh"
#include "digraph.hh"
#include "indent.hh"

//      forward declarations
#include "interface.hh"
#include "core.hh"
#include "variable.hh"
#include "ACU_Persistent.hh"
#include "ACU_Theory.hh"

//      interface class definitions
#include "argVec.hh"
#include "associativeSymbol.hh"
#include "dagNode.hh"
#include "term.hh"
#include "rawArgumentIterator.hh"
#include "lhsAutomaton.hh"
#include "rhsAutomaton.hh"
 
//      core class definitions
#include "rewritingContext.hh"
#include "equation.hh"
#include "symbolMap.hh"
#include "termBag.hh"
#include "rhsBuilder.hh"

//	variable class definitions
#include "variableTerm.hh"

//	ACU theory class definitions
#include "ACU_Symbol.hh"
#include "ACU_DagNode.hh"
#include "ACU_Term.hh"
#include "ACU_ArgumentIterator.hh"
#include "ACU_LhsAutomaton.hh"
#include "ACU_RhsAutomaton.hh"

//	ACU Red-Black class definitions
#include "ACU_TreeDagNode.hh"
#include "ACU_FastIter.hh"

//	extra source files
#include "ACU_LhsCompiler0.cc"
#include "ACU_LhsCompiler1.cc"
#include "ACU_LhsCompiler2.cc"
#include "ACU_LhsCompiler3.cc"

ACU_Term::ACU_Term(ACU_Symbol* symbol, const Vector<Term*>& arguments)
  : Term(symbol), argArray(arguments.length())
{
  int nrArgs = arguments.length();
  Assert(nrArgs >= 2, "insufficient arguments for operator " << symbol);
  for (int i = 0; i < nrArgs; i++)
    {
      argArray[i].term = arguments[i];
      argArray[i].multiplicity = 1;
    }
}

ACU_Term::ACU_Term(const ACU_Term& original, ACU_Symbol* symbol, SymbolMap* translator)
  : Term(symbol),
    argArray(original.argArray.length())
{
  int nrArgs = original.argArray.length();
  for (int i = 0; i < nrArgs; i++)
    {
      argArray[i].term = original.argArray[i].term->deepCopy(translator);
      argArray[i].multiplicity = original.argArray[i].multiplicity;
    }
}

RawArgumentIterator*
ACU_Term::arguments()
{
  return new ACU_ArgumentIterator(&argArray);
}

void
ACU_Term::deepSelfDestruct()
{
  int nrArgs = argArray.length();
  for (int i = 0; i < nrArgs; i++)
    argArray[i].term->deepSelfDestruct();
  delete this;
}

Term*
ACU_Term::deepCopy2(SymbolMap* translator) const
{
  ACU_Symbol* s = symbol();
  if (translator != 0)
    {
      Symbol* s2 = translator->translate(s);
      if (s2 == 0)
	{
	  int nrArgs = argArray.length();
	  if ((nrArgs == 1 && argArray[0].multiplicity == 2) ||
	      (nrArgs == 2 && argArray[0].multiplicity == 1 && argArray[1].multiplicity == 1))
	    return translator->translateTerm(this);
	  //
	  //	Tricky situtation - we have to use translateTerm() since
	  //	we are translating to a term but we have more than
	  //	2 arguments. We resolve it by creating a temporary
	  //	expanded term.
	  //
	  Vector<Term*> args(2);
	  Term* arg = argArray[0].term;
	  args[0] = arg;
	  int remainingMultiplicity = argArray[0].multiplicity - 1;
	  int nrNewTerms = remainingMultiplicity;
	  for (int i = 0;; nrNewTerms += remainingMultiplicity)
	    {
	      //
	      //	Normally it is a no-no for sharing of subterms, but
	      //	here we are just making a temporary construction that
	      //	will be translated and then carefully destructed.
	      //
	      for (; remainingMultiplicity > 0; --remainingMultiplicity)
		{
		  args[1] = arg;
		  args[0] = new ACU_Term(s, args);
		}
	      if (++i == nrArgs)
		break;
	      arg = argArray[i].term;
	      remainingMultiplicity = argArray[i].multiplicity;
	    }
	  Term* t = args[0];
	  Term* r = translator->translateTerm(t);
	  for (int i = 0; i < nrNewTerms; ++i)
	    {
	      Term* n = safeCast(ACU_Term*, t)->argArray[0].term;
	      delete t;
	      t = n;
	    }
	  return r;
	}
      s = dynamic_cast<ACU_Symbol*>(s2);
      if (s == 0)
	{
	  //
	  //	Another tricky situation - we are translating to a non-ACU_Symbol.
	  //
	  Vector<Term*> args(2);
	  Term* arg = argArray[0].term;
	  int remainingMultiplicity = argArray[0].multiplicity - 1;
	  args[0] = arg->deepCopy(translator);
	  int nrArgs = argArray.length();
	  for (int i = 0;;)
	    {
	      for (; remainingMultiplicity > 0; --remainingMultiplicity)
		{
		  args[1] = arg->deepCopy(translator);
		  args[0] = s2->makeTerm(args);
		}
	      if (++i == nrArgs)
		break;
	      arg = argArray[i].term;
	      remainingMultiplicity = argArray[i].multiplicity;
	    }
	  return args[0];
	}
    }
  return new ACU_Term(*this, s, translator);
}

bool
ACU_Term::normalizeAliensAndFlatten()
{
  bool changed = false;
  Symbol* s = symbol();
  int nrArgs = argArray.length();
  //
  //	Pass 1: normalize aliens and recusively flatten non-aliens, computing
  //	the number of extra arguments that will result from flattening at the top.
  //
  bool needToFlatten = false;
  int expansion = 0;
  for (int i = 0; i < nrArgs; i++)
    {
      Term* t = argArray[i].term;
      if (t->symbol() == s)
	{
	  changed = true;
	  ACU_Term* ac = safeCast(ACU_Term*, t);
	  (void) ac->normalizeAliensAndFlatten();
	  expansion += ac->argArray.length() - 1;
	  needToFlatten = true;
	}
      else
	{
	  bool subtermChanged;
	  argArray[i].term = t = t->normalize(true, subtermChanged);
	  if (subtermChanged)
	    {
	      changed = true;
	      if (t->symbol() == s)
		{
		  ACU_Term* ac = safeCast(ACU_Term*, t);
		  expansion += ac->argArray.length() - 1;
		  needToFlatten = true;
		}
	    }
	}
    }
  //
  //	Pass 2 : flatten at the top
  //
  if (needToFlatten)
    {
      argArray.expandBy(expansion);
      int p = nrArgs + expansion - 1;
      for (int i = nrArgs - 1; i >= 0; i--)
	{
	  Assert(p >= i, "loop invariant broken");
	  Term* t = argArray[i].term;
	  if (t->symbol() == s)
	    {
	      int m = argArray[i].multiplicity;
	      Vector<Pair>& argArray2 = safeCast(ACU_Term*, t)->argArray;
	      for (int j = argArray2.length() - 1; j >= 0; j--)
		{
		  argArray[p].term = argArray2[j].term;
		  argArray[p].multiplicity = m * argArray2[j].multiplicity;
		  --p;
		}
	      delete t;
	    }
	  else
	    argArray[p--] = argArray[i];
	}
    }
  return changed;
}

local_inline bool
ACU_Term::pairLt(const Pair& p1, const Pair& p2)
{
  return p1.term->compare(p2.term) < 0;  // sort pairs in ascending order of terms
}

Term*
ACU_Term::normalize(bool full, bool& changed)
{
  if (full)
    changed = normalizeAliensAndFlatten();
  else
    {
      changed = false;
      for (int i = argArray.length() - 1; i >= 0; i--)
	{
	  bool subtermChanged;
	  argArray[i].term = argArray[i].term->normalize(false, subtermChanged);
	  if (subtermChanged)
	    changed = true;
	}
    }
  int nrArgs = argArray.length();
  for (int i = 1; i < nrArgs; i++)
    {
      if (argArray[i - 1].term->compare(argArray[i].term) > 0)
	{
	  changed = true;
	  sort(argArray.begin(), argArray.end(), pairLt);
	}
    }
  int d = 0;
  for (int i = 1; i < nrArgs; i++)
    {
      if (argArray[i].term->equal(argArray[d].term))
	{
	  changed = true;
	  argArray[d].multiplicity += argArray[i].multiplicity;
	  argArray[i].term->deepSelfDestruct();
	}
      else
	argArray[++d] = argArray[i];
    }
  ++d;
  const Term* id = symbol()->getIdentity();
  if (id != 0)
    {
      //
      //	Need to eliminate any identity elements (maybe we should use binary search).
      //
      for (int i = 0; i < d; i++)
	{
	  if (id->equal(argArray[i].term))
	    {
	      changed = true;
	      if (d == 1)
		{
		  //
		  //	Only identity element left so collapse to identity element.
		  //
		  Term* t = argArray[i].term;
		  delete this;
		  return t;
		}
	      else if (d == 2 && argArray[1 - i].multiplicity == 1)
		{
		  //
		  //	Only one non-identity subterm left so collapse to it.
		  //
		  argArray[i].term->deepSelfDestruct();
		  Term* t = argArray[1 - i].term;
		  delete this;
		  return t;
		}
	      else
		{
		  //
		  //	Delete identity and shift other subterms to close gap.
		  //
		  argArray[i].term->deepSelfDestruct();
		  --d;
		  for (; i < d; i++)
		    argArray[i] = argArray[i + 1];
		  break;
		}
	    }
	}
    }
  argArray.contractTo(d);
  unsigned int hashValue = symbol()->getHashValue();
  for (int i = 0; i < d; i++)
    hashValue = hash(hashValue, argArray[i].term->getHashValue(), argArray[i].multiplicity);
  setHashValue(hashValue);
  return this;
}

int
ACU_Term::compareArguments(const Term* other) const
{
  const Vector<Pair>& argArray2 = safeCast(const ACU_Term*, other)->argArray;
  int r = argArray.length() - argArray2.length();
  if (r != 0)
    return r;
  Vector<Pair>::const_iterator j = argArray2.begin();
  Vector<Pair>::const_iterator i = argArray.begin();
  const Vector<Pair>::const_iterator e = argArray.end();
  do
    {
      r = i->multiplicity - j->multiplicity;
      if (r != 0)
	return r;
      r = i->term->compare(j->term);
      if (r != 0)
	return r;
      ++j;
      ++i;
    }
  while (i != e);
  Assert(j == argArray2.end(), "iterator problem");
  return 0;
}

int
ACU_Term::compareArguments(const DagNode* other) const
{
  int len = argArray.length();
  const ACU_BaseDagNode* d = safeCast(const ACU_BaseDagNode*, other);
  if (d->isTree())
    {
      const ACU_TreeDagNode* d2 = safeCast(const ACU_TreeDagNode*, d);
      int r = len - d2->getTree().getSize();
      if (r != 0)
	return r;
      ACU_FastIter j(d2->getTree());
      Vector<Pair>::const_iterator i = argArray.begin();
      const Vector<Pair>::const_iterator e = argArray.end();
      do
	{
	  r = i->multiplicity - j.getMultiplicity();
	  if (r != 0)
	    return r;
	  r = i->term->compare(j.getDagNode());
	  if (r != 0)
	    return r;
	  j.next();
	  ++i;
	}
      while (i != e);
      Assert(!j.valid(), "iterator problem");
    }
  else
    {
      const ArgVec<ACU_DagNode::Pair>& argArray2 =
	safeCast(const ACU_DagNode*, d)->argArray;
      int r = len - argArray2.length();
      if (r != 0)
	return r;
      ArgVec<ACU_DagNode::Pair>::const_iterator j = argArray2.begin();
      Vector<Pair>::const_iterator i = argArray.begin();
      const Vector<Pair>::const_iterator e = argArray.end();
      do
	{
	  r = i->multiplicity - j->multiplicity;
	  if (r != 0)
	    return r;
	  r = i->term->compare(j->dagNode);
	  if (r != 0)
	    return r;
	  ++j;
	  ++i;
	}
      while (i != e);
      Assert(j == argArray2.end(), "iterator problem");
    }
  return 0;
}

void
ACU_Term::findEagerVariables(bool atTop, NatSet& eagerVariables) const
{
  BinarySymbol::PermuteStrategy strat = symbol()->getPermuteStrategy();
  if (strat == BinarySymbol::EAGER ||
      (strat == BinarySymbol::SEMI_EAGER && !atTop))
    {
      int nrArgs = argArray.length();
      for (int i = 0; i < nrArgs; i++)
	argArray[i].term->findEagerVariables(false, eagerVariables);
    }
}

void
ACU_Term::markEagerArguments(int nrVariables,
			    const NatSet& eagerVariables,
			    Vector<int>& problemVariables)
{
  if (symbol()->getPermuteStrategy() == BinarySymbol::EAGER)
    {
      int nrArgs = argArray.length();
      for (int i = 0; i < nrArgs; i++)
	argArray[i].term->markEager(nrVariables, eagerVariables, problemVariables);
    }
}

DagNode*
ACU_Term::dagify2()
{
  int nrArgs = argArray.length();
  ACU_DagNode* d = new ACU_DagNode(symbol(), nrArgs);
  ArgVec<ACU_DagNode::Pair>& p = d->argArray;
  for (int i = 0; i < nrArgs; i++)
    {
      p[i].dagNode = argArray[i].term->dagify();
      p[i].multiplicity = argArray[i].multiplicity;
    }
  return d;
}

void
ACU_Term::analyseCollapses2()
{
  int nrArgs = argArray.length();
  for (int i = 0; i < nrArgs; i++)
    argArray[i].term->analyseCollapses();
  uniqueCollapseSubtermIndex = NONE;
  ACU_Symbol* topSymbol = symbol();
  const Term* identity = topSymbol->getIdentity();
  if (identity == 0)
    return;  // if no identity element then we can't collapse

  int firstNonIdArg = NONE;
  for (int i = 0; i < nrArgs; i++)
    {
      Pair& p = argArray[i];
      if (!(topSymbol->mightMatchOurIdentity(p.term)))
	{
	  if (firstNonIdArg != NONE || p.multiplicity > 1)
	    return;  // can't collapse
	  firstNonIdArg = i;
	}
    }
  if (firstNonIdArg != NONE)
    {
      //
      //	Can only collapse to firstNonIdArg.
      //
      uniqueCollapseSubtermIndex = firstNonIdArg;
      addCollapseSymbol(argArray[firstNonIdArg].term->symbol());
      addCollapseSymbols(argArray[firstNonIdArg].term->collapseSymbols());
    }
  else
    {
      //
      //	Can collapse to any of our arguments that has multiplicity 1.
      //	If no argument has multiplicity 1 then we can still collapse
      //	to our identity.
      //
      for (int i = 0; i < nrArgs; i++)
	{
	  Pair& p = argArray[i];
	  if (p.multiplicity == 1)
	    {
	      addCollapseSymbol(p.term->symbol());
	      addCollapseSymbols(p.term->collapseSymbols());
	    }
	}
      if (collapseSymbols().empty())
	addCollapseSymbol(identity->symbol());  // bizarre special case
    }
}

void
ACU_Term::insertAbstractionVariables(VariableInfo& variableInfo)
{
  ACU_Symbol* topSymbol = symbol();
  bool honorsGroundOutMatch = true;
  int nrArgs = argArray.length();
  for (int i = 0; i < nrArgs; i++)
    {
      Pair& p = argArray[i];
      p.term->insertAbstractionVariables(variableInfo);
      if (!(p.term->honorsGroundOutMatch()))
	honorsGroundOutMatch = false;
      p.abstractionVariableIndex = NONE;
      p.collapseToOurSymbol = false;
      p.matchOurIdentity = false;
      if (dynamic_cast<VariableTerm*>(p.term) == 0)
	{
	  p.matchOurIdentity = topSymbol->mightMatchOurIdentity(p.term);
	  p.collapseToOurSymbol = topSymbol->mightCollapseToOurSymbol(p.term);
	  if (p.matchOurIdentity || p.collapseToOurSymbol)
	    {
	      p.abstractionVariableIndex = variableInfo.makeProtectedVariable();
	      honorsGroundOutMatch = false;
	      DebugAdvisoryCheck(false, "Introduced abstraction variable for " <<
				 p.term << " in " << this << '.');
	    }
	}
    }
  setHonorsGroundOutMatch(honorsGroundOutMatch);
}

#ifdef DUMP
void
ACU_Term::dump(ostream& s, const VariableInfo& variableInfo, int indentLevel)
{
  s << Indent(indentLevel) << "Begin{ACU_Term}\n";
  ++indentLevel;
  dumpCommon(s, variableInfo, indentLevel);
  s << Indent(indentLevel) << "arguments:\n";
  ++indentLevel;
  int nrArgs = argArray.length();
  for (int i = 0; i < nrArgs; i++)
    {
      const Pair& p = argArray[i];
      s << Indent(indentLevel) << "multiplicity = " << p.multiplicity <<
	"\tcollapseToOurSymbol = " << bool(p.collapseToOurSymbol) <<
	"\tmatchOurIdentity = " << bool(p.matchOurIdentity);
      if (p.abstractionVariableIndex != NONE)
	s << "\tabstractionVariableIndex = " << p.abstractionVariableIndex;
      s << '\n';
      p.term->dump(s, variableInfo, indentLevel);
    }
  s << Indent(indentLevel - 2) << "End{ACU_Term}\n";
}
#endif

void
ACU_Term::findAvailableTerms(TermBag& availableTerms, bool eagerContext, bool atTop)
{
  if (ground())
    return;
  if (!atTop)
    availableTerms.insertMatchedTerm(this, eagerContext);
  BinarySymbol::PermuteStrategy strat = symbol()->getPermuteStrategy();
  bool argEager = eagerContext && (strat == BinarySymbol::EAGER ||
				   (strat == BinarySymbol::SEMI_EAGER && !atTop));
  int nrArgs = argArray.length();
  for (int i = 0; i < nrArgs; i++)
    argArray[i].term->findAvailableTerms(availableTerms, argEager);
}

int
ACU_Term::compileRhs2(RhsBuilder& rhsBuilder,
		      VariableInfo& variableInfo,
		      TermBag& availableTerms,
		      bool eagerContext)
{
  int nrArgs = argArray.length();
  //
  //	We want to minimize conflict between slots to avoid quadratic number of
  //	conflict arcs on giant right hand sides. The heuristic we use is crude:
  //	we sort in order of arguments by number of symbol occurences, and build
  //	largest first.
  //
  typedef Vector<pair<int, int> > PairVec;
  PairVec order(nrArgs);
  for (int i = 0; i < nrArgs; i++)
    {
      order[i].first = - argArray[i].term->computeSize();  // larger terms to the front
      order[i].second = i;
    }
  sort(order.begin(), order.end());
  //
  //	Compile each argument in largest first order.
  //
  bool argEager = eagerContext && symbol()->getPermuteStrategy() == BinarySymbol::EAGER;
  Vector<int> sources(nrArgs);
  FOR_EACH_CONST(i, PairVec, order)
    {
      int j = i->second;
      sources[j] = argArray[j].term->compileRhs(rhsBuilder,
						variableInfo,
						availableTerms,
						argEager);
    }
  //
  //	Now add sources to automaton in original order, and flag last use
  //	of each source for conflict arc generation.
  //
  ACU_RhsAutomaton* automaton = new ACU_RhsAutomaton(symbol(), nrArgs);
  for (int i = 0; i < nrArgs; i++)
    {
      int index = sources[i];
      automaton->addArgument(index, argArray[i].multiplicity);
      variableInfo.useIndex(index);
    }
  //
  //	Complete the automaton and add it  to the rhs builder.
  //
  int destination = variableInfo.makeConstructionIndex();
  automaton->close(destination);
  rhsBuilder.addRhsAutomaton(automaton);
  return destination;
}
