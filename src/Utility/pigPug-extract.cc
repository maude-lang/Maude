/*

    This file is part of the Maude 3 interpreter.

    Copyright 2019 SRI International, Menlo Park, CA 94025, USA.

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
//	Code for second phase, to extract a solution from a path.
//

int
PigPug::extractUnifier(Subst& unifier, ConstraintMap& constraintMap)
{
  //
  //	We found a successful sequence of moves.
  //	Now we need to convert it into a unifier.
  //
  UnificandStack::const_iterator lhs = lhsStack.begin();
  UnificandStack::const_iterator rhs = rhsStack.begin();
  int lhsIndex = 0;
  int rhsIndex = 0;
  //
  //	Start with the identity substitution.
  //
  unifier.resize(lastOriginalVariable + 1);
  for (int i = 0; i <= lastOriginalVariable; ++i)
    {
      Word& word = unifier[i];
      word.resize(1);
      word[0] = i;
    }
  //
  //	Go through the moves recorded in path, generating and composing
  //	the corresponding substitutions.
  //
  for (int move : path)
    {
      int lhsVar = lhs->word[lhsIndex];
      int rhsVar = rhs->word[rhsIndex];

      switch (move & ALL_MOVES)
	{
	case RHS_PEEL:
	  {
	    //
	    //	lhsVar |-> rhsVar lhsVar
	    //
	    //cout << "rhs peel: " << lhsVar << " |-> " << rhsVar << " " << lhsVar << endl;
	    if (!(compose2(unifier, lhsVar, rhsVar)))
	      return NONE;  // upperBound violation
	    ++rhsIndex;
	    break;
	  }
	case LHS_PEEL:
	  {
	    //
	    //	rhsVar |-> lhsVar rhsVar
	    //
	    //cout << "lhs peel: " << rhsVar << " |-> " << lhsVar << " " << rhsVar << endl;
	    if (!(compose2(unifier, rhsVar, lhsVar)))
	      return NONE;  // upperBound violation
	    ++lhsIndex;
	    break;
	  }
	case EQUATE:
	case EQUATE | FINAL:
	  {
	    if (move & RHS_ASSIGN)
	      {
		//
		//	rhsVar |-> lhsVar
		//
		//cout << "equate: " << rhsVar << " |-> " << lhsVar << endl;
		compose(unifier, rhsVar, lhsVar);
	      }
	    else
	      {
		//
		//	lhsVar |-> rhsVar (assuming they are distinct)
		//
		//cout << "equate: " << lhsVar << " |-> " << rhsVar << endl;
		if (lhsVar != rhsVar)
		  compose(unifier, lhsVar, rhsVar);
	      }
	    ++lhsIndex;
	    ++rhsIndex;
	    break;
	  }
	case LHS_TAKES_ALL:
	  {
	    //
	    //	lhsVar |-> (rest of rhs word)
	    //
	    //cout << "lhs takes all for " << lhsVar << endl;
	    if (!(composeFinal(unifier, lhsVar, rhs->word, rhsIndex)))
	      return NONE;  // upperBound violation
	    break;
	  }
	case RHS_TAKES_ALL:
	  {
	    //
	    //	rhsVar |-> (rest of lhs word)
	    //
	    //cout << "rhs takes all for " << rhsVar << endl;
	    if (!(composeFinal(unifier, rhsVar, lhs->word, lhsIndex)))
	      return NONE;  // upperBound violation
	    break;
	  }
	default:
	  CantHappen("unknown move " << move);
	}
      if (move & PUSH_LHS)
	{
	  ++lhs;
	  lhsIndex = 0;
	}
      if (move & PUSH_RHS)
	{
	  ++rhs;
	  rhsIndex = 0;
	}
    }
  //
  //	Now find out what variables show up in range of substitution.
  //
  NatSet occursInRange;
  collectRangeVariables(unifier, occursInRange);
  //
  //	Then look for original variables that map to something other than themselves, and
  //	if they occur in the range, rename them.
  //
  int nextVariableName = freshVariableStart;
  VariableRenaming variableRenaming(lastOriginalVariable + 1);
  //
  //	We start by initializing the constraintMap with a deep copy
  //	of the original constraint map we were given; this may include
  //	constraints for variables that didn't occur in the equation we
  //	just solved.
  //
  constraintMap = constraintStack.front();
  const ConstraintMap& finalConstraintMap = constraintStack.back();
  for (int i = 0; i <= lastOriginalVariable; ++i)
    {
      const Word& word = unifier[i];
      if (word.size() == 1 && word[0] == i)
	{
	  //
	  //	X |-> X
	  //	Still need to rename it to itself and pass back the
	  //	latest (and hence tightest) constraint.
	  //
	  variableRenaming[i] = i;
	  constraintMap[i] = finalConstraintMap[i];
	  DebugInfo("self-mapping for x" << i << " gets " << finalConstraintMap[i]);
	}
      else
	{
	  //
	  //	X |-> Y1 .... Yk
	  //
	  if (occursInRange.contains(i))
	    {
	      //
	      //	X appears in the rhs of some assignment; but it's
	      //	not really the same X, so we rename the rhs version.
	      //
	      variableRenaming[i] = nextVariableName;
	      DebugInfo("non-self mapping for x" << i <<
			" generates new name x" << nextVariableName <<
			" with constraint " << finalConstraintMap[i]);

	      ++nextVariableName;
	      constraintMap.append(finalConstraintMap[i]);
	    }
	  else
	    {
	      //
	      //	Variable doesn't appear in the rhs. We can leave
	      //	its renaming uninitialized.
	      //
	    }
	}
    }

  renameVariables(unifier, variableRenaming);
  return nextVariableName;
}

void
PigPug::compose(Subst& subst, int oldVar, int replacement)
{
  //
  //	Compose subst with { oldVar |-> replacement }.
  //
  for (int i = 0; i <= lastOriginalVariable; ++i)
    {
      for (int& v : subst[i])
	{
	  if (v == oldVar)
	    v = replacement;
	}
    }
}

bool
PigPug::compose2(Subst& subst, int oldVar, int replacement)
{
  //
  //	Compose subst with { oldVar |-> replacement oldVar }.
  //
  const ConstraintMap& constraintMap = constraintStack.front();
  for (int i = 0; i <= lastOriginalVariable; ++i)
    {
      Word& word = subst[i];
      int wordLength = word.size();
      for (int j = 0; j < wordLength; ++j)
	{
	  if (word[j] == oldVar)
	    {
	      Word newWord(j + 2);
	      for (int k = 0; k < j; ++k)
		newWord[k] = word[k];
	      newWord[j] = replacement;
	      newWord[j + 1] = oldVar;
	      for (int k = j + 1; k < wordLength; ++k)
		{
		  int var = word[k];
		  if (var == oldVar)
		    newWord.append(replacement);
		  newWord.append(var);
		}
	      int newWordLength = newWord.size();
	      int upperBound = constraintMap[i].getUpperBound();
	      if (upperBound != 0 && newWordLength > upperBound)
		{
		  //Verbose("compose2(): variable " << i << " has upperBound of " << upperBound <<
		  //	    " vs newWordLength of " << newWordLength);
		  return false;
		}
	      word.swap(newWord);
	      break;
	    }
	}
    }
  return true;
}

bool
PigPug::composeFinal(Subst& subst, int oldVar, const Word& replacement, int index)
{
  //
  //	Compose subst with { var |-> suffix } where suffix
  //	is the suffix of replacement starting at index.
  //
  const ConstraintMap& constraintMap = constraintStack.front();
  int replacementLength = replacement.size();
  for (int i = 0; i <= lastOriginalVariable; ++i)
    {
      Word& word = subst[i];
      int wordLength = word.size();
      for (int j = 0; j < wordLength; ++j)
	{
	  if (word[j] == oldVar)
	    {
	      Word newWord(j);
	      for (int k = 0; k < j; ++k)
		newWord[k] = word[k];
	      for (int k = j ; k < wordLength; ++k)
		{
		  int var = word[k];
		  if (var == oldVar)
		    {
		      for (int l = index; l < replacementLength; ++l)
			newWord.append(replacement[l]);
		    }
		  else
		    newWord.append(var);
		}
	      int newWordLength = newWord.size();
	      int upperBound = constraintMap[i].getUpperBound();
	      if (upperBound != 0 && newWordLength > upperBound)
		{
		  //Verbose("compose(): variable " << i << " has upperBound of " << upperBound <<
		  //	    " vs newWordLength of " << newWordLength);
		  return false;
		}
	      word.swap(newWord);
	      break;
	    }
	}
    }
  return true;
}

void
PigPug::collectRangeVariables(const Subst& subst, NatSet& occursInRange)
{
  for (const Word& word : subst)
    {
      for (int var : word)
	occursInRange.insert(var);
    }
}

void
PigPug::renameVariables(Subst& subst, const VariableRenaming& variableRenaming)
{
  for (int i = 0; i <= lastOriginalVariable; ++i)
    {
      for (int& v : subst[i])
	v = variableRenaming[v];
    }
}
