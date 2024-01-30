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
//	Misc global data and functions.
//

//      utility stuff
#include "macros.hh"
#include "vector.hh"
#include "tty.hh"

//      forward declarations
#include "interface.hh"
#include "core.hh"
#include "higher.hh"
#include "freeTheory.hh"
#include "builtIn.hh"
#include "strategyLanguage.hh"
#include "mixfix.hh"
 
//      interface class definitions
#include "term.hh"
 
//      core class definitions
#include "lineNumber.hh"
#include "sortConstraint.hh"
#include "equation.hh"
#include "rule.hh"
#include "rewriteStrategy.hh"
#include "strategyDefinition.hh"

//      built in stuff
#include "randomOpSymbol.hh"

//	higher class definitions
#include "equalityConditionFragment.hh"
#include "sortTestConditionFragment.hh"
#include "assignmentConditionFragment.hh"
#include "rewriteConditionFragment.hh"

//	strategy language class definitions
#include "strategyExpression.hh"

//      system class definitions
#include "IO_Manager.hh"

//	mixfix frontend definitions
#include "token.hh"
#include "userLevelRewritingContext.hh"
#include "fileTable.hh"
#include "directoryManager.hh"
#include "mixfixModule.hh"
#include "interpreter.hh"
#include "global.hh"

int lineNumber = 1;
FileTable fileTable;
DirectoryManager directoryManager;
//
//	We allocate the top level interpreter on the heap, rather than statically so that when
//	debugging, we can delete it and check for memory leaks.
//
Interpreter& interpreter = *(new Interpreter);
IO_Manager ioManager;

string executableDirectory;

bool alwaysAdviseFlag = false;

bool
findFile(const string& userFileName, string& directory, string& fileName, int lineNr)
{
  static char const* const ext[] = {".maude", ".fm", ".obj", 0};
  //
  //	The user gave us a file name userFileName. We want to find the true
  //	file name, which may require added an extension, and the directory where
  //	the file is located; which might be the current directory but could also
  //	be a Maude library directory, or the directory containing the Maude executable.
  //
  string::size_type p = userFileName.rfind('/');
  if (p == string::npos)
    {
      //
      //	We have a plain file name, so we will look for the file in all
      //	the places it might reasonable be.
      //
      fileName = userFileName;
      //
      //	Start with the current directory.
      //
      directory = directoryManager.getCwd();
      if (directoryManager.checkAccess(directory, fileName, R_OK, ext))
	return true;
      //
      //	Try any specified Maude library directories.
      //
      if (directoryManager.searchPath(MAUDE_LIB, directory, fileName, R_OK, ext))
	return true;
      //
      //	Last hope is the directory containing the Maude executable.
      //
      if (!(executableDirectory.empty()) &&
	  directoryManager.checkAccess(executableDirectory, fileName, R_OK, ext))
	{
	  directory = executableDirectory;
	  return true;
	}
    }
  else if (p + 1 < userFileName.length())
    {
      //
      //	We have a full path, so we will just look there.
      //
      directoryManager.realPath(userFileName.substr(0, p), directory);
      fileName = userFileName.substr(p + 1);
      if (directoryManager.checkAccess(directory, fileName, R_OK, ext))
	return true;
    }
  IssueWarning(LineNumber(lineNr) << ": unable to locate file: " << QUOTE(userFileName));
  return false;
}

//
//	We put a bunch of global print functions here so they can access the top level interpreter
//	to pass print settings.
//

ostream&
operator<<(ostream& s, const Term* term)
{
  MixfixModule::prettyPrint(s, term, interpreter);
  return s;
}

ostream&
operator<<(ostream& s, DagNode* dagNode)
{
  MixfixModule::prettyPrint(s, dagNode, interpreter);
  return s;
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
  MixfixModule::prettyPrint(s, sc->getLhs(), interpreter, true);
  s << " : " << sc->getSort();
  if (sc->hasCondition())
    MixfixModule::printCondition(s, sc);
  MixfixModule* m = safeCast(MixfixModule*, sc->getModule());
  m->printAttributes(s, sc, MixfixModule::MEMB_AX, interpreter);
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
  MixfixModule::prettyPrint(s, e->getRhs(), interpreter, true);
  if (e->hasCondition())
    MixfixModule::printCondition(s, e);
  MixfixModule* m = safeCast(MixfixModule*, e->getModule());
  m->printAttributes(s, e, MixfixModule::EQUATION, interpreter);
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
  MixfixModule::prettyPrint(s, r->getRhs(), interpreter, true);
  if (r->hasCondition())
    MixfixModule::printCondition(s, r);
  MixfixModule* m = safeCast(MixfixModule*, r->getModule());
  m->printAttributes(s, r, MixfixModule::RULE, interpreter);
  s << " .";
  return s;
}

ostream&
operator<<(ostream& s, const ConditionFragment* c)
{
  if (const EqualityConditionFragment* e = dynamic_cast<const EqualityConditionFragment*>(c))
    {
      s << e->getLhs() << " = ";
      MixfixModule::prettyPrint(s, e->getRhs(), interpreter, true);
    }
  else if (const SortTestConditionFragment* t = dynamic_cast<const SortTestConditionFragment*>(c))
    {
      MixfixModule::prettyPrint(s, t->getLhs(), interpreter, true);
      s <<  " : " << t->getSort();
    }
  else if(const AssignmentConditionFragment* a = dynamic_cast<const AssignmentConditionFragment*>(c))
    {
      s << a->getLhs() << " := ";
      MixfixModule::prettyPrint(s, a->getRhs(), interpreter, true);
    }
  else if(const RewriteConditionFragment* r = dynamic_cast<const RewriteConditionFragment*>(c))
    {
      s << r->getLhs() << " => ";
      MixfixModule::prettyPrint(s, r->getRhs(), interpreter, true);
    }
  else
    CantHappen("bad condition fragment");
  return s;
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

  m->printAttributes(s, e, MixfixModule::STRAT_DEF, interpreter);
  s << " .";
  return s;
}

//
//	These global print functions don't need the top level interpreter, but we put them here
//	for consistancy
//

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
