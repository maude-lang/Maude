# CMakeLists.txt generated from Makefile.am

# Add source files to the library target
set(variable_sources
        variableSymbol.cc
        variableTerm.cc
        variableDagNode.cc
        variableLhsAutomaton.cc
        )

# Add header files to the library target
set(variable_headers
        variable.hh
        variableDagNode.hh
        variableLhsAutomaton.hh
        variableSymbol.hh
        variableTerm.hh
        )

# Create the library target
add_library(variable STATIC ${variable_sources} ${variable_headers})

# Set include directories for the library
target_include_directories(variable PRIVATE
        ${CMAKE_SOURCE_DIR}/src/Utility
        ${CMAKE_SOURCE_DIR}/src/Interface
        ${CMAKE_SOURCE_DIR}/src/Variable
        ${CMAKE_SOURCE_DIR}/src/Core
        ${CMAKE_SOURCE_DIR}/src/FullCompiler
        )
