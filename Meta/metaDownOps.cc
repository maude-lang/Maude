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
//	Code to move operator declarations down from meta-level to object level.
//

bool
MetaLevel::downOpDecls(DagNode* metaOpDecls, MetaModule* m)
{
  Symbol* mo = metaOpDecls->symbol();
  if (mo == opDeclSetSymbol)
    {
      for (DagArgumentIterator i(metaOpDecls); i.valid(); i.next())
	{
	  if (!downOpDecl(i.argument(), m))
	    return false;
	}
    }
  else if (mo != emptyOpDeclSetSymbol)
    return downOpDecl(metaOpDecls, m);
  return true;
}

bool
MetaLevel::downOpDecl(DagNode* metaOpDecl, MetaModule* m)
{
  if (metaOpDecl->symbol() == opDeclSymbol)
    {
      int name;
      static Vector<Sort*> domainAndRange;
      Sort* range;
      AttributeInfo ai;
      FreeDagNode* f = safeCast(FreeDagNode*, metaOpDecl);
      if (downQid(f->getArgument(0), name) && downAttrSet(f->getArgument(3), ai))
	{
	  Token prefixName;
	  prefixName.tokenize(name, FileTable::META_LEVEL_CREATED);
	  if (ai.symbolType.isPolymorph())
	    {
	      if (ai.symbolType.getBasicType() == SymbolType::EQUALITY_SYMBOL)
		{
		  domainAndRange.resize(3);
		  if (!downType(f->getArgument(2), m, domainAndRange[2]))
		    goto fail;
		  domainAndRange[0] = 0;
		}
	      else
		{
		  domainAndRange.resize(4);
		  DagNode* metaTypeList = f->getArgument(1);
		  if (metaTypeList->symbol() != qidListSymbol)
		    goto fail;
		  DagArgumentIterator i(metaTypeList);
		  if (i.valid() &&
		      downType(i.argument(), m, domainAndRange[0]) &&
		      (i.next(), i.valid()) &&
		      (i.next(), i.valid()) &&
		      !(i.next(), i.valid()))
		    {
		      domainAndRange[2] = 0;
		      domainAndRange[3] = 0;
		    }
		  else
		    goto fail;
		}
	      domainAndRange[1] = 0;
	      int polymorphIndex = m->addPolymorph(prefixName,
						   domainAndRange,
						   ai.symbolType,
						   ai.strategy,
						   ai.prec,
						   ai.gather,
						   ai.format);
	      m->addComplexSymbol(POLYMORPH, polymorphIndex, ai.fixUpInfo);
	    }
	  else
	    {
	      if (downTypeList(f->getArgument(1), m, domainAndRange) &&
		  downType(f->getArgument(2), m, range))
		{
		  domainAndRange.append(range);
		  bool originator;
		  Symbol* symbol = m->addOpDeclaration(prefixName,
						       domainAndRange,
						       ai.symbolType,
						       ai.strategy,
						       ai.frozen,
						       ai.prec,
						       ai.gather,
						       ai.format,
						       originator);
		  if (ai.identity != 0)
		    {
		      m->addComplexSymbol(IDENTITY_SYMBOL,
					  symbol->getIndexWithinModule(),
					  ai.identity);
		    }
		  if (ai.symbolType.getBasicType() == SymbolType::BUBBLE)
		    {
		      int bubbleSpecIndex;
		      if (!downBubbleSpec(ai.fixUpInfo, m, symbol, bubbleSpecIndex))
			return false;
		      m->addComplexSymbol(BUBBLE, bubbleSpecIndex, ai.fixUpInfo);
		    }
		  else if (ai.fixUpInfo != 0 && ai.symbolType.hasAttachments())
		    {
		      m->addComplexSymbol(SPECIAL_SYMBOL,
					  symbol->getIndexWithinModule(),
					  ai.fixUpInfo,
					  domainAndRange);
		    }
		}
	      else
		goto fail;
	    }
	  return true;
	}
    }
 fail:
  IssueAdvisory("bad operator declaration " << QUOTE(metaOpDecl) <<
		" in meta-module " << QUOTE(m) << '.');
  return false;
}

bool
MetaLevel::downBubbleSpec(DagNode* metaBubbleSpec,
			  MetaModule* m,
			  Symbol* topSymbol,
			  int& bubbleSpecIndex)
{
  Symbol* mb = metaBubbleSpec->symbol();
  if (mb != hookListSymbol)
    return false;
  DagArgumentIterator i(metaBubbleSpec);
  if (!(i.valid()))
    return false;
  DagNode* metaIdHook = i.argument();
  if (metaIdHook->symbol() != idHookSymbol)
    return false;
  FreeDagNode* f = safeCast(FreeDagNode*, metaIdHook);
  Vector<int> args;
  if (!downQidList(f->getArgument(1), args))
    return false;
  int min = 1;
  int max = -1;
  int leftParen = NONE;
  int rightParen = NONE;
  Vector<int> excludedTokens;
  int nrArgs = args.length();
  if (nrArgs > 0)
    {
      min = atoi(Token::name(args[0]));
      if (nrArgs > 1)
	{
	  max = atoi(Token::name(args[1]));
	  if (nrArgs > 3)
	    {
	      leftParen = args[2];
	      const char* t = Token::name(leftParen);
	      if (t[0] == '`')
		leftParen = Token::encode(t + 1);
	      rightParen = args[3];
	      t = Token::name(rightParen);
	      if (t[0] == '`')
		rightParen = Token::encode(t + 1);
	    }
	}
    }
  for (i.next(); i.valid(); i.next())
    {
      FreeDagNode* f = safeCast(FreeDagNode*, i.argument());
      if (f->symbol() == idHookSymbol)
	{
	  Vector<int> tokens;
	  int id;
	  if (!downQid(f->getArgument(0), id) ||
	      !downQidList(f->getArgument(1), tokens))
	    return false;
	  if (id == Token::encode("Exclude"))
	    {
	      excludedTokens = tokens;
	      int nrTokens = excludedTokens.length();
	      for (int j = 0; j < nrTokens; j++)
		{
		  const char* t = Token::name(excludedTokens[j]);
		  if (t[0] == '`')
		    excludedTokens[j] = Token::encode(t + 1);
		}
	    }
	}
    }
  bubbleSpecIndex = m->addBubbleSpec(topSymbol, min, max, leftParen, rightParen, excludedTokens);
  return true;
}

bool
MetaLevel::downAttrSet(DagNode* metaAttrSet, AttributeInfo& ai)
{
  Symbol* ma = metaAttrSet->symbol();
  if (ma == attrSetSymbol)
    {
      for (DagArgumentIterator i(metaAttrSet); i.valid(); i.next())
	{
	  if (!downAttr(i.argument(), ai))
	    return false;
	}
      }
  else if (ma != emptyAttrSetSymbol)
    return downAttr(metaAttrSet, ai);
  return true;
}

bool
MetaLevel::downAttr(DagNode* metaAttr, AttributeInfo& ai)
{
  Symbol* ma = metaAttr->symbol();
  if (ma == assocSymbol)
    ai.symbolType.setFlags(SymbolType::ASSOC);
  else if (ma == commSymbol)
    ai.symbolType.setFlags(SymbolType::COMM);
  else if (ma == idemSymbol)
    ai.symbolType.setFlags(SymbolType::IDEM);
  else if (ma == iterSymbol)
    ai.symbolType.setFlags(SymbolType::ITER);
  else if (ma == stratSymbol)
    {
      if (!downNatList(safeCast(FreeDagNode*, metaAttr)->getArgument(0), ai.strategy))
	return false;
    }
  else if (ma == memoSymbol)
    ai.symbolType.setFlags(SymbolType::MEMO);
  else if (ma == precSymbol)
    {
      if (!(succSymbol->getSignedInt(safeCast(FreeDagNode*, metaAttr)->getArgument(0), ai.prec)))
	return false;
    }
  else if (ma == gatherSymbol)
    {
      if (!downQidList(safeCast(FreeDagNode*, metaAttr)->getArgument(0), ai.gather))
	return false;
      int gatherLength = ai.gather.length();
      for (int i = 0; i < gatherLength; i++)
	{
	  const char* string = Token::name(ai.gather[i]);
	  if (string[0] != '\0' && string[1] == '\0')
	    {
	      switch(string[0])
		{
		case '&':
		  ai.gather[i] = MixfixModule::GATHER_AMP;
		  continue;
		case 'E':
		  ai.gather[i] = MixfixModule::GATHER_E;
		  continue;
		case 'e':
		  ai.gather[i] = MixfixModule::GATHER_e;
		  continue;
		}
	    }
	  IssueAdvisory("bad value " << QUOTE(string) <<
			" in gather attribute at metalevel.");
	  return false;
	}
      ai.symbolType.setFlags(SymbolType::GATHER);
    }
  else if (ma == formatSymbol)
    {
      if (!downQidList(static_cast<FreeDagNode*>(metaAttr)->getArgument(0), ai.format))
	return false;
      int formatLength = ai.format.length();
      for (int i = 0; i < formatLength; i++)
	{
	  const char* str = Token::name(ai.format[i]);
	  if (!(PreModule::checkFormatString(str)))
	    {
	      IssueAdvisory("bad value " << QUOTE(str) <<
			    " in format attribute at metalevel.");
	      return false;
	    }
	}
      ai.symbolType.setFlags(SymbolType::FORMAT);
    }
  else if (ma == ctorSymbol)
    ai.symbolType.setFlags(SymbolType::CTOR);
  else if (ma == configSymbol)
    ai.symbolType.setFlags(SymbolType::CONFIG);
  else if (ma == objectSymbol)
    ai.symbolType.setFlags(SymbolType::OBJECT);
  else if (ma == msgSymbol)
    ai.symbolType.setFlags(SymbolType::MESSAGE);
  else if (ma == frozenSymbol)
    {
      Vector<int> frozenList;
      if (!downNatList(safeCast(FreeDagNode*, metaAttr)->getArgument(0), frozenList))
	return false;
      ai.symbolType.setFlags(SymbolType::FROZEN);
      int nrFrozenArgs = frozenList.length();
      for (int i = 0; i < nrFrozenArgs; i++)
	ai.frozen.insert(frozenList[i] - 1);  // FIX: NEED TO VALIDATE
    }
  else if (ma == specialSymbol)
    {
      ai.fixUpInfo = static_cast<FreeDagNode*>(metaAttr)->getArgument(0);
      checkHookList(ai.fixUpInfo, ai.symbolType);
    }
  else
    {
      if (ma == leftIdSymbol)
	ai.symbolType.setFlags(SymbolType::LEFT_ID);
      else if (ma == rightIdSymbol)
	ai.symbolType.setFlags(SymbolType::RIGHT_ID);
      else if (ma == idSymbol)
	ai.symbolType.setFlags(SymbolType::LEFT_ID | SymbolType::RIGHT_ID);
      else
	return false;
      ai.identity = static_cast<FreeDagNode*>(metaAttr)->getArgument(0);
    }
  return true;
}

void
MetaLevel::checkHookList(DagNode* metaHookList, SymbolType& symbolType)
{
  if (metaHookList->symbol() == hookListSymbol)
    {
      for (DagArgumentIterator i(metaHookList); i.valid(); i.next())
	checkHook(i.argument(), symbolType);
    }
  else
    checkHook(metaHookList, symbolType);
}

void
MetaLevel::checkHook(DagNode* metaIdHook, SymbolType& symbolType)
{
  Symbol* mi = metaIdHook->symbol();
  if (mi == idHookSymbol)
    {
      int id;
      if (downQid(static_cast<FreeDagNode*>(metaIdHook)->getArgument(0), id))
	{
	  int t = SymbolType::specialNameToBasicType(Token::name(id));
	  if (t != 0)
	    symbolType.setBasicType(t);
	}
    }
}
