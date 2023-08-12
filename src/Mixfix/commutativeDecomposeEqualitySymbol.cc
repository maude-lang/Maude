/*

    This file is part of the Maude 3 interpreter.

    Copyright 2023 SRI International, Menlo Park, CA 94025, USA.

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
#include "strategyLanguage.hh"
#include "mixfix.hh"

//      interface class definitions
#include "symbol.hh"
#include "dagNode.hh"
#include "term.hh"
 
//      core class definitions
#include "argumentIterator.hh"
#include "rewritingContext.hh"
#include "symbolMap.hh"

//      free theory class definitions
#include "freeSymbol.hh"
#include "freeDagNode.hh"

//      CUI theory class definitions
#include "CUI_Symbol.hh"
#include "CUI_DagNode.hh"

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
  if (ls->isStable() && ls->equationFree() && ls->isConstructor(l))
    {
      //
      //	Left symbol cannot change under axioms, cannot equationally rewrite, and is declared
      //	to be a constructor on the arguments it has.
      //
      MixfixModule* m = safeCastNonNull<MixfixModule*>(ls->getModule());
      if (m->getSymbolType(ls).getBasicType() == SymbolType::STANDARD)
	{
	  //
	  //	No special properties either.
	  //
	  //	(by axioms since we've already ruled out equational rewriting and special properties but
	  //	not a future axiom f(X, f(X, Y)) = f(X, Y) for example), then return not equal.
	  //
	  if (hasImmediateSubterm(l, r))
	    return context.builtInReplace(subject, notEqualTerm.getDag());
	  Symbol* rs = r->symbol();
	  if (ls == rs)
	    {
	      if (conjunctionSymbol)
		{
		  //
		  //	Consider decomposition.
		  //
		  if (DagNode* decomposition = decompose(m, l, r))
		    return context.builtInReplace(subject, decomposition);
		}
	    }
	  else
	    {
	      //
	      //	Consider returning not equal.
	      //
	      if (rs->isStable() && rs->equationFree() && rs->isConstructor(r) &&
		  m->getSymbolType(rs).getBasicType() == SymbolType::STANDARD)
		return context.builtInReplace(subject, notEqualTerm.getDag());
	    }
	}
    }
  else
    {
      Symbol* rs = r->symbol();
      if (rs->isStable() && rs->equationFree() && rs->isConstructor(r))
	{
	  //
	  //	Check for the converse case where rs is inert and r has an immediate subterm l.
	  //
	  MixfixModule* m = safeCastNonNull<MixfixModule*>(rs->getModule());
	  if (m->getSymbolType(rs).getBasicType() == SymbolType::STANDARD && hasImmediateSubterm(r, l))
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

DagNode*
CommutativeDecomposeEqualitySymbol::decompose(MixfixModule* m, DagNode* l, DagNode* r)
{
  Symbol* topSymbol = l->symbol();
  if (FreeSymbol* f = dynamic_cast<FreeSymbol*>(topSymbol))
    {
      //
      //	FreeSymbol case: decompose into a conjunction.
      //
      int polymorphIndex = m->getPolymorphIndex(this); // find our polymorph
      int arity = f->arity();
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
  else if (disjunctionSymbol)
    {
      //
      //	Having a defined disjunctionSymbol allow us to do more.
      //
      if (CUI_Symbol* f =  dynamic_cast<CUI_Symbol*>(topSymbol))
	{
	  int polymorphIndex = m->getPolymorphIndex(this); // find our polymorph
	  int kindIndex = f->domainComponent(0)->getIndexWithinModule();
	  Symbol* sibling = m->instantiatePolymorph(polymorphIndex, kindIndex);
	  CUI_DagNode* left = safeCastNonNull<CUI_DagNode*>(l);
	  CUI_DagNode* right = safeCastNonNull<CUI_DagNode*>(r);
	  DagNode* l0 = left->getArgument(0);
	  DagNode* l1 = left->getArgument(1);
	  DagNode* r0 = right->getArgument(0);
	  DagNode* r1 = right->getArgument(1);

	  Vector<DagNode*> orArgs(2);
	  Vector<DagNode*> andArgs(2);
	  Vector<DagNode*> pair(2);
      
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
  return 0;
}

bool
CommutativeDecomposeEqualitySymbol::hasImmediateSubterm(DagNode* bigger, DagNode* smaller)
{
  if (FreeSymbol* f = dynamic_cast<FreeSymbol*>(bigger->symbol()))
    {
      int arity = f->arity();
      FreeDagNode* b = safeCastNonNull<FreeDagNode*>(bigger);
      for (int i = 0; i < arity; ++i)
	{
	  if (b->getArgument(i)->equal(smaller))
	    return true;
	}
    }
  else if (S_DagNode* b = dynamic_cast<S_DagNode*>(bigger))
    return b->getArgument()->equal(smaller);
  else if (CUI_DagNode* b = dynamic_cast<CUI_DagNode*>(bigger))
    {
      //
      //	We know the top symbol of b is stable so it must only be cummutative.
      //
      return b->getArgument(0)->equal(smaller) || b->getArgument(1)->equal(smaller);
    }
  return false;
}
