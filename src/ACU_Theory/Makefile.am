noinst_LIBRARIES = libACU_Theory.a

libACU_Theory_a_CPPFLAGS = \
	-I$(top_srcdir)/src/Utility \
	-I$(top_srcdir)/src/Interface \
	-I$(top_srcdir)/src/Core \
	-I$(top_srcdir)/src/Variable \
	-I$(top_srcdir)/src/ACU_Persistent \
	-I$(top_srcdir)/src/FullCompiler

libACU_Theory_a_SOURCES = \
	ACU_Symbol.cc \
	ACU_Term.cc \
	ACU_BaseDagNode.cc \
	ACU_DagNode.cc \
	ACU_TreeDagNode.cc \
	ACU_RhsAutomaton.cc \
	ACU_LhsAutomaton.cc \
	ACU_Subproblem.cc \
	ACU_ExtensionInfo.cc \
	ACU_CollectorLhsAutomaton.cc \
	ACU_NGA_LhsAutomaton.cc \
	ACU_VarLhsAutomaton.cc \
	ACU_GndLhsAutomaton.cc \
	ACU_NonLinearLhsAutomaton.cc \
	ACU_ArgumentIterator.cc \
	ACU_DagArgumentIterator.cc \
	ACU_TreeDagArgumentIterator.cc \
	ACU_LazySubproblem.cc \
	ACU_UnificationSubproblem2.cc \
	ACU_BndVarLhsAutomaton.cc \
	ACU_TreeVariableSubproblem.cc

EXTRA_DIST = \
	ACU_LhsCompiler0.cc \
	ACU_LhsCompiler1.cc \
	ACU_LhsCompiler2.cc \
	ACU_LhsCompiler3.cc \
	ACU_Matcher.cc \
	ACU_GreedyMatcher.cc \
	ACU_CollapseMatcher.cc \
	ACU_TreeMatcher.cc \
	ACU_DagOperations.cc \
	ACU_FastMerge.cc \
	ACU_Normalize.cc \
	ACU_MergeSort.cc \
	ChangeLog.AC_Theory

noinst_HEADERS = \
	ACU_ArgumentIterator.hh \
	ACU_BaseDagNode.hh \
	ACU_CollectorLhsAutomaton.hh \
	ACU_DagArgumentIterator.hh \
	ACU_DagNode.hh \
	ACU_ExtensionInfo.hh \
	ACU_GndLhsAutomaton.hh \
	ACU_LazySubproblem.hh \
	ACU_LhsAutomaton.hh \
	ACU_NGA_LhsAutomaton.hh \
	ACU_NonLinearLhsAutomaton.hh \
	ACU_RhsAutomaton.hh \
	ACU_Subproblem.hh \
	ACU_Symbol.hh \
	ACU_Term.hh \
	ACU_Theory.hh \
	ACU_TreeDagArgumentIterator.hh \
	ACU_TreeDagNode.hh \
	ACU_VarLhsAutomaton.hh \
	ACU_UnificationSubproblem2.hh \
	ACU_BndVarLhsAutomaton.hh \
	ACU_TreeVariableSubproblem.hh
