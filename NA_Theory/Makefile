ROOT_DIR := ..

include $(ROOT_DIR)/MakeOptions

MODULES =	NA_Symbol.o \
		NA_Term.o \
		NA_DagNode.o \
		NA_LhsAutomaton.o \
		NA_RhsAutomaton.o

IFLAGS =        -I$(ROOT_DIR)/Core \
                -I$(ROOT_DIR)/Interface \
		-I$(ROOT_DIR)/NA_Theory \
		-I$(ROOT_DIR)/Utility \
		-I$(ROOT_DIR)/Variable \
		-I$(ROOT_DIR)/FullCompiler

CXXFLAGS =      $(DFLAGS) $(IFLAGS)

libNA_Theory.a : $(MODULES)
	$(AR) $(ARFLAGS) libNA_Theory.a $(MODULES)

clean:
	rm *.o *.a
