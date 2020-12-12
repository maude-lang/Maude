/*

    This file is part of the Maude 2 interpreter.

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

/*
//	Deserializer for dags.
*/
%{
//      utility stuff
#include "macros.hh"
#include "vector.hh"
#include "rope.hh"

#include "interface.hh"
#include "core.hh"
#include "mixfix.hh"
#include "S_Theory.hh"
#include "strategyLanguage.hh"

#include "S_Symbol.hh"
#include "S_DagNode.hh"

//	front end class definitions
#include "token.hh"
#include "mixfixModule.hh"


static void
getInputFromRope(char* buf, yy_size_t& result, yy_size_t max_size);
static DagNode* makeDagNode(MixfixModule* m);
static DagNode* makeVariableDagNode(MixfixModule* m, int code);
static DagNode* makeQuotedIdentifierDagNode(MixfixModule* m, int code);
static DagNode* makeStringDagNode(MixfixModule* m, const char* quotedString);
static DagNode* makeFloatDagNode(MixfixModule* m, const char* bitsAsInt64);
static DagNode* makeSMT_NumberDagNode(MixfixModule* m, const char* stringRep);

//
//	We return a pointer rather than a reference to avoid an issue with
//	yyterminate()/YY_NULL, though we ensure that yyterminate() never
//	gets called because we always pass back an EOT charater from YY_INPUT
//	to avoid flex EOF handling.
//
#define YY_DECL DagNode* deserializeRope(MixfixModule* m, const Rope& argumentRope)

//
//	We use safeResult to ensure a reference yy_size_t& can be passed to outlined code.
//
#define YY_INPUT(buf, result, max_size) \
  { yy_size_t safeResult; getInputFromRope(buf, safeResult, max_size); result = safeResult; }

static Vector<DagNode*> argList;
static int savedIndex;
static int savedKindIndex;
static mpz_class savedBigNum;
static Vector<DagNode*> history;
static Rope inputRope;
static Rope::const_iterator current;
static bool endOfRope;

enum OpType
  {
    REGULAR,
    EAGER_SORT_TEST,
    LAZY_SORT_TEST,
    POLYMORPH
  };

OpType opType;
%}


%option noyywrap

posInt		([1-9][0-9]*)
natNumber	(0|{posInt})
anyInt		({natNumber}|(-{posInt}))
stringContent	([^[:cntrl:]"\\]|("\\"[^[:cntrl:]])|(\\\n))
string		("\""{stringContent}*"\"")
normal		([^[:cntrl:] ()\[\]{},`_"]|{string})
normalSeq	({normal}("`"{normal})*)
special		("_"|("`"[()\[\]{},]))
maudeId		(({special}|{normalSeq})+)
posRat		({posInt}\/{posInt})
ratNumber	({anyInt}|{posRat}|(-{posRat}))

%s SEEN_NUMBER_MODE
%s ARG_LIST_MODE

%%
	argList.clear();
	history.clear();
	inputRope = argumentRope;
	current = inputRope.begin();
	endOfRope = false;


<INITIAL>{
{natNumber}					{
						  savedIndex = atoi(yytext);
						  opType = REGULAR;
						  BEGIN(SEEN_NUMBER_MODE);
						}
\004						{
						  if (endOfRope)
						    return history[history.size() - 1];
						}
\n						;
[^\004]						CantHappen(((int) *yytext) << " in init");
}

<SEEN_NUMBER_MODE>{
\(						{
						  BEGIN(ARG_LIST_MODE);
						}
\n						{
						  DagNode* d = makeDagNode(m);
						  history.append(d);
						  BEGIN(INITIAL);
						}
\^{posInt}					{
						  savedBigNum = yytext + 1;
						}
{string}					{
						  history.append(makeStringDagNode(m, yytext));
						  BEGIN(INITIAL);
						}
v{maudeId}					{
						  int code = Token::encode(yytext + 1);
						  history.append(makeVariableDagNode(m, code));
						  BEGIN(INITIAL);
						}
\'|\'{maudeId}					{
						  int code = Token::encode(yytext + 1);
						  history.append(makeQuotedIdentifierDagNode(m, code));
						  BEGIN(INITIAL);
						}
f{anyInt}					{
						  history.append(makeFloatDagNode(m, yytext + 1));
						  BEGIN(INITIAL);
						}
s						{
						  opType = EAGER_SORT_TEST;
						}
l						{
						  opType = LAZY_SORT_TEST;
						}
p{natNumber}					{
						  savedKindIndex = atoi(yytext + 1);
						  opType = POLYMORPH;
						}
q{ratNumber}					{
						  history.append(makeSMT_NumberDagNode(m, yytext + 1));
						  BEGIN(INITIAL);
						}
.						CantHappen(((int) *yytext) << " in num");
}

<ARG_LIST_MODE>{
{natNumber}					{
						  int historyIndex = atoi(yytext);
						  argList.append(history[historyIndex]);
						}
,						;
\)\n						{
						  history.append(makeDagNode(m));
						  argList.clear();
						  BEGIN(INITIAL);
						}
.						CantHappen(((int) *yytext) << " in arg");
	
}

%%

#include "deserializerAux.cc"
