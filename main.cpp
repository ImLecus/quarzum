#include "src/tokenizer/tokenizer.cpp"
#include "src/parser/parser.cpp"
#include "src/source.hpp"
#include "src/ir/irinstruction.hpp"
#include "src/backend/assembler.hpp"
#include <memory>
#include <fstream>

int main(const int argc,const char** argv) {

    if(argc < 2){
        throwError("No file specified.");
    }
    const char* filename = argv[1];
    const std::string content = getSource(filename);
    if(not content[0]){
        throwError("File not found.");
    }
    if(not format(filename, ".qz")){
        throwError("File format must be .qz or .quarzum.");
    }
    Parser parser = Parser(tokenize(content));
    symbolTable.enterScope();
    symbolTable.insert("out", {'f', "out", "function", "global"});
    symbolTable.insert("input", {'f', "input", "string", "global"});
    
    RootNode root = parser.parse();

    root.check();
    root.generateIR();
    ir.push_back(IRInstruction{EXIT, "0"});
    std::unique_ptr<Assembler> assembler = getAssembler(ir);
    std::ofstream output("output.asm");
    if(output.is_open()){
        output << assembler->assemble();
        output.close();
        system("as output.asm -o output.o"); 
        system("ld output.o ./builtins/x86_64.o -o output");
        system("./output");
        //system("rm output.asm");
        system("rm output.o");
    }
    return 0;
}