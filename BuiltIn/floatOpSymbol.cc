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
//      Implementation for class FloatOpSymbol.
//
#include <math.h>
#ifdef SOLARIS
#include <ieeefp.h>
#endif

//      utility stuff
#include "macros.hh"
#include "vector.hh"

//      forward declarations
#include "interface.hh"
#include "core.hh"
#include "freeTheory.hh"
#include "NA_Theory.hh"
#include "builtIn.hh"

//      interface class definitions
#include "symbol.hh"
#include "dagNode.hh"
#include "term.hh"

//      core class definitions
#include "rewritingContext.hh"
#include "symbolMap.hh"

//      free theory class definitions
#include "freeNet.hh"
#include "freeDagNode.hh"

//      built in class definitions
#include "succSymbol.hh"
#include "minusSymbol.hh"
#include "divisionSymbol.hh"
#include "floatSymbol.hh"
#include "floatDagNode.hh"
#include "floatOpSymbol.hh"
#include "bindingMacros.hh"

FloatOpSymbol::FloatOpSymbol(int id, int arity)
  : FreeSymbol(id, arity)
{
  op = NONE;
  floatSymbol = 0;
  succSymbol = 0;
  minusSymbol = 0;
  divisionSymbol = 0;
}

bool
FloatOpSymbol::attachData(const Vector<Sort*>& opDeclaration,
			  const char* purpose,
			  const Vector<const char*>& data)
{
  BIND_OP(purpose, FloatOpSymbol, op, data);
  return FreeSymbol::attachData(opDeclaration, purpose, data);
}

bool
FloatOpSymbol::attachSymbol(const char* purpose, Symbol* symbol)
{
  BIND_SYMBOL(purpose, symbol, floatSymbol, FloatSymbol*);
  BIND_SYMBOL(purpose, symbol, succSymbol, SuccSymbol*);
  BIND_SYMBOL(purpose, symbol, minusSymbol, MinusSymbol*);
  BIND_SYMBOL(purpose, symbol, divisionSymbol, DivisionSymbol*);
  return FreeSymbol::attachSymbol(purpose, symbol);
}

bool
FloatOpSymbol::attachTerm(const char* purpose, Term* term)
{
  BIND_TERM(purpose, term, trueTerm);
  BIND_TERM(purpose, term, falseTerm);
  return FreeSymbol::attachTerm(purpose, term);
}

void
FloatOpSymbol::copyAttachments(Symbol* original, SymbolMap* map)
{
  FloatOpSymbol* orig = safeCast(FloatOpSymbol*, original);
  op = orig->op;
  COPY_SYMBOL(orig, floatSymbol, map, FloatSymbol*);
  COPY_SYMBOL(orig, succSymbol, map, SuccSymbol*);
  COPY_SYMBOL(orig, minusSymbol, map, MinusSymbol*);
  COPY_SYMBOL(orig, divisionSymbol, map, DivisionSymbol*);
  COPY_TERM(orig, trueTerm, map);
  COPY_TERM(orig, falseTerm, map);
  FreeSymbol::copyAttachments(original, map);
}

void
FloatOpSymbol::postInterSymbolPass()
{
  PREPARE_TERM(trueTerm);
  PREPARE_TERM(falseTerm);
}

void
FloatOpSymbol::reset()
{
  trueTerm.reset();  // so true dag can be garbage collected
  falseTerm.reset();  // so false dag can be garbage collected
  FreeSymbol::reset();  // parents reset() tasks
}

bool
FloatOpSymbol::eqRewrite(DagNode* subject, RewritingContext& context)
{
  Assert(this == subject->symbol(), "bad symbol");
  int nrArgs = arity();
  FreeDagNode* d = static_cast<FreeDagNode*>(subject);
  bool floatEval = true;
  //
  //	Evaluate our arguments and check that they are all floats.
  //
  for (int i = 0; i < nrArgs; i++)
    {
      DagNode* a = d->getArgument(i);
      a->reduce(context);
      if (a->symbol() != floatSymbol)
	floatEval = false;
    }
  if (floatEval)
    {
      double a1 = static_cast<FloatDagNode*>(d->getArgument(0))->getValue();
      double r;
      if (nrArgs == 1)
	{
	  switch (op)
	    {
	    case '-':
	      r = -a1;
	      break;
	    case CODE('a', 'b'):
	      r = fabs(a1);
	      break;
	    case CODE('f', 'l'):
	      r = floor(a1);
	      break;
	    case CODE('c', 'e'):
	      r = ceil(a1);
	      break;
	    case CODE('s', 'q'):
	      r = sqrt(a1);
	      break;
	    case CODE('e', 'x'):
	      r = exp(a1);
	      break;
	    case CODE('l', 'o'):
	      {
		if (a1 < 0)
		  goto fail;  // some platforms return NaN, some -Infinity
		r = log(a1);
		break;
	      }
	    case CODE('s', 'i'):
	      r = sin(a1);
	      break;
	    case CODE('c', 'o'):
	      r = cos(a1);
	      break;
	    case CODE('t', 'a'):
	      r = tan(a1);
	      break;
	    case CODE('a', 's'):
	      {
		if (a1 < -1.0 || a1 > 1.0)
		  goto fail;
		r = asin(a1);
		break;
	      }
	    case CODE('a', 'c'):
	      {
		if (a1 < -1.0 || a1 > 1.0)
		  goto fail;
		r = acos(a1);
		break;
	      }
	    case CODE('a', 't'):
	      r = atan(a1);
	      break;
	    case CODE('r', 'a'):
	      {
		if (!(finite(a1)))
		  goto fail;
		mpq_class t;
		mpq_set_d(t.get_mpq_t(), a1);
		const mpz_class& numerator = t.get_num();
		const mpz_class& denominator = t.get_den();
		DagNode* r;
		if (denominator == 1)
		  {
		    if (numerator >= 0)
		      return succSymbol->rewriteToNat(subject, context, numerator);
		    r = minusSymbol->makeNegDag(numerator);
		  }
		else
		  r = divisionSymbol->makeRatDag(numerator, denominator);
		return context.builtInReplace(subject, r);
	      }
	    default:
	      CantHappen("bad float op");
	    }
	}
      else
	{
	  double a2 = static_cast<FloatDagNode*>(d->getArgument(1))->getValue();
	  switch (op)
	    {
	    case '+':
	      r = a1 + a2;
	      break;
	    case '-':
	      r = a1 - a2;
	      break;
	    case '*':
	      r = a1 * a2;
	      break;
	    case '/':
	      {
		if (a2 == 0)
		  goto fail;
		r = a1 / a2;
		break;
	      }
	    case CODE('r', 'e'):
	      {
		if (a2 == 0)
		  goto fail;
		r = fmod(a1, a2);
		break;
	      }
	    case '^':
	      {
		bool defined;
		r = safePow(a1, a2, defined);
		if (!defined)
		  goto fail;
		break;
	      }
	    case CODE('a', 't'):
	      {
		if (!finite(a1) && !finite(a2))
		  {
		    //
		    //	Double infinity case: make args finite
		    //
		    a1 = (a1 < 0) ? -1 : 1;
		    a2 = (a2 < 0) ? -1 : 1;
		  }
		r = atan2(a1, a2);
		break;
	      }
	    default:
	      {
		switch (op)
		  {
		  case '<':
		    r = a1 < a2;
		    break;
		  case CODE('<', '='):
		    r = a1 <= a2;
		    break;
		  case '>':
		    r = a1 > a2;
		    break;
		  case CODE('>', '='):
		    r = a1 >= a2;
		    break;
		  default:
		    CantHappen("bad float op");
		  }
		Assert(trueTerm.getTerm() != 0 && falseTerm.getTerm() != 0,
		       "null true/false for relational op");
		return context.builtInReplace(subject, r ? trueTerm.getDag() : falseTerm.getDag());
	      }
	    }
	}
      if (!isnan(r))
	return floatSymbol->rewriteToFloat(subject, context, r);  
    }
  else if (nrArgs == 1)
    {
      DagNode* a0 = d->getArgument(0);
      if (op == CODE('f', 'l'))
	{
	  Assert(succSymbol != 0, "succSymbol undefined");
	  if (succSymbol->isNat(a0))
	    {
	      mpq_class tq(succSymbol->getNat(a0), 1);
	      return floatSymbol->rewriteToFloat(subject, context, mpq_get_d(tq.get_mpq_t()));
	    }
	  else if (a0->symbol() == minusSymbol)
	    {
	      if (minusSymbol->isNeg(a0))
		{
		  mpz_class result;
		  mpq_class tq(minusSymbol->getNeg(a0, result), 1);
		  return floatSymbol->rewriteToFloat(subject, context, mpq_get_d(tq.get_mpq_t()));
		}
	    }
	  else if (a0->symbol() == divisionSymbol)
	    {
	      if (divisionSymbol->isRat(a0))
		{
		  mpz_class numerator;
		  const mpz_class& denomenator = divisionSymbol->getRat(a0, numerator);
		  mpq_class tq(numerator, denomenator);
		  return floatSymbol->rewriteToFloat(subject, context, mpq_get_d(tq.get_mpq_t()));
		}
	    }
	}
    }
 fail:
  return FreeSymbol::eqRewrite(subject, context);
}

int
FloatOpSymbol::isOdd(double n)
{
  //
  //	Decide if a floating point number is odd or even;
  //	return -1 if neither or can't decide.
  //
  if (n != floor(n))
    return -1;  // fractional
  if (n < 0)
    n = -n;
  if (n > INT_DOUBLE_MAX)  // oddness is essentially random
    return -1;
  return static_cast<Int64>(n) & 1;
}

double
FloatOpSymbol::safePow(double a1, double a2, bool& defined)
{
  defined = true;
  if (isnan(a1))
    {
      defined = false;
      return a1;
    }
  if (isnan(a2))
    {
      defined = false;
      return a2;
    }
  if (!finite(a1))
    {
      if (a2 == 0.0)
	return 1.0;
      if (a2 < 0)
	return 0.0;
      if (a1 > 0)
	return a1;
      int odd = isOdd(a2);
      if (odd == -1)
	{
	  defined = false;
	  return 0.0;
	}
      return odd ? a1 : -a1;
    }
  if (!finite(a2))
    {
      if (a1 > 1.0)
	return a2 > 0 ? a2 : 0;
      if (a1 == 1.0)
	return 1.0;
      if (a1 > 0.0)
	return a2 < 0 ? -a2 : 0;
      if (a2 > 0)
	{
	  if (a1 <= -1.0)
	    defined = false;
	}
      else
	{
	  if (a1 >= -1.0)
	    defined = false;
	}
      return 0;
    }
  double r = pow(a1, a2);
  if (isnan(r))
    defined = false;
  else if (a1 < 0 && r != 0)
    {
      //
      //	Some platforms get this badly wrong.
      //
      int odd = isOdd(a2);
      if (odd == -1)
	defined = false;
      else if ((odd == 1) != (r < 0))
	r = -r;  // fix sign if pow() got it wrong
    }
  return r;
}
