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
//	Routines for pretty printing a dag as a term graph.
//

void
MixfixModule::graphPrint(ostream& s, DagNode* dagNode, const PrintSettings& printSettings)
{
  PointerSet visited;
  Vector<mpz_class> counts;
  graphCount(dagNode, visited, counts);
  s << "Begin{Graph Representation}\n[Term has " << counts[0] <<
    " operator symbol" << (counts[0] == 1 ? "" : "s") <<
    " while graph has " << visited.cardinality() <<
    " node" << pluralize(visited.cardinality()) << ".]\n";
  int nrNodes = visited.cardinality();
  for (int i = 0; i < nrNodes; i++)
    {
      if (UserLevelRewritingContext::interrupted())
	return;
      s << '#' << i << " = ";
      DagNode* dagNode = static_cast<DagNode*>(visited.index2Pointer(i));
      Symbol* symbol = dagNode->symbol();
      SymbolType type = symbolInfo[symbol->getIndexWithinModule()].symbolType;
      switch (type.getBasicType())
	{
	case SymbolType::VARIABLE:
	  {
	    VariableDagNode* v = safeCast(VariableDagNode*, dagNode);
	    printVariable(s, v->id(), safeCast(VariableSymbol*, symbol)->getSort(), printSettings);
	    break;
	  }
	case SymbolType::FLOAT:
	  {
	    s << doubleToString(safeCast(FloatDagNode*, dagNode)->getValue());
	    break;
	  }
	case SymbolType::STRING:
	  {
	    string strValue;
	    Token::ropeToString(safeCast(StringDagNode*, dagNode)->getValue(), strValue);
	    s << strValue;
	    break;
	  }
	case SymbolType::QUOTED_IDENTIFIER:
	  {
	    s << '\'' << Token::name(safeCast(QuotedIdentifierDagNode*, dagNode)->getIdIndex());
	    break;
	  }
	case SymbolType::SMT_NUMBER_SYMBOL:
	  {
	    SMT_NumberDagNode* n = safeCast(SMT_NumberDagNode*, dagNode);
	    const mpq_class& value = n->getValue();
	    //
	    //	Look up the index of our sort.
	    //
	    Symbol* symbol = dagNode->symbol();
	    Sort* sort = symbol->getRangeSort();
	    //
	    //	Figure out what SMT sort we correspond to.
	    //
	    SMT_Info::SMT_Type t = getSMT_Info().getType(sort);
	    Assert(t != SMT_Info::NOT_SMT, "bad SMT sort " << sort);
	    s << value.get_num();
	    if (t == SMT_Info::REAL)
	      s << '/' << value.get_den();
	    else
	      Assert(t == SMT_Info::INTEGER, "SMT number sort expected");
	    break;
	  }
	default:
	  {
	    s << Token::name(symbol->id());
	    if (type.hasFlag(SymbolType::ITER))
	      {
		const mpz_class& number = safeCast(S_DagNode*, dagNode)->getNumber();
		if (number > 1)
		  s << '^' << number;
	      }
	    DagArgumentIterator a(*dagNode);
	    if (a.valid())
	      {
		s << '(';
		for (;;)
		  {
		    s << '#' << visited.pointer2Index(a.argument());
		    a.next();
		    if (!(a.valid()))
		      break;
		    s << ", ";
		  }
		s << ')';
	      }
	    break;
	  }
	}
      s << '\n';
    }
  s << "End{Graph Representation}";
}

void
MixfixModule::graphCount(DagNode* dagNode, PointerSet& visited, Vector<mpz_class>& counts)
{
  visited.insert(dagNode);
  int index = counts.length();
  Assert(index == visited.pointer2Index(dagNode), "counts out of step");
  counts.append(0);
 
  mpz_class count(1);
  for(DagArgumentIterator a(*dagNode); a.valid(); a.next())
    {
      DagNode* d = a.argument();
      if (!(visited.contains(d)))
        graphCount(d, visited, counts);
      mpz_class& childsCount = counts[visited.pointer2Index(d)];
      Assert(childsCount != 0, "cycle in dag");
      count += childsCount;
    }
  counts[index] = count;
}
