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

//	higher class definitions
#include "pattern.hh"
#include "matchSearchState.hh"
#include "rewriteSequenceSearch.hh"
#include "unificationProblem.hh"
#include "irredundantUnificationProblem.hh"
#include "narrowingSequenceSearch.hh"
#include "variantSearch.hh"
#include "filteredVariantUnifierSearch.hh"

//	variable class definitions
#include "variableDagNode.hh"

//	object system class definitions
#include "pseudoThread.hh"

//	strategy languages definitions
#include "strategyExpression.hh"
#include "depthFirstStrategicSearch.hh"
#include "fairStrategicSearch.hh"

//	front end class definitions
#include "timer.hh"
#include "cacheableRewritingContext.hh"
#include "maudemlBuffer.hh"
#include "syntacticPreModule.hh"
#include "syntacticView.hh"
#include "visibleModule.hh"
#include "loopSymbol.hh"
#include "freshVariableSource.hh"
#include "viewExpression.hh"
#include "moduleExpression.hh"
#include "interpreter.hh"

//	our stuff
#include "execute.cc"
#include "match.cc"
#include "unify.cc"
#include "variantUnify.cc"
#include "variantMatch.cc"
#include "getVariants.cc"
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

  savedState = 0;
  savedModule = 0;
  continueFunc = 0;
}

Interpreter::~Interpreter()
{
  //
  //	Must delete modules before other destruction takes place to avoid
  //	accessing free'd stuff. Deletion of named modules implicitly forces
  //	deletion of cached modules since cached modules must be derived from
  //	and hence dependent on named modules. It also implicitly forces the
  //	deletion of cached views since such views must have a named or cached
  //	module as their from-theory.
  //
  deleteNamedModules();
  clearContinueInfo();
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
  //
  //	Commands use this to set the current module to moduleExpr, which could
  //	be empty if no module was specified in the command.
  //
  //	This also serves as the gatekeeper, to prevent commands executing in
  //	a module on which markAsBad() has been called.
  //
  SyntacticPreModule* m;
  if (moduleExpr.empty())
    {
      //
      //	No module specified. See if there is a usable currentModule.
      //
      if (currentModule == 0)
	{
	  IssueWarning("no module expression provided and no last module.");
	  return false;
	}
      else if (currentModule->getFlatSignature()->isBad())
	m = currentModule;  // fall into bad case below
      else
	return true;  // we have a good current module
    }
  else
    {
      //
      //	We have a module specified by the tokens, however we may
      //	need to skip start tokens, typically "in".
      //
      int nrTokens = moduleExpr.length() - start;
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
  IssueWarning(*m << ": module " << QUOTE(m) << " is unusable due to unpatchable errors.");
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
      currentModule = module;
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
	    setCurrentView(safeCast(SyntacticView*, v));
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
      IssueAdvisory(*currentModule << ": discarding incomplete module " << QUOTE(currentModule) << ".");
      delete currentModule;
      currentModule = 0;
    }
  else if (currentView != 0 && !(currentView->isComplete()))
    {
      IssueAdvisory(*currentView << ": discarding incomplete view " << QUOTE(currentView) << ".");
      delete currentView;
      currentView = 0;
    }
}

void
Interpreter::addSelected(const Vector<Token>& opName)
{
  selected.insert(Token::bubbleToPrefixNameCode(opName));
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
  //	We need to use getFlatModule() rather than getFlatSignature()
  //	since operator sort info is not computed until the statements are
  //	flattened in and the theory is closed.
  //
  Term* s = currentModule->getFlatModule()->parseTerm(subject);
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
  if (currentView->evaluate())  // in case it became stale
    currentView->showView(cout);
  else
    IssueWarning("view " << QUOTE(currentView) << " cannot be used due to earlier errors.");
}

void
Interpreter::showProcessedView() const
{
  if (currentView->evaluate())  // in case it became stale
    currentView->showProcessedView(cout);
  else
    IssueWarning("view " << QUOTE(currentView) << " cannot be used due to earlier errors.");
}

void
Interpreter::showModules(bool all) const
{
  showNamedModules(cout);
  if (all)
    showCreatedModules(cout);
}

void
Interpreter::showViews(bool all) const
{
  showNamedViews(cout);
  if (all)
    showCreatedViews(cout);
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
Interpreter::showStrats(bool all) const
{
  currentModule->getFlatModule()->showStrats(cout, false, all);
}

void
Interpreter::showSds(bool all) const
{
  currentModule->getFlatModule()->showSds(cout, false, all);
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

Argument*
Interpreter::handleArgument(const ViewExpression* expr,
			    EnclosingObject* enclosingObject,
			    ImportModule* requiredParameterTheory,
			    int argNr)
{
  //
  //	An argument must be the name of a parameter from an enclosing object or the name of
  //	a view or an instantiation of a view.
  //	In all cases the fromTheory of the view or the theory of the parameter must match
  //	requiredParameterTheory.
  //
  if (expr->isInstantiation())
    {
      //IssueAdvisory("evaluation of view instantiation " << expr << " is experimental");
      //
      //	We have the instantiation of a parameterized view.
      //
      ViewExpression* baseViewExpr = expr->getView();
      //
      //	Base view must be a named view or a view expression - cannot be a parameter.
      //
      Argument* baseArg = handleArgument(baseViewExpr, enclosingObject, requiredParameterTheory, NONE);
      if (baseArg == 0)
	return 0;
      View* baseView = safeCast(View*, baseArg);
      //
      //	Number of parameters in base view must match number of arguments passed
      //	in instantiation.
      //
      int nrParameters = baseView->getNrParameters();
      const Vector<ViewExpression*>& argumentExpressions = expr->getArguments();
      int nrArguments = argumentExpressions.size();
      if (nrArguments != nrParameters)
	{
	  IssueWarning(nrArguments << (nrArguments == 1 ? " argument" : " arguments") <<
		       " passed in view instantiation " << QUOTE(expr) << " whereas " <<
		       nrParameters << " expected.");
	  return 0;
	}
      //
      //	We now construct an argument list of Parameters and Views.
      //
      Vector<Argument*> arguments(nrParameters);
      bool hasTheoryView = false;  // theory-view maintain free parameters
      bool hasPEO = false;  // parameters from an enclosing object (PEO) create bound parameters
      bool hasViewWithBoundParameters = false;
      for (int i = 0; i < nrParameters; ++i)
	{
	  DebugInfo("----- looking argument " << i << " which is " << argumentExpressions[i] << " --------");
	  Argument* a = handleArgument(argumentExpressions[i], enclosingObject, baseView->getParameterTheory(i), i);
	  if (a == 0)
	    return 0;

	  if (View* v = dynamic_cast<View*>(a))
	    {
	      if (v->hasFreeParameters())
		{
		  IssueWarning("view " << QUOTE(v) << " has free parameter " <<
			       QUOTE(Token::name(v->getParameterName(0))) <<
			       " and cannot be used in view instantiation " <<
			       QUOTE(expr) << ".");
		  return 0;
		}
	      if (v->hasBoundParameters())
		hasViewWithBoundParameters = true;
	      if (v->getToModule()->isTheory())
		hasTheoryView = true;
	    }
	  else
	    hasPEO = true;
	  arguments[i] = a;
	}
      if (hasTheoryView && hasPEO)
	{
	  IssueWarning("Instantiation " << QUOTE(expr) <<
		       " uses both a theory-view and a parameter from enclosing " <<
		       enclosingObject->getObjectType() << " " <<
		       QUOTE(enclosingObject->getObjectName()) << '.');
	  return 0;
	}
      if (hasTheoryView && hasViewWithBoundParameters)
	{
	  IssueWarning("Nonfinal instantiation " << QUOTE(expr) <<
		       " uses both a theory-view and a view with bound parameters from enclosing " <<
		       enclosingObject->getObjectType() << " " <<
		       QUOTE(enclosingObject->getObjectName()) << '.');
	}
      return makeViewInstantiation(baseView, arguments);
    }
  //
  //	Base case: parameter name or view name.
  //
  Token name = expr->getName();
  int code  = name.code();
  if (enclosingObject != 0 && argNr != NONE)
    {
      //
      //	Because we have an enclosing object and we're in an argument list we
      //	check for a parameter from the enclosing object.
      //
      int index = enclosingObject->findParameterIndex(code);
      if (index != NONE)
	{
	  //
	  //	Parameters from an enclosing object occlude views.
	  //
	  ImportModule* enclosingObjectParameterTheory = enclosingObject->getParameterTheory(index);
	  if (enclosingObjectParameterTheory != requiredParameterTheory)
	    {
	      IssueWarning(LineNumber(name.lineNumber()) << ": parameter " << QUOTE(name) <<
			   " from enclosing " << enclosingObject->getObjectType() <<
			   ' ' << QUOTE(enclosingObject->getObjectName()) <<
			   " is of theory " << QUOTE(enclosingObjectParameterTheory) <<
			   " whereas theory " <<  QUOTE(requiredParameterTheory) <<
			   " is required.");
	      return 0;
	    }
	  return getParameter(code);
	}
    }
  //
  //	Must be a view
  //
  if (View* v = getView(code))
    {
      //
      //	Instantiation argument is a view.
      //
      if (!(v->evaluate()))
	{
	  IssueWarning(LineNumber(name.lineNumber()) << ": unusable view " << QUOTE(v) << '.');
	  return 0;
	}
      ImportModule* fromTheory = v->getFromTheory();
      if (fromTheory != requiredParameterTheory)
	{
	  IssueWarning(LineNumber(name.lineNumber()) << ": view " << QUOTE(name) <<
		       " is from theory " << QUOTE(fromTheory) <<
		       " whereas theory " << QUOTE(requiredParameterTheory) <<
		       " is required.");
	  return 0;
	}
      return v;
    }
  IssueWarning(LineNumber(name.lineNumber()) << ": could not find a parameter or view " << QUOTE(name) << ".");
  return 0;
}

ImportModule*
Interpreter::makeModule(const ModuleExpression* expr, EnclosingObject* enclosingObject)
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
	if (ImportModule* fm = makeModule(expr->getModule(), enclosingObject))
	  {
	    if (fm->hasBoundParameters())
	      {
		//
		//	We allow this because we have to deal with this case when
		//	a module with free parameters whose imports have bound parameters
		//	is renamed, and we make use of it in the prelude.
		//
	      }
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
	    if (ImportModule* fm = makeModule(*i, enclosingObject))
	      {
		if (fm->hasFreeParameters())
		  {
		    IssueWarning("summand module " << fm << " has free parameters.");
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
	if (ImportModule* fm = makeModule(expr->getModule(), enclosingObject))
	  {
	    int nrParameters = fm->getNrParameters();
	    const Vector<ViewExpression*>& argumentExpressions = expr->getArguments();
	    int nrArguments = argumentExpressions.size();
	    if (nrArguments != nrParameters)
	      {
		IssueWarning("wrong number of parameters in module instantiation " << QUOTE(expr) << "; " <<
			     nrParameters << " expected.");
		break;
	      }
	    Vector<Argument*> arguments(nrParameters);
	    bool hasTheoryView = false;
	    bool hasPEO = false;
	    bool hasViewWithBoundParameters = false;
	    for (int i = 0; i < nrParameters; ++i)
	      {
		Argument* a = handleArgument(argumentExpressions[i], enclosingObject, fm->getParameterTheory(i), i);
		if (a != 0)
		  {
		    if (View* v = dynamic_cast<View*>(a))
		      {
			if (v->hasFreeParameters())
			  {
			    IssueWarning("view " << QUOTE(v) << " has free parameter " <<
					 QUOTE(Token::name(v->getParameterName(0))) <<
					 " and cannot be used in module instantiation " <<
					 QUOTE(expr) << ".");
			    return 0;
			  }
			if (v->hasBoundParameters())
			  hasViewWithBoundParameters = true;
			if (v->getToModule()->isTheory())
			  hasTheoryView = true;
		      }
		    else
		      hasPEO = true;
		  }
		else
		  return 0;
		arguments[i] = a;
	      }
	    if (hasTheoryView && hasPEO)
	      {
		IssueWarning("Instantiation " << QUOTE(expr) <<
			     " uses both a theory-view and a parameter from enclosing " <<
			     enclosingObject->getObjectType() << " " <<
			     QUOTE(enclosingObject->getObjectName()) << '.');
		return 0;
	      }
	    if (hasTheoryView && hasViewWithBoundParameters)
	      {
		IssueWarning("Nonfinal instantiation " << QUOTE(expr) <<
			     " uses both a theory-view and a view with bound parameters from enclosing " <<
			     enclosingObject->getObjectType() << " " <<
			     QUOTE(enclosingObject->getObjectName()) << '.');
		return 0;
	      }
	    return makeModuleInstantiation(fm, arguments);  // may return null but never has bad flag set
	  }
	break;
      }
    default:
      CantHappen("bad module expression");
    }
  return 0;
}
