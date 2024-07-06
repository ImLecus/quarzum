/*
 * Quarzum Compiler - tokenizer.cpp
 * Version 1.0, 02/07/2024
 *
 * This file is part of the Quarzum project, a proprietary software.
 *
 * Quarzum Project License
 * ------------------------
 *
 * For Contributions License Agreement (CLA), see CONTRIBUTING.md.
 * For full details, see LICENSE.
 */
#pragma once
#include "Quarzum.h"
#include "../include/quarzum.h"
#include "internal/buffer.c"
using namespace Quarzum;

std::deque<Token> tokenize(const std::vector<char>& content) noexcept{


    std::deque<Token> tokens;
    unsigned int lineNumber = 1; 
    unsigned int columnNumber = 1;

    string buffer;
    // EXPERIMENTAL
    Buffer* buff = createBuffer(10);

    CommentType commentType = CommentType::None;
    bool err = false;

    for(auto index = content.begin(); index != content.end(); ++index){
        if(*index == '\n'){
            commentType = CommentType::None;
            ++lineNumber;
            columnNumber = 1;
        }
        else if(*index == '/' and (index + 1 != content.end()) and *(index + 1) == '*'){
            columnNumber += 2;
            commentType = CommentType::Multiple;
        }
        else if(*index == '*' and (index + 1 != content.end()) and *(index + 1) == '/'){
            columnNumber += 2;
            commentType = CommentType::None;
            ++index;
        }
        else if(commentType != CommentType::None){
            ++columnNumber;
        }
        else if(*index == '\''){
            buffer.push_back('\'');
            ++columnNumber;
            if(index + 1 != content.end() and *(index + 1) == '\\'){
                buffer.push_back(*(++index));
                ++columnNumber;
            }
            if(index + 1 != content.end() and isascii(*(index + 1))){
                buffer.push_back(*(index + 1));
                ++columnNumber;
                if(index + 2 != content.end() and *(index + 2) == '\''){
                    buffer.push_back('\'');
                    ++columnNumber;
                    tokens.push_back(Token{TokenType::CharLiteral, buffer.c_str(), lineNumber, columnNumber});
                    buffer.clear();
                    index += 2;
                    continue;
                }
                tokens.push_back({TokenType::TokenError, nullptr, lineNumber, columnNumber});
                err = true;
            }
            tokens.push_back({TokenType::TokenError, nullptr, lineNumber, columnNumber});
            err = true;
        }
        else if(*index == '"'){
            buffer.push_back('"');
            ++columnNumber;
            ++index;
            while(index != content.end() and *index != '"'){
                buffer.push_back(*(index++));
                ++columnNumber;
            }
            buffer.push_back('"');
            ++columnNumber;
            tokens.push_back(Token{TokenType::StringLiteral, buffer.c_str(), lineNumber, columnNumber});
            buffer.clear();
        }
        // Identifiers and keywords
        else if(isalpha(*index)){
            while (isalnum(*index) or *index == '_')
            {
                buffer.push_back(*(index++));
                ++columnNumber;
            }
            TokenType type = bufferToKeyword(buffer.data());
            tokens.push_back(Token{type == TokenType::TokenError? TokenType::Identifier : type, buffer.c_str(), lineNumber, columnNumber});
            buffer.clear();
            --index;
        }
        // Integers and numbers
        else if(isdigit(*index)){
            bool isNumber = false;
            while (isdigit(*index) || *index == '.')
            {
                if(*index == '.'){isNumber = true;}
                buffer.push_back(*(index++));
                ++columnNumber;
            }
            tokens.push_back(Token{isNumber? TokenType::NumericLiteral: TokenType::IntLiteral, buffer.c_str(), lineNumber, columnNumber});
            buffer.clear();
            --index;
        }
        // Symbols
        else if(ispunct(*index)){

            buffer.push_back(*index);
            ++columnNumber;
            if(index + 1 != content.end() and ispunct(*(index + 1))){
                buffer.push_back(*(index + 1));
                ++columnNumber;
                TokenType type = bufferToSymbol(buffer);

                if(type == TokenType::Comment){
                    buffer.clear();
                    commentType = CommentType::Single;
                    continue;
                }
                if(type != TokenType::TokenError){ 
                    tokens.push_back(Token{type, buffer.c_str(), lineNumber, columnNumber});
                    buffer.clear();
                    ++index;
                    continue;
                }
                buffer.pop_back();
            }
            TokenType type = bufferToSymbol(buffer);
            tokens.push_back(Token{type, buffer.c_str(), lineNumber, columnNumber});
            if(type == TokenType::TokenError){
                Debug::throwError("Unexpected token", tokens.back());
                err = true;
            }
            buffer.clear();
        }
        else if(isspace(*index) or *index == '\0'){
            lineNumber += *index == '\n'; // TO-DO: CHANGE
        }
        else{
            tokens.push_back({TokenType::TokenError, nullptr, lineNumber, columnNumber});
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

    deleteBuffer(buff);
    
    return move(tokens);
}
