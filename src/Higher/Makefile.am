noinst_LIBRARIES = libhigher.a

libhigher_a_CPPFLAGS = \
	-I$(top_srcdir)/src/Utility \
	-I$(top_srcdir)/src/Interface \
	-I$(top_srcdir)/src/Core \
	-I$(top_srcdir)/src/Variable \
	-I$(top_srcdir)/src/FullCompiler \
	-I$(top_srcdir)/src/Temporal \
	-I$(top_srcdir)/src/FreeTheory \
	-I$(top_srcdir)/src/NA_Theory \
	-I$(top_srcdir)/src/BuiltIn \
	-I$(top_srcdir)/src/Mixfix

libhigher_a_SOURCES = \
	rewriteSearchState.cc \
	matchSearchState.cc \
	positionState.cc \
	pattern.cc \
	searchState.cc \
	rewriteSequenceSearch.cc \
	modelCheckerSymbol.cc \
	stateTransitionGraph.cc \
	rewriteConditionFragment.cc \
	assignmentConditionFragment.cc \
	sortTestConditionFragment.cc \
	equalityConditionFragment.cc \
	assignmentConditionState.cc \
	rewriteConditionState.cc \
	temporalSymbol.cc \
	satSolverSymbol.cc \
	unificationProblem.cc \
	narrowingSearchState.cc \
	narrowingUnificationProblem.cc \
	narrowingSequenceSearch.cc \
	unifierFilter.cc \
	variantNarrowingSearchState.cc \
	variantFolder.cc \
	variantSearch.cc

noinst_HEADERS = \
	assignmentConditionFragment.hh \
	assignmentConditionState.hh \
	equalityConditionFragment.hh \
	higher.hh \
	matchSearchState.hh \
	modelCheckerSymbol.hh \
	pattern.hh \
	positionState.hh \
	rewriteConditionFragment.hh \
	rewriteConditionState.hh \
	rewriteSearchState.hh \
	rewriteSequenceSearch.hh \
	satSolverSymbol.hh \
	searchState.hh \
	sortTestConditionFragment.hh \
	stateTransitionGraph.hh \
	temporalSymbol.hh \
	unificationProblem.hh \
	narrowingSearchState.hh \
	narrowingUnificationProblem.hh \
	narrowingSequenceSearch.hh \
	sequenceSearch.hh \
	cacheableState.hh \
	unifierFilter.hh \
	variantNarrowingSearchState.hh \
	variantFolder.hh \
	variantSearch.hh
