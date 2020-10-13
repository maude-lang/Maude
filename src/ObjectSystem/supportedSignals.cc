/*

    This file is part of the Maude 3 interpreter.

    Copyright 2020 SRI International, Menlo Park, CA 94025, USA.

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
//	List of all Unix signals supported the process subsystem.
//
//	We generate various chunks of linear (branching) code to handle
//	this collection of 29 standard signals plus 2 Darwin specific
//	signals by macro expansion.
//	The format is:
//		MACRO(Unix signal name)
//
MACRO(SIGHUP)
MACRO(SIGINT)
MACRO(SIGQUIT)
MACRO(SIGILL)
MACRO(SIGTRAP)

MACRO(SIGABRT)
#ifdef DARWIN
MACRO(SIGEMT)
#endif
MACRO(SIGFPE)
MACRO(SIGKILL)
MACRO(SIGBUS)

MACRO(SIGSEGV)
MACRO(SIGSYS)
MACRO(SIGPIPE)
MACRO(SIGALRM)
MACRO(SIGTERM)

MACRO(SIGURG)
MACRO(SIGSTOP)
MACRO(SIGTSTP)
MACRO(SIGCONT)
MACRO(SIGCHLD)

MACRO(SIGTTIN)
MACRO(SIGTTOU)
MACRO(SIGIO)
MACRO(SIGXCPU)
MACRO(SIGXFSZ)

MACRO(SIGVTALRM)
MACRO(SIGPROF)
MACRO(SIGWINCH)
#ifdef DARWIN
MACRO(SIGINFO)
#endif
MACRO(SIGUSR1)
MACRO(SIGUSR2)
