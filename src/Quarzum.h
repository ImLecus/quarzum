/**
 * Quarzum namespace organization file
 * 
 * This file contains all the dependencies between files
 * and the namespace organization design.
 * 
 * Quarzum namespaces are divided following this pattern:
 * - Debug: Generally used by all the modules to show errors or other information.
 * - Console: Used to parse arguments by console.
 * - Lex: Related to the tokenizer
 * - Parse: Related to the parsing method and AST Tree
 * - Types: Used for symbol tables and type operation rules
 * - IntermediateRepresentation: Related to the IR generation process
 * - Assembly: Related to the IR=>ASM translation.
*/
#pragma once
#define Q_VERSION "0.1.0" 

#include <string>
#include <memory>
#include <fstream>
#include <chrono>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <cmath>
#include <deque>

#include "./debug/debug.cpp"
#include "./tokenizer/tokenizer.cpp"
#include "./parser/parser.cpp"
#include "source.hpp"
//#include "./ir/irinstruction.hpp"
//#include "./backend/assembler.hpp"
namespace Quarzum {
    constexpr bool debugMode = false;
    namespace Debug = Quarzum::Debug;
    namespace Lexer = Quarzum::Lexer;
}
