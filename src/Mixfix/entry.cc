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
//	Code for entering stuff into a MixfixModule.
//

bool
MixfixModule::unpackLatexMacro(SymbolInfo& si, int arity)
{
  Vector<int> unpacked;
  bool backslashSeen = false;
  for (const char* p = Token::name(si.latexMacro); *p;)
    {
      unsigned char c = *p;
      if (backslashSeen)
	backslashSeen = false;
      else
	{
	  if (c == '\\')
	    backslashSeen = true;
	  else if (c == '#')
	    {
	      ++p;
	      char c2 = *p;
	      if (isdigit(c2) && c2 != '0')
		{
		  char* endPtr;
		  int argNr = strtol(p, &endPtr, 10);
		  if (argNr > 0 && argNr <= arity)
		    {
		      unpacked.push_back(- argNr);
		      p = endPtr;
		      continue;
		    }
		}
	      --p;
	      return false;
	    }
	}
      unpacked.push_back(c);
      ++p;
    }
  si.latexMacroUnpacked.swap(unpacked);
  return true;
}

Sort*
MixfixModule::addSort(int name)
{
  Assert(findSort(name) == 0, "redeclaration of sort" << Token::name(name));
  Sort* s = new Sort(name);
  Module::insertSort(s);
  (void) sortNames.insert(SortMap::value_type(name, s));
  return s;
}

bool
MixfixModule::hasSameDomain(const Vector<Sort*>& domainAndRange1,
			    bool assoc1,
			    const Vector<Sort*>& domainAndRange2,
			    bool assoc2)
{
  int nrArgs1 = domainAndRange1.length() - 1;
  int nrArgs2 = domainAndRange2.length() - 1;
  if (nrArgs1 == nrArgs2)
    {
      for (int i = 0; i < nrArgs1; i++)
	{
	  if (domainAndRange1[i]->component() != domainAndRange2[i]->component())
	    return false;
	}
    }
  else if (assoc1)
    {
      Assert(nrArgs1 == 2 && nrArgs2 > 2,
	     "shouldn't have been called");
      for (int i = 0; i < nrArgs2; i++)
	{
	  if (domainAndRange1[0]->component() != domainAndRange2[i]->component())
	    return false;
	}
    }
  else
    {
      Assert(assoc2 && nrArgs2 == 2 && nrArgs1 > 2,
	     "shouldn't have been called");
      for (int i = 0; i < nrArgs1; i++)
	{
	  if (domainAndRange1[i]->component() != domainAndRange2[0]->component())
	    return false;
	}
    }
  return true;
}

int
MixfixModule::checkPseudoIterated(Symbol* symbol, const Vector<Sort*>& domainAndRange)
{
  //
  //	A symbol with a prefix name that looks like f^n can be confused with
  //	an iterated symbol f when parsing and pretty printing.
  //	We keep track of this extra overloading here.
  //
  int baseName;
  mpz_class number;
  Token::split(symbol->id(), baseName, number);
  pseudoIteratedMap[baseName].insert(NumberToSymbolMap::value_type(number, symbol));
  int overloadType = 0;
  //
  //	Now can we find an iterated symbol with baseName?
  //
  for (IteratedMap::const_iterator i = iteratedMap.lower_bound(baseName);
       i != iteratedMap.end() && i->first == baseName; ++i)
    {
      Symbol* iSymbol = i->second;
      const Vector<Sort*>& iDomainAndRange = iSymbol->getOpDeclarations()[0].getDomainAndRange();
      overloadType |= ADHOC_OVERLOADED;

      bool sameRange = (domainAndRange[1]->component() == iDomainAndRange[1]->component());
      if (sameRange)
	overloadType |= RANGE_OVERLOADED;

      bool sameDomain (domainAndRange[0]->component() == iDomainAndRange[0]->component());
      if (sameDomain)
	{
	  overloadType |= DOMAIN_OVERLOADED;
	  if (sameRange)
	    {
	      IssueWarning(*symbol << ": declaration for operator " << QUOTE(symbol) <<
			   " clashes with declaration for iterated operator " << QUOTE(iSymbol) <<
			   " on " << *iSymbol <<
			   " because of iterated notation.");
	    }
	  else
	    {
	      IssueWarning(*symbol << ": declaration for operator " << QUOTE(symbol) <<
			   " clashes with declaration for iterated operator " << QUOTE(iSymbol) <<
			   " on " << *iSymbol <<
			   ", which has a different range kind, because of iterated notation.");
	    }
	}
    }
  return overloadType;
}

void
MixfixModule::checkIterated(Symbol* symbol, const Vector<Sort*>& domainAndRange)
{
  int name = symbol->id();
  iteratedMap.insert(IteratedMap::value_type(name, symbol));
  //
  //	An iterated symbol f can be confused with a pseudo iterated symbol
  //	with name f^n when parsing and pretty printing.
  //	We keep track of this extra overloading here.
  //
  PseudoIteratedMap::const_iterator i = pseudoIteratedMap.find(name);
  if (i != pseudoIteratedMap.end())
    {
      //
      //	At least one symbol aliasing the iterated forms of symbol.
      //
      for (const auto& j : i->second)
	{
	  Symbol* pSymbol = j.second;
	  int overloadType = ADHOC_OVERLOADED;
	  const Vector<Sort*>& pDomainAndRange = pSymbol->getOpDeclarations()[0].getDomainAndRange();

	  bool sameRange = (domainAndRange[1]->component() == pDomainAndRange[1]->component());
	  if (sameRange)
	    overloadType |= RANGE_OVERLOADED;

	  bool sameDomain (domainAndRange[0]->component() == pDomainAndRange[0]->component());
	  if (sameDomain)
	    {
	      overloadType |= DOMAIN_OVERLOADED;
	      if (sameRange)
		{
		  IssueWarning(*symbol << ": declaration for operator " << QUOTE(symbol) <<
			       " clashes with declaration for iterated operator " << QUOTE(pSymbol) <<
			       " on " << *pSymbol <<
			       " because of iterated notation.");
		}
	      else
		{
		  IssueWarning(*symbol << ": declaration for operator " << QUOTE(symbol) <<
			       " clashes with declaration for iterated " << QUOTE(pSymbol) <<
			       " on " << *pSymbol <<
			       ", which has a different range kind, because of iterated notation.");
		}
	    }
	  symbolInfo[pSymbol->getIndexWithinModule()].iflags |= overloadType;
	}
    }
}

bool
MixfixModule::compatible(Index existingSymbolIndex,
			 SymbolType symbolType,
			 const Vector<int>& strategy,
			 const NatSet& frozen,
			 int prec,
			 const Vector<int>& gather,
			 const Vector<int>& format,
			 int latexMacro,
			 int rpo)
{
  if (!symbolType.compatible(symbolInfo[existingSymbolIndex].symbolType))
    return false;
  //
  //	The rawStrategy field exists for the sole purpose of doing this
  //	check because the strategy inside the symbol is normalized in
  //	theory-dependent ways.
  //
  if (symbolType.hasFlag(SymbolType::STRAT) &&
      !(symbolInfo[existingSymbolIndex].rawStrategy == strategy))
    return false;
  if (symbolType.hasFlag(SymbolType::FROZEN) &&
      getSymbols()[existingSymbolIndex]->getFrozen() != frozen)
    return false;
  if (symbolType.hasFlag(SymbolType::PREC) &&
      symbolInfo[existingSymbolIndex].prec != prec)
    return false;
  if (symbolType.hasFlag(SymbolType::LATEX) &&
      symbolInfo[existingSymbolIndex].latexMacro != latexMacro)
    return false;
  //
  //	Gathers aren't converted to precedence numbers until we call
  //	computePrecAndGather() when we close the signature.
  //
  if (symbolType.hasFlag(SymbolType::GATHER) &&
      !(symbolInfo[existingSymbolIndex].gather == gather))
    return false;
  if (symbolType.hasFlag(SymbolType::FORMAT) &&
      !(symbolInfo[existingSymbolIndex].format == format))
    return false;
  if (symbolType.hasFlag(SymbolType::RPO) && symbolInfo[existingSymbolIndex].rpo != rpo)
    return false;
  return true;
}

Symbol*
MixfixModule::addOpDeclaration(Token prefixName,
			       const Vector<Sort*>& domainAndRange,
			       SymbolType symbolType,
			       const Vector<int>& strategy,
			       const NatSet& frozen,
			       int prec,
			       const Vector<int>& gather,
			       const Vector<int>& format,
			       int latexMacro,
			       int rpo,
			       int metadata,
			       bool& firstDecl)
{
  Assert(symbolType.getBasicType() != SymbolType::VARIABLE,
	 "we don't handle variables anymore");

  if (symbolType.dittoProblem())
    {
      IssueWarning(LineNumber(prefixName.lineNumber()) <<
		   ": declaration for " << QUOTE(prefixName) <<
		   " combines ditto with attributes other than ctor.");
    }

  int nrArgs = domainAndRange.length() - 1;
  int name = prefixName.code();
  int iflags = 0;
  IntMap::const_iterator first = firstSymbols.find(name);
  if (first != firstSymbols.end())
    {
      //
      //	Examine all existing symbols with same name.
      //
      for (int i = first->second; i != NONE; i = symbolInfo[i].next)
	{
	  Symbol* s = getSymbols()[i];
	  int iNrArgs = s->arity();
	  bool assoc = symbolType.hasFlag(SymbolType::ASSOC);
	  bool iAssoc = symbolInfo[i].symbolType.hasFlag(SymbolType::ASSOC);
	  if (nrArgs == iNrArgs ||
	      (assoc && nrArgs == 2 && iNrArgs > 2) ||
	      (iAssoc && nrArgs > 2))
	    {
	      //
	      //	We have the same name and could appear to have
	      //	the same number of arguments. Thus ambiguity is possible
	      //	and we have to rely on context or actual arguments for
	      //	disambiguation.
	      //
	      const Vector<Sort*>& iDomainAndRange =
		s->getOpDeclarations()[0].getDomainAndRange();
	      bool sameDomain =
		hasSameDomain(domainAndRange, assoc, iDomainAndRange, iAssoc);
	      bool sameRange = (domainAndRange[nrArgs]->component() ==
				iDomainAndRange[iNrArgs]->component());
	      int overloadType;
	      if (sameDomain)
		{
		  if (sameRange)
		    {
		      if (nrArgs == iNrArgs)
			{
			  firstDecl = false;
			  if (!symbolType.hasFlag(SymbolType::DITTO) &&
			      !compatible(i,
					  symbolType,
					  strategy,
					  frozen,
					  prec,
					  gather,
					  format,
					  latexMacro,
					  rpo))
			    {
			      IssueWarning(LineNumber(prefixName.lineNumber()) <<
					   ": declaration for " << QUOTE(s) <<
					   " has different attributes from declaration on " <<
					   *s << '.');
			      DebugAdvisory("orginal symbol type: " << getSymbolType(s) <<
					    "\tnew symbol type: " << symbolType <<
					    "\tflag difference: " <<
					    (getSymbolType(s).getFlags() ^ symbolType.getFlags()));
			      markAsBad();
			    }
			  if (metadata != NONE)
			    insertMetadata(s, s->getOpDeclarations().length(), metadata);
			  s->addOpDeclaration(domainAndRange, symbolType.hasFlag(SymbolType::CTOR));
			  return s;
			}
		      else
			{
			  IssueWarning(LineNumber(prefixName.lineNumber()) <<
				       ": declaration for " << QUOTE(s) <<
				       " clashes with declaration on " << *s <<
				       " because of associativity.");
			  overloadType = ADHOC_OVERLOADED | DOMAIN_OVERLOADED | RANGE_OVERLOADED;
			}
		    }
		  else
		    {
		      if (nrArgs > 0)
			{
			  if (nrArgs == iNrArgs)
			    {
			      IssueWarning(LineNumber(prefixName.lineNumber()) <<
					   ": declaration for " << QUOTE(s) <<
					   " has the same domain kinds as the declaration on " <<
					   *s << " but a different range kind.");
			    }
			  else
			    {
			      IssueWarning(LineNumber(prefixName.lineNumber()) <<
					   ": declaration for " << QUOTE(s) <<
					   " clashes with declaration on " << *s <<
					   ", which has a different range kind, because of associativity.");
			    }
			}
		      overloadType = ADHOC_OVERLOADED | DOMAIN_OVERLOADED;
		    }
		}
	      else
		{
		  overloadType = sameRange ? (ADHOC_OVERLOADED | RANGE_OVERLOADED) :
		    ADHOC_OVERLOADED;
		}
	      iflags |= overloadType;
	      symbolInfo[s->getIndexWithinModule()].iflags |= overloadType;
	    }
	}
    }
  //
  //	We didn't find an existing symbol to add this declaration to so
  //	we need to create a new symbol.
  //
  if (symbolType.hasFlag(SymbolType::DITTO))
    {
      IssueWarning(LineNumber(prefixName.lineNumber()) <<
		   ": declaration for " << QUOTE(prefixName) <<
		   " uses ditto without a previous declaration to refer to.");
      symbolType.clearFlags(SymbolType::DITTO);
    }	   
  //
  //	Make sure that domainAndRange supports attributes and
  //	clear any unsupported flags.
  //
  validateAttributes(prefixName, domainAndRange, symbolType);
  
  Symbol* symbol = newFancySymbol(prefixName, domainAndRange, symbolType, strategy);
  if (symbol == 0)
    {
      symbol = FreeSymbol::newFreeSymbol(name, nrArgs, strategy,
					 symbolType.hasFlag(SymbolType::MEMO));
      symbolType.clearFlags(SymbolType::AXIOMS | SymbolType::ITER);
    }
  if (symbolType.hasFlag(SymbolType::FROZEN))
    symbol->setFrozen(frozen);
  symbol->setLineNumber(prefixName.lineNumber());

  //
  //	Fill in symbolInfo entry with syntactic info.
  //
  if (nrArgs == 0)
    {
      int sp = prefixName.specialProperty();
      if (sp == Token::CONTAINS_COLON)
	{
	  pair<int, int> p;
	  Token::split(prefixName.code(), p.first, p.second);
	  if (findSort(p.second))  // not perfect since we don't handle kinds
	    {
	      WarningCheck(domainAndRange[0]->id() != p.second,
			   LineNumber(prefixName.lineNumber()) <<
			   ": operator " << QUOTE(prefixName) <<
			   " is indistinguishable from a variable.");
	      overloadedVariables.insert(p);
	      iflags |= PSEUDO_VARIABLE;
	    }
	}
      else if (sp == Token::FLOAT)
	{
	  //
	  //	Maybe we should check that it's actually in canonical form before we
	  //	insert it. Maybe we could store strings rather than doubles.
	  //	Either way we always need to set the PSEUDO_FLOAT flag.
	  //
	  overloadedFloats.insert(Token::codeToDouble(prefixName.code()));
	  iflags |= PSEUDO_FLOAT;
	} 
      else if (sp == Token::STRING)
	{
	  //
	  //	Maybe we should check that it's actually in canonical form before we
	  //	insert it. Either way we always need to set the PSEUDO_STRING flag.
	  //
	  overloadedStrings.insert(prefixName.name());
	  iflags |= PSEUDO_STRING;
	}
      else if (sp == Token::QUOTED_IDENTIFIER)
	{
	  overloadedQuotedIdentifiers.insert(Token::encode(prefixName.name() + 1));
	  iflags |= PSEUDO_QUOTED_IDENTIFIER;
	}
      else if (sp == Token::SMALL_NAT)
	{
	  mpz_class nat(prefixName.name(), 10);
	  overloadedIntegers.insert(nat);
	  iflags |= PSEUDO_NAT;
	}
      else if (sp == Token::ZERO)
	{
	  //
	  //	This is not needed for built-in Nats because 0 appears as a regular constant
	  //	and the standard operator pretty printing will handle the disambiguation.
	  //	It is needed for SMT Integers, because here 0 is a built in non-algebraic
	  //	constant, and the SMT pretty printer needs to know that another 0 exists.
	  //
	  //
	  overloadedIntegers.insert(0);
	  //
	  //	Also when we print this constant we need to force a check of kindsWithZero
	  //	when we decide whether disambiguation is needed. In the case of built-in
	  //	Nats, the Nat constant zero will already set the abiguity flag, but
	  //	in the case of SMT Integers, the constant zero is implicit.
	  //
	  iflags |= PSEUDO_ZERO;
	}
      else if (sp == Token::SMALL_NEG)
	{
	  mpz_class neg(prefixName.name(), 10);
	  overloadedIntegers.insert(neg);
	  iflags |= PSEUDO_NEG;
	}
      else if (sp == Token::RATIONAL)
	{
	  pair<mpz_class, mpz_class> rat;
	  prefixName.getRational(rat.first, rat.second);
	  overloadedRationals.insert(rat);
	  iflags |= PSEUDO_RAT;
	}
    }
  //
  //	Need to worry about unary operators whose prefix name looks like f^2; i.e.
  //	iterated notation.
  //
  if (nrArgs == 1 && prefixName.specialProperty() == Token::ITER_SYMBOL)
    iflags |= checkPseudoIterated(symbol, domainAndRange);
  //
  //	We also need the check iterated symbols against any symbols with prefix
  //	names like f^n.
  if (symbolType.hasFlag(SymbolType::ITER))
    checkIterated(symbol, domainAndRange);

  int nrSymbols = symbolInfo.length();
  symbolInfo.expandBy(1);
  SymbolInfo& si = symbolInfo[nrSymbols];
  int nrUnderscores = Token::extractMixfix(name, si.mixfixSyntax);
  if (nrUnderscores == Token::PAREN_MISMATCH)
    {
      IssueWarning(LineNumber(prefixName.lineNumber()) << ": mismatched parentheses in operator " <<
		   QUOTE(prefixName) << ". It will be treated as having prefix syntax only.");
      symbolType.clearFlags(SymbolType::PREC | SymbolType::GATHER | SymbolType::FORMAT);
    }
  else if (si.mixfixSyntax.empty())
    {
      si.prec = 0;
      WarningCheck(!(symbolType.hasFlag(SymbolType::PREC)),
		   *symbol <<
		   ": prec attribute without mixfix syntax for operator " <<
		   QUOTE(symbol) << '.');
      WarningCheck(!(symbolType.hasFlag(SymbolType::GATHER)),
		   *symbol <<
		   ": gather attribute without mixfix syntax for operator " <<
		   QUOTE(symbol) << '.');
      symbolType.clearFlags(SymbolType::PREC | SymbolType::GATHER);
      if (symbolType.hasFlag(SymbolType::FORMAT))
	{
	  if (format.length() == 2)
	    si.format = format;  // deep copy
	  else
	    {
	      IssueWarning(LineNumber(prefixName.lineNumber()) <<
			   ": bad format length of " <<
			   format.length() <<
			   ", should be 2.");
	      symbolType.clearFlags(SymbolType::FORMAT);
	    }
	}
    }
  else
    {
      if (si.mixfixSyntax.length() == 1 && nrUnderscores == 1)
	{
	  IssueWarning(*symbol << ": empty syntax not allowed for single argument operators.");
	  si.mixfixSyntax.clear();
	  si.prec = 0;
	  symbolType.clearFlags(SymbolType::PREC | SymbolType::GATHER | SymbolType::FORMAT);
	}
      if (nrArgs != nrUnderscores)
	{
	  IssueWarning(*symbol <<
		       ": number of underscores (" << nrUnderscores << ") does not match " <<
		       "number of arguments (" << nrArgs << ") for operator " <<
		       QUOTE(symbol) << '.');
	  si.mixfixSyntax.clear();
	  si.prec = 0;
	  symbolType.clearFlags(SymbolType::PREC | SymbolType::GATHER | SymbolType::FORMAT);
	}
      else
	{
	  si.prec = prec;
	  si.gather = gather;  // deep copy
	  if (symbolType.hasFlag(SymbolType::FORMAT))
	    {
	      if (si.mixfixSyntax.length() + 1 != format.length())
		{
		  IssueWarning(LineNumber(prefixName.lineNumber()) <<
			       ": bad format length of " <<
			       format.length() <<
			       ", should be " <<
			       si.mixfixSyntax.length() + 1 << '.');
		  symbolType.clearFlags(SymbolType::FORMAT);
		}
	      else
		si.format = format;  // deep copy
	    }
	}
    }
  si.latexMacro = latexMacro;
  if (symbolType.hasFlag(SymbolType::LATEX) && !unpackLatexMacro(si, nrArgs))
    {
      IssueWarning(*symbol << ": bad latex attribute for operator " << QUOTE(symbol) << '.');
      si.latexMacro = NONE;
      symbolType.clearFlags(SymbolType::LATEX);
    }
  si.rawStrategy = strategy;  // unmodified user strategy for consistency checking
  si.rpo = rpo;
  si.polymorphIndex = NONE;
  si.symbolType = symbolType;
  si.symbolType.clearFlags(SymbolType::CTOR);  // don't store ctor flag in per-symbol struct
  si.iflags = iflags;
  si.next = (first == firstSymbols.end()) ? NONE : first->second;
  firstSymbols[name] = nrSymbols;

  Module::insertSymbol(symbol);
  if (metadata != NONE)
    insertMetadata(symbol, symbol->getOpDeclarations().length(), metadata);
  symbol->addOpDeclaration(domainAndRange, symbolType.hasFlag(SymbolType::CTOR));
  firstDecl = true;
  switch (symbolType.getBasicType())
    {
    case SymbolType::SYSTEM_TRUE:
      {
	if (trueSymbol == 0)
	  {
	    trueSymbol = safeCast(FreeSymbol*, symbol);  // HACK
	    boolSort = domainAndRange[0];
	  }
	else
	  {
	    IssueWarning(*symbol << ": declaration for SystemTrue symbol " <<
			 QUOTE(symbol) << " conflicts with SystemTrue symbol " <<
			 QUOTE(trueSymbol) << " on " << *trueSymbol << '.');
	  }
	break;
      }
    case SymbolType::SYSTEM_FALSE:
      {
	if (falseSymbol == 0)
	  {
	    falseSymbol = safeCast(FreeSymbol*, symbol);  // HACK
	    boolSort = domainAndRange[0];
	  }
	else
	  {
	    IssueWarning(*symbol << ": declaration for SystemFalse symbol " <<
			 QUOTE(symbol) << " conflicts with SystemFalse symbol " <<
			 QUOTE(falseSymbol) << " on " << *falseSymbol << '.');
	  }
	break;
      }

  //  if (flags & BUBBLE)
  //  bubbleComponents.insert(domainAndRange[nrArgs]->component()->indexWithinModule());

    case SymbolType::FLOAT:
      {
	ConnectedComponent* c = domainAndRange[nrArgs]->component();
	floatSymbols[c->getIndexWithinModule()] = symbol;
	break;
      }
    case SymbolType::STRING:
      {
	ConnectedComponent* c = domainAndRange[nrArgs]->component();
	stringSymbols[c->getIndexWithinModule()] = symbol;
	break;
      }
    case SymbolType::QUOTED_IDENTIFIER:
      {
	ConnectedComponent* c = domainAndRange[nrArgs]->component();
	quotedIdentifierSymbols[c->getIndexWithinModule()] = symbol;
	break;
      }
    case SymbolType::SMT_NUMBER_SYMBOL:
      {
	ConnectedComponent* c = domainAndRange[nrArgs]->component();
	SMT_NumberSymbols[c->getIndexWithinModule()] = symbol;
      }
    }
  if (symbolType.hasFlag(SymbolType::CONFIG))
    {
      if (dynamic_cast<ConfigSymbol*>(symbol) == 0)
	{
	  IssueWarning(LineNumber(prefixName.lineNumber()) <<
		       ": operator " <<
		       BEGIN_QUOTE << symbol << END_QUOTE <<
		       " cannot take config attribute.");
	}
    }
  if (symbolType.hasFlag(SymbolType::OBJECT))
    {
      WarningCheck(!(symbolType.hasFlag(SymbolType::MESSAGE)),
		   LineNumber(prefixName.lineNumber()) <<
		   ": operator " <<
		   BEGIN_QUOTE << symbol << END_QUOTE <<
		   " is not allowed to have both object and msg attributes.");
      WarningCheck(!(symbolType.hasFlag(SymbolType::PORTAL)),
		   LineNumber(prefixName.lineNumber()) <<
		   ": operator " <<
		   BEGIN_QUOTE << symbol << END_QUOTE <<
		   " is not allowed to have both object and portal attributes.");

      if (nrArgs == 0)
	{
	  IssueWarning(LineNumber(prefixName.lineNumber()) <<
		       ": object attribute for operator " <<
		       BEGIN_QUOTE << symbol << END_QUOTE <<
		       " which lacks arguments.");
	}
      else
	objectSymbols.insert(symbol->getIndexWithinModule());
    }
  else if (symbolType.hasFlag(SymbolType::MESSAGE))
    {
      WarningCheck(!(symbolType.hasFlag(SymbolType::PORTAL)),
		   LineNumber(prefixName.lineNumber()) <<
		   ": operator " <<
		   BEGIN_QUOTE << symbol << END_QUOTE <<
		   " is not allowed to have both msg and portal attributes.");

      if (nrArgs == 0)
	{
	  IssueWarning(LineNumber(prefixName.lineNumber()) <<
		       ": msg attribute for operator " <<
		       BEGIN_QUOTE << symbol << END_QUOTE <<
		       " which lacks arguments.");
	}
      else
	messageSymbols.insert(symbol->getIndexWithinModule());
    }
  else if (symbolType.hasFlag(SymbolType::PORTAL))
    portalSymbols.insert(symbol->getIndexWithinModule());
  return symbol;
}

void
MixfixModule::addVariableAlias(Token name, Sort* sort)
{
  pair<AliasMap::iterator, bool> r =
    variableAliases.insert(AliasMap::value_type(name.code(), sort));
  WarningCheck(r.second, LineNumber(name.lineNumber()) <<
	       ": redeclaration of variable alias " << QUOTE(name) << '.');
}

int
MixfixModule::addPolymorph(Token prefixName,
			   const Vector<Sort*>& domainAndRange,
			   SymbolType symbolType,
			   const Vector<int>& strategy,
			   const NatSet& frozen,
			   int prec,
			   const Vector<int>& gather,
			   const Vector<int>& format,
			   int latexMacro,
			   int rpo,
			   int metadata)
{
  int index = findPolymorphIndex(prefixName.code(), domainAndRange);
  if (index != NONE)
    {
      IssueWarning(LineNumber(prefixName.lineNumber()) <<
		   ": declaration for polymorphic operator " << QUOTE(prefixName) <<
		   " duplicates declaration on " <<
		   LineNumber(polymorphs[index].name.lineNumber()) << '.');
      return index;
    }
  validateAttributes(prefixName, domainAndRange, symbolType);
  int nrArgs = domainAndRange.size() - 1;
  int nrPolymorphs = polymorphs.length();
  polymorphs.expandBy(1);
  Polymorph& p = polymorphs[nrPolymorphs];
  p.name = prefixName;
  p.domainAndRange = domainAndRange;  // deep copy
  p.strategy = strategy;  // deep copy
  p.frozen = frozen;  // deep copy
  p.identity = 0;
  p.metadata = metadata;
  int nrUnderscores = Token::extractMixfix(prefixName.code(), p.symbolInfo.mixfixSyntax);
  if (nrUnderscores == Token::PAREN_MISMATCH)
    {
      IssueWarning(LineNumber(prefixName.lineNumber()) << ": mismatched parentheses in operator " <<
		   QUOTE(prefixName) << ". It will be treated as having prefix syntax only.");
      symbolType.clearFlags(SymbolType::PREC | SymbolType::GATHER | SymbolType::FORMAT);
    }
  else if (p.symbolInfo.mixfixSyntax.empty())
    {
      p.symbolInfo.prec = 0;
      WarningCheck(!(symbolType.hasFlag(SymbolType::PREC)),
		   LineNumber(prefixName.lineNumber()) <<
		   ": prec attribute without mixfix syntax for operator " <<
		   QUOTE(prefixName) << '.');
      WarningCheck(!(symbolType.hasFlag(SymbolType::GATHER)),
		   LineNumber(prefixName.lineNumber()) <<
		   ": gather attribute without mixfix syntax for operator " <<
		   QUOTE(prefixName) << '.');
      symbolType.clearFlags(SymbolType::PREC | SymbolType::GATHER);
      if (symbolType.hasFlag(SymbolType::FORMAT))
	{
	  if (format.length() == 2)
	    p.symbolInfo.format = format;  // deep copy
	  else
	    {
	      IssueWarning(LineNumber(prefixName.lineNumber()) <<
			   ": bad format length of " <<
			   format.length() <<
			   ", should be 2.");
	      symbolType.clearFlags(SymbolType::FORMAT);
	    }
	}
    }
  else
    {
      if (nrArgs != nrUnderscores)
	{
	  IssueWarning(LineNumber(prefixName.lineNumber()) <<
		       ": number of underscores does not match (" << nrUnderscores <<
		       ") number of arguments (" << nrArgs <<
		       ") for operator " <<
		       QUOTE(prefixName) << '.');
	  p.symbolInfo.mixfixSyntax.clear();
	  p.symbolInfo.prec = 0;
	  symbolType.clearFlags(SymbolType::PREC | SymbolType::GATHER | SymbolType::FORMAT);
	}
      else
	{
	  p.symbolInfo.prec = prec;
	  p.symbolInfo.gather = gather;  // deep copy
	  if (symbolType.hasFlag(SymbolType::FORMAT))
	    {
	      if (p.symbolInfo.mixfixSyntax.length() + 1 != format.length())
		{
		  IssueWarning(LineNumber(prefixName.lineNumber()) <<
			       ": bad format length of " <<
			       format.length() <<
			       ", should be " <<
			       p.symbolInfo.mixfixSyntax.length() + 1 << '.');
		  symbolType.clearFlags(SymbolType::FORMAT);
		}
	      else
		p.symbolInfo.format = format;  // deep copy
	    }
	}
    }
  p.symbolInfo.latexMacro = latexMacro;  // HACK
  p.symbolInfo.rpo = rpo;
  p.symbolInfo.polymorphIndex = nrPolymorphs;  // our own index
  p.symbolInfo.symbolType = symbolType;
  p.symbolInfo.next = NONE;
  {
    p.symbolInfo.iflags = ADHOC_OVERLOADED | DOMAIN_OVERLOADED;
    for (int i = 0; i < nrArgs; i++)
      {
	if (domainAndRange[i] == 0)
	  {
	    p.symbolInfo.iflags &= ~DOMAIN_OVERLOADED;
	    break;
	  }
      }
    if (domainAndRange[nrArgs] != 0)
      p.symbolInfo.iflags |= RANGE_OVERLOADED;
  }
  return nrPolymorphs;
}

int
MixfixModule::addStrategy(Token name,
			  const Vector<Sort*>& domainSorts,
			  Sort* subjectSort,
			  int metadata,
			  bool imported)
{
  // Prepares a domain components vector to create an internal tuple symbol
  // as the auxiliary symbol for matching strategy calls in definitions
  int nrArgs = domainSorts.length();
  Vector<ConnectedComponent*> domainComponents(nrArgs);
  for (int i = 0; i < nrArgs; i++)
    domainComponents[i] = domainSorts[i]->component();

  Symbol* auxSymbol = createInternalTupleSymbol(domainComponents, strategyRangeSort->component());
  RewriteStrategy* strat = new RewriteStrategy(name.code(), domainSorts, subjectSort, auxSymbol);
  strat->setLineNumber(name.lineNumber());

  // Checks if there is already an strategy with the same name
  // and connected components in the module
  RewriteStrategy* other = findStrategy(name.code(), domainComponents);

  insertStrategy(strat);

  if (other != 0)
    {
      if (imported)
	IssueWarning(*this <<
	  ": strategy declaration " << QUOTE(strat) << " from " << *strat <<
	  " conflicts with " << QUOTE(other) << " from " << *other << ".");
      else
	IssueWarning(LineNumber(name.lineNumber()) <<
	  ": strategy declaration " << QUOTE(strat) << " conflicts with " <<
	  QUOTE(other) << " from " << *other << ".");
    }

  if (metadata != NONE)
    insertMetadata(STRAT_DECL, strat, metadata);

  return getStrategies().size() - 1;
}

int
MixfixModule::addBubbleSpec(Symbol* topSymbol,
			    int lowerBound,
			    int upperBound,
			    int leftParenToken,
			    int rightParenToken,
			    const Vector<int>& excludedTokens)
{
  int nrBubbleSpecs = bubbleSpecs.length();
  bubbleSpecs.expandBy(1);
  BubbleSpec& b = bubbleSpecs[nrBubbleSpecs];
  b.topSymbol = topSymbol;
  b.lowerBound = lowerBound;
  b.upperBound = upperBound;
  b.leftParenToken = leftParenToken;
  b.rightParenToken = rightParenToken;
  b.excludedTokens = excludedTokens;
  const Vector<Sort*>& domainAndRange =
    topSymbol->getOpDeclarations()[0].getDomainAndRange();
  b.componentIndex =
    domainAndRange[domainAndRange.length() - 1]->component()->getIndexWithinModule();
  bubbleComponents.insert(b.componentIndex);
  return nrBubbleSpecs;
}

void
MixfixModule::handleSortConstraint(SortConstraint* sortConstraint, bool dnt)
{
  if (!dnt && statementTransformer != 0)
    {
      StatementTransformer::Outcome result = statementTransformer->transformSortConstraint(sortConstraint);
      AdvisoryCheck(result != StatementTransformer::Outcome::NOT_TRANSFORMED,
		    *sortConstraint << ": transformation not performed for:\n  " << sortConstraint);
    }
  //
  //	Any transformation has taken place so its safe to analyze sortConstraint.
  //
  checkSortConstraint(sortConstraint);
}

void
MixfixModule::handleEquation(Equation* equation, bool dnt)
{
  if (!dnt && statementTransformer != 0)
    {
      StatementTransformer::Outcome result = statementTransformer->transformEquation(equation);
      AdvisoryCheck(result != StatementTransformer::Outcome::NOT_TRANSFORMED,
		    *equation << ": transformation not performed for:\n  " << equation);
    }
  //
  //	Any transformation has taken place so its safe to analyze equation.
  //
  checkEquation(equation);
}

void
MixfixModule::handleRule(Rule* rule, bool dnt)
{
  if (!dnt && statementTransformer != 0)
    {
      StatementTransformer::Outcome result = statementTransformer->transformRule(rule);
      AdvisoryCheck(result != StatementTransformer::Outcome::NOT_TRANSFORMED,
		    *rule << ": transformation not performed for:\n  " << rule);
    }
  //
  //	Any transformation has taken place so its safe to analyze rule.
  //
  checkRule(rule);
}
