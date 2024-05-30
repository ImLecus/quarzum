/**
 * TokenType values:
 * 0x00 - 0x2F : keywords
 * 0x30 - 0x5F : symbols
 * 0x60 - 0x6F : literals
 * 0xF0 - 0xFF : other
*/
enum TokenType : unsigned int {
    int_keyword = 0x00,
    num_keyword = 0x01,
    char_keyword = 0x02,
    string_keyword = 0x03,
    bool_keyword = 0x04,
    var_keyword = 0x05,
    function_keyword = 0x09,

    out_keyword = 0x10,

    equal = 0x30,
    semicolon = 0x31,
    plus = 0x32,
    minus = 0x33,
    prod = 0x34,
    division = 0x35,
    mod = 0x36,
    or_op = 0x37,
    not_op = 0x38,
    and_op = 0x39,
    xor_op = 0x3A,
    is_equal = 0x3B,
    not_equal = 0x3C,
    greater = 0x3D,
    lower = 0x3E,
    greater_eq = 0x3F,
    lower_eq = 0x40,

    left_par = 0x41,
    right_par = 0x42,
    left_curly = 0x43,
    right_curly = 0x44,

    int_literal = 0x60,
    num_literal = 0x61,
    char_literal = 0x62,
    string_literal = 0x63,
    true_literal = 0x64,
    false_literal = 0x65,
    null_literal = 0x66,
    
    identifier = 0xF0,
    comment = 0xF1,
    token_error = 0xFF
};
