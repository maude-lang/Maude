
set(ACU_Persistent_sources
        ACU_RedBlackNode.cc
        ACU_Tree.cc
        )

set(ACU_Persistent_headers
        ACU_FastIter.hh
        ACU_Persistent.hh
        ACU_RedBlackNode.hh
        ACU_Stack.hh
        ACU_Pair.hh
        ACU_RedBlack.hh
        ACU_SlowIter.hh
        ACU_Tree.hh
        )

add_library(ACU_Persistent STATIC ${ACU_Persistent_headers} ${ACU_Persistent_sources})

# Set include directories for core.a
target_include_directories(ACU_Persistent PRIVATE
        ${CMAKE_SOURCE_DIR}/src/Utility
        ${CMAKE_SOURCE_DIR}/src/Interface
        ${CMAKE_SOURCE_DIR}/src/Core
        ${CMAKE_SOURCE_DIR}/src/Variable
        ${CMAKE_SOURCE_DIR}/src/FullCompiler
        )
