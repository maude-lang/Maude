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
//	Code for constructing an VisibleModule from the tokenized user input.
//

void
SyntacticPreModule::process()
{
  flatModule = new VisibleModule(id(), getModuleType(), getOwner());
  flatModule->addUser(this);
  flatModule->setLineNumber(getLineNumber());
#ifdef QUANTIFY_PROCESSING
  quantify_start_recording_data();
#endif
  //
  //	Handle import declarations.
  //
  processImports();
  if (flatModule->isBad())
    {
      //
      //	At least one of our imports failed in someway so we're going bail to avoid 
      //	cascading warnings.
      //
      //	This is a bit of a hack; getFlatSignature() uses Module::getStatus() != Module::OPEN
      //	as a proxy for all imports done, to avoid cyclic importation. Closing the sort set
      //	moves us from Module::OPEN to Module::SORT_SET_CLOSED, avoiding the confusion.
      //	This module is toast so we don't worry about closing the sort set before we insert
      //	its sorts.
      //	
      flatModule->closeSortSet();

      IssueWarning(*flatModule <<
		   ": this module contains one or more errors that could not \
be patched up and thus it cannot be used or imported.");
      flatModule->resetImports();
      return;
    }
  //
  //	Hande sorts and subsorts.
  //
  flatModule->importSorts();  // could markAsBad()
  processSorts();  // might add missing sorts
  checkOpTypes();  // might add missing sorts
  flatModule->closeSortSet();  // computes connectedComponents; could markAsBad()
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
  computeStrategyTypes();
  flatModule->importStrategies();
  processStrategies();
  flatModule->insertPotentialLabels(potentialLabels);
  flatModule->insertPotentialRuleLabels(potentialRuleLabels);
  flatModule->importRuleLabels();
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
  //	Handle mbs, eqs, rls and sds.
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
SyntacticPreModule::processSorts()
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
			   ": redeclaration of sort " << QUOTE(sort) << '.');
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
SyntacticPreModule::getSort(Token token)
{
  //
  //	Check that token corresponds to an actual sort.
  //	If it doesn't, we assume the user just forgot to declare it,
  //	and add it so we can press on.
  //
  int code = token.code();
  Sort* sort = flatModule->findSort(code);
  if (sort == 0)
    {
      //
      sort = flatModule->addSort(code);
      sort->setLineNumber(FileTable::SYSTEM_CREATED);
      IssueWarning(LineNumber(token.lineNumber()) <<
		   ": undeclared sort " << QUOTE(sort) << '.');
    }
  return sort;
}

void
SyntacticPreModule::checkType(const Type& type)
{
  //
  //	Check that all the tokens appearing in a type name correspond
  //	to actual sorts.
  //
  int nrTokens = type.tokens.length();
  for (int i = 0; i < nrTokens; i++)
    (void) getSort(type.tokens[i]);
}

void
SyntacticPreModule::checkOpTypes()
{
  //
  //	Check that all the Tokens in the types of operator definitions
  //	correspond to actually sorts. This is a sanity check for undeclared
  //	sort names; we don't have the notion of connected components at
  //	this points.
  //
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

  // Also checks strategy types
  for (const StratDecl& decl : stratDecls)
    for (const Type& type : decl.types)
      checkType(type);
}

void
SyntacticPreModule::computeOpTypes()
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

void
SyntacticPreModule::computeStrategyTypes()
{
  for (StratDecl& decl : stratDecls)
    {
      int nrTypes = decl.types.length();
      decl.domainAndSubject.expandTo(nrTypes);

      for (int i = 0; i < nrTypes; i++)
	decl.domainAndSubject[i] = computeType(decl.types[i]);
    }
}

Sort*
SyntacticPreModule::computeType(const Type& type)
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
SyntacticPreModule::processOps()
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
							   opDef.format,
							   opDef.metadata);
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
						       opDef.metadata,
						       opDecl.originator);
	  if (flatModule->parameterDeclared(opDecl.symbol))
	    {
	      IssueWarning(LineNumber(opDecl.prefixName.lineNumber())  <<
			   ": operator declaration for operation " << QUOTE(opDecl.prefixName) <<
			   " subsort overloads an operator of the same name from a parameter.");
	      flatModule->markAsBad();
	      return;
	    }

	  if (opDef.symbolType.getBasicType() == SymbolType::BUBBLE)
	    {
	      int h = findHook(opDef.special, ID_HOOK, bubble);
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
	      h = findHook(opDef.special, ID_HOOK, exclude);
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
SyntacticPreModule::processStrategies()
{
  // The user may have written strategy declarations in a non-strategy module,
  // we already warned about this but we explicitely ignore them
  if (!MixfixModule::isStrategic(getModuleType()))
    return;

  for (StratDecl& decl : stratDecls)
    {
      int nrNames = decl.names.length();
      int nrArgs = decl.domainAndSubject.length() - 1;

      Assert(nrNames > 0, "strategy declaration without label");

      // The subject sort is removed from the domain vector
      Sort* subjectSort = decl.domainAndSubject[nrArgs];
      decl.domainAndSubject.contractTo(nrArgs);

      // Inserts the strategies in the module
      for (int j = 0; j < nrNames; j++)
	flatModule->addStrategy(decl.names[j], decl.domainAndSubject, subjectSort, decl.metadata);
    }
}

void
SyntacticPreModule::processStatements()
{
  int nrStatements = statements.length();
  for (int i = 0; i < nrStatements; i++)
    flatModule->parseStatement(statements[i]);
}

void
SyntacticPreModule::processImports()
{
  processParameters(flatModule);
  if (flatModule->isBad())
    {
      //
      //	We give up early to avoid cascading warnings about missing parameters.
      //
      return;
    }
  //
  //	Automatic imports (not for theories).
  //
  if (!(MixfixModule::isTheory(getModuleType())))
    {
      for (const auto& i : autoImports)
	{
	  if (ImportModule* fm = getOwner()->getModuleOrIssueWarning(i.first, *this))
	    flatModule->addImport(fm, i.second, *this);
	  else
	    {
	      //
	      //	Mark the module as bad to avoid cascading warnings and potential
	      //	internal errors. But press ahead with imports since they should
	      //	be independent and we might find other errors.
	      //
	      flatModule->markAsBad();
	    }
	}
    }
  //
  //	Defaut includes for object-oriented modules and theories.
  //
  for (const int i : ooIncludes)
    {
      if (ImportModule* fm = getOwner()->getModuleOrIssueWarning(i, *this))
	flatModule->addImport(fm, ImportModule::INCLUDING, *this);
      else
	{
	  //
	  //	Mark the module as bad to avoid cascading warnings and potential
	  //	internal errors. But press ahead with imports since they should
	  //	be independent and we might find other errors.
	  //
	  flatModule->markAsBad();
	}
    }

  processExplicitImports(flatModule);
}
