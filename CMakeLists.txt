cmake_minimum_required(VERSION 3.10)
project(quarzum VERSION 0.1.0 LANGUAGES CXX)
enable_testing()
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)
add_executable(quarzum ${SOURCES}
        src/lexer/token.cpp
        src/lexer/lexer.cpp
        src/parser/tests/parser_test.cpp
        src/parser/parser.cpp
        src/parser/include/parser.hpp
        src/ast/src/blocknode.cpp
        src/ast/src/vardeclarationnode.cpp
        src/ast/src/boolliteralnode.cpp
        src/parser/tests/parser_test.cpp
        src/ast/src/functiondeclarationnode.cpp
        src/ast/src/loopstatementnode.cpp
        src/ast/include/returnstatementnode.hpp
        src/ast/src/returnstatementnode.cpp
        src/ast/include/breaknode.hpp
        src/ast/include/continuenode.hpp
        src/ast/include/ifstatementnode.hpp
        src/ast/src/ifstatementnode.cpp
        src/ast/include/whilestatementnode.hpp
        src/ast/src/whilestatementnode.cpp
        src/ast/include/binaryexpressionnode.hpp
        src/ast/src/binaryexpressionnode.cpp
        src/ast/include/dowhilestatementnode.hpp
        src/ast/src/dowhilestatementnode.cpp
        src/ast/include/switchstatementnode.hpp
        src/ast/src/switchstatementnode.cpp
        src/ast/include/integerliteralnode.hpp
)
target_include_directories(quarzum PRIVATE
        src/lexer/include
        src/ast/include
        src/parser/include
)
target_compile_features(quarzum PRIVATE cxx_std_17)
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -Os -Wall -Wextra -Wpedantic")

