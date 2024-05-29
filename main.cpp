#include "src/tokenizer/tokenizer.hpp"
#include "src/parser/parser.hpp"
#include "src/source.hpp"

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
    /*
    for(Token token : tokens.getItems()) {
        std::cout << token.toString() << '\n';
    }
    */
    ASTNode* u = new UnaryExpression("-", new IntegerLiteral(12));
    u->print();
    delete u;


    return 0;
}