ROOT_DIR := ..

include $(ROOT_DIR)/MakeOptions

MODULES =	logicFormula.o \
		buchiAutomaton2.o \
		transitionSet.o \
		veryWeakAlternatingAutomaton.o \
		rawTransitionSet.o \
		genBuchiAutomaton.o \
		modelChecker2.o

#		parityGame.o

IFLAGS =	-I$(ROOT_DIR)/Utility

#IFLAGS =	-I$(GMP_INCLUDE) \
#		-I$(ROOT_DIR)/Utility

CXXFLAGS =	$(DFLAGS) $(IFLAGS)

libtemporal.a : $(MODULES)
	$(AR) $(ARFLAGS) libtemporal.a $(MODULES)

genBuchiAutomaton.o : collapseStates.cc sccOptimizations.cc sccAnalysis.cc satSolve.cc

clean:
	rm *.o *.a
