/*
 * Quarzum Compiler - defs.c
 * Version 1.0, 07/07/2024
 *
 * This file is part of the Quarzum project, a proprietary software.
 *
 * Quarzum Project License
 * ------------------------
 *
 * For Contributions License Agreement (CLA), see CONTRIBUTING.md.
 * For full details, see LICENSE.
 */
#include "../../include/quarzum/defs.h"

const TokenType keywordToType(const char* keyword){
    int search = binary_search(keyword, keywords, KEYWORDS_SIZE);
    if(search == -1){
        return Identifier;
    }
    return keywordTypes[search];
}

const TokenType symbolToType(const char* symbol){
    int search = binary_search(symbol, symbols, SYMBOLS_SIZE);
    if(search == -1){
        return TokenError;
    }
    return symbolTypes[search];
}