cmake_minimum_required(VERSION 3.10)
project(quarzum VERSION 0.1.0 LANGUAGES CXX)
enable_testing()
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)
add_executable(quarzum ${SOURCES}
        src/lexer/token.cpp
        src/lexer/lexer.cpp
        src/lexer/tests/lexer_test.cpp)
target_include_directories(quarzum PRIVATE src/lexer/include)
target_compile_features(quarzum PRIVATE cxx_std_17)
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -Os -Wall -Wextra -Wpedantic")

