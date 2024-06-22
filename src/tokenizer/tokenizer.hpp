#pragma once
#include "tokenlist.hpp"
#include "../error.hpp"

/**
 * @brief Search if the buffer is a symbol. Otherwise, returns
 * the TokenType error_token.
*/
TokenType bufferToSymbol(const std::string& buffer);
/**
 * @brief Search if the buffer is a keyword. Otherwise, returns
 * the TokenType identifier.
*/
TokenType bufferToKeyword(const std::string& buffer);
/**
 * @brief Reads a string with the source code and creates a TokenList.
 * If some unexpected token appears, the function throws an 
 * error.
 * @param content The source code.
*/
TokenList tokenize(const std::string& content);
/**
 * @brief Prints a lexical error message.
*/
void throwLexicalError(const char* message, const size_t line, std::string::const_iterator index);

const std::unordered_map<std::string, TokenType> symbols = {
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
    {"~", bit_not},
    {"+=", plus_eq},
    {"-=", minus_eq},
    {"*=", prod_eq},
    {"/=", div_eq},
    {"#", converge_sum},
    {"#=", converge_eq},
    {".", point}
};
const std::unordered_map<std::string, TokenType> keywords = {
    {"bool", b_keyword},
    {"int8", i8_keyword},
    {"int16", i16_keyword},
    {"int", i32_keyword},
    {"int32", i32_keyword},
    {"int64", i64_keyword},
    {"uint8", u8_keyword},
    {"uint16", u16_keyword},
    {"uint", u32_keyword},
    {"uint32", u32_keyword},
    {"uint64", u64_keyword},
    {"num", num_keyword},
    {"num64", num64_keyword},
    {"decimal", decimal_keyword},
    {"char", char_keyword},
    {"string", string_keyword},
    {"var", var_keyword},
    {"null",null_literal},
    {"true",true_literal},
    {"false",false_literal},
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
    {"enum", enum_keyword},
    {"foreach", foreach_keyword},
    {"in", in_keyword},
    {"public", public_keyword},
    {"private", private_keyword},
    {"protected", protected_keyword},
    {"do", do_keyword},
    {"for", for_keyword},
    {"else", else_keyword}
};
