
if(ENABLE_COMPILER)
    set(fullcompiler_sources
            compilationContext.cc
            )
    set(runtime_sources
            runtime.cc
            )

    set(fullcompiler_headers
            compilationContext.hh
            fullCompiler.hh
            variableName.hh
            runtime.hh
            )

    add_library(fullcompiler STATIC ${fullcompiler_sources} ${fullcompiler_headers})
    add_library(runtime STATIC ${runtime_sources} ${fullcompiler_headers})

    # Set include directories for fullcompiler.a
    target_include_directories(fullcompiler PRIVATE
            ${CMAKE_SOURCE_DIR}/src/Utility
            ${CMAKE_SOURCE_DIR}/src/Interface
            ${CMAKE_SOURCE_DIR}/src/Core
            ${CMAKE_SOURCE_DIR}/src/FullCompiler
            )
    target_include_directories(runtime PRIVATE
            ${CMAKE_SOURCE_DIR}/src/Utility
            ${CMAKE_SOURCE_DIR}/src/FullCompiler
            )
endif ()
