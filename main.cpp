/*
 * Quarzum Compiler - main.cpp
 * Version 1.0, 01/07/2024
 *
 * This file is part of the Quarzum project, a proprietary software.
 *
 * Quarzum Project License
 * ------------------------
 *
 * For Contributions License Agreement (CLA), see CONTRIBUTING.md.
 * For full details, see LICENSE.
 */
#include "./src/Quarzum.h"
#include "./include/quarzum.h"

using namespace Quarzum;
using namespace Debug;
using namespace CLI;

int main(const int argc,const char** argv) {
    if(argc < 2){
        err("No file specified.");
    }

    // filename not implemented yet
    parseCLIArgs(argv);

    const char* filename = argv[1];
    const std::vector<char> content = getSource(filename);
    if(content.empty()){
        err("File not found.");
    }
    if(not format(filename, ".qz")){
        warn("File format should be .qz or .quarzum.");
    }
    log("Starting the compiler...");
    log("Using architecture x86_64. Use the --a flag to change it.");
    auto start = std::chrono::high_resolution_clock::now();
    Debug::source = std::make_unique<std::vector<char>>(content);
    
    std::deque<Token> tokens = tokenize(content);

    log("Lex phase finished correctly. " + std::to_string(tokens.size()) + " tokens found.");
    debugTime(start, "Lex phase");

    //Parser parser = Parser(tokens);
    //unique_ptr<RootNode> root = parser.parse();
    //debugTime(start, "Parse phase");
    //check(*root);
    //debugTime(start, "Check phase");
    // root.generateIR();
    // debugTime(start, "IR phase");
    
    // delete root;

    // Assembler* assembler = getAssembler(ir);
    // std::ofstream output(outputFileName + ".asm");
    // if(output.is_open()){
    //     output << assembler->assemble();
    //     debugTime(start, "Asm phase");
    //     output.close();
    //     debugTime(start);
    //     system("as output.asm -o output.o"); 
    //     system("ld output.o ./builtins/x86_64.o -o output");
    //     system("./output");
    //     //system("rm output.asm");
    //     system("rm output.o");
    // }
    //Debug::exit(0);
    return 0;
}