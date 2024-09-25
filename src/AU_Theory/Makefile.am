noinst_LIBRARIES = libAU_Theory.a

libAU_Theory_a_CPPFLAGS = \
	-I$(top_srcdir)/src/Utility \
	-I$(top_srcdir)/src/Interface \
	-I$(top_srcdir)/src/Core \
	-I$(top_srcdir)/src/Variable \
	-I$(top_srcdir)/src/AU_Persistent \
	-I$(top_srcdir)/src/FullCompiler

libAU_Theory_a_SOURCES = \
	AU_Symbol.cc \
	AU_DagNode.cc \
	AU_DequeDagNode.cc \
	AU_Term.cc \
	AU_DagArgumentIterator.cc \
	AU_DequeDagArgumentIterator.cc \
	AU_ExtensionInfo.cc \
	AU_RhsAutomaton.cc \
	AU_LhsAutomaton.cc \
	AU_ArgumentIterator.cc \
	AU_Layer.cc \
	AU_Subproblem.cc \
	AU_UnificationSubproblem2.cc

EXTRA_DIST = \
	AU_DagOperations.cc \
	AU_Normalize.cc \
	AU_LhsCompiler.cc \
	AU_Matcher.cc \
	AU_CollapseMatcher.cc \
	AU_FullMatcher.cc \
	AU_GreedyMatcher.cc \
	AU_DequeMatcher.cc \
	ChangeLog.A_Theory

noinst_HEADERS = \
	AU_ArgumentIterator.hh \
	AU_BaseDagNode.hh \
	AU_DagArgumentIterator.hh \
	AU_DagNode.hh \
	AU_DequeDagArgumentIterator.hh \
	AU_DequeDagNode.hh \
	AU_ExtensionInfo.hh \
	AU_Layer.hh \
	AU_LhsAutomaton.hh \
	AU_RhsAutomaton.hh \
	AU_Subproblem.hh \
	AU_Symbol.hh \
	AU_Term.hh \
	AU_Theory.hh \
	AU_UnificationSubproblem2.hh
