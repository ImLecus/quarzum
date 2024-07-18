/*
 * Quarzum Compiler - tokenizer.c
 * Version 1.0, 08/07/2024
 *
 * This file is part of the Quarzum project, a proprietary software.
 *
 * Quarzum Project License
 * ------------------------
 *
 * For Contributions License Agreement (CLA), see CONTRIBUTING.md.
 * For full details, see LICENSE.
 */
#include "../include/toolchain/tokenizer.h"

TokenList* tokenize(char* file){
    Buffer* src = read(file);
    if(src == NULL){
        return NULL;
    }
    TokenList* tokens = createTokenList(src->len);
    Buffer* buffer = createBuffer(DEFAULT_TOKENIZER_BUFFER_SIZE);
    
    u_int64_t i = 0;
    u_int32_t lineNumber = 1;
    u_int32_t columnNumber = 1;
    u_char commentType = 0;

    while(i < src->len && src->value[i] != '\0'){
        if(src->value[i] == '\n'){
            ++lineNumber;
            columnNumber = 1;
            ++i;
            continue;
        }
        if(i < src->len && src->value[i] == '/' && src->value[i + 1] == '*'){
            commentType = 2;
            i += 2;
            continue;
        }
        if(i < src->len && src->value[i] == '*' && src->value[i + 1] == '/' && commentType ==  2){
            commentType = 0;
            i += 2;
            continue;
        }
        if(commentType != 0){
            ++i;
            ++columnNumber;
            continue;
        }
        if(src->value[i] == '"'){
            addToBuffer(buffer,'"');

            ++columnNumber;
            ++i;
            while(i <= src->len && src->value[i] != '"'){
                addToBuffer(buffer,src->value[i++]);
                ++columnNumber;
            }
            addToBuffer(buffer,'"');
            ++i;
            ++columnNumber;
            Token tok = {StringLiteral, getBuffer(buffer), TOKEN_INFO};
            ADD_TOKEN(tok);
            continue;
        }
        if(isAlpha(src->value[i])){
            while(isAlphaNumeric(src->value[i]) || src->value[i] == '_'){
                addToBuffer(buffer, src->value[i++]);
            }
           
            TokenType t = keywordToType(buffer->value);

            Token tok = {t, getBuffer(buffer),TOKEN_INFO};
            ADD_TOKEN(tok);
            continue;
        }
        if(isDigit(src->value[i])){
            int isNumber = 0;
            while(isDigit(src->value[i]) || src->value[i]=='.'){
                if(src->value[i]=='.'){
                    if(isNumber == 1){
                        // err
                    }
                    isNumber = 1;
                }
                addToBuffer(buffer, src->value[i++]);
            }
            Token tok = {isNumber == 1? NumericLiteral : IntLiteral, getBuffer(buffer), TOKEN_INFO};
            ADD_TOKEN(tok);
            continue;
        }
        if(isSymbol(src->value[i])){
            addToBuffer(buffer, src->value[i++]);

            if(i <= src->len && isSymbol(src->value[i])){
               

                addToBuffer(buffer, src->value[i]);
                TokenType t = symbolToType(buffer->value);
                if(t == Comment){
                    clearBuffer(buffer);
                    continue;
                }
                if(t != TokenError){
                    Token tok = {t, getBuffer(buffer),TOKEN_INFO};
                    ADD_TOKEN(tok);
                    continue;
                }
                popFromBuffer(buffer);
            }

            TokenType t = symbolToType(buffer->value);
            if(t == TokenError){
                Token errorToken = {t,NULL,0,0};
                ADD_TOKEN(errorToken);
            }
            Token tok = {t, getBuffer(buffer), TOKEN_INFO};
            ADD_TOKEN(tok);
            continue;
        }
        if(isSpace(src->value[i])){
            ++i;
            ++columnNumber;
            continue;
        }
        lexicalErr("Unexpected token", file, &src->value[i], lineNumber);
        ++columnNumber;
        ++i;
    }
    deleteBuffer(src);
    deleteBuffer(buffer);
    return tokens;
}