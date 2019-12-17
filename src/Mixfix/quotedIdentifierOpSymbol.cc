/*

    This file is part of the Maude 3 interpreter.

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
//      Implementation for class QuotedIdentifierOpSymbol.
//

#include <sstream>

//      utility stuff
#include "macros.hh"
#include "vector.hh"
#include "stringTable.hh"

//      forward declarations
#include "interface.hh"
#include "core.hh"
#include "freeTheory.hh"
#include "AU_Theory.hh"
#include "NA_Theory.hh"
#include "builtIn.hh"
#include "mixfix.hh"

//      interface class definitions
#include "symbol.hh"
#include "dagNode.hh"

//      core class definitions
#include "dagArgumentIterator.hh"
#include "substitution.hh"
#include "rewritingContext.hh"
#include "symbolMap.hh"
#include "pointerMap.hh"

//      free theory class definitions
#include "freeDagNode.hh"

//      AU class definitions
#include "AU_Symbol.hh"
#include "AU_DagNode.hh"

//      built in class definitions
#include "stringSymbol.hh"
#include "stringDagNode.hh"
#include "bindingMacros.hh"

//      front end class definitions
#include "userLevelRewritingContext.hh"
#include "quotedIdentifierSymbol.hh"
#include "quotedIdentifierDagNode.hh"
#include "quotedIdentifierOpSymbol.hh"
#include "token.hh"

#ifdef MOS
int QuotedIdentifierOpSymbol::counter = 1;
#endif

QuotedIdentifierOpSymbol::QuotedIdentifierOpSymbol(int id, int nrArgs)
  : FreeSymbol(id, nrArgs)
{
  op = NONE;
#define MACRO(SymbolName, SymbolClass, NrArgs) \
  SymbolName = 0;
#include "quotedIdentifierOpSignature.cc"
#undef MACRO
}

bool
QuotedIdentifierOpSymbol::attachData(const Vector<Sort*>& opDeclaration,
				     const char* purpose,
				     const Vector<const char*>& data)
{
  BIND_OP(purpose, QuotedIdentifierOpSymbol, op, data);
  return  FreeSymbol::attachData(opDeclaration, purpose, data);
}

bool
QuotedIdentifierOpSymbol::attachSymbol(const char* purpose, Symbol* symbol)
{
  Assert(symbol != 0, "null symbol for " << purpose);
#define MACRO(SymbolName, SymbolClass, NrArgs) \
  BIND_SYMBOL(purpose, symbol, SymbolName, SymbolClass*)
#include "quotedIdentifierOpSignature.cc"
#undef MACRO
  return FreeSymbol::attachSymbol(purpose, symbol);
}

void
QuotedIdentifierOpSymbol::copyAttachments(Symbol* original, SymbolMap* map)
{
  QuotedIdentifierOpSymbol* orig = safeCast(QuotedIdentifierOpSymbol*, original);
  op = orig->op;
#define MACRO(SymbolName, SymbolClass, NrArgs) \
  COPY_SYMBOL(orig, SymbolName, map, SymbolClass*)
#include "quotedIdentifierOpSignature.cc"
#undef MACRO
  FreeSymbol::copyAttachments(original, map);
}

void
QuotedIdentifierOpSymbol::getDataAttachments(const Vector<Sort*>& opDeclaration,
					     Vector<const char*>& purposes,
					     Vector<Vector<const char*> >& data)
{
  int nrDataAttachments = purposes.length();
  purposes.resize(nrDataAttachments + 1);
  purposes[nrDataAttachments] = "QuotedIdentifierOpSymbol";
  data.resize(nrDataAttachments + 1);
  data[nrDataAttachments].resize(1);
  const char*& d = data[nrDataAttachments][0];
  switch (op)
    {
    CODE_CASE(d, 's', 't', "string")
    CODE_CASE(d, 'q', 'i', "qid")
    CODE_CASE(d, 'p', 'r', "print")
    CODE_CASE(d, 't', 'o', "tokenize")
#ifdef MOS
    CODE_CASE(d, 'm', 'o', "mo")
#endif
    default:
      CantHappen("bad qid op");
    }
  FreeSymbol::getDataAttachments(opDeclaration, purposes, data);
}

void
QuotedIdentifierOpSymbol::getSymbolAttachments(Vector<const char*>& purposes,
					       Vector<Symbol*>& symbols)
{
#define MACRO(SymbolName, SymbolClass, NrArgs) \
  APPEND_SYMBOL(purposes, symbols, SymbolName)
#include "quotedIdentifierOpSignature.cc"
#undef MACRO
  FreeSymbol::getSymbolAttachments(purposes, symbols);
}

bool
QuotedIdentifierOpSymbol::eqRewrite(DagNode* subject, RewritingContext& context)
{
  Assert(this == subject->symbol(), "bad symbol");
  FreeDagNode* d = safeCast(FreeDagNode*, subject);
  //
  //	Evaluate our arguments.
  //
  DagNode* a1 = d->getArgument(0);
  a1->reduce(context);
  switch (op)
    {
    case CODE('s', 't'):
      {
	if (a1->symbol() == quotedIdentifierSymbol)
	  {
	    QuotedIdentifierDagNode* d1 = static_cast<QuotedIdentifierDagNode*>(a1);
	    bool trace = RewritingContext::getTraceStatus();
	    if (trace)
	      {
		context.tracePreEqRewrite(subject, 0, RewritingContext::BUILTIN);
		if (context.traceAbort())
		  return false;
	      }
	    (void) new(subject) StringDagNode(stringSymbol, Token::name(d1->getIdIndex()));
	    context.incrementEqCount();
	    if (trace)
	      context.tracePostEqRewrite(subject);
	    return true;  
	  }
	break;
      }
    case CODE('q', 'i'):
      {
	if (a1->symbol() == stringSymbol)
	  {
	    int idIndex = Token::ropeToPrefixNameCode(static_cast<StringDagNode*>(a1)->getValue());
	    if (idIndex != NONE)
	      {
		bool trace = RewritingContext::getTraceStatus();
		if (trace)
		  {
		    context.tracePreEqRewrite(subject, 0, RewritingContext::BUILTIN);
		    if (context.traceAbort())
		      return false;
		  }
		(void) new(subject) QuotedIdentifierDagNode(quotedIdentifierSymbol, idIndex);
		context.incrementEqCount();
		if (trace)
		  context.tracePostEqRewrite(subject);
		return true;  
	      }
	  }
	break;
      }
    case CODE('p', 'r'):
      {
	Rope output;
	if (printQidList(a1, output))
	  {
	    bool trace = RewritingContext::getTraceStatus();
	    if (trace)
	      {
		context.tracePreEqRewrite(subject, 0, RewritingContext::BUILTIN);
		if (context.traceAbort())
		  return false;
	      }
	    (void) new(subject) StringDagNode(stringSymbol, output);
	    context.incrementEqCount();
	    if (trace)
	      context.tracePostEqRewrite(subject);
	    return true;
	  }
	break;
      }
    case CODE('t', 'o'):
      {
	if (a1->symbol() == stringSymbol)
	  {
	    extern const Vector<int>* tokenizeRope(const Rope& argumentRope);

	    const Rope& r = static_cast<StringDagNode*>(a1)->getValue();
	    const Vector<int>* ids = tokenizeRope(r);
	    Assert(ids != 0, "should never return YY_NULL");

	    int nrIds = ids->length();
	    if (nrIds == 0)
	      return context.builtInReplace(subject, nilQidListSymbol->makeDagNode());

	    PointerMap qidMap;
	    if (nrIds == 1)
	      return context.builtInReplace(subject, makeQid((*ids)[0], qidMap));

	    Vector<DagNode*> args(nrIds);
	    for (int i = 0; i < nrIds; i++)
	      args[i] = makeQid((*ids)[i], qidMap);
	    DagNode* result = qidListSymbol->makeDagNode(args);
	    return context.builtInReplace(subject, result);
	  }
	break;
      }
#ifdef MOS
    case CODE('m', 'o'):
      {
	if (a1->symbol() == quotedIdentifierSymbol)
	  {
	    QuotedIdentifierDagNode* d1 = static_cast<QuotedIdentifierDagNode*>(a1);
	    const char* s1 = Token::name(d1->getIdIndex());
	    if (strcmp(s1, "RESET") == 0)
	      counter = 0;

	    ostringstream ost(s1);
	    ost << counter;
	    ++counter;
	    int idIndex = Token::encode(ost.str().c_str());

	    bool trace = RewritingContext::getTraceStatus();
	    if (trace)
	      {
		context.tracePreEqRewrite(subject, 0, RewritingContext::BUILTIN);
		if (context.traceAbort())
		  return false;
	      }
	    (void) new(subject) QuotedIdentifierDagNode(quotedIdentifierSymbol, idIndex);
	    context.incrementEqCount();
	    if (trace)
	      context.tracePostEqRewrite(subject);
	    return true;
	  }
	break;
      }
#endif
    }
  return FreeSymbol::eqRewrite(subject, context);
}

DagNode*
QuotedIdentifierOpSymbol::makeQid(int id, PointerMap& qidMap)
{
  void* p = const_cast<void*>(static_cast<const void*>(Token::name(id)));
  DagNode *d = static_cast<DagNode*>(qidMap.getMap(p));
  if (d == 0)
    {
      d = new QuotedIdentifierDagNode(quotedIdentifierSymbol, Token::backQuoteSpecials(id));
      (void) qidMap.setMap(p, d);
    }
  return d;
}


bool
QuotedIdentifierOpSymbol::printQidList(DagNode* qidList, Rope& output)
{
  bool ansiActive = false;
  bool needSpace = false;

  Symbol* ql =  qidList->symbol();
  if (ql == qidListSymbol)
    {
      for (DagArgumentIterator i(qidList); i.valid(); i.next())
	{
	  if (!printQid(i.argument(), ansiActive, needSpace, output))
	    return false;
	}
    }
  else if (ql == nilQidListSymbol)
    ;
  else
    return printQid(qidList, ansiActive, needSpace, output);
  return true;
}

bool
QuotedIdentifierOpSymbol::printQid(DagNode* qid, bool& ansiActive, bool& needSpace, Rope& output)
{
  if (qid->symbol() != quotedIdentifierSymbol)
    return false;
  
  int id = static_cast<QuotedIdentifierDagNode*>(qid)->getIdIndex();
  const char* s = Token::name(id);
  switch (s[0])
    {
    case '`':
      {
	char c = s[1];
	if (Token::specialChar(c) && s[2] == '\0')
	{
	  //
	  //	( ) [ ] { } , case
	  //
	  output += (s + 1);
	  needSpace = (c != '(') && (c != '[') && (c != '{');
	  return true;
	}
	break;
      }
    case '\\':
      {
	char c = s[1];
	if (c != '\0' && s[2] == '\0')
	  {
	    //
	    //	\c case
	    //
	    switch (c)
	      {
		case 'n':
		  {
		    output += '\n';
		    needSpace = false;
		    return true;
		  }
		case 't':
		  {
		    output += '\t';
		    needSpace = false;
		    return true;
		  }
		case 's':
		  {
		    output += ' ';
		    needSpace = false;
		    return true;
		  }
		case '\\':
		  {
		    if (needSpace)
		      output += ' ';
		    output += '\\';
		    needSpace = true;
		    return true;
		  }
#define MACRO(m, t) \
case m: { output += Tty(Tty::t).ctrlSequence(); return true; }
#include "ansiEscapeSequences.cc"
#undef MACRO
		case 'o':
		  {
		    output += Tty(Tty::RESET).ctrlSequence();
		    return true;
		  }
	      default:
		;
	      }
	  }
	break;
      }
    default:
      ;
    }
  if (needSpace)
    output += " ";
  output += s;
  needSpace = true;
  return true;
}
