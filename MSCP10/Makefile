ROOT_DIR := ..

include $(ROOT_DIR)/MakeOptions

MODULES =	scp_parser.o \
		scp_compiler.o \
		scp_memory.o

IFLAGS =	-I$(ROOT_DIR)/Utility \
		-I$(ROOT_DIR)/MSCP10

CXXFLAGS =	$(DFLAGS) $(IFLAGS)

libmscp10.a : $(MODULES)
	$(AR) $(ARFLAGS) libmscp10.a $(MODULES)

clean:
	rm *.o *.a
