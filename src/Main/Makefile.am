bin_PROGRAMS = maude

if BUILD_COMPILER
  FULL_COMPILER_LIB = ../FullCompiler/libfullCompiler.a
endif

maude_CPPFLAGS = \
	-I$(top_srcdir)/src/Utility \
	-I$(top_srcdir)/src/Temporal \
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
	-I$(top_srcdir)/src/SMT \
	-I$(top_srcdir)/src/IO_Stuff \
	-I$(top_srcdir)/src/ACU_Persistent \
	-I$(top_srcdir)/src/ACU_Theory \
	-I$(top_srcdir)/src/AU_Persistent \
	-I$(top_srcdir)/src/AU_Theory \
	-I$(top_srcdir)/src/Meta \
	-I$(top_srcdir)/src/3rdParty \
	-I$(top_srcdir)/src/FullCompiler \
	-I$(top_srcdir)/src/StrategyLanguage \
	-I$(top_srcdir)/src/Parser \
	-I$(top_srcdir)/src/Mixfix

maude_LDADD = \
	../Mixfix/libmixfix.a \
	../StrategyLanguage/libstrategyLanguage.a \
	../Meta/libmeta.a \
	../Mixfix/libmixfix.a \
	../ObjectSystem/libobjectSystem.a \
	../BuiltIn/libbuiltIn.a \
	../SMT/libSMT.a \
	../Higher/libhigher.a \
	../FreeTheory/libfreeTheory.a \
	../AU_Theory/libAU_Theory.a \
	../AU_Persistent/libAU_Persistent.a \
	../ACU_Theory/libACU_Theory.a \
	../ACU_Persistent/libACU_Persistent.a \
	../CUI_Theory/libCUI_Theory.a \
	../S_Theory/libS_Theory.a \
	../NA_Theory/libNA_Theory.a \
	../Variable/libvariable.a \
	../Interface/libinterface.a \
	../Core/libcore.a \
	../Parser/libparser.a \
	../Temporal/libtemporal.a \
	../IO_Stuff/libIO_Stuff.a \
	$(FULL_COMPILER_LIB) \
	../Utility/libutility.a \
	$(CVC4_LIB) \
	$(YICES2_LIB) \
	$(GMP_LIBS) \
	$(BUDDY_LIB) \
	$(READLINE_LIBS) \
	$(LIBSIGSEGV_LIB)

maude_SOURCES = \
	main.cc

data_DATA = \
	prelude.maude \
	model-checker.maude \
	socket.maude \
	linear.maude \
	machine-int.maude \
	term-order.maude \
	metaInterpreter.maude \
	smt.maude \
	file.maude \
	time.maude \
	prng.maude

EXTRA_DIST = \
	prelude.maude \
	model-checker.maude \
	socket.maude \
	linear.maude \
	machine-int.maude \
	term-order.maude \
	metaInterpreter.maude \
	smt.maude \
	file.maude \
	process.maude \
	time.maude \
	prng.maude \
	maude.sty
