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
  BIND_OP3(purpose, StringOpSymbol, op, data);
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
    CODE_CASE3(d, 'f', 'l', 'o', "float")
    CODE_CASE3(d, 'l', 'e', 'n', "length")
    CODE_CASE3(d, 'a', 's', 'c', "ascii")
    CODE_CASE3(d, 'u', 'p', 'p', "upperCase")
    CODE_CASE3(d, 'l', 'o', 'w', "lowerCase")
    CODE_CASE(d, '+', 0, "+")
    CODE_CASE(d, '<', 0, "<")
    CODE_CASE(d, '<', '=', "<=")
    CODE_CASE(d, '>', 0, ">")
    CODE_CASE(d, '>', '=', ">=")
    CODE_CASE3(d, 'r', 'a', 't', "rat")
    CODE_CASE3(d, 's', 'u', 'b', "substr")
    CODE_CASE3(d, 'f', 'i', 'n', "find")
    CODE_CASE3(d, 'r', 'f', 'i', "rfind")
    CODE_CASE3(d, 's', 't', 'r', "string")
    CODE_CASE3(d, 'd', 'e', 'c', "decFloat")
    CODE_CASE3(d, 'c', 'h', 'a', "char")

    CODE_CASE3(d, 'c', 'n', 't', "cntrl")
    CODE_CASE3(d, 'p', 'r', 'i', "print")
    CODE_CASE3(d, 's', 'p', 'a', "space")
    CODE_CASE3(d, 'b', 'l', 'a', "blank")
    CODE_CASE3(d, 'g', 'r', 'a', "graphic")
    CODE_CASE3(d, 'p', 'u', 'n', "punct")
    CODE_CASE3(d, 'a', 'l', 'n', "alnum")
    CODE_CASE3(d, 'a', 'l', 'p', "alpha")
    CODE_CASE3(d, 'i', 's', 'u', "isupper")
    CODE_CASE3(d, 'i', 's', 'l', "islower")
    CODE_CASE3(d, 'd', 'i', 'g', "digit")
    CODE_CASE3(d, 'x', 'd', 'i', "xdigit")

    CODE_CASE3(d, 'l', 't', 'r', "ltrim")
    CODE_CASE3(d, 'r', 't', 'r', "rtrim")
    CODE_CASE3(d, 't', 'r', 'i', "trim")
    CODE_CASE3(d, 's', 't', 'a', "startsWith")
    CODE_CASE3(d, 'e', 'n', 'd', "endsWith")
    default:
      CantHappen("bad string op" << op);
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
  DebugAdvisory("StringOpSymbol::eqRewrite() called on " << subject);
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
      const Rope& left = safeCast(StringDagNode*, a0)->getValue();
      switch (nrArgs)
	{
	case 1:
	  {
	    if (trueTerm.getTerm() == nullptr)
	      {
		//
		//	Not a predicate.
		//
		mpz_class r;
		switch (op)
		  {
		  case CODE3('f', 'l', 'o'):  // float
		    {
		      bool error;
		      char* flStr = left.makeZeroTerminatedString();
		      double fl = stringToDouble(flStr, error);
		      delete [] flStr;
		      
		      if (error)
			goto fail;
		      return floatSymbol->rewriteToFloat(subject, context, fl);
		    }
		  case CODE3('l', 'e', 'n'):  // length
		    {
		      return succSymbol->rewriteToNat(subject, context, left.length());
		    }
		  case CODE3('a', 's', 'c'):  // acsii
		    {
		      if (left.length() != 1)
			goto fail;
		      unsigned char c = left[0];
		      return succSymbol->rewriteToNat(subject, context, c);
		    }
		  case CODE3('u', 'p', 'p'):  // upperCase
		    {
		      Rope result = upperCase(left);
		      return rewriteToString(subject, context, result);
		    }
		  case CODE3('l', 'o', 'w'):  // lowerCase
		    {
		      Rope result = lowerCase(left);
		      return rewriteToString(subject, context, result);
		    }
		  case CODE3('l', 't', 'r'):  // trimStart (left trim)
		    {
		      Index len = left.length();
		      Rope::const_iterator j = left.begin();
		      for (Index i = 0; i < len; ++i, ++j)
			{
			  if (!isspace(static_cast<unsigned char>(*j)))
			    return rewriteToString(subject, context, left.substr(i, len - i));
			}
		      break;
		    }
		  case CODE3('r', 't', 'r'):  // trimEnd (right trim)
		    {
		      Rope::const_iterator j = left.end();
		      for (Index i = left.length(); i > 0; --i)
			{
			  --j;
			  if (!isspace(static_cast<unsigned char>(*j)))
			    return rewriteToString(subject, context, left.substr(0, i));
			}
		      break;
		    }
		  case CODE3('t', 'r', 'i'):  // trim
		    {
		      Index len = left.length();
		      Rope::const_iterator j = left.begin();
		      for (Index i = 0; i < len; ++i, ++j)
			{
			  if (!isspace(static_cast<unsigned char>(*j)))
			    {
			      Index nrCharsWanted = len - i;
			      for (Rope::const_iterator e = left.end() - 1;
				   nrCharsWanted > 1 && isspace(static_cast<unsigned char>(*e));
				   --nrCharsWanted, --e)
				;
			      return rewriteToString(subject, context, left.substr(i, nrCharsWanted));
			    }
			}
		      break;
		    }
		  default:
		    CantHappen("bad string op" << op);
		    goto fail;
		  }
		return rewriteToString(subject, context, Rope());  // empty rope
	      }
	    else
	      {
		//
		//	Character predicate.
		//
		if (left.length() != 1)
		  goto fail;
		unsigned char c = left[0];
		bool r;
		switch (op)
		  {
		  case CODE3('c', 'n', 't'):
		    {
		      r = iscntrl(c);
		      break;
		    }
		  case CODE3('p', 'r', 'i'):
		    {
		      r = isprint(c);
		      break;
		    }
		  case CODE3('s', 'p', 'a'):
		    {
		      r = isspace(c);
		      break;
		    }
		  case CODE3('b', 'l', 'a'):
		    {
		      r = isblank(c);
		      break;
		    }
		  case CODE3('g', 'r', 'a'):
		    {
		      r = isgraph(c);
		      break;
		    }
		  case CODE3('p', 'u', 'n'):
		    {
		      r = ispunct(c);
		      break;
		    }
		  case CODE3('a', 'l', 'n'):
		    {
		      r = isalnum(c);
		      break;
		    }
		  case CODE3('a', 'l', 'p'):
		    {
		      r = isalpha(c);
		      break;
		    }
		  case CODE3('i', 's', 'u'):  // to avoid clash with upperCase
		    {
		      r = isupper(c);
		      break;
		    }
		  case CODE3('i', 's', 'l'):  // to avoid clash with lowerCase
		    {
		      r = islower(c);
		      break;
		    }
		  case CODE3('d', 'i', 'g'):
		    {
		      r = isdigit(c);
		      break;
		    }
		  case CODE3('x', 'd', 'i'):
		    {
		      r = isxdigit(c);
		      break;
		    }
		  default:
		    {
		      CantHappen("bad string op" << op);
		      goto fail;
		    }
		  }
		return context.builtInReplace(subject, r ? trueTerm.getDag() : falseTerm.getDag());
	      }
	  }
	case 2:
	  {
	    DagNode* a1 = d->getArgument(1);
	    if (a1->symbol() == stringSymbol)
	      {
		const Rope& right = safeCast(StringDagNode*, a1)->getValue();
		bool r;
		switch (op)
		  {
		  case '+':
		    {
		      Rope t(left);
		      t += right;
		      return rewriteToString(subject, context, t);
		    }
		  case '<':
		    {
		      r = left < right;
		      break;
		    }
		  case '>':
		    {
		      r = left > right;
		      break;
		    }
		  case CODE('<', '='):
		    {
		      r = left <= right;
		      break;
		    }
		  case CODE('>', '='):
		    {
		      r = left >= right;
		      break;
		    }
		  case CODE3('s', 't', 'a'):  // startsWith
		  case CODE3('e', 'n', 'd'):  // endsWith
		    {
		      Index leftLen = left.length();
		      Index rightLen = right.length();
		      if (leftLen < rightLen)
			r = false;
		      else
			{
			  r = true;
			  Rope::const_iterator leftIter = (op == CODE3('s', 't', 'a')) ? left.begin() : left.begin() + (leftLen - rightLen);
			  Rope::const_iterator rightIter = right.begin();
			  for (Index i = 0; i < rightLen; ++i, ++leftIter, ++rightIter)
			    {
			      if (*leftIter != *rightIter)
				{
				  r = false;
				  break;
				}
			    }
			}
		      break;
		    }
		  default:
		    CantHappen("bad string op" << op);
		    goto fail;
		  }
		Assert(trueTerm.getTerm() != 0 && falseTerm.getTerm() != 0,
		       "null true/false for relational op");
		return context.builtInReplace(subject, r ? trueTerm.getDag() : falseTerm.getDag());
	      }
	    else if (op == CODE3('r', 'a', 't'))  // rat
	      {
		DebugAdvisory("StringOpSymbol::eqRewrite() entered rat case for " << subject);
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
			else
			  DebugAdvisory("StringOpSymbol::eqRewrite() rope to number failed " << subject);
		      }
		    else
		      DebugAdvisory("StringOpSymbol::eqRewrite() a1 out of range " << subject);
		  }
		else
		  DebugAdvisory("StringOpSymbol::eqRewrite() a1 not a nat " << subject);
		DebugAdvisory("StringOpSymbol::eqRewrite() failed to rewrite " << subject);
	      }
	    break;
	  }
	case 3:
	  {
	    switch (op)
	      {
	      case CODE3('s', 'u', 'b'):  // substr
		{
		  DagNode* a1 = d->getArgument(1);
		  DagNode* a2 = d->getArgument(2);
		  Assert(succSymbol != 0, "succSymbol undefined");
		  if (succSymbol->isNat(a1) && succSymbol->isNat(a2))
		    {
		      const mpz_class& n1 = succSymbol->getNat(a1);
		      unsigned long index = n1.fits_uint_p() ? n1.get_ui() : ULONG_MAX;
		      const mpz_class& n2 = succSymbol->getNat(a2);
		      unsigned long length = n2.fits_uint_p() ? n2.get_ui() : ULONG_MAX;
		      return rewriteToString(subject, context, substring(left, index, length));
		    }
		  break;
		}
	      default:
		{
		  DagNode* a1 = d->getArgument(1);
		  if (a1->symbol() == stringSymbol)
		    {
		      const Rope& pattern = safeCast(StringDagNode*, a1)->getValue();
		      DagNode* a2 = d->getArgument(2);
		      Assert(succSymbol != 0, "succSymbol undefined");
		      if (succSymbol->isNat(a2))
			{
			  const mpz_class& n2 = succSymbol->getNat(a2);
			  unsigned long index = n2.fits_uint_p() ? n2.get_ui() : ULONG_MAX;
			  int r;
			  switch (op)
			    {
			    case CODE3('f', 'i', 'n'):  // find
			      r = fwdFind(left, pattern, index);
			      break;
			    case CODE3('r', 'f', 'i'):  // rfind
			      r = revFind(left, pattern, index);
			      break;
			    default:
			      CantHappen("bad string op" << op);
			      goto fail;
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
      if (nrArgs == 1 && op == CODE3('s', 't', 'r'))  // string
	{
	  double fl = safeCast(FloatDagNode*, a0)->getValue();
	  return rewriteToString(subject, context, doubleToString(fl));
	}
      else if (nrArgs == 2 && op == CODE3('d', 'e', 'c'))  // decFloat
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
	      Vector<DagNode*> args(3);
	      args[0] = (sign < 0) ? minusSymbol->makeNegDag(sign) : succSymbol->makeNatDag(sign);
	      args[1] = new StringDagNode(stringSymbol, buffer);
	      args[2] = (decPt < 0) ? minusSymbol->makeNegDag(decPt) : succSymbol->makeNatDag(decPt);
	      return context.builtInReplace(subject, decFloatSymbol->makeDagNode(args));
	    }
	}
    } 
  else if (op == CODE3('s', 't', 'r') && nrArgs == 2)
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
		      Rope tr(ts);
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
		      Rope tr(ts);
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
		      Rope tr(ns);
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
	case CODE3('c', 'h', 'a'):  // char
	  {
	    DagNode* a0 = d->getArgument(0);
	    Assert(succSymbol != 0, "succSymbol undefined");
	    if (succSymbol->isNat(a0))
	      {
		const mpz_class& n0 = succSymbol->getNat(a0);
		if (n0 <= 255)
		  {
		    char c = n0.get_si();
		    return rewriteToString(subject, context, Rope(c));
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
StringOpSymbol::rewriteToString(DagNode* subject, RewritingContext& context, const Rope& result)
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

Rope
StringOpSymbol::substring(const Rope& subject, Rope::size_type index, Rope::size_type length)
{
  Rope::size_type sLen = subject.length();
  //  if (index < 0)
  //    {
  //      if (length > 0)
  //	    length += index;
  //      index = 0;
  //    }
  if (length == 0 || index >= sLen)
    return Rope();
  if (length > sLen - index)
    length = sLen - index;
  return subject.substr(index, length);
}
  
int
StringOpSymbol::fwdFind(const Rope& subject, const Rope& pattern, Rope::size_type start)
{
  Rope::size_type sLen = subject.length();
  if (pattern.empty())
    return (start <= sLen) ? static_cast<int>(start) : NONE;
  //
  //	Testing start < sLen is important because otherwise 2nd test
  //	could succeed by wrap around.
  //
  if (start < sLen && start + pattern.length() <= sLen)
    {
      Rope::const_iterator b(subject.begin());
      Rope::const_iterator e(subject.end());
      Rope::const_iterator p(search(b + start, e, pattern.begin(), pattern.end()));
      if (p != e)
	return p - b;
    }
  return NONE;
}

int
StringOpSymbol::revFind(const Rope& subject, const Rope& pattern, Rope::size_type start)
{
  Rope::size_type sLen = subject.length();
  if (pattern.empty())
    return (start <= sLen) ? start : sLen;
  Rope::size_type pLen = pattern.length();
  if (pLen <= sLen)
    {
      Rope::size_type reflect = sLen - pLen;  // pattern can't start after this since we need pLen characters.
      if (start > reflect)
	start = reflect;
      //
      //	We are going to search the subject from beginning to beginning + start + pLen - 1

      Rope::const_iterator b(subject.begin());
      Rope::const_iterator e(b + (start + pLen));
      Rope::const_iterator p = find_end(b, e, pattern.begin(), pattern.end());
      if (p != e)
	return p - b;
    }
  return NONE;
}

bool
StringOpSymbol::ropeToNumber(const Rope& subject,
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
  if (!isalnum(c))
    return false;
  for (i++; i < len; i++)
    {
      char c = subject[i];
      if (!isalnum(c))
	{
	  if (c == '/')
	    {
	      bool nonzeroDenom = false;
	      for (int j = i + 1; j < len; ++j)
		{
		  char c = subject[j];
		  if (!isalnum(c))
		    return false;
		  if (c != '0')
		    nonzeroDenom = true;
		}
	      if (!nonzeroDenom)
		return false;
	      //
	      //	We have detected a fraction form.
	      //
	      char* numStr = subject.substr(0, i).makeZeroTerminatedString();
	      char* denomStr = subject.substr(i + 1, len - (i + 1)).makeZeroTerminatedString();
	      bool result = (mpz_set_str(denominator.get_mpz_t(), denomStr, base) == 0 &&
			     mpz_set_str(numerator.get_mpz_t(), numStr, base) == 0);
	      delete [] numStr;
	      delete [] denomStr;
	      return result;
	    }
	  else
	    return false;
	}
    }
  //
  //	We have a regular integer form.
  //
  denominator = 0;
  char* numStr = subject.makeZeroTerminatedString();
  bool result = (mpz_set_str(numerator.get_mpz_t(), numStr, base) == 0);
  delete [] numStr;
  return result;
}

Rope
StringOpSymbol::upperCase(const Rope& subject)
{
  Rope::const_iterator b(subject.begin());
  Rope::const_iterator e(subject.end());
  for (Rope::const_iterator i(b); i != e; ++i)
    {
      char c = *i;
      if (islower(c))
	{
	  //
	  //	At least one lower-case character exists - need to convert.
	  //
	  Rope result(subject.substr(0, i - b));  // characters before *i
	  result += toupper(c);
	  for (++i; i != e; ++i)
	    result += toupper(*i);
	  return result;
	}
    }
  return subject;  // no lower-case characters to convert
}

Rope
StringOpSymbol::lowerCase(const Rope& subject)
{
  Rope::const_iterator b(subject.begin());
  Rope::const_iterator e(subject.end());
  for (Rope::const_iterator i(b); i != e; ++i)
    {
      char c = *i;
      if (isupper(c))
	{
	  //
	  //	At least one upper-case character exists - need to convert.
	  //
	  Rope result(subject.substr(0, i - b));  // characters before *i
	  result += tolower(c);
	  for (++i; i != e; ++i)
	    result += tolower(*i);
	  return result;
	}
    }
  return subject;  // no lower-case characters to convert
}
