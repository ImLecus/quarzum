cmake_minimum_required(VERSION 3.10)
project(quarzum VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED True)

include_directories(include)
set(SOURCES
    main.c
    src/core/buffer.c
    src/core/bsearch.c
    src/core/process.c
    src/core/debug.c

    src/internal/io.c
    src/internal/tokenlist.c
    src/internal/defs.c
    src/internal/ast.c
    src/internal/expr.c
    src/internal/cli.c
    src/internal/type.c

    src/toolchain/tokenizer.c
    src/toolchain/compile.c
    src/toolchain/parser.c
)
add_executable(quarzum ${SOURCES})

