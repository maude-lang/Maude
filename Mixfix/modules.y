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
 *	Modules.
 */
module		:	KW_FMOD			{ lexerIdMode(); }
			token KW_IS
			{
			  interpreter.
			    setCurrentModule(new PreModule($3, MixfixModule::FUNCTIONAL_MODULE));
			  fileTable.beginModule($1, $3);
			}
			fDecList endfm
			{
			  lexerInitialMode();
			  fileTable.endModule(lineNumber);
			  CM->finishModule();
			}
		|	KW_MOD			{ lexerIdMode(); }
			token KW_IS
			{
			  interpreter.
			    setCurrentModule(new PreModule($3, MixfixModule::SYSTEM_MODULE));
			  fileTable.beginModule($1, $3);
			}
			decList endm
			{
			  lexerInitialMode();
			  fileTable.endModule(lineNumber);
			  CM->finishModule();
			}
		|	KW_OMOD			{ lexerIdMode(); }
			token KW_IS
			{
			  interpreter.
			    setCurrentModule(new PreModule($3, MixfixModule::SYSTEM_MODULE));
			  fileTable.beginModule($1, $3);
			}
			oDecList endom
			{
			  lexerInitialMode();
			  fileTable.endModule(lineNumber);
			  CM->finishModule();
			}
		;

endfm		:	KW_ENDFM	{}
		|	notEndfm
			{
			  IssueWarning(LineNumber(lineNumber) <<
			               ": fmod ended by " << $1 << '.');
			}
		;

notEndfm	:	KW_ENDM | KW_ENDOM
		;

endm		:	KW_ENDM	{}
		|	notEndm
			{
			  IssueWarning(LineNumber(lineNumber) <<
			               ": mod ended by " << $1 << '.');
			}
		;

notEndm		:	KW_ENDFM | KW_ENDOM
		;

endom		:	KW_ENDOM	{}
		|	notEndom
			{
			  IssueWarning(LineNumber(lineNumber) <<
			               ": omod ended by " << $1 << '.');
			}
		;

notEndom	:	KW_ENDFM | KW_ENDM
		;

fDecList	:	fDecList fDeclaration
		|
		;

decList		:	decList declaration
		|
		;

oDecList	:	oDecList oDeclaration
		|
		;

fDeclaration	:	KW_IMPORT		{ clear(); store($1); }
			endTokens '.'		{ CM->addImport(bubble); }

/*
 *	Now that '.' is not a valid sort name we can probably get rid of
 *	endTokens2 and noTrailingDot2
 */
		|	KW_SORT			{ clear(); }
			endTokens2 '.'		{ CM->addSortDecl(bubble); }

		|	KW_SUBSORT token	{ clear(); store($2); }
			listBarLt '<'		{ store($5); }
			endTokens2 '.'		{ CM->addSubsortDecl(bubble); }

		|	KW_OP opName domainRangeAttr {}

		|	KW_OPS opNameList domainRangeAttr {}

		|	KW_VAR varNameList ':' type '.' {}

		|	KW_MB			{ clear(); store($1); }
			tokensBarColon ':'	{ store($4); }
			endTokens '.'		{ CM->addStatement(bubble); }

		|	KW_CMB			{ clear(); store($1); }
			tokensBarColon ':'	{ store($4); }
			tokensBarIf KW_IF	{ store($7); }
			endTokens '.'		{ CM->addStatement(bubble); }

		|	KW_EQ			{ clear(); store($1); }
			tokensBarEqual '='	{ store($4); }
			endTokens '.'		{ CM->addStatement(bubble); }

		|	KW_CEQ			{ clear(); store($1); }
			tokensBarEqual '='	{ store($4); }
			tokensBarIf KW_IF		{ store($7); }
			endTokens '.'		{ CM->addStatement(bubble); }

		|	error '.' 		{ CM->makeOpDeclsConsistent(); }
		;

declaration	:	fDeclaration
		|	KW_RL			{ clear(); store($1); }
			tokensBarArrow2 KW_ARROW2  { store($4); }
			endTokens '.'		{ CM->addStatement(bubble); }

		|	KW_CRL			{ clear(); store($1); }
			tokensBarArrow2 KW_ARROW2	{ store($4); }
			tokensBarIf KW_IF		{ store($7); }
			endTokens '.'		{ CM->addStatement(bubble); }
		;

oDeclaration	:	declaration
		|	KW_MSG opName domainRangeAttr
			{
			  CM->setFlag(SymbolType::MESSAGE);
			}
		|	KW_MSGS opNameList domainRangeAttr
			{
			  CM->setFlag(SymbolType::MESSAGE);
			}
		|	KW_CLASS token
			{
			}
			classDef '.'
			{
			}
		|	KW_SUBCLASS token	{ clear(); store($2); }
			listBarLt '<'		{ store($5); }
			endTokens2 '.'		{ CM->addSubsortDecl(bubble); }
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

varNameList	:	varNameList tokenBarColon { CM->addVarDecl($2); }
		|	tokenBarColon		{ CM->addVarDecl($1); }
		;

opName		:	token			{ clear(); store($1); }
			tokensBarColon		{ CM->addOpDecl(bubble); }
		|	'(' 			{ clear(); }
			tokens ')'		{ CM->addOpDecl(bubble); }
		;

opNameList	:	opNameList simpleOpName
		|	simpleOpName
		;

simpleOpName	:	tokenBarColon
			{
			  clear();
			  store($1);
			  CM->addOpDecl(bubble);
			}
		|	'('			{ clear(); }
			tokens ')'		{ CM->addOpDecl(bubble); }
		;

domainRangeAttr	:	':' typeList arrow type attributes '.'
			{
			  if ($3)
			    CM->convertSortsToKinds();
			}
		;

arrow		:	KW_ARROW      		{ $$ = false; }
		|	KW_PARTIAL	       	{ $$ = true; }
		;

typeList	:	typeList type
		|
		;

type		:	sortToken
			{
			  clear();
			  bubble.append($1);
			  CM->addType(false, bubble);
			}
		|	'['			{ clear(); }
			sortTokens ']'
			{
			  CM->addType(true, bubble);
			}
		;

sortTokens	:	sortTokens ',' sortToken	{ store($3); }
		|	sortToken			{ store($1); }
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
		|	idKeyword		{ clear(); }
			identity		{ CM->setIdentity(bubble); }
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
			idList ')'		{ CM->setGather(bubble); }
		|	KW_FORMAT '('		{ clear(); }
			idList ')'		{ CM->setFormat(bubble); }
		|	KW_STRAT '('		{ clear(); }
			idList ')'		{ CM->setStrat(bubble); }
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
			  CM->setFrozen(bubble);
			}
		|	KW_FROZEN '('		{ clear(); }
			idList ')'		{ CM->setFrozen(bubble); }
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
		|	KW_LATEX '('		{ lexerLatexMode(); }
			LATEX_STRING ')'	{ CM->setLatexMacro($4); }
		|	KW_SPECIAL '(' hookList ')'	{}
		|	KW_DITTO
			{
			  CM->setFlag(SymbolType::DITTO);
			}
		;

idList		:	idList IDENTIFIER	{ store($2); }
		|	IDENTIFIER		{ store($1); }
		;

hookList	:	hookList hook
		|	hook
		;

hook		:	KW_ID_HOOK token
			{
			  clear();
			  CM->addHook(PreModule::ID_HOOK, $2, bubble);
			}
		|	KW_ID_HOOK token '(' 	{ clear(); }
			tokens ')'
			{
			  CM->addHook(PreModule::ID_HOOK, $2, bubble);
			}
		|	KW_OP_HOOK token '('	{ clear(); }
			tokens ')'
			{
			  CM->addHook(PreModule::OP_HOOK, $2, bubble);
			}
		|	KW_TERM_HOOK token '('	{ clear(); }
			tokens ')'
			{
			  CM->addHook(PreModule::TERM_HOOK, $2, bubble);
			}
		;

/*
 *	Token lists.
 */
listBarLt	:	listBarLt tokenBarLt	{ store($2); }
		|
		;

endTokens2	:	noTrailingDot2
		|	endTokens2 '.'		{ store($2); }
		|	'.'			{ store($1); }
		;

noTrailingDot2	:	noTrailingDot2 startKeyword	{ store($2); }
		|	endTokens2 endToken	{ store($2); }
		|	tokenBarDot		{ store($1); }
		;

/*
 *	Token trees.
 */
tokens		:	tokens '('		{ store($2); }
			tokens ')'		{ store($5); }
		|	tokens token		{ store($2); }
		|
		;

tokensBarColon	:	tokensBarColon '('	{ store($2); }
			tokens ')'		{ store($5); }
		|	tokensBarColon tokenBarColon	{ store($2); }
		|
		;

tokensBarEqual	:	tokensBarEqual '('	{ store($2); }
			tokens ')'		{ store($5); }
		|	tokensBarEqual tokenBarEqual	{ store($2); }
		|
		;

tokensBarArrow2	:	tokensBarArrow2 '('	{ store($2); }
			tokens ')'		{ store($5); }
		|	tokensBarArrow2 tokenBarArrow2	{ store($2); }
		|
		;

tokensBarIf	:	tokensBarIf '('		{ store($2); }
			tokens ')'		{ store($5); }
		|	tokensBarIf tokenBarIf	{ store($2); }
		|
		;

endTokens	:	noTrailingDot
		|	endTokens '.'		{ store($2); }
		|	'.'			{ store($1); }
		;

noTrailingDot	:	'('			{ store($1); }
			tokens ')'		{ store($4); }
		|	endTokens '('		{ store($2); }
			tokens ')'		{ store($5); }
		|	noTrailingDot startKeyword	{ store($2); }
		|	endTokens endToken	{ store($2); }
		|	tokenBarDot		{ store($1); }
		;

/*
 *	Special trees
 */
identity	:	identityChunk		{ store($1); }
		|	identity identityChunk	{ store($2); }
		;

identityChunk	:	identifier | startKeyword2 | midKeyword | '.'
		|	'('			{ store($1); }
			tokens ')'		{ $$ = $4; }
		;

/*
 *	Token types.
 */
token		:	identifier | startKeyword | midKeyword | attrKeyword | '.'
		;

tokenBarDot	:	identifier | startKeyword | midKeyword | attrKeyword
		;

endToken	:	identifier | midKeyword | attrKeyword
		;

tokenBarArrow2	:	identifier | startKeyword | attrKeyword | '.'
		|	'<' | ':' | KW_ARROW | KW_PARTIAL | '=' | KW_IF | KW_IS
		;
tokenBarEqual	:	identifier | startKeyword | attrKeyword | '.'
		|	'<' | ':' | KW_ARROW | KW_PARTIAL | KW_ARROW2 | KW_IF | KW_IS
		;
tokenBarIf	:	identifier | startKeyword | attrKeyword | '.'
		|	'<' | ':' | KW_ARROW | KW_PARTIAL | '=' | KW_ARROW2 | KW_IS
		;
tokenBarColon	:	identifier | startKeyword | attrKeyword | '.'
		|	'<' | KW_ARROW | KW_PARTIAL | '=' | KW_ARROW2 | KW_IF | KW_IS
		;
tokenBarLt	:	identifier | startKeyword | attrKeyword | '.'
		|	':' | KW_ARROW | KW_PARTIAL | '=' | KW_ARROW2 | KW_IF | KW_IS
		;

sortToken	:	identifier | startKeyword | attrKeyword2 | '='
		|	KW_ARROW2 | KW_IF | KW_IS
		;

/*
 *	Keywords (in id mode).
 */
identifier	:	IDENTIFIER | ',' | '|'
		;

startKeyword	:	KW_MSG | startKeyword2
		;

startKeyword2	:	KW_IMPORT | KW_SORT | KW_SUBSORT | KW_OP | KW_OPS | KW_VAR
		|	KW_MSGS | KW_CLASS | KW_SUBCLASS
		|	KW_MB | KW_CMB | KW_EQ | KW_CEQ | KW_RL | KW_CRL
		|	KW_ENDFM | KW_ENDM | KW_ENDOM
		;

midKeyword	:	'<' | ':' | KW_ARROW | KW_PARTIAL | '=' | KW_ARROW2 | KW_IF | KW_IS
		;

attrKeyword	:	'[' | ']' | attrKeyword2
		;

attrKeyword2	:	KW_ASSOC | KW_COMM | KW_ID | KW_IDEM | KW_ITER | KW_LEFT | KW_RIGHT
		|	KW_PREC | KW_GATHER | KW_STRAT | KW_MEMO | KW_CTOR
		|	KW_LATEX | KW_SPECIAL | KW_FROZEN
		|	KW_CONFIG | KW_OBJ | KW_DITTO | KW_FORMAT
		|	KW_ID_HOOK | KW_OP_HOOK | KW_TERM_HOOK
		;
