#include "src/tokenizer/tokenizer.hpp"
#include "src/parser/parser.hpp"
#include "src/source.hpp"
#include "src/semantics/semantics.hpp"
#include "src/debug.hpp"

int main(int argc, char** argv) {

    if(argc < 2){
        throwError("No file specified.");
    }
    std::string filename = argv[1];
    std::string content = getSource(filename);
    if(content == ""){
        throwError("File not found.");
    }
    if(not format(filename, ".qz")){
        throwError("File format must be .qz or .quarzum.");
    }
    
    TokenList tokens = Tokenizer::tokenize(content);
    Parser parser = Parser(tokens);
    RootNode root = parser.parse();

    //Semantics s = Semantics();
    //s.check(root);

    //std::cout << s.rootToIR(root) << '\n';


    return 0;
}