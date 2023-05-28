# CMakeLists.txt generated from Makefile.am

# Add source files to the library target
set(utility_sources
        macros.cc
        preVector.cc
        intSet.cc
        pointerSet.cc
        stringTable.cc
        graph.cc
        digraph.cc
        unionFind.cc
        diophantineSystem.cc
        natSet.cc
        pointerMap.cc
        sequencePartition.cc
        indent.cc
        binBuf.cc
        decBuf.cc
        bddUser.cc
        tty.cc
        bdd.cc
        timer.cc
        mpzSystem.cc
        intSystem.cc
        allSat.cc
        sequenceAssignment.cc
        pigPug.cc
        wordLevel.cc
        wordSystem.cc
        rope.cc
        variableConstraint.cc
        )

# Add header files to the library target
set(utility_headers
        bdd.hh
        bddUser.hh
        binBuf.hh
        checkedConstIterator.hh
        checkedIterator.hh
        decBuf.hh
        digraph.hh
        diophantineSystem.hh
        flagSet.hh
        graph.hh
        indent.hh
        indexedSet.hh
        intSet.hh
        macros.hh
        natSet.hh
        pointerMap.hh
        pointerSet.hh
        preVector.hh
        sequencePartition.hh
        stringTable.hh
        timer.hh
        tty.hh
        unionFind.hh
        vector.hh
        timeStuff.hh
        mathStuff.hh
        mpzSystem.hh
        intSystem.hh
        allSat.hh
        sequenceAssignment.hh
        pigPug.hh
        wordLevel.hh
        wordSystem.hh
        rope.hh
        stack.hh
        variableConstraint.hh
        slowVector.hh
        )

# Create the library target
add_library(utility STATIC ${utility_sources} ${utility_headers})

# Set include directories for the library
target_include_directories(utility PRIVATE
        ${CMAKE_SOURCE_DIR}/src/3rdParty
        ${CMAKE_SOURCE_DIR}/src/MixFix
        )
