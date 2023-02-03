/*

    This file is part of the Maude 3 interpreter.

    Copyright 1997-2006 SRI International, Menlo Park, CA 94025, USA.

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
//	List of all the tokens special enough that we keep track of their id
//	in the symbol table.
//
//	We generate various chunks of linear (branching) code to handle
//	this hetrogeneous collection by macro expansion.
//	The format is:
//		MACRO(name, string)
//
  MACRO(leftParen, "(")
  MACRO(rightParen, ")")
  MACRO(leftBracket, "[")
  MACRO(rightBracket, "]")
  MACRO(leftBrace, "{")
  MACRO(rightBrace, "}")
  MACRO(comma, ",")
  MACRO(underscore, "_")
  MACRO(dot, ".")
  MACRO(equals, "=")
  MACRO(assign, ":=")
  MACRO(arrow, "=>")
  MACRO(arrowOne, "=>1")
  MACRO(arrowPlus, "=>+")
  MACRO(arrowStar, "=>*")
  MACRO(arrowBang, "=>!")
  MACRO(suchThat, "s.t.")
  MACRO(such, "such")
  MACRO(that, "that")
  MACRO(mb, "mb")
  MACRO(cmb, "cmb")
  MACRO(eq, "eq")
  MACRO(ceq, "ceq")
  MACRO(rl, "rl")
  MACRO(crl, "crl")
  MACRO(cq, "cq")
  MACRO(sd, "sd")
  MACRO(csd, "csd")
  MACRO(ifToken, "if")
  MACRO(colon, ":")
  MACRO(colon2, "::")
  MACRO(wedge, "/\\")
  MACRO(label, "label")
  MACRO(metadata, "metadata")
  MACRO(nonexec, "nonexec")
  MACRO(otherwise, "otherwise")
  MACRO(owise, "owise")
  MACRO(variant, "variant")
  MACRO(narrowing, "narrowing")
  MACRO(dnt, "dnt")
  MACRO(print, "print")
  MACRO(lessThan, "<")
  MACRO(partial, "~>")
  MACRO(th, "th")
  MACRO(fth, "fth")
  MACRO(sth, "sth")
  MACRO(oth, "oth")
  MACRO(mod, "mod")
  MACRO(fmod, "fmod")
  MACRO(smod, "smod")
  MACRO(omod, "omod")
  MACRO(obj, "obj")
  MACRO(endth, "endth")
  MACRO(endfth, "endfth")
  MACRO(endsth, "endsth")
  MACRO(endoth, "endoth")
  MACRO(endm, "endm")
  MACRO(endfm, "endfm")
  MACRO(endsm, "endsm")
  MACRO(endom, "endom")
  MACRO(endo, "endo")
  MACRO(jbo, "jbo")
  MACRO(pr, "pr")
  MACRO(protecting, "protecting")
  MACRO(ex, "ex")
  MACRO(extending, "extending")
  MACRO(inc, "inc")
  MACRO(including, "including")
  MACRO(us, "us")
  MACRO(usingToken, "using")
  MACRO(fail, "fail")
  MACRO(idle, "idle")
  MACRO(all, "all")
  MACRO(top, "top")
  MACRO(one, "one")
  MACRO(notToken, "not")
  MACRO(test, "test")
  MACRO(tryToken, "try")
  MACRO(semicolon, ";")
  MACRO(pipe, "|")
  MACRO(plus, "+")
  MACRO(star, "*")
  MACRO(bang, "!")
  MACRO(query, "?")
  MACRO(matches, "<=?")
  MACRO(unifies, "=?")
  MACRO(assignment, "<-")
  MACRO(orelse, "or-else")
  MACRO(match, "match")
  MACRO(xmatch, "xmatch")
  MACRO(amatch, "amatch")
  MACRO(matchrew, "matchrew")
  MACRO(xmatchrew, "xmatchrew")
  MACRO(amatchrew, "amatchrew")
  MACRO(by, "by")
  MACRO(irreducible, "irreducible")
    //
    //	Special hook identifier tokens.
    //
  MACRO(bubble, "Bubble")
  MACRO(exclude, "Exclude")
  MACRO(qidSymbolToken, "qidSymbol")
  MACRO(nilQidListSymbolToken, "nilQidListSymbol")
  MACRO(qidListSymbolToken, "qidListSymbol")
    //
    //	Pseudo-tokens needed as place holders for special terminals.
    //
  MACRO(smallNat, "[ SMALL_NAT ]")
  MACRO(zero, "[ ZERO ]")
  MACRO(smallNeg, "[ SMALL_NEG ]")
  MACRO(rational, "[ RATIONAL ]")
  MACRO(floatToken, "[ FLOAT ]")
  MACRO(quotedIdentifier, "[ QUOTED_IDENTIFIER ]")
  MACRO(stringToken, "[ STRING ]")
  MACRO(endsInColon, "[ ENDS_IN_COLON ]")
