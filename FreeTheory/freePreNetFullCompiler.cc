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
//	Routines to generate C++ code from a FreePreNet
//

void
FreePreNet::allocateVariables(int nodeNr)
{
  //
  //	Allocated slots (which will become temporary pointer variables
  //	r0, r1, r2,... in the final code) for this node and its decendents.
  //
  Node& n = net[nodeNr];
  ++n.nrParents;
  if (n.nrParents > 1)  // already done in on different path
    return;
  if (n.reducedFringe.empty())
    {
      //
      //	Remainder node; first make recursive call on child node.
      //
      int sonNr = n.nextPattern;
      if (sonNr == NONE)
	{
	  if (!(patterns[n.patternIndex].flags & UNFAILING))
	    ++nrFailParents;
	}
      else
	{
	  allocateVariables(sonNr);
	  slotMapUnion(n.slotMap, sonNr);
	}
      //
      //	For each nonfree subterm (exception: variables that are linear,
      //	unused in condition or rhs and are have the maximal sort in an
      //	error free component are ignored) under the skeleton we know that
      //	we tested it's parent and we may have tested the subterm itself.
      //	We now allocate slots so these nonfree subterm are accessable
      //	for matching/sort checking/assignment etc.
      //
      Vector<FreeSubterm>& subterms = patterns[n.patternIndex].subterms;
      int nrSubterms = subterms.length();
      for (int i = 0; i < nrSubterms; i++)
	{
	  int neededPositionIndex = subterms[i].positionIndex();
	  //
	  //	If we already tested the position then we need a slot
	  //	for it to avoid reloading the pointer. If we didn't test
	  //	it we must have tested its parent and we need a slot to
	  //	hold that the parent pointer so we can access the
	  //	the dagnode at the position when handling the remainder.
	  //
	  if (!(n.positionsTested.contains(neededPositionIndex)))
	    neededPositionIndex = positions.parentIndex(neededPositionIndex);
	  if (positions.index2Position(neededPositionIndex).length() > 1)
	    {
	      if (findSlot(n.slotMap, neededPositionIndex) == NONE)
		slotMapInsert(n.slotMap, neededPositionIndex, slots.makeElement());
	    }
	}
    }
  else
    {
      //
      //	Test node; first make recursive calls on all child nodes.
      //
      int nrSons = n.sons.length();
      for (int i = 0; i < nrSons; i++)
	{
	  int sonNr = n.sons[i].target;
	  allocateVariables(sonNr);
	  slotMapUnion(n.slotMap, sonNr);
	}
      int sonNr = n.neqTarget;
      if (sonNr != NONE)
	{
	  allocateVariables(sonNr);
	  slotMapUnion(n.slotMap, sonNr);
	}
      else
	++nrFailParents;
      int testParentIndex = positions.parentIndex(n.testPositionIndex);
      if (positions.index2Position(testParentIndex).length() > 1)
	{
	  //
	  //	In order to access the test position, we need a pointer
	  //	to the test positions parent; and since this parent isn't
	  //	the root or a top level argument, we need a slot to store
	  //	this pointer in.
	  //
	  Assert(n.positionsTested.contains(testParentIndex),
		 "missing parent index");
	  if (findSlot(n.slotMap, testParentIndex) == NONE)
	    slotMapInsert(n.slotMap, testParentIndex, slots.makeElement());
	}
    }
}

void
FreePreNet::slotMapUnion(Vector<Pair>& to, int fromNodeNr)
{
  const Node& n = net[fromNodeNr];
  //
  //	Test nodes don't pass upwards a slot mapping for their test position
  //	since they are responsible for filling that slot.
  //
  int avoidIndex = n.reducedFringe.empty() ? NONE : n.testPositionIndex;
  const Vector<Pair>& from = n.slotMap;
  int nrSlots = from.length();
  for (int i = 0; i < nrSlots; i++)
    {
      int positionIndex = from[i].positionIndex;
      if (positionIndex != avoidIndex)
	slotMapInsert(to, positionIndex, from[i].slot);
    }
}

void
FreePreNet::slotMapInsert(Vector<Pair>& to, int positionIndex, int slot)
{
  int slot2 = findSlot(to, positionIndex);
  if (slot2 == NONE)
    {
      int nrSlots = to.length();
      to.expandBy(1);
      to[nrSlots].positionIndex = positionIndex;
      to[nrSlots].slot = slot;
    }
  else
    {
      if (slot != slot2)
	slots.formUnion(slot, slot2);  // bash conflicting slots for position together
    }
}

int
FreePreNet::findSlot(const Vector<Pair>& slotMap, int positionIndex)
{
  int nrSlots = slotMap.length();
  for (int i = 0; i < nrSlots; i++)
    {
      if (slotMap[i].positionIndex == positionIndex)
	return slotMap[i].slot;
    }
  return NONE;
}

void
FreePreNet::generateCode(CompilationContext& context)
{
  if (net.length() == 0)
    {
      topSymbol->generateCons(context, 1);
      return;
    }
  allocateVariables(0);
  int nrSlots = slots.elementCount();
  slotTranslation.expandTo(nrSlots);
  for (int i = 0; i < nrSlots; i++)
    slotTranslation[i] = NONE;
  int nrDistinctSlots = 0;
  for (int i = 0; i < nrSlots; i++)
    {
      if (slotTranslation[i] == NONE)
	{
	  int rep = slots.findRep(i);
	  int t = slotTranslation[rep];
	  if (t == NONE)
	    {
	      t = nrDistinctSlots;
	      ++nrDistinctSlots;
	      slotTranslation[rep] = t;
	    }
	  slotTranslation[i] = t;
	}
    }
  for (int i = 0; i < nrDistinctSlots; i++)
    context.body() << "  Node* r" << i << ";\n";
  generateNode(context, 1, 0, false);
}

void
FreePreNet::generatePointer(CompilationContext& context,
			    const Vector<Pair>& slotMap,
			    int positionIndex)
{
  const Vector<int>& position = positions.index2Position(positionIndex);
  if (position.length() == 1)
    context.body() << 'a' << position[0];
  else
    {
      int slot = findSlot(slotMap, positionIndex);
      if (slot == NONE)
	{
	  generatePointer(context, slotMap, positions.parentIndex(positionIndex));
	  context.body() << "->args[" << position[position.length() - 1] << ']';
	}
      else
	context.body() << 'r' << slotTranslation[findSlot(slotMap, positionIndex)];
    }
}

void
FreePreNet::generatePointer2(CompilationContext& context,
			    const Vector<Pair>& slotMap,
			    int positionIndex)
{
  //
  //	This function exists because we almost always want to
  //	generate an expression that looks like r1->args[2] rather
  //	than r2; perhaps to assign to r2.
  //
  const Vector<int>& position = positions.index2Position(positionIndex);
  if (position.length() == 1)
    context.body() << 'a' << position[0];
  else
    {
      generatePointer(context, slotMap, positions.parentIndex(positionIndex));
      context.body() << "->args[" << position[position.length() - 1] << ']';
    }
}

void
FreePreNet::generateNode(CompilationContext& context,
			 int indentLevel,
			 int nodeNr,
			 bool nextRemainder)
{
  if (nodeNr == NONE)
    {
      ++nrFailVisits;
      if (nrFailVisits < nrFailParents)
	{
	  //
	  //	Not the last failure for this function so generate a goto.
	  //
	  if (!nextRemainder)
	    context.body() << Indent(indentLevel) << "goto fail;\n";
	}
      else
	{
	  //
	  //	Last failure for this function so generate cons code.
	  //
	  if (nextRemainder || nrFailParents > 1)
	    context.body() << Indent(indentLevel - 1) << "fail:\n";
#ifdef ANNOTATE
	  context.body() << Indent(indentLevel) << "// failure\n";
#endif
	  context.body() << Indent(indentLevel) << "{\n";
	  topSymbol->generateCons(context, indentLevel + 1);
	  context.body() << Indent(indentLevel) << "}\n";
	}
    }
  else
    {
      Node& n = net[nodeNr];
      ++n.nrVisits;
      if (n.nrVisits < n.nrParents)
	{
	  if (!nextRemainder)
	    context.body() << Indent(indentLevel) << "goto j" << nodeNr << ";\n";
	}
      else
	{
	  // if (nextRemainder || n.nrParents > 1) HACK
	    context.body() << Indent(indentLevel - 1) << 'j' << nodeNr << ":\n";
	  if (n.reducedFringe.empty())
	    {
	      //
	      //	Remainder node.
	      //
	      const Pattern& p = patterns[n.patternIndex];
	      const Equation* e = topSymbol->getEquations()[n.patternIndex];
#ifdef ANNOTATE
	      context.body() << Indent(indentLevel) << "// " << e << '\n';
#endif
	      context.body() << Indent(indentLevel) << "{\n";
	      Term* lhs = p.term;
	      Vector<VariableName> varNames(e->getNrRealVariables());
	      int nrSubterms = p.subterms.length();
	      //
	      //	Generate code to bind free variables.
	      //
	      for (int i = 0; i < nrSubterms; i++)
		{
		  const FreeSubterm& s = p.subterms[i];  
		  if (s.type() == FreeSubterm::FREE_VARIABLE)
		    {
		      int positionIndex = s.positionIndex();
		      const Vector<int>& position = positions.index2Position(positionIndex);
		      VariableTerm* vt = static_cast<VariableTerm*>
			(static_cast<FreeTerm*>(lhs)->locateSubterm(position));
		      int varIndex = vt->getIndex();
		      int len = position.length();
		      if (len == 1)
			varNames[varIndex] = VariableName('a', position[len - 1]);
		      else
			{
			  int slot = findSlot(n.slotMap, positionIndex);
			  if (slot == NONE)
			    {
			      context.body() << Indent(indentLevel + 1) << "Node* v" <<
				varIndex << " = ";
			      generatePointer2(context, n.slotMap, positionIndex);
			      context.body() << ";\n";
			      varNames[varIndex] = VariableName('v', varIndex);
			    }
			  else
			    varNames[varIndex] = VariableName('r', slotTranslation[slot]);
			}
		      //
		      //	Generate code to test sort of node bound to variable
		      //	if needed.
		      //
		      Sort* varSort = vt->getSort();
		      if (!(varSort->errorFreeMaximal()))
			{
			  context.usedSort(varSort);
			  context.body() << Indent(indentLevel + 1) << "if (!(s" <<
			    varSort->getIndexWithinModule() << '[' <<
			    varNames[varIndex] << "->flags.sortIndex]))\n";
			  context.body() << Indent(indentLevel + 2) << "goto ";
			  if (n.nextPattern == NONE)
			    context.body() << "fail;\n";
			  else
			    context.body() << 'j' << n.nextPattern << ";\n";
			}
		    }
		} 
	      //
	      //	Generate code to test bound variables.
	      //
	      for (int i = 0; i < nrSubterms; i++)
		{
		  const FreeSubterm& s = p.subterms[i];
		  if (s.type() == FreeSubterm::BOUND_VARIABLE)
		    {
		      int positionIndex = s.positionIndex();
		      const Vector<int>& position = positions.index2Position(positionIndex);
		      VariableTerm* vt = static_cast<VariableTerm*>
			(static_cast<FreeTerm*>(lhs)->locateSubterm(position));
		      int varIndex = vt->getIndex();
		      context.body() << Indent(indentLevel + 1) << "if (compare(" <<
			varNames[varIndex] << ", ";
		      generatePointer(context, n.slotMap, positionIndex);
		      context.body() << ") != 0)\n";
		      context.body() << Indent(indentLevel + 2) << "goto ";
		      if (n.nextPattern == NONE)
			context.body() << "fail;\n";
		      else
			context.body() << 'j' << n.nextPattern << ";\n";
		    }
		}
	      //
	      //	Generate code for rhs.
	      //
	      e->getRhs()->generateRhs(context, indentLevel, varNames, topSymbol);
	      context.body() << Indent(indentLevel) << "}\n";
	      //
	      //	Generate code for next remainder.
	      //
	      if (!(patterns[n.patternIndex].flags & UNFAILING))
		generateNode(context, indentLevel, n.nextPattern, true);
	    }
	  else
	    {
	      //
	      //	Test node.
	      //
	      int slot = findSlot(n.slotMap, n.testPositionIndex);
	      if (slot == NONE)
		{
		  context.body() << Indent(indentLevel) << "switch (";
		  generatePointer2(context, n.slotMap, n.testPositionIndex);
		}
	      else
		{
		  int trans = slotTranslation[slot];
		  context.body() << Indent(indentLevel) << 'r' << trans << " = ";
		  generatePointer2(context, n.slotMap, n.testPositionIndex);
		  context.body() << ";\n";
		  context.body() << Indent(indentLevel) << "switch (r" << trans;
		}
	      context.body() << "->symbol)\n";
	      context.body() << Indent(indentLevel + 1) << "{\n";
	      int nrArcs = n.sons.length();
	      for (int i = 0; i < nrArcs; i++)
		{
		  Arc& a = n.sons[i];
		  context.body() << Indent(indentLevel + 1) << "case " <<
		    a.label->getIndexWithinModule() << ':';
#ifdef ANNOTATE
		  context.body() << "  // " << a.label;
#endif
		  context.body() <<'\n';
		  generateNode(context, indentLevel + 2, a.target, false);
		}
	      context.body() << Indent(indentLevel + 1) << "default:\n";
	      generateNode(context, indentLevel + 2, n.neqTarget, false);
	      context.body() << Indent(indentLevel + 1) << "}\n";
	    }
	}
    }
}
