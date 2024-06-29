/*
 * Quarzum Compiler - tokenizer.cpp
 * Version 1.0, 29/06/2024
 *
 * This file is part of the Quarzum project, a proprietary software.
 *
 * Quarzum Project License
 * ------------------------
 *
 * For Contributions License Agreement (CLA), see CONTRIBUTING.md.
 * For full details, see LICENSE.
 */
#ifndef TOKENIZER_CPP
#define TOKENIZER_CPP
#include "tokenizer.h"
#include "../Quarzum.h"
using namespace Quarzum;

const std::deque<Token> tokenize(const std::string& content) noexcept{
    std::deque<Token> tokens;
    uint32_t lineNumber = 1;
    uint32_t columnNumber = 1;
    std::string buffer;
    CommentType commentType = CommentType::kNone;
    bool err = false;

    for(auto index = content.begin(); index != content.end(); ++index){
        if(*index == '\n'){
            commentType = CommentType::kNone;
            ++lineNumber;
            columnNumber = 1;
        }
        else if(*index == '/' and (index + 1 != content.end()) and *(index + 1) == '*'){
            columnNumber += 2;
            commentType = CommentType::kMultiple;
        }
        else if(*index == '*' and (index + 1 != content.end()) and *(index + 1) == '/'){
            columnNumber += 2;
            commentType = CommentType::kNone;
            ++index;
        }
        else if(commentType != CommentType::kNone){
            ++columnNumber;
        }
        else if(*index == '\''){
            buffer += '\'';
            ++columnNumber;
            if(index + 1 != content.end() and *(index + 1) == '\\'){
                buffer += *(++index);
                ++columnNumber;
            }
            if(index + 1 != content.end() and isascii(*(index + 1))){
                buffer += *(index + 1);
                ++columnNumber;
                if(index + 2 != content.end() and *(index + 2) == '\''){
                    buffer += '\'';
                    ++columnNumber;
                    tokens.push_back(Token(LITERAL, buffer, lineNumber, columnNumber));
                    buffer.clear();
                    index += 2;
                    continue;
                }
                tokens.push_back(Token(token_error, "", lineNumber, columnNumber));
                err = true;
            }
            tokens.push_back(Token(token_error, "", lineNumber, columnNumber));
            err = true;
        }
        else if(*index == '"'){
            buffer += '"';
            ++columnNumber;
            ++index;
            while(index != content.end() and *index != '"'){
                buffer += *(index++);
                ++columnNumber;
            }
            buffer += '"';
            ++columnNumber;
            tokens.push_back(Token(LITERAL, buffer, lineNumber, columnNumber));
            buffer.clear();
        }
        // Identifiers and keywords
        else if(isalpha(*index)){
            while (isalnum(*index) or *index == '_')
            {
                buffer += *(index++);
                ++columnNumber;
            }
            TokenType type = bufferToKeyword(buffer);
            tokens.push_back(Token(type == token_error? identifier : type, buffer, lineNumber, columnNumber));
            buffer.clear();
            --index;
        }
        // Integers and numbers
        else if(isdigit(*index)){
            bool isNumber = false;
            while (isdigit(*index) || *index == '.')
            {
                if(*index == '.'){isNumber = true;}
                buffer += *(index++);
                ++columnNumber;
            }
            tokens.push_back(Token(LITERAL, buffer, lineNumber, columnNumber));
            buffer.clear();
            --index;
        }
        // Symbols
        else if(ispunct(*index)){

            buffer += *index;
            ++columnNumber;
            if(index + 1 != content.end() and ispunct(*(index + 1))){
                buffer += *(index + 1);
                ++columnNumber;
                TokenType type = bufferToSymbol(buffer);

                if(type == comment){
                    buffer.clear();
                    commentType = CommentType::kSingle;
                    continue;
                }
                if(type != token_error){ 
                    tokens.push_back(Token(type, buffer, lineNumber, columnNumber));
                    buffer.clear();
                    ++index;
                    continue;
                }
                buffer.pop_back();
            }
            TokenType type = bufferToSymbol(buffer);
            tokens.push_back(Token(type, buffer, lineNumber, columnNumber));
            if(type == token_error){
                Debug::throwError("Unexpected token", tokens.back());
                err = true;
            }
            buffer.clear();
        }
        else if(isspace(*index) or *index == '\0'){
            lineNumber += *index == '\n'; // TO-DO: CHANGE
        }
        else{
            tokens.push_back(Token(token_error, "", lineNumber, columnNumber));
            Debug::throwError("Unexpected token", tokens.back());
            err = true;
            ++index;
            ++columnNumber;
        }
    }
    if(err){
        Debug::err("Lex phase finished with errors. Terminating execution...");
        Debug::exit(1);
    }
    
    return std::move(tokens);
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
#endif
