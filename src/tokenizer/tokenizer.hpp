#pragma once
#include "tokenlist.hpp"
#include <unordered_map>
#include "../error.hpp"

namespace quarzum::lexer {
/**
 * @brief Search if the buffer is a symbol. Otherwise, returns
 * the TokenType error_token.
*/
const TokenType bufferToSymbol(const std::string& buffer);
/**
 * @brief Search if the buffer is a keyword. Otherwise, returns
 * the TokenType identifier.
*/
const TokenType bufferToKeyword(const std::string& buffer);
/**
 * @brief Reads a string with the source code and creates a TokenList.
 * If some unexpected token appears, the function throws an 
 * error.
 * @param content The source code.
*/
const TokenList tokenize(const std::vector<char>& content) noexcept;
/**
 * @brief Prints a lexical error message.
 * @deprecated
*/
void throwLexicalError(const char* message, const unsigned long& line, std::vector<char>::const_iterator index);

enum class CommentType : uint8_t {
    kNone,
    kSingle,
    kMultiple
};

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
    {"+=", ASSIGN_OPERATOR},
    {"-=", ASSIGN_OPERATOR},
    {"*=", ASSIGN_OPERATOR},
    {"/=", ASSIGN_OPERATOR},
    {"#", converge_sum},
    {"#=", ASSIGN_OPERATOR},
    {".", point}
};
const std::unordered_map<std::string, TokenType> keywords = {
    {"bool", TYPE_KEYWORD},
    {"int8", TYPE_KEYWORD},
    {"int16", TYPE_KEYWORD},
    {"int", TYPE_KEYWORD},
    {"int32", TYPE_KEYWORD},
    {"int64", TYPE_KEYWORD},
    {"uint8", TYPE_KEYWORD},
    {"uint16", TYPE_KEYWORD},
    {"uint", TYPE_KEYWORD},
    {"uint32", TYPE_KEYWORD},
    {"uint64", TYPE_KEYWORD},
    {"num", TYPE_KEYWORD},
    {"num64", TYPE_KEYWORD},
    {"decimal", TYPE_KEYWORD},
    {"char", TYPE_KEYWORD},
    {"string", TYPE_KEYWORD},
    {"var", TYPE_KEYWORD},
    {"null",null_literal},
    {"true",true_literal},
    {"false",false_literal},
    {"or", or_op},
    {"and", and_op},
    {"xor", xor_op},
    {"not", not_op},
    {"function", TYPE_KEYWORD},
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
    {"public", ACCESS_SPECIFIER},
    {"private", ACCESS_SPECIFIER},
    {"protected", ACCESS_SPECIFIER},
    {"do", do_keyword},
    {"for", for_keyword},
    {"else", else_keyword}
};
}