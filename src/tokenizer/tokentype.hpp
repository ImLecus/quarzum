#pragma once
/**
 * TokenType values:
 * 0x00 - 0x2F : keywords
 * 0x30 - 0x5F: operators
 * 0x60 - 0x7F : symbols
 * 0x80 - 0x9F : literals
 * 0xF0 - 0xFF : other
*/
enum TokenType : unsigned char {
    // type keywords
    int_keyword = 0x00,
    num_keyword = 0x01,
    char_keyword = 0x02,
    string_keyword = 0x03,
    bool_keyword = 0x04,
    var_keyword = 0x05,
    function_keyword = 0x06,
    byte_keyword = 0x07,
    // keywords
    const_keyword = 0x08,
    return_keyword = 0x09,
    module_keyword = 0x0A,
    class_keyword = 0x0B,
    if_keyword = 0x0C,
    while_keyword = 0x0D,
    exit_keyword = 0x10,
    import_keyword = 0x1E,
    from_keyword = 0x1F,
    continue_keyword = 0x20,
    break_keyword = 0x21,
    enum_keyword = 0x22,
    // operators
    plus = 0x30,
    minus = 0x31,
    prod = 0x32,
    division = 0x33,
    mod = 0x34,
    or_op = 0x35,
    not_op = 0x36,
    and_op = 0x37,
    xor_op = 0x38,
    is_equal = 0x39,
    not_equal = 0x3A,
    greater = 0x3B,
    lower = 0x3C,
    greater_eq = 0x3D,
    lower_eq = 0x3E,
    plus_unary = 0x3F,
    minus_unary = 0x40,
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
    // literals
    int_literal = 0x80,
    num_literal = 0x81,
    char_literal = 0x82,
    string_literal = 0x83,
    true_literal = 0x84,
    false_literal = 0x85,
    null_literal = 0x86,
    // other
    identifier = 0xF0,
    comment = 0xF1,
    token_error = 0xFF
};
