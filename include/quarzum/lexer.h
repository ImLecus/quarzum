/*
 * Quarzum Compiler - lexer.h
 * Version 1.0, 04/07/2024
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
#include "token.h"
#include <unordered_map>
#include <deque>
#include <string>
using TT = TokenType;
/**
 * @brief Transforms a list of characters (vector<char>) into a list of
 * Tokens (deque<Token>). Returns an empty deque if something goes wrong.
 */
std::deque<Token> tokenize(const std::vector<char>& content) noexcept;

const std::unordered_map<std::string, TokenType> symbols({
    {";",       TT::Semicolon},
    {"=",       TT::Equal},
    {"+",       TT::ArithmeticOperator},
    {"-",       TT::ArithmeticOperator},
    {"*",       TT::ArithmeticOperator},
    {"/",       TT::ArithmeticOperator},
    {"%",       TT::ArithmeticOperator},
    {"(",       TT::LeftPar},
    {")",       TT::RightPar},
    {"//",      TT::Comment},
    {">",       TT::ComparationOperator},
    {"<",       TT::ComparationOperator},
    {">=",      TT::ComparationOperator},
    {"<=",      TT::ComparationOperator},
    {"==",      TT::ComparationOperator},
    {"!=",      TT::ComparationOperator},
    {"{",       TT::LeftCurly},
    {"}",       TT::RightCurly},
    {",",       TT::Comma},
    {"++",      TT::ArithmeticOperator},
    {"--",      TT::ArithmeticOperator},
    {"=>",      TT::Arrow},
    {"[",       TT::LeftSquare},
    {"]",       TT::RightSquare},
    {"&",       TT::ArithmeticOperator},
    {"|",       TT::ArithmeticOperator},
    {"^",       TT::ArithmeticOperator},
    {"~",       TT::ArithmeticOperator},
    {"+=",      TT::AssignOperator},
    {"-=",      TT::AssignOperator},
    {"*=",      TT::AssignOperator},
    {"/=",      TT::AssignOperator},
    {"%=",      TT::AssignOperator},
    {"#",       TT::ArithmeticOperator},
    {"#=",      TT::AssignOperator},
    {"&=",      TT::AssignOperator},
    {"|=",      TT::AssignOperator},
    {"^=",      TT::AssignOperator},
    {"?=",      TT::AssignOperator},
    {".",       TT::Point},
    {"?",       TT::TernaryOperator},
    {":",       TT::TernarySeparator}
}, 42);
/**
 * @brief Converts a buffer into a symbol TokenType.
 * Returns TokenType::TokenError if there is no such symbol.
 */
inline TokenType bufferToSymbol(const std::string& buffer) noexcept {
    auto it = symbols.find(buffer);
    return it == symbols.end()? TT::TokenError : it->second;
}

const std::unordered_map<std::string, TokenType> keywords({
    {"bool",        TT::TypeKeyword},
    {"int8",        TT::TypeKeyword},
    {"int16",       TT::TypeKeyword},
    {"int",         TT::TypeKeyword},
    {"int32",       TT::TypeKeyword},
    {"int64",       TT::TypeKeyword},
    {"uint8",       TT::TypeKeyword},
    {"uint16",      TT::TypeKeyword},
    {"uint",        TT::TypeKeyword},
    {"uint32",      TT::TypeKeyword},
    {"uint64",      TT::TypeKeyword},
    {"num",         TT::TypeKeyword},
    {"num16",       TT::TypeKeyword},
    {"num32",       TT::TypeKeyword},
    {"num64",       TT::TypeKeyword},
    {"decimal",     TT::TypeKeyword},
    {"char",        TT::TypeKeyword},
    {"string",      TT::TypeKeyword},
    {"var",         TT::TypeKeyword},
    {"null",        TT::NullLiteral},
    {"true",        TT::BoolLiteral},
    {"false",       TT::BoolLiteral},
    {"or",          TT::ComparationOperator},
    {"and",         TT::ComparationOperator},
    {"xor",         TT::ComparationOperator},
    {"not",         TT::ComparationOperator},
    {"function",    TT::TypeKeyword},
    {"module",      TT::Module},
    {"class",       TT::Class},
    {"return",      TT::Return},
    {"const",       TT::Const},
    {"if",          TT::If},
    {"while",       TT::While},
    {"import",      TT::Import},
    {"from",        TT::From},
    {"break",       TT::Break},
    {"continue",    TT::Continue},
    {"enum",        TT::Enum},
    {"foreach",     TT::Foreach},
    {"in",          TT::In},
    {"public",      TT::AccessSpecifier},
    {"private",     TT::AccessSpecifier},
    {"protected",   TT::AccessSpecifier},
    {"do",          TT::Do},
    {"for",         TT::For},
    {"else",        TT::Else},
    {"setup",       TT::Setup},
    {"new",         TT::New},
    {"destroy",     TT::Destroy},
    {"delete",      TT::Delete},
    {"persist",     TT::Persist},
    {"typedef",     TT::Typedef},
    {"foreign",     TT::Foreign}
},53) ;
/**
 * @brief Converts a buffer into a keyword TokenType.
 * Returns TokenType::Identifier if there is no such keyword.
 */
inline TokenType bufferToKeyword(const std::string& buffer) noexcept {
    auto it = keywords.find(buffer);
    return it == keywords.end()? TT::Identifier : it->second;
}

/**
 * @brief Defines the type of comment that the lexer
 * is currently reading.
 */
enum class CommentType : uint8_t {
    None,
    Single,
    Multiple
};