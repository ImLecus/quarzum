/*
 * Quarzum Compiler - char.c
 * Version 1.0, 18/07/2024
 *
 * This file is part of the Quarzum project, a proprietary software.
 *
 * Quarzum Project License
 * ------------------------
 *
 * For Contributions License Agreement (CLA), see CONTRIBUTING.md.
 * For full details, see LICENSE.
 */
#include "../../include/core/char.h"

inline bool_t isAlpha(char c){
    return c >= 'A' && (c <= 'Z' || c >= 'a' && c <= 'z');
}

inline bool_t isDigit(char c) {
    return c >= '0' && c <= '9';
}

inline bool_t isSymbol(char c){
    return (c >= '!' && c <= '~') && !isAlphaNumeric(c);
}

inline bool_t isAlphaNumeric(char c){
    return isAlpha(c) || isDigit(c) || c == '_';
}

inline bool_t isSpace(char c){
    return c == ' ';
}

char* deleteQuotes(char* string){
    char* newString = (char*)malloc(sizeof(char) *  (strlen(string) - 2));
    for(unsigned int i = 0; i < strlen(string) - 2; ++i){
       newString[i] = string[i + 1];
    }
    return newString;
}