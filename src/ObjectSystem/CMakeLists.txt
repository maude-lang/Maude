set(objectSystem_sources
        configSymbol.cc
        objectSystemRewritingContext.cc
        externalObjectManagerSymbol.cc
        socketManagerSymbol.cc
        pseudoThread.cc
        fileManagerSymbol.cc
        streamManagerSymbol.cc
        processManagerSymbol.cc
        timeManagerSymbol.cc
        directoryManagerSymbol.cc
        prngManagerSymbol.cc
        )

set(objectSystem_headers
        configSymbol.hh
        objectSystem.hh
        objectSystemRewritingContext.hh
        externalObjectManagerSymbol.hh
        socketManagerSymbol.hh
        pseudoThread.hh
        fileManagerSymbol.hh
        streamManagerSymbol.hh
        processManagerSymbol.hh
        timeManagerSymbol.hh
        directoryManagerSymbol.hh
        prngManagerSymbol.hh
        )

add_library(objectSystem STATIC ${objectSystem_sources} ${objectSystem_headers})

# Set include directories for libobjectSystem.a
target_include_directories(objectSystem PRIVATE
        ${CMAKE_SOURCE_DIR}/src/Utility
        ${CMAKE_SOURCE_DIR}/src/Interface
        ${CMAKE_SOURCE_DIR}/src/Core
        ${CMAKE_SOURCE_DIR}/src/Variable
        ${CMAKE_SOURCE_DIR}/src/FullCompiler
        ${CMAKE_SOURCE_DIR}/src/ACU_Persistent
        ${CMAKE_SOURCE_DIR}/src/ACU_Theory
        ${CMAKE_SOURCE_DIR}/src/AU_Persistent
        ${CMAKE_SOURCE_DIR}/src/AU_Theory
        ${CMAKE_SOURCE_DIR}/src/FreeTheory
        ${CMAKE_SOURCE_DIR}/src/NA_Theory
        ${CMAKE_SOURCE_DIR}/src/S_Theory
        ${CMAKE_SOURCE_DIR}/src/BuiltIn
        ${CMAKE_SOURCE_DIR}/src/3rdParty
        )
