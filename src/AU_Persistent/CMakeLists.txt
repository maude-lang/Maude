
set(AU_Persistent_sources
        AU_StackNode.cc
        AU_Deque.cc
        AU_DequeIter.cc
        )

set(AU_Persistent_headers
        AU_Deque.hh
        AU_DequeIter.hh
        AU_Persistent.hh
        AU_StackNode.hh
        )

add_library(AU_Persistent STATIC ${AU_Persistent_sources} ${AU_Persistent_headers})

# Set include directories for AU_Persistent.a
target_include_directories(AU_Persistent PRIVATE
        ${CMAKE_SOURCE_DIR}/src/Utility
        ${CMAKE_SOURCE_DIR}/src/Interface
        ${CMAKE_SOURCE_DIR}/src/Core
        ${CMAKE_SOURCE_DIR}/src/Variable
        ${CMAKE_SOURCE_DIR}/src/FullCompiler
        )
