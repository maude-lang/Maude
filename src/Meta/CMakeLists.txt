set(meta_sources
        metaModuleCache.cc
        metaLevel.cc
        interpreterManagerSymbol.cc
        metaOpCache.cc
        metaModule.cc
        metaLevelOpSymbol.cc
        metaPreModule.cc
        metaView.cc
        )

set(meta_headers
        meta.hh
        metaLevel.hh
        interpreterManagerSymbol.hh
        metaLevelOpSymbol.hh
        metaModule.hh
        metaModuleCache.hh
        metaOpCache.hh
        metaPreModule.hh
        metaView.hh
        )

add_library(meta STATIC ${meta_sources} ${meta_headers})

# Set include directories for libmeta.a
target_include_directories(meta PRIVATE
        ${CMAKE_SOURCE_DIR}/src/Utility
        ${CMAKE_SOURCE_DIR}/src/Interface
        ${CMAKE_SOURCE_DIR}/src/Core
        ${CMAKE_SOURCE_DIR}/src/Variable
        ${CMAKE_SOURCE_DIR}/src/FullCompiler
        ${CMAKE_SOURCE_DIR}/src/Higher
        ${CMAKE_SOURCE_DIR}/src/AU_Theory
        ${CMAKE_SOURCE_DIR}/src/CUI_Theory
        ${CMAKE_SOURCE_DIR}/src/S_Theory
        ${CMAKE_SOURCE_DIR}/src/NA_Theory
        ${CMAKE_SOURCE_DIR}/src/FreeTheory
        ${CMAKE_SOURCE_DIR}/src/ObjectSystem
        ${CMAKE_SOURCE_DIR}/src/Mixfix
        ${CMAKE_SOURCE_DIR}/src/BuiltIn
        ${CMAKE_SOURCE_DIR}/src/IO_Stuff
        ${CMAKE_SOURCE_DIR}/src/StrategyLanguage
        ${CMAKE_SOURCE_DIR}/src/3rdParty
        ${CMAKE_SOURCE_DIR}/src/SMT
        ${CMAKE_SOURCE_DIR}/src/Parser
        )
