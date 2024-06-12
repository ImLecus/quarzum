#include <string>
#include <fstream>
#include "string.h"

bool format(const char* filename, const char* format){
    for(u_int16_t i = 0; i < strlen(filename) && i < strlen(format); ++i){
        if(filename[strlen(filename) - i] != format[strlen(format) - i]){
            return false;
        }
    }
    return true;
}

std::string getSource(const char* filename){
    std::ifstream inputFile(filename);

    if (!inputFile.is_open()) {
        return "";
    }

    std::string content;
    std::string line;
    while (std::getline(inputFile, line)) {
        content.append(line);
        content += '\n';
    }
    inputFile.close();
    return content;

}  