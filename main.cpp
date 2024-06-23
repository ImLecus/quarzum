#include "./src/Quarzum.h"
#include <fstream>
#include <chrono>

using namespace quarzum;
using namespace quarzum::debug;
int main(const int argc,const char** argv) {
    
    if(argc < 2){
        throwError("No file specified.");
    }
    const char* filename = argv[1];
    const std::vector<char> content = getSource(filename);
    if(content.empty()){
        throwError("File not found.");
    }
    if(not format(filename, ".qz")){
        debug::warn("File format should be .qz or .quarzum.");
    }
    debug::log("Starting the compiler...");
    auto start = std::chrono::high_resolution_clock::now();
    TokenList tokens = tokenize(content);
    debug::log("Lex phase finished correctly. " + std::to_string(tokens.size()) + " tokens found.");
    debug::debug_time(start, "Lex phase");
    Parser parser = Parser(tokens);
    
    symbolTable.enterScope();
    symbolTable.insert("out", {'f', "out", "function", "global"});
    symbolTable.insert("input", {'f', "input", "string", "global"});
    symbolTable.insert("strcat", {'f', "strcat", "function", "global"});
    symbolTable.insert("wait", {'f', "wait", "function", "global"});
    RootNode root = parser.parse();
    debug::debug_time(start, "Parse phase");
    root.check();
    debug::debug_time(start, "Check phase");
    root.generateIR();
    ir.push_back(IRInstruction{EXIT, "0"});
    debug::debug_time(start, "IR phase");
    debug::log("Using architecture x86_64.");
    Assembler* assembler = getAssembler(ir);
    std::ofstream output("output.asm");
    if(output.is_open()){
        output << assembler->assemble();
        output.close();
        debug::debug_time(start);
        system("as output.asm -o output.o"); 
        system("ld output.o ./builtins/x86_64.o -o output");
        system("./output");
        //system("rm output.asm");
        system("rm output.o");
    }
    return 0;
}