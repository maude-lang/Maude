ROOT_DIR := ..

include $(ROOT_DIR)/MakeOptions

MODULES =       metaModuleCache.o \
		metaLevel.o \
		metaOpCache.o \
		metaModule.o \
		metaLevelOpSymbol.o

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
		-I$(ROOT_DIR)/ObjectSystem \
		-I$(ROOT_DIR)/Mixfix \
		-I$(ROOT_DIR)/FullCompiler \
		-I$(ROOT_DIR)/BuiltIn \
		-I$(ROOT_DIR)/MSCP10 \
		-I$(ROOT_DIR)/System \
		-I$(ROOT_DIR)/Meta

CXXFLAGS =      $(DFLAGS) $(IFLAGS) -DSCP

libmeta.a : $(MODULES)
	$(AR) $(ARFLAGS) libmeta.a $(MODULES)

metaLevel.o : metaDown.cc metaDownOps.cc metaDownFixUps.cc metaLevelSignature.cc metaUp.cc \
	metaUpModule.cc

metaLevelOpSymbol.o : descentFunctions.cc ascentFunctions.cc metaMatch.cc metaSearch.cc

clean:
	rm *.o *.a
