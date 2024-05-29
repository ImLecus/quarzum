/**
 * TokenType values:
 * 0x00 - 0x2F : keywords
 * 0x30 - 0x5F : symbols
 * 0x60 - 0x6F : literals
 * 0xF0 - 0xFF : other
*/
enum TokenType : unsigned int {
    int_keyword = 0x00,

    equal = 0x30,
    semicolon = 0x31,
    plus = 0x32,

    int_literal = 0x60,
    num_literal = 0x61,
    
    identifier = 0xF0,
    token_error = 0xFF
};
