noinst_LIBRARIES = libCUI_Theory.a

libCUI_Theory_a_CPPFLAGS = \
	-I$(top_srcdir)/src/Utility \
	-I$(top_srcdir)/src/Interface \
	-I$(top_srcdir)/src/Core \
	-I$(top_srcdir)/src/Variable \
	-I$(top_srcdir)/src/FullCompiler

libCUI_Theory_a_SOURCES = \
	CUI_Symbol.cc \
	CUI_DagNode.cc \
	CUI_DagArgumentIterator.cc \
	CUI_RhsAutomaton.cc \
	CUI_Term.cc \
	CUI_ArgumentIterator.cc \
	CUI_UnificationSubproblem.cc \
	CUI_LhsAutomaton.cc

EXTRA_DIST = \
	CUI_LhsCompiler.cc \
	CUI_Matcher.cc

noinst_HEADERS = \
	CUI_ArgumentIterator.hh \
	CUI_DagArgumentIterator.hh \
	CUI_DagNode.hh \
	CUI_LhsAutomaton.hh \
	CUI_RhsAutomaton.hh \
	CUI_Symbol.hh \
	CUI_Term.hh \
	CUI_UnificationSubproblem.hh \
	CUI_Theory.hh
