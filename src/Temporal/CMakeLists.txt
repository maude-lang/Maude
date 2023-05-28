set(temporal_sources
        logicFormula.cc
        buchiAutomaton2.cc
        transitionSet.cc
        veryWeakAlternatingAutomaton.cc
        rawTransitionSet.cc
        genBuchiAutomaton.cc
        modelChecker2.cc
        )

set(temporal_headers
        buchiAutomaton2.hh
        genBuchiAutomaton.hh
        logicFormula.hh
        modelChecker2.hh
        rawTransitionSet.hh
        temporal.hh
        transitionSet.hh
        veryWeakAlternatingAutomaton.hh
        )

add_library(temporal STATIC ${temporal_sources} ${temporal_headers})

# Set include directories for libtemporal.a
target_include_directories(temporal PRIVATE
        ${CMAKE_SOURCE_DIR}/src/Utility
        )
