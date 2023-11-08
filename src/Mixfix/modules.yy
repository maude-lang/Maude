/*

    This file is part of the Maude 3 interpreter.

    Copyright 1997-2023 SRI International, Menlo Park, CA 94025, USA.

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
                          $$ =  new ModuleExpression($1);
                        }
		|	endsInDot '.'
                        {
                          $$ = new ModuleExpression($1);
                        }
		|	parenExpr expectedDot
		|	renameExpr expectedDot
		|	instantExpr expectedDot
		|	moduleExpr '+' moduleExprDot
			{
			  if ($3)
			    $$ = new ModuleExpression($1, $3);
			  else
			    {
			      IssueWarning(&($2) << ": missing module expression after " << QUOTE($2) << ".");
			      $$ = $1;
			    }
			}
		|	tokenDot
			{
			  $$ = new ModuleExpression($1);
			}
		|	'.'
			{
			  $$ = 0;
			}
		;

moduleExpr	:	moduleExpr2
		|	moduleExpr '+' moduleExpr
			{
			  $$ = new ModuleExpression($1, $3);
			}
		;

moduleExpr2	:	moduleExpr3
		|	renameExpr
		;

moduleExpr3	:	parenExpr
		|	instantExpr
		|	token
		        {
                          $$ = new ModuleExpression($1);
                        }

		;
		
renameExpr	:	moduleExpr2 '*' renaming
			{
			  $$ = new ModuleExpression($1, currentRenaming);
			  currentRenaming = 0;
			}
		;

instantExpr	:	moduleExpr3 '{' instantArgs '}'
			{
			  $$ = new ModuleExpression($1, *($3));
			  delete $3;
			}
		;

parenExpr	:	'(' moduleExpr ')'
			{
			  $$ = $2;
			}
		;

/*
 *	View expressions (parameters are treated as uninstantiated views for syntax purposes).
 */
viewExpr	:	viewExpr '{' instantArgs '}'
			{
			  $$ = new ViewExpression($1, *($3));
			  delete $3;
			}
		|	token
			{
			  $$ = new ViewExpression($1);
			}
		;

instantArgs	:	instantArgs ',' viewExpr
			{
			  $1->append($3);
			  $$ = $1;
			}
		|	viewExpr
			{
			  Vector<ViewExpression*>* t =  new Vector<ViewExpression*>();
			  t->append($1);
			  $$ = t;
			}
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
		|	KW_OP			{ lexBubble(BAR_COLON | BAR_TO, 1); }
			fromSpec KW_TO		{ lexBubble(BAR_COMMA | BAR_LEFT_BRACKET | BAR_RIGHT_PAREN, 1); }
			toAttributes		{}
		|	KW_STRAT stratName	{ currentRenaming->addStratMapping($2); }
			fromStratSpec
			KW_TO stratName 	{ currentRenaming->addStratTarget($6); }
		|	KW_CLASS sortName KW_TO sortName
			{
			  currentRenaming->addClassMapping($2, $4);
			}
		|	KW_ATTR token
			{
			  currentRenaming->addAttrMapping($2);
			}
			fromAttrSpec KW_TO token
			{
			  currentRenaming->addAttrTarget($6);
			}
		|	KW_MSG
			{
			  lexBubble(BAR_COLON | BAR_TO, 1);
			}
			fromSpec KW_TO
			{
			  lexBubble(BAR_COMMA | BAR_LEFT_BRACKET | BAR_RIGHT_PAREN, 1);
			}
			toAttributes
			{
			  currentRenaming->markAsMsg();
			}
		;

/*
 *	The ':' alternative forces lookahead which allows the lexer to grab the bubble.
 */
fromSpec	:	':'			{ Token::peelParens(lexerBubble); currentRenaming->addOpMapping(lexerBubble); }
			typeList arrow typeName {}
		|				{ Token::peelParens(lexerBubble); currentRenaming->addOpMapping(lexerBubble); }
		;

fromAttrSpec	:	':' typeName1
			{
			  currentRenaming->addAttrType($2 ? tokenSequence : singleton);
			}
		|	
		;

fromStratSpec	:	stratSignature
		|
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
		|	KW_PREC			{ IssueWarning(&($1) << ": prec attribute without value in operator mapping."); }
		|	KW_GATHER '('		{ tokensClear(); }
			idList ')'		{ currentRenaming->setGather(tokenSequence); }
		|	KW_GATHER		{ IssueWarning(&($1) << ": gather attribute without pattern in operator mapping."); }
		|	KW_FORMAT '('		{ tokensClear(); }
			idList ')'		{ currentRenaming->setFormat(tokenSequence); }
		|	KW_FORMAT		{ IssueWarning(&($1) << ": format attribute without specification in operator mapping."); }
		|	KW_LATEX '('		{ lexerLatexMode(); }
			LATEX_STRING ')'	{ currentRenaming->setLatexMacro($4); }
		|	KW_LATEX     		{ IssueWarning(&($1) << ": latex attribute without latex code in operator mapping."); }
		;

/*
 *	Views.
 */
view		:	KW_VIEW			{ lexerIdMode(); }
			token
			{
			  fileTable.beginModule($1, $3);
			  interpreter.setCurrentView(new SyntacticView($3, &interpreter));
			  currentSyntaxContainer = CV;
			}
			parameters KW_FROM moduleExpr
			KW_TO moduleExpr
			expectedIs viewDecList KW_ENDV
			{
			  CV->addFrom($7);
			  CV->addTo($9);
			  lexerInitialMode();
			  fileTable.endModule(lineNumber);
			  bool displacedView = interpreter.insertView(($3).code(), CV);
			  CV->finishView();
			  if (displacedView)
			    interpreter.cleanCaches();
			}
		;

viewDecList	:	viewDecList viewDeclaration
		|	{}
		;

skipStrayArrow	:	KW_ARROW
			{
			  IssueWarning(&($1) << ": skipping " << QUOTE("->") << " in variable declaration.");
			}
		|	{}
		;

viewDeclaration	:	KW_SORT sortName KW_TO sortDot
			{
			  CV->addSortMapping($2, $4);
			}
		|	KW_VAR varNameList ':' skipStrayArrow typeDot {}
		|	KW_OP
			{
			  lexBubble(BAR_COLON | BAR_TO, 1);
			}
			viewEndOpMap
		|	KW_STRAT viewStratMap
		|	KW_CLASS sortName KW_TO sortDot
			{
			  CV->addClassMapping($2, $4);
			}
		|	KW_ATTR token
			{
			  CV->addAttrMapping($2);
			}
			fromAttrSpec KW_TO token expectedDot
			{
			  CV->addAttrTarget($6);
			}
		|	KW_MSG
			{
			  lexBubble(BAR_COLON | BAR_TO, 1);
			}
			endMsgMap
		|	error '.'
		|	error END_OF_INPUT
			{
			  fileTable.endModule(lineNumber);
			  IssueWarning(LineNumber(lineNumber) <<
			  ": unexpected end-of-input while parsing view " << QUOTE(CV) << ".");
			  YYABORT;  // rely on the caller to clear up view database and lexer
			}
		|	error CHANGE_FILE
			{
			  fileTable.endModule(lineNumber);
			  IssueWarning(LineNumber(lineNumber) <<
			  ": unexpected end-of-file while parsing view " << QUOTE(CV) << ".");
			  YYABORT;  // rely on the caller to clear up view database and lexer
			}
		;

sortDot		:	sortName expectedDot		{ $$ = $1; }
		|	tokenDot
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
			  lexBubble(END_STATEMENT, 1);
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

endMsgMap	:	':'
			{
			  //
			  //	Specific msg->msg mapping.
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
			  CV->markAsMsg();
			}
		|	KW_TO
			{
			  //
			  //	At this point we don't know if we have an msg->term mapping
			  //	or a generic msg->msg mapping so we save the from description and
			  //	press on.
			  //
			  opDescription = lexerBubble;
			  lexBubble(END_STATEMENT, 1);
			}
			endBubble
			{
			  if (lexerBubble[0].code() == Token::encode("term"))
			    {
			      //
			      //	msg->term mapping.
			      //
			      CV->addOpTermMapping(opDescription, lexerBubble, true);
			    }
			  else
			    {
			      //
			      //	Generic msg->msg mapping.
			      //
			      Token::peelParens(opDescription);  // remove any enclosing parens from op name
			      CV->addOpMapping(opDescription);
			      Token::peelParens(lexerBubble);  // remove any enclosing parens from op name
			      CV->addOpTarget(lexerBubble);
			      CV->markAsMsg();
			    }
			}
		;

strategyCall	:	stratName
			{
			  strategyCall.resize(1);
			  strategyCall[0] = $1;
			}
		|	stratName '('			{ lexBubble(BAR_RIGHT_PAREN, 1); }
			')'
			{
			  // Adds the stratName and parentheses to the lexer bubble
			  int bubbleSize = lexerBubble.length();
			  strategyCall.resize(bubbleSize + 3);
			  strategyCall[0] = $1;
			  strategyCall[1] = $2;
			  for (int i = 0; i < bubbleSize; i++)
			    strategyCall[2 + i] = lexerBubble[i];
			  strategyCall[bubbleSize + 2] = $4;
			}

viewStratMap	:	stratName
			{
			  CV->addStratMapping($1);
			}
			stratSignature KW_TO stratName '.'
			{
			  CV->addStratTarget($5);
			}
		|	strategyCall KW_TO
			{
			  lexBubble(END_STATEMENT, 1);
			}
			endBubble
			{
			  if (lexerBubble[0].code() == Token::encode("expr"))
			    {
			      //
			      //	Strat->expr mapping.
			      //
			      CV->addStratExprMapping(strategyCall, lexerBubble);
			    }
			  else if (strategyCall.length() == 1 && lexerBubble.length() == 1)
			    {
			      //
			      //	Generic strat->strat mapping.
			      //
			      CV->addStratMapping(strategyCall[0]);
			      CV->addStratTarget(lexerBubble[0]);
			    }
			  else
			    {
			      IssueWarning(&(strategyCall[0]) << ": bad syntax for strategy mapping.");
			    }
			}
		;

endBubble	:	'.'		{}
		|	tokenDot	{ lexerBubble.append($1); }
		;

parenBubble	:	'(' 		{ lexBubble(BAR_RIGHT_PAREN, 1); }
			')'		{}
		;

/*
 *	Modules and theories.
 */
module		:	KW_MOD		{ lexerIdMode(); }
			token
			{
			  interpreter.setCurrentModule(new SyntacticPreModule($1, $3, &interpreter));
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

dot		:	'.'		{}
		|	tokenDot	{ tokensStore($1); }
		;

parameters	:	'{' parameterList '}' {}
		|	{}
		;

parameterList	:	parameterList ',' parameter
		|	parameter
		;

parameter	:	token colon2 moduleExpr
			{
			  currentSyntaxContainer->addParameter2($1, $3);
			}
		;

colon2		:	KW_COLON2 {}
		|	':'
			{
			  IssueWarning(&($1) << ": saw " << QUOTE(':') << " instead of " << QUOTE("::") << " in parameter declaration.");
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

decList		:	decList declaration
		|	{}
		;

declaration	:	KW_IMPORT moduleExprDot
			{
			  if ($2)
			    CM->addImport($1, $2);
			  else
			    {
			      IssueWarning(&($1) << ": missing module expression after " << QUOTE($1) << ".");
			    }
			}

		|	KW_SORT			{ tokensClear(); }
			endSortNameList		{ CM->addSortDecl(tokenSequence); }

		|	KW_SUBSORT		{ tokensClear(); }
			endSubsortList		{ CM->addSubsortDecl(tokenSequence); }

		|	KW_OP			{ lexBubble(BAR_COLON, 1); }
			':'			{ Token::peelParens(lexerBubble); CM->addOpDecl(lexerBubble); }
			domainRangeAttr		{}

		|	KW_OPS opNameList ':' domainRangeAttr {}

		|	KW_VAR varNameList ':' skipStrayArrow typeDot {}

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

		|	KW_SD			{ lexBubble($1, BAR_ASSIGN, 1); }
			KW_ASSIGN		{ lexContinueBubble($3, END_STATEMENT, 1); }
			endBubble		{ CM->addStatement(lexerBubble); }

		|	KW_CSD			{ lexBubble($1, BAR_ASSIGN, 1); }
			KW_ASSIGN		{ lexContinueBubble($3, BAR_IF, 1); }
			KW_IF			{ lexContinueBubble($5, END_STATEMENT, 1); }
			endBubble	    	{ CM->addStatement(lexerBubble); }

		|	stratDeclKeyword	{ tokensClear(); }
			stratIdList
			stratSignature
			stratAttributes
			dot			{}

		|	KW_MSG			{ lexBubble(BAR_COLON, 1); }
			':'			{ Token::peelParens(lexerBubble); CM->addOpDecl(lexerBubble); }
			domainRangeAttr		{
						  CM->endMsg();
						}

		|	KW_MSGS opNameList ':' domainRangeAttr
			{
			  CM->endMsg();
			}

		|	KW_CLASS classDecl

		|	KW_SUBCLASS		{ tokensClear(); }
			endSubsortList		{ CM->addSubclassDecl(tokenSequence); }
		
		|	error '.'
		        {
			  //
			  //	Fix things that might be in a bad state due
			  //	to a partially processed declaration.
			  //
			  cleanUpModuleExpression();
			  CM->makeDeclsConsistent();
			}
		|	error END_OF_INPUT
			{
			  fileTable.endModule(lineNumber);
			  IssueWarning(LineNumber(lineNumber) <<
			  ": unexpected end-of-input while parsing module " << QUOTE(CM) << ".");
			  YYABORT;  // rely on the caller to clear up module database and lexer
			}
		|	error CHANGE_FILE
			{
			  fileTable.endModule(lineNumber);
			  IssueWarning(LineNumber(lineNumber) <<
			  ": unexpected end-of-file while parsing module " << QUOTE(CM) << ".");
			  YYABORT;  // rely on the caller to clear up module database and lexer
			}
		;

classDecl	:	sortName
			{
			  CM->addClassDecl($1);
			}
			classDef
			{
			}
		|	tokenDot
			{
			  CM->addClassDecl($1);
			}
		;

classDef	:	expectedDot
		|	'|' cPairList finalPair {}
		;

finalPair	:	attributeName ':' typeName1Dot
			{
			  CM->addAttributePair($1, $3, $3 ? tokenSequence : singleton);
			}
		|	attributeName typeName1Dot
			{
			  CM->addAttributePairNoColon($1, $2, $2 ? tokenSequence : singleton);
			}
		;

cPairList	:	cPairList cPair ','
		|
		;

cPair		:	attributeName ':' typeName1
			{
			  CM->addAttributePair($1, $3, $3 ? tokenSequence : singleton);
			}
		|	attributeName typeName1
			{
			  CM->addAttributePairNoColon($1, $2, $2 ? tokenSequence : singleton);
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
			  IssueWarning(&($1) << ": missing " << QUOTE("->") << " in constant declaration.");
			}
		;

stratDeclKeyword : 	KW_STRAT | KW_DSTRAT ;

stratIdList	: 	stratIdList stratId
		|	stratId
		;

stratId		:	stratName	{ CM->addStratDecl($1); }
		;

stratSignature	:	'@'
			typeName
		|	':'
			typeList
			'@'
			typeName
		;

stratAttributes :	'[' stratAttrList ']'	{}
		|	{}
		;

stratAttrList 	:	KW_METADATA IDENTIFIER
			{
			  CM->setMetadata($2);
			}
		;

skipStrayColon 	:	':'
			{
			  IssueWarning(&($1) << ": skipping stray " << QUOTE(":") << " in operator declaration.");
			}
		|	{}
		;

dra2		:	skipStrayColon rangeAttr
		|	'.'
			{
			  IssueWarning(&($1) << ": missing " << QUOTE("->") << " in operator declaration.");
			}
		|	badType
			{
			  IssueWarning(&($1) << ": missing " << QUOTE("->") << " in operator declaration.");
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
		|	{}
		;

typeName1	:	sortName
			{
			  singleton[0] = $1;
			  $$ = false;
			}
		|	kind
			{
			  $$ = true;
			}
		;
		
typeName1Dot	:	sortName expectedDot
			{
			  singleton[0] = $1;
			  $$ = false;
			}
		|	tokenDot
			{
			  singleton[0] = $1;
			  $$ = false;
			}
		|	kind expectedDot
			{
			  $$ = true;
			}
		;

kind		:	'['			{ tokensClear(); }
			sortNames ']'
		;

typeName	:	sortName
			{
			  singleton[0] = $1;
			  currentSyntaxContainer->addType(false, singleton);
			}
		|	'['			{ tokensClear(); }
			sortNames ']'
			{
			  currentSyntaxContainer->addType(true, tokenSequence);
			}
		;

sortNames	:	sortNames ',' sortName		{ tokensStore($3); }
		|	sortName			{ tokensStore($1); }
		;

attributes	:	'[' attributeList ']'	{}
		|	{}
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
		|	KW_PREC			{ IssueWarning(&($1) << ": prec attribute without value in operator declaration."); }
		|	KW_GATHER '('		{ tokensClear(); }
			idList ')'		{ CM->setGather(tokenSequence); }
		|	KW_GATHER 		{ IssueWarning(&($1) << ": gather attribute without pattern in operator declaration."); }
		|	KW_FORMAT '('		{ tokensClear(); }
			idList ')'		{ CM->setFormat(tokenSequence); }
		|	KW_FORMAT 		{ IssueWarning(&($1) << ": format attribute without specification in operator declaration."); }
		|	KW_STRAT '('		{ tokensClear(); }
			idList ')'		{ CM->setStrat(tokenSequence); }
		|	KW_STRAT 		{ IssueWarning(&($1) << ": strat attribute without strategy in operator declaration."); }
		|	KW_ASTRAT '('		{ tokensClear(); }
			idList ')'		{ CM->setStrat(tokenSequence); }
		|	KW_ASTRAT		{ IssueWarning(&($1) << ": strategy attribute without strategy in operator declaration."); }
		|	KW_POLY '('		{ tokensClear(); }
			idList ')'		{ CM->setPoly(tokenSequence); }
		|	KW_POLY			{ IssueWarning(&($1) << ": poly attribute without specification in operator declaration."); }
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
			  tokensClear();
			  CM->setFrozen(tokenSequence);
			}
		|	KW_FROZEN '('		{ tokensClear(); }
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
		|	KW_PORTAL
			{
			  CM->setFlag(SymbolType::PORTAL);
			}
		|	KW_METADATA IDENTIFIER
			{
			  CM->setMetadata($2);
			}
		|	KW_LATEX '('		{ lexerLatexMode(); }
			LATEX_STRING ')'	{ CM->setLatexMacro($4); }
		|	KW_LATEX		{ IssueWarning(&($1) << ": latex attribute without latex code in operator declaration."); }
		|	KW_SPECIAL '(' hookList ')'	{}
		|	KW_DITTO
			{
			  CM->setFlag(SymbolType::DITTO);
			}
		|	KW_PCONST
			{
			  CM->setFlag(SymbolType::PCONST);
			}
		;

/*
 *	The ony point of this rule is to force a one token lookahead and allow the lexer to grab the
 *	bubble corresponding to the identity. We never see a FORCE_LOOKAHEAD token.
 */
identity	:	FORCE_LOOKAHEAD
		|	{}
		;

idList		:	idList IDENTIFIER	{ tokensStore($2); }
		|	IDENTIFIER		{ tokensStore($1); }
		;

hookList	:	hookList hook		{}
		|	hook	 		{}
		;

hook		:	KW_ID_HOOK token		{ tokensClear(); CM->addHook(SyntacticPreModule::ID_HOOK, $2, tokenSequence); }
		|	KW_ID_HOOK token parenBubble	{ CM->addHook(SyntacticPreModule::ID_HOOK, $2, lexerBubble); }
		|	KW_OP_HOOK token parenBubble	{ CM->addHook(SyntacticPreModule::OP_HOOK, $2, lexerBubble); }
		|	KW_TERM_HOOK token parenBubble	{ CM->addHook(SyntacticPreModule::TERM_HOOK, $2, lexerBubble); }
		;

/*
 *	Recovery from missing tokens
 */
expectedIs	:	KW_IS {}
		|
			{
			  IssueWarning(LineNumber(lineNumber) << ": missing " << QUOTE("is") << " keyword.");
			}
		;

expectedDot	:	'.' {}
		|
			{
			  IssueWarning(LineNumber(lineNumber) << ": missing period.");
			}
		;

/*
 *	Sort lists.
 */
sortNameList	:	sortNameList sortName	{ tokensStore($2); }
		|	sortName     		{ tokensStore($1); }
		;

		
endSortNameList :	tokenDot		{ tokensStore($1); }
	    	|	sortNameList dot
		;

/*
 *	Subsort lists.
 */
subsortList	:	subsortList sortNameList '<'	{ tokensStore($3); }
		|	sortNameList '<'		{ tokensStore($2); }
	    	;

endSubsortList	:	subsortList endSortNameList
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
		
tokenDot	:	ENDS_IN_DOT
			{
			  Token t;
			  t.dropChar($1);
			  missingSpace(t);
			  $$ = t;
			}

/*
 *	Sets of tokens returned in <ID_MODE>.
 */

/*
 *	Single token sort names are the most restricted general class of token.
 *	We exclude
 *	  '(' ')'
 *	for all token types to avoid parentheses balancing issues
 *	  ':' KW_ARROW KW_PARTIAL
 *	to avoid ambiguity in operator declarations
 *	  ':' '@'
 *	to avoid ambiguity in strategy declarations
 *	  '<'
 *	to avoid ambiguity in subsort declarations
 *	  ':' KW_COLON2 KW_ASSIGN KW_ID
 *	because colons are not allowed in sort names (full variable name confusion)
 *	  '.' ENDS_IN_DOT
 *	because periods are not allowed sort names (metalevel constant confusion)
 *	  '[' ',' ']'
 *	to avoid ambiguity in kinds
 *	  '{' ',' '}'
 *	to avoid ambiguity in parameterized sort names
 */
sortToken	:	IDENTIFIER
 
 		|	KW_IMPORT | KW_SORT | KW_SUBSORT | KW_OP | KW_OPS | KW_VAR
		|	KW_STRAT | KW_DSTRAT
		|	KW_CLASS | KW_SUBCLASS | KW_ATTR | KW_MSG | KW_MSGS
		|	KW_MB | KW_CMB | KW_EQ | KW_CEQ | KW_RL | KW_CRL
		|	KW_SD | KW_CSD
		|	KW_ENDM | KW_ENDV

		|	KW_IF | KW_IS | KW_FROM | KW_TO | KW_LABEL

		|	KW_ASSOC | KW_COMM | KW_IDEM | KW_ITER | KW_LEFT | KW_RIGHT
		|	KW_PREC | KW_GATHER | KW_ASTRAT | KW_POLY | KW_MEMO | KW_CTOR
		|	KW_LATEX | KW_SPECIAL | KW_FROZEN | KW_METADATA
		|	KW_CONFIG | KW_OBJ | KW_DITTO | KW_FORMAT
		|	KW_ID_HOOK | KW_OP_HOOK | KW_TERM_HOOK | KW_PCONST
		
		|	'=' | '|' | '+' | '*' |	KW_ARROW2
		;

/*
 *	An unrestricted token adds back these excluded tokens except  '(' ')'
 */
token		:	sortToken | ENDS_IN_DOT | KW_ID 
		|	':' | '@' | '<' | ',' | '.' | '[' | ']' | '{' | '}'
		|	KW_ARROW | KW_PARTIAL | KW_COLON2 | KW_ASSIGN
		;

/*
 *	A strategy name excludes
 *	  ':' '@'
 *	to avoid ambiguity in strategy declarations
 */
stratName	:	sortToken | ENDS_IN_DOT | KW_ID 
		|	'<' | ',' | '.' | '[' | ']' | '{' | '}'
		|	KW_ARROW | KW_PARTIAL | KW_COLON2 | KW_ASSIGN
		;

/*
 *	An attribute name excludes
 *	  ',' '[' ']' '{' '}'
 *	to avoid the formation of illegal operator names
 */
attributeName	:	sortToken | ENDS_IN_DOT | KW_ID 
		|	':' | '@' | '<' | '.'
		|	KW_ARROW | KW_PARTIAL | KW_COLON2 | KW_ASSIGN
		;

/*
 *	A tokenBarColon (used for variable names and single token operator names) excludes
 *	  ':'
 *	to avoid ambiguity in variable declarations and ops declarations.
 */
tokenBarColon	:	sortToken | ENDS_IN_DOT | KW_ID 
		|	'@' | '<' | ',' | '.' | '[' | ']' | '{' | '}'
		|	KW_ARROW | KW_PARTIAL | KW_COLON2 | KW_ASSIGN
		;

/*
 *	For parsing modules expressions we split off
 *	  '.' ENDS_IN_DOT
 *	into a separate class so that M. can be overparsed as M . rather than M. .
 *	and a bare . without a terminating dot is not overparsed as module name.
 */
tokenBarDot	:	sortToken | KW_ID 
		|	':' | '@' | '<' | ',' | '[' | ']' | '{' | '}'
		|	KW_ARROW | KW_PARTIAL | KW_COLON2 | KW_ASSIGN
		;

endsInDot	:	'.' | ENDS_IN_DOT
		;
