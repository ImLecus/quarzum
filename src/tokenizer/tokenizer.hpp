#pragma once
#include "tokenlist.hpp"
#include "../error.hpp"

class Tokenizer {
    
public:
    static TokenList tokenize(const std::string& content){

        TokenList tokens = TokenList();
        size_t index = 0;
        size_t lineNumber = 0;
        std::string buffer;

        for(; index <= content.length(); ++index){

            // Identifiers and keywords
            if(isalpha(content[index])){
                while (isalnum(content[index]))
                {
                    buffer += content[index++];
                }
                TokenType type = bufferToKeyword(buffer);
                tokens.add(Token(type == token_error? identifier : type, buffer));
                buffer.clear();
                --index;
                continue;
            }
            // Integers and numbers
            else if(isdigit(content[index])){
                bool isNumber = false;
                while (isdigit(content[index]) || content[index] == '.')
                {
                    if(content[index] == '.'){isNumber = true;}
                    buffer += content[index++];
                }
                tokens.add(Token(isNumber? num_literal : int_literal, buffer));
                buffer.clear();
                --index;
                continue;
            }

            // Symbols
            else if(ispunct(content[index])){

                buffer += content[index];
                if(index + 1 <= content.length() and ispunct(content[index + 1])){
                    buffer += content[index + 1];
                    if(symbolToType(buffer) != token_error){
                        tokens.add(Token(symbolToType(buffer), buffer));
                        buffer.clear();
                        continue;
                    }
                    buffer.pop_back();
                }
                TokenType type = symbolToType(buffer);
                if(type == token_error){
                    throwLexicalError("Unexpected token", lineNumber);
                }
                tokens.add(Token(type, buffer));
                buffer.clear();
                continue;
            }
            else if(isspace(content[index]) or content[index] == '\0'){
                lineNumber += content[index] == '\n';
                continue;
            }
            else{
                throwLexicalError("Unexpected token", lineNumber);
            }
        }

        return tokens;
    }
private:
    static TokenType symbolToType(std::string symbol){
        
        static const std::unordered_map<std::string, TokenType> symbols = {
            {";", semicolon},
            {"=", equal},
            {"+", plus},
            {"-", minus},
            {"*", prod},
            {"/", division},
            {"%", mod},
            {"(", left_par},
            {")", right_par}
        };
        try {
            return symbols.at(symbol);
        }
        catch(...){
            return token_error;
        }
    }

    static TokenType bufferToKeyword(std::string symbol){
        
        static const std::unordered_map<std::string, TokenType> keywords = {
            {"int", int_keyword},
            {"num", num_keyword},
            {"var", var_keyword},
            {"out", out_keyword}
        };
        try {
            return keywords.at(symbol);
        }
        catch(...){
            return token_error;
        }
    }
};