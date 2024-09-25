/*

    This file is part of the Maude 3 interpreter.

    Copyright 1997-2023 SRI International, Menlo Park, CA 94025, USA.

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
//      Macros for binding stuff in builtins.
//
#ifndef _bindingMacros_hh_
#define _bindingMacros_hh_

//
//	This needs to be a macro in order to produce constant expressions.
//
#define CODE(c1, c2)	((c1) + ((c2) << 8))
#define CODE3(c1, c2, c3)	((c1) + ((c2) << 8) + ((c3) << 16))

#define BIND_OP(purpose, className, op, data) \
  if (strcmp(purpose, #className) == 0) \
    { \
      if (data.length() == 1) \
	{ \
	  const char* opName = (data)[0]; \
	  if (opName[0] != '\0') \
	    { \
	      int t = CODE(opName[0], opName[1]); \
	      if (op == NONE) \
		{ \
		  op = t; \
		  return true; \
		} \
	      if (op == t) \
		return true; \
	    } \
	} \
      return false; \
    }

#define BIND_OP3(purpose, className, op, data) \
  if (strcmp(purpose, #className) == 0) \
    { \
      if (data.length() == 1) \
	{ \
	  const char* opName = (data)[0]; \
	  if (opName[0] != '\0') \
	    { \
	      int t = (opName[1] == '\0') ? CODE(opName[0], opName[1]) : CODE3(opName[0], opName[1], opName[2]); \
	      if (op == NONE) \
		{ \
		  op = t; \
		  return true; \
		} \
	      if (op == t) \
		return true; \
	    } \
	} \
      return false; \
    }

#define NULL_DATA(purpose, className, data) \
  if (strcmp(purpose, #className) == 0) \
    { \
      return data.empty();		\
    } 

#define BIND_SYMBOL(purpose, symbol, name, symbolType) \
  if (strcmp(purpose, #name) == 0) \
    { \
      if (name != 0) \
	return name == symbol; \
      name = dynamic_cast<symbolType>(symbol); \
      return name != 0; \
    }

#define BIND_SYMBOL2(purpose, symbol, name, symbolType, nrArgs)	\
  if (strcmp(purpose, #name) == 0) \
    { \
      if (name != 0) \
	return name == symbol; \
      if (symbol->arity() != nrArgs) \
	return false; \
      name = dynamic_cast<symbolType>(symbol); \
      return name != 0; \
    }

#define BIND_TERM(purpose, term, name) \
  if (strcmp(purpose, #name) == 0) \
    { \
      bool r = true; \
      if (Term* t = name.getTerm()) \
	{ \
	  r = term->equal(t); \
	  term->deepSelfDestruct(); \
	} \
      else \
	name.setTerm(term); \
      return r; \
    }

#define PREPARE_TERM(name) \
  if (name.getTerm() != 0) \
    { \
      (void) name.normalize(); \
      name.prepare(); \
    }

#define COPY_SYMBOL(original, name, mapping, symbolType) \
  if (name == 0) \
    { \
      if (symbolType s = original->name) \
	name = (mapping == 0) ? s : safeCast(symbolType, mapping->translate(s)); \
    }

#define COPY_TERM(original, name, mapping) \
  if (name.getTerm() == 0) \
    { \
      if (Term* t = original->name.getTerm()) \
	name.setTerm(t->deepCopy(mapping)); \
    }

#define APPEND_DATA(purposes, data, name) \
  { \
    int n = purposes.length(); \
    purposes.resize(n + 1); \
    purposes[n] = #name; \
    data.resize(n + 1); \
  }

#define APPEND_SYMBOL(purposes, symbols, name) \
  if (name != 0) \
    { \
      purposes.append(#name); \
      symbols.append(name); \
    }

#define APPEND_TERM(purposes, terms, name) \
  if (Term* t = name.getTerm()) \
    { \
      purposes.append(#name); \
      terms.append(t); \
    }

#define CODE_CASE(d, c1, c2, s) \
  case CODE(c1, c2): \
    { \
      d = s; \
      break; \
    }

#define CODE_CASE3(d, c1, c2, c3, s)		\
  case CODE3(c1, c2, c3):	       		\
    { \
      d = s; \
      break; \
    }

#endif
