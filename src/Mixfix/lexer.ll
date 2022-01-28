
/*

    This file is part of the Maude 3 interpreter.

    Copyright 1997-2021 SRI International, Menlo Park, CA 94025, USA.

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
#include "strategyLanguage.hh"
#include "mixfix.hh"

//	core class definitions
#include "lineNumber.hh"

//	front end class definitions
#include "token.hh"
#include "fileTable.hh"
#include "directoryManager.hh"
//#include "preModule.hh"
#include "lexerAux.hh"
#include "global.hh"
#include "userLevelRewritingContext.hh"
#include "interpreter.hh"

//	bison generated token file
#include "surface.h"

#define YY_DECL int yylex(YYSTYPE* lvalp)

void getInput(char* buf, yy_size_t& result, yy_size_t max_size);
//
//	result might be an int or a yy_size_t depending on flex version, so we use a temporary.
//
#define YY_INPUT(buf, result, max_size) \
  { yy_size_t safeResult; getInput(buf, safeResult, max_size); result = safeResult; }

#define RETURN(token) \
  { lvalp->yyToken.tokenize(yytext, lineNumber); return (token); }

#define RETURN_FIX_UP(token) \
  { lvalp->yyToken.fixUp(yytext, lineNumber); return (token); }

#define SAVE(token) \
  { savedToken.tokenize(yytext, lineNumber); savedReturn = token; }

#define SAVE_FIX_UP(token) \
  { savedToken.fixUp(yytext, lineNumber); savedReturn = token; }

#define RETURN_SAVED(token) \
  { lvalp->yyToken = savedToken; return (token); }

#define STORE \
  { Token t; t.tokenize(yytext, lineNumber); lexerBubble.append(t); /*DebugAdvisory("Stored " << t);*/ }
#define STORE_FIX_UP \
  { Token t; t.fixUp(yytext, lineNumber); lexerBubble.append(t); /*DebugAdvisory("Stored fixUp " << t);*/ }

#define EXIT(token) \
  { yy_pop_state(); RETURN(token) }

Token savedToken;
int savedReturn;
int braceCount;
int parenCount;
int terminationSet;
string accumulator;
string fileName;

//int terminationCondition;
int minLength;
extern Vector<Token> lexerBubble;
%}

stringContent	([^[:cntrl:]"\\]|("\\"[^[:cntrl:]])|(\\\n)|\t)
string		("\""{stringContent}*"\"")
normal		([^[:cntrl:] ()\[\]{},`_"]|{string})
normalSeq	({normal}("`"{normal})*)
special		("_"|("`"[()\[\]{},]))
maudeId		(({special}|{normalSeq})+)

%s ID_MODE
%s CMD_MODE
%s SEEN_DOT
%s BUBBLE_MODE
%s END_STATEMENT_MODE
%s END_COMMAND_MODE
%x FILE_NAME_MODE
%x FILE_NAME_QUOTE_MODE
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
th|fth|sth|oth|mod|fmod|smod|omod|obj	RETURN(KW_MOD)  // need to know which one we saw
oo					RETURN(KW_OO)
view					RETURN(KW_VIEW);
load					return KW_LOAD;
sload					return KW_SLOAD;
pwd					return KW_PWD;
cd					return KW_CD;
pushd					return KW_PUSHD;
popd					return KW_POPD;
ls					return KW_LS;
ll					return KW_LL;
quit|q					return KW_QUIT;
eof					return KW_EOF;
parse					return KW_PARSE;
norm|normalize				return KW_NORMALIZE;
red|reduce				return KW_REDUCE;
cred|creduce				return KW_CREDUCE;
sred|sreduce				return KW_SREDUCE;
rew|rewrite				return KW_REWRITE;
erew|erewrite				return KW_EREWRITE;
frew|frewrite				return KW_FREWRITE;
srew|srewrite				return KW_SREWRITE;
dsrew|dsrewrite				return KW_DSREWRITE;
check					return KW_CHECK;
loop					return KW_LOOP;
cont|continue				return KW_CONTINUE;
nar|narrow				return KW_NARROW;
xg-narrow				return KW_XG_NARROW;
get					return KW_GET;
variants				return KW_VARIANTS;
variant					return KW_VARIANT;
match					return KW_MATCH;
xmatch					return KW_XMATCH;
search					return KW_SEARCH;
unify					return KW_UNIFY;
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
builtin					return KW_BUILTIN;
whole					return KW_WHOLE;
select					return KW_SELECT;
deselect				return KW_DESELECT;
conceal					return KW_CONCEAL;
reveal					return KW_REVEAL;
cond|condition				return KW_CONDITION;
subst|substitution			return KW_SUBSTITUTION;
print					return KW_PRINT;
attr|attribute				return KW_ATTRIBUTE;
newline					return KW_NEWLINE;
color					return KW_COLOR;
graph					return KW_GRAPH;
mixfix					return KW_MIXFIX;
flat|flattened				return KW_FLAT;
with					return KW_WITH;
paren|parens|parentheses		return KW_PARENS;
alias|aliases				return KW_ALIASES;
const|constant|constants		return KW_CONST;
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
op|ops					return KW_OPS2;
var|vars				return KW_VARS;
mb|mbs					return KW_MBS;
eq|eqs					return KW_EQS;
rl|rls|rule|rules			return KW_RLS;
strat|strats				return KW_STRATS;
sd|sds					return KW_SDS;
summary					return KW_SUMMARY;
kinds|components			return KW_KINDS;
compile|compiler			return KW_COMPILE;
count					return KW_COUNT;
protect					return KW_PROTECT;
extend					return KW_EXTEND;
include					return KW_INCLUDE;
exclude					return KW_EXCLUDE;
debug					return KW_DEBUG;
irredundant|irred			return KW_IRREDUNDANT;
filtered				return KW_FILTERED;
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
test					return KW_TEST;
smt-search				return KW_SMT_SEARCH;
vu-narrow				return KW_VU_NARROW;
fvu-narrow				return KW_FVU_NARROW;
fold					return KW_FOLD;
desugared				return KW_DESUGARED;
[.\[\](){}]				return *yytext;
0|([1-9][0-9]*)				{
					  bool dummy;
					  lvalp->yyInt64 = stringToInt64(yytext, dummy, 10);
					  return SIMPLE_NUMBER;
					}
{maudeId}|,				{
					  IssueWarning(LineNumber(lineNumber) <<
					    ": skipped unexpected token: " <<
					    QUOTE(yytext));
					}
\n\n					{
					  ++lineNumber;
					  yyless(1);  // only absorb one newline
					  if (generateImpliedStep())
					    return KW_IMPLIED_STEP;
					}
}

 /*
  *	In command mode we only recognize special tokens
  *	"in" "{" "}" "(" ")" "[" "]" ":" "." "," and non-negative numbers.
  *	Everything else is an identifier. Furthermore "." is only recognized
  *	at the end of a line or before a comment (ignoring white space).
  */
<CMD_MODE>{
"."(([ \t\r\f\v]*\n)|([ \t\r\f\v]+("***"|"---")))	{
                                          yyless(1);
                                          RETURN('.')
                                        }
[:,()\[\]{}]				RETURN(*yytext)
filter					RETURN(KW_FILTER)
delay					RETURN(KW_DELAY)
[1-9][0-9]*				RETURN(NUMERIC_ID)
[.{}]					RETURN(IDENTIFIER)
{maudeId}"."				{
					  SAVE_FIX_UP(ENDS_IN_DOT)
					  BEGIN(SEEN_DOT);
					}
{maudeId}				RETURN_FIX_UP(IDENTIFIER)
}

<SEEN_DOT>{
(([ \t\r\f\v]*\n)|([ \t\r\f\v]+("***"|"---")))	{
					  yyless(0);
					  BEGIN(CMD_MODE);
					  RETURN_SAVED(savedReturn)
					}
.					{
					  yyless(0);
					  BEGIN(CMD_MODE);
					  RETURN_SAVED(IDENTIFIER)
					}
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
strat					RETURN(KW_STRAT)		// both strategy attribute and declaration
strategy				RETURN(KW_ASTRAT)		// strategy attribute only
strats					RETURN(KW_DSTRAT)		// declaration of a strategy only
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
sd					RETURN(KW_SD)
csd					RETURN(KW_CSD)
end(th|fth|sth|m|fm|sm|om|o|oth)|jbo	RETURN(KW_ENDM)
endv					RETURN(KW_ENDV)
"->"					RETURN(KW_ARROW)
"=>"					RETURN(KW_ARROW2)
"~>"					RETURN(KW_PARTIAL)
"::"					RETURN(KW_COLON2)
":="					RETURN(KW_ASSIGN)
[:()\[\]{}.,<=|+*@]			RETURN(*yytext)
{maudeId}"."				RETURN_FIX_UP(ENDS_IN_DOT)
{maudeId}				RETURN_FIX_UP(IDENTIFIER)
}

 /*
  *	Bubble mode squirrels tokens away in lexerBubble until some termination criteria is met.
  */
<BUBBLE_MODE>{
:					{
					  if (parenCount == 0 && (terminationSet & BAR_COLON) && lexerBubble.length() >= minLength)
					    EXIT(*yytext)
					  else
					    STORE
					}
,					{
					  if (parenCount == 0 && (terminationSet & BAR_COMMA) && lexerBubble.length() >= minLength)
					    EXIT(*yytext)
					  else
					    STORE
					}
\[					{
					  if (parenCount == 0 && (terminationSet & BAR_LEFT_BRACKET) && lexerBubble.length() >= minLength)
					    EXIT(*yytext)
					  else
					    STORE
					}
\]					{
					  if (parenCount == 0 && (terminationSet & BAR_RIGHT_BRACKET) && lexerBubble.length() >= minLength)
					    EXIT(*yytext)
					  else
					    STORE
					}
=					{
					  if (parenCount == 0 && (terminationSet & BAR_EQUALS) && lexerBubble.length() >= minLength)
					    EXIT(*yytext)
					  else
					    STORE
					}
=>					{
					  if (parenCount == 0 && (terminationSet & BAR_ARROW2) && lexerBubble.length() >= minLength)
					    EXIT(KW_ARROW2)
					  else
					    STORE
					}
:=					{
					  if (parenCount == 0 && (terminationSet & BAR_ASSIGN) && lexerBubble.length() >= minLength)
					    EXIT(KW_ASSIGN)
					  else
					    STORE
					}
to					{
					  if (parenCount == 0 && (terminationSet & BAR_TO) && lexerBubble.length() >= minLength)
					    EXIT(KW_TO)
					  else
					    STORE
					}
if					{
					  if (parenCount == 0 && (terminationSet & BAR_IF) && lexerBubble.length() >= minLength)
					    EXIT(KW_IF)
					  else
					    STORE
					}
assoc|associative|comm|commutative|id:|identity:|idem|idempotent|iter|iterated|left|right|prec|precedence|gather|metadata|strat|strategy|frozen|poly|polymorphic|ctor|constructor|latex|special|config|configuration|obj|object|msg|message|ditto|format|memo	{
					  if (parenCount == 0 && (terminationSet & BAR_OP_ATTRIBUTE) && lexerBubble.length() >= minLength)
					    {
					      yyless(0);  // need to re-lex it to get the correct return value
					      yy_pop_state();
					    }
					  else
					    STORE
					}
\(					{
					  ++parenCount;
					  STORE
					}
\)					{
					  if (parenCount == 0)
					    {
					      if ((terminationSet & BAR_RIGHT_PAREN) && lexerBubble.length() >= minLength)
					        EXIT(*yytext)
					      IssueWarning(LineNumber(lineNumber) << ": mismatched parentheses.");
					    }
					  else
					    --parenCount;
					  STORE
					}
[\]{}]					STORE
\.					{
					  if (parenCount == 0 &&
					      lexerBubble.length() >= minLength &&
					      (terminationSet & (END_STATEMENT | END_COMMAND)))
					    {
					      SAVE(*yytext)
					      BEGIN((terminationSet & END_STATEMENT) ? END_STATEMENT_MODE : END_COMMAND_MODE);
					    }
					  else
					    STORE
					}
{maudeId}"."				{
					  if (parenCount == 0 &&
					      lexerBubble.length() + 1 >= minLength  &&
					      (terminationSet & (END_STATEMENT | END_COMMAND)))
					    {
					      SAVE_FIX_UP(ENDS_IN_DOT)
					      BEGIN((terminationSet & END_STATEMENT) ? END_STATEMENT_MODE : END_COMMAND_MODE);
					    }
					  else
					    STORE_FIX_UP
					}
{maudeId}				STORE_FIX_UP
}

 /*
  *	We have saved something that looks like a statement terminating period. We now lex the next
  *	token to see if it ends a module or starts a new statement, and if so push the lexed token back
  *	on to the input stream to be re-lexed in a new mode.
  */
<END_STATEMENT_MODE>{
pr|protecting|ex|extending|us|using|inc|including|sort|sorts|subsort|subsorts|op|ops|var|vars|mb|cmb|eq|cq|ceq|rl|crl|sd|csd|strat|strats|end(th|fth|sth|m|fm|sm|om|o|v|sv)|jbo|msg|msgs|class|classes|subclass|subclasses		{
					  yyless(0);  // BUG - need to deal with white space and comments after the .
					  yy_pop_state();
					  RETURN_SAVED(savedReturn)
					}
}

<END_COMMAND_MODE>{
(([ \t\r\f\v]*\n)|([ \t\r\f\v]+("***"|"---")))	{
					  yyless(0);
					  yy_pop_state();
					  RETURN_SAVED(savedReturn)
					}
}

<END_STATEMENT_MODE,END_COMMAND_MODE>{
\.					{
					  lexerBubble.append(savedToken);
					  SAVE(*yytext);
					}
{maudeId}"."				{
					  lexerBubble.append(savedToken);
					  SAVE_FIX_UP(ENDS_IN_DOT)
					}
{maudeId}|[^ \n\r\f\t\v]		{
	     				  //
					  //	We need {maudeId} here so that an identifier such
					  //	as .foo doesn't get split by the first rule.
					  //
					  lexerBubble.append(savedToken);
					  yyless(0);
					  BEGIN(BUBBLE_MODE);
					}
}

<FILE_NAME_MODE>{
	\"				{
					  if (accumulator.empty())
					    BEGIN(FILE_NAME_QUOTE_MODE);
					  else
					    accumulator += '"';
					}
	\\" "				accumulator += ' ';
	\\\"				accumulator += '"';
	\\\\				accumulator += '\\';
	\\				accumulator += '\\';
	[^"\\ \t\n\r\f]+		accumulator += yytext;
	[ \t\r]				{
					  if (!accumulator.empty())
					    {
					      yy_pop_state();
					      lvalp->yyString = accumulator.c_str();
					      eatComment(false);
					      return FILE_NAME_STRING;
					    }
					}
	[\n\f]				{
			                  ++lineNumber;					
					  yy_pop_state();
					  lvalp->yyString = accumulator.c_str();
					  return FILE_NAME_STRING;
					}
}

<FILE_NAME_QUOTE_MODE>{
	["\t\r]				{
					   yy_pop_state();
					   lvalp->yyString = accumulator.c_str();
					   eatComment(false);
					   return FILE_NAME_STRING;
					}
	[^"\t\n\r\f]+			accumulator += yytext;
	[\n\f]				{
			                  ++lineNumber;					
					  yy_pop_state();
					  lvalp->yyString = accumulator.c_str();
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
(\\[{}()]{0,1})|([a-zA-Z0-9.:;,?!`'\[\]\-/*@#$%&~_^+=|<> \t]+)	accumulator += yytext;
\(					{
					  if (braceCount == 0)
					    ++parenCount;
					  accumulator += yytext;
					}
\)					{
					  if (braceCount == 0)
					    {
					      --parenCount;
					      if (parenCount == 0)
					        {
						  yyless(0);
						  yy_pop_state();
						  lvalp->yyString = accumulator.c_str();
						  return LATEX_STRING;
						}
					    }
					  accumulator += yytext;
					}
\{					{
					  ++braceCount;
					  accumulator += yytext;
					}
\}					{
					  --braceCount;
					  accumulator += yytext;
					}
[\n\f]					{
			                  ++lineNumber;					
					  accumulator += yytext;
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
