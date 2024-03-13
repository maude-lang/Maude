/*

    This file is part of the Maude 3 interpreter.

    Copyright 2023-2024 SRI International, Menlo Park, CA 94025, USA.

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
//      Class to hold settings controlling printing of various entities.
//
#ifndef _printSettings_hh_
#define _printSettings_hh_
#include <set>

class PrintSettings
{
  NO_COPYING(PrintSettings);

public:
  enum PrintFlags : uint_fast32_t
    {
     PRINT_CLEARED_FLAGS = 0,
     //
     //	General prettyprinter flags.
     //
     PRINT_GRAPH = 0x1,		// print as a set of DAG nodes
     PRINT_CONCEAL = 0x2,	// respect concealed argument lists
     PRINT_FORMAT = 0x4,		// respect format attribute
     PRINT_MIXFIX = 0x8,		// mixfix notation
     PRINT_WITH_PARENS = 0x10,	// maximal parens
     PRINT_COLOR = 0x20,		// dag node coloring based on ctor/reduced status
     PRINT_DISAMBIG_CONST = 0x40,	// (c).s for every constant c
     PRINT_LABEL_ATTRIBUTE = 0x80,	// use [label foo] after statement rather than [foo] : before statement
     //
     //	Prettyprinter flags for particular symbol types.
     //
     PRINT_WITH_ALIASES = 0x100,	// for variables
     PRINT_FLAT = 0x200,		// for assoc symbols
     PRINT_NUMBER = 0x400,	// for nats & ints
     PRINT_RAT = 0x800,		// for rats
     PRINT_HOOKS = 0x1000,	// for built-ins
     PRINT_COMBINE_VARS = 0x2000,	// for variables
     //
     //	Default settings for Maude interpreter.
     //
     DEFAULT_PRINT_FLAGS = PRINT_FORMAT | PRINT_MIXFIX | PRINT_WITH_ALIASES | PRINT_FLAT | PRINT_NUMBER | PRINT_RAT | PRINT_HOOKS,
     //
     //	Turn off formatting where it could cause issues.
     //
     PLAIN_PRINT_FLAGS = PRINT_MIXFIX | PRINT_WITH_ALIASES | PRINT_FLAT | PRINT_NUMBER | PRINT_RAT | PRINT_HOOKS
  };

  PrintSettings(PrintFlags initialValue = PRINT_CLEARED_FLAGS);

  void setPrintFlag(PrintFlags flag, bool polarity = true);
  bool getPrintFlag(PrintFlags flag) const;
  void insertConcealed(int symbolId);
  void eraseConcealed(int symbolId);
  bool concealedSymbol(int symbolId) const;

private:
  PrintFlags printFlags;
  set<int> concealedSymbols;
};

inline
PrintSettings::PrintSettings(PrintFlags initialValue)
{
  printFlags = initialValue;
}

inline bool
PrintSettings::getPrintFlag(PrintFlags flag) const
{
  return printFlags & flag;
}

inline void
PrintSettings::setPrintFlag(PrintFlags flag, bool polarity)
{
  if (polarity)
    printFlags = static_cast<PrintFlags>(printFlags | flag);
  else
    printFlags = static_cast<PrintFlags>(printFlags & ~flag);
}

inline void
PrintSettings::insertConcealed(int symbolId)
{
  concealedSymbols.insert(symbolId);
}

inline void
PrintSettings::eraseConcealed(int symbolId)
{
  concealedSymbols.erase(symbolId);
}

inline bool
PrintSettings::concealedSymbol(int symbolId) const
{
  return getPrintFlag(PRINT_CONCEAL) && concealedSymbols.find(symbolId) != concealedSymbols.end();
}

#endif
