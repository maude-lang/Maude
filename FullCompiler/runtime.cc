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

#include <sys/time.h>

//      utility stuff
#include "macros.hh"
#include "vector.hh"
#include "pointerSet.hh"

#include "runtime.hh"
#define CHUNK_SIZE (1024 * 1024)
#define E_SIZE (1024 * 1024)
#define FACTOR 4

//template class Vector<void*>;

extern int arity[];
extern int comparison[];

void outputGraph(Node* node);
void depthFirstTraversal(Node* node, PointerSet& visited);
Node* inputGraph();

Context g;

//
//	Current half space
//
Vector<void*> currentSpace;
Vector<void*> endMarker;  // for breadth-first copying
int curChunk;
char* curNext;
char* curEnd;

//
//	Next half space
//
Vector<void*> newSpace;
//
//	Ephemeral space
//
Node* ephemeralStart;
Node* ephemeralEnd;

inline Node*
evacuate(Node* old)
  //
  //	Copy node to current space.
  //
{
  Flags flags = old->flags;
  if (flags.evacuated)
    return old->fwd;  // forwarding pointer
  int symbol = old->symbol;
  int nrArgs = arity[symbol];
  int nrBytes = sizeof(Node) + sizeof(Node*) * nrArgs;
  char* t = curNext + nrBytes;
  if (t > curEnd)
    {
      endMarker[curChunk] = reinterpret_cast<void*>(curNext);  // mark end of used portion
      ++curChunk;
      curNext = reinterpret_cast<char*>(currentSpace[curChunk]);
      curEnd = curNext + CHUNK_SIZE;
      t = curNext + nrBytes;
    }
  Node* n = reinterpret_cast<Node*>(curNext);
  curNext = t;
  n->symbol = symbol;
  n->flags = flags;
  for (int i = 0; i < nrArgs; i++)
    n->args[i] = old->args[i];
  const Flags c = {0, true};
  old->flags = c;
  old->fwd = n;
  return n;
}


char*
nextChunk(char* cn)
{
  endMarker[curChunk] = reinterpret_cast<void*>(cn);
  ++curChunk;
  return reinterpret_cast<char*>(currentSpace[curChunk]);
}

#define EVAC(old, dest, cn, ce) \
{ \
  Flags flags = old->flags; \
  if (flags.evacuated) \
    dest = old->fwd; \
  else \
    { \
      int symbol = old->symbol; \
      int nrArgs = arity[symbol]; \
      int nrBytes = sizeof(Node) + sizeof(Node*) * nrArgs; \
      char* w = cn + nrBytes; \
      if (w > ce) \
	{ \
	  cn = nextChunk(cn); \
	  ce = cn + CHUNK_SIZE; \
	  w = cn + nrBytes; \
	} \
      Node* m = reinterpret_cast<Node*>(cn); \
      cn = w; \
      m->symbol = symbol; \
      m->flags = flags; \
      for (int k = 0; k < nrArgs; k++) \
	m->args[k] = old->args[k]; \
      const Flags c = {0, true}; \
      old->flags = c; \
      old->fwd = m; \
      dest = m; \
    } \
}

void ephemeralGC();
void fullGC();

void
collectGarbage()
{
  if (curChunk + 1 < currentSpace.length())
    ephemeralGC();
  else
    fullGC();
  //
  //	reuse ephemeral space.
  //
  g.memNext = reinterpret_cast<char*>(ephemeralStart);
}

void 
ephemeralGC()
{  
  //
  //	keep track of where we start putting copies.
  //
  int copyStartChunk = curChunk;
  char* copyStart = curNext;

  char *cn = curNext;
  char *ce = curEnd;
  //
  //	scan arg list for pointers into ephemeral space.
  //
  int nrArgs = g.nrArgs;
  for (int i = 0; i < nrArgs; i++)
    {
      Node* t = g.args[i];
      if (t >= ephemeralStart && t < ephemeralEnd)
	EVAC(t, g.args[i], cn, ce)
    }

  //
  //	scan stack for pointers into ephemeral space.
  //
  for (Link* s = g.safePtr; s != 0; s = s[0].l)
    {
      int nrSlots = s[1].i;
      for (int i = 2; i < nrSlots + 2; i++)
	{
	  Node* t = s[i].n;
	  if (t != 0 && t >= ephemeralStart && t < ephemeralEnd)
	    EVAC(t, s[i].n, cn, ce)
	}
    }

  //
  //	breadth-first scan of copies for pointers into ephemeral space.
  //
  for (int i = copyStartChunk; i <= curChunk; i++)
    {
      char* p = static_cast<char*>((i == copyStartChunk) ? copyStart : currentSpace[i]);
      for (;;)
	{
	  if (i == curChunk)
	    {
	      if (p == cn)
		break;
	    }
	  else
	    {
	      if (p == endMarker[i])
		break;
	    }
	  Node* n = reinterpret_cast<Node*>(p);
	  int nrArgs = arity[n->symbol];
	  p += sizeof(Node) + sizeof(Node*) * nrArgs;
	  for (int j = 0; j < nrArgs; j++)
	    {
	      Node* t = n->args[j];
	      if (t >= ephemeralStart && t < ephemeralEnd)
		EVAC(t, n->args[j], cn, ce)
	    }
	}
    }

  curNext = cn;
  curEnd = ce;
}

void 
fullGC()
{
  //
  //	Set up new current space.
  //
  currentSpace.swap(newSpace);
  curChunk = 0;
  curNext = reinterpret_cast<char*>(currentSpace[0]);
  curEnd = curNext + CHUNK_SIZE;
 
  //
  //	scan arg list for pointers.
  //
  int nrArgs = g.nrArgs;
  for (int i = 0; i < nrArgs; i++)
    g.args[i] = evacuate(g.args[i]);
  
  //
  //	scan stack for pointers.
  //
  for (Link* s = g.safePtr; s != 0; s = s[0].l)
    {
      int nrSlots = s[1].i;
      for (int i = 2; i < nrSlots + 2; i++)
	{
	  Node* t = s[i].n;
	  if (t != 0)
	    s[i].n = evacuate(t);
	}
    }

  //
  //	breadth-first scan of copies for pointers.
  //
  for (int i = 0; i <= curChunk; i++)
    {
      for (char* p = reinterpret_cast<char*>(currentSpace[i]);;)
	{
	  if (i == curChunk)
	    {
	      if (p == curNext)
		break;
	    }
	  else
	    {
	      if (p == endMarker[i])
		break;
	    }
	  Node* n = reinterpret_cast<Node*>(p);
	  int nrArgs = arity[n->symbol];
	  p += sizeof(Node) + sizeof(Node*) * nrArgs;
	  for (int j = 0; j < nrArgs; j++)
	    n->args[j] = evacuate(n->args[j]);
	}
    }

  int used = (curChunk + 1) * CHUNK_SIZE - (curEnd - curNext);
  int allocated = newSpace.length() * CHUNK_SIZE;
  int needed = FACTOR * used;
  while (needed > allocated)
    {
      currentSpace.append(malloc(CHUNK_SIZE));
      newSpace.append(malloc(CHUNK_SIZE));
      void* t = 0;
      endMarker.append(t);
      allocated += CHUNK_SIZE;
    }
  
  //
  //	Keep length newSpace = length currentSpace + 1
  //
  int t = currentSpace.length() - 1;
  newSpace.append(currentSpace[t]);
  currentSpace.contractTo(t);
}

static itimerval init = { {1000000, 0}, {1000000, 0} };
static itimerval result;
static itimerval result2;

void
initMem()
{
  ephemeralStart = static_cast<Node*>(malloc(E_SIZE));
  ephemeralEnd = ephemeralStart + E_SIZE / sizeof(Node);

  currentSpace.append(malloc(CHUNK_SIZE));
  currentSpace.append(malloc(CHUNK_SIZE));
  void* t = 0;
  endMarker.append(t);
  endMarker.append(t);
  curChunk = 1;
  curNext = static_cast<char*>(currentSpace[0]);
  curEnd = curNext + CHUNK_SIZE;

  newSpace.append(malloc(CHUNK_SIZE));
  newSpace.append(malloc(CHUNK_SIZE));
  newSpace.append(malloc(CHUNK_SIZE));

  g.safePtr = 0;
  g.count = 0;
  g.memNext = reinterpret_cast<char*>(ephemeralStart);
  g.memEnd = reinterpret_cast<char*>(ephemeralEnd);
}

int
main(/* int argc, char* argv[] */)
{
  initMem();
  Node* n = inputGraph();
  
  setitimer(ITIMER_REAL, &init, 0);
  setitimer(ITIMER_PROF, &init, 0);

  n = eval(n);

  getitimer(ITIMER_PROF, &result);
  getitimer(ITIMER_REAL, &result2);

  outputGraph(n);

  /*
  //outputGraph(inputGraph());
  outputGraph(eval(inputGraph()));

  for (;;)
    {
      Node* n = inputGraph();
      //      if (n == 0)
      //	break;
      n = eval(n);
      outputGraph(n);
    }
  */
}

void
outputGraph(Node* node)
{
  ofstream ofile(inFileName);

  const Int64 M = 1000000;
  ofile << g.count << ' ' <<
    (M * (init.it_value.tv_sec - result.it_value.tv_sec) +
     init.it_value.tv_usec - result.it_value.tv_usec) << ' ' <<
    (M * (init.it_value.tv_sec - result2.it_value.tv_sec) +
     init.it_value.tv_usec - result2.it_value.tv_usec) << '\n';
  ofile << node->flags.sortIndex << '\n';
  PointerSet visited;
  depthFirstTraversal(node, visited);
  int nrNodes = visited.cardinality();
  for (int i = 0; i < nrNodes; i++)
    {
      Node* n = reinterpret_cast<Node*>(visited.index2Pointer(i));
      int symbol = n->symbol;
      ofile << symbol;
      int nrArgs = arity[symbol];
      for (int j = 0; j < nrArgs; j++)
	ofile << ' ' << visited.pointer2Index(n->args[j]);
      ofile << '\n';
    }
}

void
depthFirstTraversal(Node* node, PointerSet& visited)
{ 
  int nrArgs = arity[node->symbol];
  for (int i = 0; i < nrArgs; i++)
    {
      Node* n = node->args[i];
      if (!(visited.contains(n)))
        depthFirstTraversal(n, visited);
    }
  visited.insert(node);
}

Node*
inputGraph()
{
  ifstream ifile(outFileName);
  Vector<void*> built;
  for(;;)
    {
      int symbol;
      if(!(ifile >> symbol))
	break;
      int nrArgs = arity[symbol];
      // cerr << "symbol = " << symbol << "  arity = " << nrArgs << '\n';
      Node* r = reinterpret_cast<Node*>(g.memNext);
      char* n = g.memNext + sizeof(Node) + nrArgs * sizeof(Node*);
      if (n > g.memEnd)
	{
	  collectGarbage();
	  r = reinterpret_cast<Node*>(g.memNext);
	  g.memNext += sizeof(Node) + nrArgs * sizeof(Node*);
	}
      else
	g.memNext = n;
      r->symbol = symbol;
      const Flags c = {0, false};
      r->flags = c;
      for (int i = 0; i < nrArgs; i++)
	{
	  int t;
	  ifile >> t;
	  r->args[i] = reinterpret_cast<Node*>(built[t]);
	}
      built.append(r);
    }
  return reinterpret_cast<Node*>(built[built.length() - 1]);
}

int
compare(Node* n1, Node* n2)
{
  for (;;)
    {
      if (n1 == n2)
	return 0;
      int s1 = n1->symbol;
      int s2 = n2->symbol;
      if (s1 != s2)
	return comparison[s1] - comparison[s2];
      int nrArgs = arity[s1];
      if (nrArgs == 0)
	return 0;
      --nrArgs;
      for (int i = 0; i < nrArgs; i++)
	{
	  int t = compare(n1->args[i], n2->args[i]);
	  if (t != 0)
	    return t;
	}
      n1 = n1->args[nrArgs];
      n2 = n2->args[nrArgs];
    }
}
