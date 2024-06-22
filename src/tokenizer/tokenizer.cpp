/**
 * @file tokenizer.cpp
 * This module contains all the related functions
 * for the lexer.
 * 
*/
#pragma once
#include "tokenizer.hpp"

const TokenList tokenize(const std::vector<char>& content) noexcept{
    TokenList tokens;
    unsigned long lineNumber = 1;
    std::string buffer;
    bool isComment = false;
    bool isMultiComment = false;
    bool err = false;

    for(auto index = content.begin(); index != content.end(); ++index){
        if(*index == '\n'){
            isComment = false;
            tokens.addLine(tokens.length);
            ++lineNumber;
            continue;
        }
        if(*index == '/' and (index + 1 != content.end()) and *(index + 1) == '*'){
            isMultiComment = true;
            continue;
        }
        if(*index == '*' and (index + 1 != content.end()) and *(index + 1) == '/'){
            isMultiComment = false;
            ++index;
            continue;
        }
        if(isComment or isMultiComment){
            continue;
        }
        if(*index == '\''){
            buffer += '\'';
            if(index + 1 != content.end() and *(index + 1) == '\\'){
                buffer += *(++index);
            }
            if(index + 1 != content.end() and isascii(*(index + 1))){
                buffer += *(index + 1);
                if(index + 2 != content.end() and *(index + 2) == '\''){
                    buffer += '\'';
                    tokens.add(Token(char_literal, buffer));
                    buffer.clear();
                    index += 2;
                    continue;
                }
                throwLexicalError("Unexpected token", lineNumber,index);
                err = true;
            }
            throwLexicalError("Unexpected token", lineNumber, index);
            err = true;
        }
        if(*index == '"'){
            buffer += '"';
            ++index;
            while(index != content.end() and *index != '"'){
                buffer += *(index++);
            }
            buffer += '"';
            tokens.add(Token(string_literal, buffer));
            buffer.clear();
            continue;
        }
        // Identifiers and keywords
        if(isalpha(*index)){
            while (isalnum(*index) or *index == '_')
            {
                buffer += *(index++);
            }
            TokenType type = bufferToKeyword(buffer);
            tokens.add(Token(type == token_error? identifier : type, buffer));
            buffer.clear();
            --index;
            continue;
        }
        // Integers and numbers
        else if(isdigit(*index)){
            bool isNumber = false;
            while (isdigit(*index) || *index == '.')
            {
                if(*index == '.'){isNumber = true;}
                buffer += *(index++);
            }
            tokens.add(Token(isNumber? num_literal : int_literal, buffer));
            buffer.clear();
            --index;
            continue;
        }
        // Symbols
        else if(ispunct(*index)){

            buffer += *index;
            if(index + 1 != content.end() and ispunct(*(index + 1))){
                buffer += *(index + 1);
                TokenType type = bufferToSymbol(buffer);

                if(type == comment){
                    buffer.clear();
                    isComment = true;
                    continue;
                }
                if(type != token_error){ 
                    tokens.add(Token(type, buffer));
                    buffer.clear();
                    ++index;
                    continue;
                }
                buffer.pop_back();
            }
            TokenType type = bufferToSymbol(buffer);
            if(type == token_error){
                throwLexicalError("Unexpected token", lineNumber, index);
                err = true;
            }

            tokens.add(Token(type, buffer));
            buffer.clear();
            continue;
        }
        else if(isspace(*index) or *index == '\0'){
            lineNumber += *index == '\n';
            continue;
        }
        else{
            throwLexicalError("Unexpected token", lineNumber, index);
            err = true;
            ++index;
        }
    }
    if(err){return TokenList();}
    return tokens;
}

const TokenType bufferToSymbol(const std::string& buffer){  
    auto it = symbols.find(buffer);
    if (it != symbols.end()) {
        return it->second;
    }
    return token_error;
}

const TokenType bufferToKeyword(const std::string& buffer){ 
    auto it = keywords.find(buffer);
    if (it != keywords.end()) {
        return it->second;
    }
    return identifier;
}

void throwLexicalError(const char* message, const unsigned long& lineNumber,std::vector<char>::const_iterator index){
    std::cout << "\e[31m" << "LexicalError" << "\e[0m" << ": " << message << " at line " << lineNumber << ".\n";
    std::string line;
    u_int16_t margin = 0;
    while(*(index-1) != '\n'){--index;++margin;}
    while(*index != '\n'){line += *(index++);}
    std::cout << lineNumber << " | " << line << '\n';
    margin += 3 + std::to_string(lineNumber).length();
    line = "";
    while(margin > 1){line += " ";--margin;}
    line += "^^^\n";
    std::cout << "\e[31m" << line << "\e[0m";
}
