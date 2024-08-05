cmake_minimum_required(VERSION 3.10)
project(quarzum VERSION 0.1.0 LANGUAGES C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED True)
set(SOURCES
    main.c
    src/string.c
    src/bsearch.c
    src/process.c
    src/io.c
    src/vector.c
    src/parse.c
    src/type.c
    src/tokenize.c 
    src/expr.c
    src/check.c
    src/codegen.c
    src/ir.c
    src/symbol.c
)
add_executable(quarzum ${SOURCES})
set(CMAKE_C_FLAGS_RELEASE "-O3")
