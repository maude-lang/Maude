noinst_LIBRARIES = libACU_Persistent.a

libACU_Persistent_a_CPPFLAGS = \
	-I$(top_srcdir)/src/Utility \
	-I$(top_srcdir)/src/Interface \
	-I$(top_srcdir)/src/Core \
	-I$(top_srcdir)/src/Variable \
	-I$(top_srcdir)/src/FullCompiler

libACU_Persistent_a_SOURCES = \
	ACU_RedBlackNode.cc \
	ACU_Tree.cc

EXTRA_DIST = \
	ACU_Find.cc \
	ACU_Insert.cc \
	ACU_Copy.cc \
	ACU_Delete.cc \
	ACU_Rebalance.cc

noinst_HEADERS = \
	ACU_FastIter.hh \
	ACU_Persistent.hh \
	ACU_RedBlackNode.hh \
	ACU_Stack.hh \
	ACU_Pair.hh \
	ACU_RedBlack.hh \
	ACU_SlowIter.hh \
	ACU_Tree.hh
