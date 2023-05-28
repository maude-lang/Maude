
set(CUI_Theory_sources
        CUI_Symbol.cc
        CUI_DagNode.cc
        CUI_DagArgumentIterator.cc
        CUI_RhsAutomaton.cc
        CUI_Term.cc
        CUI_ArgumentIterator.cc
        CUI_UnificationSubproblem.cc
        CUI_UnificationSubproblem2.cc
        CUI_LhsAutomaton.cc
        )

set(CUI_Theory_headers
        CUI_ArgumentIterator.hh
        CUI_DagArgumentIterator.hh
        CUI_DagNode.hh
        CUI_LhsAutomaton.hh
        CUI_RhsAutomaton.hh
        CUI_Symbol.hh
        CUI_Term.hh
        CUI_UnificationSubproblem.hh
        CUI_UnificationSubproblem2.hh
        CUI_Theory.hh
        )

add_library(CUI_Theory STATIC ${CUI_Theory_sources} ${CUI_Theory_headers})

# Set include directories for CUI_Theory.a
target_include_directories(CUI_Theory PRIVATE
        ${CMAKE_SOURCE_DIR}/src/Utility
        ${CMAKE_SOURCE_DIR}/src/Interface
        ${CMAKE_SOURCE_DIR}/src/Core
        ${CMAKE_SOURCE_DIR}/src/Variable
        ${CMAKE_SOURCE_DIR}/src/FullCompiler
        )
