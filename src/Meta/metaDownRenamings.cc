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
MetaLevel::downModuleExpression(DagNode* metaExpr, ImportModule* enclosingModule, ImportModule*& m)
{
  Interpreter* owner = safeCast(MetaModule*, enclosingModule)->getOwner();  // HACK - probably all modules should have owners

  int moduleName;
  if (downQid(metaExpr, moduleName))
    {
      if (PreModule* pm = owner->getModule(moduleName))
	{
	  m = pm->getFlatSignature();
	  if (!(m->isBad()))
	    return true;
	  IssueAdvisory(LineNumber(FileTable::META_LEVEL_CREATED) << ": unable to use module " <<
			QUOTE(pm) << " due to unpatchable errors.");
	}
      IssueAdvisory(LineNumber(FileTable::META_LEVEL_CREATED) << ": module " <<
		    QUOTE(Token::name(moduleName)) << " does not exist.");
    }
  else
    {
      Symbol* me = metaExpr->symbol();
      if (me == sumSymbol)
	{
	  Vector<ImportModule*> fms;
	  for (DagArgumentIterator i(metaExpr); i.valid(); i.next())
	    {
	      if (!downModuleExpression(i.argument(), enclosingModule, m) || m->getNrParameters() > 0)
		return false;
	      fms.append(m);
	    }
	  m = owner->makeSummation(fms);
	  if (m->isBad())
	    return false;
	  return true;
	}
      else if (me == renamingSymbol)
	{
	  FreeDagNode* f = safeCast(FreeDagNode*, metaExpr);
	  Renaming renaming;  // object level renaming is ephemeral
	  if (downRenamings(f->getArgument(1), &renaming) &&
	      downModuleExpression(f->getArgument(0), enclosingModule, m))
	    {
	      m = owner->makeRenamedCopy(m, &renaming);
	      if (m->isBad())
		return false;
	      return true;
	    }
	}
      else if (me == instantiationSymbol)
	{
	  FreeDagNode* f = safeCast(FreeDagNode*, metaExpr);
	  Vector<int> names;
	  if (downInstantiationArguments(f->getArgument(1), names) &&
	      downModuleExpression(f->getArgument(0), enclosingModule, m))
	    {
	      int nrParameters = m->getNrParameters();
	      int nrNames = names.size();
	      if (nrParameters != nrNames)
		return false;  // wrong number of arguments

	      Vector<View*> views(nrParameters);
	      bool hasTheoryView = false;
	      bool hasPEM = false;
	      for (int i = 0; i < nrParameters; ++i)
		{
		  int code  = names[i];
		  if (enclosingModule != 0)
		    {
		      int index = enclosingModule->findParameterIndex(code);
		      if (index != NONE)
			{
			  //
			  //	Parameters from an enclosing module occlude views.
			  //
			  ImportModule* enclosingModuleParameterTheory = enclosingModule->getParameterTheory(index);
			  ImportModule* requiredParameterTheory = m->getParameterTheory(i);
			  if (enclosingModuleParameterTheory != requiredParameterTheory)
			    return false;  // theory mismatch
			  views[i] = 0;
			  hasPEM = true;
			  continue;
			}
		    }
		  if (View* v = owner->getView(code))
		    {
		      //
		      //	Instantiation is a view.
		      //
		      if (!(v->evaluate()))
			return false;  // bad view
		      ImportModule* fromTheory = v->getFromTheory();
		      ImportModule* requiredParameterTheory = m->getParameterTheory(i);
		      if (fromTheory != requiredParameterTheory)
			return false;  // theory mismatch
		      views[i] = v;
		      if (v->getToModule()->isTheory())
			hasTheoryView = true;
		    }
		  else
		    return false;  // nonexistent view
		}
	      if (hasTheoryView && hasPEM)
		return false;
	      m = owner->makeInstatiation(m, views, names);
	      if (m->isBad())
		return false;
	      return true;
	    }
	}
    }
  return false;
}

bool
MetaLevel::downInstantiationArguments(DagNode* metaArguments, Vector<int>& arguments)
{
  Assert(arguments.empty(), "nonempty arg list");
  if (metaArguments->symbol() == metaArgSymbol)
    {
      for (DagArgumentIterator i(metaArguments); i.valid(); i.next())
	{
	  int id;
	  if (!downQid(i.argument(), id))
	    return false;
	  arguments.append(id);
	}
    }
  else
    {
      int id;
      if (!downQid(metaArguments, id))
	return false;
      arguments.append(id);
    }
  return true;
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
