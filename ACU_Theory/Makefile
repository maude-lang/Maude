ROOT_DIR := ..

include $(ROOT_DIR)/MakeOptions

MODULES =	ACU_Symbol.o \
		ACU_Term.o \
		ACU_BaseDagNode.o \
		ACU_DagNode.o \
		ACU_TreeDagNode.o \
		ACU_RhsAutomaton.o \
		ACU_LhsAutomaton.o \
		ACU_Subproblem.o \
		ACU_ExtensionInfo.o \
		ACU_CollectorLhsAutomaton.o \
		ACU_NGA_LhsAutomaton.o \
		ACU_VarLhsAutomaton.o \
		ACU_GndLhsAutomaton.o \
		ACU_NonLinearLhsAutomaton.o \
		ACU_ArgumentIterator.o \
		ACU_DagArgumentIterator.o \
		ACU_TreeDagArgumentIterator.o \
		ACU_LazySubproblem.o

IFLAGS =        -I$(ROOT_DIR)/Core \
                -I$(ROOT_DIR)/Interface \
		-I$(ROOT_DIR)/ACU_Theory \
		-I$(ROOT_DIR)/ACU_Persistent \
		-I$(ROOT_DIR)/Utility \
		-I$(ROOT_DIR)/Variable \
		-I$(ROOT_DIR)/FullCompiler

CXXFLAGS =      $(DFLAGS) $(IFLAGS)

libACU_Theory.a : $(MODULES)
	$(AR) $(ARFLAGS) libACU_Theory.a $(MODULES)

ACU_Term.o : ACU_LhsCompiler0.cc ACU_LhsCompiler1.cc ACU_LhsCompiler2.cc \
	ACU_LhsCompiler3.cc

ACU_LhsAutomaton.o : ACU_Matcher.cc ACU_GreedyMatcher.cc \
	ACU_CollapseMatcher.cc ACU_TreeMatcher.cc

ACU_DagNode.o : ACU_DagOperations.cc \
	ACU_FastMerge.cc ACU_Normalize.cc \
	ACU_MergeSort.cc

clean:
	rm *.o *.a
