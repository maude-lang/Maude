/*

    This file is part of the Maude 3 interpreter.

    Copyright 1997-2022 SRI International, Menlo Park, CA 94025, USA.

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
//      Implementation for class VisibleModule.
//

//      utility stuff
#include "macros.hh"
#include "vector.hh"

//      forward declarations
#include "interface.hh"
#include "core.hh"
#include "variable.hh"
#include "strategyLanguage.hh"
#include "higher.hh"
#include "strategyLanguage.hh"
#include "mixfix.hh"

//      interface class definitions
#include "term.hh"
#include "binarySymbol.hh"
#include "variableSymbol.hh"

//      core class definitions
#include "equation.hh"
#include "rule.hh"
#include "rewriteStrategy.hh"
#include "strategyDefinition.hh"
#include "sortConstraint.hh"

//	higher class definitions
#include "equalityConditionFragment.hh"
#include "sortTestConditionFragment.hh"
#include "assignmentConditionFragment.hh"
#include "rewriteConditionFragment.hh"

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

//	front end class definitions
#include "userLevelRewritingContext.hh"
#include "visibleModule.hh"

//	our stuff
#include "latexModulePrint.cc"
#include "latexStrategyPrint.cc"

VisibleModule::VisibleModule(int name, ModuleType moduleType, Interpreter* owner)
  : ImportModule(name, moduleType),
    owner(owner)
{
}

void
VisibleModule::showSummary(ostream& s)
{
  int nrNonterminals;
  int nrTerminals;
  int nrProductions;
  getParserStats(nrNonterminals, nrTerminals, nrProductions);
  s << "Grammar:" <<
    "\n\tnonterminals: " << nrNonterminals <<
    "\n\tterminals: " << nrTerminals <<
    "\n\tproductions: " << nrProductions << "\n\n";

  int nrKinds = getConnectedComponents().length();
  s << "Term rewriting system:" <<
    "\n\tkinds: " << nrKinds <<
    "\n\tsorts: " << getSorts().length() - nrKinds <<
    "\n\tuser symbols: " << getNrUserSymbols() <<
    "\n\ttotal symbols: " << getSymbols().length() <<
    "\n\tpolymorphic operators: " << getNrPolymorphs() <<
    "\n\tmembership axioms: " << getSortConstraints().length() <<
    "\n\tequations: " << getEquations().length() <<
    "\n\trules: " << getRules().length() <<
    "\n\tstrategies: " << getStrategies().length() <<
    "\n\tstrategy definitions: " << getStrategyDefinitions().length() << '\n';
}

void
VisibleModule::showKinds(ostream& s) const
{
  const Vector<ConnectedComponent*>& kinds = getConnectedComponents();
  int nrKinds = getNrUserComponents();
  for (int i = 0; i < nrKinds; i++)
    {
      const ConnectedComponent* c = kinds[i];
      s << c->sort(Sort::KIND);
      if (c->errorFree())
	s << " (error free)";
      s<< ":\n";
      int nrSorts = c->nrSorts();
      for (int j = 1; j < nrSorts; j++)
	{
	  s << '\t' << j << '\t' << c->sort(j)  << '\n';
	  DebugInfo(Token::name(c->sort(j)->id()));
	}
      if (i + 1 < nrKinds)
	s << '\n';
      DebugAdvisory("match indices = " << c->getLastAllocatedMatchIndex());
    }
}

void
VisibleModule::showSortsAndSubsorts(ostream& s) const
{
  const Vector<Sort*>& sorts = getSorts();
  int nrUserSorts =  getNrUserSorts();
  for (int i = 0; i < nrUserSorts; i++)
    {
      if (UserLevelRewritingContext::interrupted())
	return;
      const Sort* sort = sorts[i];
      s << "sort " << sort << " .";
      
      int index = sort->index();
      const ConnectedComponent* c = sort->component();
      int nrComponentSorts = c->nrSorts();
      int nrSupersorts = 0;
      for (int j = 1; j < index; j++)
	{
	  if (leq(sort, j))
	    ++nrSupersorts;
	}
      int nrSubsorts = 0;
      for (int j = index + 1; j < nrComponentSorts; j++)
	{
	  if (leq(j, sort))
	    ++nrSubsorts;
	}

      int nrComparableSorts = nrSubsorts + nrSupersorts;
      if (nrComparableSorts > 0)
	{
	  s << "\tsubsort";
	  if (nrComparableSorts > 1)
	    s << 's';
	  if (nrSubsorts > 0)
	    {
	      for (int j = nrComponentSorts - 1; j > index ; j--)
		{
		  const Sort* sort2 = c->sort(j);
		  if (leq(sort2, sort))
		    s << ' ' << sort2;
		}
	      s << " <";
	    }
	  s << ' ' << Tty(Tty::MAGENTA) << sorts[i] << Tty(Tty::RESET);
	  if (nrSupersorts > 0)
	    {
	      s << " <";
	      for (int j = index - 1; j > 0; j--)
		{
		  const Sort* sort2 = c->sort(j);
		  if (leq(sort, sort2))
		    s << ' ' << sort2;
		}
	    }
	  s << " .";
	}
      s << '\n';
    }
}

void
VisibleModule::showImports(ostream& s) const
{
  int nrImports = getNrImports();
  for (int i = 0; i < nrImports; i++)
    {
      if (UserLevelRewritingContext::interrupted())
	return;
      s << "  " << importModeString(getImportMode(i)) <<
	' ' << Token::removeBoundParameterBrackets(getImportedModule(i)->id()) << " .\n";
    }
}

void
VisibleModule::showModule(ostream& s, bool all) const
{
  s << moduleTypeString(getModuleType()) << ' ' << this;
  int nrParameters = getNrParameters();
  if (nrParameters > 0)
    {
      s << '{' << Token::name(getParameterName(0)) << " :: " << getParameterTheory(0);
      for (int i = 1; i < nrParameters; ++i)
	s << ", " << Token::name(getParameterName(i)) << " :: " << getParameterTheory(i);
      s << '}';
    }
  s << " is\n";
  if (!all)
    showImports(s);

  showSorts1(s, true, all);
  showSubsorts(s, true, all);
  showPolymorphs(s, true, all);
  showOps(s, true, all);
  showVars(s, true);
  showMbs(s, true, all);
  showEqs(s, true, all);
  showRls(s, true, all);
  showStrats(s, true, all);
  showSds(s, true, all);
  if (UserLevelRewritingContext::interrupted())
    return;
  s << moduleEndString(getModuleType()) << '\n';
}

void
VisibleModule::showSorts1(ostream& s, bool indent, bool all) const
{
  if (UserLevelRewritingContext::interrupted())
    return;
  int begin = all ? 0 : getNrImportedSorts();
  int end =  getNrUserSorts();
  int nrSorts = end - begin;
  if (nrSorts > 0)
    {
      const Vector<Sort*>& sorts = getSorts();
      if (indent)
	s << "  ";
      s << "sort";
      if (nrSorts > 1)
	s << 's';
      for (int i = begin; i < end; i++)
	s << ' ' << sorts[i];
      s << " .\n";
    }
}

void
VisibleModule::showSubsorts(ostream& s, bool indent, bool all) const
{
  const char* ind = indent ? "  " : "";
  const Vector<Sort*>& sorts = getSorts();
  int nrUserSorts =  getNrUserSorts();
  for (int i = 0; i < nrUserSorts; i++)
    {
      if (UserLevelRewritingContext::interrupted())
	return;
      int begin = all ? 0 : getNrImportedSubsorts(i);
      const Sort* sort = sorts[i];
      const Vector<Sort*>& subsorts = sort->getSubsorts();
      int end = subsorts.length();
      int nrSubsorts = end - begin;
      if (nrSubsorts > 0)
	{
	  s << ind << "subsort";
	  if (nrSubsorts > 1)
	    s << 's';
	  for (int j = begin; j < end; j++)
	    s << ' ' << subsorts[j];
	  s << " < " << sort << " .\n";
	}
    }
}

void
VisibleModule::showVars(ostream& s, bool indent) const
{
  const char* ind = indent ? "  " : "";
  const AliasMap& variableAliases = getVariableAliases();
  for (const auto& p : variableAliases)
    {
      if (UserLevelRewritingContext::interrupted())
	return;
      s << ind << "var " << Token::name(p.first) << " : " << p.second << " .\n";
    }
}

void
VisibleModule::showMbs(ostream& s, bool indent, bool all) const
{
  const char* ind = indent ? "  " : "";
  const Vector<SortConstraint*>& mbs = getSortConstraints();
  int nrMbs = all ? mbs.length() : getNrOriginalMembershipAxioms();
  for (int i = 0; i < nrMbs; i++)
    {
      if (UserLevelRewritingContext::interrupted())
	return;
      s << ind << mbs[i] << '\n';
    }
}

void
VisibleModule::showEqs(ostream& s, bool indent, bool all) const
{
  const char* ind = indent ? "  " : "";
  const Vector<Equation*>& equations = getEquations();
  int nrEquations = all ? equations.length() : getNrOriginalEquations();
  for (int i = 0; i < nrEquations; i++)
    {
      if (UserLevelRewritingContext::interrupted())
	return;
      s << ind << equations[i] << '\n';
    }
}

void
VisibleModule::showRls(ostream& s, bool indent, bool all) const
{
  const char* ind = indent ? "  " : "";
  const Vector<Rule*>& rules = getRules();
  int nrRules = all ? rules.length() : getNrOriginalRules();
  for (int i = 0; i < nrRules; i++)
    {
      if (UserLevelRewritingContext::interrupted())
	return;
      s << ind << rules[i] << '\n';
    }
}

void
VisibleModule::showStrats(ostream& s, bool indent, bool all) const
{
  const char* ind = indent ? "  " : "";
  const Vector<RewriteStrategy*>& strategies = getStrategies();
  int nrStrategies = strategies.size();
  int start = all ? 0 : getNrImportedStrategies();
  for (int i = start; i < nrStrategies; i++)
    {
      if (UserLevelRewritingContext::interrupted())
       return;
      s << ind << strategies[i] << '\n';
    }
}

void
VisibleModule::showSds(ostream& s, bool indent, bool all) const
{
  const char* ind = indent ? "  " : "";
  const Vector<StrategyDefinition*>& defs = getStrategyDefinitions();
  int nrDefs = all ? defs.length() : getNrOriginalStrategyDefinitions();
  for (int i = 0; i < nrDefs; i++)
    {
      if (UserLevelRewritingContext::interrupted())
	return;
      s << ind << defs[i] << '\n';
    }
}

void
VisibleModule::showPolymorphAttributes(ostream& s, int index) const
{
  SymbolType st = getPolymorphType(index);
  if (st.hasFlag(SymbolType::CTOR))
    s << " ctor";
  //
  //	Theory attributes.
  //
  if (st.hasFlag(SymbolType::ASSOC))
    s << " assoc";
  if (st.hasFlag(SymbolType::COMM))
    s << " comm";
  if (st.hasFlag(SymbolType::LEFT_ID | SymbolType::RIGHT_ID))
    {
      s << (st.hasFlag(SymbolType::LEFT_ID) ? " left" : " right") <<
	" id: " << getPolymorphIdentity(index);
    }
  if (st.hasFlag(SymbolType::IDEM))
    s << " idem";
  if (st.hasFlag(SymbolType::ITER))
    s << " iter";
  if (st.hasFlag(SymbolType::PCONST))
    s << " pconst";
  //
  //	Object-oriented attributes.
  //
  if (st.hasFlag(SymbolType::OBJECT))
    s << " obj";
  if (st.hasFlag(SymbolType::MESSAGE))
    s << " msg";
  if (st.hasFlag(SymbolType::CONFIG))
    s << " config";
  //
  //	Semantic attributes.
  //
  const Vector<int>& strat = getPolymorphStrategy(index);
  int stratLen = strat.length();
  if (stratLen > 0)
    {
      s << " strat (";
      for (int k = 0; k < stratLen; k++)
	{
	  s << strat[k];
	  if (k + 1 < stratLen)
	    s << ' ';
	}
      s << ')';
    }
  if (st.hasFlag(SymbolType::MEMO))
    s << " memo";
  if (st.hasFlag(SymbolType::FROZEN))
    {
      s << " frozen (";
      const NatSet& frozen = getPolymorphFrozen(index);
      const NatSet::const_iterator e = frozen.end();
      for (NatSet::const_iterator i = frozen.begin();;)
	{
	  s << *i + 1;
	  if (++i == e)
	    break;
	  s << ' ';
	}
      s << ')';
    }
  //
  //	Syntactic attributes.
  //
  Vector<int> gather;
  getPolymorphGather(index, gather);
  int gatherLength = gather.length();
  if (st.hasFlag(SymbolType::PREC | SymbolType::GATHER) || gatherLength > 0)
    {
      s << " prec " << getPolymorphPrec(index);
      if (gatherLength > 0)
	{
	  s << " gather (";
	  for (int i = 0; i < gatherLength; i++)
	    {
	      if (i != 0)
		s << ' ';
	      switch (gather[i])
		{
		case GATHER_e:
		  s << 'e';
		  break;
		case GATHER_E:
		  s << 'E';
		  break;
		case GATHER_AMP:
		  s << '&';
		  break;
		}
	    }
	  s << ')';
	}
    }
  if (st.hasFlag(SymbolType::FORMAT))
    {
      s << " format (";
      const Vector<int>& format = getPolymorphFormat(index);
      int formatLength = format.length();
      for (int i = 0; i < formatLength; i++)
	{
	  if (i != 0)
	    s << ' ';
	  s << Token::name(format[i]);
	}
      s << ')';
    }

  int metadata = getPolymorphMetadata(index);
  if (metadata != NONE)
    s << " metadata " << Token::name(metadata);

  if (st.hasSpecial())
    {
      s << " special (";
      int purpose;
      {
	Vector<int> items;
	for (int i = 0; getPolymorphDataAttachment(index, i, purpose, items); i++)
	  {
	    s << "\n    " << "id-hook " << Token::name(purpose);
	    int nrItems = items.length();
	    if (nrItems > 0)
	      {
		for (int j = 0; j < nrItems; j++)
		  s << ((j == 0) ? " (" : " ") << Token::name(items[j]);
		s << ')';
	      }
	  }
      }
      {
	Symbol* op;
	for (int i = 0; getPolymorphSymbolAttachment(index, i, purpose, op); i++)
	  {
	    s << "\n    " << "op-hook " << Token::name(purpose) << " (" <<
	      op << " : ";
	    const Vector<Sort*>& domainAndRange =
	      op->getOpDeclarations()[0].getDomainAndRange();
	    int nrSorts = domainAndRange.length() - 1;
	    //
	    //	We don't use operator<< for sorts in an op-hook since they
	    //	should always be printed as single tokens.
	    //
	    for (int j = 0; j < nrSorts; j++)
	      {
		Sort* sort = domainAndRange[j];
		if (sort->index() == Sort::KIND)
		  sort = sort->component()->sort(1);
		s << Token::name(sort->id()) << ' ';
	      }
	    Sort* sort = domainAndRange[nrSorts];
	    if (sort->index() == Sort::KIND)
	      sort = sort->component()->sort(1);
	    s << "~> " << Token::name(sort->id()) << ')';
	  }
      }
      {
	Term* term;
	for (int i = 0; getPolymorphTermAttachment(index, i, purpose, term); i++)
	  s << "\n    " << "term-hook " << Token::name(purpose) << " (" << term << ")";
      }
      s << ')';
    }
}

void
VisibleModule::showPolymorphs(ostream& s, bool indent, bool all) const
{
  int nrPolymorphs = getNrPolymorphs();
  int begin = all ? 0 : getNrImportedPolymorphs();
  for (int i = begin; i < nrPolymorphs; i++)
    {
      if (UserLevelRewritingContext::interrupted())
	return;
      showPolymorphDecl(s, indent, i);
    }
}

void
VisibleModule::showPolymorphDecl(ostream& s, bool indent, int index) const
{
  const char* ind = indent ? "  " : "";
  s << ind << "op ";
  
  const Vector<Sort*>& domainAndRange = getPolymorphDomainAndRange(index);
  int nrArgs = domainAndRange.length() - 1;
  if (nrArgs == 0)
    s << Token::sortName(getPolymorphName(index).code()) << " :";
  else
    s << getPolymorphName(index) << " :";

  for (int i = 0; i < nrArgs; i++)
    {
      if (Sort* sort = domainAndRange[i])
	s << ' ' << sort;
      else
	s << " Universal";
    }
  if (Sort* sort = domainAndRange[nrArgs])
    s << " -> " << sort;
  else
    s << " -> Universal";
  s << " [poly ";
  const char* sep = "(";
  for (int i = 0; i < nrArgs; i++)
    {
      if (domainAndRange[i] == 0)
	{
	  s << sep << i + 1;
	  sep = " ";
	}
    }
  if (domainAndRange[nrArgs] == 0)
    s << sep << '0';
  s << ')';
  showPolymorphAttributes(s, index);
  s << "] .\n";
}

void
VisibleModule::showOps(ostream& s, bool indent, bool all) const
{
  int begin = all ? 0 : getNrImportedSymbols();
  int end = getNrUserSymbols();
  for (int i = begin; i < end; i++)
    {
      if (UserLevelRewritingContext::interrupted())
	return;
      showDecls(s, indent, i, all);
    }
}

void
VisibleModule::showDecls(ostream& s, bool indent, int index, bool all) const
{
  const char* ind = indent ? "  " : "";
  Symbol* symbol = getSymbols()[index];
  int begin = all ? 0 : getNrImportedDeclarations(index);
  int end = getNrUserDeclarations(index);
  Assert(end > 0, "op " << symbol << " has no declarations");
  const Vector<OpDeclaration>& opDecls = symbol->getOpDeclarations();

  int nrArgs = symbol->arity();
  for (int i = begin; i < end; i++)
    {
      if (UserLevelRewritingContext::interrupted())
	return;
      const Vector<Sort*>& dec = opDecls[i].getDomainAndRange();
      s << ind << "op " << symbol << " :";
      for (int j = 0; j < nrArgs; j++)
	s << ' ' << dec[j];
      s << " -> " << dec[nrArgs];
      showAttributes(s, symbol, i);
      s << " .\n";
    }
}

void
VisibleModule::showAttributes(ostream& s, Symbol* symbol, int opDeclIndex) const
{
  Vector<int> gather;
  getGather(symbol, gather);  // there will be a gather whenever there is mixfix syntax and args
  int gatherLength = gather.length();
  SymbolType st = getSymbolType(symbol);
  const OpDeclaration& decl = symbol->getOpDeclarations()[opDeclIndex];
  bool ctor = decl.isConstructor();
  int metadata = getMetadata(symbol, opDeclIndex);


  if (gatherLength == 0 &&
      st.getBasicType() == SymbolType::STANDARD &&
      !(st.hasFlag(SymbolType::ATTRIBUTES)) &&
      !ctor &&
      metadata == NONE)
    return;  // no attributes;

  const char* space = "";
  s << " [";
  if (ctor)
    {
      s << space << "ctor";
      space = " ";
    }
  //
  //	Theory attributes.
  //
  if (st.hasFlag(SymbolType::ASSOC))
    {
      s << space << "assoc";
      space = " ";
    }
  if (st.hasFlag(SymbolType::COMM))
    {
      s << space << "comm";
      space = " ";
    }
  if (st.hasFlag(SymbolType::ITER))
    {
      s << space << "iter";
      space = " ";
    }
  if (st.hasFlag(SymbolType::LEFT_ID | SymbolType::RIGHT_ID))
    {
      s << space;
      space = " ";
      if (!(st.hasFlag(SymbolType::LEFT_ID | SymbolType::COMM)))
	s << "right ";
      else if (!(st.hasFlag(SymbolType::RIGHT_ID | SymbolType::COMM)))
	s << "left ";
      s << "id: ";
      Term* id = safeCast(BinarySymbol*, symbol)->getIdentity();
      if (id != 0)
	s << id;
    }
  if (st.hasFlag(SymbolType::IDEM))
    {
      s << space << "idem";
      space = " ";
    }
  if (st.hasFlag(SymbolType::PCONST))
    {
      s << space << "pconst";
      space = " ";
    }
  //
  //	Object-oriented attributes.
  //
  if (st.hasFlag(SymbolType::MESSAGE))
    {
      s << space << "msg";
      space = " ";
    }
  if (st.hasFlag(SymbolType::OBJECT))
    {
      s << space << "obj";
      space = " ";
    }
  if (st.hasFlag(SymbolType::CONFIG))
    {
      s << space << "config";
      space = " ";
    }
  //
  //	Semantic attributes.
  //
  const Vector<int>& strat = symbol->getStrategy();
  int stratLen = strat.length();
  if (stratLen > 0)
    {
      s << space << "strat (";
      space = " ";
      for (int k = 0; k < stratLen; k++)
	{
	  s << strat[k];
	  if (k + 1 < stratLen)
	    s << ' ';
	}
      s << ')';
    }
  if (st.hasFlag(SymbolType::MEMO))
    {
      s << space << "memo";
      space = " ";
    }
  if (st.hasFlag(SymbolType::FROZEN))
    {
      s << space << "frozen (";
      space = " ";
      const NatSet& frozen = symbol->getFrozen();
      const NatSet::const_iterator e = frozen.end();
      for (NatSet::const_iterator i = frozen.begin();;)
	{
	  s << *i + 1;
	  if (++i == e)
	    break;
	  s << ' ';
	}
      s << ')';
    }
  //
  //	Syntactic attributes.
  //
  if (st.hasFlag(SymbolType::PREC | SymbolType::GATHER) || gatherLength > 0)
    {
      s << space << "prec " << getPrec(symbol);
      space = " ";
      if (gatherLength > 0)
	{
	  s << " gather (";
	  for (int i = 0; i < gatherLength; i++)
	    {
	      if (i != 0)
		s << ' ';
	      switch (gather[i])
		{
		case GATHER_e:
		  s << 'e';
		  break;
		case GATHER_E:
		  s << 'E';
		  break;
		case GATHER_AMP:
		  s << '&';
		  break;
		}
	    }
	  s << ')';
	}
    }
  if (st.hasFlag(SymbolType::FORMAT))
    {
      s << space << "format (";
      space = " ";
      const Vector<int>& format = getFormat(symbol);
      int formatLength = format.length();
      for (int i = 0; i < formatLength; i++)
	{
	  if (i != 0)
	    s << ' ';
	  s << Token::name(format[i]);
	}
      s << ')';
    }
  if (metadata != NONE)
    {
      s << space << "metadata " << Token::name(metadata);
      space = " ";
    }
  if (st.hasSpecial())
    {
      s << space << "special (";
      Vector<const char*> purposes;
      {
	Vector<Vector<const char*> > data;
	getDataAttachments(symbol, decl.getDomainAndRange(), purposes, data);
	int nrHooks = purposes.length();
	for (int i = 0; i < nrHooks; i++)
	  {
	    s << "\n    " << "id-hook " << purposes[i];
	    const Vector<const char*>& items = data[i];
	    int nrItems = items.length();
	    if (nrItems > 0)
	      {
		for (int j = 0; j < nrItems; j++)
		  s << ((j == 0) ? " (" : " ") << items[j];
		s << ')';
	      }
	  }
      }
      purposes.clear();
      {
	Vector<Symbol*> symbols;
	getSymbolAttachments(symbol, purposes, symbols);
	int nrHooks = purposes.length();
	for (int i = 0; i < nrHooks; i++)
	  {
	    Symbol* op = symbols[i];
	    s << "\n    " << "op-hook " << purposes[i] << " (" <<
	      op << " : ";
	    const Vector<Sort*>& domainAndRange =
	      op->getOpDeclarations()[0].getDomainAndRange();
	    int nrSorts = domainAndRange.length() - 1;
	    //
	    //	We don't use operator<< for sorts in an op-hook since they
	    //	should always be printed as single tokens.
	    //
	    for (int j = 0; j < nrSorts; j++)
	      {
		Sort* sort = domainAndRange[j];
		if (sort->index() == Sort::KIND)
		  sort = sort->component()->sort(1);
		s << Token::name(sort->id()) << ' ';
	      }
	    Sort* sort = domainAndRange[nrSorts];
	    if (sort->index() == Sort::KIND)
	      sort = sort->component()->sort(1);
	    s << "~> " << Token::name(sort->id()) << ')';
	  }
      }
      purposes.clear();
      {
	Vector<Term*> terms;
	getTermAttachments(symbol, purposes, terms);
	int nrHooks = purposes.length();
	for (int i = 0; i < nrHooks; i++)
	  s << "\n    " << "term-hook " << purposes[i] << " (" << terms[i] << ")";
      }
      s << ')';
    }
  s << ']';
}
