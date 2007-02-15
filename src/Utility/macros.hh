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
//	Macros, typedefs, enums and includes.
//
#ifndef _macros_hh_
#define _macros_hh_

//
//	#defines created by configure.
//
#if HAVE_CONFIG_H
#include <config.h>
#endif
//
//	C stuff.
//
#if HAVE_STDDEF_H
#include <stddef.h>
#endif
#if HAVE_LIMITS_H
#include <limits.h>
#endif
#if HAVE_STDLIB_H
#include <stdlib.h>
#endif
#if HAVE_STDIO_H
#include <stdio.h>
#endif
#if HAVE_CTYPE_H
#include <ctype.h>
#endif
#if HAVE_STRINGS_H
#include <strings.h>
#elif HAVE_STRING_H
#include <string.h>
#endif
#if HAVE_UNISTD_H
#include <unistd.h>  // needed by solaris
#endif
//
//	C++ stuff.
//
#include <new>
#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
using namespace std;

//
//	Forward declarations for utility classes.
//
class NatSet;
class Graph;
class Digraph;
class UnionFind;
class DiophantineSystem;
class SequencePartition;
class StringTable;
class SymbolTable;
class PointerMap;
class PointerSet;
class BddUser;
class Bdd;
class Tty;
class Timer;
class FlagSet;
class AllSat;

//
//	Types for storage efficiency.
//
typedef char Bool;
typedef signed char Byte;
typedef unsigned char Ubyte;
//
//	Shorthand.
//
typedef unsigned int Uint;
//
//	64 bit arithmetic; since it's now practical to deal with quantities
//	of rewrites/matchers/unifiers etc that overflow 32 bits.
//
#ifdef INT64_MAX
#undef INT64_MAX
#endif
#ifdef INT64_MIN
#undef INT64_MIN
#endif
#if SIZEOF_LONG == 8
typedef long Int64;
#define INT64_MAX		(9223372036854775807L)
#else
typedef long long int Int64;
#define INT64_MAX		(9223372036854775807LL)
#endif
#define INT64_MIN		(-INT64_MAX-1)
#define INT64_MIN_STRING	"-9223372036854775808"

#if SIZEOF_UNSIGNED_LONG == 8
typedef unsigned long Uint64;
#else
typedef unsigned long long int Uint64;
#endif
//
//	32 bit arithmetic; sometime we need guarentee of left truncation.
//
typedef int Int32;
typedef unsigned int Uint32;

//
//	floating point
//
//	This is the largest integer such that all integers in the range
//	-INT_DOUBLE_MAX, ..., INT_DOUBLE_MAX
//	can be represented exactly.
//
#define INT_DOUBLE_MAX		(1LL << 53)

//
//	Macro to forbid default copy ctor and assignment operator
//
#define NO_COPYING(c)	c(const c&); c& operator=(const c&)

//
//	Casting which is checked if debugging and fast otherwise.
//
#ifndef NO_ASSERT
#define safeCast(T, P) \
  ((P != 0 && dynamic_cast<T>(P) == 0) ? \
    ((cerr << "cast error: "<< __FILE__ << ':' << __LINE__ << '\n'), abort(), static_cast<T>(0)) : \
    static_cast<T>(P))
#else
#define safeCast(T, P)	static_cast<T>(P)
#endif

enum SpecialConstants
{
  NR_PTR_LOSE_BITS = 3,		// number of rhs bits to chop when hashing a pointer
  WORD_SIZE = 32,		// bits per machine word
  LOG_WORD_SIZE = 5,		// log_2 (WORD_SIZE)
  UNBOUNDED = INT_MAX,		// integer used as stand-in for +infinity
  INT_TEXT_SIZE = 11,		// max chars for text representation of an int (INT_MIN)
  INT64_TEXT_SIZE = 20,		// max chars for text representation of a 64 bit int (INT64_MIN)
  DOUBLE_TEXT_SIZE = 24,	// max chars for text representation of a double in
  				// in scientific format with 17 significant figures
  				// -1.2345678901234567e-123
  //
  //	-1 get used for lots of "out-of-band" purposes.
  //
  UNDEFINED = -1,
  UNDECIDED = -1,		// for true, false, don't know situations
  UNUSED = -1,
  NONE = -1,
  DEFAULT = -1,
  //
  //	For bit twiddling; things will probably break if bytes are ever
  //	longer or shorter than 8 bits.
  //
  BITS_PER_BYTE = 8,
  BITS_PER_INT = BITS_PER_BYTE * sizeof(int),
  BITS_PER_UINT = BITS_PER_BYTE * sizeof(unsigned int),
  //
  //	GARBAGE is used where we want a value to store or pass as an argument and we don't
  //	care about which value. For most CPUs, 0 is the easiest value to generate but
  //	when debugging we want a value that sticks out like a raw thumb in case we mistakenly
  //	try to use it for something. Note that GARBAGE should not be used as an out-of-band
  //	value that we test against since in this case we _do_ care what value is used.
  //
#ifdef NO_ASSERT
  GARBAGE = 0
#else
  GARBAGE = -333333333
#endif
};

#ifdef NO_IMPLEMENT_INLINES
//#define inline extern __inline__
#endif
//
//	For functions local to a compilation unit that can be inlined
//	in that compilation unit and discarded.
//
#ifdef LOCAL_INLINES
  #ifdef __GNUC__
    #define local_inline extern __inline__
  #else
    #define local_inline inline
  #endif
#else
  #define local_inline
#endif

#ifndef NO_ASSERT

#define \
Assert(condition, message) \
if (!(condition)) \
((cerr << "ASSERT FAILED: " << \
__FILE__ << ':' << __LINE__ << '\n' << message << endl), \
abort())

#define \
CantHappen(message) \
((cerr << "CAN'T HAPPEN: " << \
__FILE__ << ':' << __LINE__ << '\n' << message << endl), \
abort())

#define \
DebugAdvisoryCheck(condition, message) \
if (!(condition) && globalAdvisoryFlag) \
((cerr << Tty(Tty::BLUE) << "DEBUG ADVISORY: " << Tty(Tty::RESET) << message << endl))

#define \
DebugAdvisory(message) \
if (globalAdvisoryFlag) \
(cerr << Tty(Tty::BLUE) << "DEBUG ADVISORY: " << Tty(Tty::RESET) << message << endl)

#define \
DebugPrint(v) \
if (globalAdvisoryFlag) \
(cerr << #v << " = " << v << '\t')

#define \
DebugPrintNL(v) \
if (globalAdvisoryFlag) \
(cerr << #v << " = " << v << '\n')

#else

#define Assert(condition, message)
#define CantHappen(message)
#define DebugAdvisoryCheck(condition, message)
#define DebugAdvisory(message)
#define DebugPrint(v)
#define DebugPrintNL(v)

#endif

#include "tty.hh"
#define WARNING_HEADER	Tty(Tty::RED) << "Warning: " << Tty(Tty::RESET)
#define ADVISORY_HEADER	Tty(Tty::GREEN) << "Advisory: " << Tty(Tty::RESET)
#define BEGIN_QUOTE	Tty(Tty::MAGENTA)
#define END_QUOTE	Tty(Tty::RESET)
#define QUOTE(s)	BEGIN_QUOTE << s << END_QUOTE

#define \
WarningCheck(condition, message) \
if (!(condition)) \
(cerr << WARNING_HEADER << message << endl)

#define \
AdvisoryCheck(condition, message) \
if (!(condition) && globalAdvisoryFlag) \
(cerr << ADVISORY_HEADER << message << endl)

#define \
IssueWarning(message) \
(cerr << WARNING_HEADER << message << endl)

#define \
ComplexWarning(message) \
(cerr << WARNING_HEADER << message)

#define \
IssueAdvisory(message) \
if (globalAdvisoryFlag) \
(cerr << ADVISORY_HEADER << message << endl)

#define \
Verbose(output) \
if (globalVerboseFlag) \
(cout << output << '\n')

extern bool globalAdvisoryFlag;
extern bool globalVerboseFlag;

//
//	A machine word should be about to hold any pointer, int or size
//	(but not necessarily an Int64).
//
union MachineWord
{
  void* pointer;
  int integer;
  size_t size;
};

//
//	Macro for common const_iterator loop.
//
//	Too bad we don't have a portable typeof operator.
//
#define FOR_EACH_CONST(var, conType, container) \
const conType::const_iterator var##_end = (container).end(); \
for (conType::const_iterator var = (container).begin(); var != var##_end; ++var)

inline int
uplus(int a, int b)
{
  return (a == UNBOUNDED || b == UNBOUNDED) ? UNBOUNDED : (a + b);
}

inline int
floorDivision(int dividend, int divisor)
{
  if (divisor > 0)
    {
      return (dividend >= 0) ? (dividend / divisor)
	: -((divisor - dividend - 1) / divisor);
    }
  else
    {
      Assert(divisor < 0, "zero divisor");
      return (dividend >= 0) ? -((dividend - divisor - 1) / (-divisor))
	: ((-dividend) / (-divisor));
    }
}

inline int
ceilingDivision(int dividend, int divisor)
{
  if (divisor > 0)
    {
      return (dividend >= 0) ? ((dividend + divisor - 1) / divisor)
	: -((-dividend) / divisor);
    }
  else
    {
      Assert(divisor < 0, "zero divisor");
      return (dividend >= 0) ? -(dividend / (-divisor))
	: (((-dividend) + (-divisor) - 1) / (-divisor));
    }
}

inline const char*
pluralize(int quantity)
{
  return (quantity == 1) ? "" : "s";
}

//
//	Branch free conditional assignments. These use bit twiddling rather
//	than branches and are intended for performance critical loops
//	where the branch would be unpredicatable. Executing a couple of extra
//	instructions is better than having a high rate of mispredicts on
//	modern superpipelined architectures.
//
//	If the instruction set supports conditional moves then the naive code
//	will beat the bit twiddling.
//

inline void
setOnLs(int& d, int v, int t)
{
  //
  //	set d to v iff t < 0
  //
#ifdef HAVE_CMOV
  if (t < 0)
    d = v;
#else
  d += (t >> (BITS_PER_INT - 1)) & (v - d);
#endif
}

inline void
setOnGeq(int& d, int v, int t)
{
  //
  //	set d to v iff t >= 0
  //
#ifdef HAVE_CMOV
  if (t >= 0)
    d = v;
#else
  d += (~(t >> (BITS_PER_INT - 1))) & (v - d);
#endif
}

inline int
getSignBit(int n)
{
  //
  //	Return 1 if -ve, 0 owise.
  //
  return (n >> (BITS_PER_INT - 1)) & 1;
}

const char* int64ToString(Int64 i, int base = 10);
Int64 stringToInt64(const char* s, bool& error, int base = 10);
bool looksLikeFloat(const char* s);
const char* doubleToString(double d);
double stringToDouble(const char* s, bool& error);
void correctEcvt(double d, int nrDigits, char buffer[], int& decPt, int& sign);

#endif
