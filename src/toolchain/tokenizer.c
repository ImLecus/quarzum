/*
 * Quarzum Compiler - tokenizer.c
 * Version 1.0, 24/07/2024
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

inline void read_comment(Buffer* src, u_int64_t* index, u_int32_t* lineNumber){
    while(*index < src->len && src->value[*index] != '\n'){
        if(src->value[*index] == '\0'){
            return;
        }
        ++(*index);
    }
    ++(*lineNumber);
}

inline void read_comment_block(Buffer* src, u_int64_t* index, u_int32_t* lineNumber){
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

void read_string_literal(Buffer* src, Buffer* target, u_int64_t* index, u_int32_t* lineNumber){
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

inline int read_number_literal(Buffer* src, Buffer* target, u_int64_t* index, u_int32_t* lineNumber){
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
    
    Buffer* src = read_file(file);
    if(src == NULL){
        return NULL;
    }
    TokenList* tokens = init_tlist(src->len);
    Buffer* buffer = init_buffer(DEFAULT_TOKENIZER_BUFFER_SIZE);
    
    u_int64_t i = 0;
    u_int32_t lineNumber = 1;
    u_int32_t columnNumber = 1;

    while(t_ch){
        if(t_ch == 0){
            ADD_TOKEN(Eof);
            break;
        }
        if(t_ch == '\n'){
            ++lineNumber;
            columnNumber = 1;
            ++i;
            continue;
        }
        if(t_ch == '/' && t_next == '*'){
            t_advance;
            t_advance;
            read_comment_block(src,&i,&lineNumber);
            continue;
        }
        if(t_ch == '/' && t_next == '/'){
            read_comment(src,&i,&lineNumber);
            continue;
        }
        if(t_ch == '"'){
            read_string_literal(src, buffer, &i, &lineNumber);
            ADD_TOKEN(StringLiteral);
            continue;
        }
        if(t_ch == '\''){
            add_buffer(buffer, '\'');
            t_advance;
            if(t_ch == '\''){
                add_buffer(buffer, '\'');
                t_advance;
            }
            else if(t_ch == '\\'){
                switch (src->value[++i])
                {
                case 'n':
                    add_buffer(buffer, '\n');
                    break;
                case 'r':
                    add_buffer(buffer, '\r');
                    break;
                case 'b':
                    add_buffer(buffer, '\b');
                    break;
                case 'f':
                    add_buffer(buffer, '\f');
                    break;
                case '0':
                    add_buffer(buffer, '\0');
                    break;
                case 't':
                    add_buffer(buffer, '\t');
                    break;
                case '"':
                    add_buffer(buffer, '"');
                    break;
                case '\'':
                    add_buffer(buffer, '\'');
                    break;
                case '\\':
                    add_buffer(buffer, '\\');
                    break;
                default:
                    err("Undefined escape character",0);
                    break;
                }
            }
            else{
                add_buffer(buffer, t_ch);
                t_advance;
            }    
            ADD_TOKEN(CharLiteral);
            continue;
        }
        if(isAlpha(t_ch)){
            while(isAlphaNumeric(t_ch)){
                add_buffer(buffer, t_ch);
                t_advance;
            }
            ADD_TOKEN(keywordToType(buffer->value));
            continue;
        }
        if(isDigit(t_ch)){
            int number = read_number_literal(src,buffer,&i,&lineNumber);
            if(number == -1){
                lexicalErr("Non valid numeric literal",file,buffer->value,lineNumber);
                clear_buffer(buffer);
                continue;
            }
            ADD_TOKEN(number == 1? NumericLiteral : IntLiteral);
            continue;
        }
        if(isSymbol(t_ch)){
            add_buffer(buffer, t_ch);
            t_advance;
            if(i <= src->len && isSymbol(t_ch)){
                add_buffer(buffer, t_ch);
                t_advance;
            }
            TokenType t = symbolToType(buffer->value);
            if(t == TokenError && buffer->len > 1){
                pop_buffer(buffer);
                --i;
                --columnNumber;
                t = symbolToType(buffer->value);
            }
            if(t == TokenError){
                lexicalErr("Unexpected token", file, buffer->value, lineNumber);
                t_advance;
                continue;
            }
            ADD_TOKEN(t);
            continue;
        }
        if(isSpace(t_ch)){
            t_advance;
            continue;
        }
        lexicalErr("Unexpected token", file, &t_ch, lineNumber);
        t_advance;
    }
    delete_buffer(src);
    delete_buffer(buffer);
    return tokens;
}