/*

    This file is part of the Maude 3 interpreter.

    Copyright 2020 SRI International, Menlo Park, CA 94025, USA.

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
//	Routines for serializing a dag to a Rope.
//

Rope
MixfixModule::serialize(DagNode* dagNode)
{
  Rope accumulator;
  PointerSet visited;
  
  visit(dagNode, visited, accumulator);
  return accumulator;
}

int
MixfixModule::visit(DagNode* dagNode, PointerSet& visited, Rope& accumulator)
{
  int index = visited.pointer2Index(dagNode);
  if (index != NONE)
    return index;  // already serialized
  //
  //	Post-order traversal - serialize arguments first.
  //
  Vector<int> argIndices;
  for(DagArgumentIterator a(*dagNode); a.valid(); a.next())
    argIndices.append(visit(a.argument(), visited, accumulator));

  Symbol* symbol = dagNode->symbol();
  int indexWithinModule = symbol->getIndexWithinModule();
  SymbolType type = symbolInfo[indexWithinModule].symbolType;

  if (type.getBasicType() == SymbolType::SORT_TEST)
    {
      //
      //	Can't assume this sort test symbol will have been created
      //	before fork(), so it may not exist in other process.
      //	We can how assume that sort exists in other process so we
      //	use a special representation.
      //
      SortTestSymbol* st = safeCast(SortTestSymbol*, symbol);
      accumulator += int64ToString(st->sort()->getIndexWithinModule());
      accumulator += st->eager() ? 's' : 'l';
      accumulator += '(';
      accumulator += int64ToString(argIndices[0]);
      accumulator += ")\n";
      return visited.insert(dagNode);
    }
  if (type.getBasicType() == SymbolType::VARIABLE)
    {
      //
      //	Can't assume base variable symbol will have been created
      //	before fork(), so it may not exist in other process.
      //	Nor can we assume the name token exists.
      //	We can how assume that sort exists in other process so we
      //	use a special representation.
      //
      VariableSymbol* vs = safeCast(VariableSymbol*, symbol);
      Sort* sort = vs->getSort();
      accumulator += int64ToString(sort->getIndexWithinModule());
      accumulator += 'v';
      VariableDagNode* vd = safeCast(VariableDagNode*, dagNode);
      accumulator += Token::name(vd->id());
      accumulator += '\n';
      return visited.insert(dagNode);
    }

  short polymorphIndex = symbolInfo[indexWithinModule].polymorphIndex;
  if (polymorphIndex != NONE)
    {
      //
      //	Polymorph instance.
      //
      //	Can't assume that instance was created before fork()
      //	so we need to locate which polymorph was instantiated
      //	and what kind it was instantiated on.
      //
      Polymorph& p = polymorphs[polymorphIndex];
      int kindIndex = 0;
      while (p.instantiations[kindIndex] != symbol)
	++kindIndex;
      accumulator += int64ToString(polymorphIndex);
      accumulator += 'p';
      accumulator += int64ToString(kindIndex);
    }
  else
    {
      //
      //	Regular symbol.
      //
      accumulator += int64ToString(indexWithinModule);
    }
  
  if (type.hasFlag(SymbolType::ITER))
    {
      const mpz_class number = safeCast(S_DagNode*, dagNode)->getNumber();
      string ts = number.get_str();
      accumulator += '^';
      accumulator += ts.c_str();
    }

  switch (type.getBasicType())
    {
    case SymbolType::FLOAT:
      {
	//
	//	doubles are a pain to serialize since we need to record and
	//	recover the exact bit pattern to avoid rounding and handle
	//	exception values and denormalized numbers.
	//
	//	We rely on sizeof(double) == sizeof(Int64) and that 
	//	all bit patterns of an Int64 have a unique string representation.
	//	We can't assume that double and Int64 have the same alignment, so
	//	we copy the bytes from one to the other.
	//
	Assert(sizeof(double) == sizeof(Int64), "can't handle double on this architecture");
	double d = safeCast(FloatDagNode*, dagNode)->getValue();
	Int64 dBits;
	memcpy(&dBits, &d, sizeof(d));
	accumulator += 'f';
	accumulator += int64ToString(dBits);
	break;
      }
    case SymbolType::STRING:
      {	
	string strValue;
	Token::ropeToString(safeCast(StringDagNode*, dagNode)->getValue(), strValue);
	accumulator += strValue.c_str();
	break;
      }
    case SymbolType::QUOTED_IDENTIFIER:
      {
	accumulator += '\'';
	accumulator += Token::name(safeCast(QuotedIdentifierDagNode*, dagNode)->getIdIndex());
	break;
      }
    case SymbolType::SMT_NUMBER_SYMBOL:
      {
	accumulator += 'q';
	const mpq_class& rat = safeCast(SMT_NumberDagNode*, dagNode)->getValue();
	string ts = rat.get_str();
	accumulator += ts.c_str();
	break;
      }
    }
  
  int nrArgs = argIndices.size();
  if (nrArgs > 0)
    {
      accumulator += '(';
      accumulator += int64ToString(argIndices[0]);
      for (int i = 1; i < nrArgs; ++i)
	{
	  accumulator += ',';
	  accumulator += int64ToString(argIndices[i]);
	}
      accumulator += ')';
    }
  accumulator += '\n';
  return visited.insert(dagNode);
}
