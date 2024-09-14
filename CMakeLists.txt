cmake_minimum_required(VERSION 3.10)
project(quarzum VERSION 0.1.0 LANGUAGES C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED True)
file(GLOB_RECURSE SOURCES src/*.c)
add_executable(quarzum ${SOURCES})
set(CMAKE_C_FLAGS_RELEASE "-O3")
