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
//      Implementation for class MixfixModule.
//

//      utility stuff
#include "macros.hh"
#include "vector.hh"

//	front end class definitions
#include "token.hh"
#include "commonTokens.hh"

int CommonTokens::leftParen;
int CommonTokens::rightParen;
int CommonTokens::leftBracket;
int CommonTokens::rightBracket;
int CommonTokens::leftBrace;
int CommonTokens::rightBrace;
int CommonTokens::comma;
int CommonTokens::underscore;
int CommonTokens::dot;
int CommonTokens::equals;
int CommonTokens::assign;
int CommonTokens::arrow;
int CommonTokens::arrowOne;
int CommonTokens::arrowPlus;
int CommonTokens::arrowStar;
int CommonTokens::arrowBang;
int CommonTokens::suchThat;
int CommonTokens::such;
int CommonTokens::that;
int CommonTokens::mb;
int CommonTokens::cmb;
int CommonTokens::eq;
int CommonTokens::ceq;
int CommonTokens::rl;
int CommonTokens::crl;
int CommonTokens::cq;
int CommonTokens::ifToken;
int CommonTokens::colon;
int CommonTokens::colon2;
int CommonTokens::colon3;
int CommonTokens::wedge;
int CommonTokens::label;
int CommonTokens::metadata;
int CommonTokens::nonexec;
int CommonTokens::otherwise;
int CommonTokens::owise;

CommonTokens::CommonTokens()
{
  if (leftParen == 0)
    {
      leftParen = Token::encode("(");
      rightParen = Token::encode(")");
      leftBracket = Token::encode("[");
      rightBracket = Token::encode("]");
      leftBrace = Token::encode("{");
      rightBrace = Token::encode("}");
      comma = Token::encode(",");
      underscore = Token::encode("_");
      dot = Token::encode(".");
      equals = Token::encode("=");
      assign = Token::encode(":=");
      arrow = Token::encode("=>");
      arrowOne = Token::encode("=>1");
      arrowPlus = Token::encode("=>+");
      arrowStar = Token::encode("=>*");
      arrowBang = Token::encode("=>!");
      suchThat = Token::encode("s.t.");
      such = Token::encode("such");
      that = Token::encode("that");
      mb = Token::encode("mb");
      cmb = Token::encode("cmb");
      eq = Token::encode("eq");
      ceq = Token::encode("ceq");
      rl = Token::encode("rl");
      crl = Token::encode("crl");
      cq = Token::encode("cq");
      ifToken = Token::encode("if");
      colon = Token::encode(":");
      colon2 = Token::encode("::");
      colon3 = Token::encode(":::");
      wedge = Token::encode("/\\");
      label = Token::encode("label");
      metadata = Token::encode("metadata");
      nonexec = Token::encode("nonexec");
      otherwise = Token::encode("otherwise");
      owise = Token::encode("owise");
    }
}
