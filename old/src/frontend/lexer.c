#include "lexer.h"

Lexer* const init_lexer(const char* const filename, char* const input){

    if(strcmp(get_extension(filename), ".qz") != 0){
        throw_warning((Position){filename, 0, 0},"File format is not '.qz'");
    }

    Lexer* const lex = malloc(sizeof(Lexer));
    if(lex == NULL) return NULL;

    *lex = (Lexer){
        .buffer = init_string(32),
        .input = input,
        .tok = NULL,
        .position = (Position){
            .line = 1,
            .column = 1,
            .file = filename
        }
    };
    return lex;
}

void advance(Lexer* const lexer){
    update_pos(&lexer->position, lexer->input++[0]);
}

const char peek(Lexer* lexer){
    return lexer->input[0];
}

const char consume(Lexer* lexer){
    return lexer->input++[0];
}

const Token* const new_token(const TokenType type, Lexer* const lexer){

    Token* tok = malloc(sizeof(Token));
    if(tok == NULL) return NULL;
    *tok = token_new(type, lexer->buffer->content);

    string_clear(lexer->buffer);
    return tok;
}

void read_escape_char(Lexer* const lexer){
    // the first char is the backslash '\'
    string_push(lexer->buffer, consume(lexer));
    switch (peek(lexer))
    {
    case 'n':
    case 't':
    case '0':
        string_push(lexer->buffer, peek(lexer));
        break;
    default:
        // err
        break;
    }
    advance(lexer);
}

void read_char_literal(Lexer* const lexer){
    string_push(lexer->buffer, consume(lexer));
    const char c = peek(lexer);
    if(c == '\\'){
        read_escape_char(lexer);
    }
    else {
        string_push(lexer->buffer, consume(lexer));
    }
    if(peek(lexer) != '\''){
        // err
        return;
    }
    string_push(lexer->buffer, consume(lexer));
}

void read_string_literal(Lexer* const lexer){
    string_push(lexer->buffer, consume(lexer));
    char c = peek(lexer);
    Position first_pos = lexer->position;
    while(c != '"'){
        if(c == 0){
            unclosed_quotes_err(first_pos);
            return;
        }
        if(c == '\\'){
            read_escape_char(lexer);
        }
        else {
            string_push(lexer->buffer, c);
            advance(lexer);
        }
        c = peek(lexer);
    }
    string_push(lexer->buffer, consume(lexer));
}

void read_digit_chain(Lexer* const lexer){
    while(isdigit(peek(lexer))){
        string_push(lexer->buffer, consume(lexer));
    }
}

const int read_numeric_literal(Lexer* const lexer){
    string_push(lexer->buffer, peek(lexer));
    if(consume(lexer) != '0'){
        read_digit_chain(lexer);
        if(peek(lexer) == '.'){
            string_push(lexer->buffer, consume(lexer));
            read_digit_chain(lexer);
        }
        else{
            return 0;
        }
        if(peek(lexer) == '.'){
            invalid_decimal_err(lexer->position);
        }
        return 1;
    }
    switch (peek(lexer))
    {
    case 'b':
        string_push(lexer->buffer, consume(lexer));
        read_digit_chain(lexer);
        return 0;
    case 'o':
        string_push(lexer->buffer, consume(lexer));
        read_digit_chain(lexer);
        return 0;
    case 'x':
        string_push(lexer->buffer, consume(lexer));
        read_digit_chain(lexer);
        return 0;
    default:
        read_digit_chain(lexer);
        if(peek(lexer) == '.'){
            string_push(lexer->buffer, consume(lexer));
            read_digit_chain(lexer);
        }
        else{
            return 0;
        }
        if(peek(lexer) == '.'){
           invalid_decimal_err(lexer->position);
        }
        return 1;
    }
}

const int read_id_or_keyword(Lexer* const lexer){
    while(isalnum(peek(lexer)) || peek(lexer) == '_'){
        string_push(lexer->buffer, consume(lexer));
    }
    int search = binary_search(lexer->buffer->content, keywords, KEYWORDS_SIZE);
    return search == -1 ? TK_IDENTIFIER : keyword_types[search];      
}

const int read_symbol(Lexer* const lexer){
    string_push(lexer->buffer, consume(lexer));
    if(ispunct(peek(lexer))){
        string_push(lexer->buffer, peek(lexer));
        int search = binary_search(lexer->buffer->content, symbols, SYMBOLS_SIZE);
        if(search != -1){
            advance(lexer);
            return symbol_types[search];
        } 
        string_pop(lexer->buffer);
    }
    const int search = binary_search(lexer->buffer->content, symbols, SYMBOLS_SIZE);
    if(search == -1){
        unexpected_token_err(lexer->position, string_copy(lexer->buffer));
        return TK_ERROR;
    }
    return symbol_types[search];
}

void ignore_multi_comment(Lexer* const lexer){
    advance(lexer);
    char next = lexer->input[1];
    while(peek(lexer) != '*' || next != '/'){
        if(peek(lexer) == '\n'){
            ++lexer->position.line;
            lexer->position.column = 1;
        }
        advance(lexer);
        next = lexer->input[1];
    }
    advance(lexer);
    advance(lexer);
}

const bool check_comment(Lexer* const lexer){
    if(peek(lexer) != '/') return false;
    char next = lexer->input[1];
    if(next == '/'){
        while(consume(lexer) != '\n'); // Ignore single-line comment
        return true;
    }
    if(next == '*'){
        advance(lexer);
        ignore_multi_comment(lexer);
        return true;
    }
    return false;
}

const Token* const next_token(Lexer* const lexer){
    while (isspace(peek(lexer))) advance(lexer);
    if(check_comment(lexer) == 1) return next_token(lexer);

    const char c = peek(lexer);
    
    if(isalpha(c) || c == '_'){
       return new_token(read_id_or_keyword(lexer), lexer);
    }
    if(c == '"'){
        read_string_literal(lexer);
        return new_token(TK_STRING_LITERAL, lexer);
    }
    if(c == '\''){
        read_char_literal(lexer);
        return new_token(TK_CHAR_LITERAL, lexer);
    }
    if(isdigit(c)){
        int is_decimal = read_numeric_literal(lexer);
        return new_token(is_decimal == 1? 
                            TK_NUMERIC_LITERAL :
                            TK_INT_LITERAL, lexer);
    }
    if(ispunct(c)){
        return new_token(read_symbol(lexer), lexer);
    }
    if(c == '\0') return new_token(TK_EOF, lexer);
    
    unexpected_token_err(lexer->position, string_copy(lexer->buffer));
    advance(lexer);
    return new_token(TK_ERROR, lexer);
}

inline void next(Lexer* const lexer){
    lexer->tok = next_token(lexer);
}