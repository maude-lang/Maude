ROOT_DIR := ..

include $(ROOT_DIR)/MakeOptions

MODULES =	freeSymbol.o \
		freeUnarySymbol.o \
		freeBinarySymbol.o \
		freeTernarySymbol.o \
		freeTerm.o \
		freeArgumentIterator.o \
		freeDagNode.o \
		freeDagArgumentIterator.o \
		freeLhsAutomaton.o \
		freeRhsAutomaton.o \
		freeNet.o \
		freePreNet.o \
		freeRemainder.o \
		freePositionTable.o


IFLAGS =        -I$(ROOT_DIR)/Core \
                -I$(ROOT_DIR)/Interface \
		-I$(ROOT_DIR)/FreeTheory \
		-I$(ROOT_DIR)/Utility \
		-I$(ROOT_DIR)/Variable \
		-I$(ROOT_DIR)/FullCompiler

CXXFLAGS =      $(DFLAGS) $(IFLAGS)

libfreeTheory.a : $(MODULES)
	$(AR) $(ARFLAGS) libfreeTheory.a $(MODULES)

freeNet.o : freeNetExec.cc
freeTerm.o : freeTermFullCompiler.cc freeLhsCompiler.cc
freePreNet.o : freePreNetSubsumption.cc freePreNetSemiCompiler.cc freePreNetFullCompiler.cc

clean:
	rm *.o *.a
