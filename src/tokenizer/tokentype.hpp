#pragma once
namespace Quarzum::Lexer {
/**
 * TokenType values:
 * 0x00 - 0x2F : keywords
 * 0x30 - 0x5F: operators
 * 0x60 - 0x7F : symbols
 * 0x80 - 0x9F : literals
 * 0xF0 - 0xFF : other
*/
enum TokenType : uint8_t {
    TYPE_KEYWORD = 0x00,
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
    ACCESS_SPECIFIER = 0x1E,
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
    // other
    identifier = 0xF0,
    comment = 0xF1,
    token_error = 0xFF
};
}