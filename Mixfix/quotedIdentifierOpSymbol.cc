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

//      free theory class definitions
#include "freeDagNode.hh"

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
  quotedIdentifierSymbol = 0;
  stringSymbol = 0;
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
  BIND_SYMBOL(purpose, symbol, quotedIdentifierSymbol, QuotedIdentifierSymbol*);
  BIND_SYMBOL(purpose, symbol, stringSymbol, StringSymbol*);
  return FreeSymbol::attachSymbol(purpose, symbol);
}

void
QuotedIdentifierOpSymbol::copyAttachments(Symbol* original, SymbolMap* map)
{
  QuotedIdentifierOpSymbol* orig = safeCast(QuotedIdentifierOpSymbol*, original);
  op = orig->op;
  COPY_SYMBOL(orig, quotedIdentifierSymbol, map, QuotedIdentifierSymbol*);
  COPY_SYMBOL(orig, stringSymbol, map, StringSymbol*);
  FreeSymbol::copyAttachments(original, map);
}

bool
QuotedIdentifierOpSymbol::eqRewrite(DagNode* subject, RewritingContext& context)
{
  Assert(this == subject->symbol(), "bad symbol");
  FreeDagNode* d = static_cast<FreeDagNode*>(subject);
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
