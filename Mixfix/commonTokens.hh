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
//      Class to hold token codes.
//
#ifndef _commonTokens_hh_
#define _commonTokens_hh_

class CommonTokens
{
public:
  CommonTokens();

protected:
  static int leftParen;
  static int rightParen;
  static int leftBracket;
  static int rightBracket;
  static int leftBrace;
  static int rightBrace;
  static int comma;
  static int underscore;
  static int dot;
  static int equals;
  static int assign;
  static int arrow;
  static int arrowOne;
  static int arrowPlus;
  static int arrowStar;
  static int arrowBang;
  static int suchThat;
  static int such;
  static int that;
  static int mb;
  static int cmb;
  static int eq;
  static int ceq;
  static int rl;
  static int crl;
  static int cq;
  static int ifToken;
  static int colon;
  static int colon2;
  static int colon3;
  static int wedge;
  static int label;
  static int metadata;
  static int nonexec;
  static int otherwise;
  static int owise;
};

#endif
