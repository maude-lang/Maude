noinst_LIBRARIES = libmixfix.a

if BUILD_COMPILER
  COMPILER_SRC = compiler.cc
endif

libmixfix_a_CPPFLAGS = \
	-I$(top_srcdir)/src/Utility \
	-I$(top_srcdir)/src/Temporal \
	-I$(top_srcdir)/src/Interface \
	-I$(top_srcdir)/src/Core \
	-I$(top_srcdir)/src/Variable \
	-I$(top_srcdir)/src/FullCompiler \
	-I$(top_srcdir)/src/Higher \
	-I$(top_srcdir)/src/CUI_Theory \
	-I$(top_srcdir)/src/S_Theory \
	-I$(top_srcdir)/src/NA_Theory \
	-I$(top_srcdir)/src/FreeTheory \
	-I$(top_srcdir)/src/ObjectSystem \
	-I$(top_srcdir)/src/Mixfix \
	-I$(top_srcdir)/src/BuiltIn \
	-I$(top_srcdir)/src/Parser \
	-I$(top_srcdir)/src/IO_Stuff \
	-I$(top_srcdir)/src/ACU_Persistent \
	-I$(top_srcdir)/src/ACU_Theory \
	-I$(top_srcdir)/src/AU_Persistent \
	-I$(top_srcdir)/src/AU_Theory \
	-I$(top_srcdir)/src/Meta \
	-I$(top_srcdir)/src/3rdParty \
	-I$(top_srcdir)/src/FullCompiler \
	-I$(top_srcdir)/src/StrategyLanguage \
	-I$(top_srcdir)/src/SMT

libmixfix_a_SOURCES = \
	parameterDatabase.cc \
	syntaxContainer.cc \
	renaming.cc \
	view.cc \
	syntacticView.cc \
	entity.cc \
	moduleCache.cc \
	moduleExpression.cc \
	viewExpression.cc \
	viewCache.cc \
	enclosingObject.cc \
	importModule.cc \
	importTranslation.cc \
	strategyTranslation.cc \
	fileTable.cc \
	loopSymbol.cc \
	quotedIdentifierSymbol.cc \
	quotedIdentifierDagNode.cc \
	quotedIdentifierTerm.cc \
	quotedIdentifierOpSymbol.cc \
	mixfixModule.cc \
	token.cc \
	global.cc \
	userLevelRewritingContext.cc \
	preModule.cc \
	syntacticPreModule.cc \
	symbolType.cc \
	xmlBuffer.cc \
	maudemlBuffer.cc \
	interpreter.cc \
	moduleDatabase.cc \
	viewDatabase.cc \
	profileModule.cc \
	mixfixParser.cc \
	visibleModule.cc \
	sharedTokens.cc \
	metadataStore.cc \
	printAttribute.cc \
	banner.cc \
	lexer.cc \
	tokenizer.cc \
	deserializer.cc \
	surface.cc \
	freshVariableSource.cc \
	variableGenerator.cc \
	objectConstructorSymbol.cc \
	$(COMPILER_SRC)

EXTRA_libmixfix_a_SOURCES = \
	compiler.cc

BUILT_SOURCES = surface.yy surface.cc surface.h lexer.cc tokenizer.cc deserializer.cc
CLEANFILES = surface.yy surface.cc surface.h lexer.cc tokenizer.cc deserializer.cc

surface.yy: \
	$(srcdir)/top.yy \
	$(srcdir)/modules.yy \
	$(srcdir)/commands.yy \
	$(srcdir)/bottom.yy
	cat \
	$(srcdir)/top.yy \
	$(srcdir)/modules.yy \
	$(srcdir)/commands.yy \
	$(srcdir)/bottom.yy \
	> surface.yy

surface.cc: surface.yy
	$(BISON) -dv surface.yy -o surface.c
	mv surface.c surface.cc
	touch surface.h

surface.h: surface.cc

#	According to the manual, automake is supposed to grok
#	.ll files and do the right thing... but it doesn't
#	seem to work so...
lexer.cc: \
	$(srcdir)/lexer.ll surface.h
	$(FLEX) -t -p -p $(srcdir)/lexer.ll > lexer.cc

tokenizer.cc: \
	$(srcdir)/tokenizer.ll
	$(FLEX) -Pzz -t -p -p $(srcdir)/tokenizer.ll > tokenizer.cc

deserializer.cc: \
	$(srcdir)/deserializer.ll
	$(FLEX) -Pxx -t -p -p $(srcdir)/deserializer.ll > deserializer.cc

EXTRA_DIST = \
	renameModule.cc \
	parameterization.cc \
	instantiateModuleWithFreeParameters.cc \
	instantiateModuleWithBoundParameters.cc \
	instantiateViewWithFreeParameters.cc \
	instantiateViewWithBoundParameters.cc \
	lexerAux.cc \
	deserializerAux.cc \
	ooSorts.cc \
	ooProcess.cc \
	ooTransform.cc \
	ooView.cc \
	ooRenaming.cc \
	process.cc \
	fixUp.cc \
	ops.cc \
	command.cc \
	entry.cc \
	fancySymbols.cc \
	validateAttributes.cc \
	prettyPrint.cc \
	bufferPrint.cc \
	specialSymbolTypes.cc \
	specialTokens.cc \
	doParse.cc \
	makeGrammar.cc \
	auxProperty.cc \
	termPrint.cc \
	dagNodePrint.cc \
	sharedPrint.cc \
	strategyPrint.cc \
	interact.cc \
	trial.cc \
	execute.cc \
	loopMode.cc \
	search.cc \
	srewrite.cc \
	erewrite.cc \
	match.cc \
	unify.cc \
	top.yy \
	modules.yy \
	commands.yy \
	bottom.yy \
	lexer.ll \
	tokenizer.ll \
	deserializer.ll \
	graphPrint.cc \
	ansiEscapeSequences.cc \
	narrowing.cc \
	variantUnify.cc \
	variantMatch.cc \
	getVariants.cc \
	smtSearch.cc \
	cvc4_Bindings.cc \
	yices2_Bindings.cc \
	quotedIdentifierOpSignature.cc \
	serialize.cc

noinst_HEADERS = \
	renaming.hh \
	view.hh \
	syntacticView.hh \
	entity.hh \
	moduleCache.hh \
	moduleExpression.hh \
	viewCache.hh \
	viewExpression.hh \
	sharedTokens.hh \
	compiler.hh \
	fileTable.hh \
	importModule.hh \
	importTranslation.hh \
	interpreter.hh \
	lexerAux.hh \
	loopSymbol.hh \
	global.hh \
	maudemlBuffer.hh \
	metadataStore.hh \
	printAttribute.hh \
	mixfix.hh \
	mixfixModule.hh \
	mixfixParser.hh \
	moduleDatabase.hh \
	viewDatabase.hh \
	preModule.hh \
	syntacticPreModule.hh \
	profileModule.hh \
	quotedIdentifierDagNode.hh \
	quotedIdentifierOpSymbol.hh \
	quotedIdentifierSymbol.hh \
	quotedIdentifierTerm.hh \
	symbolType.hh \
	token.hh \
	userLevelRewritingContext.hh \
	visibleModule.hh \
	xmlBuffer.hh \
	syntaxContainer.hh \
	freshVariableSource.hh \
	variableGenerator.hh \
	cacheableRewritingContext.hh \
	cvc4_Bindings.hh \
	yices2_Bindings.hh \
	enclosingObject.hh \
	argument.hh \
	parameter.hh \
	parameterDatabase.hh \
	objectConstructorSymbol.hh \
	statementTransformer.hh
