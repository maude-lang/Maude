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
//      Implementation for class StringOpSymbol.
//

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
#include "floatSymbol.hh"
#include "floatDagNode.hh"
#include "succSymbol.hh"
#include "minusSymbol.hh"
#include "divisionSymbol.hh"
#include "stringSymbol.hh"
#include "stringDagNode.hh"
#include "stringOpSymbol.hh"
#include "bindingMacros.hh"

StringOpSymbol::StringOpSymbol(int id, int arity)
  : FreeSymbol(id, arity)
{
  op = NONE;
  stringSymbol = 0;
  succSymbol = 0;
  minusSymbol = 0;
  divisionSymbol = 0;
  floatSymbol = 0;
  decFloatSymbol = 0;
}

bool
StringOpSymbol::attachData(const Vector<Sort*>& opDeclaration,
			   const char* purpose,
			   const Vector<const char*>& data)
{
  BIND_OP(purpose, StringOpSymbol, op, data);
  return FreeSymbol::attachData(opDeclaration, purpose, data);
}

bool
StringOpSymbol::attachSymbol(const char* purpose, Symbol* symbol)
{
  BIND_SYMBOL(purpose, symbol, stringSymbol, StringSymbol*);
  BIND_SYMBOL(purpose, symbol, succSymbol, SuccSymbol*);
  BIND_SYMBOL(purpose, symbol, minusSymbol, MinusSymbol*);
  BIND_SYMBOL(purpose, symbol, divisionSymbol, DivisionSymbol*);
  BIND_SYMBOL(purpose, symbol, floatSymbol, FloatSymbol*);
  BIND_SYMBOL(purpose, symbol, decFloatSymbol, Symbol*);
  return FreeSymbol::attachSymbol(purpose, symbol);
}

bool
StringOpSymbol::attachTerm(const char* purpose, Term* term)
{
  BIND_TERM(purpose, term, trueTerm);
  BIND_TERM(purpose, term, falseTerm);
  BIND_TERM(purpose, term, notFoundTerm);
  return FreeSymbol::attachTerm(purpose, term);
}

void
StringOpSymbol::copyAttachments(Symbol* original, SymbolMap* map)
{
  StringOpSymbol* orig = safeCast(StringOpSymbol*, original);
  op = orig->op;
  COPY_SYMBOL(orig, stringSymbol, map, StringSymbol*);
  COPY_SYMBOL(orig, succSymbol, map, SuccSymbol*);
  COPY_SYMBOL(orig, minusSymbol, map, MinusSymbol*);
  COPY_SYMBOL(orig, divisionSymbol, map, DivisionSymbol*);
  COPY_SYMBOL(orig, floatSymbol, map, FloatSymbol*);
  COPY_SYMBOL(orig, decFloatSymbol, map, Symbol*);
  COPY_TERM(orig, trueTerm, map);
  COPY_TERM(orig, falseTerm, map);
  COPY_TERM(orig, notFoundTerm, map);
  FreeSymbol::copyAttachments(original, map);
}

void
StringOpSymbol::getDataAttachments(const Vector<Sort*>& opDeclaration,
				   Vector<const char*>& purposes,
				   Vector<Vector<const char*> >& data)
{
  int nrDataAttachments = purposes.length();
  purposes.resize(nrDataAttachments + 1);
  purposes[nrDataAttachments] = "StringOpSymbol";
  data.resize(nrDataAttachments + 1);
  data[nrDataAttachments].resize(1);
  const char*& d = data[nrDataAttachments][0];
  switch (op)
    {
    CODE_CASE(d, 'f', 'l', "float")
    CODE_CASE(d, 'l', 'e', "length")
    CODE_CASE(d, 'a', 's', "ascii")
    CODE_CASE(d, '+', 0, "+")
    CODE_CASE(d, '<', 0, "<")
    CODE_CASE(d, '<', '=', "<=")
    CODE_CASE(d, '>', 0, ">")
    CODE_CASE(d, '>', '=', ">=")
    CODE_CASE(d, 'r', 'a', "rat")
    CODE_CASE(d, 's', 'u', "substr")
    CODE_CASE(d, 'f', 'i', "find")
    CODE_CASE(d, 'r', 'f', "rfind")
    CODE_CASE(d, 's', 't', "string")
    CODE_CASE(d, 'd', 'e', "decFloat")
    CODE_CASE(d, 'c', 'h', "char")
    default:
      CantHappen("bad string op");
    }
  FreeSymbol::getDataAttachments(opDeclaration, purposes, data);
}

void
StringOpSymbol::getSymbolAttachments(Vector<const char*>& purposes,
				     Vector<Symbol*>& symbols)
{
  APPEND_SYMBOL(purposes, symbols, stringSymbol);
  APPEND_SYMBOL(purposes, symbols, succSymbol);
  APPEND_SYMBOL(purposes, symbols, minusSymbol);
  APPEND_SYMBOL(purposes, symbols, divisionSymbol);
  APPEND_SYMBOL(purposes, symbols, floatSymbol);
  APPEND_SYMBOL(purposes, symbols, decFloatSymbol);
  FreeSymbol::getSymbolAttachments(purposes, symbols);
}

void
StringOpSymbol::getTermAttachments(Vector<const char*>& purposes,
				   Vector<Term*>& terms)
{
  APPEND_TERM(purposes, terms, trueTerm);
  APPEND_TERM(purposes, terms, falseTerm);
  APPEND_TERM(purposes, terms, notFoundTerm);
  FreeSymbol::getTermAttachments(purposes, terms);
}

void
StringOpSymbol::postInterSymbolPass()
{
  PREPARE_TERM(trueTerm);
  PREPARE_TERM(falseTerm);
  PREPARE_TERM(notFoundTerm);
}

void
StringOpSymbol::reset()
{
  trueTerm.reset();  // so true dag can be garbage collected
  falseTerm.reset();  // so false dag can be garbage collected
  notFoundTerm.reset();  // so notFound dag can be garbage collected
  FreeSymbol::reset();  // parents reset() tasks
}

bool
StringOpSymbol::eqRewrite(DagNode* subject, RewritingContext& context)
{
  Assert(this == subject->symbol(), "bad symbol");
  int nrArgs = arity();
  FreeDagNode* d = safeCast(FreeDagNode*, subject);
  //
  //	Evaluate our arguments.
  //
  for (int i = 0; i < nrArgs; i++)
    {
      DagNode* a = d->getArgument(i);
      a->reduce(context);
    }
  DagNode* a0 = d->getArgument(0);
  if (a0->symbol() == stringSymbol)
    {
      const crope& left = safeCast(StringDagNode*, a0)->getValue();
      switch (nrArgs)
	{
	case 1:
	  {
	    mpz_class r;
	    switch (op)
	      {
	      case CODE('f', 'l'):
		{
		  bool error;
		  //double fl = stringToDouble(left.c_str(), error);
		  //
		  //	This messing about is needed because Rope::c_str()
		  //	fails in libstdc++-v3
		  //
		  int len = left.length();
		  char* t = new char[len + 1];
		  left.copy(t);
		  t[len] = '\0';
		  double fl = stringToDouble(t, error);
		  delete [] t;
		  //
		  if (error)
		    goto fail;
		  return floatSymbol->rewriteToFloat(subject, context, fl);
		}
	      case CODE('l', 'e'):  // length
		{
		  r = left.length();
		  break;
		}
	      case CODE('a', 's'):  // acsii
		{
		  if (left.length() != 1)
		    goto fail;
		  r = static_cast<unsigned char>(left[0]);
		  break;
		}
	      default:
		CantHappen("bad string op");
	      }
	    return succSymbol->rewriteToNat(subject, context, r);
	  }
	case 2:
	  {
	    DagNode* a1 = d->getArgument(1);
	    if (a1->symbol() == stringSymbol)
	      {
		const crope& right = safeCast(StringDagNode*, a1)->getValue();
		bool r;
		switch (op)
		  {
		  case '+':
		    {
		      crope t(left);
		      t += right;
		      return rewriteToString(subject, context, t);
		    }
		  case '<':
		    r = left < right;
		    break;
		  case '>':
		    r = left > right;
		    break;
		  case CODE('<', '='):
		    r = left <= right;
		    break;
		  case CODE('>', '='):
 		    r = left >= right;
		    break;
		  default:
		    CantHappen("bad string op");
		  }
		Assert(trueTerm.getTerm() != 0 && falseTerm.getTerm() != 0,
		       "null true/false for relational op");
		return context.builtInReplace(subject, r ? trueTerm.getDag() : falseTerm.getDag());
	      }
	    else if (op == CODE('r', 'a'))
	      {
		DagNode* a1 = d->getArgument(1);
		Assert(succSymbol != 0, "succSymbol undefined");
		if (succSymbol->isNat(a1))
		  {
		    const mpz_class& n1 = succSymbol->getNat(a1);
		    if (n1 >= 2 && n1 <= 36)
		      {
			mpz_class numerator;
			mpz_class denominator;
			if (ropeToNumber(left, n1.get_si(), numerator, denominator))
			  {
			    DagNode* r;
			    if (denominator == 0)
			      {
				if (numerator >= 0)
				  return succSymbol->rewriteToNat(subject, context, numerator);
				r = minusSymbol->makeNegDag(numerator);
			      }
			    else
			      r = divisionSymbol->makeRatDag(numerator, denominator);
			    return context.builtInReplace(subject, r);
			  }
		      }
		  }
	      }
	    break;
	  }
	case 3:
	  {
	    switch (op)
	      {
	      case CODE('s', 'u'):  // substr
		{
		  DagNode* a1 = d->getArgument(1);
		  DagNode* a2 = d->getArgument(2);
		  Assert(succSymbol != 0, "succSymbol undefined");
		  if (succSymbol->isNat(a1) && succSymbol->isNat(a2))
		    {
		      const mpz_class& n1 = succSymbol->getNat(a1);
		      Uint index = n1.fits_uint_p() ? n1.get_ui() : UINT_MAX;
		      const mpz_class& n2 = succSymbol->getNat(a2);
		      Uint length = n2.fits_uint_p() ? n2.get_ui() : UINT_MAX;
		      return rewriteToString(subject, context, substring(left, index, length));
		    }
		  break;
		}
	      default:
		{
		  DagNode* a1 = d->getArgument(1);
		  if (a1->symbol() == stringSymbol)
		    {
		      const crope& pattern = safeCast(StringDagNode*, a1)->getValue();
		      DagNode* a2 = d->getArgument(2);
		      Assert(succSymbol != 0, "succSymbol undefined");
		      if (succSymbol->isNat(a2))
			{
			  const mpz_class& n2 = succSymbol->getNat(a2);
			  Uint index = n2.fits_uint_p() ? n2.get_ui() : UINT_MAX;
			  int r;
			  switch (op)
			    {
			    case CODE('f', 'i'):  // find
			      r = fwdFind(left, pattern, index);
			      break;
			    case CODE('r', 'f'):  // rfind
			      r = revFind(left, pattern, index);
			      break;
			    default:
			      CantHappen("bad string op");
			    }
			  Assert(notFoundTerm.getTerm() != 0, "null notFound for find op");
			  if (r == NONE)
			    return context.builtInReplace(subject, notFoundTerm.getDag());
			  return succSymbol->rewriteToNat(subject, context, r);
			}
		    }
		  break;
		}
	      }
	  }
	}
    }
  else if (a0->symbol() == floatSymbol)
    {
      if (nrArgs == 1 && op == CODE('s', 't'))
	{
	  double fl = safeCast(FloatDagNode*, a0)->getValue();
	  return rewriteToString(subject, context, doubleToString(fl));
	}
      else if (nrArgs == 2 && op == CODE('d', 'e'))
	{
	  DagNode* a1 = d->getArgument(1);
	  Assert(succSymbol != 0, "succSymbol undefined");
	  Assert(minusSymbol != 0, "minusSymbol undefined");
	  if (succSymbol->isNat(a1))
	    {
	      double fl = safeCast(FloatDagNode*, a0)->getValue();
	      const mpz_class& n1 = succSymbol->getNat(a1);
	      int nrDigits = (n1 < MAX_FLOAT_DIGITS) ? n1.get_si() : MAX_FLOAT_DIGITS;
	      char buffer[MAX_FLOAT_DIGITS + 1];
	      int decPt;
	      int sign;
	      correctEcvt(fl, nrDigits, buffer, decPt, sign);
	      Vector<DagNode*> args(0, 3);
	      args.append((sign < 0) ? minusSymbol->makeNegDag(sign) :
			  succSymbol->makeNatDag(sign));
	      args.append(new StringDagNode(stringSymbol, buffer));
	      args.append((decPt < 0) ? minusSymbol->makeNegDag(decPt) :
			  succSymbol->makeNatDag(decPt));
	      return context.builtInReplace(subject, decFloatSymbol->makeDagNode(args));
	    }
	}
    } 
  else if (op == CODE('s', 't') && nrArgs == 2)
    {
      DagNode* a1 = d->getArgument(1);
      Assert(succSymbol != 0, "succSymbol undefined");
      if (succSymbol->isNat(a1))
	{
	  const mpz_class& n1 = succSymbol->getNat(a1);
	  if (n1 >= 2 && n1 <= 36)
	    {
	      int base = n1.get_si();
	      if (succSymbol->isNat(a0))
		{
		  if (succSymbol->isNat(a0))
		    {
		      char* ts = mpz_get_str(0, base, succSymbol->getNat(a0).get_mpz_t());
		      crope tr(ts);
		      free(ts);
		      return rewriteToString(subject, context, tr);
		    }
		}
	      else if (a0->symbol() == minusSymbol)
		{
		  if (minusSymbol->isNeg(a0))
		    {
		      mpz_class result;
		      char* ts =
			mpz_get_str(0, base, minusSymbol->getNeg(a0, result).get_mpz_t());
		      crope tr(ts);
		      free(ts);
		      return rewriteToString(subject, context, tr);
		    }
		}
	      else if (a0->symbol() == divisionSymbol)
		{
		  if (divisionSymbol->isRat(a0))
		    {
		      mpz_class numerator;
		      const mpz_class& denomenator = divisionSymbol->getRat(a0, numerator);
		      char* ns = mpz_get_str(0, base, numerator.get_mpz_t());
		      crope tr(ns);
		      free(ns);
		      tr += '/';
		      char* ds = mpz_get_str(0, base, denomenator.get_mpz_t());
		      tr += ds;
		      free(ds);
		      return rewriteToString(subject, context, tr);
		    }
		}
	    }
	}
    }
  else 
    {
      switch (op)
	{
	case CODE('c', 'h'):  // char
	  {
	    DagNode* a0 = d->getArgument(0);
	    Assert(succSymbol != 0, "succSymbol undefined");
	    if (succSymbol->isNat(a0))
	      {
		const mpz_class& n0 = succSymbol->getNat(a0);
		if (n0 <= 255)
		  {
		    char c = n0.get_si();
		    return rewriteToString(subject, context, crope(c));
		  }
	      }
	    break;
	  }
	default:
	  ;  // Can get here if args are bad
	}
    }
 fail:
  return FreeSymbol::eqRewrite(subject, context);
}

bool
StringOpSymbol::rewriteToString(DagNode* subject, RewritingContext& context, const crope& result)
{
  bool trace = RewritingContext::getTraceStatus();
  if (trace)
    {
      context.tracePreEqRewrite(subject, 0, RewritingContext::BUILTIN);
      if (context.traceAbort())
	return false;
    }
  (void) new(subject) StringDagNode(stringSymbol, result);
  context.incrementEqCount();
  if (trace)
    context.tracePostEqRewrite(subject);
  return true;
}

crope
StringOpSymbol::substring(const crope& subject, crope::size_type index, crope::size_type length)
{
  crope::size_type sLen = subject.length();
  //  if (index < 0)
  //    {
  //      if (length > 0)
  //	    length += index;
  //      index = 0;
  //    }
  if (length == 0 || index >= sLen)
    return crope();
  if (length > sLen - index)
    length = sLen - index;
  return subject.substr(index, length);
}
  
int
StringOpSymbol::fwdFind(const crope& subject, const crope& pattern, crope::size_type start)
{
  crope::size_type sLen = subject.length();
  if (pattern.empty())
    return (start <= sLen) ? static_cast<int>(start) : NONE;
  //
  //	Testing start < sLen is important because otherwise 2nd test
  //	could succeed by wrap around.
  //
  if (start < sLen && start + pattern.length() <= sLen)
    {
      crope::const_iterator b(subject.begin());
      crope::const_iterator e(subject.end());
      crope::const_iterator p(search(b + start, e, pattern.begin(), pattern.end()));
      if (p != e)
	return p - b;
    }
  return NONE;
}

int
StringOpSymbol::revFind(const crope& subject, const crope& pattern, crope::size_type start)
{
  crope::size_type sLen = subject.length();
  if (pattern.empty())
    return (start <= sLen) ? start : sLen;
  crope::size_type pLen = pattern.length();
  if (pLen <= sLen)
    {
      crope::size_type reflect = sLen - pLen;
      if (start > reflect)
	start = reflect;
      crope::const_reverse_iterator b(subject.rbegin());
      crope::const_reverse_iterator e(subject.rend());
      crope::const_reverse_iterator p =
	search(b + (reflect - start), e, pattern.rbegin(), pattern.rend());
      if (p != e)
	return reflect - (p - b);
    }
  return NONE;
}

bool
StringOpSymbol::ropeToNumber(const crope& subject,
			     int base,
			     mpz_class& numerator,
			     mpz_class& denominator)
{
  int len = subject.length();
  if (len == 0)
    return false;
  int i = 0;
  if (subject[i] == '-')
    {
      if (len == 1)
	return false;
      ++i;
    }
  char c = subject[i];
  if (!isalnum(c) || (c == '0' && len > 1))
    return false;
  for (i++; i < len; i++)
    {
      char c = subject[i];
      if (!isalnum(c))
	{
	  if (c == '/')
	    {
	      int j = i + 1;
	      if (j == len || subject[j] == '0')
		return false;
	      for (; j < len; j++)
		{
		  if (!isalnum(subject[j]))
		    return false;
		}
	      char* t = new char[len];  // longer than needed but who cares
	      int dLen = len - (i + 1);
	      subject.copy(i + 1, dLen, t);
	      t[dLen] = '\0';
	      if (mpz_set_str(denominator.get_mpz_t(), t, base) != 0)
		{
		  delete [] t;
		  return false;
		}
	      subject.copy(0, i, t);
	      t[i] = '\0';
	      if (mpz_set_str(numerator.get_mpz_t(), t, base) != 0)
		{
		  delete [] t;
		  return false;
		}
	      delete [] t;
	      return true;
	    }
	  else
	    return false;
	}
    }
  char* t = new char[len + 1];
  subject.copy(t);
  t[len] = '\0';
  if (mpz_set_str(numerator.get_mpz_t(), t, base) == 0)
    {
      delete [] t;
      denominator = 0;
      return true;
    }
  delete [] t;
  return false;    
}
