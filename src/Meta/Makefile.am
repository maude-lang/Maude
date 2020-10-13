noinst_LIBRARIES = libmeta.a

libmeta_a_CPPFLAGS = \
	-I$(top_srcdir)/src/Utility \
	-I$(top_srcdir)/src/Interface \
	-I$(top_srcdir)/src/Core \
	-I$(top_srcdir)/src/Variable \
	-I$(top_srcdir)/src/FullCompiler \
	-I$(top_srcdir)/src/Higher \
	-I$(top_srcdir)/src/AU_Theory \
	-I$(top_srcdir)/src/CUI_Theory \
	-I$(top_srcdir)/src/S_Theory \
	-I$(top_srcdir)/src/NA_Theory \
	-I$(top_srcdir)/src/FreeTheory \
	-I$(top_srcdir)/src/ObjectSystem \
	-I$(top_srcdir)/src/Mixfix \
	-I$(top_srcdir)/src/BuiltIn \
	-I$(top_srcdir)/src/IO_Stuff \
	-I$(top_srcdir)/src/StrategyLanguage \
	-I$(top_srcdir)/src/3rdParty \
	-I$(top_srcdir)/src/SMT \
	-I$(top_srcdir)/src/Parser

libmeta_a_SOURCES = \
	metaModuleCache.cc \
	metaLevel.cc \
	interpreterManagerSymbol.cc \
	metaOpCache.cc \
	metaModule.cc \
	metaLevelOpSymbol.cc \
	metaPreModule.cc \
	metaView.cc

EXTRA_DIST = \
	metaDown.cc \
	metaDownRenamings.cc \
	metaDownOps.cc \
	metaDownStrats.cc \
	metaDownFixUps.cc \
	metaLevelSignature.cc \
	metaUp.cc \
	metaUpModule.cc \
	descentFunctions.cc \
	ascentFunctions.cc \
	metaApply.cc \
	metaMatch.cc \
	metaUnify.cc \
	metaVariant.cc \
	metaVariantUnify.cc \
	metaVariantMatch.cc \
	metaSearch.cc \
	metaSrewrite.cc \
	metaNarrow.cc \
	metaNewNarrow.cc \
	metaNewNarrow2.cc \
	descentSignature.cc \
	interpreterSignature.cc \
	metaUpView.cc \
	metaDownView.cc \
	metaVariant.cc \
	metaDownSignature.cc \
	interpreterRewrite.cc \
	interpreterSearch.cc \
	interpreterMatch.cc \
	interpreterUnify.cc \
	interpreterVariant.cc \
	interpreterVariantUnify.cc \
	interpreterVariantMatch.cc \
	interpreterSort.cc \
	interpreterPrint.cc \
	interpreterApply.cc \
	interpreterNewNarrow.cc \
	interpreterNewNarrowSearch.cc \
	legacyMetaUp.cc \
	legacyMetaUnify.cc \
	legacyMetaVariant.cc \
	legacyMetaVariantUnify.cc

noinst_HEADERS = \
	meta.hh \
	metaLevel.hh \
	interpreterManagerSymbol.hh \
	metaLevelOpSymbol.hh \
	metaModule.hh \
	metaModuleCache.hh \
	metaOpCache.hh \
	metaPreModule.hh \
	metaView.hh
