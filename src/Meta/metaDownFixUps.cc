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
//	Code to fixup operators that have previously be pulled down.
//

bool
MetaLevel::downFixUps(MetaModule* m)
{
  int type;
  int index;
  DagNode* identity;
  DagNode* fixUpInfo;
  Vector<Sort*> domainAndRange;
  while (m->removeComplexSymbol(type, index, identity, fixUpInfo, domainAndRange))
    {
      switch (type)
	{
	case REGULAR_SYMBOL:
	  {
	    Symbol* s = m->getSymbols()[index];
	    SymbolType st = m->getSymbolType(s);
	    if (identity != 0 && !handleIdentity(identity, m, safeCast(BinarySymbol*, s)))
	      return false;
	    if (fixUpInfo != 0 &&
		st.hasAttachments() &&
		!handleSpecial(fixUpInfo, m, s, domainAndRange))
	      return false;
	    break;
	  }
	case POLYMORPH:
	  {
	    SymbolType st = m->getPolymorphType(index);
	    if (identity != 0 && !handleIdentity(identity, m, index, domainAndRange))
	      return false;
	    if (fixUpInfo != 0 &&
		st.hasAttachments() &&
		!handleSpecial(fixUpInfo, m, index))
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
MetaLevel::handleIdentity(DagNode* metaIdentity,
			  MetaModule* m,
			  int index,
			  const Vector<Sort*>& domainAndRange)
{
  if (Term* id = downTerm(metaIdentity, m))
    {
      bool result = false;
      const ConnectedComponent* c = id->symbol()->rangeComponent();
      SymbolType symbolType = m->getPolymorphType(index);

      if ((!symbolType.hasFlag(SymbolType::LEFT_ID) ||
	   (domainAndRange[0] != 0 && domainAndRange[0]->component() == c)) &&
	  (!symbolType.hasFlag(SymbolType::RIGHT_ID) ||
	   (domainAndRange[1] != 0 && domainAndRange[1]->component() == c)))
	{
	  Term* oldId = m->getPolymorphIdentity(index);
	  if (oldId == 0)
	    {
	      m->addIdentityToPolymorph(index, id);
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
MetaLevel::handleIdentity(DagNode* metaIdentity, MetaModule* m, BinarySymbol* s)
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
MetaLevel::handleSpecial(DagNode* metaHookList, MetaModule* m, int index)
{
  if (metaHookList->symbol() == hookListSymbol)
    {
      for (DagArgumentIterator i(metaHookList); i.valid(); i.next())
	{
	  if (!downHook(i.argument(), m, index))
	    return false;
	}
      return true;
    }
  return downHook(metaHookList, m, index);
}

bool
MetaLevel::downHook(DagNode* metaHook, MetaModule* m, int index)
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
	      m->addIdHookToPolymorph(index, purpose, items);
	      return true;
	    }
	}
    }
  else if (mh == termHookSymbol)
    {
      int purpose;
      FreeDagNode* f = safeCast(FreeDagNode*, metaHook);
      if (downQid(f->getArgument(0), purpose))
	{
	  if (Term* ht = downTerm(f->getArgument(1), m))
	    {
	      m->addTermHookToPolymorph(index, purpose, ht);
		return true;
	    }
	}
    }
  else
    {
      int purpose;
      Symbol* op;
      if (downOpHook(metaHook, m, purpose, op))
	{
	  m->addOpHookToPolymorph(index, purpose, op);
	  return true;
	}
    }
  return false;
}

bool
MetaLevel::handleSpecial(DagNode* metaHookList,
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
      FreeDagNode* f = safeCast(FreeDagNode*, metaHook);
      if (downQid(f->getArgument(0), purpose))
	{
	  if (Term* ht = downTerm(f->getArgument(1), m))
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
      FreeDagNode* f = safeCast(FreeDagNode*, metaOpHook);
      int id;
      Vector<Sort*> domain;
      Sort* range;
      if (downQid(f->getArgument(0), purpose) &&
	  downOpName(f->getArgument(1), id) &&
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
  Symbol* qidSymbol = 0;
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
	  const char* str = Token::name(purpose);
	  if (strcmp(str, "qidSymbol") == 0)
	    qidSymbol = s;
	  else if (strcmp(str, "nilQidListSymbol") == 0)
	    nilQidListSymbol = s;
	  else if (strcmp(str, "qidListSymbol") == 0)
	    qidListSymbol = s;
	  else
	    IssueAdvisory("bad bubble op-hook " << QUOTE(str) <<
			  " in meta-module " << QUOTE(m) << '.');
	}
    }
  m->fixUpBubbleSpec(bubbleSpecIndex, qidSymbol, nilQidListSymbol, qidListSymbol);
  return true;
}
