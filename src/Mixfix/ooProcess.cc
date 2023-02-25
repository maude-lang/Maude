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
SyntacticPreModule::addHonoraryClassNames(ImportModule* import, set<int>& classNames) const
{
  static const Vector<ConnectedComponent*> dummy;

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

void
SyntacticPreModule::addHonoraryClassNames(set<int>& classNames) const
{
  //
  //	An honorary class name is the name of a sort Foo from an imported module
  //	that looks as if it could have been produced from a class.
  //
  //	This information is needed for subclass processing, before operators
  //	have been imported so we need to examine parameter copies and imported modules.
  //
  int nrParameters = flatModule->getNrParameters();
  for (int i = 0; i < nrParameters; ++i)
    addHonoraryClassNames(flatModule->getParameterTheoryCopy(i), classNames);

  int nrImports = flatModule->getNrImports();
  for (int i = 0; i < nrImports; ++i)
    addHonoraryClassNames(flatModule->getImportedModule(i), classNames);
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
	      //	Name must end in in attributeSuffix
	      //
	      if (hasAttributeSuffix(s))
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
		  DebugInfo("module " << flatModule << " adding honorary attribute symbol " << s);
		  attributeSymbols.insert(s);
		}
	    }
	}
    nextSymbol:
      ;
    }
}

Sort*
SyntacticPreModule::findClassIdSort() const
{
  //
  //	We need to identify the sort that plays the role of Cid.
  //	We do this by looking at operators with the object attribute and the
  //	ObjectConstructorSymbol op-hook.
  //	However, we need to do this identification before we close the sort set so
  //	we will not have imported operators or processed our operator declarations yet
  //	and thus we can not use flatModule->findClassIdSort(). Instead
  //	we need to examine the modules we will import and our operator declarations.
  //	We ignore polymorphic operators.
  //
  set<Sort*> candidates;
  //
  //	First we go through the modules we are going to import.
  //
  int nrImports = flatModule->getNrImports();
  for (int i = 0; i < nrImports; ++i)
    {
      set<Sort*> candidatesFromImport;
      flatModule->getImportedModule(i)->insertClassIdSortCandidates(candidatesFromImport);
      for (Sort* importedSort : candidatesFromImport)
	{
	  Sort* localSort = flatModule->findSort(importedSort->id());
	  Assert(localSort != 0, "couldn't find " << importedSort << " in " << this);
	  candidates.insert(localSort);
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
	{
	  //
	  //	We expect that all regular sorts have been imported so we
	  //	just do a regular lookup.
	  //
	  int name = opDef.types[1].tokens[0].code();  // only sort of second argument type
	  Sort* sort = flatModule->findSort(name);
	  Assert(sort != 0, "sort doesn't exist");
	  candidates.insert(sort);
	}
    }
  //
  //	We succeed if there is a unique candidate.
  //
  return flatModule->uniqueClassIdSortCandidate(candidates);
}

void
SyntacticPreModule::processClassSorts()
{
  classIdSort = findClassIdSort();
  if (classIdSort == 0)
    {
      flatModule->markAsBad();
      return;
    }
  //
  //	Try to add a subsort for each class.
  //
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
	      localClasses.insert({code, SymbolSet()});
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
SyntacticPreModule::checkAttributes()
{
  const ConnectedComponent* classComponent = classIdSort->component();
  int nrSorts = classComponent->nrSorts();
  for (int i = 1; i < nrSorts; ++i)
    {
      Sort* sort = classComponent->sort(i);
      int name = sort->id();
      auto localClass = localClasses.find(name);
      if (localClass != localClasses.end())
	{
	  //
	  //	We have a local class.
	  //
	  const NatSet& leqSorts = sort->getLeqSorts();
	  for (int j : leqSorts)
	    {
	      if (j == i)
		continue;  // don't count ourselves
	      Sort* leqSort = classComponent->sort(j);
	      int leqSortName = leqSort->id();
	      auto subclass = localClasses.find(leqSortName);
	      if (subclass != localClasses.end())
		{
		  //
		  //	We have a local subclass.
		  //
		  for (Symbol* s : localClass->second)
		    {
		      if (subclass->second.find(s) != subclass->second.end())
			{
			  IssueAdvisory(*leqSort << ": class " << QUOTE(leqSort) << " declares an attribute "
					<< QUOTE(ATTRIBUTE(s)) << " that it inherits from class " << QUOTE(sort) << ".");
			}
		    }
		}
	    }
	}
    }
}

void
SyntacticPreModule::purgeImpureClasses()
{

  //
  //	Impurity percholates down from sorts that are not Cid or above it.
  //	Between pure classes, attributes percholate down.
  //	At the end of this processing we will just be left with pure classes, each with
  //	a complete set of attribute symbols.
  //
  const ConnectedComponent* classComponent = classIdSort->component();
  int nrSorts = classComponent->nrSorts();
  for (int i = 1; i < nrSorts; ++i)
    {
      if (leq(classIdSort, i))
	{
	  //
	  //	Sort is Cid or above Cid so ignore it.
	  //
	  continue;
	}
      Sort* sort = classComponent->sort(i);
      const NatSet& leqSorts = sort->getLeqSorts();
      int name = sort->id();

      auto localClass = localClasses.find(name);
      if (localClass != localClasses.end())
	{
	  //
	  //	It's a pure class (and can't be purged since we've already examined
	  //	all lower index sorts) so distribute its attributes to its subclasses
	  //	which could still be purged if they inherit from an impure class.
	  //
	  //Verbose("Class " << sort << " is pure with " << localClass->second.size() << " attributes.");
	  for (int j : leqSorts)
	    {
	      Sort* leqSort = classComponent->sort(j);
	      int leqSortName = leqSort->id();
	      auto subclass = localClasses.find(leqSortName);
	      if (subclass != localClasses.end())
		subclass->second.insert(localClass->second.begin(), localClass->second.end());
	    }
	}
      else
	{
	  //
	  //	We have an impure class or a stray sort; it purges everything below it.
	  //
	  for (int j : leqSorts)
	    {
	      Sort* leqSort = classComponent->sort(j);
	      localClasses.erase(leqSort->id());
	    }
	}
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
  attributeSort = flatModule->findAtttributeSort();
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
  if (isTheory())
    classSymbolType.setFlags(SymbolType::PCONST);
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
	  (void) flatModule->addOpDeclaration(c.name,
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
		  localClasses[c.name.code()].insert(attrSymbol);
		}
	    }
	}
    }
}
