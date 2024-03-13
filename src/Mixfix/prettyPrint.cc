/*

    This file is part of the Maude 3 interpreter.

    Copyright 1997-2024 SRI International, Menlo Park, CA 94025, USA.

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

void
MixfixModule::prettyPrint(ostream& s, const Term* term, const PrintSettings& printSettings, bool rangeKnown)
{
  clearIndent();
  MixfixModule* parentModule = safeCastNonNull<MixfixModule*>(term->symbol()->getModule());
  parentModule->prettyPrint(s, printSettings, term, UNBOUNDED, UNBOUNDED, 0, UNBOUNDED, 0, rangeKnown);
  clearColor(s);
}

void
MixfixModule::prettyPrint(ostream& s, DagNode* dagNode, const PrintSettings& printSettings, bool rangeKnown)
{
  if (dagNode == 0)
    {
      s << "(null DagNode*)";
      return;
    }
  clearIndent();
  MixfixModule* module = safeCastNonNull<MixfixModule*>(dagNode->symbol()->getModule());
  if (printSettings.getPrintFlag(PrintSettings::PRINT_GRAPH))
    module->graphPrint(s, dagNode, printSettings);
  else
    {
      ColoringInfo coloringInfo;
      if (printSettings.getPrintFlag(PrintSettings::PRINT_COLOR))
	{
	  computeGraphStatus(dagNode, coloringInfo.visited, coloringInfo.statusVec);
	  coloringInfo.reducedAbove = false;
	  coloringInfo.reducedDirectlyAbove = false;
	}
      module->prettyPrint(s, coloringInfo, printSettings, dagNode, UNBOUNDED, UNBOUNDED, 0, UNBOUNDED, 0, rangeKnown);
    }
  clearColor(s);
}

void
MixfixModule::printAttributes(ostream& s, const PreEquation* pe, ItemType itemType, const PrintSettings& printSettings)
{
  const Label& l = pe->getLabel();
  int id = printSettings.getPrintFlag(PrintSettings::PRINT_LABEL_ATTRIBUTE) ? l.id() : NONE;
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

void
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

void
MixfixModule::printConditionFragment(ostream& s, const ConditionFragment* cf, const PrintSettings& printSettings)
{
  if (const EqualityConditionFragment* e = dynamic_cast<const EqualityConditionFragment*>(cf))
    {
      prettyPrint(s, e->getLhs(), printSettings, false);
      s << " = ";
      prettyPrint(s, e->getRhs(), printSettings, true);
    }
  else if (const SortTestConditionFragment* t = dynamic_cast<const SortTestConditionFragment*>(cf))
    {
      prettyPrint(s, t->getLhs(), printSettings, true);
      s <<  " : " << t->getSort();
    }
  else if(const AssignmentConditionFragment* a = dynamic_cast<const AssignmentConditionFragment*>(cf))
    {
      prettyPrint(s, a->getLhs(), printSettings, false);
      s << " := ";
      prettyPrint(s, a->getRhs(), printSettings, true);
    }
  else if(const RewriteConditionFragment* r = dynamic_cast<const RewriteConditionFragment*>(cf))
    {
      prettyPrint(s, r->getLhs(), printSettings, false);
      s << " => ";
      prettyPrint(s, r->getRhs(), printSettings, true);
    }
  else
    CantHappen("bad condition fragment");
}

void
MixfixModule::printCondition(ostream& s, const Vector<ConditionFragment*>& condition, const PrintSettings& printSettings)
{
  const char* sep = "";
  for (ConditionFragment* cf : condition)
    {
      s << sep;
      sep = " /\\ ";
      printConditionFragment(s, cf, printSettings);
    }
}

void
MixfixModule::printCondition(ostream& s, const PreEquation* pe, const PrintSettings& printSettings)
{
  s << " if ";
  printCondition(s, pe->getCondition(), printSettings);
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

string
MixfixModule::prettyOpName(int code, int situations)
{
  auto pair = Token::makePrettyOpName(code, situations);  
  //
  //	If making the pretty name returned the problematic flag, we need to fix the problem in some way.
  //
  if (pair.second)
    {
      //
      //	We have a problem. If we're concerned about BARE_COLON we're in an op-hook and we return the single token
      //	ugly name. Otherwise we just put a set of parentheses around the pretty name if it exists or the regular
      //	name if it doesn't.
      //
      if (situations & Token::BARE_COLON)
	return Token::name(code);
      string nameToUse = pair.first.empty() ? Token::name(code) : pair.first;
      return "(" + nameToUse + ")";
    }
  //
  //	If we didn't prettify the name, return the original name.
  //
  return pair.first.empty() ? Token::name(code) : pair.first;
}
