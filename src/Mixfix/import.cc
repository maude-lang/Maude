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
//	Code for handling importation.
//

void
PreModule::processImports()
{
  //
  //	Parameters.
  //
  {
    FOR_EACH_CONST(i, Vector<Parameter>, parameters)
      {
	if (ImportModule* fm = makeModule(i->theory))
	  flatModule->addParameter(i->name, interpreter.makeParameterCopy(i->name.code(), fm));  // HACK maybe pass Token
      }
  }
  //
  //	Automatic imports (not for theories).
  //
  if (!(MixfixModule::isTheory(moduleType)))
    {
      FOR_EACH_CONST(i, ModuleDatabase::ImportMap, autoImports)
	{
	  if (ImportModule* fm = getModule(i->first, *this))
	    flatModule->addImport(fm, i->second, *this);
	}
    }
  //
  //	Explicit imports.
  //
  {
    FOR_EACH_CONST(i, Vector<Import>, imports)
      {
	if (ImportModule* fm = makeModule(i->expr, flatModule))
	  {
	    ImportModule::ImportMode mode;
	    int code = i->mode.code();
	    LineNumber lineNumber(i->mode.lineNumber());
	    if (code == Token::encode("pr") || code == Token::encode("protecting"))
	      mode = ImportModule::PROTECTING;
	    else if (code == Token::encode("ex") || code == Token::encode("extending"))
	      mode = ImportModule::EXTENDING;
	    else if (code == Token::encode("inc") || code == Token::encode("including"))
	      mode = ImportModule::INCLUDING;
	    else if (code == Token::encode("us") || code == Token::encode("using"))
	      {
		IssueWarning(lineNumber <<
			     ": importation mode " << QUOTE("using") <<
			     " not supported - treating it like " <<
			     QUOTE("including") << '.');
		mode = ImportModule::INCLUDING;
	      }
	    if (fm->getNrParameters() != 0 && !(fm->parametersBound()))
	      {
		IssueWarning(lineNumber << ": cannot import module " << fm <<
			     " because it has free parameters.");
	      }
	    else
	      flatModule->addImport(fm, mode, lineNumber);
	  }
      }
  }
  //
  //	House keeping.
  //
  interpreter.destructUnusedModules();
}

ImportModule*
PreModule::getModule(int name, const LineNumber& lineNumber)
{
  if (PreModule* m = interpreter.getModule(name))
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
PreModule::makeModule(const ModuleExpression* expr, ImportModule* enclosingModule)
{
  switch (expr->getType())
    {
    case ModuleExpression::MODULE:
      {
	Token name = expr->getModuleName();
	if (ImportModule* fm = getModule(name.code(), name.lineNumber()))
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
	    ImportModule* m = interpreter.makeRenamedCopy(fm, expr->getRenaming());
	    if (!(m->isBad()))
	      return m;
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
	  {
	    ImportModule* m = interpreter.makeSummation(fms);
	    if (!(m->isBad()))
	      return m;
	  }
	break;
      }
    case ModuleExpression::INSTANTIATION:
      {
	if (ImportModule* fm = makeModule(expr->getModule(), enclosingModule))
	  {
	    int nrParameters = fm->getNrParameters();
	    const Vector<Token>& arguments = expr->getArguments();
	    if (arguments.size() != nrParameters)
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
		if (View* v = interpreter.getView(code))
		  {
		    //
		    //	Instantiation is a view.
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
	    ImportModule* m = interpreter.makeInstatiation(fm, views, names);
	    if (!(m->isBad()))
	      return m;
	  }
	break;
      }
    default:
      CantHappen("bad module expression");
    }
  return 0;
}
