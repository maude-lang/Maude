if BUILD_COMPILER

noinst_LIBRARIES = libfullCompiler.a libruntime.a

libfullCompiler_a_CPPFLAGS = \
	-I$(top_srcdir)/src/Utility \
	-I$(top_srcdir)/src/Interface \
	-I$(top_srcdir)/src/Core \
	-I$(top_srcdir)/src/FullCompiler

libfullCompiler_a_SOURCES = \
	compilationContext.cc

libruntime_a_CPPFLAGS = \
	-I$(top_srcdir)/src/Utility \
	-I$(top_srcdir)/src/FullCompiler

libruntime_a_SOURCES = \
	runtime.cc

endif

EXTRA_DIST = \
	compilationContext.cc \
	runtime.cc

noinst_HEADERS = \
	compilationContext.hh \
	fullCompiler.hh \
	variableName.hh \
	runtime.hh
