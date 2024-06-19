#include "src/tokenizer/tokenizer.cpp"
#include "src/parser/parser.cpp"
#include "src/source.hpp"

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
    RootNode root = parser.parse();
    root.check();

    return 0;
}