# Determine header files
set(mixfix_headers
        renaming.hh
        view.hh
        syntacticView.hh
        entity.hh
        moduleCache.hh
        moduleExpression.hh
        viewCache.hh
        viewExpression.hh
        sharedTokens.hh
        compiler.hh
        fileTable.hh
        importModule.hh
        importTranslation.hh
        interpreter.hh
        lexerAux.hh
        loopSymbol.hh
        global.hh
        maudemlBuffer.hh
        metadataStore.hh
        printAttribute.hh
        mixfix.hh
        mixfixModule.hh
        mixfixParser.hh
        moduleDatabase.hh
        viewDatabase.hh
        preModule.hh
        syntacticPreModule.hh
        profileModule.hh
        quotedIdentifierDagNode.hh
        quotedIdentifierOpSymbol.hh
        quotedIdentifierSymbol.hh
        quotedIdentifierTerm.hh
        symbolType.hh
        token.hh
        userLevelRewritingContext.hh
        visibleModule.hh
        xmlBuffer.hh
        syntaxContainer.hh
        freshVariableSource.hh
        variableGenerator.hh
        cacheableRewritingContext.hh
        cvc4_Bindings.hh
        yices2_Bindings.hh
        enclosingObject.hh
        argument.hh
        parameter.hh
        parameterDatabase.hh
        objectConstructorSymbol.hh
        statementTransformer.hh)

# Determine source files
set(mixfix_sources
        parameterDatabase.cc
        syntaxContainer.cc
        renaming.cc
        view.cc
        syntacticView.cc
        entity.cc
        moduleCache.cc
        moduleExpression.cc
        viewExpression.cc
        viewCache.cc
        enclosingObject.cc
        importModule.cc
        importTranslation.cc
        strategyTranslation.cc
        fileTable.cc
        loopSymbol.cc
        quotedIdentifierSymbol.cc
        quotedIdentifierDagNode.cc
        quotedIdentifierTerm.cc
        quotedIdentifierOpSymbol.cc
        mixfixModule.cc
        token.cc
        global.cc
        userLevelRewritingContext.cc
        preModule.cc
        syntacticPreModule.cc
        symbolType.cc
        xmlBuffer.cc
        maudemlBuffer.cc
        interpreter.cc
        moduleDatabase.cc
        viewDatabase.cc
        profileModule.cc
        mixfixParser.cc
        visibleModule.cc
        sharedTokens.cc
        metadataStore.cc
        printAttribute.cc
        banner.cc
        lexer.cc
        tokenizer.cc
        deserializer.cc
        surface.cc
        freshVariableSource.cc
        variableGenerator.cc
        objectConstructorSymbol.cc
        surface.yy)

# Add optional compiler source
#option(ENABLE_COMPILER "Build with compiler source" OFF)
if(ENABLE_COMPILER)
    list(APPEND mixfix_sources compiler.cc)
endif()

# Add include directories
include_directories(
        ${CMAKE_SOURCE_DIR}/src/Utility
        ${CMAKE_SOURCE_DIR}/src/Temporal
        ${CMAKE_SOURCE_DIR}/src/Interface
        ${CMAKE_SOURCE_DIR}/src/Core
        ${CMAKE_SOURCE_DIR}/src/Variable
        ${CMAKE_SOURCE_DIR}/src/FullCompiler
        ${CMAKE_SOURCE_DIR}/src/Higher
        ${CMAKE_SOURCE_DIR}/src/CUI_Theory
        ${CMAKE_SOURCE_DIR}/src/S_Theory
        ${CMAKE_SOURCE_DIR}/src/NA_Theory
        ${CMAKE_SOURCE_DIR}/src/FreeTheory
        ${CMAKE_SOURCE_DIR}/src/ObjectSystem
        ${CMAKE_SOURCE_DIR}/src/Mixfix
        ${CMAKE_SOURCE_DIR}/src/BuiltIn
        ${CMAKE_SOURCE_DIR}/src/Parser
        ${CMAKE_SOURCE_DIR}/src/IO_Stuff
        ${CMAKE_SOURCE_DIR}/src/ACU_Persistent
        ${CMAKE_SOURCE_DIR}/src/ACU_Theory
        ${CMAKE_SOURCE_DIR}/src/AU_Persistent
        ${CMAKE_SOURCE_DIR}/src/AU_Theory
        ${CMAKE_SOURCE_DIR}/src/Meta
        ${CMAKE_SOURCE_DIR}/src/3rdParty
        ${CMAKE_SOURCE_DIR}/src/FullCompiler
        ${CMAKE_SOURCE_DIR}/src/StrategyLanguage
        ${CMAKE_SOURCE_DIR}/src/SMT)

# Create the library
add_library(mixfix STATIC ${mixfix_sources} ${mixfix_headers})

# Prep the Bison parser file surface.yy.
# TODO: There seems to be a conflict with dependencies and file generation order for `surface.yy`.
set(YY_FILES top.yy modules.yy commands.yy bottom.yy)
# Create full paths for the source files
list(TRANSFORM YY_FILES PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/")
message(STATUS "cat ${YY_FILES} > surface.yy")
add_custom_command(
        OUTPUT surface.yy
        COMMAND cat ${YY_FILES} > ${CMAKE_CURRENT_BINARY_DIR}/surface.yy
        DEPENDS ${YY_FILES}
        COMMENT "Generating surface.yy"
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        VERBATIM
)

# Bison and Flex generation
BISON_TARGET(surface_parser ${CMAKE_CURRENT_BINARY_DIR}/surface.yy ${CMAKE_CURRENT_BINARY_DIR}/surface.cc DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/surface.h)
FLEX_TARGET(surface_lexer ${CMAKE_CURRENT_SOURCE_DIR}/lexer.ll ${CMAKE_CURRENT_BINARY_DIR}/lexer.cc)
FLEX_TARGET(tokenizer ${CMAKE_CURRENT_SOURCE_DIR}/tokenizer.ll ${CMAKE_CURRENT_BINARY_DIR}/tokenizer.cc COMPILE_FLAGS "-Pzz")
FLEX_TARGET(tokenizer ${CMAKE_CURRENT_SOURCE_DIR}/deserializer.ll ${CMAKE_CURRENT_BINARY_DIR}/deserializer.cc COMPILE_FLAGS "-Pxx")
ADD_FLEX_BISON_DEPENDENCY(surface_lexer surface_parser)

# Dependencies conflict with file generation order.
#add_dependencies(mixfix surface_lexer)

# Include the generated files in our sources
list(
        APPEND
        mixfix_sources
        ${BISON_surface_parser_OUTPUTS}
        ${FLEX_surface_lexer_OUTPUTS}
        ${FLEX_tokenizer_OUTPUTS}
        ${FLEX_deserializer_OUTPUTS}
)

# Remove generated files on "make clean"
set_directory_properties(
        PROPERTIES
        ADDITIONAL_MAKE_CLEAN_FILES "${BISON_surface_parser_OUTPUTS};${FLEX_surface_lexer_OUTPUTS};${FLEX_tokenizer_OUTPUTS};${FLEX_deserializer_OUTPUTS}"
)
