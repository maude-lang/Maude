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
//	Code to move renamings down from meta-level to object level.
//

void
MetaLevel::convertToTokens(const Vector<int>& ids, Vector<Token>& tokens)
{
  tokens.resize(ids.size());
  Vector<Token>::iterator j = tokens.begin();
  FOR_EACH_CONST(i, Vector<int>, ids)
    {
      j->tokenize(*i, FileTable::META_LEVEL_CREATED);
      ++j;
    }
}

bool
MetaLevel::downModuleExpression(DagNode* metaExpr, MetaModule* enclosingModule, ImportModule*& m)
{
  if (ModuleExpression* modExpr = downModuleExpression(metaExpr))
    {
      Interpreter* owner = enclosingModule->getOwner();
      ImportModule* module = owner->makeModule(modExpr, enclosingModule);
      modExpr->deepSelfDestruct();
      if (module != 0)
	{
	  m = module;
	  return true;
	}
    }
  return false;
}

bool
MetaLevel::downRenamings(DagNode* metaRenamings, Renaming* renaming)
{
  Symbol* mr = metaRenamings->symbol();
  if (mr == renamingSetSymbol)
    {
      for (DagArgumentIterator i(metaRenamings); i.valid(); i.next())
	{
	  if (!downRenaming(i.argument(), renaming))
	    return false;
	}
    }
  else
    return downRenaming(metaRenamings, renaming);
  return true;
}

bool
MetaLevel::downRenaming(DagNode* metaRenaming, Renaming* renaming)
{
  Symbol* mr = metaRenaming->symbol();
  if (mr == sortRenamingSymbol)
    {
      FreeDagNode* f = safeCast(FreeDagNode*, metaRenaming);
      int from;
      int to;
      if (downQid(f->getArgument(0), from) && downQid(f->getArgument(1), to))
	{
	  Token fromTok;
	  Token toTok;
	  fromTok.tokenize(from, FileTable::META_LEVEL_CREATED);
	  toTok.tokenize(to, FileTable::META_LEVEL_CREATED);
	  renaming->addSortMapping(fromTok, toTok);
	  return true;
	}
    }
  else if (mr == opRenamingSymbol)
    {
      FreeDagNode* f = safeCast(FreeDagNode*, metaRenaming);
      int from;
      int to;
      if (downQid(f->getArgument(0), from) && downQid(f->getArgument(1), to))
	{
	  Vector<Token> bubble(1);
	  bubble[0].tokenize(from, FileTable::META_LEVEL_CREATED);
	  renaming->addOpMapping(bubble);
	  bubble[0].tokenize(to, FileTable::META_LEVEL_CREATED);
	  renaming->addOpTarget(bubble);
	  return downRenamingAttributes(f->getArgument(2), renaming);
	}
    }
  else if (mr == opRenamingSymbol2)
    {
      FreeDagNode* f = safeCast(FreeDagNode*, metaRenaming);
      int from;
      int to;
      if (downQid(f->getArgument(0), from) && downQid(f->getArgument(3), to))
	{
	  Vector<Token> bubble(1);
	  bubble[0].tokenize(from, FileTable::META_LEVEL_CREATED);
	  renaming->addOpMapping(bubble);
	  bubble[0].tokenize(to, FileTable::META_LEVEL_CREATED);
	  renaming->addOpTarget(bubble);
	  return downRenamingTypes(f->getArgument(1), renaming) &&
	    downRenamingType(f->getArgument(2), renaming) &&
	    downRenamingAttributes(f->getArgument(4), renaming);
	}
    }
  else if (mr == labelRenamingSymbol)
    {
      FreeDagNode* f = safeCast(FreeDagNode*, metaRenaming);
      int from;
      int to;
      if (downQid(f->getArgument(0), from) && downQid(f->getArgument(1), to))
	{
	  Token fromTok;
	  Token toTok;
	  fromTok.tokenize(from, FileTable::META_LEVEL_CREATED);
	  toTok.tokenize(to, FileTable::META_LEVEL_CREATED);
	  renaming->addLabelMapping(fromTok, toTok);
	  return true;
	}
    }
  else if (mr == stratRenamingSymbol)
    {
      FreeDagNode* f = safeCast(FreeDagNode*, metaRenaming);
      Token from;
      Token to;
      if (downToken(f->getArgument(0), from) && downToken(f->getArgument(1), to))
	{
	  renaming->addStratMapping(from);
	  renaming->addStratTarget(to);
	  return true;
	}
    }
  else if (mr == stratRenamingSymbol2)
    {
      FreeDagNode* f = safeCast(FreeDagNode*, metaRenaming);
      Token from;
      Token to;
      if (downToken(f->getArgument(0), from) && downToken(f->getArgument(3), to))
	{
	  renaming->addStratMapping(from);
	  renaming->addStratTarget(to);
	  return downRenamingTypes(f->getArgument(1), renaming) &&
	    downRenamingType(f->getArgument(2), renaming);
	}
    }
  return false;
}

bool
MetaLevel::downRenamingTypes(DagNode* metaTypes, Renaming* renaming)
{
  Symbol* mr = metaTypes->symbol();
  if (mr == qidListSymbol)
    {
      for (DagArgumentIterator i(metaTypes); i.valid(); i.next())
	{
	  if (!downRenamingType(i.argument(), renaming))
	    return false;
	}
    }
  else if (mr != nilQidListSymbol)
    return downRenamingType(metaTypes, renaming);
  return true;
}

bool
MetaLevel::downRenamingType(DagNode* metaType, Renaming* renaming)
{
  int id;
  if (downQid(metaType, id))
    {
      switch (Token::auxProperty(id))
	{
	case Token::AUX_SORT:
	case Token::AUX_STRUCTURED_SORT:
	  {
	    Vector<Token> type(1);
	    type[0].tokenize(id, FileTable::META_LEVEL_CREATED);
	    renaming->addType(false, type);
	    return true;
	  }
	case Token::AUX_KIND:
	  {
	    Vector<int> sortNames;
	    Token::splitKind(id, sortNames);
	    Vector<Token> type;
	    convertToTokens(sortNames, type);
	    renaming->addType(true, type);
	    return true;
	  }
	default:
	  {
	    IssueAdvisory("expected sort or kind, not " << QUOTE(Token::name(id)) <<
			  " in operator renaming.");
	    break;
	  }
	}
    }
  return false;
}

bool
MetaLevel::downRenamingAttributes(DagNode* metaRenamingAttributes, Renaming* renaming)
{
  Symbol* mr = metaRenamingAttributes->symbol();
  if (mr == attrSetSymbol)
    {
      for (DagArgumentIterator i(metaRenamingAttributes); i.valid(); i.next())
	{
	  if (!downRenamingAttribute(i.argument(), renaming))
	    return false;
	}
    }
  else if (mr != emptyAttrSetSymbol)
    return downRenamingAttribute(metaRenamingAttributes, renaming);
  return true;
}

bool
MetaLevel::downRenamingAttribute(DagNode* metaRenamingAttribute, Renaming* renaming)
{
  Symbol* mr = metaRenamingAttribute->symbol();
  if (mr == precSymbol)
    {
      int prec;
      if (succSymbol->getSignedInt(safeCast(FreeDagNode*, metaRenamingAttribute)->getArgument(0), prec))
	{
	  Token t;
	  t.tokenize(Token::int64ToCode(prec), FileTable::META_LEVEL_CREATED);
	  renaming->setPrec(t);
	  return true;
	}
    }
  else if (mr == gatherSymbol)
    {
      Vector<int> gather;
      if (downQidList(safeCast(FreeDagNode*, metaRenamingAttribute)->getArgument(0), gather))
	{
	  Vector<Token> gatherToks;
	  convertToTokens(gather, gatherToks);
	  renaming->setGather(gatherToks);
	  return true;
	}
    }
  else if (mr == formatSymbol)
    {
      Vector<int> format;
      if (downQidList(safeCast(FreeDagNode*, metaRenamingAttribute)->getArgument(0), format))
	{
	  Vector<Token> formatToks;
	  convertToTokens(format, formatToks);
	  renaming->setFormat(formatToks);
	  return true;
	}
    }
  return false;
}
