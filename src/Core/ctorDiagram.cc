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
//	Code for building the constructor diagram.
//

bool
SortTable::containsConstructor(const NatSet& state, bool& unique)
{
  bool seenCtor = false;
  bool seenNonCtor = false;
  const NatSet::const_iterator e = state.end();
  for (NatSet::const_iterator i = state.begin(); i != e; ++i)
    {
      if (opDeclarations[*i].isConstructor())
	seenCtor = true;
      else
	seenNonCtor = true;
    }
  unique = !(seenCtor && seenNonCtor);
  return seenCtor;
}

bool
SortTable::partlyMoreGeneral(const OpDeclaration& subsumer,
			     const OpDeclaration& victim,
			     int argNr)
{
  const Vector<Sort*>& s = subsumer.getDomainAndRange();
  const Vector<Sort*>& v = victim.getDomainAndRange();
  int nrArgs = s.length() - 1;
  for (int i = argNr; i < nrArgs; i++)
    {
      if (!(leq(v[i], s[i])))
	return false;
    }
  return true;
}

bool
SortTable::ctorSubsumes(const OpDeclaration& subsumer,
			const OpDeclaration& victim,
			int argNr)
{
  const Vector<Sort*>& s = subsumer.getDomainAndRange();
  const Vector<Sort*>& v = victim.getDomainAndRange();
  int nrArgs = s.length() - 1;
  //
  //	Check to see that we are equal on unchecked args.
  //
  for (int i = argNr; i < nrArgs; i++)
    {
      if (v[i] != s[i])
	return false;
    }
  //
  //	Check to see that we are <= on checked args.
  //
  bool strict = false;
  for (int i = 0; i < argNr; i++)
    {
      if (v[i] != s[i])
	{
	  if (leq(s[i], v[i]))
	    strict = true;
	  else
	    return false;
	}
    }
  //
  //	If ctor status differs we need strictness for subsumption.
  //
  return strict || (subsumer.isConstructor() == victim.isConstructor());
}

void
SortTable::minimizeWrtCtor(NatSet& alive, int argNr) const
{
  bool seenCtor = false;
  bool seenNonCtor = false;
  const NatSet::const_iterator e = alive.end();
  for (NatSet::const_iterator i = alive.begin(); i != e; ++i)
    {
      if (opDeclarations[*i].isConstructor())
	seenCtor = true;
      else
	seenNonCtor = true;
    }
  if (!seenCtor)
    alive.makeEmpty();
  else if (!seenNonCtor)
    {
      //
      //	We are going to be ctor in any declaration
      //	survives so ctor subsumes ctor if it is >=
      //	on unchecked arguments.
      //
      int min = alive.min();
      int max = alive.max();
      for (int i = min; i <= max; i++)
	{
	  if (alive.contains(i))
	    {
	      for (int j = min; j <= max; j++)
		{
		  if (j != i &&
		      alive.contains(j) &&
		      partlyMoreGeneral(opDeclarations[i],
					opDeclarations[j],
					argNr))
		    alive.subtract(j);
		}
	    }
	}
    }
  else
    {
      //
      //	Like can subsume like by being <= on checked
      //	arguments and = on unchecked args.
      //	For ctor to subsume non-ctor and viceversa
      //	we also need strict < on at least one checked
      //	argument.
      //
      int min = alive.min();
      int max = alive.max();
      for (int i = min; i <= max; i++)
	{
	  if (alive.contains(i))
	    {
	      for (int j = min; j <= max; j++)
		{
		  if (j != i &&
		      alive.contains(j) &&
		      ctorSubsumes(opDeclarations[i],
				   opDeclarations[j],
				   argNr))
		    alive.subtract(j);
		}
	    }
	}
    }
}

void
SortTable::buildCtorDiagram()
{
  //
  //	Start with all op declarations alive.
  //
  Vector<NatSet> currentStates(1);
  NatSet& all = currentStates[0];
  int nrDeclarations = opDeclarations.length();
  for (int i = nrDeclarations - 1; i >= 0; i--)
    all.insert(i);  // insert in reverse order for efficiency

  if (nrArgs == 0)
    {
      bool unique;
      ctorDiagram.append(containsConstructor(all, unique));
      WarningCheck(unique, "constructor declarations for constant " << QUOTE(safeCast(Symbol*, this)) <<
		   " are inconsistant.");
      return;
    }

  Vector<NatSet> nextStates;
  int currentBase = 0;
  set<int> badTerminals;
  for (int i = 0; i < nrArgs; i++)
    {
      const ConnectedComponent* component = componentVector[i];
      int nrSorts = component->nrSorts();
      int nrCurrentStates = currentStates.length();

      int nextBase = currentBase + nrSorts * nrCurrentStates;
      ctorDiagram.resize(nextBase);
      int nrNextSorts = (i == nrArgs - 1) ? 0 : componentVector[i + 1]->nrSorts();
      
      for (int j = 0; j < nrSorts; j++)
	{
	  Sort* s = component->sort(j);
	  NatSet viable;
	  for (int k = 0; k < nrDeclarations; k++)
	    {
	      if (leq(s, opDeclarations[k].getDomainAndRange()[i]))
		viable.insert(k);
	    }
	  for (int k = 0; k < nrCurrentStates; k++)
	    {
	      NatSet nextState(viable);
	      nextState.intersect(currentStates[k]);
	      int index = currentBase + k * nrSorts + j;
	      if (nrNextSorts == 0)
		{
		  minimizeWrtCtor(nextState, i + 1);
		  bool unique;
		  ctorDiagram[index] = containsConstructor(nextState, unique);
		  if (!unique)
		    badTerminals.insert(index);
		}
	      else
		{
		  minimizeWrtCtor(nextState, i + 1);
		  ctorDiagram[index] =
		    nextBase + nrNextSorts * findStateNumber(nextStates, nextState);
		}
	    }
	}
      currentStates.swap(nextStates);
      nextStates.contractTo(0);
      currentBase = nextBase;
    }
  if (!(badTerminals.empty()))
    sortErrorAnalysis(false, badTerminals);
}
