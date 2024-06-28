#include "./src/Quarzum.h"

using namespace Quarzum;
using namespace Debug;
using namespace Lexer;
int main(const int argc,const char** argv) {
    
    if(argc < 2){
        throwError("No file specified.");
    }
    const char* filename = argv[1];
    const std::string content = getSource(filename);
    if(content.empty()){
        throwError("File not found.");
    }
    if(not format(filename, ".qz")){
        warn("File format should be .qz or .quarzum.");
    }
    log("Starting the compiler...");
    log("Using architecture x86_64. Use the --a flag to change it.");
    auto start = std::chrono::high_resolution_clock::now();
    Debug::source = std::make_unique<std::string>(content);
    
    std::deque<Token> tokens = tokenize(content);

    log("Lex phase finished correctly. " + std::to_string(tokens.size()) + " tokens found.");
    debugTime(start, "Lex phase");

    Parser parser = Parser(tokens);
    
    // symbolTable.enterScope();
    // symbolTable.insert("out", {'f', "out", "function", "global"});
    // symbolTable.insert("input", {'f', "input", "string", "global"});
    // symbolTable.insert("strcat", {'f', "strcat", "function", "global"});
    // symbolTable.insert("wait", {'f', "wait", "function", "global"});
    std::unique_ptr<RootNode> root = parser.parse();
    debugTime(start, "Parse phase");
    //root->check();
    //debugTime(start, "Check phase");
    // root.generateIR();
    // ir.push_back(IRInstruction{EXIT, "0"});
    // debugTime(start, "IR phase");
    
    // Assembler* assembler = getAssembler(ir);
    // std::ofstream output("output.asm");
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
    Debug::exit(0);
    return 0;
}