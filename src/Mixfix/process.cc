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
//	Code for constructing an VisibleModule from the tokenized user input.
//

void
PreModule::process()
{
  flatModule = new VisibleModule(id(), moduleType, this);
  flatModule->setLineNumber(getLineNumber());
#ifdef QUANTIFY_PROCESSING
  quantify_start_recording_data();
#endif
  //
  //	Handle import declarations.
  //
  processImports();
  //
  //	Hande sorts and subsorts.
  //
  flatModule->importSorts();
  processSorts();
  checkOpTypes();
  flatModule->closeSortSet();
  if (flatModule->isBad())
    {
      IssueWarning(*flatModule <<
		   ": this module contains one or more errors that could not \
be patched up and thus it cannot be used or imported.");
      flatModule->resetImports();
      return;
    }
  //
  //	Handle ops and vars.
  //
  flatModule->importOps();
  computeOpTypes();
  processOps();
  if (flatModule->isBad())
    {
      IssueWarning(*flatModule <<
		   ": this module contains one or more errors that could not \
be patched up and thus it cannot be used or imported.");
      flatModule->resetImports();
      return;
    }
  flatModule->closeSignature();
  flatModule->insertLabels(labels);
  flatModule->fixUpImportedOps();
  fixUpSymbols();  // this set bad flag for some reason
  if (flatModule->isBad())
    {
      IssueWarning(*flatModule <<
		   ": this module contains one or more errors that could not \
be patched up and thus it cannot be used or imported.");
      flatModule->resetImports();
      return;
    }
  flatModule->closeFixUps();
  //
  //	Handle mbs, eqs and rls.
  //
  processStatements();
  flatModule->localStatementsComplete();
  //
  //	Reset phase counter in each imported module
  //
  flatModule->resetImports();
#ifdef QUANTIFY_PROCESSING
  quantify_stop_recording_data();
#endif
}

void
PreModule::processSorts()
{
  //
  //	Handle sorts.
  //
  int nrSortDecls = sortDecls.length();
  for (int i = 0; i < nrSortDecls; i++)
    {
      Vector<Token>& sortDecl = sortDecls[i];
      int nrSorts = sortDecl.length();
      for (int j = 0; j < nrSorts; j++)
	{
	  Token& token = sortDecl[j];
	  int code = token.code();
	  Sort* sort = flatModule->findSort(code);
	  if (sort == 0)
	    {
	      sort = flatModule->addSort(code);
	      sort->setLineNumber(token.lineNumber());
	    }
	  else
	    {
	      IssueWarning(LineNumber(token.lineNumber()) <<
			   ": redeclaration of sort " << QUOTE(token.name()) << '.');
	    }
	}
    }
  //
  //	Handle subsorts.
  //
  int nrSubsortDecls = subsortDecls.length();
  Vector<Sort*> smaller;
  Vector<Sort*> bigger;
  for (int i = 0; i < nrSubsortDecls; i++)
    {
      Vector<Token>& subsortDecl = subsortDecls[i];
      int len = subsortDecl.length();
      int j = 0;
      do
	{
	  for (; j < len; j++)
	    {
	      Token& token = subsortDecl[j];
	      if (bigger.length() > 0 && token.code() == lessThan)
		{
		  ++j;
		  WarningCheck(j < len,
			       LineNumber(token.lineNumber()) <<
			       ": stray < at the end of subsort declaration.");
		  break;
		}
	      bigger.append(getSort(token));
	    }
	  int nrSmaller = smaller.length();
	  if (nrSmaller > 0)
	    {
	      int nrBigger = bigger.length();
	      for (int k = 0; k < nrBigger; k++)
		{
		  Sort* s = bigger[k];
		  for (int l = 0; l < nrSmaller; l++)
		    s->insertSubsort(smaller[l]);
		}
	    }
	  smaller.swap(bigger);
	  bigger.contractTo(0);
	}
      while (j < len);
      smaller.contractTo(0);
    }
}

Sort*
PreModule::getSort(Token token)
{
  int code = token.code();
  Sort* sort = flatModule->findSort(code);
  if (sort == 0)
    {
      IssueWarning(LineNumber(token.lineNumber()) <<
		   ": undeclared sort " << QUOTE(token.name()) << '.');
      sort = flatModule->addSort(code);
      sort->setLineNumber(FileTable::SYSTEM_CREATED);
    }
  return sort;
}

void
PreModule::checkOpTypes()
{
  int nrOpDefs = opDefs.length();
  for (int i = 0; i < nrOpDefs; i++)
    {
      OpDef& def = opDefs[i];
      int nrTypes = def.types.length();
      for (int j = 0; j < nrTypes; j++)
	{
	  int k = j + 1;
	  if (k == nrTypes)
	    k = 0;
	  if (!(def.polyArgs.contains(k)))
	    checkType(def.types[j]);
	}
    }
}

void
PreModule::checkType(const Type& type)
{
  int nrTokens = type.tokens.length();
  for (int i = 0; i < nrTokens; i++)
    (void) getSort(type.tokens[i]);
}

void
PreModule::computeOpTypes()
{
 int nrOpDefs = opDefs.length();
  for (int i = 0; i < nrOpDefs; i++)
    {
      OpDef& def = opDefs[i];
      int nrTypes = def.types.length();
      def.domainAndRange.expandTo(nrTypes);
      for (int j = 0; j < nrTypes; j++)
	{
	  int k = j + 1;
	  if (k == nrTypes)
	    k = 0;
	  def.domainAndRange[j] = def.polyArgs.contains(k) ? 0 :
	    computeType(def.types[j]);
	}
    }
}

Sort*
PreModule::computeType(const Type& type)
{
  if (type.kind)
    {
      int nrTokens = type.tokens.length();
      Sort* s = flatModule->findSort(type.tokens[0].code());
      Assert(s != 0, "missing sort");
      ConnectedComponent* c = s->component();
      for (int i = 1; i < nrTokens; i++)
	{
	  Sort* t = flatModule->findSort(type.tokens[i].code());
	  Assert(t != 0, "missing sort");
	  WarningCheck(t->component() == c,
		       LineNumber(type.tokens[i].lineNumber()) <<
		       ": sorts " << QUOTE(s) << " and " << QUOTE(t) <<
		       " are in different components.");
	}
      return c->sort(Sort::ERROR_SORT);
    }
  else
    {
      Assert(type.tokens.length() == 1,
	     "bad number of tokens " << type.tokens.length());
      Sort* s = flatModule->findSort(type.tokens[0].code());
      Assert(s != 0, "missing sort");
      return s;
    }
}

void
PreModule::processOps()
{
  //
  //	Process opDecls.
  //
  int nrOpDecls = opDecls.length();
  for (int i = 0; i < nrOpDecls; i++)
    {
      OpDecl& opDecl = opDecls[i];
      OpDef& opDef = opDefs[opDecl.defIndex];
      if (opDef.symbolType.hasFlag(SymbolType::POLY))
	{
	  opDecl.polymorphIndex = flatModule->addPolymorph(opDecl.prefixName,
							   opDef.domainAndRange,
							   opDef.symbolType,
							   opDef.strategy,
							   opDef.frozen,
							   opDef.prec,
							   opDef.gather,
							   opDef.format);
	  opDecl.originator = true;  // HACK
	}
      else if (opDef.symbolType.getBasicType() == SymbolType::VARIABLE)
	{
	  flatModule->addVariableAlias(opDecl.prefixName, opDef.domainAndRange[0]);
	  opDecl.symbol = 0;
	  opDecl.originator = false;  // HACK
	}
      else
	{
	  opDecl.symbol = flatModule->addOpDeclaration(opDecl.prefixName,
						       opDef.domainAndRange,
						       opDef.symbolType,
						       opDef.strategy,
						       opDef.frozen,
						       opDef.prec,
						       opDef.gather,
						       opDef.format,
						       opDecl.originator);
	  if (opDef.symbolType.getBasicType() == SymbolType::BUBBLE)
	    {
	      int h = findHook(opDef.special, ID_HOOK, Token::encode("Bubble"));
	      Vector<Token>& details = opDef.special[h].details;
	      int min = 1;
	      int max = -1;
	      int left = NONE;
	      int right = NONE;
	      Vector<int> excluded;
	      int nrDetails = details.length();
	      if (nrDetails > 0)
		{
		  min = atoi(details[0].name());
		  if (nrDetails > 1)
		    {
		      max = atoi(details[1].name());
		      if (nrDetails > 3)
			{
			  left = details[2].code();
			  right = details[3].code();
			}
		    }
		}
	      h = findHook(opDef.special, ID_HOOK, Token::encode("Exclude"));
	      if (h != NONE)
		{
		  Vector<Token>& details = opDef.special[h].details;
		  int nrExcluded = details.length();
		  excluded.expandTo(nrExcluded);
		  for (int j = 0; j < nrExcluded; j++)
		    excluded[j] = details[j].code();
		}
	      opDecl.bubbleSpecIndex =
		flatModule->addBubbleSpec(opDecl.symbol, min, max, left, right, excluded);
	    }
	}
    }
}

void
PreModule::processStatements()
{
  int nrStatements = statements.length();
  for (int i = 0; i < nrStatements; i++)
    flatModule->parseStatement(statements[i]);
}
