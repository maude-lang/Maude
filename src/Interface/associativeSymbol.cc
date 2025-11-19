/*

    This file is part of the Maude 3 interpreter.

    Copyright 1997-2025 SRI International, Menlo Park, CA 94025, USA.

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
//      Implementation for class AssociativeSymbol.
//
#include <map>

//	utility stuff
#include "macros.hh"
#include "vector.hh"
#include "pointerSet.hh"

//      forward declarations
#include "interface.hh"
#include "core.hh"
#include "variable.hh"

//      interface class definitions
#include "associativeSymbol.hh"
#include "term.hh"

//      core class definitions
#include "argumentIterator.hh"
#include "dagArgumentIterator.hh"
#include "sortConstraint.hh"

//
//	This can't be local to a function because we're not allowed to
//	instantiate template classes on local types.
//
struct AssociativeSymbol::Inv
{
  int sortIndex;
  int count;
};

AssociativeSymbol::AssociativeSymbol(int id,
				     const Vector<int>& strategy,
				     bool memoFlag,
				     Term* identity)
  : BinarySymbol(id, memoFlag, identity)
{
  setPermuteStrategy(strategy);
}

void 
AssociativeSymbol::finalizeSortInfo()
{
  BinarySymbol::finalizeSortInfo();  // do parent classes stuff
  //
  //	These things are dependent on sort constraints and hence can't be done
  //	correctly until all sort constraints that could apply to our symbol
  //	have been determined.
  //
  associativeSortCheck();
  associativeCtorCheck();
  associativeSortBoundsAnalysis();
  associativeSortStructureAnalysis();
}

void
AssociativeSymbol::processIdentity()
{
  BinarySymbol::processIdentity();
  Term* identity = getIdentity();
  WarningCheck(identity == 0 || identity->symbol() != this,
	       *identity << ": associative operator " << QUOTE(this) <<
	       " occurs on top of its own identity.");
}

bool
AssociativeSymbol::mightCollapseToOurSymbol(const Term* subterm) const
{
  const PointerSet& cs = subterm->collapseSymbols();
  int nrSymbols = cs.cardinality();
  for (int i = 0; i < nrSymbols; i++)
    {
      Symbol* s = static_cast<Symbol*>(cs.index2Pointer(i));
      if (static_cast<const Symbol*>(s) == this)
        return true;
      VariableSymbol* vs = dynamic_cast<VariableSymbol*>(s);
      if (vs != 0 && sortBound(vs->getSort()) > 1)
        return true;
    }
  return false;
}

bool
AssociativeSymbol::checkUniformity(const Sort* uniformSort, int nrSorts)
{
  int uniformIndex = uniformSort->index();
  for (int i = 1; i < nrSorts; i++)
    {
      bool good = leq(i, uniformSort);
      int step = traverse(0, i);
      for (int j = 1; j < nrSorts; j++)
	{
	  int resultIndex = traverse(step, j);
	  if (good && leq(j, uniformSort))
	    {
	      if (resultIndex != uniformIndex)
		return false;
	    }
	  else
	    {
	      if (resultIndex != Sort::ERROR_SORT)
		return false;
	    }
	}
    }
  return true;
}

void
AssociativeSymbol::associativeSortCheck()
{
  const ConnectedComponent* component = rangeComponent();
  Assert(domainComponent(0) == component && domainComponent(1) == component,
	 "Associative operator " << this <<
	 " has a domain sort in a different connected component from its range sort");
  uniSort = 0;

  if (kindLevelDeclarationsOnly())
    return;
  WarningCheck(getSortConstraints().empty(),
	       "membership axioms are not guaranteed to work correctly for associative symbol " <<
	       QUOTE(this) << " as it has declarations that are not at the kind level.");

  int nrSorts = component->nrSorts();
  //
  //	Check to see if symbol has a uniform sort table.
  //	(in which case it is automatically  associative).
  //	We do this by taking the singleNonErrorSort if it has one
  //	and checking it for uniformity.
  //
  Sort* candidate = getSingleNonErrorSort();
  if (candidate != 0 && checkUniformity(candidate, nrSorts))
    {
      uniSort = candidate;
      return;
    }
  //
  //   Sort table is not uniform so check all triples to see if it is associative.
  //
  int nrBadTriples = 0;
  int bad1 = 0;
  int bad2 = 0;
  int bad3 = 0;
  int badLeftAssocResult = 0;
  int badRightAssocResult = 0;
  {
    //
    //	The naive O(n^3) algorithm can be slow when the number of user sorts
    //	n is large. This more complicated approach is O(k.n^2) where k is the
    //	width of the sort diagram - often a lot less than n.
    //
    typedef map<int, Inv> InvMap;
    InvMap invMap;
    //
    //	First invert the first step of the sort diagram.
    //
    for (int i = 0; i < nrSorts; i++)
      {
	int step = traverse(0, i);
	pair<InvMap::iterator, bool> p = invMap.insert(InvMap::value_type(step, Inv()));
	if (p.second)
	  {
	    p.first->second.sortIndex = i;
	    p.first->second.count = 1;
	  }
	else
	  ++(p.first->second.count);
      }
    //
    //	Now for each possible first step, we check the n^2 possible triples.
    //
    const InvMap::const_iterator e = invMap.end();
    for (InvMap::const_iterator i = invMap.begin(); i != e; ++i)
      {
	int step_i = i->first;
	for (int j = 0; j < nrSorts; j++)
	{
	  int step_j = traverse(0, j);
	  int step_ij = traverse(0, traverse(step_i, j));
	  for (int k = 1; k < nrSorts; k++)
	    {
	      int leftAssocResult = traverse(step_ij, k);
	      int rightAssocResult = traverse(step_i, traverse(step_j, k));
	      if (leftAssocResult != rightAssocResult)
		{
		  if (nrBadTriples == 0)
		    {
		      bad1 = i->second.sortIndex;
		      bad2 = j;
		      bad3 = k;
		      badLeftAssocResult = leftAssocResult;
		      badRightAssocResult = rightAssocResult;
		    }
		  nrBadTriples += i->second.count;
		}
	    }
	}
      }
  }
  WarningCheck(nrBadTriples == 0,
	       "sort declarations for associative operator " << QUOTE(this) <<
	       " are non-associative on " << nrBadTriples <<
	       " out of " << nrSorts * nrSorts *  nrSorts << 
	       " sort triples. First such triple is (" <<
	       QUOTE(component->sort(bad1)) << ", " << 
	       QUOTE(component->sort(bad2)) << ", " << 
	       QUOTE(component->sort(bad3)) << "):\n  " <<
	       //
	       QUOTE(this) << "(" << QUOTE(this) << "(" <<
	       "X:" << QUOTE(component->sort(bad1)) << ", " <<
	       "X:" << QUOTE(component->sort(bad2)) << "), " <<
	       "X:" << QUOTE(component->sort(bad3)) << ")\nhas sort " <<
	       QUOTE(component->sort(badLeftAssocResult)) << ", while\n  " <<
	       //
	       QUOTE(this) << "(" << "X:" << QUOTE(component->sort(bad1)) << ", " <<
	       QUOTE(this) << "(X:" << QUOTE(component->sort(bad2)) << ", " <<
	       "X:" << QUOTE(component->sort(bad3)) << "))\nhas sort " <<
	       QUOTE(component->sort(badRightAssocResult)) << "."
	       );
}

void
AssociativeSymbol::associativeCtorCheck()
{
  if (getCtorStatus() != SortTable::IS_COMPLEX)
    return;  // trivial case - either all constructors or all non-constructors
  //
  //	In order for all members of a congruence class to have the same ctor it is
  //	necessary and sufficient (as long as s_f is associative) that:
  //
  //	(for all sorts x, y, z).
  //      [c_f(s_f(x, y), z) /\ c_f(x, y)) <=> c_f(x, s_f(y, z)) /\ c_f(y, z))]
  //
  //	where s_f is the sorting function and c_f is the constructor function.
  //
  const ConnectedComponent* component = rangeComponent();
  int nrSorts = component->nrSorts();


  int nrBadTriples = 0;
  int bad1 = 0;
  int bad2 = 0;
  int bad3 = 0;
  Index badLeftAssocStatus = 0;
  Index badRightAssocStatus = 0;
  //
  //	We hope that the number of first steps in the sort diagram times the number
  //	of first steps in the ctor diagram is less than the number of sorts, so we
  //	win by inverting the product of the first steps in both diagrams.
  //
  {
    typedef pair<int, int> IntPair;  // (sortStep, ctorStep)
    typedef map<IntPair, Inv> InvMap;
    InvMap invMap;
    //
    //	First invert the first step of the sort and ctor diagrams.
    //
    for (int x = 0; x < nrSorts; x++)
      {
	int sortStep = traverse(0, x);
	int ctorStep = ctorTraverse(0, x);

	pair<InvMap::iterator, bool> p = invMap.insert(InvMap::value_type(IntPair(sortStep, ctorStep), Inv()));
	if  (p.second)
	  {
	    p.first->second.sortIndex = x;
	    p.first->second.count = 1;
	  }
	else
	  ++(p.first->second.count);
      }

    const InvMap::const_iterator e = invMap.end();
    for (InvMap::const_iterator i = invMap.begin(); i != e; ++i)
      {
	int sx = i->first.first;
	int cx = i->first.second;
	for (int y = 0; y < nrSorts; ++y)
	  {
	    int sy = traverse(0, y);
	    int cy = ctorTraverse(0, y);
	    int sxy = traverse(sx, y);
	    int cxy = ctorTraverse(cx, y);
	    int csxy = ctorTraverse(0, sxy);

	    for (int z = 0; z < nrSorts; ++z)
	      {
		int syz = traverse(sy, z);
		int cyz = ctorTraverse(cy, z);
		int csxy_z = ctorTraverse(csxy, z);
		int cx_syz = ctorTraverse(cx, syz);
		bool leftAssocCtor = csxy_z && cxy;
		bool rightAssocCtor = cx_syz && cyz;
		if (leftAssocCtor != rightAssocCtor)
		  {
		    if (nrBadTriples == 0)
		      {
			bad1 = i->second.sortIndex;
			bad2 = y;
			bad3 = z;
			badLeftAssocStatus = cxy | (csxy_z << 1);
			badRightAssocStatus = cyz | (cx_syz << 1);
		      }		
		    nrBadTriples += i->second.count;
		  }
	      }
	  }
      }
  }

  const char* const outcomes[] = {
    "neither operator is a constructor",
    "only the inner operator is a constructor",
    "only the outer operator is a constructor",
    "both operators are constructors"
  };
  WarningCheck(nrBadTriples == 0,
	       "ctor declarations for associative operator " << QUOTE(this) <<
	       " conflict on " << nrBadTriples <<
	       " out of " << nrSorts * nrSorts *  nrSorts << 
	       " sort triples. First such triple is (" <<
	       QUOTE(component->sort(bad1)) << ", " << 
	       QUOTE(component->sort(bad2)) << ", " << 
	       QUOTE(component->sort(bad3)) << ").\nIn\n  " <<
	       //
	       QUOTE(this) << "(" << QUOTE(this) << "(" <<
	       "X:" << QUOTE(component->sort(bad1)) << ", " <<
	       "X:" << QUOTE(component->sort(bad2)) << "), " <<
	       "X:" << QUOTE(component->sort(bad3)) << ")\n" <<
	       outcomes[badLeftAssocStatus] << ", while in\n  " <<
	       //
	       QUOTE(this) << "(" << "X:" << QUOTE(component->sort(bad1)) << ", " <<
	       QUOTE(this) << "(X:" << QUOTE(component->sort(bad2)) << ", " <<
	       "X:" << QUOTE(component->sort(bad3)) << "))\n" <<
	       outcomes[badRightAssocStatus] << "."
	       );
}

void
AssociativeSymbol::insertGreaterOrEqualSorts(const Sort* sort, NatSet& set)
{
  int index = sort->index();
  for (int i = 1; i <= index; i++)
    {
      if (leq(sort, i))
	set.insert(i);
    }
}

void
AssociativeSymbol::associativeSortBoundsAnalysis()
{
  //
  //	If there is a sort constraint to some sort s then s and all larger sorts
  //	must always be unbounded
  //
  const Vector<SortConstraint*>& sc = getSortConstraints();
  int nrSortConstraints = sc.length();
  static NatSet unbounded;
  unbounded.makeEmpty();
  for (int i = 0; i < nrSortConstraints; i++)
    insertGreaterOrEqualSorts(sc[i]->getSort(), unbounded);
  //
  //	Start off with all sorts unbounded
  //
  const ConnectedComponent* component = rangeComponent();
  int nrSorts = component->nrSorts();
  sortBounds.expandTo(nrSorts);
  for (int i = 0; i < nrSorts; i++)
    sortBounds[i] = UNBOUNDED;
  //
  //	Now for each possible bound; 1, 2, 3, ... we check to see which sorts are
  //	too big because of their occurrence in the regularity table. When a sort
  //	is too big, all bigger sorts are also too big
  //
  int largestBound = 1;
  for (int i = 1; i <= largestBound; i++)
    {
      static NatSet tooBig;
      tooBig = unbounded;
      for (int j = 1; j < nrSorts; j++)
	{
	  int step = traverse(0, j);
	  int jBound = sortBounds[j];
	  for (int k = 1; k < nrSorts; k++)
	    {
	      int kBound = sortBounds[k];
	      if (jBound == UNBOUNDED || kBound == UNBOUNDED || jBound + kBound > i)
		{
		  int resultIndex = traverse(step, k);
		  if (resultIndex != Sort::ERROR_SORT && !(tooBig.contains(resultIndex)))
		    insertGreaterOrEqualSorts(component->sort(resultIndex), tooBig);
		}
	    }
	}
      for (int j = 1; j < nrSorts; j++)
	{
	  if (!(tooBig.contains(j)) && sortBounds[j] == UNBOUNDED)
	    {
	      sortBounds[j] = i;
	      largestBound = 2 * i;
	    }
	}
    }
}

void 
AssociativeSymbol::associativeSortStructureAnalysis()
{
  const Vector<SortConstraint*>& sc = getSortConstraints();
  int nrSortConstraints = sc.length();
  const ConnectedComponent* component = rangeComponent();
  int nrSorts = component->nrSorts();
  int nrMaxSorts = component->nrMaximalSorts();
  sortStructures.expandTo(nrSorts);
  sortStructures[0] = PURE_SORT;  // error sorts are always pure
  //
  //	We examine each non-error sort s in turn
  //
  for (int i = 1; i < nrSorts; i++)
    {
      const Sort* sort = component->sort(i);
      //
      //	First check where it has the limit property:
      //	  s1, s2 <= s ===> s_f(s1, s2) <= s
      //
      for (int j = i; j < nrSorts; j++)
	{
	  if (leq(j, sort))
	    {
	      int step = traverse(0, j);
	      for (int k = i; k < nrSorts; k++)
		{
		  if (leq(k, sort) && !(leq(traverse(step, k), sort)))
		    {
		      sortStructures[i] = UNSTRUCTURED;
		      goto nextSort;
		    }
		}
	    }
	}
      //
      //	First check to see if a sort constraint destroys the purity
      //	of a non-maximal, non-error sort.
      //
      if (i > nrMaxSorts)  // maximal sorts are 1...nrMaxSorts
	{
	  for (int j = 0; j < nrSortConstraints; j++)
	    {
	      if (leq(sc[j]->getSort(), sort))
		{
		  sortStructures[i] = LIMIT_SORT;
		  goto nextSort;
		}
	    }
	}
      //
      //	Now check main part of purity property:
      //	  s_f(s1, s2) <= s ===> s1, s2 <= s
      //
      for (int j = 1; j < nrSorts; j++)
	{
	  bool j_leq = leq(j, sort);
	  int step = traverse(0, j);
	  for (int k = 1; k < nrSorts; k++)
	    {
	      if ((!j_leq || !(leq(k, sort))) && leq(traverse(step, k), sort))
		{
		  sortStructures[i] = LIMIT_SORT;
		  goto nextSort;
		}
	    }
	}
      sortStructures[i] = PURE_SORT;
    nextSort:
      ;
    }
}

void
AssociativeSymbol::fillInSortInfo(Term* subject)
{
  Assert(this == subject->symbol(), "bad Symbol");
  ConnectedComponent* component = rangeComponent();  // should be const
  Assert(component != 0, "couldn't get component");

  ArgumentIterator a(*subject);
  Term* t = a.argument();
  t->symbol()->fillInSortInfo(t);
  Assert(t->getComponent() == component, "assoc component error");
  int si = t->getSortIndex();
#ifndef NO_ASSERT
  int nrArgsSeen = 1;
#endif
  for (a.next(); a.valid(); a.next())
    {
      Term* t = a.argument();
      t->symbol()->fillInSortInfo(t);
      Assert(t->getComponent() == component, "assoc component error");
      si = traverse(traverse(0, si), t->getSortIndex());
#ifndef NO_ASSERT
      ++nrArgsSeen;
#endif
    }
  Assert(nrArgsSeen >= 2, "bad # of args for assoc op");
  subject->setSortInfo(component, si);
}


bool
AssociativeSymbol::isConstructor(DagNode* subject)
{
  if (specialSortHandling())
    return false;  // HACK
  
  switch (getCtorStatus())
    {
    case SortTable::IS_CTOR:
      return true;
    case SortTable::IS_COMPLEX:
      {
	DagArgumentIterator a(*subject);
	int sortIndex = a.argument()->getSortIndex();
	Assert(sortIndex != Sort::SORT_UNKNOWN, "Unknown sort");

	for (a.next(); a.valid(); a.next())
	  {
	    int t = a.argument()->getSortIndex();
	    Assert(t != Sort::SORT_UNKNOWN, "Unknown sort");
	    int ctorStatus = ctorTraverse(ctorTraverse(0, sortIndex), t);
	    if (!ctorStatus)
	      return false;  // one non-ctor subterm make us non-ctor
	    sortIndex = traverse(traverse(0, sortIndex), t);
	  }
	return true;
      }
    default:
      break;
    }
  return false;
}

void
AssociativeSymbol::setFrozen(const NatSet& frozen)
{
  setPermuteFrozen(frozen);
}

ostream& operator<<(ostream& s, AssociativeSymbol::Structure structure)
{
  static const char* const names[] = {"UNSTRUCTURED",
				      "LIMIT_SORT",
				      "PURE_SORT"};
  s << names[structure];
  return s;
}
