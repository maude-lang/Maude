//
//	Code for expanding the assignments in a level.
//

bool
WordLevel::fullyExpandAssignments()
{
  //
  //	Expand assignments to fixed-point.
  //
  for (;;)
    {
      Result result = expandAssignments();
      if (result == FAIL)
	return false;
      if (result == DONE)
	break;
    }
  return true;
}

WordLevel::Result
WordLevel::expandAssignments()
{
  //
  //	Expand each assignment.
  //
  bool changed = false;
  int nrAssignments = partialSolution.size();
  for (int i = 0; i < nrAssignments; ++i)
    {
      Result result = expandAssignment(i, partialSolution[i]);
      if (result == FAIL)
	return FAIL;
      if (result == CHANGED)
	changed = true;
    }
  return changed ? CHANGED : DONE;
}

WordLevel::Result
WordLevel::expandAssignment(int var, Word& word)
{
  //
  //	Check if assignment needs expansion, i.e. that a variable in
  //	range has an assignment different from itself.
  //
  FOR_EACH_CONST(i, Word, word)
    {
      int var2 = *i;
      if (var2 == var)
	return word.size() == 1 ? DONE : FAIL;  // either identity mapping or occur-check failure

      Word& assigned = partialSolution[var2];
      if (!(assigned.size() == 1 && assigned[0] == var2))
	return reallyExpandAssignment(var, word, i, assigned) ? CHANGED : FAIL;
    }
  return DONE;
}

bool
WordLevel::reallyExpandAssignment(int var,
				  Word& word,
				  Word::const_iterator firstToExpand,
				  const Word& expansion)
{
  //
  //	Do the actual expansion; return false if there was an occur-check failure.
  //
  Word newWord;
  //
  //	Copy in any variables that didn't need expansion.
  //
  for (Word::const_iterator i = word.begin(); i != firstToExpand; ++i)
    newWord.append(*i);
  //
  //	Copy in the assignment of the first variable that needed expansion.
  //
  if (!append(newWord, expansion, var))
    return false;
  //
  //	Got through remaining variables, expanding those that are assigned non-identity values.
  //
  const Word::const_iterator e = word.end();
  for (++firstToExpand; firstToExpand != e; ++firstToExpand)
    {
      int var2 = *firstToExpand;
      if (var2 == var)
	return false;  // occur-check failure

      Word& assigned = partialSolution[var2];
      if (assigned.size() == 1 && assigned[0] == var2)
	newWord.append(var2);
      else
	{
	  if (!(append(newWord, assigned, var)))
	    return false;
	}
    }
  //
  //	Replace the range with its expansion.
  //
  word.swap(newWord);
  return true;
}

bool
WordLevel::append(Word& newWord, const Word& word, int var)
{
  FOR_EACH_CONST(i, Word, word)
    {
      int var2 = *i;
      if (var2 == var)
	return false;  // occur-check failure
      newWord.append(var2);
    }
  return true;
}
