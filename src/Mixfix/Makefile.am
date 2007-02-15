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
	-I$(top_srcdir)/src/MSCP10 \
	-I$(top_srcdir)/src/IO_Stuff \
	-I$(top_srcdir)/src/ACU_Persistent \
	-I$(top_srcdir)/src/ACU_Theory \
	-I$(top_srcdir)/src/AU_Persistent \
	-I$(top_srcdir)/src/AU_Theory \
	-I$(top_srcdir)/src/Meta \
	-I$(top_srcdir)/src/3rdParty \
	-I$(top_srcdir)/src/FullCompiler \
	-I$(top_srcdir)/src/StrategyLanguage

libmixfix_a_SOURCES = \
	syntaxContainer.cc \
	renaming.cc \
	view.cc \
	entity.cc \
	moduleCache.cc \
	moduleExpression.cc \
	importModule.cc \
	importTranslation.cc \
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
	banner.cc \
	lexer.cc \
	surface.cc \
	freshVariableSource.cc \
	$(COMPILER_SRC)

EXTRA_libmixfix_a_SOURCES = \
	compiler.cc

BUILT_SOURCES = surface.yy surface.cc surface.h lexer.cc
CLEANFILES = surface.yy surface.cc surface.h lexer.cc

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

surface.cc surface.h: surface.yy
	$(BISON) -dv surface.yy -o surface.c
	mv surface.c surface.cc

#	According to the manual, automake is supposed to grok
#	.ll files and do the right thing... but it doesn't
#	seem to work so...
lexer.cc: \
	$(srcdir)/lexer.ll surface.h
	$(FLEX) -t -p -p $(srcdir)/lexer.ll > lexer.cc

EXTRA_DIST = \
	renameModule.cc \
	parameterization.cc \
	lexerAux.cc \
	import.cc \
	process.cc \
	fixUp.cc \
	ops.cc \
	command.cc \
	entry.cc \
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
	graphPrint.cc \
	ansiEscapeSequences.cc

noinst_HEADERS = \
	renaming.hh \
	view.hh \
	entity.hh \
	moduleCache.hh \
	moduleExpression.hh \
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
	mixfix.hh \
	mixfixModule.hh \
	mixfixParser.hh \
	moduleDatabase.hh \
	viewDatabase.hh \
	preModule.hh \
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
	freshVariableSource.hh
