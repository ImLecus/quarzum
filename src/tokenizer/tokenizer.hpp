#pragma once
#include "tokenlist.hpp"
#include "../error.hpp"
#include "../debug.hpp"

class Tokenizer {
    
public:
    static TokenList tokenize(const std::string& content){

        TokenList tokens = TokenList();
        size_t index = 0;
        size_t lineNumber = 0;
        std::string buffer;
        bool isComment = false;
        bool isMultiComment = false;

        for(; index <= content.length(); ++index){
            if(content[index] == '\n'){
                isComment = false;
                tokens.addLine(tokens.length);
                ++lineNumber;
                continue;
            }
            if(content[index] == '/' and index + 1 <= content.length() and content[index + 1] == '*'){
                isMultiComment = true;
                continue;
            }
            if(content[index] == '*' and index + 1 <= content.length() and content[index + 1] == '/'){
                isMultiComment = false;
                ++index;
                continue;
            }
            if(isComment or isMultiComment){
                continue;
            }
            if(content[index] == '\''){
                buffer += '\'';
                if(index + 1 <= content.size() and content[index + 1] == '\\'){
                    buffer += content[++index];
                }
                if(index + 1 <= content.size() and isascii(content[index + 1])){
                    buffer += content[index + 1];
                    if(index + 2 <= content.size() and content[index + 2] == '\''){
                        buffer += '\'';
                        tokens.add(Token(char_literal, buffer));
                        buffer.clear();
                        index += 2;
                        continue;
                    }
                    throwLexicalError("Unexpected token", lineNumber);
                }
                throwLexicalError("Unexpected token", lineNumber);
            }
            if(content[index] == '"'){
                buffer += '"';
                ++index;
                while(index <= content.size() and content[index ] != '"'){
                    buffer += content[index++];
                }
                buffer += '"';
                tokens.add(Token(string_literal, buffer));
                buffer.clear();
                continue;
            }
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
                    TokenType type = symbolToType(buffer);

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
            {")", right_par},
            {"//", comment},
            {">", greater},
            {"<", lower},
            {">=", greater_eq},
            {"<=", lower_eq},
            {"==", is_equal},
            {"!=", not_equal},
            {"{", left_curly},
            {"}", right_curly},
            {",", comma},
            {"++", plus_unary},
            {"--", minus_unary},
            {"=>", arrow},
            {"[", left_square},
            {"]", right_square},
            {"&", bit_and},
            {"|", bit_or},
            {"^", bit_xor},
            {"~", bit_not}
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
            {"char", char_keyword},
            {"string", string_keyword},
            {"bool", bool_keyword},
            {"var", var_keyword},
            {"null",null_literal},
            {"true",true_literal},
            {"false",false_literal},
            {"byte", byte_keyword},
            {"or", or_op},
            {"and", and_op},
            {"xor", xor_op},
            {"not", not_op},
            {"function", function_keyword},
            {"module", module_keyword},
            {"class", class_keyword},
            {"return", return_keyword},
            {"const", const_keyword},
            {"if", if_keyword},
            {"while", while_keyword},
            {"exit", exit_keyword},
            {"import", import_keyword},
            {"from", from_keyword},
            {"break", break_keyword},
            {"continue", continue_keyword},
            {"enum", enum_keyword}
        };
        try {
            return keywords.at(symbol);
        }
        catch(...){
            return token_error;
        }
    }
};