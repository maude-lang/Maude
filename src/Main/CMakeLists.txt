add_executable(maude main.cc)

target_include_directories(maude PRIVATE
        ../Utility
        ../Temporal
        ../Interface
        ../Core
        ../Variable
        ../FullCompiler
        ../Higher
        ../CUI_Theory
        ../S_Theory
        ../NA_Theory
        ../FreeTheory
        ../ObjectSystem
        ../Mixfix
        ../BuiltIn
        ../SMT
        ../IO_Stuff
        ../ACU_Persistent
        ../ACU_Theory
        ../AU_Persistent
        ../AU_Theory
        ../Meta
        ../3rdParty
        ../FullCompiler
        ../StrategyLanguage
        ../Mixfix
        )

target_link_libraries(maude PRIVATE
        mixfix
        strategyLanguage
        meta
        objectSystem
        builtIn
        SMT
        higher
        freeTheory
        AU_Theory
        AU_Persistent
        ACU_Theory
        ACU_Persistent
        CUI_Theory
        S_Theory
        NA_Theory
        variable
        interface
        core
        parser
        temporal
        IO_Stuff
        utility
        ${EXTRA_LIBS} # GMP, Yices2, etc.
        )

if(ENABLE_COMPILER)
    target_link_libraries(maude PRIVATE fullCompiler)
endif()

# For installation of data files (optional)
#install(FILES
#        prelude.maude
#        model-checker.maude
#        socket.maude
#        linear.maude
#        machine-int.maude
#        term-order.maude
#        metaInterpreter.maude
#        smt.maude
#        file.maude
#        process.maude
#        time.maude
#        prng.maude
#        DESTINATION ${CMAKE_INSTALL_DATADIR}
#        )
