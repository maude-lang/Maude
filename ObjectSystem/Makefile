ROOT_DIR := ..

include $(ROOT_DIR)/MakeOptions

MODULES =       configSymbol.o

IFLAGS =        -I$(ROOT_DIR)/Core \
                -I$(ROOT_DIR)/Interface \
                -I$(ROOT_DIR)/FreeTheory \
                -I$(ROOT_DIR)/ACU_Persistent \
                -I$(ROOT_DIR)/ACU_Theory \
		-I$(ROOT_DIR)/Utility \
		-I$(ROOT_DIR)/Variable \
		-I$(ROOT_DIR)/FullCompiler

CXXFLAGS =      $(DFLAGS) $(IFLAGS)

libobjectSystem.a : $(MODULES)
	$(AR) $(ARFLAGS) libobjectSystem.a $(MODULES)

clean:
	rm *.o *.a

configSymbol.o : objectMap.cc remainder.cc
