set(SMT_sources
        SMT_Symbol.cc
        SMT_NumberSymbol.cc
        SMT_NumberDagNode.cc
        SMT_NumberTerm.cc
        SMT_RewriteSearchState.cc
        SMT_RewriteSequenceSearch.cc
        SMT_Info.cc
        )

set(SMT_headers
        SMT.hh
        SMT_Base.hh
        SMT_Symbol.hh
        SMT_NumberSymbol.hh
        SMT_NumberDagNode.hh
        SMT_NumberTerm.hh
        SMT_EngineWrapper.hh
        SMT_RewriteSequenceSearch.hh
        SMT_RewriteSearchState.hh
        SMT_Info.hh
        )

add_library(SMT STATIC ${SMT_sources} ${SMT_headers})

# Set include directories for libSMT.a
target_include_directories(SMT PRIVATE
        ${CMAKE_SOURCE_DIR}/src/Utility
        ${CMAKE_SOURCE_DIR}/src/Interface
        ${CMAKE_SOURCE_DIR}/src/Core
        ${CMAKE_SOURCE_DIR}/src/Variable
        ${CMAKE_SOURCE_DIR}/src/Higher
        ${CMAKE_SOURCE_DIR}/src/FullCompiler
        ${CMAKE_SOURCE_DIR}/src/NA_Theory
        ${CMAKE_SOURCE_DIR}/src/FreeTheory
        ${CMAKE_SOURCE_DIR}/src/BuiltIn
        )
