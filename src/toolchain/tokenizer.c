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

inline void readComment(Buffer* src, u_int64_t* index, u_int32_t* lineNumber){
    while(*index < src->len && src->value[*index] != '\n'){
        if(src->value[*index] == '\0'){
            return;
        }
        ++(*index);
    }
    ++(*lineNumber);
}

void readCommentBlock(Buffer* src, u_int64_t* index, u_int32_t* lineNumber){
    while(*index < src->len){
        if(src->value[*index] == '\n'){
            ++(*lineNumber);
        }
        if(src->value[*index] == '*' && src->value[*index + 1] == '/'){
            ++(*index);
            ++(*index);
            return;
        }
        ++(*index);
    }
    err("Unclosed comment block",0);
}

void readStringLiteral(Buffer* src, Buffer* target, u_int64_t* index, u_int32_t* lineNumber){
    addToBuffer(target, '"');
    ++(*index);
    while(*index < src->len){
        if(src->value[*index] == '"'){
            ++(*index);
            addToBuffer(target, '"');
            return;
        }
        if(src->value[*index] == '\n'){
            ++(*index);
            ++(*lineNumber);
            continue;
        }
        if(src->value[*index] == '\\'){
            switch (src->value[++(*index)])
            {
            case 'n':
                addToBuffer(target, '\n');
                break;
            case 'r':
                addToBuffer(target, '\r');
                break;
            case 'b':
                addToBuffer(target, '\b');
                break;
            case 'f':
                addToBuffer(target, '\f');
                break;
            case '0':
                addToBuffer(target, '\0');
                break;
            case 't':
                addToBuffer(target, '\t');
                break;
            case '"':
                addToBuffer(target, '"');
                break;
            case '\'':
                addToBuffer(target, '\'');
                break;
            case '\\':
                addToBuffer(target, '\\');
                break;
            default:
                err("Undefined escape character",0);
                break;
            }
            continue;
        }
        addToBuffer(target, src->value[*index]);
        ++(*index);
    }
    err("Unclosed string literal",0);
}

inline int readNumberLiteral(Buffer* src, Buffer* target, u_int64_t* index, u_int32_t* lineNumber){
    int points = 0;
    while(*index < src->len && (isDigit(src->value[*index]) || src->value[*index] == '.')){
        if(src->value[*index] == '.'){
            ++points;
        }
        addToBuffer(target, src->value[(*index)]);
        ++(*index);
    }
    return points > 1? -1 : points;
}

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

    while(i < src->len && src->value[i] != '\0'){
        if(src->value[i] == '\n'){
            ++lineNumber;
            columnNumber = 1;
            ++i;
            continue;
        }
        if(src->value[i] == '/' && src->value[i + 1] == '*'){
            i += 2;
            readCommentBlock(src,&i,&lineNumber);
            continue;
        }
        if(src->value[i] == '/' && src->value[i + 1] == '/'){
            i += 2;
            readComment(src,&i,&lineNumber);
            continue;
        }
        if(src->value[i] == '"'){
            readStringLiteral(src, buffer, &i, &lineNumber);
            Token tok = {StringLiteral, getBuffer(buffer), TOKEN_INFO};
            ADD_TOKEN(tok);
            continue;
        }
        if(isAlpha(src->value[i])){
            while(isAlphaNumeric(src->value[i])){
                addToBuffer(buffer, src->value[i++]);
            }
            TokenType t = keywordToType(buffer->value);
            Token tok = {t, getBuffer(buffer),TOKEN_INFO};
            ADD_TOKEN(tok);
            continue;
        }
        if(isDigit(src->value[i])){
            int number = readNumberLiteral(src,buffer,&i,&lineNumber);
            if(number == -1){
                lexicalErr("Non valid numeric literal",file,buffer->value,lineNumber);
                clearBuffer(buffer);
                continue;
            }
            Token tok = {number == 1? NumericLiteral : IntLiteral, getBuffer(buffer), TOKEN_INFO};
            ADD_TOKEN(tok);
            continue;
        }
        if(isSymbol(src->value[i])){
            addToBuffer(buffer, src->value[i++]);
            if(i <= src->len && isSymbol(src->value[i])){
                addToBuffer(buffer, src->value[i]);
                TokenType t = symbolToType(buffer->value);
                if(t != TokenError){
                    Token tok = {t, getBuffer(buffer),TOKEN_INFO};
                    ADD_TOKEN(tok);
                    continue;
                }
                popFromBuffer(buffer);
            }
            TokenType t = symbolToType(buffer->value);
            if(t == TokenError){
                lexicalErr("Unexpected token", file, buffer->value, lineNumber);
                continue;
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