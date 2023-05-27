
set(AU_Theory_sources
        AU_Symbol.cc
        AU_DagNode.cc
        AU_DequeDagNode.cc
        AU_Term.cc
        AU_DagArgumentIterator.cc
        AU_DequeDagArgumentIterator.cc
        AU_ExtensionInfo.cc
        AU_RhsAutomaton.cc
        AU_LhsAutomaton.cc
        AU_ArgumentIterator.cc
        AU_Layer.cc
        AU_Subproblem.cc
        AU_UnificationSubproblem2.cc
        )

set(AU_Theory_headers
        AU_ArgumentIterator.hh
        AU_BaseDagNode.hh
        AU_DagArgumentIterator.hh
        AU_DagNode.hh
        AU_DequeDagArgumentIterator.hh
        AU_DequeDagNode.hh
        AU_ExtensionInfo.hh
        AU_Layer.hh
        AU_LhsAutomaton.hh
        AU_RhsAutomaton.hh
        AU_Subproblem.hh
        AU_Symbol.hh
        AU_Term.hh
        AU_Theory.hh
        AU_UnificationSubproblem2.hh
        )

add_library(AU_Theory STATIC ${AU_Theory_sources} ${AU_Theory_headers})

# Set include directories for AU_Theory.a
target_include_directories(AU_Theory PRIVATE
        ${CMAKE_SOURCE_DIR}/src/Utility
        ${CMAKE_SOURCE_DIR}/src/Interface
        ${CMAKE_SOURCE_DIR}/src/Core
        ${CMAKE_SOURCE_DIR}/src/Variable
        ${CMAKE_SOURCE_DIR}/src/AU_Persistent
        ${CMAKE_SOURCE_DIR}/src/FullCompiler
        )
