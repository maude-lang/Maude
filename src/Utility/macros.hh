/*

    This file is part of the Maude 3 interpreter.

    Copyright 1997-2024 SRI International, Menlo Park, CA 94025, USA.

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
#include <cstddef>
#include <climits>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#if HAVE_UNISTD_H
#include <unistd.h>  // needed by solaris
#endif
//
//	C++ stuff.
//
#include <typeinfo>
#include <cstdint>
#include <cstring>
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
class IntSet;
class Indent;
class MpzSystem;
class IntSystem;
class Rope;

//
//	Type for indexing arrays and vectors.
//
//	We want to use a signed type to avoid subtle errors caused by unsigned underflow
//	such as testing i >= 0 in a loop iterating backwards through a vector.
//      https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#es107-dont-use-unsigned-for-subscripts-prefer-gslindex
//	ES.107: Don’t use unsigned for subscripts, prefer gsl::index
//	We want to avoid 32-bit ints on a 64-bit architectures since we don't want to pay for sign-extension in loops.
//
typedef ptrdiff_t Index;

//
//	Types for storage efficiency.
//
typedef signed char Byte;
typedef unsigned char Ubyte;
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
#define NO_COPYING(c) \
  c(const c&) = delete; \
  c& operator=(const c&) = delete
 
//
//	Casting which is checked if debugging and fast otherwise.
//
#ifndef NO_ASSERT
//
//	This is good for all pointers including null pointers.
//
#define safeCast(T, P) \
  ((P != 0 && dynamic_cast<T>(P) == 0) ? \
    ((cerr << "cast error: "<< __FILE__ << ':' << __LINE__ << '\n'), abort(), static_cast<T>(0)) : \
    static_cast<T>(P))

#else
#define safeCast(T, P)	static_cast<T>(P)
#endif

template <typename T, typename P>
inline T
safeCastNonNull(P p)
{
#ifndef NO_ASSERT
  if (dynamic_cast<T>(p) == 0)
    {
      cerr << "unexpected null or cast error: "<< __FILE__ << ':' << __LINE__ << '\n';
      abort();
    }
#endif
  return static_cast<T>(p);
}

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
AlwaysAssert(condition, message) \
if (!(condition)) \
((cerr << "ASSERT FAILED: " << \
__FILE__ << ':' << __LINE__ << '\n' << message << endl), \
abort())


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
if (!(condition) && globalDebugFlag) \
((cerr << Tty(Tty::BLUE) << "DEBUG ADVISORY: " << Tty(Tty::RESET) << message << endl))

#define \
DebugAdvisory(message) \
if (globalDebugFlag) \
(cerr << Tty(Tty::BLUE) << "DEBUG ADVISORY: " << Tty(Tty::RESET) << message << endl)

#define \
DebugEnter(message) \
if (globalDebugFlag) \
(cerr << Tty(Tty::MAGENTA) << "DEBUG ENTER: " << __PRETTY_FUNCTION__ << ": " << Tty(Tty::RESET) << message << endl)

#define \
DebugExit(message) \
if (globalDebugFlag) \
(cerr << Tty(Tty::CYAN) << "DEBUG EXIT: " << __PRETTY_FUNCTION__ << ": " << Tty(Tty::RESET) << message << endl)

#define \
DebugInfo(message) \
if (globalDebugFlag) \
(cerr << Tty(Tty::BLUE) << "DEBUG INFO: " << __PRETTY_FUNCTION__ << ": " << Tty(Tty::RESET) << message << endl)

//
//	Can't be turned off - for debugging temporary use only
//
#define \
DebugAlways(message) \
(cerr << Tty(Tty::RED) << "DEBUG ALWAYS: " << __PRETTY_FUNCTION__ << ": " << Tty(Tty::RESET) << message << endl)

#define \
DebugNew(message) \
if (globalDebugFlag) \
(cerr << Tty(Tty::REVERSE) << "DEBUG NEW: " << __PRETTY_FUNCTION__ << ": " << Tty(Tty::RESET) << message << endl)

#define \
DebugPrint(v) \
if (globalDebugFlag) \
(cerr << #v << " = " << v << '\t')

#define \
DebugPrintNL(v) \
if (globalDebugFlag) \
(cerr << #v << " = " << v << '\n')

//
//	Always evaluate e and save the result in new variable v only if needed for debug mode.
//
#define DebugSave(v, e) auto v = (e)
#else

#define \
AlwaysAssert(condition, message) \
if (!(condition)) \
((cerr << "ASSERT FAILED: " << \
__FILE__ << ':' << __LINE__ << '\n' << message << endl), \
selfCheckFailHandler())

#define Assert(condition, message)
#define CantHappen(message)
#define DebugAdvisoryCheck(condition, message)
#define DebugAdvisory(message)
#define DebugEnter(message)
#define DebugExit(message)
#define DebugInfo(message)
#define DebugAlways(message)
#define DebugNew(message)
#define DebugPrint(v)
#define DebugPrintNL(v)

//
//	Always evaluate e and save the result in new variable v only if needed for debug mode.
//
#define DebugSave(v, e) (void) (e)

#endif

//	To quiten old debug messages instead of commenting them out
#define DebugOld(message)


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
ContinueWarning(message) \
(cerr << message)

#define \
IssueAdvisory(message) \
(globalAdvisoryFlag ? (cerr << ADVISORY_HEADER << message << endl) : cerr)

#define \
Verbose(output) \
if (globalVerboseFlag) \
  (cerr << Tty(Tty::CYAN) << output << Tty(Tty::RESET) << '\n')

#ifdef PROFILING

#define \
Profile(color, message) \
  (cerr << Tty(Tty::color) << message << Tty(Tty::RESET))

#else

#define \
Profile(color, message)

#endif

extern bool globalAdvisoryFlag;
extern bool globalVerboseFlag;
extern bool globalDebugFlag;
//
//	Used to circumvent GCC's unused result warnings; because (void)
//	is deemed an inadequate indication that we REALLY don't care about
//	a system call or library result, no matter what the runtime system
//	or library author thinks.
//
extern int returnValueDump;

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
pluralize(int_fast64_t quantity)
{
  return (quantity == 1) ? "" : "s";
}

const char* int64ToString(int64_t i, int base = 10);
int64_t stringToInt64(const char* s, bool& error, int base = 10);
bool looksLikeFloat(const char* s);
const char* doubleToString(double d);
double stringToDouble(const char* s, bool& error);
void correctEcvt(double d, int nrDigits, char buffer[], int& decPt, int& sign);
void selfCheckFailHandler();

#endif
