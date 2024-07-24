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

inline void read_comment(Buffer* src, uint64* index, uint32* lineNumber){
    while(*index < src->len && src->value[*index] != '\n'){
        if(src->value[*index] == '\0'){
            return;
        }
        ++(*index);
    }
    ++(*lineNumber);
}

inline void read_comment_block(Buffer* src, uint64* index, uint32* lineNumber){
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

void read_string_literal(Buffer* src, Buffer* target, uint64* index, uint32* lineNumber){
    add_buffer(target, '"');
     
    ++(*index);
    while(*index < src->len){
        if(src->value[*index] == '"'){
            add_buffer(target, '"');
            ++(*index);
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
                add_buffer(target, '\n');
                break;
            case 'r':
                add_buffer(target, '\r');
                break;
            case 'b':
                add_buffer(target, '\b');
                break;
            case 'f':
                add_buffer(target, '\f');
                break;
            case '0':
                add_buffer(target, '\0');
                break;
            case 't':
                add_buffer(target, '\t');
                break;
            case '"':
                add_buffer(target, '"');
                break;
            case '\'':
                add_buffer(target, '\'');
                break;
            case '\\':
                add_buffer(target, '\\');
                break;
            default:
                err("Undefined escape character",0);
                break;
            }
            continue;
        }
        add_buffer(target, src->value[*index]);
        ++(*index);
    }
    err("Unclosed string literal",0);
}

inline int readNumberLiteral(Buffer* src, Buffer* target, uint64* index, uint32* lineNumber){
    int points = 0;
    while(*index < src->len && (isDigit(src->value[*index]) || src->value[*index] == '.')){
        if(src->value[*index] == '.'){
            ++points;
        }
        add_buffer(target, src->value[(*index)]);
        ++(*index);
    }
    return points > 1? -1 : points;
}

TokenList* tokenize(char* file){
    
    Buffer* src = readFile(file);
    if(src == NULL){
        return NULL;
    }
    TokenList* tokens = init_tokenlist(src->len);
    Buffer* buffer = init_buffer(DEFAULT_TOKENIZER_BUFFER_SIZE);
    
    uint64 i = 0;
    uint32 lineNumber = 1;
    uint32 columnNumber = 1;

    while(i < src->len && src->value[i] != '\0'){
        if(src->value[i] == '\n'){
            ++lineNumber;
            columnNumber = 1;
            ++i;
            continue;
        }
        if(src->value[i] == '/' && src->value[i + 1] == '*'){
            i += 2;
            read_comment_block(src,&i,&lineNumber);
            continue;
        }
        if(src->value[i] == '/' && src->value[i + 1] == '/'){
            i += 2;
            read_comment(src,&i,&lineNumber);
            continue;
        }
        if(src->value[i] == '"'){
            read_string_literal(src, buffer, &i, &lineNumber);
            Token tok = {StringLiteral, get_buffer(buffer), TOKEN_INFO};
            ADD_TOKEN(tok);
            continue;
        }
        if(isAlpha(src->value[i])){
            while(isAlphaNumeric(src->value[i])){
                add_buffer(buffer, src->value[i++]);
            }
            TokenType t = keywordToType(buffer->value);
            Token tok = {t, get_buffer(buffer),TOKEN_INFO};
            ADD_TOKEN(tok);
            continue;
        }
        if(isDigit(src->value[i])){
            int number = readNumberLiteral(src,buffer,&i,&lineNumber);
            if(number == -1){
                lexicalErr("Non valid numeric literal",file,buffer->value,lineNumber);
                clear_buffer(buffer);
                continue;
            }
            Token tok = {number == 1? NumericLiteral : IntLiteral, get_buffer(buffer), TOKEN_INFO};
            ADD_TOKEN(tok);
            continue;
        }
        if(isSymbol(src->value[i])){
            add_buffer(buffer, src->value[i++]);
            if(i <= src->len && isSymbol(src->value[i])){

                add_buffer(buffer, src->value[i]);
            }
            TokenType t = symbolToType(buffer->value);
            if(t == TokenError && buffer->len > 1){
                pop_buffer(buffer);
                t = symbolToType(buffer->value);
            }
            if(t == TokenError){
                lexicalErr("Unexpected token", file, buffer->value, lineNumber);
                continue;
            }
            Token tok = {t, get_buffer(buffer), TOKEN_INFO};
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
    delete_buffer(src);
    delete_buffer(buffer);
    return tokens;
}