noinst_LIBRARIES = libfreeTheory.a

libfreeTheory_a_CPPFLAGS = \
	-I$(top_srcdir)/src/Utility \
	-I$(top_srcdir)/src/Interface \
	-I$(top_srcdir)/src/Core \
	-I$(top_srcdir)/src/Variable \
	-I$(top_srcdir)/src/FullCompiler

libfreeTheory_a_SOURCES = \
	freeSymbol.cc \
	freeUnarySymbol.cc \
	freeNullarySymbol.cc \
	freeBinarySymbol.cc \
	freeTernarySymbol.cc \
	freeTerm.cc \
	freeArgumentIterator.cc \
	freeDagNode.cc \
	freeDagArgumentIterator.cc \
	freeLhsAutomaton.cc \
	freeRhsAutomaton.cc \
	freeNet.cc \
	freePreNet.cc \
	freeRemainder.cc \
	freePositionTable.cc \
	freeInstruction.cc \
	freeFastInstruction.cc \
	freeGeneralCtor.cc \
	freeGeneralCtorFinal.cc \
	freeGeneralExtor.cc \
	freeGeneralExtorFinal.cc

EXTRA_DIST = \
	freeNetExec.cc \
	freeTermFullCompiler.cc \
	freeLhsCompiler.cc \
	freePreNetSubsumption.cc \
	freePreNetSemiCompiler.cc \
	freePreNetFullCompiler.cc

noinst_HEADERS = \
	freeArgumentIterator.hh \
	freeNullarySymbol.hh \
	freeUnarySymbol.hh \
	freeBinarySymbol.hh \
	freeTernarySymbol.hh \
	freeDagArgumentIterator.hh \
	freeDagNode.hh \
	freeLhsAutomaton.hh \
	freeLhsStructs.hh \
	freeNet.hh \
	freeNetExec.hh \
	freeOccurrence.hh \
	freePositionTable.hh \
	freePreNet.hh \
	freeRemainder.hh \
	freeRhsAutomaton.hh \
	freeSubterm.hh \
	freeSymbol.hh \
	freeTerm.hh \
	freeTheory.hh \
	freeInstruction.hh \
	freeFastInstruction.hh \
	freeGeneralCtor.hh \
	freeGeneralCtorFinal.hh \
	freeGeneralExtor.hh \
	freeGeneralExtorFinal.hh
