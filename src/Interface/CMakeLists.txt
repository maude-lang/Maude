
set(interface_sources
        symbol.cc
        binarySymbol.cc
        associativeSymbol.cc
        term.cc
        dagNode.cc
        regularInstruction.cc
        nonFinalInstruction.cc
        nonFinalExtor.cc
        )

set(interface_headers
        associativeSymbol.hh
        binarySymbol.hh
        dagNode.hh
        extensionInfo.hh
        interface.hh
        lhsAutomaton.hh
        rawArgumentIterator.hh
        rawDagArgumentIterator.hh
        rhsAutomaton.hh
        subproblem.hh
        unificationSubproblem.hh
        subproblem.hh
        symbol.hh
        symbol2.hh
        term.hh
        delayedSubproblem.hh
        instruction.hh
        regularInstruction.hh
        finalInstruction.hh
        nonFinalInstruction.hh
        nonFinalExtor.hh
        nonFinalCtor.hh
        )

add_library(interface STATIC ${interface_sources} ${interface_headers})

# Set include directories for interface.a
target_include_directories(interface PRIVATE
        ${CMAKE_SOURCE_DIR}/src/Utility
        ${CMAKE_SOURCE_DIR}/src/Interface
        ${CMAKE_SOURCE_DIR}/src/Core
        ${CMAKE_SOURCE_DIR}/src/Variable
        ${CMAKE_SOURCE_DIR}/src/FullCompiler
        )
