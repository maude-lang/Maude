noinst_LIBRARIES = libAU_Persistent.a

libAU_Persistent_a_CPPFLAGS = \
	-I$(top_srcdir)/src/Utility \
	-I$(top_srcdir)/src/Interface \
	-I$(top_srcdir)/src/Core \
	-I$(top_srcdir)/src/Variable \
	-I$(top_srcdir)/src/FullCompiler

libAU_Persistent_a_SOURCES = \
	AU_StackNode.cc \
	AU_Deque.cc \
	AU_DequeIter.cc

EXTRA_DIST = \
	AU_StackArgVec.cc \
	AU_StackSort.cc

noinst_HEADERS = \
	AU_Deque.hh \
	AU_DequeIter.hh \
	AU_Persistent.hh \
	AU_StackNode.hh
