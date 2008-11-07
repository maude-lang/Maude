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
 *	Module expressions.
 */
moduleExprDot	:	tokenBarDot expectedDot
                        {
                          moduleExpressions.push(new ModuleExpression($1));
                        }
		|	endsInDot '.'
                        {
                          moduleExpressions.push(new ModuleExpression($1));
                        }
		|	parenExpr expectedDot
		|	renameExpr expectedDot
		|	instantExpr expectedDot
		|	moduleExpr '+' moduleExprDot
			{
			  ModuleExpression* m1 = moduleExpressions.top();
			  moduleExpressions.pop();
			  ModuleExpression* m2 = moduleExpressions.top();
			  moduleExpressions.pop();
			  moduleExpressions.push(new ModuleExpression(m1, m2));
			}
		|	ENDS_IN_DOT
			{
			  Token t;
			  t.dropChar($1);
			  missingSpace(t);
			  moduleExpressions.push(new ModuleExpression(t));
			}
		;

moduleExpr	:	moduleExpr2
		|	moduleExpr '+' moduleExpr
			{
			  ModuleExpression* m1 = moduleExpressions.top();
			  moduleExpressions.pop();
			  ModuleExpression* m2 = moduleExpressions.top();
			  moduleExpressions.pop();
			  moduleExpressions.push(new ModuleExpression(m1, m2));
			}
		;

moduleExpr2	:	moduleExpr3
		|	renameExpr
		;

moduleExpr3	:	parenExpr
		|	instantExpr
		|	token
		        {
                          moduleExpressions.push(new ModuleExpression($1));
                        }

		;
		
renameExpr	:	moduleExpr2 '*' renaming
			{
			  ModuleExpression* m = moduleExpressions.top();
			  moduleExpressions.pop();
			  moduleExpressions.push(new ModuleExpression(m, currentRenaming));
			  currentRenaming = 0;
			}
		;

instantExpr	:	moduleExpr3 '{'			{ clear(); }
			argList '}'
			{
			  ModuleExpression* m = moduleExpressions.top();
			  moduleExpressions.pop();
			  moduleExpressions.push(new ModuleExpression(m, tokenSequence));
			}
		;

parenExpr	:	'(' moduleExpr ')' {}
		;

argList		:	argList ',' token		{ store($3); }
		|	token				{ store($1); }
		;

/*
 *	Renamings.
 */
renaming	:	'('
			{
			  oldSyntaxContainer = currentSyntaxContainer;
			  currentSyntaxContainer = currentRenaming = new Renaming;
			}
			renaming2  /* must succeed so we can restore currentSyntaxContainer */
			{
			  currentSyntaxContainer = oldSyntaxContainer;
			}
			')'
		;

renaming2	:	mappingList
		|	error
		;

mappingList	:	mappingList ',' mapping
		|	mapping
		;

mapping		:	KW_SORT sortName KW_TO sortName
			{
			  currentRenaming->addSortMapping($2, $4);
			}
		|	KW_LABEL token KW_TO token
			{
			  currentRenaming->addLabelMapping($2, $4);
			}
		|	KW_OP 			{ lexBubble(BAR_COLON | BAR_TO, 1); }
			fromSpec KW_TO		{ lexBubble(BAR_COMMA | BAR_LEFT_BRACKET | BAR_RIGHT_PAREN, 1); }
			toAttributes		{}
		;
/*
 *	The ':' alternative forces lookahead which allows the lexer to grab the bubble.
 */
fromSpec	:	':'			{ Token::peelParens(lexerBubble); currentRenaming->addOpMapping(lexerBubble); }
			typeList arrow typeName {}
		|				{ Token::peelParens(lexerBubble); currentRenaming->addOpMapping(lexerBubble); }
		;

/*
 *	The '[' alternative forces lookahead which allows the lexer to grab the bubble.
 */
toAttributes	:	'['			{ Token::peelParens(lexerBubble); currentRenaming->addOpTarget(lexerBubble); }
			toAttributeList ']'	{}
		|				{ Token::peelParens(lexerBubble); currentRenaming->addOpTarget(lexerBubble); }
		;

toAttributeList	:	toAttributeList toAttribute
		|	toAttribute
		;

toAttribute	:	KW_PREC IDENTIFIER	{ currentRenaming->setPrec($2); }
		|	KW_GATHER '('		{ clear(); }
			idList ')'		{ currentRenaming->setGather(tokenSequence); }
		|	KW_FORMAT '('		{ clear(); }
			idList ')'		{ currentRenaming->setFormat(tokenSequence); }
		|	KW_LATEX '('		{ lexerLatexMode(); }
			LATEX_STRING ')'	{ currentRenaming->setLatexMacro($4); }
		;

/*
 *	Views.
 */
view		:	KW_VIEW			{ lexerIdMode(); }
			token KW_FROM moduleExpr
			{
			  fileTable.beginModule($1, $3);
			  interpreter.setCurrentView(new View($3));
			  currentSyntaxContainer = CV;
			  CV->addFrom(moduleExpressions.top());
			  moduleExpressions.pop();
			}
			KW_TO moduleExpr
			{
			  CV->addTo(moduleExpressions.top());
			  moduleExpressions.pop();
			}
			expectedIs viewDecList KW_ENDV
			{
			  lexerInitialMode();
			  fileTable.endModule(lineNumber);
			  interpreter.insertView(($3).code(), CV);
			  CV->finishView();
			}
		;

viewDecList	:	viewDecList viewDeclaration
		|
		;

viewDeclaration	:	KW_SORT sortName KW_TO sortDot
			{
			  CV->addSortMapping($2, $4);
			}
		|	KW_VAR varNameList ':' typeDot {}
		|	KW_OP			{ lexBubble(BAR_COLON | BAR_TO, 1); }
			viewEndOpMap
		|	error '.'
		;

sortDot		:	sortName expectedDot		{ $$ = $1; }
		|	ENDS_IN_DOT
			{
			  Token t;
			  t.dropChar($1);
			  missingSpace(t);
			  $$ = t;
			}
		;

viewEndOpMap	:	':'
			{
			  //
			  //	Specific op->op mapping.
			  //
			  Token::peelParens(lexerBubble);  // remove any enclosing parens from op name
			  CV->addOpMapping(lexerBubble);
			}
			typeList arrow typeName KW_TO
			{
			  lexBubble(END_STATEMENT, 1);
			}
			endBubble
			{
			  Token::peelParens(lexerBubble);  // remove any enclosing parens from op name
			  CV->addOpTarget(lexerBubble);
			}
		|	KW_TO
			{
			  //
			  //	At this point we don't know if we have an op->term mapping
			  //	or a generic op->op mapping so we save the from description and
			  //	press on.
			  //
			  opDescription = lexerBubble;
			  lexBubble(END_STATEMENT, 1)
			}
			endBubble
			{
			  if (lexerBubble[0].code() == Token::encode("term"))
			    {
			      //
			      //	Op->term mapping.
			      //
			      CV->addOpTermMapping(opDescription, lexerBubble);
			    }
			  else
			    {
			      //
			      //	Generic op->op mapping.
			      //
			      Token::peelParens(opDescription);  // remove any enclosing parens from op name
			      CV->addOpMapping(opDescription);
			      Token::peelParens(lexerBubble);  // remove any enclosing parens from op name
			      CV->addOpTarget(lexerBubble);
			    }
			}
		;


endBubble	:	'.' {}
		|	ENDS_IN_DOT
			{
			  Token t;
			  t.dropChar($1);
			  missingSpace(t);
			  lexerBubble.append(t);
			}
		;

parenBubble	:	'(' 			{ lexBubble(BAR_RIGHT_PAREN, 1); }
			')'			{}
		;

/*
 *	Modules and theories.
 */
module		:	startModule		{ lexerIdMode(); }
			token
			{
			  interpreter.setCurrentModule(new PreModule($1, $3));
			  currentSyntaxContainer = CM;
			  fileTable.beginModule($1, $3);
			}
			parameters expectedIs decList KW_ENDM
			{
			  lexerInitialMode();
			  fileTable.endModule(lineNumber);
			  CM->finishModule($8);
			}
		;

dot		:	'.' {}
		|	ENDS_IN_DOT
			{
			  Token t;
			  t.dropChar($1);
			  missingSpace(t);
			  store(t);
			}
		;

parameters	:	'{' parameterList '}' {}
		|
		;

parameterList	:	parameterList ',' parameter
		|	parameter
		;

parameter	:	token KW_COLON2 moduleExpr
			{
			  ModuleExpression* me = moduleExpressions.top();
			  moduleExpressions.pop();
			  CM->addParameter($1, me);
			}
		;

badType		:	ENDS_IN_DOT
			{
			  singleton[0].dropChar($1);
			  missingSpace(singleton[0]);
			  currentSyntaxContainer->addType(false, singleton);
			  $$ = $1;  // needed for line number
			}
		;

typeDot		:	typeName expectedDot
		|	badType {}
		;

startModule	:	KW_MOD | KW_OMOD
		;

decList		:	decList declaration
		|
		;

declaration	:	KW_IMPORT moduleExprDot
			{
			  ModuleExpression* me = moduleExpressions.top();
			  moduleExpressions.pop();
			  CM->addImport($1, me);
			}

		|	KW_SORT			{ clear(); }
			sortNameList dot	{ CM->addSortDecl(tokenSequence); }

		|	KW_SUBSORT		{ clear(); }
			subsortList dot		{ CM->addSubsortDecl(tokenSequence); }

		|	KW_OP			{ lexBubble(BAR_COLON, 1); }
			':'			{ Token::peelParens(lexerBubble); CM->addOpDecl(lexerBubble); }
			domainRangeAttr		{}

		|	KW_OPS opNameList ':' domainRangeAttr {}

		|	KW_VAR varNameList ':' typeDot {}

		|	KW_MB			{ lexBubble($1, BAR_COLON, 1); }
			':'			{ lexContinueBubble($3, END_STATEMENT, 1); }
			endBubble	       	{ CM->addStatement(lexerBubble); }

		|	KW_CMB			{ lexBubble($1, BAR_COLON, 1);  }
			':'			{ lexContinueBubble($3, BAR_IF, 1); }
			KW_IF			{ lexContinueBubble($5, END_STATEMENT, 1); }
			endBubble		{ CM->addStatement(lexerBubble); }

		|	KW_EQ			{ lexBubble($1, BAR_EQUALS, 1); }
			'='			{ lexContinueBubble($3, END_STATEMENT, 1); }
			endBubble	  	{ CM->addStatement(lexerBubble); }

		|	KW_CEQ			{ lexBubble($1, BAR_EQUALS, 1); }
			'='			{ lexContinueBubble($3, BAR_IF, 1); }
			KW_IF			{ lexContinueBubble($5, END_STATEMENT, 1); }
			endBubble	  	{ CM->addStatement(lexerBubble); }

		|	KW_RL			{ lexBubble($1, BAR_ARROW2, 1); }
			KW_ARROW2		{ lexContinueBubble($3, END_STATEMENT, 1); }
			endBubble		{ CM->addStatement(lexerBubble); }

		|	KW_CRL			{ lexBubble($1, BAR_ARROW2, 1); }
			KW_ARROW2		{ lexContinueBubble($3, BAR_IF, 1); }
			KW_IF			{ lexContinueBubble($5, END_STATEMENT, 1); }
			endBubble	    	{ CM->addStatement(lexerBubble); }

		|	KW_MSG			{ lexBubble(BAR_COLON, 1); }
			':'			{ Token::peelParens(lexerBubble); CM->addOpDecl(lexerBubble); }
			domainRangeAttr		{ CM->setFlag(SymbolType::MESSAGE); }

		|	KW_MSGS opNameList ':' domainRangeAttr
			{
			  CM->setFlag(SymbolType::MESSAGE);
			}

		|	KW_CLASS token
			{
			}
			classDef '.'
			{
			}

		|	KW_SUBCLASS		{ clear(); }
			subsortList dot		{ CM->addSubsortDecl(tokenSequence); }

		|	error '.'
		        {
			  //
			  //	Fix things that might be in a bad state due
			  //	to a partially processed declaration.
			  //
			  cleanUpModuleExpression();
			  CM->makeOpDeclsConsistent();
			}
		;

classDef	:	'|' {}
		|	'|' cPairList {}
		;

cPairList	:	cPair
		|	cPairList ',' cPair
		;

cPair		:	tokenBarDot ':' token
			{
			}
		;

varNameList	:	varNameList tokenBarColon	{ currentSyntaxContainer->addVarDecl($2); }
		|	tokenBarColon			{ currentSyntaxContainer->addVarDecl($1); }
		;

opNameList	:	opNameList simpleOpName
		|	simpleOpName
		;

simpleOpName	:	tokenBarColon		{ singleton[0] = $1; CM->addOpDecl(singleton); }
		|	parenBubble		{ CM->addOpDecl(lexerBubble); }
		;

domainRangeAttr	:	typeName typeList dra2
		|	rangeAttr
		|	badType
			{
			  IssueWarning(LineNumber(lineNumber) <<
				       ": missing " << QUOTE("->") << " in constant declaration.");
			}
		;

dra2		:	rangeAttr
		|	'.'
			{
			  IssueWarning(LineNumber($1.lineNumber()) <<
			  ": missing " << QUOTE("->") << " in operator declaration.");
			}
		|	badType
			{
			  IssueWarning(LineNumber($1.lineNumber()) <<
			  ": missing " << QUOTE("->") << " in operator declaration.");
			}
		;

rangeAttr	:	arrow typeAttr
			{
			  if ($1)
			    CM->convertSortsToKinds();
			}
		;

typeAttr	:	typeName attributes expectedDot
		|	badType {}
		;

arrow		:	KW_ARROW      		{ $$ = false; }
		|	KW_PARTIAL	       	{ $$ = true; }
		;

typeList	:	typeList typeName
		|
		;

typeName	:	sortName
			{
			  singleton[0] = $1;
			  currentSyntaxContainer->addType(false, singleton);
			}
		|	'['			{ clear(); }
			sortNames ']'
			{
			  currentSyntaxContainer->addType(true, tokenSequence);
			}
		;

sortNames	:	sortNames ',' sortName		{ store($3); }
		|	sortName			{ store($1); }
		;

attributes	:	'[' attributeList ']'	{}
		|
		;

attributeList	:	attributeList attribute
		|	attribute
		;

idKeyword	:	KW_ID
			{
			  CM->setFlag(SymbolType::LEFT_ID | SymbolType::RIGHT_ID);
			}
		|	KW_LEFT KW_ID
			{
			  CM->setFlag(SymbolType::LEFT_ID);
			}
		|	KW_RIGHT KW_ID
			{
			  CM->setFlag(SymbolType::RIGHT_ID);
			}
		;

attribute	:	KW_ASSOC		
			{
			  CM->setFlag(SymbolType::ASSOC);
			}
		|	KW_COMM
			{
			  CM->setFlag(SymbolType::COMM);
			}
		|	idKeyword		{ lexBubble(BAR_RIGHT_BRACKET | BAR_OP_ATTRIBUTE, 1); }
			identity		{ CM->setIdentity(lexerBubble); }
		|	KW_IDEM
			{
			  CM->setFlag(SymbolType::IDEM);
			}
		|	KW_ITER
			{
			  CM->setFlag(SymbolType::ITER);
			}
		|	KW_PREC IDENTIFIER	{ CM->setPrec($2); }
		|	KW_GATHER '('		{ clear(); }
			idList ')'		{ CM->setGather(tokenSequence); }
		|	KW_FORMAT '('		{ clear(); }
			idList ')'		{ CM->setFormat(tokenSequence); }
		|	KW_STRAT '('		{ clear(); }
			idList ')'		{ CM->setStrat(tokenSequence); }
		|	KW_POLY '('		{ clear(); }
			idList ')'		{ CM->setPoly(tokenSequence); }
		|	KW_MEMO
			{
			  CM->setFlag(SymbolType::MEMO);
			}
		|	KW_CTOR
			{
			  CM->setFlag(SymbolType::CTOR);
			}
		|	KW_FROZEN
			{
			  clear();
			  CM->setFrozen(tokenSequence);
			}
		|	KW_FROZEN '('		{ clear(); }
			idList ')'		{ CM->setFrozen(tokenSequence); }
		|	KW_CONFIG
			{
			  CM->setFlag(SymbolType::CONFIG);
			}
		|	KW_OBJ
			{
			  CM->setFlag(SymbolType::OBJECT);
			}
		|	KW_MSG
			{
			  CM->setFlag(SymbolType::MESSAGE);
			}
		|	KW_METADATA IDENTIFIER
			{
			  CM->setMetadata($2);
			}
		|	KW_LATEX '('		{ lexerLatexMode(); }
			LATEX_STRING ')'	{ CM->setLatexMacro($4); }
		|	KW_SPECIAL '(' hookList ')'	{}
		|	KW_DITTO
			{
			  CM->setFlag(SymbolType::DITTO);
			}
		;

/*
 *	The ony point of this rule is to force a one token lookahead and allow the lexer to grab the
 *	bubble corresponding to the identity. We never see a FORCE_LOOKAHEAD token.
 */
identity	:	FORCE_LOOKAHEAD
		|
		;

idList		:	idList IDENTIFIER	{ store($2); }
		|	IDENTIFIER		{ store($1); }
		;

hookList	:	hookList hook
		|	hook
		;

hook		:	KW_ID_HOOK token		{ clear(); CM->addHook(PreModule::ID_HOOK, $2, tokenSequence); }
		|	KW_ID_HOOK token parenBubble	{ CM->addHook(PreModule::ID_HOOK, $2, lexerBubble); }
		|	KW_OP_HOOK token parenBubble	{ CM->addHook(PreModule::OP_HOOK, $2, lexerBubble); }
		|	KW_TERM_HOOK token parenBubble	{ CM->addHook(PreModule::TERM_HOOK, $2, lexerBubble); }
		;

/*
 *	Recovery from missing tokens
 */
expectedIs	:	KW_IS {}
		|
			{
			  IssueWarning(LineNumber(lineNumber) << ": missing " <<
				       QUOTE("is") << " keyword.");
			}
		;

expectedDot	:	'.' {}
		|
			{
			  IssueWarning(LineNumber(lineNumber) << ": missing period.");
			}
		;

/*
 *	Sort and subsort lists.
 */
sortNameList	:	sortNameList sortName	{ store($2); }
		|
		;

subsortList	:	subsortList sortName	{ store($2); }
		|	subsortList '<'		{ store($2); }
		|	sortName		{ store($1); }
			sortNameList '<'	{ store($4); }
		;

/*
 *	Sort names
 */
sortName	:	sortNameFrag
			{
			  Token t;
			  if (fragments.size() == 1)
			    t = fragments[0];
			  else
			    t.tokenize(Token::bubbleToPrefixNameCode(fragments), fragments[0].lineNumber());
			  fragClear();
			  $$ = t;
			}
		;

sortNameFrag	:	sortToken		{ fragStore($1); }
		|	sortNameFrag '{'	{ fragStore($2); }
			sortNameFrags '}'	{ fragStore($5); }
		;

sortNameFrags	:	sortNameFrags ','	{ fragStore($2); }
			sortNameFrag		{}
		|	sortNameFrag		{}
		;

/*
 *	Token types.
 */
token		:	identifier | startKeyword | midKeyword | attrKeyword | '.'
		;

tokenBarDot	:	inert | ',' | KW_TO
		|	startKeyword | midKeyword | attrKeyword
		;

tokenBarColon	:	identifier | startKeyword | attrKeyword | '.'
		|	'<' | KW_ARROW | KW_PARTIAL | '=' | KW_ARROW2 | KW_IF
		;

sortToken	:	IDENTIFIER | startKeyword | attrKeyword2
		|	'=' | '|' | '+' | '*'
		|	KW_ARROW2 | KW_IF | KW_IS | KW_LABEL | KW_TO
		;

endsInDot	:	'.' | ENDS_IN_DOT
		;

/*
 *	Keywords (in id mode).
 */
inert		:	IDENTIFIER | '{' | '}' | '+' | '*' | '|' | KW_COLON2 | KW_LABEL
		|	KW_FROM | KW_IS
		;

identifier	:	inert | ENDS_IN_DOT | ',' | KW_TO
		;

startKeyword	:	KW_MSG | startKeyword2
		;

startKeyword2	:	KW_IMPORT | KW_SORT | KW_SUBSORT | KW_OP | KW_OPS | KW_VAR
		|	KW_MSGS | KW_CLASS | KW_SUBCLASS
		|	KW_MB | KW_CMB | KW_EQ | KW_CEQ | KW_RL | KW_CRL | KW_ENDM | KW_ENDV
		;

midKeyword	:	'<' | ':' | KW_ARROW | KW_PARTIAL | '=' | KW_ARROW2 | KW_IF
		;

attrKeyword	:	'[' | ']' | attrKeyword2
		;

attrKeyword2	:	KW_ASSOC | KW_COMM | KW_ID | KW_IDEM | KW_ITER | KW_LEFT | KW_RIGHT
		|	KW_PREC | KW_GATHER | KW_STRAT | KW_POLY | KW_MEMO | KW_CTOR
		|	KW_LATEX | KW_SPECIAL | KW_FROZEN | KW_METADATA
		|	KW_CONFIG | KW_OBJ | KW_DITTO | KW_FORMAT
		|	KW_ID_HOOK | KW_OP_HOOK | KW_TERM_HOOK
		;
