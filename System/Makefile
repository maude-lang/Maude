ROOT_DIR := ..

include $(ROOT_DIR)/MakeOptions

MODULES =	IO_Manager.o \
		autoWrapBuffer.o \
		directoryManager.o

IFLAGS =	-I$(ROOT_DIR)/Utility

CXXFLAGS =	$(DFLAGS) $(IFLAGS)

libsystem.a : $(MODULES)
	$(AR) $(ARFLAGS) libsystem.a $(MODULES)

clean:
	rm *.o *.a
