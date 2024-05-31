#pragma once
#include "./tokenizer/token.hpp"
#include <iostream>
#include <string>

class Debug {
public:
    static void printToken(Token token){
        static const std::unordered_map<TokenType, std::string> typeToString = {
            {identifier, "identifier"},
            {equal, "equal"},
            {int_keyword, "int_keyword"},
            {int_literal, "int_literal"},
            {semicolon, "semicolon"},
            {num_literal, "num_literal"},
            {plus, "plus"},
            {minus, "minus"},
            {prod, "product"},
            {division, "division"},
            {mod, "module"},
            {left_par, "left_par"},
            {right_par, "right_par"},
            {out_keyword, "out_keyword"},
            {num_keyword, "num_keyword"},
            {char_keyword, "char_keyword"},
            {string_keyword, "string_keyword"},
            {var_keyword, "var_keyword"},
            {null_literal, "null_literal"},
            {char_literal, "char_literal"},
            {string_literal, "string_literal"},
            {true_literal, "true_literal"},
            {false_literal, "false_literal"},
            {or_op, "or"},
            {and_op, "and"},
            {not_op, "not"},
            {xor_op, "xor"},
            {function_keyword, "function_keyword"},
            {left_curly, "left_curly_bracket"},
            {right_curly, "rigth_curly_bracket"},
            {module_keyword, "module"},
            {class_keyword, "class"},
            {return_keyword, "return"},
            {const_keyword, "const"},
            {if_keyword, "if_keyword"},
            {while_keyword, "while_keyword"},
            {exit_keyword, "exit_keyword"}
        };
        try{
            std::cout << typeToString.at(token.getType()) << ": " << token.getValue() << '\n';
        }
        catch(...){return;}
    }
};