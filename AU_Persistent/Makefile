ROOT_DIR := ..

include $(ROOT_DIR)/MakeOptions

MODULES =	AU_StackNode.o \
		AU_Deque.o \
		AU_DequeIter.o

IFLAGS =        -I$(ROOT_DIR)/Core \
                -I$(ROOT_DIR)/Interface \
		-I$(ROOT_DIR)/AU_Stack \
		-I$(ROOT_DIR)/Utility \
                -I$(ROOT_DIR)/Variable \
                -I$(ROOT_DIR)/FullCompiler

CXXFLAGS =      $(DFLAGS) $(IFLAGS)
#CXXFLAGS =      $(DFLAGS) $(IFLAGS) -DCHECK_DEQUE

libAU_Persistent.a : $(MODULES)
	$(AR) $(ARFLAGS) libAU_Persistent.a $(MODULES)


AU_StackNode.o : AU_StackArgVec.cc AU_StackSort.cc

clean:
	rm *.o *.a
