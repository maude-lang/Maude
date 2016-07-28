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
//      Implementation for class Interpreter.
//

//      utility stuff
#include "macros.hh"
#include "vector.hh"
#include "tty.hh"

//      forward declarations
#include "interface.hh"
#include "core.hh"
#include "variable.hh"
#include "AU_Theory.hh"
#include "higher.hh"
#include "strategyLanguage.hh"
#include "mixfix.hh"
#include "SMT.hh"

//      interface class definitions
#include "term.hh"
#include "extensionInfo.hh"

//      core class definitions
#include "unificationProblem.hh"

//	higher class definitions
#include "pattern.hh"
#include "matchSearchState.hh"
#include "rewriteSequenceSearch.hh"
#include "unificationProblem.hh"
#include "narrowingSequenceSearch.hh"
#include "variantSearch.hh"

//	variable class definitions
#include "variableDagNode.hh"

//	object system class definitions
#include "pseudoThread.hh"

//	strategy languages definitions
#include "strategyExpression.hh"
#include "strategicSearch.hh"

//	front end class definitions
#include "timer.hh"
#include "userLevelRewritingContext.hh"
#include "maudemlBuffer.hh"
#include "syntacticPreModule.hh"
#include "view.hh"
#include "visibleModule.hh"
#include "loopSymbol.hh"
#include "freshVariableSource.hh"
#include "moduleExpression.hh"
#include "interpreter.hh"

//	our stuff
#include "execute.cc"
#include "match.cc"
#include "unify.cc"
#include "search.cc"
#include "loopMode.cc"
#include "erewrite.cc"
#include "srewrite.cc"

Interpreter::Interpreter()
{
  xmlLog = 0;
  xmlBuffer = 0;

  flags = DEFAULT_FLAGS;
  printFlags = DEFAULT_PRINT_FLAGS;
  currentModule = 0;
  currentView = 0;

  savedContext = 0;
  savedMatchSearchState = 0;
  savedUnificationProblem = 0;
  savedRewriteSequenceSearch = 0;
  savedStrategicSearch = 0;
  savedModule = 0;
  continueFunc = 0;
}

Interpreter::~Interpreter()
{
  //
  //	Must delete modules before other destruction takes place to avoid
  //	accessing free'd stuff.
  //
  deleteNamedModules();
  delete xmlBuffer;
  delete xmlLog;
}

void
Interpreter::setFlag(Flags flag, bool polarity)
{
  if (polarity)
    flags |= flag;
  else
    flags &= ~flag;
  RewritingContext::setTraceStatus(flags & EXCEPTION_FLAGS);
}

void
Interpreter::setPrintFlag(PrintFlags flag, bool polarity)
{
  if (polarity)
    printFlags |= flag;
  else
    printFlags &= ~flag;
}

void
Interpreter::showProfile() const
{
  currentModule->getFlatModule()->showProfile(cout);
}

void
Interpreter::beginXmlLog(const char* fileName)
{
  delete xmlBuffer;
  delete xmlLog;
  xmlLog = new ofstream(fileName);
  xmlBuffer = new MaudemlBuffer(*xmlLog);
}

void
Interpreter::endXmlLog()
{
  delete xmlBuffer;
  xmlBuffer = 0;
  delete xmlLog;
  xmlLog = 0;
}

bool
Interpreter::setCurrentModule(const Vector<Token>& moduleExpr, int start)
{
  SyntacticPreModule* m;
  int nrTokens = moduleExpr.length() - start;
  if (moduleExpr.length() == 0)
    {
      if (currentModule == 0)
	{
	  IssueWarning("no module expression provided and no last module.");
	  return false;
	}
      else if (currentModule->getFlatSignature()->isBad())
	m = currentModule;
      else
	return true;
    }
  else
    {
      if (nrTokens == 1)
	{
	  m = safeCast(SyntacticPreModule*, getModule(moduleExpr[start].code()));  // HACK
	  if (m != 0)
	    {      
	      if (m->getFlatSignature()->isBad())
		goto bad;
	      setCurrentModule(m);
	      return true;
	    }
	  IssueWarning(LineNumber(moduleExpr[start].lineNumber()) <<
		       ": no module " << QUOTE(moduleExpr[start]) << '.');
	}
      else 
	{
	  IssueWarning(LineNumber(moduleExpr[start].lineNumber()) <<
		       ": module expressions not supported in commands.");
	}
      return false;
    }
 bad:
  IssueWarning(*m << ": module " << QUOTE(m) <<
	       " is unusable due to unpatchable errors.");
  return false;
}

void
Interpreter::setCurrentModule(SyntacticPreModule* module)
{
  if (currentModule != module)
    {
      if (currentModule != 0)
	{
	  clearContinueInfo();
	  currentModule->loseFocus();
	}
      currentModule  = module;
    }
}

bool
Interpreter::setCurrentView(const Vector<Token>& viewExpr)
{
  switch (viewExpr.size())
    {
    case 0:
      {
	if (currentView == 0)
	  {
	    IssueWarning("no view expression provided and no last view.");
	    return false;
	  }
	return true;
      }
    case 1:
      {
	if (View* v = getView(viewExpr[0].code()))
	  {
	    setCurrentView(v);
	    return true;
	  }
	// fall thru
      }
    default:
      {
	IssueWarning(LineNumber(viewExpr[0].lineNumber()) <<
		   ": no view " << QUOTE(viewExpr) << '.');
      }
    }
  return false;
}

void
Interpreter::makeClean(int lineNumber)
{
  if (currentModule != 0 && !(currentModule->isComplete()))
    {
      IssueAdvisory(LineNumber(lineNumber) << ": discarding incomplete module.");
      delete currentModule;
      currentModule = 0;
    }
  else if (currentView != 0 && !(currentView->isComplete()))
    {
      IssueAdvisory(LineNumber(lineNumber) << ": discarding incomplete view.");
      delete currentView;
      currentView = 0;
    }
}

void
Interpreter::addSelected(const Vector<Token>& opName)
{
  selected.insert(Token::bubbleToPrefixNameCode(opName));
  //  opName.contractTo(0);
}


void
Interpreter::updateSet(set<int>& target, bool add)
{
  if (add)
    target.insert(selected.begin(), selected.end());
  else
    {
      FOR_EACH_CONST(i, set<int>, selected)
	target.erase(*i);
    }
  selected.clear();
}

bool
Interpreter::concealedSymbol(Symbol* symbol)
{
  return getPrintFlag(PRINT_CONCEAL) && concealedSymbols.find(symbol->id()) != concealedSymbols.end();
}

void
Interpreter::parse(const Vector<Token>& subject)
{
  //
  //	We need can't use getFlatSignature() since sort info
  //	is not computed until module is flattened.
  //
  Term *s = currentModule->getFlatModule()->parseTerm(subject);
  if (s != 0)
    {
      if (s->getSortIndex() == Sort::SORT_UNKNOWN)
	s->symbol()->fillInSortInfo(s);
      cout << s->getSort() << ": " << s << '\n';
      s->deepSelfDestruct();
    }
}

void
Interpreter::showSortsAndSubsorts() const
{
  currentModule->getFlatModule()->showSortsAndSubsorts(cout);
}

void
Interpreter::showModule(bool all) const
{
  currentModule->getFlatModule()->showModule(cout, all);
}

void
Interpreter::showView() const
{
  currentView->showView(cout);
}

void
Interpreter::showModules(bool all) const
{
  showNamedModules(cout);
  if (all)
    showCreatedModules(cout);
}

void
Interpreter::showOps(bool all) const
{
  currentModule->getFlatModule()->showPolymorphs(cout, false, all);
  currentModule->getFlatModule()->showOps(cout, false, all);
}

void
Interpreter::showVars() const
{
  currentModule->getFlatModule()->showVars(cout, false);
}

void
Interpreter::showMbs(bool all) const
{
  currentModule->getFlatModule()->showMbs(cout, false, all);
}

void
Interpreter::showEqs(bool all) const
{
  currentModule->getFlatModule()->showEqs(cout, false, all);
}

void
Interpreter::showRls(bool all) const
{
  currentModule->getFlatModule()->showRls(cout, false, all);
}

void
Interpreter::showKinds() const
{
  currentModule->getFlatModule()->showKinds(cout);
}

void
Interpreter::showSummary() const
{
  currentModule->getFlatModule()->showSummary(cout);
}


ImportModule*
Interpreter::getModuleOrIssueWarning(int name, const LineNumber& lineNumber)
{
  if (PreModule* m = getModule(name))
    {
      if (ImportModule* fm = m->getFlatSignature())
	{
	  //
	  //	We might have had to build a parser for this
	  //	module in order to deal with local statements,
	  //	term hooks and identities.
	  //	We delete the parser since we don't
	  //	have any further use for it.
	  //
	  fm->economize();
	  if (fm->isBad())
	    {
	      IssueWarning(lineNumber << ": unable to use module " <<
			   QUOTE(m) << " due to unpatchable errors.");
	    }
	  else
	    return fm;
	}
      else
	{
	  IssueWarning(lineNumber <<
		       ": mutually recursive import of module " <<
		       QUOTE(m) << " ignored.");
	}
    }
  else
    {
      IssueWarning(lineNumber <<
		   ": module " << QUOTE(Token::name(name)) <<
		   " does not exist.");
    }
  return 0;
}

ImportModule*
Interpreter::makeModule(const ModuleExpression* expr, ImportModule* enclosingModule)
{
  switch (expr->getType())
    {
    case ModuleExpression::MODULE:
      {
	Token name = expr->getModuleName();
	if (ImportModule* fm = getModuleOrIssueWarning(name.code(), name.lineNumber()))
	  return fm;
	break;
      }
    case ModuleExpression::RENAMING:
      {
	if (ImportModule* fm = makeModule(expr->getModule(), enclosingModule))
	  {
	    /*
	    if (fm->parametersBound())  // NEED TO FIX
	      {
		IssueWarning("renamed module " << fm << " has bound parameters.");
		return 0;
	      }
	    */
	    return makeRenamedCopy(fm, expr->getRenaming());
	  }
	break;
      }
    case ModuleExpression::SUMMATION:
      {
	const list<ModuleExpression*>& modules = expr->getModules();
	Vector<ImportModule*> fms;
	FOR_EACH_CONST(i, list<ModuleExpression*>, modules)
	  {
	    if (ImportModule* fm = makeModule(*i, enclosingModule))
	      {
		if (fm->getNrParameters() > 0)
		  {
		    IssueWarning("summand module " << fm << " has parameters.");
		    return 0;
		  }
		fms.append(fm);
	      }
	  }
	if (!fms.empty())
	  return makeSummation(fms);
	break;
      }
    case ModuleExpression::INSTANTIATION:
      {
	if (ImportModule* fm = makeModule(expr->getModule(), enclosingModule))
	  {
	    int nrParameters = fm->getNrParameters();
	    const Vector<Token>& arguments = expr->getArguments();
	    int nrArguments = arguments.size();
	    if (nrArguments != nrParameters)
	      {
		IssueWarning("wrong number of parameters in module instantiation " << QUOTE(expr) << "; " <<
			     nrParameters << " expected.");
		break;
	      }
	    Vector<View*> views(nrParameters);
	    Vector<int> names(nrParameters);
	    bool hasTheoryView = false;
	    bool hasPEM = false;
	    for (int i = 0; i < nrParameters; ++i)
	      {
		Token name = arguments[i];
		int code  = name.code();
		if (enclosingModule != 0)
		  {
		    int index = enclosingModule->findParameterIndex(code);
		    if (index != NONE)
		      {
			//
			//	Parameters from an enclosing module occlude views.
			//
			ImportModule* enclosingModuleParameterTheory = enclosingModule->getParameterTheory(index);
			ImportModule* requiredParameterTheory = fm->getParameterTheory(i);
			if (enclosingModuleParameterTheory != requiredParameterTheory)
			  {
			    IssueWarning("In argument " << i + 1 << " of module instantiation " << QUOTE(expr) <<
					 ", parameter " << QUOTE(name) << " from enclosing module " <<
					 QUOTE(enclosingModule) << " has theory " <<
					 QUOTE(enclosingModuleParameterTheory) <<
					 " whereas theory " <<  QUOTE(requiredParameterTheory) << " is required.");
			    return 0;
			  }
			views[i] = 0;
			names[i] = code;
			hasPEM = true;
			continue;
		      }
		  }
		if (View* v = getView(code))
		  {
		    //
		    //	Instantiation argument is a view.
		    //
		    if (!(v->evaluate()))
		      {
			IssueWarning("unusable view " << QUOTE(name) << " while evaluating module instantiation " <<
				     QUOTE(expr) << '.');
			return 0;
		      }
		    ImportModule* fromTheory = v->getFromTheory();
		    ImportModule* requiredParameterTheory = fm->getParameterTheory(i);
		    if (fromTheory != requiredParameterTheory)
		      {
			IssueWarning("In argument " << i + 1 << " of module instantiation " << QUOTE(expr) <<
				     ", view " << QUOTE(static_cast<NamedEntity*>(v)) << " is from theory " <<
				     QUOTE(fromTheory) << " whereas theory " <<  QUOTE(requiredParameterTheory) <<
				     " is required.");
			return 0;
		      }
		    views[i] = v;
		    names[i] = 0;
		    if (v->getToModule()->isTheory())
		      hasTheoryView = true;
		  }
		else
		  {
		    IssueWarning("In argument " << i + 1 << " of module instantiation " << QUOTE(expr) <<
				 " could not find a parameter or view " << QUOTE(name) << ".");
		    return 0;
		  }
	      }
	    if (hasTheoryView && hasPEM)
	      {
		IssueWarning("Instantiation " << QUOTE(expr) <<
			     " uses both a theory-view and a parameter from enclosing module " <<
			     QUOTE(enclosingModule) << '.');
		return 0;
	      }
	    return makeInstatiation(fm, views, names);  // may return null but never has bad flag set
	  }
	break;
      }
    default:
      CantHappen("bad module expression");
    }
  return 0;
}
