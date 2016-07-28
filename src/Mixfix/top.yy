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
//	Parser for Maude surface syntax.
//

%{
#include <string>
#include <stack>

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
#include "renaming.hh"
#include "view.hh"
#include "moduleExpression.hh"
#include "fileTable.hh"
#include "directoryManager.hh"
#include "syntacticPreModule.hh"
#include "visibleModule.hh"  // HACK
#include "userLevelRewritingContext.hh"
#include "interpreter.hh"

#include "global.hh"
#define clear()			tokenSequence.clear();
#define store(token)		tokenSequence.append(token)
#define fragClear()		fragments.contractTo(0);
#define fragStore(token)	fragments.append(token)
//#define YYPARSE_PARAM	parseResult
//#define PARSE_RESULT	(*((UserLevelRewritingContext::ParseResult*) parseResult))
#define PARSE_RESULT	(*parseResult)

#define CM		interpreter.getCurrentModule()
#define CV		interpreter.getCurrentView()

#include "lexerAux.hh"
/*
void lexerInitialMode();
void lexerIdMode();
void lexerCmdMode();
void lexerFileNameMode();
void lexerStringMode();
void lexerLatexMode();
bool handleEof();
bool includeFile(const string& directory, const string& fileName, bool silent, int lineNr);
//void eatComment(bool firstNonWhite);
*/
Vector<Token> singleton(1);
Vector<Token> tokenSequence;
Vector<Token> lexerBubble;
Vector<Token> fragments;
Vector<Token> moduleExpr;
Vector<Token> opDescription;
stack<ModuleExpression*> moduleExpressions;
Renaming* currentRenaming = 0;
SyntaxContainer* currentSyntaxContainer = 0;
SyntaxContainer* oldSyntaxContainer = 0;

Int64 number;
Int64 number2;

static void yyerror(UserLevelRewritingContext::ParseResult* parseResult, char *s);

void cleanUpModuleExpression();
void cleanUpParser();
void missingSpace(const Token& token);
%}
%pure-parser
%parse-param {UserLevelRewritingContext::ParseResult* parseResult}

%union
{
  bool yyBool;
  Int64 yyInt64;
  const char* yyString;
  Token yyToken;
  ImportModule::ImportMode yyImportMode;
  Interpreter::Flags yyFlags;
  Interpreter::PrintFlags yyPrintFlags;
  Interpreter::SearchKind yySearchKind;
}

%{
int yylex(YYSTYPE* lvalp);
%}

%token <yyString> FILE_NAME_STRING UNINTERPRETED_STRING LATEX_STRING
/*
 *	Inert keywords: these are only recognized by lexer when in initial mode.
 */
%token <yyToken> KW_MOD KW_OMOD KW_VIEW
%token KW_PARSE KW_NORMALIZE KW_REDUCE KW_REWRITE
%token KW_LOOP KW_NARROW KW_XG_NARROW KW_MATCH KW_XMATCH KW_UNIFY KW_CHECK
%token KW_GET KW_VARIANTS KW_VARIANT
%token KW_EREWRITE KW_FREWRITE KW_SREWRITE
%token KW_CONTINUE KW_SEARCH
%token KW_SET KW_SHOW KW_ON KW_OFF 
%token KW_TRACE KW_BODY KW_BUILTIN KW_WHOLE KW_SELECT KW_DESELECT KW_CONDITION KW_SUBSTITUTION
%token KW_PRINT KW_GRAPH KW_MIXFIX KW_FLAT KW_ATTRIBUTE KW_NEWLINE
%token KW_WITH KW_PARENS KW_ALIASES KW_GC KW_TIME KW_STATS KW_TIMING
%token KW_CMD KW_BREAKDOWN KW_BREAK KW_PATH
%token KW_MODULE KW_MODULES KW_VIEWS KW_ALL KW_SORTS KW_OPS2 KW_VARS
%token KW_MBS KW_EQS KW_RLS KW_SUMMARY KW_KINDS KW_ADVISE KW_VERBOSE
%token KW_DO KW_CLEAR
%token KW_PROTECT KW_EXTEND KW_INCLUDE KW_EXCLUDE
%token KW_CONCEAL KW_REVEAL KW_COMPILE KW_COUNT
%token KW_DEBUG KW_IRREDUNDANT KW_RESUME KW_ABORT KW_STEP KW_WHERE KW_CREDUCE KW_SREDUCE KW_DUMP KW_PROFILE
%token KW_NUMBER KW_RAT KW_COLOR
%token <yyInt64> SIMPLE_NUMBER
%token KW_PWD KW_CD KW_PUSHD KW_POPD KW_LS KW_LOAD KW_QUIT KW_EOF KW_TEST KW_SMT_SEARCH

/*
 *	Start keywords: signal end of mixfix statement if following '.'.
 */
%token <yyToken> KW_ENDM KW_IMPORT KW_ENDV
%token <yyToken> KW_SORT KW_SUBSORT KW_OP KW_OPS KW_MSGS KW_VAR KW_CLASS KW_SUBCLASS
%token <yyToken> KW_MB KW_CMB KW_EQ KW_CEQ KW_RL KW_CRL

/*
 *	Mid keywords: need to be recognized in the middle of mixfix syntax.
 */
%token <yyToken> KW_IS KW_FROM
%token <yyToken> KW_ARROW KW_ARROW2 KW_PARTIAL KW_IF
%type <yyToken> ':' '=' '(' ')' '.' '<' '[' ']' ',' '|'

/*
 *	Module expression keywords.
 */
%token <yyToken> KW_LABEL KW_TO KW_COLON2
%left <yyToken> '+'
%left <yyToken> '*'
%type <yyToken> '{' '}'

/*
 *	Attribute keywords need to be recognized when parsing attributes.
 */
%token <yyToken> KW_ASSOC KW_COMM KW_ID KW_IDEM KW_ITER
%token <yyToken> KW_LEFT KW_RIGHT KW_PREC KW_GATHER KW_METADATA KW_STRAT KW_POLY
%token <yyToken> KW_MEMO KW_FROZEN KW_CTOR KW_LATEX KW_SPECIAL KW_CONFIG KW_OBJ KW_MSG
%token <yyToken> KW_DITTO KW_FORMAT
%token <yyToken> KW_ID_HOOK KW_OP_HOOK KW_TERM_HOOK

/*
 *	Command keywords need to be recognized when parsing commands.
 */
%token <yyToken> KW_IN

/*
 *	Special tokens.
 */
%token <yyToken> IDENTIFIER NUMERIC_ID ENDS_IN_DOT

/*
 *	This is a dummy token that is never passed by the lexer but by
 *	giving this as an alternative we force the parser to lookahead
 *	one token and allow the lexer to grab a bubble.
 */
%token FORCE_LOOKAHEAD

/*
 *	Nonterminals that return tokens.
 */
%type <yyToken> identifier inert startKeyword startKeyword2 midKeyword attrKeyword attrKeyword2
%type <yyToken> token endsInDot badType
%type <yyToken> tokenBarColon
%type <yyToken> tokenBarDot
%type <yyToken> cTokenBarIn
%type <yyToken> cTokenBarLeftIn cTokenBarDotNumber cTokenBarDotRight
%type <yyToken> cSimpleTokenBarDot
%type <yyToken> cTokenBarDotCommaRight cTokenBarDotCommaNumber
%type <yyToken> sortName sortToken startModule sortDot

/*
 *	Nonterminals that return Interpreter::SearchKind.
 */
%type <yySearchKind> search
/*
 *	Nonterminals that return bool.
 */
%type <yyBool> polarity select match optDebug optIrredundant conceal exclude arrow
/*
 *	Nonterminals that return int.
 */
%type <yyInt64> optNumber
/*
 *	Nonterminals that return ImportMode.
 */
%type <yyImportMode> importMode
/*
 *	Nonterminals that return Flags.
 */
%type <yyFlags> traceOption
/*
 *	Nonterminals that return PrintFlags.
 */
%type <yyPrintFlags> printOption

%start top

%%

top		:	item { YYACCEPT; }
		|
			{
			  PARSE_RESULT = UserLevelRewritingContext::QUIT;
			}
		;

item		:	module
		|	view
		|	directive
		|	command
		;

/*
 *	Directives.
 */
directive	:	KW_IN		{ lexerFileNameMode(); }
			FILE_NAME_STRING
			{
			  int lineNr = lineNumber;
			  //eatComment(false); // eat \n so that line number is correct
			  string directory;
			  string fileName;
			  if (findFile($3, directory, fileName, lineNr))
			    includeFile(directory, fileName, false, lineNr);
			}
		|	KW_LOAD		{ lexerFileNameMode(); }
			FILE_NAME_STRING
			{
			  int lineNr = lineNumber;
			  //eatComment(false); // eat \n so that line number is correct
			  string directory;
			  string fileName;
			  if (findFile($3, directory, fileName, lineNr))
			    includeFile(directory, fileName, true, lineNr);
			}
		|	KW_PWD
			{
			  cout << directoryManager.getCwd() << '\n';
			}
		|	KW_CD		{ lexerFileNameMode(); }
			FILE_NAME_STRING
			{
			  string directory;
			  directoryManager.realPath($3, directory);
			  if (!directoryManager.cd(directory))
			    {
			      IssueWarning(LineNumber(lineNumber) <<
					   ": cd failed");
			    }
			}
		|	KW_PUSHD	{ lexerFileNameMode(); }
			FILE_NAME_STRING
			{
			  string directory;
			  directoryManager.realPath($3, directory);
			  if (directoryManager.pushd(directory) == UNDEFINED)
			    {
			      IssueWarning(LineNumber(lineNumber) <<
					   ": pushd failed");
			    }
			}
		|	KW_POPD
			{
			  const char* path = directoryManager.popd();
			  if (path != 0)
			    cout << path << '\n';
			  else
			    {
			      IssueWarning(LineNumber(lineNumber) <<
					   ": popd failed");
			    }
			}
		|	KW_LS		{ lexerStringMode(); }
			UNINTERPRETED_STRING
			{
			  system((string("ls") + $3).c_str());
			}
		|	KW_QUIT
			{
			  PARSE_RESULT = UserLevelRewritingContext::QUIT;
			  YYACCEPT;
			}
		|	KW_EOF
			{
			  if(!handleEof())
			    {
			      PARSE_RESULT = UserLevelRewritingContext::QUIT;
			    }
			}
		;
