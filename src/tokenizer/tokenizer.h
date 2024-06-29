/*
 * Quarzum Compiler - tokenizer.h
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
#ifndef TOKENIZER_H
#define TOKENIZER_H
#include "token.h"
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
    {"bool", type_keyword},
    {"int8", type_keyword},
    {"int16", type_keyword},
    {"int", type_keyword},
    {"int32", type_keyword},
    {"int64", type_keyword},
    {"uint8", type_keyword},
    {"uint16", type_keyword},
    {"uint", type_keyword},
    {"uint32", type_keyword},
    {"uint64", type_keyword},
    {"num16", type_keyword},
    {"num", type_keyword},
    {"num32", type_keyword},
    {"num64", type_keyword},
    {"decimal", type_keyword},
    {"char", type_keyword},
    {"string", type_keyword},
    {"var", type_keyword},
    {"null",LITERAL},
    {"true",LITERAL},
    {"false",LITERAL},
    {"or", COMPARATION_OPERATOR},
    {"and", COMPARATION_OPERATOR},
    {"xor", COMPARATION_OPERATOR},
    {"not", COMPARATION_OPERATOR},
    {"function", type_keyword},
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
    {"public", access_specifier},
    {"private", access_specifier},
    {"protected", access_specifier},
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
#endif
