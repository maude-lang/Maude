ROOT_DIR := ..

include $(ROOT_DIR)/MakeOptions

MODULES =	macros.o \
		preVector.o \
		intSet.o \
		pointerSet.o \
		stringTable.o \
		graph.o \
		digraph.o \
		unionFind.o \
		diophantineSystem.o \
		natSet.o \
		pointerMap.o \
		sequencePartition.o \
		indent.o \
		binBuf.o \
		decBuf.o \
		bddUser.o \
		tty.o \
		bdd.o \
		timer.o

IFLAGS =	-I$(ROOT_DIR)/Utility

CXXFLAGS =	$(DFLAGS) $(IFLAGS)
# -DDIO_STATS

libutility.a : $(MODULES)
	$(AR) $(ARFLAGS) libutility.a $(MODULES)

clean:
	rm *.o *.a
