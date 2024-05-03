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
//      Implementation for class MixfixModule.
//

//      utility stuff
#include "macros.hh"
#include "vector.hh"
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
#include "strategyLanguage.hh"
#include "mixfix.hh"
#include "SMT.hh"

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
#include "rewriteStrategy.hh"
#include "strategyDefinition.hh"

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
#include "bindingMacros.hh"
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
#include "randomOpSymbol.hh"
#include "matrixOpSymbol.hh"
#include "counterSymbol.hh"

//	SMT class definitions
#include "SMT_Info.hh"
#include "SMT_Symbol.hh"
#include "SMT_NumberSymbol.hh"
#include "SMT_NumberTerm.hh"
#include "SMT_NumberDagNode.hh"

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
#include "socketManagerSymbol.hh"
#include "fileManagerSymbol.hh"
#include "streamManagerSymbol.hh"
#include "directoryManagerSymbol.hh"
#include "processManagerSymbol.hh"
#include "timeManagerSymbol.hh"
#include "prngManagerSymbol.hh"

//	strategy language class definitions
#include "trivialStrategy.hh"
#include "applicationStrategy.hh"
#include "concatenationStrategy.hh"
#include "unionStrategy.hh"
#include "iterationStrategy.hh"
#include "branchStrategy.hh"
#include "testStrategy.hh"
#include "subtermStrategy.hh"
#include "callStrategy.hh"
#include "oneStrategy.hh"

//	metalevel class definitions
#include "metaLevelOpSymbol.hh"
#include "interpreterManagerSymbol.hh"

//	front end class definitions
#include "mixfixParser.hh"
#include "mixfixModule.hh"
#include "quotedIdentifierSymbol.hh"
#include "quotedIdentifierOpSymbol.hh"
#include "quotedIdentifierTerm.hh"
#include "quotedIdentifierDagNode.hh"
#include "loopSymbol.hh"
#include "userLevelRewritingContext.hh"
#include "freshVariableSource.hh"
#include "objectConstructorSymbol.hh"
#include "commutativeDecomposeEqualitySymbol.hh"
#include "importModule.hh"

#include "interpreter.hh"
#include "global.hh"  // HACK shouldn't be accessing global variables

Vector<int> MixfixModule::emptyGather;
Vector<int> MixfixModule::gatherAny(1);
Vector<int> MixfixModule::gatherAnyAny(2);
Vector<int> MixfixModule::gatherAnyAnyAny(3);
Vector<int> MixfixModule::gatherAny4(4);
Vector<int> MixfixModule::gatherPrefix(1);
Vector<int> MixfixModule::gatherPrefixPrefix(2);
Vector<int> MixfixModule::gatherAny0(2);

//	our stuff
#include "latexCommon.cc"
#include "latexTermPrint.cc"
#include "latexDagNodePrint.cc"
#include "makeGrammar.cc"
#include "doParse.cc"
#include "entry.cc"
#include "validateAttributes.cc"
#include "fancySymbols.cc"
#include "prettyPrint.cc"
#include "sharedPrint.cc"
#include "termPrint.cc"
#include "dagNodePrint.cc"
#include "bufferPrint.cc"
#include "graphPrint.cc"
#include "strategyPrint.cc"
#include "serialize.cc"

MixfixModule::MixfixModule(int name, ModuleType moduleType)
  : ProfileModule(name),
    moduleType(moduleType)
{
    {
      // HACK
      gatherAny[0] = ANY;
      gatherAnyAny[0] = ANY;
      gatherAnyAny[1] = ANY;
      gatherAnyAnyAny[0] = ANY;
      gatherAnyAnyAny[1] = ANY;
      gatherAnyAnyAny[2] = ANY;
      gatherAny4[0] = ANY;
      gatherAny4[1] = ANY;
      gatherAny4[2] = ANY;
      gatherAny4[3] = ANY;
      gatherPrefix[0] = PREFIX_GATHER;
      gatherPrefixPrefix[0] = PREFIX_GATHER;
      gatherPrefixPrefix[1] = PREFIX_GATHER;
      gatherAny0[0] = ANY;
      gatherAny0[1] = 0;
    }
  trueSymbol = 0;
  falseSymbol = 0;
  parser = 0;
  strategyRangeSort = 0;
  smtStatus = UNCHECKED;
}

MixfixModule::~MixfixModule()
{
  for (Polymorph& p : polymorphs)
    {
      if (p.identity != 0)
	p.identity->deepSelfDestruct();
      for (TermHook& h : p.termHooks)
	h.term->deepSelfDestruct();
    }
  delete parser;
}

void
MixfixModule::checkFreshVariableNames()
{
  DebugNew("checking module " << this);
  FreshVariableSource varSource(this);
  for (Rule* r : getRules())
    {
      if (r->isNarrowing())
	{
	  if (Term* t = r->variableNameConflict(varSource))
	    {
	      IssueWarning(*r << " : fresh variable name " << QUOTE(t) <<
			   " used in narrowing rule. Recovering by ignoring narrowing attribute.");
	      r->clearNarrowing();
	    }
	}
    }
  for (Equation* e : getEquations())
    {
      if (e->isVariant())
	{
	  if (Term* t = e->variableNameConflict(varSource))
	    {
	      IssueWarning(*e << " : fresh variable name " << QUOTE(t) <<
			   " used in variant equation. Recovering by ignoring variant attribute.");
	      e->clearVariant();
	    }
	}
    }
}

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

const char*
MixfixModule::moduleTypeString(ModuleType type)
{
  static const char* const typeStrings[] = {"fmod", "mod", "fth", "th"};

  if (type & STRATEGY)
    return type & THEORY ? "sth" : "smod";
  if (type & OBJECT_ORIENTED)
    return type & THEORY ? "oth" : "omod";

  return typeStrings[type];
}

const char*
MixfixModule::moduleEndString(ModuleType type)
{
  static const char* const typeStrings[] = {"endfm", "endm", "endfth", "endth"};

  if (type & STRATEGY)
    return type & THEORY ? "endsth" : "endsm";
  if (type & OBJECT_ORIENTED)
    return type & THEORY ? "endoth" : "endom";

  return typeStrings[type];
}

void
MixfixModule::closeSortSet()
{
  //	Create a universal type for strategies' auxiliary symbol range
  if (isStrategic())
    {
      int code = Token::encode("strategy[internal]");

      strategyRangeSort = addSort(code);
      strategyRangeSort->setLineNumber(getLineNumber());
    }

  ProfileModule::closeSortSet();
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
	  cs->addPortals(portalSymbols);
	  DebugInfo("added " << portalSymbols.size() << " portals");
	}
    }
}

void
MixfixModule::economize()
{
  DebugInfo(" this = " << this << "  parser = " << parser);
  delete parser;
  parser = 0;
}

void
MixfixModule::swapVariableAliasMap(AliasMap& other, MixfixParser*& otherParser)
{
  //
  //	We swap the variable alias map with an external one; this is
  //	intended to be temporary - the caller is expected to swap it
  //	back after some parsing or pretty-printing. This version
  //	carries the parser around as well.
  //
  variableAliases.swap(other);
  swap(parser, otherParser);
}

Sort*
MixfixModule::findSort(int name) const
{
  SortMap::const_iterator i = sortNames.find(name);
  return (i == sortNames.end()) ? 0 : (*i).second;
}

RewriteStrategy*
MixfixModule::findStrategy(int name,
			   const Vector<ConnectedComponent*>& domainComponents) const
{
  int nrArgs = domainComponents.size();

  const Vector<RewriteStrategy*>& strats = getStrategies();
  int nrStrats = strats.length();

  for (int i = 0; i < nrStrats; i++)
    {
      RewriteStrategy* strategy = strats[i];

      if (strategy->id() == name && strategy->arity() == nrArgs)
	{
	  const Vector<Sort*>& domain = strategy->getDomain();
	  int nrEquals;

	  for (nrEquals = 0; nrEquals < nrArgs; nrEquals++)
	    if (domainComponents[nrEquals] != domain[nrEquals]->component())
	      break;

	  if (nrEquals == nrArgs)
	    return strategy;
	}
    }

  return 0;
}

Symbol*
MixfixModule::findFirstUnarySymbol(int name, const ConnectedComponent* rangeComponent) const
{
  IntMap::const_iterator first = firstSymbols.find(name);
  if (first != firstSymbols.end())
    {
      for (int i = first->second; i != NONE; i = symbolInfo[i].next)
	{
	  Symbol* s = getSymbols()[i];
	  if (s->arity() == 1 && s->rangeComponent() == rangeComponent)
	    return s;
	}
    }
  return 0;
}

Symbol*
MixfixModule::findNextUnarySymbol(Symbol* previous, const ConnectedComponent* rangeComponent) const
{
  for (int i = symbolInfo[previous->getIndexWithinModule()].next; i != NONE; i = symbolInfo[i].next)
    {
      Symbol* s = getSymbols()[i];
      if (s->arity() == 1 && s->rangeComponent() == rangeComponent)
	return s;
    }
  return 0;
}

Symbol*
MixfixModule::findSymbol(int name,
			 const Vector<ConnectedComponent*>& domainComponents,
			 ConnectedComponent* rangeComponent)
{
  int nrArgs = domainComponents.length();
  IntMap::const_iterator first = firstSymbols.find(name);
  if (first != firstSymbols.end())
    {
      for (int i = first->second; i != NONE; i = symbolInfo[i].next)
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
		       rangeComponent == domainAndRange[2]->component()))
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
      if (p.name.code() == name && p.domainAndRange.length() - 1 == nrArgs)
	{
	  //
	  //	Check range component.
	  //
	  const ConnectedComponent* c = 0;
	  if (rangeComponent != 0)
	    {
	      const Sort* s = p.domainAndRange[nrArgs];
	      if (s == 0)
		c = rangeComponent;
	      else if (s->component() != rangeComponent)
		continue;
	    }
	  //
	  //	Check domain components.
	  //
	  for (int j = 0; j < nrArgs; j++)
	    {
	      const Sort* s = p.domainAndRange[j];
	      const ConnectedComponent* d = domainComponents[j];
	      if (s == 0)
		{
		  if (c == 0)
		    c = d;
		  else
		    {
		      if (c != d)
			{
			  c = 0;
			  break;
			}
		    }
		}
	      else
		{
		  if (s->component() != d)
		    {
		      c = 0;
		      break;
		    }
		}
	    }
	  if (c != 0)
	    return instantiatePolymorph(i, c->getIndexWithinModule());
	}
    }
  //
  //	See if we can make a suitable symbol by instantiating a sort test.
  //
  if (trueSymbol != 0 &&
      falseSymbol != 0 &&
      trueSymbol->rangeComponent() == falseSymbol->rangeComponent() &&
      nrArgs == 1)
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

DagNode*
MixfixModule::makeUnificationProblemDag(Vector<Term*>& lhs, Vector<Term*>& rhs)
{
  Assert(lhs.size() == rhs.size(), "size mismatch");
  int nrPairs = lhs.size();
  Vector<ConnectedComponent*> domain(nrPairs);
  Vector<DagNode*> lhsDags(nrPairs);
  Vector<DagNode*> rhsDags(nrPairs);
  for (int i = 0; i < nrPairs; ++i)
    {
      Term* l = lhs[i];
      domain[i] = l->symbol()->rangeComponent();
      l = l->normalize(true);  // we don't really need to normalize but we do need to set hash values
      lhsDags[i] = l->term2Dag();
      l->deepSelfDestruct();

      Term* r = rhs[i];
      Assert(domain[i] == r->symbol()->rangeComponent(), "domain mismatch");
      r = r->normalize(true);  // we don't really need to normalize but we do need to set hash values
      rhsDags[i] = r->term2Dag();
      r->deepSelfDestruct();
    }

  ConnectedComponent* range = domain[0];
  Vector<DagNode*> args(2);
  if (nrPairs == 1)
    {
      args[0] = lhsDags[0];
      args[1] = rhsDags[0];
    }
  else
    {
      Symbol* tupleSymbol = createInternalTupleSymbol(domain, range);
      args[0] = tupleSymbol->makeDagNode(lhsDags);
      args[1] = tupleSymbol->makeDagNode(rhsDags);
    }

  domain.resize(2);
  domain[1] = range;
  Symbol* tupleSymbol = createInternalTupleSymbol(domain, range);
  return tupleSymbol->makeDagNode(args);
}

pair<DagNode*, DagNode*>
MixfixModule::makeMatchProblemDags(Vector<Term*>& lhs, Vector<Term*>& rhs)
{
  Assert(lhs.size() == rhs.size(), "size mismatch");
  int nrPairs = lhs.size();
  Vector<ConnectedComponent*> domain(nrPairs);
  Vector<DagNode*> lhsDags(nrPairs);
  Vector<DagNode*> rhsDags(nrPairs);
  for (int i = 0; i < nrPairs; ++i)
    {
      Term* l = lhs[i];
      domain[i] = l->symbol()->rangeComponent();
      l = l->normalize(true);  // we don't really need to normalize but we do need to set hash values
      lhsDags[i] = l->term2Dag();
      l->deepSelfDestruct();

      Term* r = rhs[i];
      Assert(domain[i] == r->symbol()->rangeComponent(), "domain mismatch");
      r = r->normalize(true);  // we don't really need to normalize but we do need to set hash values
      rhsDags[i] = r->term2Dag();
      r->deepSelfDestruct();
    }
  if (nrPairs == 1)
    return pair<DagNode*, DagNode*>(lhsDags[0], rhsDags[0]);

  Symbol* tupleSymbol = createInternalTupleSymbol(domain, domain[0]);
  DagNode* pattern = tupleSymbol->makeDagNode(lhsDags);
  DagNode* subject = tupleSymbol->makeDagNode(rhsDags);
  return pair<DagNode*, DagNode*>(pattern, subject);
}
				   
Symbol*
MixfixModule::createInternalTupleSymbol(const Vector<ConnectedComponent*>& domain,
					ConnectedComponent* range)
{
  //
  //	Internal tuple symbols are made on-the-fly and are not intended to be seen by the
  //	user except perhaps during tracing.
  //
  IntList key;
  for (ConnectedComponent* c : domain)
    key.push_back(c->getIndexWithinModule());
  key.push_back(range->getIndexWithinModule());

  InternalTupleMap::iterator i = tupleSymbols.find(key);
  if (i != tupleSymbols.end())
    return i->second;
  
  int domainSize = domain.size();
  int id = Token::encode("tuple[internal]");
  Symbol* tupleSymbol = FreeSymbol::newFreeSymbol(id, domainSize);
  tupleSymbols.insert(InternalTupleMap::value_type(key, tupleSymbol));

  Vector<Sort*> domainAndRange(domainSize + 1);
  for (int i = 0; i < domainSize; ++i)
    domainAndRange[i] = domain[i]->sort(Sort::KIND);
  domainAndRange[domainSize] = range->sort(Sort::KIND);
  tupleSymbol->addOpDeclaration(domainAndRange, true);  // tuple symbols are always constructors
  //
  //	Need to record some syntactic info for symbol.
  //
  int nrSymbols = symbolInfo.length();
  symbolInfo.expandBy(1);
  SymbolInfo& si = symbolInfo[nrSymbols];

  si.prec = 0;
  si.polymorphIndex = NONE;
  si.symbolType.setBasicType(SymbolType::INTERNAL_TUPLE);
  si.iflags = 0;
  si.next = NONE;

  insertLateSymbol(tupleSymbol);
  return tupleSymbol;
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
      si.polymorphIndex = NONE;
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
      domainAndRange[0] = sort->component()->sort(Sort::KIND);
      domainAndRange[1] = boolSort;
      symbol->addOpDeclaration(domainAndRange, false);  // sort tests are never constructors
      int nrSymbols = symbolInfo.length();
      symbolInfo.expandBy(1);
      SymbolInfo& si = symbolInfo[nrSymbols];
      (void) Token::extractMixfix(prefixCode, si.mixfixSyntax);
      si.prec = 0;
      si.polymorphIndex = NONE;
      si.gather.append(ANY);
      si.symbolType.setBasicType(SymbolType::SORT_TEST);
      si.iflags = LEFT_BARE;
      si.next = NONE;
      insertLateSymbol(symbol);
      sortTests[sortIndex] = symbol;
    }
  return symbol;
}

bool
MixfixModule::domainAndRangeMatch(const Vector<Sort*>& domainAndRange1,
				  const Vector<Sort*>& domainAndRange2)
{
  int nrSorts = domainAndRange1.length();
  if (nrSorts != domainAndRange2.length())
    return false;
  for (int i = 0; i < nrSorts; i++)
    {
      Sort* s1 = domainAndRange1[i];
      Sort* s2 = domainAndRange2[i];
      if (s1 == 0)
	{
	  if (s2 != 0)
	    return false;
	}
      else
	{
	  if (s2 == 0 || s2->component() != s1->component())
	    return false;
	}
    }
  return true;
}

int
MixfixModule::findPolymorphIndex(int polymorphName, const Vector<Sort*>& domainAndRange) const
{
  //
  //	This is inefficient - but we don't expect to have many
  //	polymorphs in a given module.
  //
  int nrPolymorphs = polymorphs.size();
  for (int i = 0; i < nrPolymorphs; i++)
    {
      if (polymorphs[i].name.code() == polymorphName &&
	  domainAndRangeMatch(polymorphs[i].domainAndRange, domainAndRange))
	return i;
    }
  return NONE;
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
      SymbolType& symbolType = p.symbolInfo.symbolType;
      Vector<Sort*> domainAndRange(p.domainAndRange);
      {
	Sort* s = getConnectedComponents()[kindIndex]->sort(Sort::KIND);
	int len = domainAndRange.length();
	for (int i = 0; i < len; i++)
	  {
	    if (domainAndRange[i] == 0)
	      domainAndRange[i] = s;
	  }
      }
      symbol = newFancySymbol(p.name, domainAndRange, symbolType, p.strategy);
      if (symbol == 0)
	{
	  
	  symbol = FreeSymbol::newFreeSymbol(p.name.code(),
					     domainAndRange.length() - 1,
					     p.strategy,
					     symbolType.hasFlag(SymbolType::MEMO));
	  symbolType.clearFlags(SymbolType::AXIOMS | SymbolType::ITER);
	}
      symbol->setLineNumber(p.name.lineNumber());
      symbol->addOpDeclaration(domainAndRange, symbolType.hasFlag(SymbolType::CTOR));
      //
      //	Deal with fixups.
      //
      if (p.identity != 0)
	{
	  if (BinarySymbol* b = dynamic_cast<BinarySymbol*>(symbol))
	    b->setIdentity(p.identity->deepCopy());
	}
      if (symbolType.hasAttachments())
	{
	  {
	    int nrIdHooks = p.idHooks.length();
	    for (int i = 0; i < nrIdHooks; i++)
	      {
		const IdHook& idHook = p.idHooks[i];
		const char* purpose = Token::name(idHook.purpose);
		int len = idHook.data.length();
		Vector<const char*> data(len);
		for (int j = 0; j < len; j++)
		  data[j] = Token::name(idHook.data[j]);
		if (!(symbol->attachData(domainAndRange, purpose, data)))
		  ;  // HACK
	      }
	  }
	  {
	    int nrOpHooks = p.opHooks.length();
	    for (int i = 0; i < nrOpHooks; i++)
	      {
		const char* purpose = Token::name(p.opHooks[i].purpose);
		if (!(symbol->attachSymbol(purpose, p.opHooks[i].symbol)))
		  ;  // HACK
	      }
	  }
	  {
	    int nrTermHooks = p.termHooks.length();
	    for (int i = 0; i < nrTermHooks; i++)
	      {
		const char* purpose = Token::name(p.termHooks[i].purpose);
		Term* copy = p.termHooks[i].term->deepCopy();
		if (!(symbol->attachTerm(purpose, copy)))
		  ;  // HACK
	      }
	  }
	}
      
      int nrSymbols = symbolInfo.length();
      symbolInfo.expandBy(1);
      symbolInfo[nrSymbols] = p.symbolInfo;  // deep copy, including polymorphIndex
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

SMT_NumberSymbol*
MixfixModule::findSMT_NumberSymbol(const ConnectedComponent* component, SMT_Info::SMT_Type type)
{
  map<int, Symbol*>::const_iterator i =
    SMT_NumberSymbols.find(component->getIndexWithinModule());
  if ((i == SMT_NumberSymbols.end()))
    return 0;
  //
  //	Found an SMT_NumberSymbol; need to check if it is the right type.
  //
  Symbol* symbol = (*i).second;
  Sort* sort = symbol->getRangeSort();
  SMT_Info::SMT_Type t = getSMT_Info().getType(sort);
  Assert(t != SMT_Info::NOT_SMT, "bad SMT sort " << sort);
  return (t == type) ? safeCast(SMT_NumberSymbol*, symbol) : 0;
}

void
MixfixModule::addIdentityToPolymorph(int polymorphIndex,
				     Term* identity)
{
  polymorphs[polymorphIndex].identity = identity;
}

void
MixfixModule::addIdHookToPolymorph(int polymorphIndex,
				   int purpose,
				   const Vector<int>& data)
{
  Vector<IdHook>& idHooks = polymorphs[polymorphIndex].idHooks;
  int nrIdHooks = idHooks.length();
  idHooks.resize(nrIdHooks + 1);
  IdHook& idHook = idHooks[nrIdHooks];
  idHook.purpose = purpose;
  idHook.data = data;  // deep copy
}

void
MixfixModule::addOpHookToPolymorph(int polymorphIndex,
				   int purpose,
				   Symbol* symbol)
{
  Vector<OpHook>& opHooks = polymorphs[polymorphIndex].opHooks;
  int nrOpHooks = opHooks.length();
  opHooks.resize(nrOpHooks + 1);
  OpHook& opHook = opHooks[nrOpHooks];
  opHook.purpose = purpose;
  opHook.symbol = symbol;
}

void
MixfixModule::addTermHookToPolymorph(int polymorphIndex,
				     int purpose,
				     Term* term)
{
  Vector<TermHook>& termHooks = polymorphs[polymorphIndex].termHooks;
  int nrTermHooks = termHooks.length();
  termHooks.resize(nrTermHooks + 1);
  TermHook& termHook = termHooks[nrTermHooks];
  termHook.purpose = purpose;
  termHook.term = term;
}

bool
MixfixModule::getPolymorphDataAttachment(int index,
					 int nr,
					 int& purpose,
					 Vector<int>& items) const
{
  const Vector<IdHook>& idHooks = polymorphs[index].idHooks;
  if (nr < idHooks.length())
    {
      const IdHook& idHook = idHooks[nr];
      purpose = idHook.purpose;
      items = idHook.data;  // deep copy
      return true;
    }
  return false;
}

bool
MixfixModule::getPolymorphSymbolAttachment(int index,
					   int nr,
					   int& purpose,
					   Symbol*& op) const
{
  const Vector<OpHook>& opHooks = polymorphs[index].opHooks;
  if (nr < opHooks.length())
    {
      const OpHook& opHook = opHooks[nr];
      purpose = opHook.purpose;
      op = opHook.symbol;
      return true;
    }
  return false;
}

bool
MixfixModule::getPolymorphTermAttachment(int index,
					 int nr,
					 int& purpose,
					 Term*& term) const
{
  const Vector<TermHook>& termHooks = polymorphs[index].termHooks;
  if (nr < termHooks.length())
    {
      const TermHook& termHook = termHooks[nr];
      purpose = termHook.purpose;
      term = termHook.term;
      return true;
    }
  return false;
}

void
MixfixModule::copyFixUpPolymorph(int polymorphIndex,
				 const MixfixModule* originalModule,
				 int originalPolymorphIndex,
				 SymbolMap* map)
{
  {
    Term* identity =  originalModule->polymorphs[originalPolymorphIndex].identity;
    polymorphs[polymorphIndex].identity = (identity == 0) ? 0 :
      identity->deepCopy(map);
  }
  {
    const Vector<IdHook>& originalIdHooks =
      originalModule->polymorphs[originalPolymorphIndex].idHooks;
    int nrIdHooks = originalIdHooks.length();
    Vector<IdHook>& idHooks = polymorphs[polymorphIndex].idHooks;
    idHooks.resize(nrIdHooks);
    for (int i = 0; i < nrIdHooks; i++)
      idHooks[i] = originalIdHooks[i];  // deep copy
  }
  {
    const Vector<OpHook>& originalOpHooks =
      originalModule->polymorphs[originalPolymorphIndex].opHooks;
    int nrOpHooks = originalOpHooks.length();
    Vector<OpHook>& opHooks = polymorphs[polymorphIndex].opHooks;
    opHooks.resize(nrOpHooks);
    for (int i = 0; i < nrOpHooks; i++)
      {
	opHooks[i].purpose = originalOpHooks[i].purpose;
	opHooks[i].symbol = map->translate(originalOpHooks[i].symbol);
      }
  }
  {
    const Vector<TermHook>& originalTermHooks =
      originalModule->polymorphs[originalPolymorphIndex].termHooks;
    int nrTermHooks = originalTermHooks.length();
    Vector<TermHook>& termHooks = polymorphs[polymorphIndex].termHooks;
    termHooks.resize(nrTermHooks);
    for (int i = 0; i < nrTermHooks; i++)
      {
	termHooks[i].purpose = originalTermHooks[i].purpose;
	termHooks[i].term = originalTermHooks[i].term->deepCopy(map);
      }
  }
}

int
MixfixModule::findBubbleSpecIndex(Symbol* topSymbol) const
{
  int nrBubbleSpecs = bubbleSpecs.length();
  int i = 0;
  for (; i < nrBubbleSpecs; i++)
    {
      if (bubbleSpecs[i].topSymbol == topSymbol)
	break;
    }
  return i;
}

void
MixfixModule::copyFixUpBubbleSpec(Symbol* originalSymbol, SymbolMap* map)
{
  MixfixModule* originalModule = safeCast(MixfixModule*, originalSymbol->getModule());
  BubbleSpec& ob = originalModule->
    bubbleSpecs[originalModule->findBubbleSpecIndex(originalSymbol)];
  Symbol* newSymbol = map->translate(originalSymbol);
  BubbleSpec& b = bubbleSpecs[findBubbleSpecIndex(newSymbol)];

  if (b.qidSymbol == 0 && ob.qidSymbol != 0)
    b.qidSymbol = safeCast(QuotedIdentifierSymbol*, map->translate(ob.qidSymbol));
  if (b.nilQidListSymbol == 0 && ob.nilQidListSymbol != 0)
    b.nilQidListSymbol = map->translate(ob.nilQidListSymbol);
  if (b.qidListSymbol == 0 && ob.qidListSymbol != 0)
    b.qidListSymbol = map->translate(ob.qidListSymbol);
}

void
MixfixModule::copyBubbleSpec(Symbol* originalSymbol, Symbol* newSymbol)
{
  MixfixModule* originalModule = safeCast(MixfixModule*, originalSymbol->getModule());
  BubbleSpec& ob = originalModule->
    bubbleSpecs[originalModule->findBubbleSpecIndex(originalSymbol)];

  int nrBubbleSpecs = bubbleSpecs.length();
  bubbleSpecs.append(ob);  // deep copy
  BubbleSpec& b = bubbleSpecs[nrBubbleSpecs];
  b.topSymbol = newSymbol;
  b.qidSymbol = 0;
  b.nilQidListSymbol = 0;
  b.qidListSymbol = 0;
  const Vector<Sort*>& domainAndRange =
    newSymbol->getOpDeclarations()[0].getDomainAndRange();
  b.componentIndex =
    domainAndRange[domainAndRange.length() - 1]->component()->getIndexWithinModule();
  bubbleComponents.insert(b.componentIndex);
}

void
MixfixModule::getDataAttachments(Symbol* symbol,
				 const Vector<Sort*>& opDeclaration,
				 Vector<const char*>& purposes,
				 Vector<Vector<const char*> >& data) const
{
  if (symbol == trueSymbol)
    {
      APPEND_DATA(purposes, data, SystemTrue);
    }
  else if (symbol == falseSymbol)
    {
      APPEND_DATA(purposes, data, SystemFalse);
    }
  else
    {
      switch (getSymbolType(symbol).getBasicType())
	{
	case SymbolType::BUBBLE:
	  {
	    const BubbleSpec& b = bubbleSpecs[findBubbleSpecIndex(symbol)];
	    {
	      purposes.append("Bubble");
	      int nrAttachments = data.length();
	      data.resize(nrAttachments + 1);
	      Vector<const char*>& items = data[nrAttachments];
	      items.append(Token::name(Token::encode(int64ToString(b.lowerBound))));
	      items.append(Token::name(Token::encode(int64ToString(b.upperBound))));
	      if (b.leftParenToken != NONE)
		{
		  items.append(Token::name(b.leftParenToken));
		  items.append(Token::name(b.rightParenToken));
		}
	    }
	    int nrExcluded = b.excludedTokens.length();
	    if (nrExcluded > 0)
	      {
		purposes.append("Exclude");
		int nrAttachments = data.length();
		data.resize(nrAttachments + 1);
		Vector<const char*>& items = data[nrAttachments];
		items.resize(nrExcluded);
		for (int i = 0; i < nrExcluded; i++)
		  items[i] = Token::name(b.excludedTokens[i]);
	      }
	    break;
	  }
	case SymbolType::FLOAT:
	  {
	    APPEND_DATA(purposes, data, FloatSymbol);
	    break;
	  }
	case SymbolType::STRING:
	  {
	    APPEND_DATA(purposes, data, StringSymbol);
	    break;
	  }
	default:
	  symbol->getDataAttachments(opDeclaration, purposes, data);
	}
    }
}

void
MixfixModule::getSymbolAttachments(Symbol* symbol,
				   Vector<const char*>& purposes,
				   Vector<Symbol*>& symbols) const
{
  if (getSymbolType(symbol).getBasicType() == SymbolType::BUBBLE)
    {
      const BubbleSpec& b = bubbleSpecs[findBubbleSpecIndex(symbol)];
      if (b.qidSymbol != 0)
	{
	  purposes.append("qidSymbol");
	  symbols.append(b.qidSymbol);
	}
      if (b.nilQidListSymbol != 0)
	{
	  purposes.append("nilQidListSymbol");
	  symbols.append(b.nilQidListSymbol);
	}
      if (b.qidListSymbol != 0)
	{
	  purposes.append("qidListSymbol");
	  symbols.append(b.qidListSymbol);
	}
    }
  else
    symbol->getSymbolAttachments(purposes, symbols);
}

void
MixfixModule::getTermAttachments(Symbol* symbol,
				 Vector<const char*>& purposes,
				 Vector<Term*>& terms) const
{
  symbol->getTermAttachments(purposes, terms);
}

void
MixfixModule::fixUpBubbleSpec(int bubbleSpecIndex,
			      Symbol* qidSymbol,
			      Symbol* nilQidListSymbol,
			      Symbol* qidListSymbol)
{
  BubbleSpec& b =  bubbleSpecs[bubbleSpecIndex];
  b.nilQidListSymbol = nilQidListSymbol;
  b.qidListSymbol = qidListSymbol;
  //
  //	Check that needed symbols exist and have the right properties/arities/sorts.
  //
  if (qidSymbol == 0)
    {
      IssueWarning(*(b.topSymbol) << ": qidSymbol hook needed for bubble.");
      markAsBad();
      return;
    }
  b.qidSymbol = dynamic_cast<QuotedIdentifierSymbol*>(qidSymbol);
  if (b.qidSymbol == 0)
    {
      IssueWarning(*(b.topSymbol) << ": inappropriate symbol " << QUOTE(qidSymbol) <<
		   " for qidSymbol hook.");
      markAsBad();
      return;
    }
  if (b.lowerBound < 1)
    {
      if (nilQidListSymbol == 0)
	{
	  IssueWarning(*(b.topSymbol) << ": nilQidListSymbol hook needed for bubble.");
	  markAsBad();
	}
      else
	{
	  if (nilQidListSymbol->arity() != 0 ||  // not perfect - might be special symbol
	      nilQidListSymbol->rangeComponent() != qidSymbol->rangeComponent())
	    {
	      IssueWarning(*(b.topSymbol) << ": inappropriate symbol " << QUOTE(nilQidListSymbol) <<
			   " for nilQidListSymbol hook.");
	      markAsBad();
	    }
	}
    }
  if (b.upperBound > 1)
    {
      if (qidListSymbol == 0)
	{
	  IssueWarning(*(b.topSymbol) << ": qidListSymbol hook needed for bubble.");
	  markAsBad();
	  return;
	}
      if (getSymbolType(qidListSymbol).hasFlag(SymbolType::ASSOC))
	{
	  if (qidListSymbol->rangeComponent() == qidSymbol->rangeComponent())
	    return;  // OK
	}
      else
	{
	  int nrArgs = qidListSymbol->arity();
	  if (b.upperBound == nrArgs &&
	      (b.upperBound == b.lowerBound ||
	       (b.upperBound == 2 && qidListSymbol->rangeComponent() == qidSymbol->rangeComponent())))
	    {
	      for (int i = 0; i < nrArgs; i++)
		{
		  if (qidListSymbol->domainComponent(i) != qidSymbol->rangeComponent())
		    {
		      IssueWarning(*(b.topSymbol) << ": bad domain kind in symbol " <<
				   QUOTE(qidListSymbol) << " for qidListSymbol hook.");
		      markAsBad();
		      return;
		    }
		}
	      return;  // OK
	    }
	}
      IssueWarning(*(b.topSymbol) << ": inappropriate symbol " << QUOTE(qidListSymbol) <<
		   " for qidListSymbol hook.");
      markAsBad();
    }
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

const SMT_Info&
MixfixModule::getSMT_Info()
{
  if (smtInfo.getConjunctionOperator() == 0)  // HACK - we could have a problem if an SMT signature didn't have a conjunction operator
    {
      for (Symbol* s : getSymbols())
 	{
	  if (SMT_Symbol* ss = dynamic_cast<SMT_Symbol*>(s))
	    ss->fillOutSMT_Info(smtInfo);
	  else if (SMT_NumberSymbol* sns = dynamic_cast<SMT_NumberSymbol*>(s))
	    sns->fillOutSMT_Info(smtInfo);
	}
      //
      //	Make pretty printer aware of number sorts whose ASCII representation might cause
      //	ambiguous syntax.
      //
      for (Sort* sort : getSorts())
	{
	  switch (smtInfo.getType(sort))
	    {
	    case SMT_Info::INTEGER:
	      {
		int kindIndex = sort->component()->getIndexWithinModule();
		pair<set<int>::iterator, bool> p = kindsWithSucc.insert(kindIndex);
		if (!(p.second))
		  {
		    IssueWarning(LineNumber(sort->getLineNumber()) <<
				 ": multiple sets of constants that look like integers in same kind will cause pretty printing problems.");
		  }
		//
		//	Only built-ins we care about so far that have a minus will look like integers so no need to issue
		//	an additional warning. But we do need to record the kind so that minus symbol disambiguation will work.
		//
		kindsWithMinus.insert(kindIndex);
		//
		//	SMT Integers have an implicit zero constant rather than an explicit zero constant, so we need to record that
		//	so disambiguation of zero will work correctly.
		//
		kindsWithZero.insert(kindIndex);
		break;
	      }
	    case SMT_Info::REAL:
	      {
		int kindIndex = sort->component()->getIndexWithinModule();
		pair<set<int>::iterator, bool> p = kindsWithDivision.insert(kindIndex);
		if (!(p.second))
		  {
		    IssueWarning(LineNumber(sort->getLineNumber()) <<
				 ": multiple sets of constants that look like rational numbers in same kind will cause pretty printing problems.");
		  }
		//
		//	We don't record it in kindsWithMinus at the moment since REALs are always printed with a division symbol
		//	and can only be confused with other things having a division symbol.
		//
		break;
	      }
	    default:
	      break;
	    }
	}
    }
  return smtInfo;
}

int
MixfixModule::getSMT_NumberToken(const mpq_class& value, Sort* sort)
{
  //
  //	Figure out what SMT sort we correspond to.
  //
  SMT_Info::SMT_Type t = getSMT_Info().getType(sort);
  Assert(t != SMT_Info::NOT_SMT, "bad SMT sort " << sort);
  string name = value.get_num().get_str();
  if (t == SMT_Info::REAL)
    {
      name += '/';
      name += value.get_den().get_str();
    }
  else
    Assert(t == SMT_Info::INTEGER, "SMT number sort expected");
  return Token::encode(name.c_str());
}

bool
MixfixModule::validForSMT_Rewriting()
{
  if (smtStatus != UNCHECKED)
    return smtStatus == GOOD;

  bool ok = true;
  if (!getEquations().empty())
    {
      IssueWarning(*this << ": module " << QUOTE(this) << " has equations.");
      ok = false;
    }
  
  if (!getSortConstraints().empty())
    {
      IssueWarning(*this << ": module " << QUOTE(this) << " has membership axioms.");
      ok = false;
    }

  const Vector<Rule*>& rules = getRules();
  if (rules.empty())
    {
      IssueWarning(*this << ": module " << QUOTE(this) << " has no rules.");
      ok = false;
    }

  const SMT_Info& info = getSMT_Info();
  if (info.getConjunctionOperator() == 0)
    {
      IssueWarning(*this << ": module " << QUOTE(this) << " has no SMT conjunction operator.");
      ok = false;
    }

  const Vector<Symbol*>& symbols = getSymbols();
  {
    //
    //	Check for regular symbols with a collapse axiom.
    //
    int nrSymbols = symbolInfo.size();
    for (int i = 0; i < nrSymbols; ++i)
      {
	if (symbolInfo[i].symbolType.hasAtLeastOneFlag(SymbolType::COLLAPSE))
	  {
	    Symbol* s = symbols[i];
	    IssueWarning(*s << ": operator " << QUOTE(s) << " has a collapse axiom.");
	    ok = false;
	  }
      }
  }
  {
    //
    //	Check for polymorphic symbols with a collapse axiom.
    //
    int nrPolymorphs = polymorphs.size();
    for (int i = 0; i < nrPolymorphs; ++i)
      {
	if (polymorphs[i].symbolInfo.symbolType.hasAtLeastOneFlag(SymbolType::COLLAPSE))
	  {
	    Token& name = polymorphs[i].name;
	    IssueWarning(LineNumber(name.lineNumber()) << ": polymorphic operator " <<
			 QUOTE(name) << " has a collapse axiom.");
	    ok = false;
	  }
      }
  }

  NatSet smtSortIndices;
  Index nrSymbols = symbols.size();
  //
  //	Determine SMT sorts.
  //
  //	A sort is considered an SMT sort if any SMT operator has it in it's domain
  //	or range, since the SMT signature is considered to be a subsignature.
  //
  for (Index i = 0; i < nrSymbols; ++i)
    {
      int basicType = symbolInfo[i].symbolType.getBasicType();
      if (basicType == SymbolType::SMT_SYMBOL || basicType == SymbolType::SMT_NUMBER_SYMBOL)
	{
	  for (const OpDeclaration& opDecl : symbols[i]->getOpDeclarations())
	    {
	      for (const Sort* sort : opDecl.getDomainAndRange())
		smtSortIndices.insert(sort->getIndexWithinModule());
	    }
	}
    }
  //
  //	Check that no non-SMT operator has an SMT sort as its range.
  //
  for (Index i = 0; i < nrSymbols; ++i)
    {
      int basicType = symbolInfo[i].symbolType.getBasicType();
      if (basicType != SymbolType::SMT_SYMBOL &&
	  basicType != SymbolType::SMT_NUMBER_SYMBOL &&
	  basicType != SymbolType::VARIABLE)
	{
	  Symbol* s = symbols[i];
	  for (const OpDeclaration& opDecl : s->getOpDeclarations())
	    {
	      const Vector<Sort*>& domainAndRange = opDecl.getDomainAndRange();
	      Sort* rangeSort = domainAndRange[domainAndRange.size() - 1];
	      if (smtSortIndices.contains(rangeSort->getIndexWithinModule()))
		{
		  IssueWarning(*s << ": non-SMT operator " << QUOTE(s) <<
			       " has an SMT sort " << QUOTE(rangeSort) << " as its range.");
		  ok = false;
		}
	    }
	}
    }
  //
  //	Check each rule. The left-hand side may not contain SMT operators or nonlinear variables.
  //
  for (Rule* rule : getRules())
    {
      Term* lhs = rule->getLhs();
      if (Symbol* s = findSMT_Symbol(lhs))
	{
	  IssueWarning(*rule << ": left-hand side of rule\n  " << rule <<
		       "\ncontains SMT symbol " << QUOTE(s) << ".");
	  ok = false;
	}
      NatSet variableIndices;
      if (Term* v = findNonlinearVariable(lhs, variableIndices))
	{
	  IssueWarning(*rule << ": left-hand side of rule\n  " << rule <<
		       "\ncontains a nonlinear variable " << QUOTE(v) << ".");
	  ok = false;
	}
    }
  //
  //	Should probably check for true so we know that sort Boolean is determined. Or maybe conjunction operator should determine true sort.
  //

  //
  //	Need to check for each rule that lhs only contains no SMT operators and condition contains only SMT operators and variables.
  //	Should also check that each condition fragment is of the form <Boolean term> = true or a suitable SMT equality operator exists.
  //
  smtStatus = ok ? GOOD : BAD;
  return ok;  // might want to cache this when computing it becomes more expensive
}

Symbol*
MixfixModule::findSMT_Symbol(Term* term)
{
  Symbol* s = term->symbol();
  int basicType = symbolInfo[s->getIndexWithinModule()].symbolType.getBasicType();
  if (basicType == SymbolType::SMT_SYMBOL || basicType == SymbolType::SMT_NUMBER_SYMBOL)
    return s;

  for (ArgumentIterator a(*term); a.valid(); a.next())
    {
      if (Symbol* s = findSMT_Symbol(a.argument()))
	return s;
    }
  return 0;
}

Term* 
MixfixModule::findNonlinearVariable(Term* term, NatSet& seenIndices)
{
  if (VariableTerm* v = dynamic_cast<VariableTerm*>(term))
    {
      int index = v->getIndex();
      if (seenIndices.contains(index))
	return v;
      seenIndices.insert(index);
    }
  else
    {
      for (ArgumentIterator a(*term); a.valid(); a.next())
	{
	  if (Term* v = findNonlinearVariable(a.argument(), seenIndices))
	    return v;
	}
    }
  return 0;
}

Term* 
MixfixModule::findNonlinearVariable(Term* term, VariableInfo& variableInfo)
{
  if (VariableTerm* v = dynamic_cast<VariableTerm*>(term))
    {
      int nrVariables = variableInfo.getNrRealVariables();
      (void) variableInfo.variable2Index(v);
      if (nrVariables == variableInfo.getNrRealVariables())
	return v;  // no increase in number of variables means v is a duplicate
    }
  else
    {
      for (ArgumentIterator a(*term); a.valid(); a.next())
	{
	  if (Term* v = findNonlinearVariable(a.argument(), variableInfo))
	    return v;
	}
    }
  return 0;
}
