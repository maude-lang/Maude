
set(ACU_Theory_sources
        ACU_Symbol.cc
        ACU_Term.cc
        ACU_BaseDagNode.cc
        ACU_DagNode.cc
        ACU_TreeDagNode.cc
        ACU_RhsAutomaton.cc
        ACU_LhsAutomaton.cc
        ACU_Subproblem.cc
        ACU_ExtensionInfo.cc
        ACU_CollectorLhsAutomaton.cc
        ACU_NGA_LhsAutomaton.cc
        ACU_VarLhsAutomaton.cc
        ACU_GndLhsAutomaton.cc
        ACU_NonLinearLhsAutomaton.cc
        ACU_ArgumentIterator.cc
        ACU_DagArgumentIterator.cc
        ACU_TreeDagArgumentIterator.cc
        ACU_LazySubproblem.cc
        ACU_UnificationSubproblem2.cc
        ACU_BndVarLhsAutomaton.cc
        ACU_TreeVariableSubproblem.cc
        )

set(ACU_Theory_headers
        ACU_ArgumentIterator.hh
        ACU_BaseDagNode.hh
        ACU_CollectorLhsAutomaton.hh
        ACU_DagArgumentIterator.hh
        ACU_DagNode.hh
        ACU_ExtensionInfo.hh
        ACU_GndLhsAutomaton.hh
        ACU_LazySubproblem.hh
        ACU_LhsAutomaton.hh
        ACU_NGA_LhsAutomaton.hh
        ACU_NonLinearLhsAutomaton.hh
        ACU_RhsAutomaton.hh
        ACU_Subproblem.hh
        ACU_Symbol.hh
        ACU_Term.hh
        ACU_Theory.hh
        ACU_TreeDagArgumentIterator.hh
        ACU_TreeDagNode.hh
        ACU_VarLhsAutomaton.hh
        ACU_UnificationSubproblem2.hh
        ACU_BndVarLhsAutomaton.hh
        ACU_TreeVariableSubproblem.hh
        )

add_library(ACU_Theory STATIC ${ACU_Theory_headers} ${ACU_Theory_sources})

# Set include directories for ACU_Theory.a
target_include_directories(ACU_Theory PRIVATE
        ${CMAKE_SOURCE_DIR}/src/Utility
        ${CMAKE_SOURCE_DIR}/src/Interface
        ${CMAKE_SOURCE_DIR}/src/Core
        ${CMAKE_SOURCE_DIR}/src/Variable
        ${CMAKE_SOURCE_DIR}/src/ACU_Persistent
        ${CMAKE_SOURCE_DIR}/src/FullCompiler
        )
