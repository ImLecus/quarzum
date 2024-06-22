#include <fstream>
#include <vector>
#include "string.h"

bool format(const char*& filename, const char* format){
    for(u_int16_t i = 0; i < strlen(filename) and i < strlen(format); ++i){
        if(filename[strlen(filename) - i] != format[strlen(format) - i]){
            return false;
        }
    }
    return true;
}

const std::vector<char> getSource(const char*& filename){
    std::ifstream inputFile(filename);
    std::vector<char> content;
    char c;
    if (not inputFile.is_open()) {
        return content;
    }
    while (inputFile.get(c)) {
        content.push_back(c);
    }
    inputFile.close();
    return content;
}  