set(IO_Stuff_sources
        IO_Manager.cc
        autoWrapBuffer.cc
        directoryManager.cc
        )

set(IO_Stuff_headers
        IO_Manager.hh
        autoWrapBuffer.hh
        directoryManager.hh
        )

add_library(IO_Stuff STATIC ${IO_Stuff_sources} ${IO_Stuff_headers})

# Set include directories for libIO_Stuff.a
target_include_directories(IO_Stuff PRIVATE
        ${CMAKE_SOURCE_DIR}/src/Utility
        )
