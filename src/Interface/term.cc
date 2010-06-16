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
//	Implementation for base class Term.
//

//	utility stuff
#include "macros.hh"
#include "vector.hh"
#include "indent.hh"

//	forward declarations
#include "interface.hh"
#include "core.hh"
#include "fullCompiler.hh"

//	interface class definitions
#include "symbol.hh"
#include "dagNode.hh"
#include "term.hh"

//	core class definitions
#include "argumentIterator.hh"
#include "bindingLhsAutomaton.hh"
#include "termBag.hh"
#include "rhsBuilder.hh"
#include "trivialRhsAutomaton.hh"
#include "copyRhsAutomaton.hh"
#include "symbolMap.hh"

//	variable class definitions
#include "variableSymbol.hh"
#include "variableTerm.hh"
#include "variableInfo.hh"

//	full compiler class definitions
#include "compilationContext.hh"
#include "variableName.hh"

Vector<DagNode*> Term::subDags;
TermSet Term::converted;
bool Term::setSortInfoFlag;
bool Term::discard;

DagNode*
Term::term2Dag(bool setSortInfo)
{
  setSortInfoFlag = setSortInfo;
  subDags.clear();
  converted.makeEmpty();
  return dagify();
}

int
Term::compileRhs(RhsBuilder& rhsBuilder,
		 VariableInfo& variableInfo,
		 TermBag& availableTerms,
		 bool eagerContext)
{
  if (Term* t = availableTerms.findTerm(this, eagerContext))
    {
      //
      //	We have seen this term before either in a lhs or
      //	in a rhs (perhaps of a condition fragment) and we
      //	can reuse it. 
      //
      if (t->saveIndex == NONE)  // must be lhs term/variable
	{
	  if (VariableTerm* vt = dynamic_cast<VariableTerm*>(this))
	    return vt->getIndex();  // lhs varible
	  t->saveIndex = variableInfo.makeProtectedVariable();  // left->right sharing
	}
      return t->saveIndex;
    }
  if (VariableTerm* vt = dynamic_cast<VariableTerm*>(this))
    {
      //
      //	Variable must be either unbound (because we didn't see it during matching) OR
      //	we're in eager context and variable was only matched in a lazy context.
      //
      int varIndex = vt->getIndex();
      if (eagerContext)
	{
	  //
	  //	Since we're using an unbound or lazy variable in
	  //	an eager context we must arrange to copy its binding.
	  //
	  DebugAdvisory("made CopyRhsAutomaton for " << this);
	  int index = variableInfo.makeConstructionIndex();
	  rhsBuilder.addRhsAutomaton(new CopyRhsAutomaton(varIndex, index));
	  saveIndex = index;
	  availableTerms.insertBuiltTerm(this, true);
	  return index;  
	}
      return varIndex;  // unbound variable in lazy context
    }
  //
  //	Previously unseen non-variable term - let its theory compile it.
  //
  int index = compileRhs2(rhsBuilder, variableInfo, availableTerms, eagerContext);
  //
  //	Save the term and the index it was placed at so we can share its
  //	instantiation if we encounter an equal term later on.
  //
  saveIndex = index;
  availableTerms.insertBuiltTerm(this, eagerContext);
  return index;
}

void
Term::compileTopRhs(RhsBuilder& rhsBuilder,
		    VariableInfo& variableInfo,
		    TermBag& availableTerms)
{
  int index = compileRhs(rhsBuilder, variableInfo, availableTerms, true);
  variableInfo.useIndex(index);
  //
  //	If we don't have any automata we must create one, if only to do the
  //	replacement.
  //
  if (rhsBuilder.empty())
    rhsBuilder.addRhsAutomaton(new TrivialRhsAutomaton(index));
}

void
Term::indexVariables(VariableInfo& indicies)
{
  if (VariableTerm* vt = dynamic_cast<VariableTerm*>(this))
    {
      int index = indicies.variable2Index(vt);
      vt->setIndex(index);
      occursSet.insert(index);
    }
  else
    { 
      for (ArgumentIterator a(*this); a.valid(); a.next())
	{
	  Term* t = a.argument();
	  t->indexVariables(indicies);
	  occursSet.insert(t->occursSet);
	}
    }
}

int
Term::computeSize()
{
  if (cachedSize == UNDEFINED)
    {
      int size = 1;
      for (ArgumentIterator a(*this); a.valid(); a.next())
	size += a.argument()->computeSize();
      cachedSize = size;
    }
  return cachedSize;
}

void
Term::determineContextVariables()
{
  for (ArgumentIterator a(*this); a.valid(); a.next())
    {
      Term* t = a.argument();
      t->contextSet.insert(contextSet);  // insert parents context set
      for (ArgumentIterator b(*this); b.valid(); b.next())
	{
	  Term* u = b.argument();
	  if (u != t)
	    t->contextSet.insert(u->occursSet);  // insert siblings occurs set
	}
      t->determineContextVariables();
    }
}

void
Term::analyseCollapses()
{
  analyseCollapses2();
  if (dynamic_cast<VariableSymbol*>(topSymbol) == 0 && collapseSet.empty())
    flags |= STABLE;
}

void
Term::analyseCollapses2()
{
  for (ArgumentIterator a(*this); a.valid(); a.next())
    a.argument()->analyseCollapses();
}

void
Term::insertAbstractionVariables(VariableInfo& variableInfo)
{
  setHonorsGroundOutMatch(true);
  for (ArgumentIterator a(*this); a.valid(); a.next())
    {
      Term* t = a.argument();
      t->insertAbstractionVariables(variableInfo);
      if (!(t->honorsGroundOutMatch()))
	setHonorsGroundOutMatch(false);
    }
}

LhsAutomaton*
Term::compileLhs(bool matchAtTop,
		 const VariableInfo& variableInfo,
		 NatSet& boundUniquely,
		 bool& subproblemLikely)
{
  LhsAutomaton* a = compileLhs2(matchAtTop, variableInfo, boundUniquely, subproblemLikely);
  if (saveIndex != NONE)
    a = new BindingLhsAutomaton(saveIndex, a);
  return a;
}

bool
Term::earlyMatchFailOnInstanceOf(const Term* other) const
{
  if (stable() && other->stable() && topSymbol != other->topSymbol)
    return true;
  return false;
}

bool
Term::subsumes(const Term* other, bool sameVariableSet) const
{
  return false;
}

int
Term::partialCompareUnstable(const Substitution& /* partialSubstitution */,
			      DagNode* /* other */) const
{
  return UNDECIDED;
}

int
Term::partialCompareArguments(const Substitution& /* partialSubstitution */,
			      DagNode* /* other */) const
{
  return UNDECIDED;
}

void
Term::commonSymbols(Vector<Term*>& patterns, PointerSet& common)
{
  //
  //	Generate set of symbols that could be at the top of subject which matches
  //	all patterns. Patterns are assumed to have had their collapse analysis done
  //	and we assume they are in the same connected component.
  //	We make no assumption about where sort constraints have been indexed.
  //	If there in any doubt about some symbol we add it anyway.
  //	Variables are handled in a special way: a variable v which is in one patterns
  //	collapse set (or which is that pattern) is added if for each other pattern
  //	p_i there is some variable v_i which is p_i of is in p_i's collapse set
  //	with sort greater, equal or incomparable to v's sort.
  //
  Assert(common.empty(), "common not empty to start with");
  int nrPatterns = patterns.length();
  for (int i = 0; i < nrPatterns; i++)
    {
      Term* p = patterns[i];
      PointerSet cs(p->collapseSet);
      (void) cs.insert(p->topSymbol);
      int nrSymbols = cs.cardinality();
      for (int j = 0; j < nrSymbols; j++)
	{
	  Symbol* s = static_cast<Symbol*>(cs.index2Pointer(j));
	  if (commonWithOtherPatterns(patterns, i, s))
	    (void) common.insert(s);
	}
    }
}

bool
Term::commonWithOtherPatterns(Vector<Term*>& patterns, int excluded, Symbol* symbol)
{
  int nrPatterns = patterns.length();
  VariableSymbol* v = dynamic_cast<VariableSymbol*>(symbol);
  if (v == 0)
    {
      for (int i = 0; i < nrPatterns; i++)
	{
	  if (i != excluded && !(symbol->mightMatchPattern(patterns[i])))
	    return false;
	}
    }
  else
    {
      for (int i = 0; i < nrPatterns; i++)
	{
	  if (i != excluded && !hasGeqOrIncomparableVariable(patterns[i], v))
	    return false;
	}
    }
  return true;
}

bool
Term::hasGeqOrIncomparableVariable(Term* pattern, VariableSymbol* v)
{
  const Sort*s = v->getSort();
  VariableSymbol* v2 = dynamic_cast<VariableSymbol*>(pattern->topSymbol);
  if (v2 != 0)
    {
      const Sort* s2 = v2->getSort();
      return s2 == s || !::leq(s2, s);
    }
  const PointerSet& cs = pattern->collapseSet;
  int cardinality = cs.cardinality();
  for (int i = 0; i < cardinality; i++)
    {
      Symbol* s2 = static_cast<Symbol*>(cs.index2Pointer(i));
      v2 = dynamic_cast<VariableSymbol*>(s2);
      if (v2 != 0)
	{
	  const Sort* s2 = v2->getSort();
	  if (s2 == s || !::leq(s2, s))
	    return true;
	}
    }
  return false;
}

bool
Term::greedySafe(const VariableInfo& variableInfo, const NatSet& boundUniquely) const
{
  NatSet badVariables(variableInfo.getConditionVariables());
  badVariables.insert(contextSet);
  badVariables.intersect(occursSet);
  return boundUniquely.contains(badVariables);
}

Term*
Term::instantiate2(const Vector<Term*>& varBindings, SymbolMap* translator)
{
  //
  //	Naive implementation. Doesn't work for variables or any subclass
  //	with hidden data. Possibly inefficient in other cases.
  //
  Vector<Term*> args;
  for (ArgumentIterator a(*this); a.valid(); a.next())
    args.append(a.argument()->instantiate2(varBindings, translator));
  return translator->findTargetVersionOfSymbol(topSymbol)->makeTerm(args);
}

#ifdef COMPILER
int
Term::gatherPartialResults(int nrVariables,
			   TermSet& compiled,
			   Vector<Symbol*>& symbols,
			   Vector<Vector<int> >& argLists)
{
  VariableTerm* v = dynamic_cast<VariableTerm*>(this);
  if (v != 0)
    return v->getIndex();
  int source = compiled.term2Index(this);
  if (source >= 0)
    return source + nrVariables;
  //
  //	Need to generate new partial result
  //
  Symbol* s = symbol();
  Vector<int> args;
  for (ArgumentIterator a(*this); a.valid(); a.next())
    {
      int source = a.argument()->
	gatherPartialResults(nrVariables, compiled, symbols, argLists);
      args.append(source);
    }
  int index = symbols.length();
  compiled.insert(this);
  symbols.append(s);
  argLists.append(args);
  return index + nrVariables;
}

void
Term::generateRhs(CompilationContext& context,
		  int indentLevel,
		  const Vector<VariableName>& varNames,
		  Symbol* lhsSymbol)
{
  //
  //	First we generate the a convenient DAG representation of the term.
  //
  TermSet compiled;
  Vector<Symbol*> symbols;
  Vector<Vector<int> > argLists;
  int nrVariables = varNames.length();
  int index = gatherPartialResults(nrVariables, compiled, symbols, argLists);
  int nrPartialResults = symbols.length();
  if (nrPartialResults == 0)
    {
      //
      //	Handle bare variable case.
      //
      context.generateIncrement(indentLevel);
      context.body() << Indent(indentLevel) << "return " << varNames[index] << ";\n";
      return;
    }
  //
  //	Now find out which partial result is the last user of each
  //	variable and partial result.
  //
  int nrItems = nrVariables + nrPartialResults - 1;  // final partial result not used
  Vector<int> lastUse(nrItems);
  for (int i = 0; i < nrItems; i++)
    lastUse[i] = UNUSED;
  for (int i = 0; i < nrPartialResults; i++)
    {
      Vector<int> args = argLists[i];
      int nrArgs = args.length();
      for (int j = 0; j < nrArgs; j++)
	lastUse[args[j]] = i;
    }
  //
  //	Then we decide what pointers will need to be placed in safe slots
  //	across each function call.
  //
  Vector<Vector<int> > safeSlots(nrPartialResults - 1);
  if (nrPartialResults > 1)
    {
      Vector<int>& safe = safeSlots[0];
      for (int i = 0; i < nrVariables; i++)
	{
	  if (lastUse[i] > 0)
	    safe.append(i);
	}
    }
  for (int i = 1; i < nrPartialResults - 1; i++)
    {
      int prevResult = i - 1 + nrVariables;
      bool needToSavePrevResult = lastUse[prevResult] > i;
      Vector<int>& prevSaved = safeSlots[i - 1];
      Vector<int>& save = safeSlots[i];
      int nrPrevSaved = prevSaved.length();
      for (int j = 0; j < nrPrevSaved; j++)
	{
	  int ps = prevSaved[j];
	  if (ps == UNUSED)
	    save.append(UNUSED);
	  else
	    {
	      if (lastUse[ps] > i)
		save.append(ps);
	      else
		{
		  if (needToSavePrevResult)
		    {
		      save.append(prevResult);
		      needToSavePrevResult = false;
		    }
		  else
		    save.append(UNUSED);
		}
	    }
	}
      if (needToSavePrevResult)
	{
	  for (int j = 0; j < nrPrevSaved; j++)
	    {
	      if (save[j] == UNUSED)
		{
		  save[j] = prevResult;
		  needToSavePrevResult = false;
		  break;
		}
	    }
	}
      if (needToSavePrevResult)
	save.append(prevResult); // need extra slot
    }
  //
  //	Now we can generate code.
  //
  //context.body() << Indent(indentLevel) << "{\n";
  //
  //	Declare partial result variables.
  //
  for (int i = 0; i < nrPartialResults - 1; i++)
    context.body() << Indent(indentLevel + 1) << "Node* p" << i << ";\n";
  //
  //	Set up safe slots.
  //
  context.generateIncrement(indentLevel + 1);
  int nrSafeSlots = nrPartialResults >= 2 ? safeSlots[nrPartialResults - 2].length() : 0;
  context.setNrSafeSlots(nrSafeSlots);
  if (nrSafeSlots > 0)
    {
      context.body() << Indent(indentLevel + 1) << "safe[0].l = g.safePtr;\n";
      context.body() << Indent(indentLevel + 1) << "g.safePtr = safe;\n";
      context.body() << Indent(indentLevel + 1) << "safe[1].i = " << nrSafeSlots << ";\n";
      Vector<int>& firstSlots = safeSlots[0];
      int nrFirstSlots = firstSlots.length();
      for (int i = 0; i < nrSafeSlots; i++)
	{
	  context.body() << Indent(indentLevel + 1) << "safe[" << i + 2 << "].n = ";
	  if (i < nrFirstSlots)
	    context.body() << varNames[firstSlots[i]];
	  else
	    context.body() << '0';
	  context.body() << ";\n";
	}
    }
  //
  //	Generate first partial result.
  //
  {
    Symbol* symbol = symbols[0];
    int nrArgs = symbol->arity();
    Vector<int>& argList = argLists[0];
    context.body() << Indent(indentLevel + 1) << "{\n";
    if (nrPartialResults == 1 && symbol == lhsSymbol)
      {
	//
	//	Tail recursion case.
	//
	for (int i = 0; i < nrArgs; i++)
	  {
	    context.body() << Indent(indentLevel + 2) << 'a' << i << " = " <<
	      varNames[argList[i]] << ";\n";
	  }
	context.body() << Indent(indentLevel + 2) << "goto start;\n";
	context.setTailRecursive();
      }
    else
      {
	if (nrPartialResults > 1)
	  context.body() << Indent(indentLevel + 2) << "p0 =";
	else
	  context.body() << Indent(indentLevel + 2) << "return";
	context.body() << " f" << symbol->getIndexWithinModule() << '(';
	for (int i = 0; i < nrArgs; i++)
	  {
	    context.body() << varNames[argList[i]];
	    if (i + 1 < nrArgs)
	      context.body() << ", ";
	  }
	context.body() << ");\n";
      }
    context.body() << Indent(indentLevel + 1) << "}\n";
  }
  //
  //	Generate remaining partial results.
  //
  for (int i = 1; i < nrPartialResults; i++)
    {
      Symbol* symbol = symbols[i];
      int nrArgs = symbol->arity();
      Vector<int>& argList = argLists[i];
      Vector<int>& prevSaved = safeSlots[i - 1];
      int nrSlots = prevSaved.length();

      context.body() << Indent(indentLevel + 1) << "{\n";
      //
      //	Copy saved values needed for args into local 'b' variables.
      //
      int bCount = 1;
      for (int j = 0; j < nrSlots; j++)
	{
	  int contents = prevSaved[j];
	  if (contents != UNUSED)
	    {
	      int localCopy = NONE;
	      for (int k = 0; k < nrArgs; k++)
		{
		  if (argList[k] == contents)
		    {
		      if (localCopy == NONE)
			{
			  localCopy = bCount++;
			  context.body() << Indent(indentLevel + 2) << "Node* b" << localCopy <<
			    " = safe[" << j + 2 << "].n;\n";
			}
		      argList[k] = -localCopy;
		    }
		}
	    }
	}
      if (i < nrPartialResults - 1)
	{
	  //
	  //	Clear saves values no longer needed, and save previous
	  //	partial result if need after our function call.
	  //
	  Vector<int>& toSave = safeSlots[i];
	  for (int j = 0; j < nrSlots; j++)
	    {
	      int ps = prevSaved[j];
	      int ts = toSave[j];
	      if (ps != ts)
		{
		  if (ts == UNUSED)
		    context.body() << Indent(indentLevel + 2) << "safe[" << j + 2 << "].n = 0;\n";
		  else
		    {
		      context.body() << Indent(indentLevel + 2) << "safe[" << j + 2 <<
			"].n = p" << i - 1 << ";\n";
		    }
		}
	    }
	  if (toSave.length() > nrSlots)
	    {
	      context.body() << Indent(indentLevel + 2) << "safe[" << nrSlots + 2 <<
		"].n = p" << i - 1 << ";\n";
	    }
	  context.body() << Indent(indentLevel + 2) << 'p' << i <<  " =";
	}
      else
	{
	  if (nrSafeSlots > 0)
	    context.body() << Indent(indentLevel + 2) << "g.safePtr = safe[0].l;\n";
	  if (symbol == lhsSymbol)
	    {
	      //
	      //	Tail recursion case.
	      //
	      for (int j = 0; j < nrArgs; j++)
		{
		  context.body() << Indent(indentLevel + 2) << 'a' << j << " = ";
		  int a = argList[j];
		  if (a < 0)
		    context.body() << 'b' << -a;
		  else
		    context.body() << 'p' << i - 1;
		  context.body() << ";\n";
		}
	      context.body() << Indent(indentLevel + 2) << "goto start;\n";
	      context.body() << Indent(indentLevel + 1) << "}\n";
	      context.setTailRecursive();
	      break;
	    }
	  context.body() << Indent(indentLevel + 2) << "return";
	}
      context.body() << " f" << symbol->getIndexWithinModule() << '(';
      for (int j = 0; j < nrArgs; j++)
	{
	  int a = argList[j];
	  if (a < 0)
	    context.body() << 'b' << -a;
	  else
	    context.body() << 'p' << i - 1;
	  if (j + 1 < nrArgs)
	    context.body() << ", ";
	}
      context.body() << ");\n";
      context.body() << Indent(indentLevel + 1) << "}\n";
    }
  //  context.body() << Indent(indentLevel) << "}\n";
  return;
}
#endif

#ifdef DUMP
void
Term::dump(ostream& s, const VariableInfo& variableInfo, int indentLevel)
{
  s << Indent(indentLevel) << "Begin{Term}\n";
  ++indentLevel;
  dumpCommon(s, variableInfo, indentLevel);
  s << Indent(indentLevel) << "arguments:\n";
  ++indentLevel;
  for (ArgumentIterator a(*this); a.valid(); a.next())
    a.argument()->dump(s, variableInfo, indentLevel);
  s << Indent(indentLevel - 2) << "End{Term}\n";
}

void
Term::dumpCommon(ostream& s, const VariableInfo& variableInfo, int indentLevel)
{
  s << Indent(indentLevel) << "topSymbol = " << topSymbol <<
    "\thonorsGroundOutMatch = " << honorsGroundOutMatch() <<
    "\teagerContext = " << hasEagerContext() << '\n';
  if (sortIndex == Sort::SORT_UNKNOWN)
    s << Indent(indentLevel) << "sort information not valid\n";
  else
    s << Indent(indentLevel) << "sortIndex = " << sortIndex << '\n';
  s << Indent(indentLevel) << "occursSet = ";
  dumpVariableSet(s, occursSet, variableInfo);
  s << Indent(indentLevel) << "contextSet = ";
  dumpVariableSet(s, contextSet, variableInfo);
  s << Indent(indentLevel) << "collapseSet = ";
  dumpSymbolSet(s, collapseSet);
}

void
Term::dumpVariableSet(ostream& s,
		      const NatSet& variableSet,
		      const VariableInfo& variableInfo)
{
  int m = variableSet.max();
  for (int i = 0; i <= m; i++)
    {
      if (variableSet.contains(i))
	s << ' ' << variableInfo.index2Variable(i);
    }
  s << '\n';
}

void
Term::dumpSymbolSet(ostream& s, const PointerSet& symbolSet)
{
  int c = symbolSet.cardinality();
  for (int i = 0; i < c; i++)
    s << ' ' << static_cast<Symbol*>(symbolSet.index2Pointer(i));
  s << '\n';
}
#endif
