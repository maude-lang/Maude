/*

    This file is part of the Maude 3 interpreter.

    Copyright 2023 SRI International, Menlo Park, CA 94025, USA.

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
//	Code for processing object-oriented syntactic sugar.
//

void
SyntacticPreModule::addHonoraryClassNames(set<int>& classNames) const
{
  //
  //	An honorary class name is a sort Foo from an imported module
  //	that looks as if it could have been produced from a class.
  //
  //	This information is needed for subclass processing, before operators
  //	have been imported so we need to examine imported modules.
  //
  Vector<ConnectedComponent*> dummy;
  int nrImports = flatModule->getNrImports();
  for (int i = 0; i < nrImports; ++i)
    {
      //
      //	We only look at system modules and theories.
      //
      ImportModule* import = flatModule->getImportedModule(i);
      if (import->isSystem())
	{
	  //
	  //	Imported module must have a Cid sort to be considered.
	  //
	  Sort* cid = import->findSort(classIdSort->id());
	  if (cid)
	    {
	      //
	      //	We only consider sorts below Cid.
	      //
	      ConnectedComponent* c = cid->component();
	      for (int index : cid->getLeqSorts())
		{
		  Sort* sort = c->sort(index);
		  int name = sort->id();
		  //
		  //	Must have a constant of the same name in the Cid component.
		  //
		  Symbol* symbol = import->findSymbol(name, dummy, c);
		  if (symbol != 0)
		    {
		      //
		      //	Symbol must have a single declaration.
		      //
		      const Vector<OpDeclaration>& opDecls = symbol->getOpDeclarations();
		      if (opDecls.size() == 1)
			{
			  //
			  //	Declaration must be a ctor.
			  //
			  const OpDeclaration& opDecl = opDecls[0];
			  if (opDecl.isConstructor())
			    {
			      const Vector<Sort*>& domainAndRange = opDecls[0].getDomainAndRange();
			      //
			      //	Declaration must put it in the candidate sort.
			      //
			      if (domainAndRange[0] == sort)
				classNames.insert(name);
			    }
			}
		    }
		}
	    }
	}
    }
}

void
SyntacticPreModule::addHonoraryAttributeSymbols()
{
  //
  //	Go through our flat module.
  //
  for (Symbol* s : flatModule->getSymbols())
    {
      //
      //	We only look at symbols with standard basic type, without certain flags.
      //
      SymbolType st = flatModule->getSymbolType(s);
      constexpr int forbiddenFlags = SymbolType::AXIOMS | SymbolType::ITER |
	SymbolType::STRAT | SymbolType::MEMO | SymbolType::FROZEN |
	SymbolType::CONFIG | SymbolType::OBJECT | SymbolType::MESSAGE;
      if (st.getBasicType() == SymbolType::STANDARD && (st.getFlags() & forbiddenFlags) == 0)
	{
	  //
	  //	Must be a unary symbol.
	  //
	  if (s->arity() == 1)
	    {
	      //
	      //	Name must end in :_
	      //
	      const char* name = Token::name(s->id());
	      int len = strlen(name);
	      if (len > 2 && name[len - 2] == ':' && name[len - 1] == '_')
		{
		  //
		  //	We allow subsort overloading of attributes so there could be many
		  //	declarations.
		  //
		  for (const OpDeclaration& opDecl : s->getOpDeclarations())
		    {
		      //
		      //	Each declaration must be a ctor.
		      //
		      if (!opDecl.isConstructor())
			goto nextSymbol;
		      //
		      //	Each declaration must put it in the attribute sort.
		      //
		      const Vector<Sort*>& domainAndRange = opDecl.getDomainAndRange();
		      if (domainAndRange[1] != attributeSort)
			goto nextSymbol;
		    }
		  //
		  //	Treat it as an attribute symbol.
		  //
		  attributeSymbols.insert(s);
		}
	    }
	}
    nextSymbol:
      ;
    }
}

Sort*
SyntacticPreModule::findClassIdSortName() const
{
  //
  //	We need to identify the sort that plays the role of Cid.
  //	We do this by looking at operators with the object attribute and the
  //	ObjectConstructorSymbol op-hook.
  //	However, we need to do this identification before we close the sort set so
  //	we will not have imported operators or processed our operator declarations yet
  //	so we need to example the modules we will import and our operator declarations.
  //	We ignore polymorphic operators.
  //
  set<int> candidates;
  //
  //	First we go through the modules we are going to import.
  //
  int nrImports = flatModule->getNrImports();
  for (int i = 0; i < nrImports; ++i)
    {
      const ImportModule* import = flatModule->getImportedModule(i);
      const NatSet& objectSymbols = import->getObjectSymbols();
      if (!objectSymbols.empty())
	{
	  //
	  //	If the module has object symbols we examine them.
	  //
	  const Vector<Symbol*>& symbols = import->getSymbols();
	  for (int j : objectSymbols)
	    {
	      Symbol* s = symbols[j];
	      SymbolType st = import->getSymbolType(s);
	      if (st.getBasicType() == SymbolType::OBJECT_CONSTRUCTOR_SYMBOL)
		{
		  //
		  //	An object symbol with the ObjectConstructorSymbol hook, so every second argument sort
		  //	in a declaration is a potential class id sort name.
		  //
		  for (const OpDeclaration& d : s->getOpDeclarations())
		    {
		      if (d.isConstructor())
			{
			  const Sort* sort = d.getDomainAndRange()[1];
			  if (sort->index() != Sort::KIND)
			    candidates.insert(sort->id());
			}
		    }
		}
	    }
	}
    }
  //
  //	Next we go through our operator declarations (which have yet to be processed).
  //	In fact, since we are only looking for a sort name and don't care about the object
  //	operator name, it is sufficient just to look through operator definitions.
  //
  for (const OpDef& opDef : opDefs)
    {
      if (opDef.symbolType.hasAllFlags(SymbolType::OBJECT | SymbolType::CTOR) &&	// has object and ctor attributes
	  opDef.symbolType.getBasicType() == SymbolType::OBJECT_CONSTRUCTOR_SYMBOL &&	// ObjectConstructorSymbol op-hook
	  opDef.polyArgs.empty() &&				// not polymorphic
	  opDef.types.size() == 4 &&				// 3 arguments
	  !opDef.types[1].kind)					// 2nd argument is not a kind
	candidates.insert(opDef.types[1].tokens[0].code());	// only sort of second argument type
    }
  //
  //	We succeed if there is a unique candidate.
  //
  if (candidates.size() == 1)
    {
      int classIdSortName = *(candidates.begin());
      //
      //	We expect that all regular sorts have been imported so we
      //	just do a regular lookup.
      //
      Sort* classIdSort = flatModule->findSort(classIdSortName);
      Assert(classIdSort != 0, "classSort doesn't exist");
      return classIdSort;
    }
  if (candidates.empty())
    IssueWarning("Unable to find a class id sort.");
  else
    {
      ComplexWarning("Unable to find a unique class id sort. Candidates are:");
      for (int i : candidates)
	cerr << ' ' << QUOTE(Token::name(i));
      cerr << endl;
    }
  return 0;
}

Sort*
SyntacticPreModule::findAtttributeSetSort() const
{
  //
  //	We need to identify the sort that play the rope of AttributeSet.
  //	We do this by looking at operators with the object attribute and the
  //	ObjectConstructorSymbol op-hook.
  //	At this point, the sort set is closed and all the operators other than
  //	the ones we will create of attributes and late symbols have been inserted.
  //
  set<Sort*> candidates;
  //
  //	Go through our flat module.
  //
  const Vector<Symbol*>& symbols = flatModule->getSymbols();
  const NatSet& objectSymbols = flatModule->getObjectSymbols();
  for (int i : objectSymbols)
    {
      Symbol* s = symbols[i];
      SymbolType st = flatModule->getSymbolType(s);
      if (st.getBasicType() == SymbolType::OBJECT_CONSTRUCTOR_SYMBOL)
	{
	  for (const OpDeclaration& d : s->getOpDeclarations())
	    {
	      if (d.isConstructor())
		{
		  Sort* sort = d.getDomainAndRange()[2];
		  if (sort->index() != Sort::KIND)
		    candidates.insert(sort);
		}
	    }
	}
    }
  //
  //	We succeed if there is a unique candidate.
  //
  if (candidates.size() == 1)
    return *(candidates.begin());
  if (candidates.empty())
    IssueWarning("Unable to find an attribute set sort.");
  else
    {
      ComplexWarning("Unable to find a unique attribute sort. Candidates are:");
      for (Sort* s : candidates)
	cerr << ' ' << QUOTE(s);
      cerr << endl;
    }
  return 0;
}

Sort*
SyntacticPreModule::findAtttributeSort() const
{
  set<int> candidates;
  const ConnectedComponent* component = attributeSetSort->component();
  NatSet indices = attributeSetSort->getLeqSorts();
  indices.subtract(attributeSetSort->index());
  while (!indices.empty())
    {
      int candidateIndex = indices.min();
      candidates.insert(candidateIndex);
      indices.subtract(component->getLeqSorts(candidateIndex));
    }
  if (candidates.size() == 1)
    return component->sort(*(candidates.begin()));
  if (candidates.empty())
    IssueWarning("Unable to find an attribute sort.");
  else
    {
      ComplexWarning("Unable to find a unique attribute set sort. Candidates are:");
      for (int i : candidates)
	cerr << ' ' << QUOTE(component->sort(i));
      cerr << endl;
    }
  return 0;
}

void
SyntacticPreModule::processClassSorts()
{
  classIdSort = findClassIdSortName();
  if (classIdSort == 0)
    {
      flatModule->markAsBad();
      return;
    }
  //
  //	Try to add a subsort for each class.
  //
  //set<int> classNames;
  for (ClassDecl& c : classDecls)
    {
      if (c.name.containsUnderscore())
	IssueWarning(LineNumber(c.name.lineNumber()) << ": underscore not allowed in class name " << QUOTE(c.name) << ".");
      else
	{
	  int code = c.name.code();
	  classNames.insert(code);
	  c.classSort = flatModule->findSort(code);
	  if (c.classSort == 0)
	    {
	      c.classSort = flatModule->addSort(code);
	      c.classSort->setLineNumber(c.name.lineNumber());
	    }
	  else
	    {
	      IssueWarning(LineNumber(c.name.lineNumber()) <<
			   ": class name clashes with existing sort " << QUOTE(c.classSort) << '.');
	    }
	  classIdSort->insertSubsort(c.classSort);
	}
    }
  //
  //	Handle subclasses.
  //
  addHonoraryClassNames(classNames);
  Vector<Sort*> smaller;
  Vector<Sort*> bigger;
  for (const Vector<Token>& subclassDecl : subclassDecls)
    {
      for (const Token& token : subclassDecl)
	{
	  if (token.code() == lessThan)
	    {
	      insertSubsorts(smaller, bigger);
	      smaller.swap(bigger);
	      bigger.clear();
	    }
	  else
	    {
	      int code = token.code();
	      if (classNames.find(code) == classNames.end())
		IssueWarning(LineNumber(token.lineNumber()) << ": " << QUOTE(token) << " in subclass declaration is not a class.");
	      else
		{
		  Sort* sort = flatModule->findSort(code);
		  Assert(sort != 0, "couldn't find sort for class " << token);  // because we just added it
		  bigger.append(sort);
		}
	    }
	}
      insertSubsorts(smaller, bigger);
      smaller.clear();
      bigger.clear();
    }
}

void
SyntacticPreModule::checkAttributeTypes()
{
  for (ClassDecl& c : classDecls)
    {
      for (AttributePair& ap : c.attributes)
	checkType(ap.type);
    }
}

void
SyntacticPreModule::computeAttributeTypes()
{
  for (ClassDecl& c : classDecls)
    {
      for (AttributePair& ap : c.attributes)
	ap.sort = computeType(ap.type);
    }
}

void
SyntacticPreModule::processClassOps()
{
  attributeSetSort = findAtttributeSetSort();
  if (attributeSetSort == 0)
    {
      flatModule->markAsBad();
      return;
    }
  attributeSort = findAtttributeSort();
  if (attributeSort == 0)
    {
      flatModule->markAsBad();
      return;
    }
  addHonoraryAttributeSymbols();
  //
  //	Values for attribute operators.
  //
  SymbolType attributeSymbolType;
  attributeSymbolType.setFlags(SymbolType::CTOR | SymbolType::GATHER);
  Vector<Sort*> domainAndRange(2);
  domainAndRange[1] = attributeSort;
  Vector<int> attributeGather(1);
  attributeGather[0] = MixfixModule::GATHER_AMP;
  //
  //	Values for class operators.
  //
  SymbolType classSymbolType;
  classSymbolType.setFlags(SymbolType::CTOR);
  Vector<Sort*> range(1);
  //
  //	Dummy values.
  //
  Vector<int> dummyVec;
  NatSet dummySet;
  bool dummyBool;
  for (ClassDecl& c : classDecls)
    {
      if (!c.name.containsUnderscore())
	{
	  range[0] = c.classSort;
	  c.classSymbol = flatModule->addOpDeclaration(c.name,
						       range,
						       classSymbolType,
						       dummyVec,
						       dummySet,
						       DEFAULT,
						       dummyVec,
						       dummyVec,
						       NONE,
						       dummyBool);
	  for (AttributePair& ap : c.attributes)
	    {
	      if (ap.attributeName.containsUnderscore())
		{
		  IssueWarning(LineNumber(ap.attributeName.lineNumber()) << ": underscore not allowed in attribute name " <<
			       QUOTE(ap.attributeName) << ".");
		}
	      else
		{
		  domainAndRange[0] = ap.sort;
		  string name(ap.attributeName.name());
		  name += attributeSuffix;
		  Token opName;
		  opName.tokenize(name.c_str(), ap.attributeName.lineNumber());
		  Symbol* attrSymbol = flatModule->addOpDeclaration(opName,
								      domainAndRange,
								      attributeSymbolType,
								      dummyVec,
								      dummySet,
								      DEFAULT,
								      attributeGather,
								      dummyVec,
								      NONE,
								      dummyBool);
		  attributeSymbols.insert(attrSymbol);
		}
	    }
	}
    }
}
