/*

    This file is part of the Maude 3 interpreter.

    Copyright 1997-2024 SRI International, Menlo Park, CA 94025, USA.

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
 *	Commands.
 */
command		:	KW_SELECT		{ lexBubble(END_COMMAND, 1); }
			endBubble
			{
			  interpreter.setCurrentModule(lexerBubble);
			}
		|	KW_DUMP			{ lexBubble(END_COMMAND, 1); }
			endBubble
			{
			  if (interpreter.setCurrentModule(lexerBubble))
			    CM->dump();
			}
		|	KW_PARSE
			{
			  lexerCmdMode();
			  moduleExpr.clear();
			}
			moduleAndTerm
			{
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.parse(lexerBubble);
			}

		|	KW_CREDUCE
			{
			  lexerCmdMode();
			  moduleExpr.clear();
			}
			moduleAndTerm
			{
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.creduce(lexerBubble);
			}

		|	KW_SREDUCE
			{
			  lexerCmdMode();
			  moduleExpr.clear();
			}
			moduleAndTerm
			{
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.sreduce(lexerBubble);
			}

		|	optDebug KW_REDUCE
			{
			  lexerCmdMode();
			  moduleExpr.clear();
			}
			moduleAndTerm
			{
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.reduce(lexerBubble, $1);
			}

		|	optDebug KW_REWRITE
			{
			  lexerCmdMode();
			  moduleExpr.clear();
			  number = NONE;
			}
			numberModuleTerm
			{
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.rewrite(lexerBubble, number, $1);
			}
		|	optDebug KW_EREWRITE
			{
			  lexerCmdMode();
			  moduleExpr.clear();
			  number = NONE;
			  number2 = NONE;
			}
			numbersModuleTerm
			{
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.eRewrite(lexerBubble, number, number2, $1);
			}
		|	optDebug KW_FREWRITE
			{
			  lexerCmdMode();
			  moduleExpr.clear();
			  number = NONE;
			  number2 = NONE;
			}
			numbersModuleTerm
			{
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.fRewrite(lexerBubble, number, number2, $1);
			}
		|	optDebug KW_SREWRITE
			{
			  lexerCmdMode();
			  moduleExpr.clear();
			  number = NONE;
			}
			numberModuleTerm
			{
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.sRewrite(lexerBubble, number, $1);
			}
		|	optDebug KW_DSREWRITE
			{
			  lexerCmdMode();
			  moduleExpr.clear();
			  number = NONE;
			}
			numberModuleTerm
			{
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.sRewrite(lexerBubble, number, $1, true);
			}

		|	KW_CHECK
			{
			  lexerCmdMode();
			  moduleExpr.clear();
			}
			moduleAndTerm
			{
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.check(lexerBubble);
			}

		|	optDebug search
			{
			  lexerCmdMode();
			  moduleExpr.clear();
			  number = NONE;
			  number2 = NONE;
			}
			numbersModuleTerm
			{
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.search(lexerBubble, number, number2, $2, $1);
			}
		|	optDebug optOptions KW_VU_NARROW
			{
			  variantOptions = $2;
			  lexerCmdMode();
			  moduleExpr.clear();
			  number = NONE;
			  number2 = NONE;			  
			}
			optionsNumbersModuleTerm
			{
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.search(lexerBubble, number, number2, Interpreter::VU_NARROW, $1, variantOptions);
			}
		|	match
			{
			  lexerCmdMode();
			  moduleExpr.clear();
			  number = NONE;
			}
			numberModuleTerm
			{
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.match(lexerBubble, $1, number);
			}
		|	optIrredundant KW_UNIFY
			{
			  lexerCmdMode();
			  moduleExpr.clear();
			  number = NONE;
			}
			numberModuleTerm
			{
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.unify(lexerBubble, number, $1);
			}
		|	optDebug KW_VARIANT KW_UNIFY
			{
			  lexerCmdMode();
			  moduleExpr.clear();
			  number = NONE;
			}
			numberModuleTerm
			{
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.variantUnify(lexerBubble, number, false, $1);
			}
		|	optDebug KW_FILTERED KW_VARIANT KW_UNIFY
			{
			  lexerCmdMode();
			  moduleExpr.clear();
			  number = NONE;
			}
			numberModuleTerm
			{
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.variantUnify(lexerBubble, number, true, $1);
			}
		|	optDebug KW_VARIANT KW_MATCH
			{
			  lexerCmdMode();
			  moduleExpr.clear();
			  number = NONE;
			}
			numberModuleTerm
			{
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.variantMatch(lexerBubble, number, $1);
			}

		|	optDebug KW_GET optIrredundant KW_VARIANTS
			{
			  lexerCmdMode();
			  moduleExpr.clear();
			  number = NONE;
			}
			numberModuleTerm
			{
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.getVariants(lexerBubble, number, $3, $1);
			}
		|	optDebug KW_CONTINUE optNumber '.'
			{
			  interpreter.cont($3, $1);
			}
		|	KW_TEST 
			{
			  //
			  //	test is a generic command to call code with a term for development purposes.
			  //
			  lexerCmdMode();
			  moduleExpr.clear();
			}
			moduleAndTerm
			{
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.test(lexerBubble);
			    //interpreter.newNarrow(lexerBubble);

			}
		|	KW_LOOP
			{
			  lexerCmdMode();
			  moduleExpr.clear();
			}
			moduleAndTerm
			{
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.loop(lexerBubble);
			}
		|	parenBubble
			{
			  moduleExpr.clear();
			  if (interpreter.setCurrentModule(moduleExpr))  // HACK
			    interpreter.contLoop(lexerBubble);
			}
		|	KW_TRACE select		{ lexerCmdMode(); }
			opSelect
			{
			  lexerInitialMode();
			  interpreter.traceSelect($2);
			}
		|	KW_TRACE exclude		{ lexerCmdMode(); }
			opSelect
			{
			  lexerInitialMode();
			  interpreter.traceExclude($2);
			}
		|	KW_BREAK select		{ lexerCmdMode(); }
			opSelect
			{
			  lexerInitialMode();
			  interpreter.breakSelect($2);
			}
		|	KW_PRINT conceal		{ lexerCmdMode(); }
			opSelect
			{
			  lexerInitialMode();
			  interpreter.printConceal($2);
			}
		|	KW_DO KW_CLEAR KW_MEMO	{ lexBubble(END_COMMAND, 0); }
			endBubble
			{
			  if (interpreter.setCurrentModule(lexerBubble))
			    CM->getFlatSignature()->clearMemo();
			}
/*
 *	Commands to show interpreter state.
 */
		|	KW_SHOW kw_module	{ lexBubble(END_COMMAND, 0); }
			endBubble
			{
			  if (interpreter.setCurrentModule(lexerBubble))
			    interpreter.showPreModule();
			}
		|	KW_SHOW KW_ALL		{ lexBubble(END_COMMAND, 0); }
			endBubble
			{
			  if (interpreter.setCurrentModule(lexerBubble))
			    interpreter.showModule(true);
			}
		|	KW_SHOW KW_DESUGARED	{ lexBubble(END_COMMAND, 0); }
			endBubble
			{
			  if (interpreter.setCurrentModule(lexerBubble))
			    interpreter.showModule(false);
			}
		|	KW_SHOW KW_VIEW		{ lexBubble(END_COMMAND, 0); }
			endBubble
			{
			  if (interpreter.setCurrentView(lexerBubble))
			    interpreter.showView();
			}
		|	KW_SHOW KW_PROCESSED KW_VIEW	{ lexBubble(END_COMMAND, 0); }
			endBubble
			{
			  if (interpreter.setCurrentView(lexerBubble))
			    interpreter.showProcessedView();
			}
		|	KW_SHOW KW_MODULES '.'
			{
			  interpreter.showModules(true);
			}
		|	KW_SHOW KW_VIEWS '.'
			{
			  interpreter.showViews(true);
			}
		|	KW_SHOW KW_SORTS	{ lexBubble(END_COMMAND, 0); }
			endBubble
			{
			  if (interpreter.setCurrentModule(lexerBubble))
			    interpreter.showSortsAndSubsorts();
			}
		|	KW_SHOW KW_OPS2		{ lexBubble(END_COMMAND, 0); }
			endBubble
			{
			  if (interpreter.setCurrentModule(lexerBubble))
			    interpreter.showOps();
			}
		|	KW_SHOW KW_VARS		{ lexBubble(END_COMMAND, 0); }
			endBubble
			{
			  if (interpreter.setCurrentModule(lexerBubble))
			    interpreter.showVars();
			}
		|	KW_SHOW KW_MBS		{ lexBubble(END_COMMAND, 0); }
			endBubble
			{
			  if (interpreter.setCurrentModule(lexerBubble))
			    interpreter.showMbs();
			}
		|	KW_SHOW KW_EQS		{ lexBubble(END_COMMAND, 0); }
			endBubble
			{
			  if (interpreter.setCurrentModule(lexerBubble))
			    interpreter.showEqs();
			}
		|	KW_SHOW KW_RLS		{ lexBubble(END_COMMAND, 0); }
			endBubble
			{
			  if (interpreter.setCurrentModule(lexerBubble))
			    interpreter.showRls();
			}
		|	KW_SHOW KW_STRATS	{ lexBubble(END_COMMAND, 0); }
			endBubble
			{
			  if (interpreter.setCurrentModule(lexerBubble))
			    interpreter.showStrats();
			}
		|	KW_SHOW KW_SDS		{ lexBubble(END_COMMAND, 0); }
			endBubble
			{
			  if (interpreter.setCurrentModule(lexerBubble))
			    interpreter.showSds();
			}
		|	KW_SHOW KW_SUMMARY	{ lexBubble(END_COMMAND, 0); }
			endBubble
			{
			  if (interpreter.setCurrentModule(lexerBubble))
			    interpreter.showSummary();
			}
		|	KW_SHOW KW_KINDS	{ lexBubble(END_COMMAND, 0); }
			endBubble
			{
			  if (interpreter.setCurrentModule(lexerBubble))
			    interpreter.showKinds();
			}
		|	KW_SHOW KW_PATH SIMPLE_NUMBER '.'
			{
			  interpreter.showSearchPath($3, true);
			}
		|	KW_SHOW KW_PATH KW_STATE SIMPLE_NUMBER '.'
			{
			  interpreter.showSearchPath($4, false);
			}
		|	KW_SHOW KW_PATH KW_LABEL SIMPLE_NUMBER '.'
			{
			  interpreter.showSearchPathLabels($4);
			}
		|	KW_SHOW KW_SEARCH KW_GRAPH '.'
			{
			  interpreter.showSearchGraph();
			}
		|	KW_SHOW KW_PROFILE	{ lexBubble(END_COMMAND, 0); }
			endBubble
			{
			  if (interpreter.setCurrentModule(lexerBubble))
			    interpreter.showProfile();
			}
		|	KW_SHOW KW_FRONTIER KW_STATE '.'
			{
			  interpreter.showFrontierStates();
			}
		|	KW_SHOW KW_MOST KW_GENERAL KW_STATE '.'
			{
			  interpreter.showMostGeneralStates();
			}
/*
 *	Commands to set interpreter state variables.
 */
		|	KW_SET KW_SHOW KW_ADVISE polarityDot
			{
			  globalAdvisoryFlag = alwaysAdviseFlag ? true : $4;
			}
		|	KW_SET KW_SHOW KW_STATS polarityDot
			{
			  interpreter.setFlag(Interpreter::SHOW_STATS, $4);
			}
		|	KW_SET KW_SHOW KW_LOOP KW_STATS polarityDot
			{
			  interpreter.setFlag(Interpreter::SHOW_LOOP_STATS, $5);
			}
		|	KW_SET KW_SHOW KW_TIMING polarityDot
			{
			  interpreter.setFlag(Interpreter::SHOW_TIMING, $4);
			}
		|	KW_SET KW_SHOW KW_BREAKDOWN polarityDot
			{
			  interpreter.setFlag(Interpreter::SHOW_BREAKDOWN, $4);
			}
		|	KW_SET KW_SHOW KW_LOOP KW_TIMING polarityDot
			{
			  interpreter.setFlag(Interpreter::SHOW_LOOP_TIMING, $5);
			}
		|	KW_SET KW_SHOW KW_CMD polarityDot
			{
			  interpreter.setFlag(Interpreter::SHOW_COMMAND, $4);
			}
		|	KW_SET KW_SHOW KW_GC polarityDot
			{
			  MemoryCell::setShowGC($4);
			}
		|	KW_SET KW_SHOW KW_RESOURCES polarityDot
			{
			  MemoryCell::setShowResources($4);
			}
		|	KW_SET KW_PRINT printOption polarityDot
			{
			  interpreter.setPrintFlag($3, $4);
			}
		|	KW_SET KW_PRINT KW_ATTRIBUTE polarityDot
			{
			  // a general flag, not a print flag, to activate print attribute
			  interpreter.setFlag(Interpreter::PRINT_ATTRIBUTE, $4);
			}
		|	KW_SET KW_PRINT KW_ATTRIBUTE KW_NEWLINE polarityDot
			{
			  // a general flag, not a print flag, to print a newline after print attribute message
			  interpreter.setFlag(Interpreter::PRINT_ATTRIBUTE_NEWLINE, $5);
			}
		|	KW_SET KW_TRACE traceOption polarityDot
			{
			  interpreter.setFlag($3, $4);
			}
		|	KW_SET KW_BREAK polarityDot
			{
			  interpreter.setFlag(Interpreter::BREAK, $3);
			}
		|	KW_SET importMode		{ lexerCmdMode(); }
			cSimpleTokenBarDot		{ lexerInitialMode(); }
			polarityDot
			{
			  interpreter.setAutoImport($2, $4, $6);
			}
		|	KW_SET KW_OO KW_INCLUDE		{ lexerCmdMode(); }
			cSimpleTokenBarDot		{ lexerInitialMode(); }
			polarityDot
			{
			  interpreter.setOoInclude($5, $7);
			}
		|	KW_SET KW_VERBOSE polarityDot
			{
			  globalVerboseFlag = $3;
			}
		|	KW_SET KW_CLEAR KW_MEMO polarityDot
			{
			  interpreter.setFlag(Interpreter::AUTO_CLEAR_MEMO, $4);
			}
		|	KW_SET KW_CLEAR KW_RLS polarityDot
			{
			  interpreter.setFlag(Interpreter::AUTO_CLEAR_RULES, $4);
			}
		|	KW_SET KW_COMPILE KW_COUNT polarityDot
			{
			  interpreter.setFlag(Interpreter::COMPILE_COUNT, $4);
			}
		|	KW_SET KW_PROFILE polarityDot
			{
			  interpreter.setFlag(Interpreter::PROFILE, $3);
			}
		|	KW_SET KW_CLEAR KW_PROFILE polarityDot
			{
			  interpreter.setFlag(Interpreter::AUTO_CLEAR_PROFILE, $4);
			}
		|	KW_SET KW_CLEAR kw_module KW_CACHES polarityDot
			{
			  interpreter.setFlag(Interpreter::AUTO_CLEAR_CACHES, $5);
			}
/*
 *	Debugger mode commands
 */
		|	KW_RESUME '.'
			{
			  PARSE_RESULT = UserLevelRewritingContext::RESUME;
			}
		|	KW_ABORT '.'
			{
			  PARSE_RESULT = UserLevelRewritingContext::ABORT;
			}
		|	KW_STEP '.'
			{
			  PARSE_RESULT = UserLevelRewritingContext::STEP;
			}
		|	KW_IMPLIED_STEP
			{
			  PARSE_RESULT = UserLevelRewritingContext::STEP;
			}
		|	KW_WHERE '.'
			{
			  PARSE_RESULT = UserLevelRewritingContext::WHERE;
			}
/*
 *	OBJ3 legacy cruft.
 */
		|	KW_SET KW_GC KW_SHOW polarityDot
			{
			  MemoryCell::setShowGC($4);
			}
		|	KW_SET KW_STATS polarityDot
			{
			  interpreter.setFlag(Interpreter::SHOW_STATS, $3);
			}
/*
 *	Error recovery.
 */
		|	error			{ lexerInitialMode(); }
			'.'
		;
/*
 *	Allow mod as an abbreviation for module (can't do this is lexer because of mod ... endm for system modules).
 */
kw_module	:	KW_MODULE
		|	KW_MOD
		;

/*
 *	Options
 */
printOption	:	KW_MIXFIX		{ $$ = PrintSettings::PRINT_MIXFIX; }
		|	KW_FLAT			{ $$ = PrintSettings::PRINT_FLAT; }
		|	KW_WITH KW_ALIASES	{ $$ = PrintSettings::PRINT_WITH_ALIASES; }
		|	KW_WITH KW_PARENS	{ $$ = PrintSettings::PRINT_WITH_PARENS; }
		|	KW_GRAPH		{ $$ = PrintSettings::PRINT_GRAPH; }
		|	KW_CONCEAL		{ $$ = PrintSettings::PRINT_CONCEAL; }
		|	KW_NUMBER		{ $$ = PrintSettings::PRINT_NUMBER; }
		|	KW_RAT			{ $$ = PrintSettings::PRINT_RAT; }
		|	KW_COLOR		{ $$ = PrintSettings::PRINT_COLOR; }
		|	KW_FORMAT		{ $$ = PrintSettings::PRINT_FORMAT; }
		|	KW_CONST KW_WITH KW_SORTS	{ $$ = PrintSettings::PRINT_DISAMBIG_CONST; }
		|	KW_LABEL KW_ATTRIBUTE	{ $$ = PrintSettings::PRINT_LABEL_ATTRIBUTE; }
		|	KW_HOOKS 		{ $$ = PrintSettings::PRINT_HOOKS; }
		|	KW_COMBINE KW_VARS	{ $$ = PrintSettings::PRINT_COMBINE_VARS; }
		|	KW_LATEX   		{ $$ = PrintSettings::PRINT_LATEX; }
		;

traceOption	:				{ $$ = Interpreter::TRACE; }
		|	KW_CONDITION		{ $$ = Interpreter::TRACE_CONDITION; }
		|	KW_WHOLE		{ $$ = Interpreter::TRACE_WHOLE; }
		|	KW_SUBSTITUTION		{ $$ = Interpreter::TRACE_SUBSTITUTION; }
		|	KW_SELECT		{ $$ = Interpreter::TRACE_SELECT; }
		|	KW_MBS			{ $$ = Interpreter::TRACE_MB; }
		|	KW_EQS			{ $$ = Interpreter::TRACE_EQ; }
		|	KW_RLS			{ $$ = Interpreter::TRACE_RL; }
		|	KW_SDS			{ $$ = Interpreter::TRACE_SD; }
		|	KW_REWRITE		{ $$ = Interpreter::TRACE_REWRITE; }
		|	KW_BODY			{ $$ = Interpreter::TRACE_BODY; }
		|	KW_BUILTIN		{ $$ = Interpreter::TRACE_BUILTIN; }
		;

polarityDot	:	KW_ON '.'		{ $$ = true; }
		|	KW_OFF '.'		{ $$ = false; }
		|	KW_ON_DOT
			{
			  IssueWarning(LineNumber(lineNumber) << ": missing space between " << QUOTE("on") << " and period.");
			  $$ = true;
			}
		|	KW_OFF_DOT
			{
			  IssueWarning(LineNumber(lineNumber) << ": missing space between " << QUOTE("off") << " and period.");
			  $$ = false;
			}
		;

select		:	KW_SELECT		{ $$ = true; }
		|	KW_DESELECT		{ $$ = false; }
		;

exclude		:	KW_EXCLUDE	       	{ $$ = true; }
		|	KW_INCLUDE	       	{ $$ = false; }
		;

conceal		:	KW_CONCEAL		{ $$ = true; }
		|	KW_REVEAL		{ $$ = false; }
		;
/*
 *	Return what sort of search we should do; vu-narrow is handle separately because it has two sets of options.
 */
search		:	KW_NARROW		{ $$ = Interpreter::NARROW; }
		|	KW_XG_NARROW		{ $$ = Interpreter::XG_NARROW; }
		|	KW_SEARCH		{ $$ = Interpreter::SEARCH; }
		|	KW_SMT_SEARCH		{ $$ = Interpreter::SMT_SEARCH; }
		|	KW_FVU_NARROW		{ $$ = Interpreter::FVU_NARROW; }
		;

match		:	KW_XMATCH		{ $$ = true; }
		|	KW_MATCH		{ $$ = false; }
		;

optDebug       	:	KW_DEBUG 	       	{ $$ = true; }
		|	       			{ $$ = false; }
		;

optIrredundant	:	KW_IRREDUNDANT		{ $$ = true; }
		|	       			{ $$ = false; }
		;

optNumber	:	SIMPLE_NUMBER	        { $$ = $1; }
		|				{ $$ = NONE; }
		;

optOptions	:	'{' optionsList '}'	{ $$ = $2; }
		|	    			{ $$ = 0; }
		;

optionsList	:	option			{ $$ = $1; }
		|	optionsList ',' option  { $$ = $1 | $3; }
		;

option		:	KW_FOLD			{ $$ = NarrowingSequenceSearch3::FOLD; }
		|	KW_VFOLD		{ $$ = NarrowingSequenceSearch3::VFOLD; }
		|	KW_PATH			{ $$ = NarrowingSequenceSearch3::KEEP_PATHS; }
		;

importMode	:	KW_PROTECT		{ $$ = ImportModule::PROTECTING; }
		|	KW_EXTEND		{ $$ = ImportModule::EXTENDING; }
		|	KW_GENERATE_BY		{ $$ = ImportModule::GENERATED_BY; }
		|	KW_INCLUDE		{ $$ = ImportModule::INCLUDING; }
		;
/*
 *	Optional module expression followed by term followed by dot.
 *	{"in" <module expression> ":"} <term> "."
 *	<module expression> is a (possibly empty) bubble not containing ':' or '.'
 *	<term> is a (non-empty) bubble not containing '.' except as first token
 */

moduleAndTerm	:	initialEndBubble
		|	cTokenBarIn		{ lexBubble($1, END_COMMAND, 0); }
			endBubble
		;

inEnd		:	':'			{ moduleExpr = lexerBubble; lexBubble(END_COMMAND, 1); }
			endBubble
		|	endBubble
		;

/*
 *	Optional [number] followed by optional module expression, followed
 *	by term, followed by dot.
 *	{"[" <number> "]"} {"in" <module expression> ":"} <term> "."
 */

numberModuleTerm
		:	'['			{ lexSave($1); }
			numberModuleTerm1
		|	initialEndBubble
		|	cTokenBarLeftIn		{ lexBubble($1, END_COMMAND, 0);  }
			endBubble
		;

numberModuleTerm1
		:	NUMERIC_ID		{ lexContinueSave($1); }
			numberModuleTerm2
		|	cTokenBarDotNumber	{ lexContinueBubble($1, END_COMMAND, 0); }
			endBubble
		|	miscEndBubble
		;

numberModuleTerm2
		:	']'			{ number = Token::codeToInt64(lexerBubble[1].code()); }
			moduleAndTerm
		|	cTokenBarDotRight	{ lexContinueBubble($1, END_COMMAND, 0); }
			endBubble
		|	miscEndBubble
		;

/*
 *	Optional [number],  [, number] or [number, number] followed by optional
 *	module expression, followed by term, followed by dot.
 *	{"[" { {<number>} , } <number> "]"} {"in" <module expression> ":"} <term> "."
 */


/*
 *	Seen <command>; looking for "{", "[", "in", or start of term.
 */
optionsNumbersModuleTerm
		:	'{'			{ lexSave($1); }
			optionsNumbersModuleTerm1
		|	'['			{ lexSave($1); }
			numbersModuleTerm1
		|	initialEndBubble
		|	cTokenBarOpenLeftIn	{ lexBubble($1, END_COMMAND, 0); }
			endBubble
		;

optionsNumbersModuleTerm1
		:	KW_FILTER
			{
			  lexContinueSave($1);
			  variantOptions |= VariantUnificationProblem::FILTER_VARIANT_UNIFIERS;
			}
			optionsNumbersModuleTerm2
		|	KW_DELAY
			{
			  lexContinueSave($1);
			  variantOptions |= VariantSearch::IRREDUNDANT_MODE;
			}
			optionsNumbersModuleTerm2
		|	cTokenBarDotOptionToken	{ lexContinueBubble($1, END_COMMAND, 0); }
			endBubble
		|	miscEndBubble
		;
		
optionsNumbersModuleTerm2
		:	'}'
			numbersModuleTerm
		|	','			{ lexContinueSave($1); }
			optionsNumbersModuleTerm1
		|	cTokenBarDotCommaClose	{ lexContinueBubble($1, END_COMMAND, 0); }
			endBubble
		|	miscEndBubble
		;

/*
 *	Seen <command>; looking for "[", "in", or start of term.
 */
numbersModuleTerm
		:	'['			{ lexSave($1); }
			numbersModuleTerm1
		|	initialEndBubble
		|	cTokenBarLeftIn		{ lexBubble($1, END_COMMAND, 0); }
			endBubble
		;

/*
 *	Seen <command> "["; looking for <number>, ",", continuation of
 *	term or "." to end command.
 */
numbersModuleTerm1
		:	NUMERIC_ID		{ lexContinueSave($1); }
			numbersModuleTerm2
		|	','			{ lexContinueSave($1); }
			numbersModuleTerm5
		|	cTokenBarDotCommaNumber	{ lexContinueBubble($1, END_COMMAND, 0); }
			endBubble
		|	miscEndBubble
		;

/*
 *	Seen <command> "[" <number>; looking for "]", ",",
 *	continuation of term or "." to end command.
 */
numbersModuleTerm2
		:	']'			{ number = Token::codeToInt64(lexerBubble[1].code()); }
			moduleAndTerm
		|	','			{ lexContinueSave($1); }
			numbersModuleTerm3
		|	cTokenBarDotCommaRight	{ lexContinueBubble($1, END_COMMAND, 0); }
			endBubble
		|	miscEndBubble
		;

/*
 *	Seen <command> "[" <number> ","; looking for <number>,
 *	continuation of term or "." to end command.
 */
numbersModuleTerm3
		:	NUMERIC_ID		{ lexContinueSave($1); }
			numbersModuleTerm4
		|	cTokenBarDotNumber	{ lexContinueBubble($1, END_COMMAND, 0); }
			endBubble
		|	miscEndBubble
		;

/*
 *	Seen <command> "[" <number> "," <number>; looking for "]",
 *	continuation of term or "." to end command.
 */
numbersModuleTerm4
		:	']'
			{
			  number = Token::codeToInt64(lexerBubble[1].code());
			  number2 = Token::codeToInt64(lexerBubble[3].code());
			  tokensClear();
			}
			moduleAndTerm
		|	cTokenBarDotRight	{ lexContinueBubble($1, END_COMMAND, 0); }
			endBubble
		|	miscEndBubble
		;

/*
 *	Seen <command> "[" ","; looking for <number>, continuation of
 *	term or "." to end command. 
 */
numbersModuleTerm5
		:	NUMERIC_ID		{ lexContinueSave($1); }
			numbersModuleTerm6
		|	cTokenBarDotNumber	{ lexContinueBubble($1, END_COMMAND, 0); }
			endBubble
		|	miscEndBubble
		;

/*
 *	Seen <command> "[" "," <number>; looking for "]", continuation
 *	of term or "." to end command. 
 */
numbersModuleTerm6
		:	']'			{ number2 = Token::codeToInt64(lexerBubble[2].code()); }
			moduleAndTerm
		|	cTokenBarDotRight	{ lexContinueBubble($1, END_COMMAND, 0); }
			endBubble
		|	miscEndBubble
		;

miscEndBubble	:	'('			{ lexContinueBubble($1, END_COMMAND, 0, 1); }
			endBubble
		|	endBubble
		;

/*
 *	Handles ( <bubble> )
 *	        in <bubble> :
 *		foo.
 */
initialEndBubble
		:	'('			{ lexBubble($1, END_COMMAND, 1, 1); }
			endBubble
		|	KW_IN			{ lexBubble($1, BAR_COLON | END_COMMAND, 0); }
			inEnd
		|	ENDS_IN_DOT
			{
			  lexerBubble.resize(1);
			  lexerBubble[0].dropChar($1);
			  missingSpace(lexerBubble[0]);
			}
		;

/*
 *	Command mode token types.
 */
cTokenBarIn	:	cOptionToken | IDENTIFIER | NUMERIC_ID | '{' | '}' | '[' | ']' | ':' | '.' | ','
		;

cTokenBarLeftIn	:	cOptionToken | IDENTIFIER | NUMERIC_ID | '{' | '}' | ']' | ':' | '.' | ','
		;

cTokenBarOpenLeftIn
		:	cOptionToken | IDENTIFIER | NUMERIC_ID | '}' | ']' | ':' | '.' | ','
		;

cTokenBarDotNumber
		:	cOptionToken | IDENTIFIER | '{' | '}' | '[' | ']' | KW_IN | ':' | ','
		;

cTokenBarDotRight
		:	cOptionToken | IDENTIFIER | NUMERIC_ID | '{' | '}' | '[' | KW_IN | ':' | ','
		;

cTokenBarDotCommaNumber
		:	cOptionToken | IDENTIFIER | '{' | '}' | '[' | ']' | KW_IN | ':'
		;

cTokenBarDotCommaRight
		:	cOptionToken | IDENTIFIER | NUMERIC_ID | '{' | '}' | '[' | KW_IN | ':'
		;

cTokenBarDotCommaClose
		:	cOptionToken | IDENTIFIER | NUMERIC_ID | '{' | '[' | ']' | KW_IN | ':'
		;

cTokenBarDotOptionToken
		:	IDENTIFIER | NUMERIC_ID | '{' | '}' | '[' | ']' | KW_IN | ':' | ','
		;

/*
 *	Selections are lists of operator names.
 */
opSelect	:	cOpNameList endSelect
		|	badSelect
		;

endSelect	:	'.'		{}
		|	badSelect
		;

badSelect	:	ENDS_IN_DOT
			{
			  singleton[0].dropChar($1);
			  missingSpace(singleton[0]);
			  interpreter.addSelected(singleton);
			}

cOpNameList	:	cOpNameList cSimpleOpName
		|	cSimpleOpName
		;

cSimpleOpName	:	cSimpleTokenBarDot
			{
			  singleton[0] = $1;
			  interpreter.addSelected(singleton);
			}
		|	parenBubble
			{
			  interpreter.addSelected(lexerBubble);
			}
		;

cSimpleTokenBarDot
		:	cOptionToken | IDENTIFIER | NUMERIC_ID | '{' | '}' | '[' | ']' | KW_IN | ':' | ','
		;

cOptionToken	:	KW_FILTER
		|	KW_DELAY
		;
