ROOT_DIR := ..

include $(ROOT_DIR)/MakeOptions

MODULES =	compilationContext.o

BORROWED_MODULES =	$(ROOT_DIR)/Utility/pointerSet.o \
			$(ROOT_DIR)/Utility/preVector.o

RMODULES =      runtime.o

IFLAGS =        -I$(ROOT_DIR)/Core \
                -I$(ROOT_DIR)/Interface \
		-I$(ROOT_DIR)/Utility \
		-I$(ROOT_DIR)/FullCompiler

CXXFLAGS =	$(DFLAGS) $(IFLAGS)

all :	libfullCompiler.a libruntime.a

libfullCompiler.a : $(MODULES)
	$(AR) $(ARFLAGS) libfullCompiler.a $(MODULES)

libruntime.a : $(RMODULES)
	$(AR) $(ARFLAGS) libruntime.a $(RMODULES) $(BORROWED_MODULES)

clean:
	rm *.o *.a
