
set(freeTheory_sources
        freeSymbol.cc
        freeUnarySymbol.cc
        freeNullarySymbol.cc
        freeBinarySymbol.cc
        freeTernarySymbol.cc
        freeTerm.cc
        freeArgumentIterator.cc
        freeDagNode.cc
        freeDagArgumentIterator.cc
        freeLhsAutomaton.cc
        freeRhsAutomaton.cc
        freeNet.cc
        freePreNet.cc
        freeRemainder.cc
        freePositionTable.cc
        freeInstruction.cc
        freeFastInstruction.cc
        freeGeneralCtor.cc
        freeGeneralCtorFinal.cc
        freeGeneralExtor.cc
        freeGeneralExtorFinal.cc
        freeFast3RhsAutomaton.cc
        freeFast2RhsAutomaton.cc
        freeNullaryRhsAutomaton.cc
        freeUnaryRhsAutomaton.cc
        freeBinaryRhsAutomaton.cc
        freeTernaryRhsAutomaton.cc
        )

set(freeTheory_headers
        freeArgumentIterator.hh
        freeNullarySymbol.hh
        freeUnarySymbol.hh
        freeBinarySymbol.hh
        freeTernarySymbol.hh
        freeDagArgumentIterator.hh
        freeDagNode.hh
        freeLhsAutomaton.hh
        freeLhsStructs.hh
        freeNet.hh
        freeNetExec.hh
        freeOccurrence.hh
        freePositionTable.hh
        freePreNet.hh
        freeRemainder.hh
        freeRhsAutomaton.hh
        freeSubterm.hh
        freeSymbol.hh
        freeTerm.hh
        freeTheory.hh
        freeInstruction.hh
        freeFastInstruction.hh
        freeGeneralCtor.hh
        freeGeneralCtorFinal.hh
        freeGeneralExtor.hh
        freeGeneralExtorFinal.hh
        freeFast3RhsAutomaton.hh
        freeFast2RhsAutomaton.hh
        freeNullaryRhsAutomaton.hh
        freeUnaryRhsAutomaton.hh
        freeBinaryRhsAutomaton.hh
        freeTernaryRhsAutomaton.hh
        )

add_library(freeTheory STATIC ${freeTheory_sources} ${freeTheory_headers})

# Set include directories for freeTheory.a
target_include_directories(freeTheory PRIVATE
        ${CMAKE_SOURCE_DIR}/src/Utility
        ${CMAKE_SOURCE_DIR}/src/Interface
        ${CMAKE_SOURCE_DIR}/src/Core
        ${CMAKE_SOURCE_DIR}/src/Variable
        ${CMAKE_SOURCE_DIR}/src/FullCompiler
        )
