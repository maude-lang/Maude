ROOT_DIR := ..

include $(ROOT_DIR)/MakeOptions

MODULES =	ACU_RedBlackNode.o \
		ACU_Tree.o

IFLAGS =        -I$(ROOT_DIR)/Core \
                -I$(ROOT_DIR)/Interface \
		-I$(ROOT_DIR)/ACU_RedBlack \
		-I$(ROOT_DIR)/Utility \
                -I$(ROOT_DIR)/Variable \
                -I$(ROOT_DIR)/FullCompiler

CXXFLAGS =      $(DFLAGS) $(IFLAGS)

libACU_Persistent.a : $(MODULES)
	$(AR) $(ARFLAGS) libACU_Persistent.a $(MODULES)

ACU_RedBlackNode.o : \
	ACU_Find.cc \
	ACU_Insert.cc \
	ACU_Copy.cc \
	ACU_Delete.cc \
	ACU_Rebalance.cc

clean:
	rm *.o *.a
