set(parser_sources
        parser.cc
        )

set(parser_headers
        parser.hh
        )

add_library(parser STATIC ${parser_sources} ${parser_headers})

# Set include directories for libparser.a
target_include_directories(parser PRIVATE
        ${CMAKE_SOURCE_DIR}/src/Utility
        )
