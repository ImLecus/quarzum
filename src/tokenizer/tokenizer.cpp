#pragma once
#include "tokenizer.hpp"

namespace quarzum::lexer {

const TokenList tokenize(const std::vector<char>& content) noexcept{
    TokenList tokens;
    uint64_t lineNumber = 1;
    std::string buffer;
    CommentType commentType = CommentType::kNone;
    bool err = false;

    for(auto index = content.begin(); index != content.end(); ++index){
        if(*index == '\n'){
            commentType = CommentType::kNone;
            tokens.addLine(tokens.length);
            ++lineNumber;
            continue;
        }
        if(*index == '/' and (index + 1 != content.end()) and *(index + 1) == '*'){
            commentType = CommentType::kMultiple;
            continue;
        }
        if(*index == '*' and (index + 1 != content.end()) and *(index + 1) == '/'){
            commentType = CommentType::kNone;
            ++index;
            continue;
        }
        if(commentType != CommentType::kNone){
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
                    commentType = CommentType::kSingle;
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
            tokens.add(Token(type, buffer));
            if(type == token_error){
                debug::throwLexicalError("Unexpected token", tokens.getItems().back());
                err = true;
            }
            buffer.clear();
            continue;
        }
        else if(isspace(*index) or *index == '\0'){
            lineNumber += *index == '\n';
            continue;
        }
        else{
            tokens.add(ERROR_TOKEN);
            debug::throwLexicalError("Unexpected token", tokens.getItems().back());
            err = true;
            ++index;
        }
    }
    if(err){
        debug::err("Lex phase finished with errors. Terminating execution...");
        std::exit(1);
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
// @deprecated
void throwLexicalError(const char* message, const uint64_t& lineNumber,std::vector<char>::const_iterator index){
    std::cout << "\e[31m" << "LexicalError" << "\e[0m" << ": " << message << " at line " << lineNumber << ".\n";
    std::string line;
    uint16_t margin = 0;
    while(*(index-1) != '\n'){--index;++margin;}
    while(*index != '\n'){line += *(index++);}
    std::cout << lineNumber << " | " << line << '\n';
    margin += 3 + std::to_string(lineNumber).length();
    line = "";
    while(margin > 1){line += " ";--margin;}
    line += "^^^\n";
    std::cout << "\e[31m" << line << "\e[0m";
}

}