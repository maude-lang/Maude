set(S_Theory_sources
        S_Symbol.cc
        S_DagNode.cc
        S_Term.cc
        S_ExtensionInfo.cc
        S_DagArgumentIterator.cc
        S_ArgumentIterator.cc
        S_LhsAutomaton.cc
        S_RhsAutomaton.cc
        S_Subproblem.cc
        )

set(S_Theory_headers
        S_ArgumentIterator.hh
        S_DagArgumentIterator.hh
        S_DagNode.hh
        S_ExtensionInfo.hh
        S_LhsAutomaton.hh
        S_RhsAutomaton.hh
        S_Subproblem.hh
        S_Symbol.hh
        S_Term.hh
        S_Theory.hh
        )

add_library(S_Theory STATIC ${S_Theory_sources} ${S_Theory_headers})

# Set include directories for libS_Theory.a
target_include_directories(S_Theory PRIVATE
        ${CMAKE_SOURCE_DIR}/src/Utility
        ${CMAKE_SOURCE_DIR}/src/Interface
        ${CMAKE_SOURCE_DIR}/src/Core
        ${CMAKE_SOURCE_DIR}/src/Variable
        ${CMAKE_SOURCE_DIR}/src/FullCompiler
        )
