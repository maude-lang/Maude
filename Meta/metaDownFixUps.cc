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
//	Code to fixup operators that have previously be pulled down.
//

bool
MetaLevel::downFixUps(MetaModule* m)
{
  int type;
  int index;
  DagNode* fixUpInfo;
  Vector<Sort*> domainAndRange;
  while (m->removeComplexSymbol(type, index, fixUpInfo, domainAndRange))
    {
      switch (type)
	{
	case IDENTITY_SYMBOL:
	  {
	    if (!fixUpIdentitySymbol(fixUpInfo,
				     m,
				     safeCast(BinarySymbol*, m->getSymbols()[index])))
	      return false;
	    break;
	  }
	case SPECIAL_SYMBOL:
	  {
	    if (!fixUpSpecialSymbol(fixUpInfo, m, m->getSymbols()[index], domainAndRange))
	      return false;
	    break;
	  }
	case POLYMORPH:
	  {
	    if (!fixUpPolymorph(fixUpInfo, m, index))
	      return false;
	    break;
	  }
	case BUBBLE:
	  {
	    if (!fixUpBubble(fixUpInfo, m, index))
	      return false;
	    break;
	  }
	}
    }
  return true;
}

bool
MetaLevel::fixUpIdentitySymbol(DagNode* metaIdentity, MetaModule* m, BinarySymbol* s)
{
  if (Term* id = downTerm(metaIdentity, m))
    {
      bool result = false;
      SymbolType symbolType = m->getSymbolType(s);
      if ((!(symbolType.hasFlag(SymbolType::LEFT_ID)) ||
	   id->symbol()->rangeComponent() == s->domainComponent(0)) &&
	  (!(symbolType.hasFlag(SymbolType::RIGHT_ID)) ||
	   id->symbol()->rangeComponent() == s->domainComponent(1)))
	{
	  Term* oldId = s->getIdentity();
	  if (oldId == 0)
	    {
	      s->setIdentity(id);
	      return true;
	    }
	  else
	    result = id->equal(oldId);
	}
      id->deepSelfDestruct();
      return result;
    }
  return false;
}

bool
MetaLevel::fixUpSpecialSymbol(DagNode* metaHookList,
			      MetaModule* m,
			      Symbol* symbol,
			      const Vector<Sort*>& domainAndRange)
{
  if (metaHookList->symbol() == hookListSymbol)
    {
      for (DagArgumentIterator i(metaHookList); i.valid(); i.next())
	{
	  if (!downHook(i.argument(), m, symbol, domainAndRange))
	    return false;
	}
      return true;
    }
  return downHook(metaHookList, m, symbol, domainAndRange);
}

bool
MetaLevel::downHook(DagNode* metaHook,
		    MetaModule* m,
		    Symbol* symbol,
		    const Vector<Sort*>& domainAndRange)
{
  Symbol* mh = metaHook->symbol();
  if (mh == idHookSymbol)
    {
      int purpose;
      FreeDagNode* f = safeCast(FreeDagNode*, metaHook);
      if (downQid(f->getArgument(0), purpose))
	{
	  Vector<int> items;
	  if (downQidList(f->getArgument(1), items))
	    {
	      int nrItems = items.length();
	      Vector<const char*> strings(nrItems);
	      for (int i = 0; i < nrItems; i++)
		strings[i] = Token::name(items[i]);
	      if (symbol->attachData(domainAndRange, Token::name(purpose), strings))
		return true;
	    }
	}
    }
  else if (mh == termHookSymbol)
    {
      int purpose;
      if (downQid(static_cast<FreeDagNode*>(metaHook)->getArgument(0), purpose))
	{
	  if (Term* ht = downTerm(static_cast<FreeDagNode*>(metaHook)->getArgument(1), m))
	    {
	      if (symbol->attachTerm(Token::name(purpose), ht))
		return true;
	    }
	}
    }
  else
    {
      int purpose;
      Symbol* op;
      if (downOpHook(metaHook, m, purpose, op) &&
	  symbol->attachSymbol(Token::name(purpose), op))
	return true;
    }
  return false;
}

bool
MetaLevel::downOpHook(DagNode* metaOpHook, MetaModule* m, int& purpose, Symbol*& op)
{
  Symbol* mo = metaOpHook->symbol();
  if (mo == opHookSymbol)
    {
      FreeDagNode* f = static_cast<FreeDagNode*>(metaOpHook);
      int id;
      Vector<Sort*> domain;
      Sort* range;
      if (downQid(f->getArgument(0), purpose) &&
	  downQid(f->getArgument(1), id) &&
	  downSimpleSortList(f->getArgument(2), m, domain) &&
	  downSimpleSort(f->getArgument(3), m, range))
	{
	  int domainLength = domain.length();
	  Vector<ConnectedComponent*> domainC(domainLength);
	  for (int i = 0; i < domainLength; i++)
	    domainC[i] = domain[i]->component();
	  op = m->findSymbol(id, domainC, range->component());
	  if (op != 0)
	    return op;
	  IssueAdvisory("couldn't find an operator " << QUOTE(Token::name(id)) <<
			" which was mentioned in an op-hook in meta-module " <<
			QUOTE(m) << '.');
	}
    }
  return false;
}

bool
MetaLevel::fixUpBubble(DagNode* metaHookList, MetaModule* m, int bubbleSpecIndex)
{
  QuotedIdentifierSymbol* qidSymbol = 0;
  Symbol* nilQidListSymbol = 0;
  Symbol* qidListSymbol = 0;
  if (metaHookList->symbol() != hookListSymbol)
    return false;
  for (DagArgumentIterator i(metaHookList); i.valid(); i.next())
    {
      DagNode* metaHook = i.argument();
      if (metaHook->symbol() == opHookSymbol)
	{
	  int purpose;
	  Symbol* s;
	  if (!downOpHook(metaHook, m, purpose, s))
	    return false;
	  const char* string = Token::name(purpose);
	  if (strcmp(string, "qidSymbol") == 0)
	    qidSymbol = static_cast<QuotedIdentifierSymbol*>(s);
	  else if (strcmp(string, "nilQidListSymbol") == 0)
	    nilQidListSymbol = s;
	  else if (strcmp(string, "qidListSymbol") == 0)
	    qidListSymbol = s;
	  else
	    IssueAdvisory("bad bubble op-hook " << QUOTE(string) <<
			  " in meta-module " << QUOTE(m) << '.');
	}
    }
  m->fixUpBubbleSpec(bubbleSpecIndex, qidSymbol, nilQidListSymbol, qidListSymbol);
  return true;
}

bool
MetaLevel::fixUpPolymorph(DagNode* metaHookList, MetaModule* m, int polymorphIndex)
{
  // HACK no way to know which kind of polymorph we have
  if (metaHookList->symbol() != hookListSymbol)
    return false;
  static Vector<Term*> terms(2);
  terms[0] = 0;
  terms[1] = 0;
  for (DagArgumentIterator i(metaHookList); i.valid(); i.next())
    {
      DagNode* metaHook = i.argument();
      if (metaHook->symbol() == termHookSymbol)
	{
	  FreeDagNode* f = safeCast(FreeDagNode*, metaHook);
	  int purpose;
	  if (downQid(f->getArgument(0), purpose))
	    {
	      int index = NONE;
	      const char* string = Token::name(purpose);
	      if (strcmp(string, "trueTerm") == 0)
		index = 0;
	      else if (strcmp(string, "falseTerm") == 0)
		index = 1;
	      else if (strcmp(string, "equalTerm") == 0)
		index = 0;
	      else if (strcmp(string, "notEqualTerm") == 0)
		index = 1;
	      else
		return false;  // potential memory leak
	      if (terms[index] == 0)
		terms[index] = downTerm(f->getArgument(1), m);
	      else
		return false;  // potential memory leak
	    }
	}
    }
  if (terms[0] != 0 && terms[1] != 0)
    {
      m->fixUpPolymorph(polymorphIndex, terms);
      return true;
    }
  return false;
}
