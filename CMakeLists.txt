# This file handles building each task as sub-project.
#
# Each task becomes an independent project in which you can experiment.
#
# Tasks are added as separate sub-projects so as compilation errors in
# one task don't affect another task.

# This file is intended to mimic the original configure.ac file to the extent possible.
# CMake commands are annotated with their autotools equivalents.

#
#	Initialize CMake stuff.
#
cmake_minimum_required(VERSION 3.24)
# AC_INIT(Maude, 3.3, [maude-bugs@lists.cs.illinois.edu])
project(Maude VERSION 3.3)

set(CMAKE_CXX_STANDARD 11)

# Package information
message(STATUS "For bug reports, please email: maude-bugs@lists.cs.illinois.edu")
string(TOLOWER ${PROJECT_NAME} PACKAGE)
set(PACKAGE_BUGREPORT "maude-bugs@lists.cs.illinois.edu")
set(PACKAGE_NAME "${PROJECT_NAME}")
set(PACKAGE_STRING "${PROJECT_NAME} ${PROJECT_VERSION}")
set(PACKAGE_TARNAME "${PACKAGE}")
set(PACKAGE_URL "https://maude.cs.illinois.edu/")
set(PACKAGE_VERSION "${PROJECT_VERSION}")


#
#	If the user set CFLAGS or CXXFLAGS we leave them alone.
#	Otherwise we set them to -g -Wall to stop AC_PROG_CC and
#	AC_PROG_CXX from setting them.
#
if(NOT CMAKE_CXX_FLAGS)
    set(CMAKE_CXX_FLAGS "-g -Wall")
endif()

if(NOT CMAKE_C_FLAGS)
    set(CMAKE_C_FLAGS "-g -Wall")
endif()

# For config.h
include_directories(${CMAKE_BINARY_DIR})

#
# Add Homebrew's lib to the library search path if it exists
#
option(CHECK_HOMEBREW "Look for Homebrew's lib directory" ON)
if(CHECK_HOMEBREW)
    # Try to execute 'brew --prefix' command
    execute_process(COMMAND brew --prefix
            RESULT_VARIABLE BREW_RESULT
            OUTPUT_VARIABLE BREW_PREFIX
            OUTPUT_STRIP_TRAILING_WHITESPACE)
    if (BREW_RESULT EQUAL 0)
        set(BREW_PATH ${BREW_PREFIX})
    else()
        # If the 'brew --prefix' command failed, try to get BREW_PATH from environment variable
        set(BREW_PATH $ENV{BREW_PATH})
    endif()
    # Check if BREW_PATH is set and is a valid directory
    if(BREW_PATH AND EXISTS ${BREW_PATH})
        # Add the 'lib' subdirectory of BREW_PATH to the library search path
        link_directories(${BREW_PATH}/lib)
        message("Added ${BREW_PATH}/lib to the library search path")
    endif()
endif()


# Function to find both a library and its header files. This function uses a user-supplied library instead if
# it is supplied.
function(find_and_include libname)
    # Don't overwrite a user-supplied library
    if((NOT ${libname}_LIBRARY) OR (${${libname}_LIBRARY} MATCHES "-NOTFOUND$"))
        # Prefer static libraries. CMake is supposed to already do this.
        find_library(${libname}_LIBRARY NAMES ${libname}.a ${libname})
        set(${libname}_LIBRARY ${${libname}_LIBRARY} CACHE STRING "Path of the ${libname} library" FORCE PARENT_SCOPE)
    else ()
        message(STATUS "Using existing library for ${libname}: ${${libname}_LIBRARY}")
    endif ()
    # Add the library to EXTRA_LIBS
    if(${libname}_LIBRARY AND NOT ${${libname}_LIBRARY} MATCHES "-NOTFOUND$")
        set(EXTRA_LIBS ${EXTRA_LIBS} ${${libname}_LIBRARY} PARENT_SCOPE)
    endif()

    # Don't overwrite a user-supplied library header path
    if((NOT ${libname}_INCLUDE_DIR) OR (${${libname}_INCLUDE_DIR} MATCHES "-NOTFOUND$"))
        find_path(${libname}_INCLUDE_DIR NAMES ${libname}.h DOC "Found include directory for ${libname}.h")
        set(${libname}_INCLUDE_DIR ${${libname}_INCLUDE_DIR} CACHE STRING "Path of the ${libname} include directory" FORCE PARENT_SCOPE)
    else ()
        message(STATUS "Using existing include directory for ${libname}: ${${libname}_INCLUDE_DIR}")
    endif ()
    # Add the include dir as an include directory
    if(${libname}_INCLUDE_DIR AND (NOT ${${libname}_INCLUDE_DIR} MATCHES "-NOTFOUND$"))
        include_directories(${${libname}_INCLUDE_DIR})
    endif()
endfunction()


#
#	*** PROGRAMS ***
#

# Right now a reentrant C parser is generated using `%define api.pure` and compiled as a C++ file. This option requires
# a min Bison version of 2.7, which introduced the feature..
find_package(BISON 2.7 REQUIRED)
# Minimum Flex version. It is unknown if an older version works, but this version from 2015 ships with macOS Ventura
find_package(FLEX 2.6 REQUIRED)


#
#	*** SYSTEM CALLS ***
#
include(CheckFunctionExists)
check_function_exists(ppoll HAVE_PPOLL)

#
#	*** LIBRARIES ***
#

#
#	Figure out what libraries need to be linked in to use sockets.
#
find_and_include(socket)

#
#	Check if user set particular Buddy library to use and if
#	not set default.
#
find_and_include(bdd)

#
#	Check to see if we should use Yices2 for SMT solving.
#
option(USE_YICES2 "Use Yices2 SMT solver library" ON)
if(USE_YICES2)
    add_definitions(-DUSE_YICES2)
    #
    #	Check if user set a particular Yices2 library to use and if
    #	not set default.
    #
    find_and_include(yices)
endif()

#
#	Check to see if we should use CVC4 for SMT solving.
#
option(USE_CVC4 "Use CVC4 SMT solver library" OFF)
if(USE_CVC4)
    add_definitions(-DUSE_CVC4)
    #
    #	Check if user set a particular CVC4 library to use and if
    #	not set default.
    #
    find_and_include(cvc4)
    if(cvc4_LIBRARY)
        #
        #	CVC4 needs the clock_gettime() function from the POSIX Realtime Extensions library.
        #
        find_and_include(rt_LIBRARY NAMES rt)
        if(NOT rt_LIBRARY)
            message(FATAL_ERROR "RT library required by CVC4 not found")
        endif()
    else()
        message(WARNING "CVC4 library not found")
    endif()
endif()

# One or the other of CVC4 or Yices2 must be enabled for the SMT features to be compiled.
if(NOT yices_LIBRARY AND NOT cvc4_LIBRARY)
    message(NOTICE "The experimental SMT features are disabled.")
endif()

#
#	Check to see if we should use Tecla for command line editing. This is too tricky to use `find_and_include`.
#
option(USE_TECLA "Use Tecla command line editing library" ON)
if(USE_TECLA)
    message(NOTICE "Using Tecla command line editing library")
    add_definitions(-DUSE_TECLA)
    find_and_include(tecla)
    if(NOT tecla_LIBRARY)
        find_and_include(tinfo)
        if(NOT tinfo_LIBRARY)
            find_and_include(ncurses)
        endif ()
    endif ()
endif()

#
#	Should we use libsigsegv to handle segmentation faults
#
option(USE_LIBSIGSEGV "Use libsigsegv to handle segmentation faults" ON)
if(USE_LIBSIGSEGV)
    add_definitions(-DUSE_LIBSIGSEGV)
    find_and_include(sigsegv)
endif()

#
#	Check if user set particular GMP libraries to use and if
#	not set defaults.
#
find_and_include(gmpxx)
find_and_include(gmp)
if((NOT gmpxx_LIBRARY) OR (NOT gmp_LIBRARY))
    message(FATAL_ERROR "The GMP library is required, but gmp and/or gmpxx not found.")
endif()


#
#	Check if the user set particular GCC libraries to use - otherwise
#	let the compiler pick.
#
# TODO: This line appears to have no effect on compilation. Investigate.
set(GCC_LIBS "" CACHE STRING "Specify the GCC libraries manually")

#
#	*** HEADERS ***
#
include(CheckIncludeFileCXX)
check_include_file_cxx("time.h" HAVE_TIME_H)
check_include_file_cxx("sys/time.h" HAVE_SYS_TIME_H)
check_include_file_cxx("math.h" HAVE_MATH_H)
check_include_file_cxx("ieeefp.h" HAVE_IEEEFP_H)
check_include_file_cxx("sys/termios.h" HAVE_SYS_TERMIOS_H)
check_include_file_cxx("stddef.h" HAVE_STDDEF_H)
check_include_file_cxx("limits.h" HAVE_LIMITS_H)
check_include_file_cxx("stdlib.h" HAVE_STDLIB_H)
check_include_file_cxx("stdio.h" HAVE_STDIO_H)
check_include_file_cxx("ctype.h" HAVE_CTYPE_H)
check_include_file_cxx("unistd.h" HAVE_UNISTD_H)
check_include_file_cxx("ostream" HAVE_OSTREAM)
check_include_file_cxx("ostream.h" HAVE_OSTREAM_H)

#
#	*** SIZES ***
#
include(CheckTypeSize)
check_type_size("short" SIZEOF_SHORT LANGUAGE CXX)
message(NOTICE "sizeof(short) = ${SIZEOF_SHORT}")
check_type_size("int" SIZEOF_INT LANGUAGE CXX)
message(NOTICE "sizeof(int) = ${SIZEOF_INT}")
check_type_size("long" SIZEOF_LONG LANGUAGE CXX)
message(NOTICE "sizeof(long) = ${SIZEOF_LONG}")
# Must be same as (signed) long according to the standard.
check_type_size("unsigned long" SIZEOF_UNSIGNED_LONG LANGUAGE CXX)
message(NOTICE "sizeof(unsigned long) = ${SIZEOF_UNSIGNED_LONG}")
check_type_size("void*" SIZEOF_VOID_P LANGUAGE CXX)
message(NOTICE "sizeof(void*) = ${SIZEOF_VOID_P}")

#
#   Modify optimizations for release builds.
#
string(REPLACE "-O3" "-O2" CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    #
    #   GCC specific flags
    #
    set(OPTIMIZATION_FLAGS "-fomit-frame-pointer -fforce-addr")

    if (CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 3.0)
        set(OPTIMIZATION_FLAGS "${OPTIMIZATION_FLAGS} -finline-limit=10000")
    endif()

    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${OPTIMIZATION_FLAGS}")
endif()

# Define variables for each build mode.
option(ENABLE_DEBUG "Obsolete. Use CMake build modes." OFF)
if(ENABLE_DEBUG)
    message(WARNING "The ENABLE_DEBUG flag is obsolete. Use CMake build modes instead.")
endif ()

##
## The codebase does not compile without NO_ASSERT, so we define it unconditionally.
##
#add_compile_definitions(
#    $<$<CONFIG:Debug>:DUMP>            # include dump code
#)
#add_compile_definitions(
#    $<$<CONFIG:Release>:NO_ASSERT>     # don't do runtime checks
#)
#add_compile_definitions(
#    $<$<CONFIG:Release>:LOCAL_INLINES> # inline and discard selected local functions
#)
add_compile_definitions(NO_ASSERT=ON)
add_compile_definitions(LOCAL_INLINES=ON)


#
#	Performance tweaks for known hosts.
#
set(TWEAKFLAGS "")

if (${CMAKE_SYSTEM_PROCESSOR} MATCHES "sparc-")
    #
    #	We assume the v8 instruction set as a baseline in order to
    #	have multiplication and division instructions (gcc defaults
    #	to v7 which does not) but schedule for a more recent v9
    #	implementation.
    #
    set(TWEAKFLAGS "-march=v8 -mtune=ultrasparc")
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    #
    #	Changing the stack boundary causes problems on x86 Macs.
    #
    # Do nothing.
elseif (${CMAKE_SYSTEM_PROCESSOR} MATCHES "i386-*" OR
        ${CMAKE_SYSTEM_PROCESSOR} MATCHES "i486-*" OR
        ${CMAKE_SYSTEM_PROCESSOR} MATCHES "i586-*" OR
        ${CMAKE_SYSTEM_PROCESSOR} MATCHES "i686-*")
    #
    #	For the Intel x86 family we select the smallest stack alignment
    #	in order to trade speed for memory on deeply recursive calls.
    #	The Pentium Pro and later have conditional move instructions,
    #	which out-perform various bit twiddling tricks, but gcc will
    #	only use them if given -march=i686.
    #
    set(TWEAKFLAGS "-mpreferred-stack-boundary=2")
    if (NOT USER_SET_CXXFLAGS)
        add_definitions(-DHAVE_CMOV)
    endif()
elseif (${CMAKE_SYSTEM_PROCESSOR} MATCHES "x86_64-*")
    #
    #	The x86-64 supports conditional moves and gcc will use
    #	them without being given additional flags. Smallest legal
    #	stack alignment is 4.
    #
    set(TWEAKFLAGS "-mpreferred-stack-boundary=4")
    add_definitions(-DHAVE_CMOV)
elseif (${CMAKE_SYSTEM_PROCESSOR} MATCHES "alpha-*")
    #
    #	The DEC Alpha supports conditional moves and gcc will use
    #	them without being given additional flags.
    #
    add_definitions(-DHAVE_CMOV)
endif()

if (NOT USER_SET_CFLAGS AND GCC)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${TWEAKFLAGS}")
endif()

if (NOT USER_SET_CXXFLAGS AND GXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${TWEAKFLAGS}")
endif()


#
#	Fixes for known hosts.
#
set(FIXFLAGS "")

if (${CMAKE_SYSTEM_PROCESSOR} MATCHES "alpha-*")
    #
    #	Needed to handle +/- infinity and underflow without
    #	causing a SIGFPE.
    #
    set(FIXFLAGS "-mieee")
elseif (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    #
    #	(no longer deal with dlmalloc)
    #
    add_definitions(-DDARWIN)
endif()

if (NOT USER_SET_CFLAGS AND GCC)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${FIXFLAGS}")
endif()

if (NOT USER_SET_CXXFLAGS AND GXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${FIXFLAGS}")
endif()


#
#	Needed defines.
#
#	Check to see if we should build the compiler.
#
option(ENABLE_COMPILER "Build the experimental integrated compiler" OFF)

if (ENABLE_COMPILER)
    add_definitions(-DCOMPILER)  # build the experimental integrated compiler
    add_definitions(-DANNOTATED) # annotate compiler output
endif()

#
#	Check to see if we should support MOS oracles.
#
option(ENABLE_MOS "Support MOS oracles (DO NOT USE!)" OFF)

if (ENABLE_MOS)
    add_definitions(-DMOS)
endif()


#
#	Cause defines to be put in a header file
#
add_definitions(-DHAVE_CONFIG_H)
configure_file(config.h.in config.h @ONLY)


# Add tasks subprojects
add_subdirectory(src)
#add_subdirectory(tests)
#add_subdirectory(docs)


