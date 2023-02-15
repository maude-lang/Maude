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
//	Code for supporting object-oriented syntactic sugar.
//

Sort*
ImportModule::findClassIdSort() const
{
  //
  //	We need to identify the sort that play the rope of Cid.
  //
  //	We do this by looking at operators with the object attribute and the
  //	ObjectConstructorSymbol op-hook.
  //
  set<Sort*> candidates;
  //
  //	Go through our object symbols.
  //
  const Vector<Symbol*>& symbols = getSymbols();
  const NatSet& objectSymbols = getObjectSymbols();
  for (int i : objectSymbols)
    {
      Symbol* s = symbols[i];
      SymbolType st = getSymbolType(s);
      if (st.getBasicType() == SymbolType::OBJECT_CONSTRUCTOR_SYMBOL)
	{
	  for (const OpDeclaration& d : s->getOpDeclarations())
	    {
	      if (d.isConstructor())
		{
		  Sort* sort = d.getDomainAndRange()[1];
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
    IssueWarning(*this  << ": unable to find a class id sort (usually " << QUOTE("Cid") << ") in " << QUOTE(this) << ".");
  else
    {
      ComplexWarning(*this  << ": unable to find a unique class id sort (usually " << QUOTE("Cid") <<
		     ") in " << QUOTE(this) << ". Candidates are:");
      for (Sort* s : candidates)
	cerr << ' ' << QUOTE(s);
      cerr << endl;
    }
  return 0;
}

Sort*
ImportModule::findAtttributeSort() const
{
  //
  //	We need to identify the sort that plays the rope of Attribute.
  //
  //	We first identify the set of sorts that could be AttributeSet
  //	We do this by looking at operators with the object attribute and the
  //	ObjectConstructorSymbol op-hook.
  //
  set<Sort*> attributeSetCandidates;
  //
  //	Go through our object symbols.
  //
  const Vector<Symbol*>& symbols = getSymbols();
  const NatSet& objectSymbols = getObjectSymbols();
  for (int i : objectSymbols)
    {
      Symbol* s = symbols[i];
      SymbolType st = getSymbolType(s);
      if (st.getBasicType() == SymbolType::OBJECT_CONSTRUCTOR_SYMBOL)
	{
	  for (const OpDeclaration& d : s->getOpDeclarations())
	    {
	      if (d.isConstructor())
		{
		  Sort* sort = d.getDomainAndRange()[2];
		  if (sort->index() != Sort::KIND)
		    attributeSetCandidates.insert(sort);
		}
	    }
	}
    }
  //
  //	Next, every immediate subsort of a potential AttributeSet sort is
  //	a potential Attribute sort.
  //
  set<Sort*> attributeCandidates;
  for (Sort* as : attributeSetCandidates)
    {
      const ConnectedComponent* component = as->component();
      //
      //	We get the set of subsorts.
      //
      NatSet indices = as->getLeqSorts();
      //
      //	Remove the AttributeSet candidate itself.
      //
      indices.subtract(as->index());
      while (!indices.empty())
	{
	  //
	  //	The sort with the least index cannot be a subsort of any of the remaining sorts
	  //	sot it must be a candidate for Attribute.
	  //
	  int candidateIndex = indices.min();
	  attributeCandidates.insert(component->sort(candidateIndex));
	  //
	  //	We remove all of its subsorts; if there are any sorts left, at least one of them
	  //	will be a candidate for attribute.
	  //
	  indices.subtract(component->getLeqSorts(candidateIndex));
	}
    }
  //
  //	We succeed if there is a unique candidate.
  //
  if (attributeCandidates.size() == 1)
    return *(attributeCandidates.begin());
  if (attributeCandidates.empty())
    IssueWarning(*this  << ": unable to find an attribute sort (usually " << QUOTE("Attribute") << ") in " << QUOTE(this) << ".");
  else
    {
      ComplexWarning(*this  << ": unable to find a unique attribute sort (usually " << QUOTE("Attribute") <<
		     ") in " << QUOTE(this) << " . Candidates are:");
      for (Sort* s : attributeCandidates)
	cerr << ' ' << QUOTE(s);
      cerr << endl;
    }
  return 0;
}
