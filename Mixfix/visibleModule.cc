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
//      Implementation for class VisibleModule.
//

//      utility stuff
#include "macros.hh"
#include "vector.hh"

//      forward declarations
#include "interface.hh"
#include "core.hh"
#include "variable.hh"
#include "mixfix.hh"

//      interface class definitions
#include "term.hh"
#include "binarySymbol.hh"
#include "variableSymbol.hh"

//      core class definitions
#include "equation.hh"
#include "rule.hh"
#include "sortConstraint.hh"
//#include "conditionFragment.hh"

//     higher class definitions
//#include "equalityConditionFragment.hh"
//#include "sortTestConditionFragment.hh"
//#include "assignmentConditionFragment.hh"
//#include "rewriteConditionFragment.hh"

//	front end class definitions
#include "userLevelRewritingContext.hh"
#include "visibleModule.hh"
//#include "importTranslation.hh"
//#include "quotedIdentifierSymbol.hh"

VisibleModule::VisibleModule(int name, ModuleType moduleType, Parent* parent)
  : ImportModule(name, moduleType, parent)
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
    "\n\tsymbols: " << getSymbols().length() <<
    "\n\tmembership axioms: " << getSortConstraints().length() <<
    "\n\tequations: " << getEquations().length() <<
    "\n\trules: " << getRules().length() << '\n';
}

void
VisibleModule::showKinds(ostream& s) const
{
  const Vector<ConnectedComponent*>& kinds = getConnectedComponents();
  int nrKinds = kinds.length();
  for (int i = 0; i < nrKinds; i++)
    {
      const ConnectedComponent* c = kinds[i];
      s << c->sort(0);
      if (c->errorFree())
	s << " (error free)";
      s<< ":\n";
      int nrSorts = c->nrSorts();
      for (int j = 1; j < nrSorts; j++)
	{
	  s << '\t' << j << '\t' << c->sort(j) << '\n';
	}
      if (i + 1 < nrKinds)
	s << '\n';
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
	  s << ' ' << sorts[i];
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
VisibleModule::showModule(ostream& s, bool all) const
{
  if (getModuleType() == FUNCTIONAL_MODULE)
    s << 'f';
  s << "mod " << this << " is\n";
  showSorts1(s, true, all);
  showSubsorts(s, true, all);
  showOps(s, true, all);
  showVars(s, true);
  showMbs(s, true, all);
  showEqs(s, true, all);
  showRls(s, true, all);
  if (UserLevelRewritingContext::interrupted())
    return;
  s << ((getModuleType() == FUNCTIONAL_MODULE) ? "endfm\n" : "endm\n");
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
  for (AliasMap::const_iterator i = variableAliases.begin(); i != variableAliases.end(); ++i)
    {
      if (UserLevelRewritingContext::interrupted())
	return;
      s << ind << "var " << Token::name((*i).first) << " : " << (*i).second << " .\n";
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
VisibleModule::showDecls(ostream& s, bool indent, Symbol* symbol, bool all) const
{
  const char* ind = indent ? "  " : "";
  int begin = 0;
  const Vector<OpDeclaration>& opDecls = symbol->getOpDeclarations();
  int end = opDecls.length();
  Assert(end > 0, "op " << symbol << " has no declarations");

  if (!all)
    {
      int index = symbol->getIndexWithinModule();
      begin = getNrImportedDeclarations(index);
      end = getNrUserDeclarations(index);
    }

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
      showAttributes(s, symbol, opDecls[i].isConstructor());
      s << " .\n";
    }
}

void
VisibleModule::showOps(ostream& s, bool indent, bool all) const
{
  const Vector<Symbol*>& symbols = getSymbols();
  int begin = all ? 0 : getNrImportedSymbols();
  int end = all ? symbols.length() : getNrUserSymbols();
  for (int i = begin; i < end; i++)
    {
      if (UserLevelRewritingContext::interrupted())
	return;
      Symbol* symbol = symbols[i];
      if (dynamic_cast<VariableSymbol*>(symbol) == 0)
	showDecls(s, indent, symbol, all);
    }
}

void
VisibleModule::showAttributes(ostream& s, Symbol* symbol, bool ctor) const
{
  Vector<int> gather;
  getGather(symbol, gather);  // there will be a gather whenever there is mixfix syntax and args
  int gatherLength = gather.length();
  SymbolType st = getSymbolType(symbol);

  if (gatherLength == 0 &&
      // until we handle specials
      // st.getBasicType() == SymbolType::STANDARD &&
      !(st.hasFlag(SymbolType::ATTRIBUTES)) &&
      !ctor)
    return;  // no attributes;

  const char* space = "";
  s << " [";
  //
  //	Theory attributes.
  //
  if (st.hasFlag(SymbolType::ASSOC))
    {
      s << "assoc";
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
  if (st.hasFlag(SymbolType::LEFT_ID | SymbolType::RIGHT_ID))
    {
      s << space;
      space = " ";
      if (!(st.hasFlag(SymbolType::LEFT_ID | SymbolType::COMM)))
	s << "right";
      else if (!(st.hasFlag(SymbolType::RIGHT_ID | SymbolType::COMM)))
	s << "left";
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
  if (ctor)
    {
      s << space << "ctor";
      space = " ";
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
  s << ']';
}
