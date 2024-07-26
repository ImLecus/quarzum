cmake_minimum_required(VERSION 3.10)
project(quarzum VERSION 0.1.0 LANGUAGES C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED True)

include_directories(include)
set(SOURCES
    main.c
    src/string.c
    src/core/bsearch.c
    src/process.c
    src/core/debug.c
    src/core/char.c 

    src/map.c
    src/io.c
    src/vector.c
    src/parse.c
    src/internal/type.c
    src/internal/symbol.c
    src/internal/ir.c
    src/tokenize.c
  
    src/check.c
)
add_executable(quarzum ${SOURCES})
set(CMAKE_C_FLAGS_RELEASE "-O3")
