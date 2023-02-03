/*

    This file is part of the Maude 3 interpreter.

    Copyright 1997-2023 SRI International, Menlo Park, CA 94025, USA.

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
  processSubsorts();  // might add missing sorts
  checkOpTypes();  // might add missing sorts

  if (MixfixModule::isObjectOriented(getModuleType()))
    {
      processClassSorts();  // could markAsBad()
      checkAttributeTypes();  // might add missing sorts
    }

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
  if (MixfixModule::isObjectOriented(getModuleType()))
    {
      computeAttributeTypes();
      processClassOps();
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
  if (MixfixModule::isObjectOriented(getModuleType()))
    {
      //
      //	Install ourself as a statement transformer so we get to modify statements
      //	fresh from the parser, before they are inserted into the module.
      //
      flatModule->installStatementTransformer(this);
    }
  processStatements();
  flatModule->processingComplete();
  flatModule->localStatementsComplete();
  //
  //	Reset phase counter in each imported module
  //
  flatModule->resetImports();
  if (MixfixModule::isObjectOriented(getModuleType()))
    {
      //
      //	Clear object-oriented processing data in case we need to re-examine this PreModule.
      //
      classIdSort = 0;
      attributeSetSort = 0;
      attributeSort = 0;
      classNames.clear();
      attributeSymbols.clear();
    }
#ifdef QUANTIFY_PROCESSING
  quantify_stop_recording_data();
#endif
}

void
SyntacticPreModule::processSorts()
{
  for (const Vector<Token>& sortDecl : sortDecls)
    {
      for (const Token& token : sortDecl)
	{
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
}

void
SyntacticPreModule::processSubsorts()
{
  Vector<Sort*> smaller;
  Vector<Sort*> bigger;
  for (const Vector<Token>& subsortDecl : subsortDecls)
    {
      for (const Token& token : subsortDecl)
	{
	  if (token.code() == lessThan)
	    {
	      insertSubsorts(smaller, bigger);
	      smaller.swap(bigger);
	      bigger.clear();
	    }
	  else
	    bigger.append(getSort(token));
	}
      insertSubsorts(smaller, bigger);
      smaller.clear();
      bigger.clear();
    }
}

void
SyntacticPreModule::insertSubsorts(const Vector<Sort*> smaller, Vector<Sort*> bigger)
{
  for (Sort* ss : smaller)
    {
      for (Sort* bs : bigger)
	bs->insertSubsort(ss);
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
  for (const Token& t : type.tokens)
    (void) getSort(t);
}

void
SyntacticPreModule::checkOpTypes()
{
  //
  //	Check that all the Tokens in the types of operator definitions
  //	correspond to actually sorts. This is a sanity check for undeclared
  //	sort names; we don't have the notion of connected components at
  //	this point.
  //
  for (const OpDef& def : opDefs)
    {
      Index nrTypes = def.types.size();
      for (Index i = 0; i < nrTypes; ++i)
	{
	  Index argPosition = i + 1;
	  if (argPosition == nrTypes)  // range is argPosition 0
	    argPosition = 0;
	  if (!(def.polyArgs.contains(argPosition)))
	    checkType(def.types[i]);
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
  for (OpDef& def : opDefs)
    {
      Index nrTypes = def.types.size();
      def.domainAndRange.resize(nrTypes);
      for (Index i = 0; i < nrTypes; ++i)
	{
	  Index argPosition = i + 1;
	  if (argPosition == nrTypes)
	    argPosition = 0;
	  def.domainAndRange[i] = def.polyArgs.contains(argPosition) ? 0 : computeType(def.types[i]);
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
      Index nrTokens = type.tokens.length();
      Sort* s = flatModule->findSort(type.tokens[0].code());
      Assert(s != 0, "missing sort");
      ConnectedComponent* c = s->component();
      for (Index i = 1; i < nrTokens; ++i)
	{
	  Sort* t = flatModule->findSort(type.tokens[i].code());
	  Assert(t != 0, "missing sort");
	  WarningCheck(t->component() == c,
		       LineNumber(type.tokens[i].lineNumber()) <<
		       ": sorts " << QUOTE(s) << " and " << QUOTE(t) <<
		       " are in different components.");
	}
      return c->sort(Sort::KIND);
    }
  else
    {
      Assert(type.tokens.size() == 1, "bad number of tokens " << type.tokens.length());
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
  for (OpDecl& opDecl : opDecls)
    {
      OpDef& opDef = opDefs[opDecl.defIndex];
      //
      //	Must clear MSG_STATEMENT attribute before passing it to flatModule otherwise we
      //	could have operators that should polymorphic overload but fail because one
      //	was declared by a message statement while the other was declared with a message
      //	attribute.
      //
      SymbolType symbolType = opDef.symbolType;
      symbolType.clearFlags(SymbolType::MSG_STATEMENT);
      if (symbolType.hasFlag(SymbolType::POLY))
	{
	  opDecl.polymorphIndex = flatModule->addPolymorph(opDecl.prefixName,
							   opDef.domainAndRange,
							   symbolType,
							   opDef.strategy,
							   opDef.frozen,
							   opDef.prec,
							   opDef.gather,
							   opDef.format,
							   opDef.metadata);
	  opDecl.originator = true;  // HACK
	}
      else if (symbolType.getBasicType() == SymbolType::VARIABLE)
	{
	  flatModule->addVariableAlias(opDecl.prefixName, opDef.domainAndRange[0]);
	  opDecl.symbol = 0;
	  opDecl.originator = false;  // HACK
	}
      else
	{
	  opDecl.symbol = flatModule->addOpDeclaration(opDecl.prefixName,
						       opDef.domainAndRange,
						       symbolType,
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

	  if (symbolType.getBasicType() == SymbolType::BUBBLE)
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
  for (const Vector<Token>& statement : statements)
    flatModule->parseStatement(statement);
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
  //	Automatic imports (will be empty for theories).
  //
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
  //
  //	Default includes for object-oriented modules and theories.
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
