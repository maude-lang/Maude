ROOT_DIR := ..

include $(ROOT_DIR)/MakeOptions

MODULES =	symbol.o \
		binarySymbol.o \
		associativeSymbol.o \
		term.o \
		dagNode.o

IFLAGS =        -I$(ROOT_DIR)/Utility \
		-I$(ROOT_DIR)/Core \
                -I$(ROOT_DIR)/Interface \
		-I$(ROOT_DIR)/Variable \
		-I$(ROOT_DIR)/FullCompiler

CXXFLAGS =      $(DFLAGS) $(IFLAGS)

libinterface.a : $(MODULES)
	$(AR) $(ARFLAGS) libinterface.a $(MODULES)

clean:
	rm *.o *.a
