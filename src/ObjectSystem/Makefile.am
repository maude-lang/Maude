noinst_LIBRARIES = libobjectSystem.a

libobjectSystem_a_CPPFLAGS = \
	-I$(top_srcdir)/src/Utility \
	-I$(top_srcdir)/src/Interface \
	-I$(top_srcdir)/src/Core \
	-I$(top_srcdir)/src/Variable \
	-I$(top_srcdir)/src/FullCompiler \
	-I$(top_srcdir)/src/ACU_Persistent \
	-I$(top_srcdir)/src/ACU_Theory \
	-I$(top_srcdir)/src/FreeTheory \
	-I$(top_srcdir)/src/NA_Theory \
	-I$(top_srcdir)/src/S_Theory \
	-I$(top_srcdir)/src/BuiltIn

libobjectSystem_a_SOURCES = \
	configSymbol.cc \
	objectSystemRewritingContext.cc \
	externalObjectManagerSymbol.cc \
	socketManagerSymbol.cc \
	pseudoThread.cc 

EXTRA_DIST = \
	objectMap.cc \
	remainder.cc \
	socketSignature.cc \
	socketStuff.cc \
	socketAsync.cc \
	socketOutcomes.cc

noinst_HEADERS = \
	configSymbol.hh \
	objectSystem.hh \
	objectSystemRewritingContext.hh \
	externalObjectManagerSymbol.hh \
	socketManagerSymbol.hh \
	pseudoThread.hh
