
set(builtIn_sources
        equalitySymbol.cc
        sortTestSymbol.cc
        branchSymbol.cc
        stringSymbol.cc
        stringTerm.cc
        stringDagNode.cc
        stringOpSymbol.cc
        floatSymbol.cc
        floatTerm.cc
        floatDagNode.cc
        floatOpSymbol.cc
        succSymbol.cc
        numberOpSymbol.cc
        minusSymbol.cc
        ACU_NumberOpSymbol.cc
        CUI_NumberOpSymbol.cc
        divisionSymbol.cc
        randomOpSymbol.cc
        counterSymbol.cc
        matrixOpSymbol.cc
        equalityExtor.cc
        equalityExtorFinal.cc
        )

set(builtIn_headers
        ACU_NumberOpSymbol.hh
        CUI_NumberOpSymbol.hh
        bindingMacros.hh
        branchSymbol.hh
        builtIn.hh
        divisionSymbol.hh
        equalitySymbol.hh
        floatDagNode.hh
        floatOpSymbol.hh
        floatSymbol.hh
        floatTerm.hh
        minusSymbol.hh
        numberOpSymbol.hh
        sortTestSymbol.hh
        stringDagNode.hh
        stringOpSymbol.hh
        stringSymbol.hh
        stringTerm.hh
        succSymbol.hh
        randomOpSymbol.hh
        counterSymbol.hh
        matrixOpSymbol.hh
        equalityExtor.hh
        equalityExtorFinal.hh
        )

add_library(builtIn STATIC ${builtIn_sources} ${builtIn_headers})

# Set include directories for builtIn.a
target_include_directories(builtIn PRIVATE
        ${CMAKE_SOURCE_DIR}/src/3rdParty
        ${CMAKE_SOURCE_DIR}/src/ACU_Persistent
        ${CMAKE_SOURCE_DIR}/src/ACU_Theory
        ${CMAKE_SOURCE_DIR}/src/Core
        ${CMAKE_SOURCE_DIR}/src/CUI_Theory
        ${CMAKE_SOURCE_DIR}/src/FreeTheory
        ${CMAKE_SOURCE_DIR}/src/FullCompiler
        ${CMAKE_SOURCE_DIR}/src/Interface
        ${CMAKE_SOURCE_DIR}/src/NA_Theory
        ${CMAKE_SOURCE_DIR}/src/S_Theory
        ${CMAKE_SOURCE_DIR}/src/Utility
        ${CMAKE_SOURCE_DIR}/src/Variable
        )
