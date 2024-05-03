/*

    This file is part of the Maude 3 interpreter.

    Copyright 2023-2024 SRI International, Menlo Park, CA 94025, USA.

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
//      Implementation for class CommutativeDecomposeEqualitySymbol.
//
 
//	utility stuff
#include "macros.hh"
#include "vector.hh"
#include "indent.hh"

//      forward declarations
#include "interface.hh"
#include "core.hh"
#include "freeTheory.hh"
#include "CUI_Theory.hh"
#include "AU_Theory.hh"
#include "ACU_Theory.hh"
#include "mixfix.hh"

//      interface class definitions
#include "symbol.hh"
#include "dagNode.hh"
#include "term.hh"
 
//      core class definitions
#include "dagArgumentIterator.hh"
#include "rewritingContext.hh"
#include "symbolMap.hh"

//      free theory class definitions
#include "freeSymbol.hh"
#include "freeDagNode.hh"

//      CUI theory class definitions
#include "CUI_Symbol.hh"
#include "CUI_DagNode.hh"

//      AU theory class definitions
#include "AU_Symbol.hh"

//      ACU theory class definitions
#include "ACU_Symbol.hh"

//      S theory class definitions
#include "S_Symbol.hh"
#include "S_DagNode.hh"

//	built in class definitions
#include "bindingMacros.hh"
#include "commutativeDecomposeEqualitySymbol.hh"

//	mixfix class defintions.
#include "mixfixModule.hh"

CommutativeDecomposeEqualitySymbol::CommutativeDecomposeEqualitySymbol(int id, const Vector<int>& strategy)
  : CUI_Symbol(id, strategy, false, COMM)
{
  conjunctionSymbol = 0;
  disjunctionSymbol = 0;
}

bool
CommutativeDecomposeEqualitySymbol::attachData(const Vector<Sort*>& opDeclaration,
					       const char* purpose,
					       const Vector<const char*>& data)
{
  NULL_DATA(purpose, CommutativeDecomposeEqualitySymbol, data);
  return CUI_Symbol::attachData(opDeclaration, purpose, data);
}

bool
CommutativeDecomposeEqualitySymbol::attachSymbol(const char* purpose, Symbol* symbol)
{
  BIND_SYMBOL(purpose, symbol, conjunctionSymbol, Symbol*);
  BIND_SYMBOL(purpose, symbol, disjunctionSymbol, Symbol*);
  return CUI_Symbol::attachSymbol(purpose, symbol);
}

bool
CommutativeDecomposeEqualitySymbol::attachTerm(const char* purpose, Term* term)
{
  BIND_TERM(purpose, term, equalTerm);
  BIND_TERM(purpose, term, notEqualTerm);
  return CUI_Symbol::attachTerm(purpose, term);
}

void
CommutativeDecomposeEqualitySymbol::copyAttachments(Symbol* original, SymbolMap* map)
{
  CommutativeDecomposeEqualitySymbol* orig = safeCastNonNull<CommutativeDecomposeEqualitySymbol*>(original);
  COPY_SYMBOL(orig, conjunctionSymbol, map, Symbol*);
  COPY_SYMBOL(orig, disjunctionSymbol, map, Symbol*);
  COPY_TERM(orig, equalTerm, map);
  COPY_TERM(orig, notEqualTerm, map);
  CUI_Symbol::copyAttachments(original, map);
}

void
CommutativeDecomposeEqualitySymbol::getDataAttachments(const Vector<Sort*>& opDeclaration,
						       Vector<const char*>& purposes,
						       Vector<Vector<const char*> >& data)
{
  APPEND_DATA(purposes, data, CommutativeDecomposeEqualitySymbol);
  CUI_Symbol::getDataAttachments(opDeclaration, purposes, data);
}

void
CommutativeDecomposeEqualitySymbol::getSymbolAttachments(Vector<const char*>& purposes,
							 Vector<Symbol*>& symbols)
{
  APPEND_SYMBOL(purposes, symbols, conjunctionSymbol);
  APPEND_SYMBOL(purposes, symbols, disjunctionSymbol);
  CUI_Symbol::getSymbolAttachments(purposes, symbols);
}

void
CommutativeDecomposeEqualitySymbol::getTermAttachments(Vector<const char*>& purposes,
						       Vector<Term*>& terms)
{
  APPEND_TERM(purposes, terms, equalTerm);
  APPEND_TERM(purposes, terms, notEqualTerm);
  CUI_Symbol::getTermAttachments(purposes, terms);
}

void
CommutativeDecomposeEqualitySymbol::postInterSymbolPass()
{
  DebugInfo("using (raw pointer) conjunctionSymbol=" << (void*) conjunctionSymbol);
  DebugInfo("using conjunctionSymbol=" << conjunctionSymbol);
  (void) equalTerm.normalize();
  equalTerm.prepare();
  (void) notEqualTerm.normalize();
  notEqualTerm.prepare();
}

void
CommutativeDecomposeEqualitySymbol::reset()
{
  equalTerm.reset();  // so equal dag can be garbage collected
  notEqualTerm.reset();  // so notEqualDag dag can be garbage collected
  CUI_Symbol::reset();  // parents reset() tasks
}

bool
CommutativeDecomposeEqualitySymbol::isEquationallyStable(Symbol* s)
{
  return s->isStable() && // can't disappear by instantiation and axioms
    s->equationFree() &&  // can't equationally rewrite at the top
    safeCastNonNull<MixfixModule*>(s->getModule())->getSymbolType(s).getBasicType() ==
    SymbolType::STANDARD;  // can't disappear by built-in rewriting
}

bool
CommutativeDecomposeEqualitySymbol::eqRewrite(DagNode* subject, RewritingContext& context)
{
  Assert(this == subject->symbol(), "bad symbol");

  const CUI_DagNode* f = static_cast<CUI_DagNode*>(subject);
  DagNode* l = f->getArgument(0);
  DagNode* r = f->getArgument(1);
  //
  //	This only makes sense with the standard, eager strategy.
  //
  l->reduce(context);
  r->reduce(context);
  //
  //	Equal dags are always equal.
  //
  if (l->equal(r))
    return context.builtInReplace(subject, equalTerm.getDag());
  //
  //	Unequal ground dags are always unequal.
  //
  if (determineGround(subject))
    return context.builtInReplace(subject, notEqualTerm.getDag());
  //
  //	Otherwise we consider the properties of the top symbols.
  //
  Symbol* ls = l->symbol();
  if (isEquationallyStable(ls))
    {
      //
      //	Left top symbol cannot change under instantiation and equational rewriting.
      //
      if (hasImmediateSubterm(l, r))
	return context.builtInReplace(subject, notEqualTerm.getDag());
      Symbol* rs = r->symbol();
      if (ls == rs)
	{
	  //
	  //	Consider decomposition.
	  //
	  if (DagNode* decomposition = decompose(l, r))
	    return context.builtInReplace(subject, decomposition);
	}
      else
	{
	  //
	  //	Return not equal if the right-hand side symbol can't change under instantiation
	  //	equational rewriting.
	  //
	  if (isEquationallyStable(rs) || rs->determineGround(r))
	    return context.builtInReplace(subject, notEqualTerm.getDag());
	}
    }
  else
    {
      Symbol* rs = r->symbol();
      if (isEquationallyStable(rs))  // thus rs != ls
	{
	  //
	  //	Check for the converse case where rs is inert and either l is ground or
	  //	r has an immediate subterm l.
	  //
	  if (ls->determineGround(l) || hasImmediateSubterm(r, l))
	    return context.builtInReplace(subject, notEqualTerm.getDag());
	}
    }
  //
  //	Support user reductions.
  //
  return CUI_Symbol::eqRewrite(subject, context);
}

bool
CommutativeDecomposeEqualitySymbol::domainSortAlwaysLeqThan(Sort* /* sort */, int /* argNr */)
{
  //
  //	We're a polymorph instance that is defined on kinds so this is never true.
  //
  return false;
}

bool
CommutativeDecomposeEqualitySymbol::hasImmediateSubterm(DagNode* bigger, DagNode* smaller)
{
  for (DagArgumentIterator d(bigger); d.valid(); d.next())
    {
      if (d.argument()->equal(smaller))
	return true;
    }
  return false;
}

DagNode*
CommutativeDecomposeEqualitySymbol::decompose(DagNode* l, DagNode* r)
{
  Symbol* topSymbol = l->symbol();
  if (FreeSymbol* f = dynamic_cast<FreeSymbol*>(topSymbol))
    {
      //
      //	FreeSymbol case: decompose into a conjunction.
      //
      MixfixModule* m = safeCastNonNull<MixfixModule*>(getModule());
      int polymorphIndex = m->getPolymorphIndex(this); // find our polymorph
      int arity = f->arity();
      if (arity == 1 || conjunctionSymbol != nullptr)
	{
	  FreeDagNode* left = safeCastNonNull<FreeDagNode*>(l);
	  FreeDagNode* right = safeCastNonNull<FreeDagNode*>(r);
	  Vector<DagNode*> subterms(arity);
	  Vector<DagNode*> pair(2);
	  for (int i = 0; i < arity; ++i)
	    {
	      int kindIndex = f->domainComponent(i)->getIndexWithinModule();
	      Symbol* sibling = m->instantiatePolymorph(polymorphIndex, kindIndex);
	      pair[0] = left->getArgument(i);
	      pair[1] = right->getArgument(i);
	      subterms[i] = sibling->makeDagNode(pair);
	    }
	  return (arity == 1) ? subterms[0] : conjunctionSymbol->makeDagNode(subterms);
	}
    }
  else if (S_Symbol* f = dynamic_cast<S_Symbol*>(topSymbol))
    {
      //
      //	Range and domain of the iter symbol must be the same
      //	so we use the same polymorph instance.
      //
      S_DagNode* left = safeCastNonNull<S_DagNode*>(l);
      S_DagNode* right = safeCastNonNull<S_DagNode*>(r);
      const mpz_class& ln = left->getNumber();
      DagNode* la = left->getArgument();
      const mpz_class& rn = right->getNumber();
      DagNode* ra = right->getArgument();

      Vector<DagNode*> pair(2);      
      if (ln > rn)
	{
	  pair[0] = new S_DagNode(f, ln - rn, la);
	  pair[1] = ra;
	}
      else
	{
	  pair[0] = la;
	  pair[1] = (ln < rn) ? new S_DagNode(f, rn - ln, ra) : ra;
	}
      return makeDagNode(pair);
    }
  else if (CUI_Symbol* f =  dynamic_cast<CUI_Symbol*>(topSymbol))
    {
      //
      //	Consider the commutative case.
      //
      MixfixModule* m = safeCastNonNull<MixfixModule*>(getModule());
      int polymorphIndex = m->getPolymorphIndex(this); // find our polymorph
      int kindIndex = f->domainComponent(0)->getIndexWithinModule();
      Symbol* sibling = m->instantiatePolymorph(polymorphIndex, kindIndex);
      CUI_DagNode* left = safeCastNonNull<CUI_DagNode*>(l);
      CUI_DagNode* right = safeCastNonNull<CUI_DagNode*>(r);
      DagNode* l0 = left->getArgument(0);
      DagNode* l1 = left->getArgument(1);
      DagNode* r0 = right->getArgument(0);
      DagNode* r1 = right->getArgument(1);
      //
      //	Pick off 4 cases that don't need a disjunction of conjunctions.
      //
      Vector<DagNode*> pair(2);
      if (l0->equal(r0))
	{
	  pair[0] = l1;
	  pair[1] = r1;
	  return sibling->makeDagNode(pair);
	}
      if (l1->equal(r1))
	{
	  pair[0] = l0;
	  pair[1] = r0;
	  return sibling->makeDagNode(pair);
	}
      if (l0->equal(r1))
	{
	  pair[0] = l1;
	  pair[1] = r0;
	  return sibling->makeDagNode(pair);
	}
      if (l1->equal(r0))
	{
	  pair[0] = l0;
	  pair[1] = r1;
	  return sibling->makeDagNode(pair);
	}
      //
      //	Otherwise we need to decompose as a disjunction of conjunctions.
      //
      if (conjunctionSymbol != nullptr && disjunctionSymbol != nullptr)
	{
	  Vector<DagNode*> orArgs(2);
	  Vector<DagNode*> andArgs(2);

	  pair[0] = l0;
	  pair[1] = r0;
	  andArgs[0] = sibling->makeDagNode(pair);
	  pair[0] = l1;
	  pair[1] = r1;
	  andArgs[1] = sibling->makeDagNode(pair);
	  orArgs[0] = conjunctionSymbol->makeDagNode(andArgs);

	  pair[0] = l0;
	  pair[1] = r1;
	  andArgs[0] = sibling->makeDagNode(pair);
	  pair[0] = l1;
	  pair[1] = r0;
	  andArgs[1] = sibling->makeDagNode(pair);
	  orArgs[1] = conjunctionSymbol->makeDagNode(andArgs);

	  return disjunctionSymbol->makeDagNode(orArgs);
	}
    }
  else if (AU_Symbol* f =  dynamic_cast<AU_Symbol*>(topSymbol))
    return associativeDecompose(f, l, r);
  else if (ACU_Symbol* f = dynamic_cast<ACU_Symbol*>(topSymbol))
    return acDecompose(f, l, r);
  return 0;
}

DagNode*
CommutativeDecomposeEqualitySymbol::associativeDecompose(AU_Symbol* f, DagNode* l, DagNode* r)
{
  //
  //	One or both sides of .=. could be in persistent form, so collect arguments
  //	in vectors.
  //
  Vector<DagNode*> leftArgs;
  for (DagArgumentIterator d(l); d.valid(); d.next())
    leftArgs.push_back(d.argument());
  Vector<DagNode*> rightArgs;
  for (DagArgumentIterator d(r); d.valid(); d.next())
    rightArgs.push_back(d.argument());
  //
  //	Try peeling matching arguments from the ends.
  //
  Vector<DagNode*> leftPeels;
  Vector<DagNode*> rightPeels;
  Index leftEnd = leftArgs.size();
  Index rightEnd = rightArgs.size();
  Index maxEnd = max(leftEnd, rightEnd);
  Index startMarker = 0;
  //
  //	Peel arguments from the start.
  //
  for (; startMarker < maxEnd; ++startMarker)
    {
      DagNode* leftArg = leftArgs[startMarker];
      DagNode* rightArg = rightArgs[startMarker];
      if (leftArg->equal(rightArg))
	continue;  // can peel off these arguments
      Symbol* ls = leftArg->symbol();
      Symbol* rs = rightArg->symbol();
      if (ls->determineGround(leftArg))
	{
	  if (rs->determineGround(rightArg))
	    return notEqualTerm.getDag();  // unequal ground terms decompose to false
	  else if (!isEquationallyStable(rs))
	    break;  // can't peel arguments with a right argument that could change under substitution
	}
      else
	{
	  if (!isEquationallyStable(ls))
	    break;  // can't peel arguments with a left argument that could change under substitution
	  if (!(isEquationallyStable(rs) || rs->determineGround(rightArg)))
	    break;  // can't peel arguments with a right argument that could change under substitution
	}     
      if (ls != rs)
	return notEqualTerm.getDag();  // decompose to false
      //
      //	A pair if distinct terms with the same equationally-stable symbol on top
      //	and at least one is non-ground.
      //
      leftPeels.push_back(leftArg);
      rightPeels.push_back(rightArg);
    }
  //
  //	Peel arguments from the end.
  //
  for (--leftEnd, --rightEnd; leftEnd >= startMarker && rightEnd >= startMarker; --leftEnd, --rightEnd)
    {
      DagNode* leftArg = leftArgs[leftEnd];
      DagNode* rightArg = rightArgs[rightEnd];
      if (leftArg->equal(rightArg))
	continue;  // can peel off these arguments
      Symbol* ls = leftArg->symbol();
      Symbol* rs = rightArg->symbol();
      if (ls->determineGround(leftArg))
	{
	  if (rs->determineGround(rightArg))
	    return notEqualTerm.getDag();  // unequal ground terms decompose to false
	  else if (!isEquationallyStable(rs))
	    break;  // can't peel arguments with a right argument that could change under substitution
	}
      else
	{
	  if (!isEquationallyStable(ls))
	    break;  // can't peel arguments with a left argument that could change under substitution
	  if (!(isEquationallyStable(rs) || rs->determineGround(rightArg)))
	    break;  // can't peel arguments with a right argument that could change under substitution
	}     
      if (ls != rs)
	return notEqualTerm.getDag();  // decompose to false
      //
      //	A pair if distinct terms with the same equationally-stable symbol on top
      //	and at least one is non-ground.
      //
      leftPeels.push_back(leftArg);
      rightPeels.push_back(rightArg);
    }
  //
  //	We've peeled all we can; what's left?
  //
  Index leftSideRemaining = leftEnd + 1 - startMarker;
  Index rightSideRemaining = rightEnd + 1 - startMarker;
  if ((leftSideRemaining == 0) != (rightSideRemaining == 0))
    return notEqualTerm.getDag();  // decompose to false
  //
  //	See if we can prove not equal on the remainders by using the commutative axiom.
  //
  DagNodeMultiset leftMultiset = makeMultiset(leftArgs, startMarker, leftEnd);
  DagNodeMultiset rightMultiset = makeMultiset(rightArgs, startMarker, rightEnd);
  if (acProvablyUnequal(leftMultiset, rightMultiset))
    return notEqualTerm.getDag();  // decompose to false
  Index leftSideTotal = leftArgs.size();
  if (leftSideRemaining == leftSideTotal)
    return 0;  // no progress - need to exit to avoid an infinite loop
  //
  //	Make decomposition.
  //
  Index nrPeels = leftPeels.size();
  Index arity = nrPeels + (leftSideRemaining != 0);
  //
  //	We must have some unresolved subterms since left and right terms compared unequal.
  //
  Assert(arity != 0, "0 arity from peeling unequal assoc terms");
  if (arity == 1 || conjunctionSymbol != nullptr)
    {
      Vector<DagNode*> pair(2);
      Vector<DagNode*> andArgs(arity);
      for (Index i = 0; i < nrPeels; ++i)
	{
	  pair[0] = leftPeels[i];
	  pair[1] = rightPeels[i];
	  andArgs[i] = makeDagNode(pair);
	}
      if (leftSideRemaining != 0)
	{
	  if (leftSideRemaining == 1)
	    pair[0] = leftArgs[startMarker];
	  else
	    {
	      Vector<DagNode*> args(leftSideRemaining);
	      for (Index i = 0; i < leftSideRemaining; ++i)
		args[i] = leftArgs[startMarker + i];
	      pair[0] = f->makeDagNode(args);
	    }
	  if (rightSideRemaining == 1)
	    pair[1] = rightArgs[startMarker];
	  else
	    {
	      Vector<DagNode*> args(rightSideRemaining);
	      for (Index i = 0; i < rightSideRemaining; ++i)
		args[i] = rightArgs[startMarker + i];
	      pair[1] = f->makeDagNode(args);
	    }
	  andArgs[nrPeels] = makeDagNode(pair);
	}
      return (arity == 1) ? andArgs[0] : conjunctionSymbol->makeDagNode(andArgs);
    }
  return 0;  // can't do anything
}


DagNode*
CommutativeDecomposeEqualitySymbol::acDecompose(ACU_Symbol* f, DagNode* l, DagNode* r)
{
  DagNodeMultiset leftMultiset = makeMultisetFromArguments(l);
  DagNodeMultiset rightMultiset = makeMultisetFromArguments(r);
  Index nrLeftArgs = leftMultiset.size();
  //
  //	If acProvablyUnequal() can't prove unequal, it may still cancel some subterms.
  //
  if (acProvablyUnequal(leftMultiset, rightMultiset))
    return notEqualTerm.getDag();  // decompose to false

  Index leftSideRemaining = leftMultiset.size();
  if (leftSideRemaining < nrLeftArgs)
    {
      //
      //	acProvablyUnequal() did some cancelation.
      //
      Vector<DagNode*> pair(2);
      if (leftSideRemaining == 1)
	pair[0] = *(leftMultiset.begin());
      else
	{
	  Vector<DagNode*> args(leftSideRemaining);
	  Index i = 0;
	  for (DagNode* d : leftMultiset)
	    args[i++] = d;
	  pair[0] = f->makeDagNode(args);
	}
      Index rightSideRemaining = rightMultiset.size();
      if (rightSideRemaining == 1)
	pair[1] = *(rightMultiset.begin());
      else
	{
	  Vector<DagNode*> args(rightSideRemaining);
	  Index i = 0;
	  for (DagNode* d : rightMultiset)
	    args[i++] = d;
	  pair[1] = f->makeDagNode(args);
	}
      return makeDagNode(pair);
    }
  return 0;
}

bool
CommutativeDecomposeEqualitySymbol::acProvablyUnequal(DagNodeMultiset& leftMultiset, DagNodeMultiset& rightMultiset)
{
  //
  //	We try to prove that the left multiset of subterms can never be equal to the right
  //	set multiset of subterms, under stubstitution, axioms and equational rewriting.
  //	We return true if we have a proof, and false if we don't.
  //	As a side effect, we delete common subterms from the multisets.
  //
  //	Start by deleting equal subterms from each side. We use the naive O(n . log m) approach
  //	but we could be smarter and do O(n + m).
  //
  for (auto l = leftMultiset.begin(); l != leftMultiset.end(); )
    {
      auto r = rightMultiset.find(*l);
      if (r != rightMultiset.end())
	{
	  auto t = l;
	  ++l;
	  leftMultiset.erase(t);
	  rightMultiset.erase(r);
	}
      else
	++l;
    }
  if (leftMultiset.empty())
    return !rightMultiset.empty();  // if left side is empty and right side is not, we have our proof
  if (rightMultiset.empty())
    return true;  // if right side is empty and left side is not, we have our proof
  //
  //	Both sides are nonempty. See if one side only contains subterms that are ground or headed by
  //	equationally-stable symbols.
  //
  bool swapMultisets = false;
  bool tryPairingTopSymbols = false;
  if (equationallyStableOrGround(rightMultiset))
    {
      if (leftMultiset.size() > rightMultiset.size())
	return true;  // proof by cardinality argument
      swapMultisets = true;
      tryPairingTopSymbols = true;  // we can go through rightSet, trying to find something that can't be paired
    }
  if (equationallyStableOrGround(leftMultiset))
    {
      if (leftMultiset.size() < rightMultiset.size())
	return true;  // proof by cardinality argument
      swapMultisets = false;
      tryPairingTopSymbols = true;  // we can go through rightSet, trying to find something that can't be paired
    }
  if (tryPairingTopSymbols)
    {
      if (swapMultisets)
	leftMultiset.swap(rightMultiset);
      //
      //	We know the leftMultiset only has subterms that can't change their top symbols by
      //	instantiation, axioms and equational rewriting. We now go throught rightMultiset and
      //	look for subterms with the same property that can be cancelled with leftMutliset subterms.
      //	If we ever fail to get a cancellation, we have a proof of inequality.
      //
      DagNodeMultiset leftCopy(leftMultiset);
      for (DagNode* r : rightMultiset)
	{
	  Symbol* rs = r->symbol();
	  if (isEquationallyStable(rs) || rs->determineGround(r))
	    {
	      //
	      //	Can only equal something with rs on top.
	      //
	      for (DagNode* l : leftCopy)
		{
		  if (l->symbol() == rs)
		    {
		      leftCopy.erase(l);
		      goto nextRightSubterm;
		    }
		}
	      //
	      //	There is nothing left in leftMultiset that could be equal to r so we have our proof.
	      //
	      return true;
	    nextRightSubterm:
	      ;
	    }
	}
      //
      //	Restore multisets to their canceled form.
      //
      if (swapMultisets)
	leftMultiset.swap(rightMultiset);
    }
  return false;
}

bool
CommutativeDecomposeEqualitySymbol::equationallyStableOrGround(DagNodeMultiset& dagMultiset)
{
  for (DagNode* d : dagMultiset)
    {
      Symbol* s = d->symbol();
      if (!(isEquationallyStable(s) || s->determineGround(d)))
	return false;
    }
  return true;
}
