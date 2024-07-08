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

TokenList* tokenize(Buffer* src){
    TokenList* tokens = createTokenList(src->len);
    Buffer* buffer = createBuffer(DEFAULT_TOKENIZER_BUFFER_SIZE);
    unsigned long i = 0;
    unsigned int lineNumber = 1;
    unsigned int columnNumber = 1;
    unsigned char commentType = 0;

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
            Token tok = {StringLiteral, getBuffer(buffer), lineNumber, columnNumber};
            ADD_TOKEN(tok);
            continue;
        }
        if(isalpha(src->value[i])){
            while(isalnum(src->value[i]) || src->value[i] == '_'){
                addToBuffer(buffer, src->value[i++]);
            }
            TokenType t = keywordToType(buffer->value);
            const char* value = getBuffer(buffer);
            Token tok = {t, value, 0, 0};
            ADD_TOKEN(tok);
            continue;
        }
        if(isdigit(src->value[i])){
            int isNumber = 0;
            while(isdigit(src->value[i]) || src->value[i]=='_'){
                if(src->value[i]=='_'){
                    if(isNumber == 1){
                        // err
                    }
                    isNumber = 1;
                }
                addToBuffer(buffer, src->value[i++]);
            }
            Token tok = {isNumber == 1? NumericLiteral : IntLiteral, getBuffer(buffer), 0, 0};
            ADD_TOKEN(tok);
            continue;
        }
        if(ispunct(src->value[i])){
            addToBuffer(buffer, src->value[i++]);

            if(i <= src->len && ispunct(src->value[i])){
               

                addToBuffer(buffer, src->value[i]);
                TokenType t = symbolToType(buffer->value);
                if(t == Comment){
                    clearBuffer(buffer);
                    continue;
                }
                if(t != TokenError){
                    Token tok = {t, getBuffer(buffer), lineNumber, 0};
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
            Token tok = {t, getBuffer(buffer), lineNumber, 0};
            ADD_TOKEN(tok);
            continue;
        }
        if(isspace(src->value[i])){
            ++i;
            ++columnNumber;
            continue;
        }

        printf("Unexpected token %c at line %d.\n",src->value[i], lineNumber);
        ++columnNumber;
        ++i;
    }
    deleteBuffer(buffer);
    return tokens;
}