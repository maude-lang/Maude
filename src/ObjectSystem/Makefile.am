noinst_LIBRARIES = libobjectSystem.a

libobjectSystem_a_CPPFLAGS = \
	-I$(top_srcdir)/src/Utility \
	-I$(top_srcdir)/src/Interface \
	-I$(top_srcdir)/src/Core \
	-I$(top_srcdir)/src/Variable \
	-I$(top_srcdir)/src/FullCompiler \
	-I$(top_srcdir)/src/ACU_Persistent \
	-I$(top_srcdir)/src/ACU_Theory

libobjectSystem_a_SOURCES = \
	configSymbol.cc

EXTRA_DIST = \
	objectMap.cc \
	remainder.cc

noinst_HEADERS = \
	configSymbol.hh \
	objectSystem.hh \
	objectSystemRewritingContext.hh
