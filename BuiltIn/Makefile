ROOT_DIR := ..

include $(ROOT_DIR)/MakeOptions

MODULES =       equalitySymbol.o \
		sortTestSymbol.o \
		branchSymbol.o \
		stringSymbol.o \
		stringTerm.o \
		stringDagNode.o \
		stringOpSymbol.o \
		floatSymbol.o \
		floatTerm.o \
		floatDagNode.o \
		floatOpSymbol.o \
		succSymbol.o \
		numberOpSymbol.o \
		minusSymbol.o \
		ACU_NumberOpSymbol.o \
		CUI_NumberOpSymbol.o \
		divisionSymbol.o

IFLAGS =        -I$(ROOT_DIR)/Core \
                -I$(ROOT_DIR)/Interface \
                -I$(ROOT_DIR)/FreeTheory \
		-I$(ROOT_DIR)/BuiltIn \
		-I$(ROOT_DIR)/Utility \
		-I$(ROOT_DIR)/Variable \
		-I$(ROOT_DIR)/NA_Theory \
		-I$(ROOT_DIR)/ACU_Persistent \
		-I$(ROOT_DIR)/ACU_Theory \
		-I$(ROOT_DIR)/CUI_Theory \
		-I$(ROOT_DIR)/S_Theory \
		-I$(ROOT_DIR)/FullCompiler

CXXFLAGS =      $(DFLAGS) $(IFLAGS)

libbuiltIn.a : $(MODULES)
	$(AR) $(ARFLAGS) libbuiltIn.a $(MODULES)

clean:
	rm *.o *.a
