ROOT_DIR := ..

include $(ROOT_DIR)/MakeOptions

MODULES =	memoryCell.o \
		rhsBuilder.o \
		termBag.o \
		bindingLhsAutomaton.o \
		trivialRhsAutomaton.o \
		copyRhsAutomaton.o \
		variableInfo.o \
		termSet.o \
		strategy.o \
		sortTable.o \
		sortConstraintTable.o \
		equationTable.o \
		ruleTable.o \
		sort.o \
		connectedComponent.o \
		module.o \
		substitution.o \
		localBinding.o \
		subproblemSequence.o \
		subproblemDisjunction.o \
		sortCheckSubproblem.o \
		disjunctiveSubproblemAccumulator.o \
		preEquation.o \
		equation.o \
		rule.o \
		sortConstraint.o \
		rewritingContext.o \
		variableAbstractionSubproblem.o \
		equalitySubproblem.o \
		dagRoot.o \
		cachedDag.o \
		extensionMatchSubproblem.o \
		dagNodeSet.o \
		memoTable.o \
		rootContainer.o

IFLAGS =	-I$(ROOT_DIR)/Utility \
		-I$(ROOT_DIR)/Core \
		-I$(ROOT_DIR)/Interface \
		-I$(ROOT_DIR)/Variable \
		-I$(ROOT_DIR)/FullCompiler

CXXFLAGS =	$(DFLAGS) $(IFLAGS)

libcore.a : $(MODULES)
	$(AR) $(ARFLAGS) libcore.a $(MODULES)

rewritingContext.o : run.cc

sortTable.o : ctorDiagram.cc

clean:
	rm *.o *.a
