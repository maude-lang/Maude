ROOT_DIR := ..

include $(ROOT_DIR)/MakeOptions

MODULES =       importModule.o \
		importTranslation.o \
		fileTable.o \
		loopSymbol.o \
		quotedIdentifierSymbol.o \
		quotedIdentifierDagNode.o \
		quotedIdentifierTerm.o \
		quotedIdentifierOpSymbol.o \
		mixfixModule.o \
		lexer.o \
		token.o \
		main.o \
		userLevelRewritingContext.o \
		parser.o \
		preModule.o \
		symbolType.o \
		xmlBuffer.o \
		maudemlBuffer.o \
		interpreter.o \
		moduleDatabase.o \
		profileModule.o \
		mixfixParser.o \
		visibleModule.o \
		commonTokens.o \
		metadataStore.o

#		compiler.o \


IFLAGS =        -I$(ROOT_DIR)/Core \
                -I$(ROOT_DIR)/Interface \
		-I$(ROOT_DIR)/Variable \
		-I$(ROOT_DIR)/Higher \
                -I$(ROOT_DIR)/FreeTheory \
		-I$(ROOT_DIR)/AU_Theory \
		-I$(ROOT_DIR)/ACU_Theory \
		-I$(ROOT_DIR)/CUI_Theory \
		-I$(ROOT_DIR)/S_Theory \
		-I$(ROOT_DIR)/NA_Theory \
		-I$(ROOT_DIR)/Utility \
		-I$(ROOT_DIR)/Temporal \
		-I$(ROOT_DIR)/Mixfix \
		-I$(ROOT_DIR)/BuiltIn \
		-I$(ROOT_DIR)/FullCompiler \
		-I$(ROOT_DIR)/MSCP10 \
		-I$(ROOT_DIR)/Meta \
		-I$(ROOT_DIR)/ObjectSystem \
		-I$(ROOT_DIR)/System


LFLAGS =	-L$(ROOT_DIR)/Core \
		-L$(ROOT_DIR)/Interface \
		-L$(ROOT_DIR)/Variable \
		-L$(ROOT_DIR)/Higher \
                -L$(ROOT_DIR)/FreeTheory \
		-L$(ROOT_DIR)/AU_Theory \
		-L$(ROOT_DIR)/AU_Persistent \
		-L$(ROOT_DIR)/ACU_Theory \
		-L$(ROOT_DIR)/ACU_Persistent \
		-L$(ROOT_DIR)/CUI_Theory \
		-L$(ROOT_DIR)/S_Theory \
		-L$(ROOT_DIR)/NA_Theory \
		-L$(ROOT_DIR)/Utility \
		-L$(ROOT_DIR)/Temporal \
		-L$(ROOT_DIR)/BuiltIn \
		-L$(ROOT_DIR)/Meta \
		-L$(ROOT_DIR)/MSCP10 \
		-L$(ROOT_DIR)/ObjectSystem \
		-L$(ROOT_DIR)/System

#		-L$(ROOT_DIR)/FullCompiler \

OUR_LIBS =	-lmeta \
		-lobjectSystem \
		-lbuiltIn \
		-lhigher \
		-lfreeTheory \
		-lAU_Theory \
		-lAU_Persistent \
		-lACU_Theory \
		-lACU_Persistent \
		-lCUI_Theory \
		-lS_Theory \
		-lNA_Theory \
		-lcore \
		-linterface \
		-lvariable \
		-lcore \
		-linterface \
		-lmscp10 \
		-ltemporal \
		-lsystem \
		-lutility

#		-lfullCompiler \

#  We need the following 3rd party libraries
#  (in addition to those normally linked in by g++):
#	dlmalloc	Doug Lea's malloc (ftp://g.oswego.edu/pub/misc/malloc.c)
#	bdd		BuDDy (http://www.itu.dk/research/buddy/)
#	tecla		Tecla (http://www.astro.caltech.edu/~mcs/tecla/)
#	curses		UNIX library needed by Tecla
#	gmp		GNU GMP (http://www.swox.com/gmp/)
#	gmpxx		C++ bindings for GNU GMP

OTHER_LIBS =	$(DLMALLOC) \
		-lbdd \
		-ltecla \
		-lcurses

LIBRARIES =	$(OUR_LIBS) $(OTHER_LIBS)

#GMP_LIBS =	-lgmpxx \
#		-lgmp

#  Force static linking for GNU GMP.
GMP_LIBS =	$(GMP_LIB_DIR)/libgmpxx.a \
		$(GMP_LIB_DIR)/libgmp.a

#  Compiled without optimized asm. Slow but safe for Purify.
SAFE_GMP_LIBS =	/homes/eker/Solaris/3.2/Safer/lib/libgmpxx.a \
		/homes/eker/Solaris/3.2/Safer/lib/libgmp.a


#SAFE_GMP_LIBS =	/csl/sparc_gmp/lib/libgmpxx.a \
#		/csl/sparc_gmp/lib/libgmp.a

CXXFLAGS =      $(DFLAGS) $(IFLAGS)  -DMOS -DBUBBLES -DSCP
#CXXFLAGS =      $(DFLAGS) $(IFLAGS) -DBUBBLES -DSCP -DPARSER_DEBUG

PURIFY_FLAGS =	-free-queue-length=10000 \
		-free-queue-threshold=1000000 \
		-chain-length="20" \
		-cache-dir=$(CACHE_DIR) \
		-always-use-cache-dir

PURECOV_FLAGS =

maude : $(MODULES) banner.cc
	$(CXX) $(DFLAGS) $(IFLAGS) -c banner.cc -o banner.o
	$(CXX) $(LINKAGE) $(DFLAGS) $(LFLAGS) banner.o $(MODULES) $(LIBRARIES) $(GMP_LIBS) -o maude

maude.purify : $(MODULES) banner.cc
	$(CXX) $(DFLAGS) $(IFLAGS) -c banner.cc -o banner.o
	$(PURIFY) $(PURIFY_FLAGS) \
	$(CXX) $(DFLAGS) $(LFLAGS) banner.o \
	$(MODULES) $(LIBRARIES) $(SAFE_GMP_LIBS) -o maude.purify

#	$(PURECOV) $(PURECOV_FLAGS) \

maude.quantify : $(MODULES) banner.cc
	$(CXX) $(DFLAGS) $(IFLAGS) -c banner.cc -o banner.o
	$(QUANTIFY) -cache-dir=$(CACHE_DIR) \
	$(CXX) $(DFLAGS) $(LFLAGS) banner.o $(MODULES) $(LIBRARIES) $(GMP_LIBS) -o maude.quantify

maude.processing : $(MODULES) banner.cc
	$(CXX) $(CXXFLAGS) -DQUANTIFY_PROCESSING -I$(QUANTIFY_INCLUDE) \
		-c preModule.cc -o preModule.o
	$(CXX) $(DFLAGS) $(IFLAGS) -c banner.cc -o banner.o
	$(QUANTIFY) -record-data=no -cache-dir=$(CACHE_DIR) \
	$(CXX) $(DFLAGS) $(LFLAGS) banner.o $(MODULES) $(LIBRARIES) $(GMP_LIBS) -o maude.processing

maude.rewriting : $(MODULES) banner.cc
	$(CXX) $(CXXFLAGS) -DQUANTIFY_REWRITING -I$(QUANTIFY_INCLUDE) \
		-c interpreter.cc -o interpreter.o
	$(CXX) $(DFLAGS) $(IFLAGS) -c banner.cc -o banner.o
	$(QUANTIFY) -record-data=no -cache-dir=$(CACHE_DIR) \
	$(CXX) $(DFLAGS) $(LFLAGS) banner.o $(MODULES) $(LIBRARIES) $(GMP_LIBS) -o maude.rewriting

bubble.o lexer.o : parser.cc.h

parser.cc parser.cc.h : top.y modules.y commands.y bottom.y
	cat top.y modules.y commands.y bottom.y > tmp.y
	$(BISON) -dv tmp.y -o parser.cc

#	$(BISON) -dtv tmp.y -o parser.cc

lexer.cc : lexer.flex
	$(FLEX) -t -pp lexer.flex > lexer.cc

lexer.o : lexerAux.hh lexerAux.cc

preModule.o : import.cc process.cc fixUp.cc ops.cc command.cc

mixfixModule.o : entry.cc prettyPrint.cc bufferPrint.cc \
	specialSymbolTypes.cc doParse.cc makeGrammar.cc \
	termPrint.cc dagNodePrint.cc sharedPrint.cc

userLevelRewritingContext.o : interact.cc trial.cc

interpreter.o : execute.cc loopMode.cc search.cc match.cc

clean:
	rm *.o lexer.cc parser.cc parser.cc.h
