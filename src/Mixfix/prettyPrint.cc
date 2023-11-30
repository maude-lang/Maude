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
//	Code for pretty printing all kinds of things.
//

int MixfixModule::globalIndent = 0;
bool MixfixModule::attributeUsed = false;

ostream&
operator<<(ostream& s, const NamedEntity* e)
{
  return s << ((e == 0) ? "(null)" : Token::name(e->id()));
}

ostream&
operator<<(ostream& s, const Symbol* symbol)
{
  if (symbol == 0)
    return s << "(null)";
  int code = symbol->id();
  if (symbol->arity() == 0)
    return s << Token::sortName(code);  // hack to handle parameterized constant names
  return s << Token::name(code);
}

ostream&
operator<<(ostream& s, const Sort* sort)
{
  if (sort == 0)
    return s << "(sort not calculated)";
  
  ConnectedComponent* c = sort->component();
  if (c != 0 && sort->index() == Sort::KIND)
    {
      s << '[' << c->sort(1);
      int nrMax = c->nrMaximalSorts();
      for (int i = 2; i <= nrMax; i++)
	s << ',' << c->sort(i);
      return s << ']';
    }
  return s << Token::sortName(sort->id());
}

ostream&
operator<<(ostream& s, const ConnectedComponent* component)
{
  //
  //	We identify a connect component (aka kind) with its ERROR (aka KIND) sort.
  //
  return s << component->sort(Sort::KIND);
}

void
MixfixModule::prettyPrint(ostream& s, const Term* term, bool rangeKnown)
{
  clearIndent();
  MixfixModule* parentModule = safeCastNonNull<MixfixModule*>(term->symbol()->getModule());
  const PrintSettings& printSettings = interpreter;  // HACK
  parentModule->prettyPrint(s, printSettings, const_cast<Term*>(term), UNBOUNDED, UNBOUNDED, 0, UNBOUNDED, 0, rangeKnown);
  clearColor(s);
}

ostream&
operator<<(ostream& s, const Term* term)
{
  MixfixModule::prettyPrint(s, term);
  return s;
}

void
MixfixModule::prettyPrint(ostream& s, DagNode* dagNode, bool rangeKnown)
{
  if (dagNode == 0)
    {
      s << "(null DagNode*)";
      return;
    }
  MixfixModule::clearIndent();
  MixfixModule* module = safeCastNonNull<MixfixModule*>(dagNode->symbol()->getModule());
  const PrintSettings& printSettings = interpreter;  // HACK
  if (printSettings.getPrintFlag(PrintSettings::PRINT_GRAPH))
    module->graphPrint(s, dagNode, printSettings);
  else
    {
      MixfixModule::ColoringInfo coloringInfo;
      if (printSettings.getPrintFlag(PrintSettings::PRINT_COLOR))
	{
	  MixfixModule::computeGraphStatus(dagNode, coloringInfo.visited, coloringInfo.statusVec);
	  coloringInfo.reducedAbove = false;
	  coloringInfo.reducedDirectlyAbove = false;
	}
      module->prettyPrint(s, coloringInfo, printSettings, dagNode, UNBOUNDED, UNBOUNDED, 0, UNBOUNDED, 0, rangeKnown);
    }
  MixfixModule::clearColor(s);
}

ostream&
operator<<(ostream& s, DagNode* dagNode)
{
  MixfixModule::prettyPrint(s, dagNode);
  return s;
}

void
MixfixModule::printAttributes(ostream& s, const PreEquation* pe, ItemType itemType)
{
  const Label& l = pe->getLabel();
  int id = interpreter.getPrintFlag(PrintSettings::PRINT_LABEL_ATTRIBUTE) ? l.id() : NONE;
  const Equation* eq = dynamic_cast<const Equation*>(pe);
  const Rule* rl = dynamic_cast<const Rule*>(pe);
  bool owise = eq != 0 && eq->isOwise();
  bool variant = eq != 0 && eq->isVariant();
  bool narrowing = rl != 0 && rl->isNarrowing();
  bool nonexec = pe->isNonexec();
  int metadata = getMetadata(itemType, pe);
  const PrintAttribute* printAttribute = getPrintAttribute(itemType, pe);

  if (!nonexec && !owise && !variant && !narrowing && id == NONE && metadata == NONE && printAttribute == 0)
    return;
  s << " [";
  const char* space = "";
  if (nonexec)
    {
      s << "nonexec";
      space = " ";
    }
  if (owise)
    {
      s << space << "owise";
      space = " ";
    }
  if (variant)
    {
      s << space << "variant";
      space = " ";
    }
  if (narrowing)
    {
      s << space << "narrowing";
      space = " ";
    }
  if (id != NONE)
    {
      s << space << "label " << &l;
      space = " ";
    }
  if (metadata != NONE)
    {
      s << space << "metadata " << Token::name(metadata);
      space = " ";
    }
  if (printAttribute != 0)
    {
      s << space;
      printAttribute->print(s, *pe);
    }
  s << ']';
}


ostream&
operator<<(ostream& s, const SortConstraint* sc)
{
  if (sc->hasCondition())
    s << 'c';
  s << "mb ";
  if (!interpreter.getPrintFlag(PrintSettings::PRINT_LABEL_ATTRIBUTE))
    {
      const Label& l = sc->getLabel();
      if (l.id() != NONE)
	s << "[" << &l << "] : ";
    }
  MixfixModule::prettyPrint(s, sc->getLhs(), true);
  s << " : " << sc->getSort();
  if (sc->hasCondition())
    MixfixModule::printCondition(s, sc);
  MixfixModule* m = safeCast(MixfixModule*, sc->getModule());
  m->printAttributes(s, sc, MixfixModule::MEMB_AX);
  s << " .";
  return s;
}

ostream&
operator<<(ostream& s, const Equation* e)
{
  if (e->hasCondition())
    s << 'c';
  s << "eq ";
  if (!interpreter.getPrintFlag(PrintSettings::PRINT_LABEL_ATTRIBUTE))
    {
      const Label& l = e->getLabel();
      if (l.id() != NONE)
	s << "[" << &l << "] : ";
    }
  s << e->getLhs() << " = ";
  MixfixModule::prettyPrint(s, e->getRhs(), true);
  if (e->hasCondition())
    MixfixModule::printCondition(s, e);
  MixfixModule* m = safeCast(MixfixModule*, e->getModule());
  m->printAttributes(s, e, MixfixModule::EQUATION);
  s << " .";
  return s;
}

ostream&
operator<<(ostream& s, const Rule* r)
{
  if (r->hasCondition())
    s << 'c';
  s << "rl ";
  if (!interpreter.getPrintFlag(PrintSettings::PRINT_LABEL_ATTRIBUTE))
    {
      const Label& l = r->getLabel();
      if (l.id() != NONE)
	s << "[" << &l << "] : ";
    }
  s << r->getLhs() << " => ";
  MixfixModule::prettyPrint(s, r->getRhs(), true);
  if (r->hasCondition())
    MixfixModule::printCondition(s, r);
  MixfixModule* m = safeCast(MixfixModule*, r->getModule());
  m->printAttributes(s, r, MixfixModule::RULE);
  s << " .";
  return s;
}

ostream&
operator<<(ostream& s, const ConditionFragment* c)
{
  if (const EqualityConditionFragment* e = dynamic_cast<const EqualityConditionFragment*>(c))
    {
      s << e->getLhs() << " = ";
      MixfixModule::prettyPrint(s, e->getRhs(), true);
    }
  else if (const SortTestConditionFragment* t = dynamic_cast<const SortTestConditionFragment*>(c))
    {
      MixfixModule::prettyPrint(s, t->getLhs(), true);
      s <<  " : " << t->getSort();
    }
  else if(const AssignmentConditionFragment* a = dynamic_cast<const AssignmentConditionFragment*>(c))
    {
      s << a->getLhs() << " := ";
      MixfixModule::prettyPrint(s, a->getRhs(), true);
    }
  else if(const RewriteConditionFragment* r = dynamic_cast<const RewriteConditionFragment*>(c))
    {
      s << r->getLhs() << " => ";
      MixfixModule::prettyPrint(s, r->getRhs(), true);
    }
  else
    CantHappen("bad condition fragment");
  return s;
}

ostream&
operator<<(ostream& s, const RewriteStrategy* rs)
{
  s << "strat " << Token::name(rs->id()) << " ";

  // Prints domain sorts
  const Vector<Sort*>& domain = rs->getDomain();
  int arity = rs->arity();
  if (arity > 0)
    {
      s << ": ";
      for (int i = 0; i < arity; i++)
	s << domain[i] << ' ';
    }

  s << "@ " << rs->getSubjectSort();
  // Print attributes (only metadata is allowed)
  MixfixModule* m = safeCast(MixfixModule*, rs->getModule());
  int metadata = m->getMetadata(MixfixModule::STRAT_DECL, rs);
  if (metadata != NONE)
    s << " [metadata " << Token::name(metadata) << "] ";
  s << " .";
  return s;
}

inline void
MixfixModule::printStrategyTerm(ostream& s, RewriteStrategy* strat, Term* term)
{
  // The term itself is not what we want to print because it contains
  // the auxiliary symbol label instead of the strategy label

  s << Token::name(strat->id());

  if (strat->arity() > 0 || ruleLabels.find(strat->id()) != ruleLabels.end())
    {
      s << "(";
      bool first = true;
      for (ArgumentIterator it(*term); it.valid(); it.next())
	{
	  if (first)
	    first = false;
	  else
	    s << ", ";
	  s << it.argument();
	}
      s << ")";
    }
}

ostream&
operator<<(ostream& s, const StrategyDefinition* e)
{
  if (e->hasCondition())
    s << 'c';
  s << "sd ";
  // Prints the LHS with the strategy label
  MixfixModule* m = safeCast(MixfixModule*, e->getModule());
  m->printStrategyTerm(s, e->getStrategy(), e->getLhs());
  s << " := " << e->getRhs();
  if (e->hasCondition())
    MixfixModule::printCondition(s, e);

  m->printAttributes(s, e, MixfixModule::STRAT_DEF);
  s << " .";
  return s;
}

void
MixfixModule::printCondition(ostream& s, const Vector<ConditionFragment*>& condition)
{
  int nrFragments = condition.length();
  for (int i = 0; i < nrFragments; i++)
    {
      s << condition[i];
      if (i + 1 < nrFragments)
	s << " /\\ ";
    }
}

void
MixfixModule::printCondition(ostream& s, const PreEquation* pe)
{
  s << " if ";
  printCondition(s, pe->getCondition());
}

void
MixfixModule::printVariable(ostream& s, int name, Sort* sort, const PrintSettings& printSettings) const
{
  if (Token::isFlagged(name))
    {
      //
      //	Variables with flagged codes are used internally to distinguish between two variables
      //	with identical names during disjoint unification, and might be seen in a debugging
      //	print statement.	
      //
      s << "(flagged)";
      name = Token::unflaggedCode(name);
    }
  s << Token::name(name);
  if (printSettings.getPrintFlag(PrintSettings::PRINT_WITH_ALIASES))
    {
      AliasMap::const_iterator i = variableAliases.find(name);
      if (i != variableAliases.end() && (*i).second == sort)
	return;
    }
  s << ':' << sort;
}

void
MixfixModule::printPrefixName(ostream& s, const char* prefixName, SymbolInfo& si, const PrintSettings& printSettings)
{
  if (printSettings.getPrintFlag(PrintSettings::PRINT_FORMAT) && (si.format.length() == 2))
    {
      fancySpace(s, si.format[0], printSettings);
      s << prefixName;
      fancySpace(s, si.format[1], printSettings);
    }
  else
    s << prefixName;
}

int
MixfixModule::printTokens(ostream& s,
			  const SymbolInfo& si,
			  int pos,
			  const char* color,
			  const PrintSettings& printSettings)
{
  bool noSpace = (pos == 0);
  bool hasFormat = printSettings.getPrintFlag(PrintSettings::PRINT_FORMAT) && (si.format.length() > 0);
  for (;;)
    {
      int token = si.mixfixSyntax[pos++];
      if (token == underscore)
	break;
      bool special = (token == leftParen || token == rightParen ||
		      token == leftBracket || token == rightBracket ||
		      token == leftBrace || token == rightBrace);
      if (!((hasFormat && fancySpace(s, si.format[pos - 1], printSettings)) || special || noSpace || token == comma))
	s << ' ';
      noSpace = special || (hasFormat && token == comma);
      if (color != 0)
	s << color;
      s << Token::name(token);
      if (color != 0)
	s << Tty(Tty::RESET);
    }
  if (!((hasFormat && fancySpace(s, si.format[pos - 1], printSettings)) || noSpace))
    s << ' ';
  return pos;
}

void
MixfixModule::printTails(ostream& s,
			 const SymbolInfo& si,
			 int pos,
			 int nrTails,
			 bool needAssocParen,
			 bool checkForInterrupt,
			 const char* color,
			 const PrintSettings& printSettings)
{
  bool hasFormat = printSettings.getPrintFlag(PrintSettings::PRINT_FORMAT) && (si.format.length() > 0);
  int mixfixLength = si.mixfixSyntax.length();
  for (int i = 0;;)
    {
      if (checkForInterrupt && UserLevelRewritingContext::interrupted())
	return;
      bool noSpace = (pos == 0);
      for (int j = pos; j < mixfixLength; j++)
	{
	  int token = si.mixfixSyntax[j];
	  bool special = (token == leftParen || token == rightParen ||
			  token == leftBracket || token == rightBracket ||
			  token == leftBrace || token == rightBrace);
	  if (!((hasFormat && fancySpace(s, si.format[j], printSettings)) || special || noSpace || token == comma))
	    s << ' ';
	  noSpace = special || (hasFormat && token == comma);
	  if (color != 0)
	    s << color;
	  s << Token::name(token);
	  if (color != 0)
	    s << Tty(Tty::RESET);
	}
      if (hasFormat)
	(void) fancySpace(s, si.format[mixfixLength], printSettings);
      if (++i == nrTails)
	break;
      if (needAssocParen)
	  s << ')';
    }
}

bool
MixfixModule::fancySpace(ostream& s, int spaceToken, const PrintSettings& printSettings)
{
  bool space = false;
  for (const char* cmd = Token::name(spaceToken); *cmd; cmd++)
    {
      char c = *cmd;
      switch (c)
	{
	case '+':
	  {
	    ++globalIndent;
	    break;
	  }
	case '-':
	  {
	    if (globalIndent > 0)
	      --globalIndent;
	    break;
	  }
	case 'n':
	  {
	    s << '\n';
	    space = true;
	    break;
	  }
	case 't':
	  {
	    s << '\t';
	    space = true;
	    break;
	  }
	case 's':
	  {
	    s << ' ';
	    space = true;
	    break;
	  }
	case 'i':
	  {
	    if (globalIndent > 0)
	      {
		for (int i = 0; i < globalIndent; i++)
		  s << ' ';
		space = true;
	      }
	    break;
	  }
	default:
	  {
	    if (printSettings.getPrintFlag(PrintSettings::PRINT_COLOR))
	      break;
	    switch (c)
	      {
#define MACRO(m, t) \
case m: { s << Tty(Tty::t); attributeUsed = true; break; }
#include "ansiEscapeSequences.cc"
#undef MACRO
	      case 'o':
		{
		  s << Tty(Tty::RESET);
		  break;
		}
	      }
	    break;
	  }
	}
    }
  return space;
}

int
MixfixModule::chooseDisambiguator(Symbol* s)
{
  //
  //	Choose the index of a disambiguating sort for an ad-hoc overloaded symbol.
  //
  //	Return smallest non-error sort index (largest sort) that the symbol can produce
  //	or 1 if such an index does not exist.
  //
  const Vector<OpDeclaration>& opDecls = s->getOpDeclarations();
  int nrOpDecls = opDecls.length();
  int nrArgs = s->arity();
  int chosenIndex = UNBOUNDED;
  for (int i = 0; i < nrOpDecls; i++)
    {
      int index = opDecls[i].getDomainAndRange()[nrArgs]->index();
      if (index > Sort::ERROR_SORT && index < chosenIndex)
	chosenIndex = index;
    }
  return (chosenIndex == UNBOUNDED) ? 1 : chosenIndex;
}

void
MixfixModule::makeIterName(string& name, int id, const mpz_class& number)
{
  name = Token::name(id);
  name += '^';
  char* numberStr = mpz_get_str(0, 10, number.get_mpz_t());
  name += numberStr;
  free(numberStr);
}

int
MixfixModule::computeGraphStatus(DagNode* dagNode,
				 PointerSet& visited,
				 Vector<int>& statusVec)
{
  visited.insert(dagNode);
  int index = statusVec.length();
  Assert(index == visited.pointer2Index(dagNode),
	 "statusVec out of step");
  statusVec.resize(index + 1);
  //
  //	We have strangeness below if we are reduced and one of our arguments
  //	(a) has strangeness below
  //	(b) is unreduced
  //	(c) is a non constructor
  //
  int status = 0;
  bool reduced = dagNode->isReduced();
  if (reduced && dagNode->symbol()->isConstructor(dagNode))
    status |= CONSTRUCTOR;
  for(DagArgumentIterator a(*dagNode); a.valid(); a.next())
    {
      DagNode* d = a.argument();
      int childIndex = visited.pointer2Index(d);
      int childStatus = (childIndex == NONE) ?
	computeGraphStatus(d, visited, statusVec) :
	statusVec[childIndex];
      if (reduced &&
	  ((childStatus & STRANGENESS_BELOW) ||
	   !(d->isReduced()) ||
	   !(childStatus & CONSTRUCTOR)))
	status |= STRANGENESS_BELOW;
    }
  statusVec[index] = status;
  return status;
}

void
MixfixModule::printModifiers(ostream& s, Int64 number, Int64 number2)
{
  if (number != NONE || number2 != NONE)
    {
      s << '[';
      if (number != NONE)
	s << number;
      if (number2 != NONE)
	s << ", " << number2;
      s << "] ";
    }
  s << "in " << this << " : ";
}
