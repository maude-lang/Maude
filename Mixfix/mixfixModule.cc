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
//      Implementation for class MixfixModule.
//

//      utility stuff
#include "macros.hh"
#include "vector.hh"
#include "intSet.hh"
#include "pointerSet.hh"
#include "tty.hh"

//      forward declarations
#include "interface.hh"
#include "core.hh"
#include "variable.hh"
#include "higher.hh"
#include "freeTheory.hh"
#include "AU_Theory.hh"
#include "ACU_Theory.hh"
#include "CUI_Theory.hh"
#include "S_Theory.hh"
#include "NA_Theory.hh"
#include "builtIn.hh"
#include "temporal.hh"
#include "higher.hh"
#include "objectSystem.hh"
#include "meta.hh"
#include "mixfix.hh"

//      interface class definitions
#include "symbol.hh"
#include "dagNode.hh"
#include "term.hh"

//      core class definitions
#include "lineNumber.hh"
#include "argumentIterator.hh"
#include "dagArgumentIterator.hh"
#include "equation.hh"
#include "rule.hh"
#include "sortConstraint.hh"
#include "conditionFragment.hh"
#include "symbolMap.hh"

//	variable class definitions
#include "variableSymbol.hh"
#include "variableTerm.hh"
#include "variableDagNode.hh"

//      free theory class definitions
#include "freeSymbol.hh"

//      AU theory class definitions
#include "AU_Symbol.hh"

//      ACU theory class definitions
#include "ACU_Symbol.hh"

//      CUI theory class definitions
#include "CUI_Symbol.hh"

//      S theory class definitions
#include "S_Symbol.hh"
#include "S_DagNode.hh"
#include "S_Term.hh"

//	builtin class definitions
#include "branchSymbol.hh"
#include "equalitySymbol.hh"
#include "sortTestSymbol.hh"
#include "floatTerm.hh"
#include "floatSymbol.hh"
#include "floatDagNode.hh"
#include "floatOpSymbol.hh"
#include "stringSymbol.hh"
#include "stringTerm.hh"
#include "stringDagNode.hh"
#include "stringOpSymbol.hh"
#include "succSymbol.hh"
#include "minusSymbol.hh"
#include "numberOpSymbol.hh"
#include "ACU_NumberOpSymbol.hh"
#include "CUI_NumberOpSymbol.hh"
#include "divisionSymbol.hh"

//	higher class definitions
#include "rewriteSequenceSearch.hh"
#include "modelCheckerSymbol.hh"
#include "satSolverSymbol.hh"
#include "equalityConditionFragment.hh"
#include "sortTestConditionFragment.hh"
#include "assignmentConditionFragment.hh"
#include "rewriteConditionFragment.hh"

//	object system class definitions
#include "configSymbol.hh"

//	front end class definitions
#include "mixfixParser.hh"
#include "mixfixModule.hh"
#include "quotedIdentifierSymbol.hh"
#include "quotedIdentifierOpSymbol.hh"
#include "quotedIdentifierTerm.hh"
#include "quotedIdentifierDagNode.hh"
#include "metaLevelOpSymbol.hh"
#include "loopSymbol.hh"
#include "userLevelRewritingContext.hh"

#include "interpreter.hh"
#include "main.hh"  // HACK shouldn't be accessing global variables

Vector<int> MixfixModule::emptyGather;
Vector<int> MixfixModule::gatherAny(1);
Vector<int> MixfixModule::gatherAnyAny(2);
Vector<int> MixfixModule::gatherAnyAnyAny(3);
Vector<int> MixfixModule::gatherPrefix(1);
Vector<int> MixfixModule::gatherPrefixPrefix(2);
Vector<int> MixfixModule::gatherAny0(2);

inline int
MixfixModule::domainComponentIndex(const Symbol* symbol, int argNr)
{
  return symbol->domainComponent(argNr)->getIndexWithinModule();
}

inline int
MixfixModule::nonTerminal(int componentIndex, NonTerminalType type)
{
  return componentNonTerminalBase - componentIndex * NUMBER_OF_TYPES - type;
}

inline int
MixfixModule::iterSymbolNonTerminal(int iterSymbolIndex)
{
  return componentNonTerminalBase - 1 - iterSymbolIndex;
}

//	our stuff
#include "makeGrammar.cc"
#include "doParse.cc"
#include "entry.cc"
#include "prettyPrint.cc"
#include "sharedPrint.cc"
#include "termPrint.cc"
#include "dagNodePrint.cc"
#include "bufferPrint.cc"
#include "graphPrint.cc"

void
MixfixModule::SymbolInfo::revertGather(Vector<int>& gatherSymbols) const
{
  int nrElts = gather.length();
  gatherSymbols.resize(nrElts);
  for (int i = 0; i < nrElts; i++)
    {
      int d = gather[i] - prec;
      gatherSymbols[i] = ((d < 0) ? GATHER_e : ((d == 0) ? GATHER_E : GATHER_AMP));
    }
}

MixfixModule::MixfixModule(int name, ModuleType moduleType)
  : ProfileModule(name), moduleType(moduleType)
{
    {
      // HACK
      gatherAny[0] = ANY;
      gatherAnyAny[0] = ANY;
      gatherAnyAny[1] = ANY;
      gatherAnyAnyAny[0] = ANY;
      gatherAnyAnyAny[1] = ANY;
      gatherAnyAnyAny[2] = ANY;
      gatherPrefix[0] = PREFIX_GATHER;
      gatherPrefixPrefix[0] = PREFIX_GATHER;
      gatherPrefixPrefix[1] = PREFIX_GATHER;
      gatherAny0[0] = ANY;
      gatherAny0[1] = 0;
    }
  trueSymbol = 0;
  falseSymbol = 0;
  parser = 0;
}

MixfixModule::~MixfixModule()
{
  int nrPolymorphs = polymorphs.length();
  for (int i = 0; i < nrPolymorphs; i++)
    {
      Vector<Term*>& specialTerms = polymorphs[i].specialTerms;
      int nrSpecialTerms = specialTerms.length();
      for (int j = 0; j < nrSpecialTerms; j++)
	{
	  if (Term* t = specialTerms[j])
	    t->deepSelfDestruct();
	}
    }
  delete parser;
}

void 
MixfixModule::closeSignature()
{
  Module::closeSignature();  // compute sort info
  //
  //	Finalize pretty print info for polymorphs.
  //
  int nrPolymorphs = polymorphs.length();
  for (int i = 0; i < nrPolymorphs; i++)
    {
      Polymorph& p = polymorphs[i];
      SymbolInfo& si = p.symbolInfo;
      if (si.mixfixSyntax.length() > 0)
	computePrecAndGather(p.domainAndRange.length() - 1, si);
    }
  //
  //	Finalize pretty print info for symbols; insert object/message
  //	info for configuration symbols.
  //
  const Vector<Symbol*>& symbols = getSymbols();
  int nrSymbols = symbols.length();
  for (int i = 0; i < nrSymbols; i++)
    {
      Symbol* s = symbols[i];
      SymbolInfo& si = symbolInfo[i];
      if (si.mixfixSyntax.length() > 0)
	computePrecAndGather(s->arity(), si, s);
      if (ConfigSymbol* cs = dynamic_cast<ConfigSymbol*>(s))
	{
	  cs->addObjects(objectSymbols);
	  cs->addMessages(messageSymbols);
	}
    }
}

void
MixfixModule::economize()
{
  delete parser;
  parser = 0;
}

Sort*
MixfixModule::findSort(int name)
{
  SortMap::const_iterator i = sortNames.find(name);
  return (i == sortNames.end()) ? 0 : (*i).second;
}

Symbol*
MixfixModule::findSymbol(int name,
			 const Vector<ConnectedComponent*>& domainComponents,
			 ConnectedComponent* rangeComponent)
{
  int nrArgs = domainComponents.length();
  int index = symbolNames.int2Index(name);
  if (index != NONE)
    {
      for (int i = firstSymbols[index]; i != NONE; i = symbolInfo[i].next)
	{
	  Symbol* s = getSymbols()[i];
	  Assert(s->id() == name, "name lookup error " <<
		 Token::name(s->id()) << " vs " << Token::name(name));
	  if (s->arity() == nrArgs)
	    {
	      const Vector<Sort*>& domainAndRange =
		s->getOpDeclarations()[0].getDomainAndRange();
	      bool sameDomain = true;
	      for (int j = 0; j < nrArgs; j++)
		{
		  if (domainComponents[j] != domainAndRange[j]->component())
		    {
		      sameDomain = false;
		      break;
		    }
		}
	      if (sameDomain == true &&
		  (rangeComponent == 0 || rangeComponent == domainAndRange[nrArgs]->component()))
		return s;
	    }
	  else if (nrArgs > 2)
	    {
	      if (symbolInfo[i].symbolType.hasFlag(SymbolType::ASSOC))
		{
		  const Vector<Sort*>& domainAndRange =
		    s->getOpDeclarations()[0].getDomainAndRange();
		  ConnectedComponent* component = domainAndRange[0]->component();
		  bool sameDomain = true;
		  for (int j = 0; j < nrArgs; j++)
		    {
		      if (domainComponents[j] != component)
			{
			  sameDomain = false;
			  break;
			}
		    }
		  if (sameDomain == true &&
		      (rangeComponent == 0 ||
		       rangeComponent == domainAndRange[nrArgs]->component()))
		    return s;
		}   
	    }
	}
    }
  //
  //	See if we can make a suitable symbol by instantiating a polymorph.
  //
  int nrPolymorphs = polymorphs.length();
  for (int i = 0; i < nrPolymorphs; i++)
    {
      Polymorph& p = polymorphs[i];
      if (p.name == name && p.domainAndRange.length() - 1 == nrArgs)
	{
	  int detArgNr = (p.symbolInfo.symbolType.getBasicType() == SymbolType::BRANCH_SYMBOL) ? 1 : 0;
	  return instantiatePolymorph(i, domainComponents[detArgNr]->getIndexWithinModule());
	}
    }
  //
  //	See if we can make a suitable symbol by instantiating a sort test.
  //
  if (trueSymbol != 0 && falseSymbol != 0 && nrArgs == 1)
    {
      const char* nameText = Token::name(name);
      if (*nameText++ == '_' && *nameText++ == ':' && *nameText++ == ':')
	{
	  bool eager = true;
	  switch (*nameText++)
	    {
	    case ':':
	      {
		if (*nameText++ != '`')
		  break;
		eager = false;
		//  fall thru
	      }
	    case '`':
	      {
		if (Sort* sort = findSort(Token::encode(nameText)))
		  return instantiateSortTest(sort, eager);
	      }
	    }
	}
    }

  /*
  cerr << "Couldn't find operator " << Token::name(name) << '\n';;
  for (int i = 0; i < nrArgs; i++)
    cerr << domainComponents[i]->sort(0) << '\n';
  if (rangeComponent != 0)
    cerr << "\nrange = " << rangeComponent->sort(0) << '\n';
  abort();
  AdvisoryCheck(false, cerr << "Couldn't find operator " << Token::name(name));
  */
  return 0;
}

Symbol*
MixfixModule::instantiateVariable(Sort* sort)
{
  int sortIndex = sort->getIndexWithinModule();
  int nrVariables = variableBase.length();
  if (nrVariables <= sortIndex)
    {
      variableBase.expandTo(sortIndex + 1);
      for (int i = nrVariables; i <= sortIndex; i++)
	variableBase[i] = 0;
    }
  Symbol* symbol = variableBase[sortIndex];
  if (symbol == 0)
    {
      symbol = new VariableSymbol(sort->id());
      static Vector<Sort*> domainAndRange(1);
      domainAndRange[0] = sort;
      symbol->addOpDeclaration(domainAndRange, true);  // variables are always constructors
      int nrSymbols = symbolInfo.length();
      symbolInfo.expandBy(1);
      SymbolInfo& si = symbolInfo[nrSymbols];
      si.prec = 0;
      si.symbolType.setBasicType(SymbolType::VARIABLE);
      si.iflags = 0;
      si.next = NONE;
      insertLateSymbol(symbol);
      variableBase[sortIndex] = symbol;
    }
  return symbol;
}

Symbol*
MixfixModule::instantiateSortTest(Sort* sort, bool eager)
{
  int sortIndex = sort->getIndexWithinModule();
  Vector<Symbol*>& sortTests = eager ? eagerSortTests : lazySortTests;
  int nrSortTests = sortTests.length();
  if (nrSortTests <= sortIndex)
    {
      sortTests.expandTo(sortIndex + 1);
      for (int i = nrSortTests; i <= sortIndex; i++)
	sortTests[i] = 0;
    }
  Symbol* symbol = sortTests[sortIndex];
  if (symbol == 0)
    {
      string opName(eager ? "_::`" : "_:::`");
      opName += Token::name(sort->id());
      int prefixCode = Token::encode(opName.c_str());
      symbol = new SortTestSymbol(prefixCode, sort, trueSymbol, falseSymbol, eager);
      static Vector<Sort*> domainAndRange(2);
      domainAndRange[0] = sort->component()->sort(0);
      domainAndRange[1] = boolSort;
      symbol->addOpDeclaration(domainAndRange, false);  // sort tests are never constructors
      int nrSymbols = symbolInfo.length();
      symbolInfo.expandBy(1);
      SymbolInfo& si = symbolInfo[nrSymbols];
      (void) Token::extractMixfix(prefixCode, si.mixfixSyntax);
      si.prec = 0;
      si.gather.append(ANY);
      si.symbolType.setBasicType(SymbolType::SORT_TEST);
      si.iflags = LEFT_BARE;
      si.next = NONE;
      insertLateSymbol(symbol);
      sortTests[sortIndex] = symbol;
    }
  return symbol;
}

Term*
MixfixModule::makeTrueTerm()
{
  static Vector<Term*> args;
  return trueSymbol->makeTerm(args);
}

Term*
MixfixModule::makeBubble(int bubbleSpecIndex, const Vector<Token>& tokens, int first, int last)
{
  Vector<Term*> args;
  BubbleSpec& b = bubbleSpecs[bubbleSpecIndex];
  for (int i = first; i <= last; i++)
    {
      int code = tokens[i].code();
      if (code == leftParen || code == rightParen ||
	  code == leftBracket || code == rightBracket ||
	  code == leftBrace || code == rightBrace ||
	  code == comma)
	{
	  static char backQuote[] = "`(";
	  backQuote[1] = Token::name(code)[0];
	  code = Token::encode(backQuote);
	}
      args.append(new QuotedIdentifierTerm(b.qidSymbol, code));
    }
  int bubbleSize = args.length();
  if (bubbleSize != b.topSymbol->arity())
    {
      Term* t = bubbleSize == 0 ? b.nilQidListSymbol->makeTerm(args):
	b.qidListSymbol->makeTerm(args);
      args.resize(1);
      args[0] = t;
    }
  return b.topSymbol->makeTerm(args);
}

Symbol*
MixfixModule::instantiatePolymorph(int polymorphIndex, int kindIndex)
{
  Polymorph& p = polymorphs[polymorphIndex];
  int nrInstantiations = p.instantiations.length();
  if (nrInstantiations <= kindIndex)
    {
      p.instantiations.expandTo(kindIndex + 1);
      for (int i = nrInstantiations; i <= kindIndex; i++)
	p.instantiations[i] = 0;
    }
  Symbol* symbol = p.instantiations[kindIndex];
  if (symbol == 0)
    {
      Sort* s = getConnectedComponents()[kindIndex]->sort(Sort::KIND);
      bool isConstructor = p.symbolInfo.symbolType.hasFlag(SymbolType::CTOR);
      if (p.symbolInfo.symbolType.getBasicType() == SymbolType::BRANCH_SYMBOL)
	{
	  symbol = new BranchSymbol(p.name, p.specialTerms);
	  p.domainAndRange[1] = s;
	  p.domainAndRange[2] = s;
	  p.domainAndRange[3] = s;
	  symbol->addOpDeclaration(p.domainAndRange, isConstructor);
	}
      else
	{
	  symbol = new EqualitySymbol(p.name, p.specialTerms[0], p.specialTerms[1], p.strategy);
	  p.domainAndRange[0] = s;
	  p.domainAndRange[1] = s;
	  symbol->addOpDeclaration(p.domainAndRange, isConstructor);
	}
      int nrSymbols = symbolInfo.length();
      symbolInfo.expandBy(1);
      symbolInfo[nrSymbols] = p.symbolInfo;  // deep copy
      insertLateSymbol(symbol);
      p.instantiations[kindIndex] = symbol;
    }
  return symbol;
}

QuotedIdentifierSymbol*
MixfixModule::findQuotedIdentifierSymbol(const ConnectedComponent* component) const
{
  map<int, Symbol*>::const_iterator i =
    quotedIdentifierSymbols.find(component->getIndexWithinModule());
  return (i == quotedIdentifierSymbols.end()) ? 0 :
    static_cast<QuotedIdentifierSymbol*>((*i).second);
}

StringSymbol*
MixfixModule::findStringSymbol(const ConnectedComponent* component) const
{
  map<int, Symbol*>::const_iterator i =
    stringSymbols.find(component->getIndexWithinModule());
  return (i == stringSymbols.end()) ? 0 :
    static_cast<StringSymbol*>((*i).second);
}

FloatSymbol*
MixfixModule::findFloatSymbol(const ConnectedComponent* component) const
{
  map<int, Symbol*>::const_iterator i =
    floatSymbols.find(component->getIndexWithinModule());
  return (i == floatSymbols.end()) ? 0 :
    static_cast<FloatSymbol*>((*i).second);
}

void
MixfixModule::fixUpPolymorph(int polymorphIndex, const Vector<Term*>& specialTerms)
{
  polymorphs[polymorphIndex].specialTerms = specialTerms;  // deep copy
}

void
MixfixModule::copyFixUpPolymorph(int polymorphIndex,
				 const MixfixModule* originalModule,
				 int originalPolymorphIndex,
				 SymbolMap* map)
{
  const Vector<Term*>& originalSpecialTerms =
    originalModule->polymorphs[originalPolymorphIndex].specialTerms;
  int nrSpecialTerms = originalSpecialTerms.length();
  Vector<Term*>& specialTerms = polymorphs[polymorphIndex].specialTerms;
  specialTerms.expandTo(nrSpecialTerms);
  for (int i = 0; i < nrSpecialTerms; i++)
    specialTerms[i] = originalSpecialTerms[i]->deepCopy(map);
}

int
MixfixModule::copyPolymorph(const MixfixModule* originalModule,
			    int originalPolymorphIndex)
{
  int nrPolymorphs = polymorphs.length();
  polymorphs.expandBy(1);
  Polymorph& p = polymorphs[nrPolymorphs];
  const Polymorph& original = originalModule->polymorphs[originalPolymorphIndex];
  p.name = original.name;
  SymbolType symbolType = original.symbolInfo.symbolType;
  if (symbolType.getBasicType() == SymbolType::BRANCH_SYMBOL)
    {
      p.domainAndRange.expandTo(4);
      p.domainAndRange[0] = findSort(original.domainAndRange[0]->id());
    }
  else
    {
      p.domainAndRange.expandTo(3);
      p.domainAndRange[2] = findSort(original.domainAndRange[2]->id());
    }
  p.strategy = original.strategy;  // deep copy
  p.symbolInfo.mixfixSyntax = original.symbolInfo.mixfixSyntax;  // deep copy
  p.symbolInfo.prec = symbolType.hasFlag(SymbolType::PREC) ? original.symbolInfo.prec : DEFAULT;
  if (symbolType.hasFlag(SymbolType::GATHER))
    original.symbolInfo.revertGather(p.symbolInfo.gather);
  if (symbolType.hasFlag(SymbolType::FORMAT))
    p.symbolInfo.gather = original.symbolInfo.format;
  p.symbolInfo.symbolType = symbolType;
  p.symbolInfo.iflags = original.symbolInfo.iflags;
  p.symbolInfo.next = NONE;
  return nrPolymorphs;
}

MixfixModule::BubbleSpec&
MixfixModule::findBubbleSpec(Symbol* topSymbol)
{
  int nrBubbleSpecs = bubbleSpecs.length();
  int i = 0;
  for (; i < nrBubbleSpecs; i++)
    {
      if (bubbleSpecs[i].topSymbol == topSymbol)
	break;
    }
  return bubbleSpecs[i];
}

void
MixfixModule::copyFixUpBubbleSpec(Symbol* newSymbol, SymbolMap* map)
{
  BubbleSpec& b = findBubbleSpec(newSymbol);
  b.qidSymbol = dynamic_cast<QuotedIdentifierSymbol*>(b.qidSymbol ?
						      map->translate(b.qidSymbol) : 0);
  b.nilQidListSymbol = b.nilQidListSymbol ? map->translate(b.nilQidListSymbol) : 0;
  b.qidListSymbol = b.qidListSymbol ? map->translate(b.qidListSymbol) : 0;
}

void
MixfixModule::copyBubbleSpec(MixfixModule* originalModule,
			     Symbol* originalSymbol,
			     Symbol* newSymbol)
{
  int nrBubbleSpecs = bubbleSpecs.length();
  bubbleSpecs.append(originalModule->findBubbleSpec(originalSymbol));  // deep copy
  BubbleSpec& b = bubbleSpecs[nrBubbleSpecs];
  b.topSymbol = newSymbol;
  const Vector<Sort*>& domainAndRange =
    newSymbol->getOpDeclarations()[0].getDomainAndRange();
  b.componentIndex =
    domainAndRange[domainAndRange.length() - 1]->component()->getIndexWithinModule();
  bubbleComponents.insert(b.componentIndex);
}

void
MixfixModule::fixUpBubbleSpec(int bubbleSpecIndex,
			      QuotedIdentifierSymbol *qidSymbol,
			      Symbol* nilQidListSymbol,
			      Symbol* qidListSymbol)
{
  BubbleSpec& b =  bubbleSpecs[bubbleSpecIndex];
  b.qidSymbol = qidSymbol;
  b.nilQidListSymbol = nilQidListSymbol;
  b.qidListSymbol = qidListSymbol;
}

int
MixfixModule::findMatchingParen(const Vector<Token>& tokens, int pos)
{
  if (tokens[pos].code() == leftParen)
    {
      int level = 1;
      int nrTokens = tokens.length();
      for (int i = pos + 1; i < nrTokens; i++)
	{
	  int code = tokens[i].code();
	  if (code == leftParen)
	    ++level;
	  else if (code == rightParen)
	    {
	      --level;
	      if (level == 0)
		return i;
	    }
	}
    }
  return pos;
}

int
MixfixModule::mayAssoc(Symbol* symbol, int argNr)
{
  const Vector<OpDeclaration>& opDecls = symbol->getOpDeclarations();
  int nrOpDecls = opDecls.length();
  if (nrOpDecls == 0)
    return UNDEFINED;
  int nrArgs = symbol->arity();
  if (opDecls[0].getDomainAndRange()[argNr]->component() !=
      opDecls[0].getDomainAndRange()[nrArgs]->component())
    return UNDEFINED;
  for (int i = 0; i < nrOpDecls; i++)
    {
      Sort* argSort = opDecls[i].getDomainAndRange()[argNr];
      for (int j = 0; j < nrOpDecls; j++)
	{
	  if (leq(opDecls[j].getDomainAndRange()[nrArgs], argSort))
	    return true;
	}
    }
  return false;
}

void
MixfixModule::computePrecAndGather(int nrArgs, SymbolInfo& si, Symbol* symbol)
{
  int nrItems = si.mixfixSyntax.length();
  if (si.mixfixSyntax[0] == underscore)
    si.iflags |= LEFT_BARE;
  if (si.mixfixSyntax[nrItems - 1] == underscore)
    si.iflags |= RIGHT_BARE;
  if (si.prec == DEFAULT)
    {
      //
      //	Compute default precedence.
      //
      si.prec = 0;
      if ((si.iflags & LEFT_BARE) || (si.iflags & RIGHT_BARE))
	si.prec = (nrArgs == 1) ? UNARY_PREC : INFIX_PREC;
    }
  if (si.gather.length() > 0)
    {
      Assert(si.gather.length() == nrArgs, "gather length != nrArgs");
      for (int i = 0; i < nrArgs; i++)
	{
	  int g = si.gather[i];
	  if (g == GATHER_AMP)
	    g = ANY;
	  else
	    {
	      g += si.prec;
	      if (g < 0)
		g = 0;
	    }
	  si.gather[i] = g;
	}
    }
  else
    {
      if (nrArgs > 0)
	{
	  //
	  //	Compute default gather pattern.
	  //
	  if (si.symbolType.hasFlag(SymbolType::ASSOC) && (si.iflags & LEFT_BARE) &&
	      (si.iflags & RIGHT_BARE) && si.prec > 0)
	    {
	      //
	      //	gather (e E) to right associate infix assoc operator.
	      //
	      si.gather.append(si.prec - 1);
	      si.gather.append(si.prec);
	    }
	  else
	    {
	      for (int i = 0; i < nrItems; i++)
		{
		  if (si.mixfixSyntax[i] == underscore)
		    {
		      if (i == 0 || si.mixfixSyntax[i - 1] == underscore ||
			  i + 1 == nrItems || si.mixfixSyntax[i + 1] == underscore)
			si.gather.append(si.prec);
		      else
			si.gather.append(ANY);
		    }
		}
	      //
	      //	Check for exception case where we bias associative grouping
	      //	according to sort structure.
	      //
	      if ((si.iflags & LEFT_BARE) && (si.iflags & RIGHT_BARE) &&
		  si.prec > 0 && symbol != 0)
		{
		  int leftAssoc = mayAssoc(symbol, 0);
		  if (leftAssoc != UNDEFINED)
		    {
		      int rightAssoc = mayAssoc(symbol, nrArgs - 1);
		      if (rightAssoc != UNDEFINED)
			{
			  if (leftAssoc)
			    {
			      if (!rightAssoc)
				si.gather[nrArgs - 1] = si.prec - 1;  // prevent right assoc
			    }
			  else
			    {
			      if (rightAssoc)
				si.gather[0] = si.prec - 1;  // prevent left assoc
			    }
			}
		    }
		}
	    }
	}
      Assert(si.gather.length() == nrArgs, "gather length != nrArgs");
    }
}
