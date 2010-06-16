noinst_LIBRARIES = libinterface.a

libinterface_a_CPPFLAGS = \
	-I$(top_srcdir)/src/Utility \
	-I$(top_srcdir)/src/Core \
	-I$(top_srcdir)/src/Variable \
	-I$(top_srcdir)/src/FullCompiler

libinterface_a_SOURCES = \
	symbol.cc \
	binarySymbol.cc \
	associativeSymbol.cc \
	term.cc \
	dagNode.cc

noinst_HEADERS = \
	associativeSymbol.hh \
	binarySymbol.hh \
	dagNode.hh \
	extensionInfo.hh \
	interface.hh \
	lhsAutomaton.hh \
	rawArgumentIterator.hh \
	rawDagArgumentIterator.hh \
	rhsAutomaton.hh \
	subproblem.hh \
	unificationSubproblem.hh \
	subproblem.hh \
	symbol.hh \
	symbol2.hh \
	term.hh \
	delayedSubproblem.hh
