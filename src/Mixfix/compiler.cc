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
//      Implementation for class Compiler.
//

#include <unistd.h>
#include <sstream>

//      utility stuff
#include "macros.hh"
#include "vector.hh"

//      forward declarations
#include "interface.hh"
#include "core.hh"
#include "fullCompiler.hh"
#include "strategyLanguage.hh"
#include "mixfix.hh"

//      interface class definitions
#include "term.hh"

//      core class definitions
#include "dagArgumentIterator.hh"

//	full compiler definitions
#include "compilationContext.hh"

//	front end class definitions
#include "visibleModule.hh"
#include "syntacticPreModule.hh"
#include "compiler.hh"

Compiler::Compiler()
{
  currentExecutable = 0;
}

Compiler::~Compiler()
{
  if (currentExecutable != 0)
    /* unlink(makeBaseName().c_str()) */;  // not safe - baseName already destructed
}

const string&
Compiler::makeBaseName()
{

  static string baseName;
  if (baseName.empty())
    {
      static ostringstream buffer;
      const char* tmpDir = getenv("TMPDIR");
      if (tmpDir == 0)
	tmpDir = (access("/usr/tmp", W_OK) == 0) ? "/usr/tmp" : "/tmp";
      buffer << tmpDir << "/maude" << getpid();
      baseName = buffer.str();  // deep copy
    }
  return baseName;
}

bool
Compiler::fullCompile(SyntacticPreModule* module, bool countRewrites)
{
  IssueAdvisory("compiling module " << QUOTE(module) << '.');
  string hhName(makeBaseName() + ".hh");
  string ccName(makeBaseName() + ".cc");
  ofstream hhFile(hhName.c_str());
  ofstream ccFile(ccName.c_str());
  CompilationContext context(hhFile, ccFile, countRewrites);
  ccFile << "#include \"runtime.hh\"\n";
  ccFile << "#include \"" << hhName << "\"\n\n";
  ccFile << "const char outFileName[] = \"" << makeBaseName() << ".out\";\n";
  ccFile << "const char inFileName[] = \"" << makeBaseName() << ".in\";\n\n";

  VisibleModule* fm = module->getFlatModule();
  const Vector<Symbol*>& symbols = fm->getSymbols();
  int nrSymbols = symbols.length();
  //
  //	Generate arity table.
  //	(eventually this will be more than arities and probably moved
  //	into CompilationContext).
  //
  ccFile << "int arity[] =\n{";
  for (int i = 0; i < nrSymbols; i++)
    {
      if (i % 16 == 0)
	ccFile << "\n ";
      ccFile << ' ' << symbols[i]->arity();
      if (i + 1 < nrSymbols)
	ccFile << ',';
    }
  ccFile << "\n};\n\n";
  //
  //	Generate symbol comparison table.
  //
  ccFile << "int comparison[] =\n{";
  for (int i = 0; i < nrSymbols; i++)
    {
      if (i % 8 == 0)
	ccFile << "\n ";
      ccFile << ' ' << symbols[i]->getHashValue();  // HACK
      if (i + 1 < nrSymbols)
	ccFile << ',';
    }
  ccFile << "\n};\n\n";
  //
  //	Generate inline functions for all pure constructors.
  //
  for (int i = 0; i < nrSymbols; i++)
    {
      Symbol* s = symbols[i];
      if (s->equationFree())
	s->fullCompile(context, true);
    }
  //
  //	Generate normal functions for remaining operators.
  //
  for (int i = 0; i < nrSymbols; i++)
    {
      Symbol* s = symbols[i];
      if (!(s->equationFree()))
	s->fullCompile(context, false);
    }
  context.generateSortVectors(fm);
  context.generateEval(fm);
  hhFile.flush();
  ccFile.flush();

  static const char defaultOptFlags[] =
#ifdef SOLARIS
    "-O2 -mcpu=v8 -mtune=ultrasparc -mflat";
#elif LINUX
  "-O2 -mcpu=pentiumpro -mpreferred-stack-boundary=2 -fomit-frame-pointer";
#elif ALPHA
  "-O2 -fomit-frame-pointer";
#elif FREE_BSD
  "-O2 -mcpu=pentiumpro -mpreferred-stack-boundary=2 -fomit-frame-pointer";
#else
  "-O2";
#endif

  string cmd("g++-3.2 ");
  const char* optFlags = getenv("MAUDE_GCC_FLAGS");
  if (optFlags == 0)
    optFlags = defaultOptFlags;
  cmd += optFlags;

  if (const char* incDir = getenv("MAUDE_INCLUDE"))
    {
      cmd += " -I";
      cmd += incDir;
    }
  else
    return false;

  if (const char* libDir = getenv("MAUDE_RUNTIME"))
    {
      cmd += " -L";
      cmd += libDir;
    }
  else
    return false;
  
  cmd += ' ';
  cmd += ccName;
  cmd += " -lruntime -o ";
  cmd += makeBaseName();
  int status = system(cmd.c_str());
  cout << status << '\n' << cmd.c_str() << '\n';
  cout.flush();
#ifndef ANNOTATE
  unlink(hhName.c_str());
  unlink(ccName.c_str());
#endif
  return status == 0;
}

bool
Compiler::makeExecutable(SyntacticPreModule* module, bool countRewrites)
{
  int nrSymbols = module->getFlatModule()->getSymbols().length();
  if (currentExecutable == module &&
      compiledWithCount == countRewrites &&
      currentNrSymbols == nrSymbols)
    return true;  // use existing executable

  if(!(fullCompile(module, countRewrites)))
    {
      IssueWarning("compilation failed.");
      currentExecutable = 0;  // assume corrupted
      return false;
    }

  currentExecutable = module;
  compiledWithCount = countRewrites;
  currentNrSymbols = nrSymbols;
  return true;
}

void
Compiler::outputGraph(DagNode* dagNode)
{
  string outName(makeBaseName() + ".out");
  ofstream ofile(outName.c_str());
  PointerSet visited;
  depthFirstTraversal(dagNode, visited);
  int nrNodes = visited.cardinality();
  for (int i = 0; i < nrNodes; i++)
    {
      DagNode* d = static_cast<DagNode*>(visited.index2Pointer(i));
      ofile << d->symbol()->getIndexWithinModule();
      for(DagArgumentIterator a(*d); a.valid(); a.next())
	ofile << ' ' << visited.pointer2Index(a.argument());
      ofile << '\n';
    }
}

void
Compiler::depthFirstTraversal(DagNode* dagNode, PointerSet& visited)
{ 
  for(DagArgumentIterator a(*dagNode); a.valid(); a.next())
    {
      DagNode* d = a.argument();
      if (!(visited.contains(d)))
        depthFirstTraversal(d, visited);
    }
  visited.insert(dagNode);
}

DagNode*
Compiler::inputGraph(Int64& nrRewrites, Int64& cpu, Int64& real)
{
  const Vector<Symbol*>& symbols = currentExecutable->getFlatSignature()->getSymbols();
  string inName(makeBaseName() + ".in");
  ifstream ifile(inName.c_str());
  Vector<DagNode*> built;
  Vector<DagNode*> args;
  ifile >> nrRewrites;
  ifile >> cpu;
  ifile >> real;
  int sortIndex;
  ifile >> sortIndex;
  for(;;)
    {
      int index;
      if(!(ifile >> index))
	break;
      Symbol* s = symbols[index];
      int nrArgs = s->arity();
      args.contractTo(0);
      for (int i = 0; i < nrArgs; i++)
	{
	  int t;
	  ifile >> t;
	  args.append(built[t]);
	}
      built.append(s->makeDagNode(args));
    }
  DagNode* top = built[built.length() - 1];
  top->setSortIndex(sortIndex);
  unlink((makeBaseName() + ".out").c_str());
  unlink(inName.c_str());
  return top;
}

void
Compiler::runExecutable()
{
  returnValueDump = system(makeBaseName().c_str());
}

void
Compiler::invalidate(SyntacticPreModule* module)
{
  if (currentExecutable == module)
    {
      currentExecutable = 0;
      unlink(makeBaseName().c_str());
    }
}
