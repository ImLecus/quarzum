#pragma once
#include "tokenlist.hpp"
#include <unordered_map>
#include "../error.hpp"

namespace Quarzum::Lexer {
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
const std::deque<Token> tokenize(const std::string& content) noexcept;

enum class CommentType : uint8_t {
    kNone,
    kSingle,
    kMultiple
};

const std::unordered_map<std::string, TokenType> symbols = {
    {";", semicolon},
    {"=", equal},
    {"+", ARITHMETIC_OPERATOR},
    {"-", ARITHMETIC_OPERATOR},
    {"*", ARITHMETIC_OPERATOR},
    {"/", ARITHMETIC_OPERATOR},
    {"%", ARITHMETIC_OPERATOR},
    {"(", left_par},
    {")", right_par},
    {"//", comment},
    {">", COMPARATION_OPERATOR},
    {"<", COMPARATION_OPERATOR},
    {">=", COMPARATION_OPERATOR},
    {"<=", COMPARATION_OPERATOR},
    {"==", COMPARATION_OPERATOR},
    {"!=", COMPARATION_OPERATOR},
    {"{", left_curly},
    {"}", right_curly},
    {",", comma},
    {"++", ARITHMETIC_OPERATOR},
    {"--", ARITHMETIC_OPERATOR},
    {"=>", arrow},
    {"[", left_square},
    {"]", right_square},
    {"&", ARITHMETIC_OPERATOR},
    {"|", ARITHMETIC_OPERATOR},
    {"^", ARITHMETIC_OPERATOR},
    {"~", ARITHMETIC_OPERATOR},
    {"+=", ASSIGN_OPERATOR},
    {"-=", ASSIGN_OPERATOR},
    {"*=", ASSIGN_OPERATOR},
    {"/=", ASSIGN_OPERATOR},
    {"%=", ASSIGN_OPERATOR},
    {"#", ARITHMETIC_OPERATOR},
    {"#=", ASSIGN_OPERATOR},
    {"&=", ASSIGN_OPERATOR},
    {"|=", ASSIGN_OPERATOR},
    {"^=", ASSIGN_OPERATOR},
    {"?=", ASSIGN_OPERATOR},
    {".", point},
    {"?", ternary_operator},
    {":", ternary_separator}
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
    {"num16", TYPE_KEYWORD},
    {"num", TYPE_KEYWORD},
    {"num32", TYPE_KEYWORD},
    {"num64", TYPE_KEYWORD},
    {"decimal", TYPE_KEYWORD},
    {"char", TYPE_KEYWORD},
    {"string", TYPE_KEYWORD},
    {"var", TYPE_KEYWORD},
    {"null",LITERAL},
    {"true",LITERAL},
    {"false",LITERAL},
    {"or", COMPARATION_OPERATOR},
    {"and", COMPARATION_OPERATOR},
    {"xor", COMPARATION_OPERATOR},
    {"not", COMPARATION_OPERATOR},
    {"function", TYPE_KEYWORD},
    {"module", module_keyword},
    {"class", class_keyword},
    {"return", return_keyword},
    {"const", const_keyword},
    {"if", if_keyword},
    {"while", while_keyword},
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
    {"else", else_keyword},
    {"setup", setup_keyword},
    {"new", new_keyword},
    {"destroy", destroy_keyword},
    {"delete", delete_keyword},
    {"persist", persist_keyword},
    {"type", typedef_keyword},
    {"foreign", foreign_keyword}
};
}