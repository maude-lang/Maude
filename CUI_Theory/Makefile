ROOT_DIR := ..

include $(ROOT_DIR)/MakeOptions

MODULES =	CUI_Symbol.o \
		CUI_DagNode.o \
		CUI_DagArgumentIterator.o \
		CUI_RhsAutomaton.o \
		CUI_Term.o \
		CUI_ArgumentIterator.o \
		CUI_LhsAutomaton.o

IFLAGS =        -I$(ROOT_DIR)/Core \
                -I$(ROOT_DIR)/Interface \
		-I$(ROOT_DIR)/Variable \
		-I$(ROOT_DIR)/Utility \
		-I$(ROOT_DIR)/CUI_Theory \
		-I$(ROOT_DIR)/FullCompiler

CXXFLAGS =      $(DFLAGS) $(IFLAGS)

libCUI_Theory.a : $(MODULES)
	$(AR) $(ARFLAGS) libCUI_Theory.a $(MODULES)

CUI_Term.o : CUI_LhsCompiler.cc
CUI_LhsAutomaton.o : CUI_Matcher.cc

clean:
	rm *.o *.a
