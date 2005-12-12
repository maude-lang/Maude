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

/*
//	Lexical analyser
*/
%{
//      utility stuff
#include "macros.hh"
#include "vector.hh"

//	forward declarations
#include "interface.hh"
#include "core.hh"
#include "higher.hh"
#include "mixfix.hh"

//	core class definitions
#include "lineNumber.hh"

//	front end class definitions
#include "token.hh"
#include "fileTable.hh"
#include "directoryManager.hh"
#include "preModule.hh"
#include "lexerAux.hh"
#include "main.hh"
#include "userLevelRewritingContext.hh"
#include "interpreter.hh"

//	bison generated token file
#include "surface.h"

#define YY_DECL int yylex(YYSTYPE* lvalp)

#define YY_INPUT(buf, result, max_size)		getInput(buf, result, max_size)

#define RETURN(token) \
  { lvalp->yyToken.tokenize(yytext, lineNumber); return (token); }

#define FIX_UP(token) \
  { lvalp->yyToken.fixUp(yytext, lineNumber); return (token); }

int braceCount;
int parenCount;
string latexCode;
%}

stringContent	([^[:cntrl:]"\\]|("\\"[^[:cntrl:]])|(\\\n))
string		("\""{stringContent}*"\"")
normal		([^[:cntrl:] ()\[\]{},`_"]|{string})
normalSeq	({normal}("`"{normal})*)
special		("_"|("`"[()\[\]{},]))
maudeId		(({special}|{normalSeq})+)

%s ID_MODE
%s CMD_MODE
%x FILE_NAME_MODE
%x STRING_MODE
%x LATEX_MODE
%option stack
%option noyywrap

%%
	if (UserLevelRewritingContext::interrupted())
	  yyterminate();

("***>"|"--->")[^ \t\n\r\f]*		{
					  yyless(0);
					  eatComment(false);
					}
("***"|"---")[^ \t\n\r\f]*		{
					  yyless(3);
					  eatComment(true);
					}

<INITIAL,ID_MODE>{
memo					RETURN(KW_MEMO)
format					RETURN(KW_FORMAT)
}

<INITIAL,CMD_MODE>{
in					RETURN(KW_IN)
}

<INITIAL>{
th|fth|mod|fmod|obj			RETURN(KW_MOD)  // need to know which one we saw
omod					RETURN(KW_OMOD)
view					RETURN(KW_VIEW);
load					return KW_LOAD;
pwd					return KW_PWD;
cd					return KW_CD;
pushd					return KW_PUSHD;
popd					return KW_POPD;
ls					return KW_LS;
quit|q					return KW_QUIT;
eof					return KW_EOF;
parse					return KW_PARSE;
norm|normalize				return KW_NORMALIZE;
red|reduce				return KW_REDUCE;
cred|creduce				return KW_CREDUCE;
rew|rewrite				return KW_REWRITE;
erew|erewrite				return KW_EREWRITE;
frew|frewrite				return KW_FREWRITE;
orew|orewrite				return KW_OREWRITE;
loop					return KW_LOOP;
cont|continue				return KW_CONTINUE;
nar|narrow				return KW_NARROW;
match					return KW_MATCH;
xmatch					return KW_XMATCH;
search					return KW_SEARCH;
unify					return KW_UNIFY;
xunify					return KW_XUNIFY;
set					return KW_SET;
show					return KW_SHOW;
on					return KW_ON;
off					return KW_OFF;
trace					return KW_TRACE;
advise|advisory|advisories		return KW_ADVISE;
verbose					return KW_VERBOSE;
do					return KW_DO;
clear					return KW_CLEAR;
body					return KW_BODY;
whole					return KW_WHOLE;
select					return KW_SELECT;
deselect				return KW_DESELECT;
conceal					return KW_CONCEAL;
reveal					return KW_REVEAL;
cond|condition				return KW_CONDITION;
subst|substitution			return KW_SUBSTITUTION;
print					return KW_PRINT;
color					return KW_COLOR;
graph					return KW_GRAPH;
mixfix					return KW_MIXFIX;
flat|flattened				return KW_FLAT;
with					return KW_WITH;
paren|parens|parentheses		return KW_PARENS;
alias|aliases				return KW_ALIASES;
gc					return KW_GC;
time					return KW_TIME;
stats					return KW_STATS;
timing					return KW_TIMING;
cmd|command				return KW_CMD;
all					return KW_ALL;
modules					return KW_MODULES;
module					return KW_MODULE;
views					return KW_VIEWS;
sort|sorts				return KW_SORTS;
op|ops					return KW_OPS;
var|vars				return KW_VARS;
mb|mbs					return KW_MBS;
eq|eqs					return KW_EQS;
rl|rls|rule|rules			return KW_RLS;
summary					return KW_SUMMARY;
kinds|components			return KW_KINDS;
compile|compiler			return KW_COMPILE;
count					return KW_COUNT;
protect					return KW_PROTECT;
extend					return KW_EXTEND;
include					return KW_INCLUDE;
exclude					return KW_EXCLUDE;
debug					return KW_DEBUG;
resume					return KW_RESUME;
abort					return KW_ABORT;
step					return KW_STEP;
where					return KW_WHERE;
dump					return KW_DUMP;
break					return KW_BREAK;
breakdown				return KW_BREAKDOWN;
path					return KW_PATH;
label|labels				return KW_LABEL;
profile					return KW_PROFILE;
number					return KW_NUMBER;
rat|rational				return KW_RAT;
[.\[\]()]				return *yytext;
0|([1-9][0-9]*)				{
					  bool dummy;
					  lvalp->yyInt64 = stringToInt64(yytext, dummy, 10);
					  return SIMPLE_NUMBER;
					}
{maudeId}|[{},]				{
					  IssueWarning(LineNumber(lineNumber) <<
					    ": skipped unexpected token: " <<
					    QUOTE(yytext));
					}
}

 /*
  *	In command mode we only recognize special tokens
  *	"in" "(" ")" "[" "]" ":" "." "," and non-negative numbers.
  *	Everything else is an identifier. Furthermore "." is only recognized
  *	at the end of a line or before a comment (ignoring white space).
  */
<CMD_MODE>{
"."(([ \t\r\f\v]*\n)|([ \t\r\f\v]+("***"|"---")))	{
                                          yyless(1);
                                          RETURN('.')
                                        }
[:,()\[\]]				RETURN(*yytext)
[1-9][0-9]*				RETURN(NUMERIC_ID)
{maudeId}|[.{},]			FIX_UP(IDENTIFIER)
}

 /*
  *	Id mode is used for scanning modules; a whole bunch of special tokens
  *	are recognized.
  */
<ID_MODE>{
to					RETURN(KW_TO)
from					RETURN(KW_FROM)
label					RETURN(KW_LABEL)
assoc|associative			RETURN(KW_ASSOC)
comm|commutative			RETURN(KW_COMM)
id:|identity:				RETURN(KW_ID)
idem|idempotent				RETURN(KW_IDEM)
iter|iterated				RETURN(KW_ITER)
left					RETURN(KW_LEFT)
right					RETURN(KW_RIGHT)
prec|precedence				RETURN(KW_PREC)
gather					RETURN(KW_GATHER)
metadata				RETURN(KW_METADATA)
strat|strategy				RETURN(KW_STRAT)
frozen					RETURN(KW_FROZEN)
poly|polymorphic			RETURN(KW_POLY)
ctor|constructor			RETURN(KW_CTOR)
latex					RETURN(KW_LATEX)
special					RETURN(KW_SPECIAL)
config|configuration			RETURN(KW_CONFIG)
obj|object				RETURN(KW_OBJ)
msg|message				RETURN(KW_MSG)
ditto					RETURN(KW_DITTO)
id-hook					RETURN(KW_ID_HOOK)
op-hook					RETURN(KW_OP_HOOK)
term-hook				RETURN(KW_TERM_HOOK)
is					RETURN(KW_IS)
if					RETURN(KW_IF)
pr|protecting|ex|extending|us|using|inc|including	RETURN(KW_IMPORT)
sort|sorts				RETURN(KW_SORT)
subsort|subsorts			RETURN(KW_SUBSORT)
class					RETURN(KW_CLASS)
subclass|subclasses			RETURN(KW_SUBCLASS)
op					RETURN(KW_OP)
ops					RETURN(KW_OPS)
msgs					RETURN(KW_MSGS)
var|vars				RETURN(KW_VAR)
mb					RETURN(KW_MB)
cmb					RETURN(KW_CMB)
eq					RETURN(KW_EQ)
ceq|cq					RETURN(KW_CEQ)
rl					RETURN(KW_RL)
crl					RETURN(KW_CRL)
end(th|fth|m|fm|om|o)|jbo		RETURN(KW_ENDM)
endv					RETURN(KW_ENDV)
"->"					RETURN(KW_ARROW)
"=>"					RETURN(KW_ARROW2)
"~>"					RETURN(KW_PARTIAL)
"::"					RETURN(KW_COLON2)
[:()\[\]{}.,<=|+*]			RETURN(*yytext)
{maudeId}"."				FIX_UP(ENDS_IN_DOT)
{maudeId}				FIX_UP(IDENTIFIER)
}

<FILE_NAME_MODE>{
	[ \t]*				// eat white space
	[^ \t\n\r\f]+			{
					  yy_pop_state();
					  lvalp->yyString = yytext;			  
					  return FILE_NAME_STRING;
					}
}

<STRING_MODE>{
	[^\n\r\f]*			{
					  yy_pop_state();
					  lvalp->yyString = yytext;			  
					  return UNINTERPRETED_STRING;
					}
	[\n\r\f]			{
					  yyless(0);  // so that line numbers will be correct
					  yy_pop_state();
					  lvalp->yyString = "";			  
					  return UNINTERPRETED_STRING;
					}
}

<LATEX_MODE>{
(\\[{}()]{0,1})|([a-zA-Z0-9.:;,?!`'\[\]\-/*@#$%&~_^+=|<> \t]+)	latexCode += yytext;
\(					{
					  if (braceCount == 0)
					    ++parenCount;
					  latexCode += yytext;
					}
\)					{
					  if (braceCount == 0)
					    {
					      --parenCount;
					      if (parenCount == 0)
					        {
						  yyless(0);
						  yy_pop_state();
						  lvalp->yyString = latexCode.c_str();
						  return LATEX_STRING;
						}
					    }
					  latexCode += yytext;
					}
\{					{
					  ++braceCount;
					  latexCode += yytext;
					}
\}					{
					  --braceCount;
					  latexCode += yytext;
					}
[\n\f]					{
			                  ++lineNumber;					
					  latexCode += yytext;
					}
.					yy_pop_state();  // mindless recovery
}

<<EOF>>					{
					  if (UserLevelRewritingContext::interrupted() ||
					      !handleEof())
					    yyterminate();
					}

[ \t\r]*				;
[\n\f]			                ++lineNumber;
.                                       {
					  IssueWarning(LineNumber(lineNumber) <<
					    ": skipped: " << QUOTE(yytext));
					}

%%

#include "lexerAux.cc"
