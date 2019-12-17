/*

    This file is part of the Maude 2 interpreter.

    Copyright 2017 SRI International, Menlo Park, CA 94025, USA.

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

/*
//	Tokenizer for tokenize() operator.
*/
%{
//      utility stuff
#include "macros.hh"
#include "vector.hh"
#include "rope.hh"

//	front end class definitions
#include "token.hh"

void
getInputFromRope(char* buf, yy_size_t& result, yy_size_t max_size);

//
//	We return a pointer rather than a reference to avoid an issue with
//	yyterminate()/YY_NULL, though we ensure that yyterminate() never
//	gets called because we always pass back an EOT charater from YY_INPUT
//	to avoid flex EOF handling.
//
#define YY_DECL const Vector<int>* tokenizeRope(const Rope& argumentRope)

//
//	We use safeResult to ensure a reference yy_size_t& can be passed to outlined code.
//
#define YY_INPUT(buf, result, max_size) \
  { yy_size_t safeResult; getInputFromRope(buf, safeResult, max_size); result = safeResult; }

Vector<int> tokenList;
Rope inputRope;
Rope::const_iterator current;
bool endOfRope;
%}

%option noyywrap

stringContent	([^[:cntrl:]"\\]|("\\"[^[:cntrl:]])|(\\\n))
string		("\""{stringContent}*"\"")
normal		([^[:cntrl:] ()\[\]{},`_"]|{string})
normalSeq	({normal}("`"{normal})*)
special		("_"|("`"[()\[\]{},]))
maudeId		(({special}|{normalSeq})+)


%%
	tokenList.clear();
	inputRope = argumentRope;
	current = inputRope.begin();
	endOfRope = false;

{maudeId}|[()\[\]{},]				tokenList.append(Token::fixUp(yytext));
\004						{
						  if (endOfRope)
						    return &tokenList;
						}
[^\004]						;

%%

void
getInputFromRope(char* buf, yy_size_t& result, yy_size_t max_size)
{
  Rope::size_type available = inputRope.end() - current;
  if (available == 0)
    {
      endOfRope = true;
      result = 1;
      buf[0] = '\004';  // EOT character
    }
  else
    {
      result = (available > max_size) ? max_size : available;
      for (Rope::size_type i = 0; i != result; ++i, ++current)
        buf[i] = *current;
    }
}
