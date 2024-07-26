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

inline bool isAlpha(char c){
    return c >= 'A' && (c <= 'Z' || c >= 'a' && c <= 'z');
}

inline bool isDigit(char c, char base) {
    switch (base)
    {
    case 'd':
        return c >= '0' && c <= '9';
    case 'b':
        return c == '0' || c == '1';
    case 'o':
       return c >= '0' && c <= '7';
    case 'x':
       return (c >= '0' && c <= '9') || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f');
    default: return false;
    }
    
}

inline bool isSymbol(char c){
    return (c >= '!' && c <= '~') && !isAlphaNumeric(c);
}

inline bool isAlphaNumeric(char c){
    return isAlpha(c) || isDigit(c, 'd') || c == '_';
}

inline bool isSpace(char c){
    return c == ' ';
}