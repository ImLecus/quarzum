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

    out_keyword = 0x10,

    equal = 0x30,
    semicolon = 0x31,
    plus = 0x32,
    minus = 0x33,
    prod = 0x34,
    division = 0x35,
    mod = 0x36,

    left_par = 0x40,
    right_par = 0x41,

    int_literal = 0x60,
    num_literal = 0x61,
    
    identifier = 0xF0,
    comment = 0xF1,
    token_error = 0xFF
};
