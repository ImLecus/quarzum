/*
 * Quarzum Compiler - source.hpp
 * Version 1.0, 02/07/2024
 *
 * This file is part of the Quarzum project, a proprietary software.
 *
 * Quarzum Project License
 * ------------------------
 *
 * For Contributions License Agreement (CLA), see CONTRIBUTING.md.
 * For full details, see LICENSE.
 */
#include "string.h"

bool format(const char*& filename, const char* format){
    for(u_int16_t i = 0; i < strlen(filename) and i < strlen(format); ++i){
        if(filename[strlen(filename) - i] != format[strlen(format) - i]){
            return false;
        }
    }

    return true;
}

const std::string getSource(const char*& filename){
    std::ifstream inputFile(filename);
    std::string content;
    char c;
    if (not inputFile.is_open()) {
        return content;
    }
    while (inputFile.get(c)) {
        content += c;
    }
    inputFile.close();
    return std::move(content);
}  