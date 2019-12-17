/*

    This file is part of the Maude 3 interpreter.

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
//      Detailed analysis of a preregularity or constuctor consistancy issue to impress the user.
//
#include <gmpxx.h>
#include <map>
//#include <iterator>

struct SortTable::Node
{
  mpz_class pathCount;
  int parent;
  int sortIndex;
};

void
SortTable::sortErrorAnalysis(bool preregProblem,
			     const set<int>& badTerminals)
{
  //copy(badTerminals.begin(), badTerminals.end(), ostream_iterator<const int>(cout, " "));   
  //cout << endl;
  //
  //	First we build a spanning tree with a path count, parent node and sort index
  //	(from parent) for each node. Nonterminal nodes are named by their start index
  //	in the diagram vector while terminals are named by the absolute index of the
  //	terminal in the diagram vector.
  //
  const Vector<int>& diagram = preregProblem ? sortDiagram : ctorDiagram;
  typedef map<int, Node> SpanningTree;
  SpanningTree spanningTree;
  spanningTree[0].pathCount = 1;
  spanningTree[0].parent = NONE;
  spanningTree[0].sortIndex = NONE;
  mpz_class product = 1;
  mpz_class badCount = 0;
  Vector<int> firstBad(nrArgs);

  set<int> currentNodes;
  currentNodes.insert(0);

  for (int i = 0; i < nrArgs; i++)
    {
      set<int> nextNodes;
      const ConnectedComponent* component = componentVector[i];
      int nrSorts = component->nrSorts();
      product *= nrSorts;

      FOR_EACH_CONST(j, set<int>, currentNodes)
	{
	  int parent = *j;
	  mpz_class& pathCount = spanningTree[parent].pathCount;

	  for (int k = 0; k < nrSorts; k++)
	    {
	      int index = parent + k;
	      if (i == nrArgs - 1)
		{
		  //
		  //	Terminal node - see if it is bad.
		  //
		  if (badTerminals.find(index) != badTerminals.end())
		    {
		      //cerr << " pathCount = " << pathCount << endl;
		      if (badCount == 0)
			{
			  badCount = pathCount;
			  firstBad[nrArgs - 1] = k;
			  int n = parent;
			  for (int l = nrArgs - 2; l >= 0; --l)
			    {
			      SpanningTree::const_iterator t = spanningTree.find(n);
			      Assert(t != spanningTree.end(), "missing node");
			      firstBad[l] = t->second.sortIndex;
			      n = t->second.parent;
			    }
			  //copy(firstBad.begin(), firstBad.end(), ostream_iterator<const int>(cout, " "));   
			  //cout << endl;

			}
		      else
			badCount += pathCount;
		    }
		}
	      else
		{
		  //
		  //	Nonterminal node - generate next nodes.
		  //
		  int target = diagram[index];
		  pair<SpanningTree::iterator, bool> p =
		    spanningTree.insert(SpanningTree::value_type(target, Node()));
		  if (p.second)
		    {
		      p.first->second.pathCount = pathCount;
		      p.first->second.parent = parent;
		      p.first->second.sortIndex = k;
		      nextNodes.insert(target);
		    }
		  else
		    p.first->second.pathCount += pathCount;
		}
	    }
	}
      currentNodes.swap(nextNodes);
    }

  ComplexWarning((preregProblem ? "sort" : "constructor") <<
	       " declarations for operator " <<
		 QUOTE(safeCastNonNull<Symbol*>(this)) << " failed " <<
	       (preregProblem ? "preregularity" : "constructor consistency") <<
	       " check on " << badCount << " out of " << product <<
	       " sort tuples. First such tuple is (");
  for (int i = 0; i < nrArgs; i++)
    {
      cerr << QUOTE(componentVector[i]->sort(firstBad[i]));
      if (i != nrArgs - 1)
	cerr << ", ";
    }
  cerr << ")." << endl;
}
