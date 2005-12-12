noinst_LIBRARIES = libbuiltIn.a

libbuiltIn_a_CPPFLAGS = \
	-I$(top_srcdir)/src/Utility \
	-I$(top_srcdir)/src/Interface \
	-I$(top_srcdir)/src/Core \
	-I$(top_srcdir)/src/Variable \
	-I$(top_srcdir)/src/FullCompiler \
	-I$(top_srcdir)/src/NA_Theory \
	-I$(top_srcdir)/src/ACU_Persistent \
	-I$(top_srcdir)/src/ACU_Theory \
	-I$(top_srcdir)/src/CUI_Theory \
	-I$(top_srcdir)/src/S_Theory \
	-I$(top_srcdir)/src/FreeTheory \
	-I$(top_srcdir)/src/3rdParty

libbuiltIn_a_SOURCES = \
	equalitySymbol.cc \
	sortTestSymbol.cc \
	branchSymbol.cc \
	stringSymbol.cc \
	stringTerm.cc \
	stringDagNode.cc \
	stringOpSymbol.cc \
	floatSymbol.cc \
	floatTerm.cc \
	floatDagNode.cc \
	floatOpSymbol.cc \
	succSymbol.cc \
	numberOpSymbol.cc \
	minusSymbol.cc \
	ACU_NumberOpSymbol.cc \
	CUI_NumberOpSymbol.cc \
	divisionSymbol.cc \
	randomOpSymbol.cc \
	counterSymbol.cc \
	matrixOpSymbol.cc

EXTRA_DIST = \
	matrixOpSignature.cc

noinst_HEADERS = \
	ACU_NumberOpSymbol.hh \
	CUI_NumberOpSymbol.hh \
	bindingMacros.hh \
	branchSymbol.hh \
	builtIn.hh \
	divisionSymbol.hh \
	equalitySymbol.hh \
	floatDagNode.hh \
	floatOpSymbol.hh \
	floatSymbol.hh \
	floatTerm.hh \
	minusSymbol.hh \
	numberOpSymbol.hh \
	sortTestSymbol.hh \
	stringDagNode.hh \
	stringOpSymbol.hh \
	stringSymbol.hh \
	stringTerm.hh \
	succSymbol.hh \
	randomOpSymbol.hh \
	counterSymbol.hh \
	matrixOpSymbol.hh
