/*
 * Quarzum Compiler - token.h
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
#ifndef TOKEN_H
#define TOKEN_H
#include "unordered_map"
enum TokenType : uint8_t {
    // special tokens
    token_error = 0x00,
    type_keyword = 0x01,
    access_specifier = 0x02,
    identifier = 0x03,
    comment = 0x04,
    // keywords
    const_keyword = 0x10,
    return_keyword = 0x11,
    module_keyword = 0x12,
    class_keyword = 0x13,
    if_keyword = 0x14,
    while_keyword = 0x15,
    import_keyword = 0x17,
    from_keyword = 0x18,
    continue_keyword = 0x19,
    break_keyword = 0x1A,
    enum_keyword = 0x1B,
    foreach_keyword = 0x1C,
    in_keyword = 0x1D,

    setup_keyword = 0x1F,
    destroy_keyword = 0x20,
    do_keyword = 0x21,
    for_keyword = 0x22,
    else_keyword = 0x23,
    new_keyword = 0x24,
    delete_keyword = 0x25,
    persist_keyword = 0x26,
    typedef_keyword = 0x27,
    foreign_keyword = 0x28,
    // operators
    ASSIGN_OPERATOR = 0x46,
    UNARY_OPERATOR = 0x47,
    ARITHMETIC_OPERATOR = 0x48,
    COMPARATION_OPERATOR = 0x49,
    ternary_operator = 0x4A,
    ternary_separator = 0x4B,

    // symbols
    equal = 0x60,
    semicolon = 0x61,
    left_par = 0x62,
    right_par = 0x63,
    left_curly = 0x64,
    right_curly = 0x65,
    left_square = 0x66,
    right_square = 0x67,
    comma = 0x68,
    arrow = 0x69,
    point = 0x6A,
    // literals
    LITERAL = 0x7F,
};

struct Token {

    Token(): 
        type(TokenType::token_error), 
        value(""), 
        line(0), 
        column(0) {}

    Token(const TokenType& type, const std::string& value, const uint32_t line, const uint32_t column): 
        type(std::move(type)), 
        value(std::move(value)), 
        line(std::move(line)), 
        column(std::move(column)) {}

    inline bool isOperator() const noexcept {
        return this->type == ARITHMETIC_OPERATOR or this->type == COMPARATION_OPERATOR;
    };
    const TokenType type;
    const std::string value;
    const uint32_t line;
    const uint32_t column;
};
#endif
