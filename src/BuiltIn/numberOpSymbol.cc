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
//      Implementation for class NumberOpSymbol.
//

//      utility stuff
#include "macros.hh"
#include "vector.hh"

//      forward declarations
#include "interface.hh"
#include "core.hh"
#include "freeTheory.hh"
#include "builtIn.hh"

//      interface class definitions
#include "term.hh"

//      core class definitions
#include "rewritingContext.hh"
#include "symbolMap.hh"

//      free theory class definitions
#include "freeNet.hh"
#include "freeDagNode.hh"

//      built in stuff
#include "bindingMacros.hh"
#include "succSymbol.hh"
#include "minusSymbol.hh"
#include "numberOpSymbol.hh"

NumberOpSymbol::NumberOpSymbol(int id, int arity)
  : FreeSymbol(id, arity)
{
  op = NONE;
  succSymbol = 0;
  minusSymbol = 0;
}

bool
NumberOpSymbol::attachData(const Vector<Sort*>& opDeclaration,
			   const char* purpose,
			   const Vector<const char*>& data)
{
  BIND_OP(purpose, NumberOpSymbol, op, data);
  return FreeSymbol::attachData(opDeclaration, purpose, data);
}

bool
NumberOpSymbol::attachSymbol(const char* purpose, Symbol* symbol)
{
  BIND_SYMBOL(purpose, symbol, succSymbol, SuccSymbol*);
  BIND_SYMBOL(purpose, symbol, minusSymbol, MinusSymbol*);
  return FreeSymbol::attachSymbol(purpose, symbol);
}

bool
NumberOpSymbol::attachTerm(const char* purpose, Term* term)
{
  BIND_TERM(purpose, term, trueTerm);
  BIND_TERM(purpose, term, falseTerm);
  return FreeSymbol::attachTerm(purpose, term);
}

void
NumberOpSymbol::copyAttachments(Symbol* original, SymbolMap* map)
{
  NumberOpSymbol* orig = safeCast(NumberOpSymbol*, original);
  op = orig->op;
  COPY_SYMBOL(orig, succSymbol, map, SuccSymbol*);
  COPY_SYMBOL(orig, minusSymbol, map, MinusSymbol*);
  COPY_TERM(orig, trueTerm, map);
  COPY_TERM(orig, falseTerm, map);
  FreeSymbol::copyAttachments(original, map);
}

void
NumberOpSymbol::getDataAttachments(const Vector<Sort*>& opDeclaration,
				   Vector<const char*>& purposes,
				   Vector<Vector<const char*> >& data)
{
  int nrDataAttachments = purposes.length();
  purposes.resize(nrDataAttachments + 1);
  purposes[nrDataAttachments] = "NumberOpSymbol";
  data.resize(nrDataAttachments + 1);
  data[nrDataAttachments].resize(1);
  const char*& d = data[nrDataAttachments][0];
  switch (op)
    {
    CODE_CASE(d, '-', 0, "-")
    CODE_CASE(d, '~', 0, "~")
    CODE_CASE(d, 'a', 'b', "abs")
    CODE_CASE(d, '+', 0, "+")
    CODE_CASE(d, '*', 0, "*")
    CODE_CASE(d, 'g', 'c', "gcd")
    CODE_CASE(d, 'l', 'c', "lcm")
    CODE_CASE(d, '|', 0, "|")
    CODE_CASE(d, '&', 0, "&")
    CODE_CASE(d, 'x', 'o', "xor")
    CODE_CASE(d, 'q', 'u', "quo")
    CODE_CASE(d, 'r', 'e', "rem")
    CODE_CASE(d, '^', 0, "^")
    CODE_CASE(d, '<', '<', "<<")
    CODE_CASE(d, '>', '>', ">>")
    CODE_CASE(d, '<', 0, "<")
    CODE_CASE(d, '<', '=', "<=")
    CODE_CASE(d, '>', 0, ">")
    CODE_CASE(d, '>', '=', ">=")
    CODE_CASE(d, 'd', 'i', "divides")
    CODE_CASE(d, 'm', 'o', "modExp")
    default:
      CantHappen("bad number op");
    }
  FreeSymbol::getDataAttachments(opDeclaration, purposes, data);
}

void
NumberOpSymbol::getSymbolAttachments(Vector<const char*>& purposes,
				     Vector<Symbol*>& symbols)
{
  APPEND_SYMBOL(purposes, symbols, succSymbol);
  APPEND_SYMBOL(purposes, symbols, minusSymbol);
  FreeSymbol::getSymbolAttachments(purposes, symbols);
}

void
NumberOpSymbol::getTermAttachments(Vector<const char*>& purposes,
				   Vector<Term*>& terms)
{
  APPEND_TERM(purposes, terms, trueTerm);
  APPEND_TERM(purposes, terms, falseTerm);
  FreeSymbol::getTermAttachments(purposes, terms);
}

void
NumberOpSymbol::postInterSymbolPass()
{
  PREPARE_TERM(trueTerm);
  PREPARE_TERM(falseTerm);
}

void
NumberOpSymbol::reset()
{
  trueTerm.reset();  // so true dag can be garbage collected
  falseTerm.reset();  // so false dag can be garbage collected
  FreeSymbol::reset();  // parents reset() tasks
}

bool
NumberOpSymbol::eqRewrite(DagNode* subject, RewritingContext& context)
{
  Assert(this == subject->symbol(), "bad symbol");
  int nrArgs = arity();
  FreeDagNode* d = safeCast(FreeDagNode*, subject);
  bool specialEval = (succSymbol != 0);
  //
  //	Evaluate our arguments and check that they are all numbers
  //
  for (int i = 0; i < nrArgs; i++)
    {
      DagNode* a = d->getArgument(i);
      a->reduce(context);
      if (specialEval &&
	  !((a->symbol() == minusSymbol) ? minusSymbol->isNeg(a) : succSymbol->isNat(a)))
	specialEval = false;
    }
  if (specialEval)
    {
      mpz_class storage0;
      DagNode* d0 = d->getArgument(0);
      const mpz_class& a0 = (d0->symbol() == minusSymbol) ? 
	minusSymbol->getNeg(d0, storage0) : succSymbol->getNat(d0);
      mpz_class r;
      if (nrArgs == 1)
	{
	  switch (op)
	    {
	    case '-':
	      {
		if (a0 > 0)  // avoid infinite loop!
		  goto fail;
		r = -a0;
		break;
	      }
	    case '~':
	      {
		if (minusSymbol == 0)
		  goto fail;
		r = ~a0;
		break;
	      }
	    case CODE('a', 'b'):
	      {
		r = abs(a0);
		break;
	      }
	    default:
	      CantHappen("bad number op");
	    }
	}
      else
	{
	  mpz_class storage1;
	  DagNode* d1 = d->getArgument(1);
	  const mpz_class& a1 = (d1->symbol() == minusSymbol) ? 
	    minusSymbol->getNeg(d1, storage1) : succSymbol->getNat(d1);
	  if (nrArgs == 2)
	    {
	      switch (op)
		{
		  //
		  //	These seven operations are really AC and are just
		  //	included here for completeness.
		  //
		case '+':
		  {
		    r = a0 + a1;
		    break;
		  }
		case '*':
		  {
		    r = a0 * a1;
		    break;
		  }
		case CODE('g', 'c'):
		  {
		    mpz_gcd(r.get_mpz_t(), a0.get_mpz_t(), a1.get_mpz_t());
		    break;
		  }
		case CODE('l', 'c'):
		  {
		    mpz_lcm(r.get_mpz_t(), a0.get_mpz_t(), a1.get_mpz_t());
		    break;
		  }
		case '&':
		  {
		    r = a0 & a1;
		    break;
		  }
		case '|':
		  {
		    r = a0 | a1;
		    break;
		  }
		case CODE('x', 'o'):
		  {
		    r = a0 ^ a1;
		    break;
		  }
		  //
		  //	These remaining operations, together with the unary
		  //	operations above are the raison d'etre for this class.
		  //
		case '-':
		  {
		    if (minusSymbol == 0)
		      goto fail;
		    r = a0 - a1;
		    break;
		  }
		case CODE('q', 'u'):
		  {
		    if (a1 == 0)
		      goto fail;
		    r = a0 / a1;
		    break;
		  }
		case CODE('r', 'e'):
		  {
		    if (a1 == 0)
		      goto fail;
		    r = a0 % a1;
		    break;
		  }
		case '^':
		  {
		    if (a1 < 0)
		      goto fail;
		    if (a1 > EXPONENT_BOUND)  // handle a0 = -1, 0 or 1
		      {
			if (a0 == 0 || a0 == 1)
			  {
			    r = a0;
			    break;
			  }
			else if (a0 == -1)
			  {
			    mpz_class t = 1;
			    r = ((a1 & t) == 0) ? 1 : -1;
			    break;
			  }
			goto fail;
		      }
		    mpz_pow_ui(r.get_mpz_t(), a0.get_mpz_t(), mpz_get_ui(a1.get_mpz_t()));
		    break;
		  }
		case CODE('<', '<'):
		  {
		    if (a1 < 0)
		      goto fail;
		    if (a1 > EXPONENT_BOUND)
		      {
			if (a0 == 0)
			  {
			    r = 0;
			    break;
			  }
			goto fail;
		      }
		    r = a0 << mpz_get_ui(a1.get_mpz_t());
		    break;
		  }
		case CODE('>', '>'):
		  {
		    if (a1 < 0)
		      goto fail;
		    if (mpz_fits_uint_p(a1.get_mpz_t()))
		      {
			//
			//	Note that that the C++ binding of operator>> defines
			//	a0 >> a1 using mpz_tdiv_q_2exp() which gives a logical
			//	rather than an arithmetic right shift
			//	with respect to 2's complement representation.
			//
			mpz_fdiv_q_2exp(r.get_mpz_t(), a0.get_mpz_t(), mpz_get_ui(a1.get_mpz_t()));
		      }
		    else
		      {
			mpz_fdiv_q_2exp(r.get_mpz_t(), a0.get_mpz_t(), UINT_MAX);
			if (r != 0 && r != -1)
			  {
			    //
			    //	We get here only in extreme cases where a1
			    //	is in the billions and a0 has billions of
			    //	significant bits.
			    //
			    mpz_class t(a1);
			    t -= UINT_MAX;
			    while (!(mpz_fits_uint_p(t.get_mpz_t())))
			      {
				mpz_fdiv_q_2exp(r.get_mpz_t(), r.get_mpz_t(), UINT_MAX);
				if (r == 0 || r == -1)
				  goto done;
				t -= UINT_MAX;
			      }
			    mpz_fdiv_q_2exp(r.get_mpz_t(), r.get_mpz_t(), mpz_get_ui(t.get_mpz_t()));
			  }
		      done:
			;
		      }
		    break;
		  }
		default:
		  {
		    bool b;
		    switch (op)
		      {
		      case '<':
			{
			  b = a0 < a1;
			  break;
			}
		      case CODE('<', '='):
			{
			  b = a0 <= a1;
			  break;
			}
		      case '>':
			{
			  b = a0 > a1;
			  break;
			}
		      case CODE('>', '='):
			{
			  b = a0 >= a1;
			  break;
			}
		      case CODE('d', 'i'):
			{
			  if (a0 == 0)
			    goto fail;
			  b = mpz_divisible_p(a1.get_mpz_t(), a0.get_mpz_t());
			  break;
			}
		      default:
			CantHappen("bad number op");
		      }
		    Assert(trueTerm.getTerm() != 0 && falseTerm.getTerm() != 0,
			   "null true/false for relational op");
		    return context.builtInReplace(subject, b ? trueTerm.getDag() :
						  falseTerm.getDag());
		  }
		}
	    }
	  else
	    {
	      Assert(nrArgs == 3, "bad number of args");
	      mpz_class storage2;
	      DagNode* d2 = d->getArgument(2);
	      const mpz_class& a2 = (d2->symbol() == minusSymbol) ? 
		minusSymbol->getNeg(d2, storage2) : succSymbol->getNat(d2);
	      switch (op)
		{
		case CODE('m', 'o'):
		  {
		    if (a0 < 0 || a1 < 0 || a2 < 0)
		      goto fail;
		    mpz_powm(r.get_mpz_t(), a0.get_mpz_t(), a1.get_mpz_t(), a2.get_mpz_t());
		    break;
		  }
		default:
		  CantHappen("bad number op");
		}
	    }
	}
      Assert(minusSymbol != 0 || r >= 0, "can't make -ve int");
      return (r >= 0) ? succSymbol->rewriteToNat(subject, context, r) :
	context.builtInReplace(subject, minusSymbol->makeNegDag(r));
    }
 fail:
  return FreeSymbol::eqRewrite(subject, context);
}
