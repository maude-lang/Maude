ROOT_DIR := ..

include $(ROOT_DIR)/MakeOptions

MODULES =	variableSymbol.o \
		variableTerm.o \
		variableDagNode.o \
		variableLhsAutomaton.o

IFLAGS =	-I$(ROOT_DIR)/Utility \
		-I$(ROOT_DIR)/Core \
		-I$(ROOT_DIR)/Interface \
		-I$(ROOT_DIR)/Variable \
		-I$(ROOT_DIR)/FullCompiler

CXXFLAGS =	$(DFLAGS) $(IFLAGS)

libcore.a : $(MODULES)
	$(AR) $(ARFLAGS) libvariable.a $(MODULES)

clean:
	rm *.o *.a
