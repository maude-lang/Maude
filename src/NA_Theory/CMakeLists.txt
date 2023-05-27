set(NA_Theory_sources
        NA_Symbol.cc
        NA_Term.cc
        NA_DagNode.cc
        NA_LhsAutomaton.cc
        NA_RhsAutomaton.cc
        )

set(NA_Theory_headers
        NA_DagNode.hh
        NA_LhsAutomaton.hh
        NA_RhsAutomaton.hh
        NA_Symbol.hh
        NA_Term.hh
        NA_Theory.hh
        )

add_library(NA_Theory STATIC ${NA_Theory_sources} ${NA_Theory_headers})

# Set include directories for libNA_Theory.a
target_include_directories(NA_Theory PRIVATE
        ${CMAKE_SOURCE_DIR}/src/Utility
        ${CMAKE_SOURCE_DIR}/src/Interface
        ${CMAKE_SOURCE_DIR}/src/Core
        ${CMAKE_SOURCE_DIR}/src/Variable
        ${CMAKE_SOURCE_DIR}/src/FullCompiler
        )
