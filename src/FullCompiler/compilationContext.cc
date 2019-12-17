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
//	Implementation for class CompilationContext.
//

//	utility stuff
#include "macros.hh"
#include "vector.hh"
#include "indent.hh"

//      forward declarations
#include "interface.hh"
#include "core.hh"

//      interface class definitions
#include "symbol.hh"

//      core class definitions
#include "module.hh"
#include "sort.hh"

//	full compiler classes
#include "compilationContext.hh"

CompilationContext::CompilationContext(ostream& hhFile, ostream& ccFile, bool countRewrites)
  : hhFile(hhFile),
    ccFile(ccFile),
    countRewrites(countRewrites)
{
}

void
CompilationContext::generateIncrement(int indent)
{
  if (countRewrites)
    ccFile << Indent(indent) << "++g.count;\n";
}

void
CompilationContext::generateSortVectors(Module* module)
{
  const Vector<Sort*>& sorts = module->getSorts();
  int nrSorts = sorts.length();
  for (int i = 0; i < nrSorts; i++)
    {
      if (usedSorts.contains(i))
	sorts[i]->generateSortVector(*this);
    }
  hhFile << '\n';
}

void
CompilationContext::generateEval(Module* module)
{
  const Vector<Symbol*>& symbols = module->getSymbols();
  int nrSymbols = symbols.length();

  ccFile << "Node*\neval(Node* a)\n{\n" <<
    "  EVAL;\n" <<
    "  safe[0].l = g.safePtr;\n" <<
    "  g.safePtr = safe;\n" <<
    "  safe[2].n = a;\n" <<
    "  Node* r;\n" <<
    "  switch (a->symbol)\n" <<
    "    {\n";
  int maxArgs = 0;
  for (int i = 0; i < nrSymbols; i++)
    {
      Symbol* symbol = symbols[i];
      int nrArgs = symbol->arity();
      if (nrArgs > maxArgs)
	maxArgs = nrArgs;
      if (i == nrSymbols - 1)
	ccFile << "    default:\n";
      else
	ccFile << "    case " << i << ":\n";
      ccFile << "      {\n        safe[1].i = " <<
	(nrArgs > 0 ? nrArgs : 1) << ";\n";
      //
      //	Generate code to clear all safe slots.
      //
      for (int j = 0; j < nrArgs - 1; j++)
	ccFile << "        safe[" << j + 3 << "].n = 0;\n";
      //
      //	Generate code to evaluate each argument.
      //
      for (int j = 0; j < nrArgs; j++)
	{
	  if (j == nrArgs - 1)
	    ccFile << "        Node* t";
	  else
	    ccFile << "        safe[" << j + 3 << "].n";
	  ccFile << " = eval(" <<
	    ((j == 0) ? "a" : "safe[2].n") <<
	    "->args[" << j << "]);\n";
	}
      //
      //	Generate code to evaulate at top.
      //
      ccFile << "        r = f" << i << '(';
      for (int j = 0; j < nrArgs - 1; j++)
	ccFile << "safe[" << j + 3 << "].n, ";
      if (nrArgs > 0)
	ccFile << 't';
      ccFile << ");\n";
      //
      //	Generate end code.
      //
      ccFile << "        break;\n" <<
	"      }\n";
    }
  //
  //	Generate code to tidy up and return result.
  //
  ccFile << "    }\n" <<
    "  g.safePtr = safe[0].l;\n" <<
    "  return r;\n" << "}\n\n";
  //
  //	Generate #define for safe slots.
  //
  hhFile << "#define EVAL\tLink safe[" << (maxArgs == 0 ? 3 : maxArgs + 2) << "]\n";
}
