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
//	Code for pretty printing all kinds of things.
//
bool MixfixModule::printMixfix = true;
bool MixfixModule::printWithParens = false;
bool MixfixModule::printWithAliases = true;
bool MixfixModule::printFlat = true;
bool MixfixModule::printGraph = false;
bool MixfixModule::printConceal = false;
bool MixfixModule::printFormat = true;
IntSet MixfixModule::concealed;
int MixfixModule::globalIndent = 0;
bool MixfixModule::attributeUsed = false;

void
MixfixModule::concealSymbols(const IntSet& symbols, bool add)
{
  if (add)
    concealed.insert(symbols);
  else
    concealed.subtract(symbols);
}

ostream&
operator<<(ostream& s, const NamedEntity* e)
{
  return s << Token::name(e->id());
}

ostream&
operator<<(ostream& s, const Sort* sort)
{
  if (sort == 0)
    return s << "(sort not calculated)";
  if (sort->index() == Sort::ERROR_SORT)
    {
      s << '[';
      ConnectedComponent* c = sort->component();
      s << Token::name(c->sort(1)->id());
      int nrMax = c->nrMaximalSorts();
      for (int i = 2; i <= nrMax; i++)
	s << ',' << Token::name(c->sort(i)->id());
      return s << ']';
    }
  return s << Token::name(sort->id());
}

ostream&
operator<<(ostream& s, const Term* term)
{
  MixfixModule::globalIndent = 0;
  static_cast<MixfixModule*>(term->symbol()->getModule())->
    prettyPrint(s, const_cast<Term*>(term), UNBOUNDED, UNBOUNDED, 0, UNBOUNDED, 0, false);  // HACK
  if (MixfixModule::attributeUsed)
    {
      MixfixModule::attributeUsed = false;
      s << Tty(Tty::RESET);
    }
  return s;
}

ostream&
operator<<(ostream& s, DagNode* dagNode)
{
  MixfixModule::globalIndent = 0;
  MixfixModule* module = static_cast<MixfixModule*>(dagNode->symbol()->getModule());
  if (MixfixModule::printGraph)
    module->graphPrint(s, dagNode);
  else
    {
      MixfixModule::ColoringInfo coloringInfo;
      if (interpreter.getFlag(Interpreter::PRINT_COLOR))
	{
	  MixfixModule::computeGraphStatus(dagNode,
					   coloringInfo.visited,
					   coloringInfo.statusVec);
	  coloringInfo.reducedAbove = false;
	  coloringInfo.reducedDirectlyAbove = false;
	}
      module->prettyPrint(s, coloringInfo, dagNode,
			  UNBOUNDED, UNBOUNDED, 0, UNBOUNDED, 0, false);
    }
  if (MixfixModule::attributeUsed)
    {
      MixfixModule::attributeUsed = false;
      s << Tty(Tty::RESET);
    }
  return s;
}

void
MixfixModule::printAttributes(ostream& s, const PreEquation* pe, int metadata)
{
  const Label& l = pe->getLabel();
  int id = l.id();
  const Equation* eq = dynamic_cast<const Equation*>(pe);
  bool owise = eq != 0 && eq->isOwise();
  bool nonexec = pe->isNonexec();
  if (!nonexec && !owise && id == NONE && metadata == NONE)
    return;
  s << " [";
  char *space = "";
  if (nonexec)
    {
      s << "nonexec";
      space = " ";
    }
  if (owise)
    {
      s << "owise";
      space = " ";
    }
  if (id != NONE)
    {
      s << space << "label " << &l;
      space = " ";
    }
  if (metadata != NONE)
    s << space << "metadata " << Token::name(metadata);
  s << ']';
}

ostream&
operator<<(ostream& s, const SortConstraint* sc)
{
  if (sc->hasCondition())
    s << 'c';
  s << "mb ";
  s << sc->getLhs() << " : " << sc->getSort();
  if (sc->hasCondition())
    MixfixModule::printCondition(s, sc);
  MixfixModule* m = safeCast(MixfixModule*, sc->getModule());
  MixfixModule::printAttributes(s,
				sc,
				m->getMetadata(MixfixModule::MEMB_AX, sc));
  s << " .";
  return s;
}

ostream&
operator<<(ostream& s, const Equation* e)
{
  if (e->hasCondition())
    s << 'c';
  s << "eq ";
  s << e->getLhs() << " = " << e->getRhs();
  if (e->hasCondition())
    MixfixModule::printCondition(s, e);
  MixfixModule* m = safeCast(MixfixModule*, e->getModule());
  MixfixModule::printAttributes(s,
				e,
				m->getMetadata(MixfixModule::EQUATION, e));
  s << " .";
  return s;
}

ostream&
operator<<(ostream& s, const Rule* r)
{
  if (r->hasCondition())
    s << 'c';
  s << "rl ";
  s << r->getLhs() << " => " << r->getRhs();
  if (r->hasCondition())
    MixfixModule::printCondition(s, r);
  MixfixModule* m = safeCast(MixfixModule*, r->getModule());
  MixfixModule::printAttributes(s,
				r,
				m->getMetadata(MixfixModule::RULE, r));
  s << " .";
  return s;
}

ostream&
operator<<(ostream& s, const ConditionFragment* c)
{
  if (const EqualityConditionFragment* e =
      dynamic_cast<const EqualityConditionFragment*>(c))
    s << e->getLhs() << " = " << e->getRhs();
  else if (const SortTestConditionFragment* t =
	   dynamic_cast<const SortTestConditionFragment*>(c))
    s << t->getLhs() << " : " << t->getSort();
  else if(const AssignmentConditionFragment* a =
	  dynamic_cast<const AssignmentConditionFragment*>(c))
    s << a->getLhs() << " := " << a->getRhs();
  else if(const RewriteConditionFragment* r =
	  dynamic_cast<const RewriteConditionFragment*>(c))
    s << r->getLhs() << " => " << r->getRhs();
  else
    CantHappen("bad condition fragment");
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
MixfixModule::printVariable(ostream& s, int name, Sort* sort) const
{
  s << Token::name(name);
  if (printWithAliases)
    {
      AliasMap::const_iterator i = variableAliases.find(name);
      if (i != variableAliases.end() && (*i).second == sort)
	return;
    }
  s << ':' << sort;
}

void
MixfixModule::printPrefixName(ostream& s, const char* prefixName, SymbolInfo& si)
{
  if (printFormat && (si.format.length() == 2))
    {
      fancySpace(s, si.format[0]);
      s << prefixName;
      fancySpace(s, si.format[1]);
    }
  else
    s << prefixName;
}

int
MixfixModule::printTokens(ostream& s,
			  const SymbolInfo& si,
			  int pos,
			  const char* color)
{
  bool noSpace = (pos == 0);
  bool hasFormat = printFormat && (si.format.length() > 0);
  for (;;)
    {
      int token = si.mixfixSyntax[pos++];
      if (token == underscore)
	break;
      bool special = (token == leftParen || token == rightParen ||
		      token == leftBracket || token == rightBracket ||
		      token == leftBrace || token == rightBrace || token == comma);
      if (!((hasFormat && fancySpace(s, si.format[pos - 1])) || special || noSpace))
	s << ' ';
      noSpace = special;
      if (color != 0)
	s << color;
      s << Token::name(token);
      if (color != 0)
	s << Tty(Tty::RESET);
    }
  if (!((hasFormat && fancySpace(s, si.format[pos - 1])) || noSpace))
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
			 const char* color)
{
  bool hasFormat = printFormat && (si.format.length() > 0);
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
			  token == leftBrace || token == rightBrace || token == comma);
	  if (!((hasFormat && fancySpace(s, si.format[j])) || special || noSpace))
	    s << ' ';
	  noSpace = special;
	  if (color != 0)
	    s << color;
	  s << Token::name(token);
	  if (color != 0)
	    s << Tty(Tty::RESET);
	}
      if (hasFormat)
	(void) fancySpace(s, si.format[mixfixLength]);
      if (++i == nrTails)
	break;
      if (needAssocParen)
	  s << ')';
    }
}

bool
MixfixModule::fancySpace(ostream& s, int spaceToken)
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
	    if (interpreter.getFlag(Interpreter::PRINT_COLOR))
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
  //	or 1 if such an index such not exist.
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
