ROOT_DIR := ..

include $(ROOT_DIR)/MakeOptions

MODULES =	rewriteSearchState.o \
		matchSearchState.o \
		positionState.o \
		pattern.o \
		searchState.o \
		rewriteSequenceSearch.o \
		modelCheckerSymbol.o \
		stateTransitionGraph3.o \
		rewriteConditionFragment.o \
		assignmentConditionFragment.o \
		sortTestConditionFragment.o \
		equalityConditionFragment.o \
		assignmentConditionState.o \
		rewriteConditionState.o \
		temporalSymbol.o \
		satSolverSymbol.o

IFLAGS =	-I$(ROOT_DIR)/Utility \
		-I$(ROOT_DIR)/Core \
		-I$(ROOT_DIR)/Temporal \
		-I$(ROOT_DIR)/Interface \
		-I$(ROOT_DIR)/Variable \
		-I$(ROOT_DIR)/FreeTheory \
		-I$(ROOT_DIR)/BuiltIn \
		-I$(ROOT_DIR)/FullCompiler \
		-I$(ROOT_DIR)/Higher \
		-I$(ROOT_DIR)/NA_Theory \
		-I$(ROOT_DIR)/Mixfix

CXXFLAGS =	$(DFLAGS) $(IFLAGS)

libhigher.a : $(MODULES)
	$(AR) $(ARFLAGS) libhigher.a $(MODULES)

clean:
	rm *.o *.a
