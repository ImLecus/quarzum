#include "quarzum.h"

lexer* init_lexer(char* input){
    lexer* lex = (lexer*)malloc(sizeof(lexer));
    lex->input = input;
    lex->line = 1;
    lex->column = 1;
    lex->pos = 0;
    lex->buffer = init_string(DEFAULT_TOKENIZER_BUFFER_SIZE);
    return lex;
}

static inline void lexer_advance(lexer* lexer){
    if(lexer->input[lexer->pos] == '\n'){
        ++lexer->line;
        lexer->column = 1;
    }
    ++lexer->pos;
}

static inline char lexer_peek(lexer* lexer){
    return lexer->input[lexer->pos];
}

static inline char lexer_consume(lexer* lexer){
    return lexer->input[lexer->pos++];
}

token* new_token(int type, lexer* lexer){
    token* tok = (token*)malloc(sizeof(token));
    tok->type = type;
    tok->value = string_copy(lexer->buffer);
    tok->line = lexer->line;
    tok->column = lexer->column;
    tok->file = "none.qz"; // placeholder

    string_clear(lexer->buffer);
    return tok;
}

static void read_escape_char(lexer* lexer){
    // the first char is the inverse bar
    lexer_advance(lexer);
    char c = lexer_peek(lexer);
    switch (c)
    {
    case 'n':
        string_push(lexer->buffer, '\n');
        break;
    case 't':
        string_push(lexer->buffer, '\t');
        break;
    default:
        // err
        break;
    }
    lexer_advance(lexer);
}

static void read_char_literal(lexer* lexer){
    string_push(lexer->buffer, lexer_consume(lexer));
    char c = lexer_peek(lexer);
    if(c == '\\'){
        read_escape_char(lexer);
    }
    if(c != '\''){
        string_push(lexer->buffer, lexer_consume(lexer));
    }
    if(lexer_peek(lexer) != '\''){
        // err
        return;
    }
    string_push(lexer->buffer, lexer_consume(lexer));
}

static void read_string_literal(lexer* lexer){
    string_push(lexer->buffer, lexer_consume(lexer));
    char c = lexer_peek(lexer);
    while(c != '"'){
        if(c == 0){
            // err
            return;
        }
        if(c == '\\'){
            read_escape_char(lexer);
        }
        else {
            string_push(lexer->buffer, c);
            lexer_advance(lexer);
        }
        c = lexer_peek(lexer);
    }
    string_push(lexer->buffer, lexer_consume(lexer));
}

static void read_digit_chain(lexer* lexer){
    while(isdigit(lexer_peek(lexer))){
        string_push(lexer->buffer, lexer_consume(lexer));
    }
}

static int read_numeric_literal(lexer* lexer){
    string_push(lexer->buffer, lexer_peek(lexer));
    char c = lexer_consume(lexer);
    if(c != '0'){
        read_digit_chain(lexer);
        if(lexer_peek(lexer) == '.'){
            string_push(lexer->buffer, lexer_consume(lexer));
            read_digit_chain(lexer);
        }
        else{
            return 0;
        }
        if(lexer_peek(lexer) == '.'){
           // err
        }
        return 1;
    }
    switch (lexer_peek(lexer))
    {
    case 'b':
        string_push(lexer->buffer, lexer_consume(lexer));
        read_digit_chain(lexer);
        return 0;
    case 'o':
        string_push(lexer->buffer, lexer_consume(lexer));
        read_digit_chain(lexer);
        return 0;
    case 'x':
        string_push(lexer->buffer, lexer_consume(lexer));
        read_digit_chain(lexer);
        return 0;
    default:
        read_digit_chain(lexer);
        if(lexer_peek(lexer) == '.'){
            string_push(lexer->buffer, lexer_consume(lexer));
            read_digit_chain(lexer);
        }
        else{
            return 0;
        }
        if(lexer_peek(lexer) == '.'){
           // err
        }
        return 1;
    }
}

static int read_id_or_keyword(lexer* lexer){
    while(isalnum(lexer_peek(lexer)) || lexer_peek(lexer) == '_'){
        string_push(lexer->buffer, lexer_consume(lexer));
    }
    int search = binary_search(lexer->buffer->value, keywords, KEYWORDS_SIZE);

    return search == -1 ? T_IDENTIFIER : keyword_types[search];
            
}

static int read_symbol(lexer* lexer){
    string_push(lexer->buffer, lexer_consume(lexer));
    if(ispunct(lexer_peek(lexer))){
        string_push(lexer->buffer, lexer_peek(lexer));
        int search = binary_search(lexer->buffer->value, symbols, SYMBOLS_SIZE);
        if(search != -1){
            return symbol_types[search];
        } 
    }
    int search = binary_search(lexer->buffer->value, symbols, SYMBOLS_SIZE);
    if(search != -1){
        return symbol_types[search];
    }
    return T_TOKEN_ERROR;
}

token* next_token(lexer* lexer){
    while (isspace(lexer_peek(lexer)))
    {
        lexer_advance(lexer);
    }
    char c = lexer_peek(lexer);
    if(isalpha(c) || c == '_'){
       int type = read_id_or_keyword(lexer);
       return new_token(type, lexer);
    }
    else if(c == '"'){
        read_string_literal(lexer);
        return new_token(T_STRING_LITERAL, lexer);
    }
    else if(c == '\''){
        read_char_literal(lexer);
        return new_token(T_CHAR_LITERAL, lexer);
    }
    else if(isdigit(c)){
        int is_decimal = read_numeric_literal(lexer);
        return new_token(is_decimal == 1? 
                            T_NUMERIC_LITERAL :
                            T_INT_LITERAL, lexer);
    }
    else if(ispunct(c)){
        int search = read_symbol(lexer);
        return new_token(search, lexer);
    }
    else if(c == 0){
        return new_token(T_EOF, lexer);
    }
    // lexical err
    lexer_advance(lexer);
    return new_token(T_TOKEN_ERROR, lexer);
}

void read_next(lexer* lexer){
    lexer->tok = next_token(lexer);
}