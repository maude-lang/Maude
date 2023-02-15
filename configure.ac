#	Process this file with autoconf to produce a configure script.

#
#	Initialize autoconf stuff.
#
AC_INIT(Maude, alpha140, [maude-bugs@lists.cs.illinois.edu])
#
#	Allow directory names that look like macros.
#
m4_pattern_allow([AU_Persistent])
m4_pattern_allow([AU_Theory])
#
#	Local macros.
#
AC_CONFIG_MACRO_DIRS([m4])
#
#	Figure out system we are building for and set $host shell variable.
#
AC_CANONICAL_HOST
#
#	Initialize automake stuff.
#
AM_INIT_AUTOMAKE([tar-ustar foreign])
#
#	Cause defines to be put in a header file rather than passed as
#	compiler flags.
#
AC_CONFIG_HEADER(config.h)
#
#	If the user set CFLAGS or CXXFLAGS we leave them alone.
#	Otherwise we set them to -g -Wall to stop AC_PROG_CC and
#	AC_PROG_CXX from setting them.
#
USER_SET_CFLAGS=yes
if (test -z "$CFLAGS") then
  USER_SET_CFLAGS=no
  CFLAGS="-g -Wall"
fi
USER_SET_CXXFLAGS=yes
if (test -z "$CXXFLAGS") then
  USER_SET_CXXFLAGS=no
  CXXFLAGS="-g -Wall"
fi
#
#	*** PROGRAMS ***
#
AC_PROG_CC
AC_PROG_CXX
if (test "$GXX" = yes) then
  AC_MSG_CHECKING([g++ compiler version])
  GXX_VERSION=`$CXX -dumpversion`
  AC_MSG_RESULT($GXX_VERSION)
fi
AC_PROG_RANLIB
if (test -z "$BISON") then
  BISON="bison"
fi
AC_SUBST(BISON)
if (test -z "$FLEX") then
  FLEX="flex"
fi
AC_SUBST(FLEX)
#
#	*** SYSTEM CALLS ***
#
AX_HAVE_PPOLL(AC_DEFINE([HAVE_PPOLL], [], [use ppoll()]))
#
#	*** LIBRARIES ***
#
#
#	Figure out what libraries need to be linked in to use sockets.
#
#AC_CHECK_LIB(nsl, inet_addr)	# Adds unnecessary dependencies to the binaries in Linux
AC_CHECK_LIB(socket, socket)
#
#	Check if user set particular Buddy library to use and if
#	not set default.
#
if (test -z "$BUDDY_LIB") then
  BUDDY_LIB="-lbdd"
fi
AC_SUBST(BUDDY_LIB)
#
#	Check to see if we should use Yices2 for SMT solving.
#
AC_ARG_WITH(yices2,
	    AC_HELP_STRING([--with-yices2],
                           [use Yices2 SMT solver library [default=yes]]),
			   [WITH_YICES2=$withval],
			   [WITH_YICES2=yes])
if (test $WITH_YICES2 = yes) then
#
#	Check if user set a particular Yices2 library to use and if
#	not set default.
#
  AC_DEFINE([USE_YICES2], [], [use Yices2 SMT solver library])
  if (test -z "$YICES2_LIB") then
    YICES2_LIB="-lyices"
  fi
  AC_SUBST(YICES2_LIB)
else
  YICES2_LIB=""
fi
#
#	Check to see if we should use CVC4 for SMT solving.
#
AC_ARG_WITH(cvc4,
	    AC_HELP_STRING([--with-cvc4],
                           [use CVC4 SMT solver library [default=no]]),
			   [WITH_CVC4=$withval],
			   [WITH_CVC4=no])
if (test $WITH_CVC4 = yes) then
#
#	Check if user set a particular CVC4 library to use and if
#	not set default.
#
  AC_DEFINE([USE_CVC4], [], [use CVC4 SMT solver library])
  if (test -z "$CVC4_LIB") then
    CVC4_LIB="-lcvc4"
  fi
  AC_SUBST(CVC4_LIB)

#
#	CVC4 needs the clock_gettime() function from the POSIX Realtime Extensions library.
#
  AC_CHECK_LIB(rt, clock_gettime)
else
  CVC4_LIB=""
fi
#
#	Check to see if we should use Tecla for command line editing.
#
AC_ARG_WITH(tecla,
	    AC_HELP_STRING([--with-tecla],
                           [use Tecla command line editing library [default=yes]]),
			   [WITH_TECLA=$withval],
			   [WITH_TECLA=yes])
if (test $WITH_TECLA = yes) then
#
#	Check if user set particular Tecla libraries to use and if
#	not set defaults.
#
  AC_DEFINE([USE_TECLA], [], [use Tecla command line editing library])
  if (test -z "$TECLA_LIBS") then
    TECLA_LIBS="-ltecla -lcurses"
  fi
else
  TECLA_LIBS=""
fi
AC_SUBST(TECLA_LIBS)
#
#	Check to see if we should use libsigsegv to handle segmentation faults.
#
AC_ARG_WITH(libsigsegv,
	    AC_HELP_STRING([--with-libsigsegv],
                           [use libsigsegv to handle segmentation faults [default=yes]]),
			   [WITH_LIBSIGSEGV=$withval],
			   [WITH_LIBSIGSEGV=yes])
if (test $WITH_LIBSIGSEGV = yes) then
#
#	Check if user set particular libsigsegv to use and if
#	not set default.
#
  AC_DEFINE([USE_LIBSIGSEGV], [], [use libsigsegv to handle segmentation faults])
  if (test -z "$LIBSIGSEGV_LIB") then
    LIBSIGSEGV_LIB="-lsigsegv"
  fi
else
  LIBSIGSEGV_LIB=""
fi
AC_SUBST(LIBSIGSEGV_LIB)
#
#	Check if user set particular GMP libraries to use and if
#	not set defaults.
#
if (test -z "$GMP_LIBS") then
  GMP_LIBS="-lgmpxx -lgmp"
fi
AC_SUBST(GMP_LIBS)
#
#	Check if the user set particular GCC libraries to use - otherwise
#	let the compiler pick.
#
if (test -z "$GCC_LIBS") then
  GCC_LIBS=""
fi
AC_SUBST(GCC_LIBS)
#
#	*** HEADERS ***
#
AC_LANG(C++)
AC_HEADER_TIME 
AC_CHECK_HEADERS(sys/time.h)
AC_CHECK_HEADERS(math.h)
AC_CHECK_HEADERS(ieeefp.h)
AC_CHECK_HEADERS(sys/termios.h)
AC_CHECK_HEADERS(stddef.h)
AC_CHECK_HEADERS(limits.h)
AC_CHECK_HEADERS(stdlib.h)
AC_CHECK_HEADERS(stdio.h)
AC_CHECK_HEADERS(ctype.h)
AC_CHECK_HEADERS(unistd.h)
AC_CHECK_HEADERS(ostream)
AC_CHECK_HEADERS(ostream.h)
#
#	*** SIZES ***
#
AC_CHECK_SIZEOF(short)
AC_CHECK_SIZEOF(int)
AC_CHECK_SIZEOF(long)
AC_CHECK_SIZEOF(unsigned long)
AC_CHECK_SIZEOF(void *)
#
#	Check to see if we are debugging.
#	If we are not debugging, assume we are optimizing.
#
AC_ARG_ENABLE(debug,
	      AC_HELP_STRING([--enable-debug],
                             [no optimization, runtime checks, dump routines [default=no]]),
	      [DEBUGGING=$enableval],
	      [DEBUGGING=no])

if (test "$DEBUGGING" = yes) then
  AC_DEFINE([DUMP], [], [include dump code])
else
  AC_DEFINE([NO_ASSERT], [], [don't do runtime checks])
  AC_DEFINE([LOCAL_INLINES], [], [inline and discard selected local functions])
#
#	Set compiler optimization flags if user didn't set CXXFLAGS.
#
  if (test "$GXX" = yes) then
    OFLAGS="-O2 -fomit-frame-pointer -fforce-addr"
    case $GXX_VERSION in
    3.*)
      OFLAGS="$OFLAGS -finline-limit=10000"
      ;;
    4.*)
      OFLAGS="$OFLAGS -finline-limit=10000"
      ;;
    esac
  else
    OFLAGS="-O"
  fi
  if (test $USER_SET_CFLAGS = no) then
    CFLAGS="$CFLAGS $OFLAGS"
  fi
  if (test $USER_SET_CXXFLAGS = no) then
    CXXFLAGS="$CXXFLAGS $OFLAGS"
  fi
#
#	Performance tweaks for known hosts.
#
  TWEAKFLAGS=""

  case $host in

  sparc-*)
#
#	We assume the v8 instruction set as a baseline in order to
#	have multiplication and division instructions (gcc defaults
#	to v7 which does not) but schedule for a more recent v9
#	implementation.
#
    TWEAKFLAGS="-march=v8 -mtune=ultrasparc"
    ;;

  *-darwin*)
#
#	Changing the stack boundary causes problems on x86 Macs.
#
    ;;
#
#	For the Intel x86 family we select the smallest stack alignment
#	in order to trade speed for memory on deeply recursive calls.
#	The Pentium Pro and later have conditional move instructions,
#	which out-perform various bit twiddling tricks, but gcc will
#	only use them if given -march=i686.
#
  i386-*)
    TWEAKFLAGS="-march=i386 -mpreferred-stack-boundary=2"
    ;;

  i486-*)
    TWEAKFLAGS="-march=i486 -mpreferred-stack-boundary=2"
    ;;

  i586-*)
    TWEAKFLAGS="-march=i586 -mpreferred-stack-boundary=2"
    ;;

  i686-*)
    TWEAKFLAGS="-march=i686 -mpreferred-stack-boundary=2"
    if (test $USER_SET_CXXFLAGS = no) then
      AC_DEFINE([HAVE_CMOV], [], [have conditional move instruction])
    fi
    ;;
#
#	The x86-64 supports conditional moves and gcc will use
#	them without being given additional flags. Smallest legal
#	stack alignment is 4.
#
  x86_64-*)
    TWEAKFLAGS="-mpreferred-stack-boundary=4"
    AC_DEFINE([HAVE_CMOV], [], [have conditional move instruction])
    ;;
#
#	The DEC Alpha supports conditional moves and gcc will use
#	them without being given additional flags.
#
  alpha*)
    AC_DEFINE([HAVE_CMOV], [], [have conditional move instruction])
    ;;

  esac

  if (test $USER_SET_CFLAGS = no) && (test $GCC = yes) then
    CFLAGS="$CFLAGS $TWEAKFLAGS"
  fi
  if (test $USER_SET_CXXFLAGS = no) && (test $GXX = yes) then
    CXXFLAGS="$CXXFLAGS $TWEAKFLAGS"
  fi
fi
#
#	Fixes for known hosts.
#
FIXFLAGS=""
case $host in

#
#	Needed to handle +/- infinity and underflow without
#	causing a SIGFPE.
#
  alpha*)
    FIXFLAGS="-mieee"
    ;;
#
#	(no longer deal with dlmalloc)
#
  x86_64-*-linux-*)
    ;;
#
#	(no longer deal with dlmalloc)
#    
  *-darwin*)
    AC_DEFINE([DARWIN], [], [enable Darwin specific fixes])
    ;;

esac

if (test $USER_SET_CFLAGS = no) && (test $GCC = yes) then
  CFLAGS="$CFLAGS $FIXFLAGS"
fi
if (test $USER_SET_CXXFLAGS = no) && (test $GXX = yes) then
  CXXFLAGS="$CXXFLAGS $FIXFLAGS"
fi

#
#	Needed defines.
#
#	Check to see if we should build the compiler.
#
AC_ARG_ENABLE(compiler,
	      AC_HELP_STRING([--enable-compiler],
                             [build the experimental integrated compiler [default=no]]),
	      [COMPILER=$enableval],
	      [COMPILER=no])
if (test "$COMPILER" = yes) then
  AC_DEFINE([COMPILER], [], [build the experimental integrated compiler])
  AC_DEFINE([ANNOTATED], [], [annotate comiler output])
fi
#
#	Check to see if we should support MOS oracles.
#
AC_ARG_ENABLE(mos,
	      AC_HELP_STRING([--enable-mos],
                             [support MOS oracles (DO NOT USE!) [default=no]]),
	      [MOS=$enableval],
	      [MOS=no])
if (test "$MOS" = yes) then
  AC_DEFINE([MOS], [], [support MOS oracles])
fi
#
#	Conditional compilation and linking.
#
AM_CONDITIONAL(BUILD_COMPILER, test $COMPILER = yes) 

AC_CONFIG_FILES([Makefile
	         src/Makefile
		 src/Utility/Makefile
		 src/Parser/Makefile
		 src/Temporal/Makefile
		 src/Interface/Makefile
		 src/Core/Makefile
		 src/Variable/Makefile
		 src/NA_Theory/Makefile
		 src/ACU_Persistent/Makefile
		 src/ACU_Theory/Makefile
		 src/AU_Persistent/Makefile
		 src/AU_Theory/Makefile
		 src/CUI_Theory/Makefile
		 src/S_Theory/Makefile
		 src/FreeTheory/Makefile
		 src/Higher/Makefile
		 src/BuiltIn/Makefile
		 src/IO_Stuff/Makefile
		 src/ObjectSystem/Makefile
		 src/Meta/Makefile
		 src/FullCompiler/Makefile
		 src/StrategyLanguage/Makefile
		 src/Mixfix/Makefile
		 src/SMT/Makefile
		 src/Main/Makefile
		 tests/Makefile
		 tests/BuiltIn/Makefile
		 tests/Meta/Makefile
		 tests/Misc/Makefile
		 tests/ResolvedBugs/Makefile
		 tests/StrategyLanguage/Makefile
		 tests/ObjectOriented/Makefile
])

AC_OUTPUT
