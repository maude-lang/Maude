ROOT_DIR := ..

include $(ROOT_DIR)/MakeOptions

MODULES =	S_Symbol.o \
		S_DagNode.o \
		S_Term.o \
		S_ExtensionInfo.o \
		S_DagArgumentIterator.o \
		S_ArgumentIterator.o \
		S_LhsAutomaton.o \
		S_RhsAutomaton.o \
		S_Subproblem.o

IFLAGS =        -I$(ROOT_DIR)/Core \
                -I$(ROOT_DIR)/Interface \
		-I$(ROOT_DIR)/Variable \
		-I$(ROOT_DIR)/S_Theory \
		-I$(ROOT_DIR)/Utility \
		-I$(ROOT_DIR)/FullCompiler


CXXFLAGS =      $(DFLAGS) $(IFLAGS)

libS_Theory.a : $(MODULES)
	$(AR) rus libS_Theory.a $(MODULES)

clean:
	rm *.o *.a
