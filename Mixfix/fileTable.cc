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
//      Implementation for class FileTable.
//

//      utility stuff
#include "macros.hh"
#include "vector.hh"

#include "fileTable.hh"

#ifdef EXPLICIT_TEMPLATES
template class Vector<FileTable::Entry>;
#endif

FileTable::FileTable()
{
  firstSilent = NONE;
}

void
FileTable::openFile(int lineNumber, const char* name, bool silent)
{
  if (silent && firstSilent == NONE)
    firstSilent = fileStack.length();
  if (firstSilent == NONE && fileStack.length() > 0)
    cout << "Reading in file: \"" << name << "\"\n";
  int nrFileChanges = fileChanges.length();
  fileChanges.expandBy(1);
  Entry& fc = fileChanges[nrFileChanges];
  fc.absoluteLineNumber = lineNumber;
  fc.fileNameIndex = fileNames.encode(name);
  fc.relativeLineNumber = 1;
  fileStack.append(nrFileChanges);
}

void
FileTable::closeFile(int lineNumber)
{
  int topOfStack = fileStack.length() - 1;
  int matchingOpen = fileStack[topOfStack];
  if (firstSilent == NONE && topOfStack > 0)
    {
      cout << "Done reading in file: \"" <<
	fileNames.name(fileChanges[matchingOpen].fileNameIndex) << "\"\n";
    }
  if (firstSilent == topOfStack)
    firstSilent = NONE;
  int prevFileNameIndex = NONE;
  int prevLineNumberDiff = 0;
  if (matchingOpen > 0)
    {
      Entry& fe = fileChanges[matchingOpen - 1];
      prevFileNameIndex = fe.fileNameIndex;
      prevLineNumberDiff = fe.absoluteLineNumber - fe.relativeLineNumber;
    }
  int nrFileChanges = fileChanges.length();
  fileChanges.expandBy(1);
  Entry& fc = fileChanges[nrFileChanges];
  fc.absoluteLineNumber = lineNumber;
  fc.fileNameIndex = prevFileNameIndex;
  fc.relativeLineNumber = fileChanges[matchingOpen].absoluteLineNumber - prevLineNumberDiff;
  fileStack.contractTo(topOfStack);
}

void
FileTable::beginModule(Token type, Token name)
{
  int nrModuleChanges = moduleChanges.length();
  moduleChanges.expandBy(1);
  Entry& mc = moduleChanges[nrModuleChanges];
  mc.absoluteLineNumber = type.lineNumber();
  mc.moduleNameIndex = name.code();
  mc.moduleTypeIndex = type.code();
  moduleStack.append(nrModuleChanges);
  if (outputLine())
    cout << type << ' ' << name << '\n';
}

bool
FileTable::outputLine()
{
  if (firstSilent == NONE && fileStack.length() > 0)
    {
      cout << "==========================================\n";
      return true;
    }
  return false;
}

void
FileTable::endModule(int lineNumber)
{
  int topOfStack = moduleStack.length() - 1;
  int matchingOpen = moduleStack[topOfStack];
  int nrModuleChanges = moduleChanges.length();
  moduleChanges.expandBy(1);
  Entry& mc = moduleChanges[nrModuleChanges];
  mc.absoluteLineNumber = lineNumber;
  if (matchingOpen > 0)
    {
      mc.moduleNameIndex = moduleChanges[matchingOpen - 1].moduleNameIndex;
      mc.moduleTypeIndex = moduleChanges[matchingOpen - 1].moduleTypeIndex;
    }
  else
    {
      mc.moduleNameIndex = NONE;
      mc.moduleTypeIndex = NONE;
    }
  moduleStack.contractTo(topOfStack);
}

void
FileTable::abortEverything(int lineNumber)
{
  //
  //	Revert to top level file.
  //
  if (fileStack.length() > 0)
    {
      int matchingOpen = fileStack[0];
      int prevLineNumberDiff = 0;
      if (matchingOpen > 0)
	{
	  Entry& fe = fileChanges[matchingOpen - 1];
	  prevLineNumberDiff = fe.absoluteLineNumber - fe.relativeLineNumber; 
	}
      int nrFileChanges = fileChanges.length();
      fileChanges.expandBy(1);
      Entry& fc = fileChanges[nrFileChanges];
      fc.absoluteLineNumber = lineNumber;
      fc.fileNameIndex = NONE;
      fc.relativeLineNumber = fileChanges[matchingOpen].absoluteLineNumber - prevLineNumberDiff;
      fileStack.contractTo(0);
      firstSilent = NONE;
    }
  //
  //	End all modules.
  //
  if (moduleStack.length() > 0)
    {
      int nrModuleChanges = moduleChanges.length();
      moduleChanges.expandBy(1);
      Entry& mc = moduleChanges[nrModuleChanges];
      mc.absoluteLineNumber = lineNumber;
      mc.moduleNameIndex = NONE;
      mc.moduleTypeIndex = NONE;
      moduleStack.contractTo(0);
    }
}

int
FileTable::binarySearch(Vector<Entry>& table, int lineNumber)
{
  //
  //	Find index of entry with largest absoluteLineNumber that is <= lineNumber.
  //	Return -1 if no such entry exists.
  //
  int max = table.length() - 1;
  int min = 0;
  while (max >= min)
    {
      int probe = (max + min) / 2;
      int n = table[probe].absoluteLineNumber;
      if (n == lineNumber)
	return probe;
      else if (n > lineNumber)
	max = probe - 1;
      else
	min = probe + 1;
    }
  return min - 1;
}

void
FileTable::printLineNumber(ostream& s, int lineNumber)
{
  switch (lineNumber)
    {
    case SYSTEM_CREATED:
      {
	s << "<automatic>";
	break;
      }
    case META_LEVEL_CREATED:
      {
	s << "<metalevel>";
	break;
      }
    case COMMAND_LINE:
      {
	s << "<command line>";
	break;
      }
    default:
      {
	int fileEntry = binarySearch(fileChanges, lineNumber);
	if (fileEntry == -1)
	  s << "<standard input>, line " << lineNumber;
	else
	  {
	    Entry& fe = fileChanges[fileEntry];
	    if (fe.fileNameIndex == NONE)
	      s << "<standard input>";
	    else
	      s << '"' << fileNames.name(fe.fileNameIndex) << '"';
	    s << ", line " << fe.relativeLineNumber + (lineNumber - fe.absoluteLineNumber);
	  }
	int moduleEntry = binarySearch(moduleChanges, lineNumber);
	if (moduleEntry == -1 || moduleChanges[moduleEntry].moduleTypeIndex == NONE)
	  break;
	Entry& me = moduleChanges[moduleEntry];
	s << " (" << Token::name(me.moduleTypeIndex) <<
	  ' ' << Token::name(me.moduleNameIndex) << ')';
      } 
    }
}
