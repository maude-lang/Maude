noinst_LIBRARIES = libtemporal.a

libtemporal_a_CPPFLAGS = -I$(top_srcdir)/src/Utility

libtemporal_a_SOURCES = \
	logicFormula.cc \
	buchiAutomaton2.cc \
	transitionSet.cc \
	veryWeakAlternatingAutomaton.cc \
	rawTransitionSet.cc \
	genBuchiAutomaton.cc \
	modelChecker2.cc

EXTRA_DIST = \
	collapseStates.cc \
	sccOptimizations.cc \
	sccAnalysis.cc \
	satSolve.cc

noinst_HEADERS = \
	buchiAutomaton2.hh \
	genBuchiAutomaton.hh \
	logicFormula.hh \
	modelChecker2.hh \
	rawTransitionSet.hh \
	temporal.hh \
	transitionSet.hh \
	veryWeakAlternatingAutomaton.hh
