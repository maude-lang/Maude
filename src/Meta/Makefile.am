noinst_LIBRARIES = libmeta.a

libmeta_a_CPPFLAGS = \
	-I$(top_srcdir)/src/Utility \
	-I$(top_srcdir)/src/Interface \
	-I$(top_srcdir)/src/Core \
	-I$(top_srcdir)/src/Variable \
	-I$(top_srcdir)/src/FullCompiler \
	-I$(top_srcdir)/src/Higher \
	-I$(top_srcdir)/src/CUI_Theory \
	-I$(top_srcdir)/src/S_Theory \
	-I$(top_srcdir)/src/NA_Theory \
	-I$(top_srcdir)/src/FreeTheory \
	-I$(top_srcdir)/src/ObjectSystem \
	-I$(top_srcdir)/src/Mixfix \
	-I$(top_srcdir)/src/BuiltIn \
	-I$(top_srcdir)/src/MSCP10 \
	-I$(top_srcdir)/src/IO_Stuff \
	-I$(top_srcdir)/src/StrategyLanguage

libmeta_a_SOURCES = \
	metaModuleCache.cc \
	metaLevel.cc \
	metaOpCache.cc \
	metaModule.cc \
	metaLevelOpSymbol.cc

EXTRA_DIST = \
	metaDown.cc \
	metaDownRenamings.cc \
	metaDownOps.cc \
	metaDownFixUps.cc \
	metaLevelSignature.cc \
	metaUp.cc \
	metaUpModule.cc \
	descentFunctions.cc \
	ascentFunctions.cc \
	metaApply.cc \
	metaMatch.cc \
	metaUnify.cc \
	metaSearch.cc \
	metaNarrow.cc \
	descentSignature.cc

noinst_HEADERS = \
	meta.hh \
	metaLevel.hh \
	metaLevelOpSymbol.hh \
	metaModule.hh \
	metaModuleCache.hh \
	metaOpCache.hh
