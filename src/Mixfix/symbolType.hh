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
//      Class for holding symbol type info.
//
#ifndef _symbolType_hh_
#define _symbolType_hh_

class SymbolType
{
public:
  //
  //	A symbol type consist of a single basic type combined with
  //	zero or more modification flags.
  //
  enum BasicTypes
  {
    STANDARD,
    //
    //	System created symbols.
    //
    VARIABLE,
    SORT_TEST,
    INTERNAL_TUPLE,
    //
    //	Special properties.
    //
    SYSTEM_TRUE,
    SYSTEM_FALSE,
    BUBBLE,
    //
    //	Special symbols that do not deal with attachments.
    //
    FLOAT,
    STRING,
    //
    //	Special symbols that do deal with attachments.
    //
    BRANCH_SYMBOL,
    EQUALITY_SYMBOL,
    FLOAT_OP,
    STRING_OP,
    QUOTED_IDENTIFIER,
    QUOTED_IDENTIFIER_OP,
    MODEL_CHECKER_SYMBOL,
    SAT_SOLVER_SYMBOL,
    META_LEVEL_OP_SYMBOL,
    LOOP_SYMBOL,
    SUCC_SYMBOL,
    MINUS_SYMBOL,
    NUMBER_OP_SYMBOL,
    ACU_NUMBER_OP_SYMBOL,
    CUI_NUMBER_OP_SYMBOL,
    DIVISION_SYMBOL,
    RANDOM_OP_SYMBOL,
    MATRIX_OP_SYMBOL,
    COUNTER_SYMBOL,
    SOCKET_MANAGER_SYMBOL,
    INTERPRETER_MANAGER_SYMBOL,

    END_OF_SYMBOLS_WITH_ATTACHMENTS
  };

  enum Flags
  {
    //
    //	Syntactic attributes.
    //
    PREC = 0x1,
    GATHER = 0x2,
    FORMAT = 0x4,
    LATEX = 0x8,
    //
    //	Semantic attributes.
    //
    STRAT = 0x10,
    MEMO = 0x20,
    FROZEN = 0x40,
    CTOR = 0x80,
    //
    //	OO attributes.
    //
    CONFIG = 0x100,
    OBJECT = 0x200,
    MESSAGE = 0x400,
    //
    //	Theory attributes.
    //
    ASSOC = 0x1000,
    COMM = 0x2000,
    LEFT_ID = 0x4000,
    RIGHT_ID = 0x8000,
    IDEM = 0x10000,
    ITER = 0x20000,
    //
    //	Misc.
    //
    POLY = 0x400000,
    DITTO = 0x800000,
    //
    //	Conjunctions.
    //
    AXIOMS = ASSOC | COMM | LEFT_ID | RIGHT_ID | IDEM,
    SIMPLE_ATTRIBUTES = ASSOC | COMM | IDEM | MEMO | CTOR | CONFIG | OBJECT | MESSAGE,
    ATTRIBUTES = PREC | GATHER | FORMAT | LATEX | STRAT | MEMO | FROZEN |
    CONFIG | OBJECT | MESSAGE | AXIOMS | ITER
  };

  SymbolType();
  // SymbolType(const SymbolType& other);

  int getFlags() const;
  void setFlags(int flags);
  void clearFlags(int flags);
  void assignFlags(int flags, bool value);
  int getBasicType() const;
  void setBasicType(int type);

  bool hasFlag(int flag) const;
  bool hasAllFlags(int flags) const;
  //  bool hasAxioms() const;
  bool hasAttachments() const;
  bool hasSpecial() const;
  // bool isVariable() const;
  bool isCreatedOnTheFly() const;
  bool compatible(SymbolType other) const;
  bool dittoProblem() const;
  bool nonAlgebraic() const;

  static int specialNameToBasicType(const char* name);

private:
  enum BitTwiddling
  {
    FLAG_MASK = 0xffffff,
    TYPE_SHIFT = 24
  };

  int info;
};

inline
SymbolType::SymbolType()
{
  info = 0;
}

inline int
SymbolType::getFlags() const
{
  return info & FLAG_MASK;
}

inline void
SymbolType::setFlags(int flags)
{
  info |= flags;
}

inline void
SymbolType::clearFlags(int flags)
{
  info &= ~flags;
}

inline void
SymbolType::assignFlags(int flags, bool value)
{
  if (value)
    setFlags(flags);
  else
    clearFlags(flags);
}

inline int
SymbolType::getBasicType() const
{
  return info >> TYPE_SHIFT;
}

inline void
SymbolType::setBasicType(int type)
{
  info = (info & FLAG_MASK) | type << TYPE_SHIFT;
}

inline bool
SymbolType::hasFlag(int flag) const
{
  return getFlags() & flag;
}

inline bool
SymbolType::hasAllFlags(int flags) const
{
  return (getFlags() & flags) == flags;
}

inline bool
SymbolType::hasAttachments() const
{
  int t = getBasicType();
  return (t >= BRANCH_SYMBOL) && (t < END_OF_SYMBOLS_WITH_ATTACHMENTS);
}

inline bool
SymbolType::hasSpecial() const
{
  int t = getBasicType();
  return (t >= SYSTEM_TRUE) && (t < END_OF_SYMBOLS_WITH_ATTACHMENTS);
}

inline bool
SymbolType::nonAlgebraic() const
{
  //
  //	These symbols with these types store nonalebraic "hidden" data.
  //
  int t = getBasicType();
  return t == STRING || t == FLOAT || t == QUOTED_IDENTIFIER;
}

inline bool
SymbolType::isCreatedOnTheFly() const
{
  int t = getBasicType();
  return hasFlag(POLY) || t == VARIABLE || t == SORT_TEST;
}

#ifndef NO_ASSERT
ostream& operator<<(ostream& s, SymbolType st);
#endif

#endif
