#include "quarzum.h"

static uint32_t last_line_pos;

lexer* init_lexer(char* filename, char* input){
    if(!input) return NULL;
    lexer* lex = malloc(sizeof(lexer));
    if(!lex) return NULL;
    lex->input = input;
    lex->line = 1;
    lex->column = 1;
    lex->pos = 0;
    lex->file = filename;
    lex->buffer = init_string(32);
    return lex;
}

static inline void lexer_advance(lexer* lexer){
    if(lexer->input[lexer->pos] == '\n'){
        ++lexer->line;
        lexer->column = 1;
        last_line_pos = lexer->pos;
    }
    ++lexer->pos; 
    ++lexer->column;
}

static inline char lexer_peek(lexer* lexer){
    return lexer->input[lexer->pos];
}

static inline char lexer_consume(lexer* lexer){
    return lexer->input[lexer->pos++];
}

// Generates a new token in the heap.
// The lexer buffer will be the token value and
// then it will be reseted.
token* new_token(uint8_t type, lexer* lexer){
    token* tok = malloc(sizeof(token));
    if(!tok){
        return NULL;
    }

    tok->type = type;
    tok->value = string_copy(lexer->buffer);
    tok->line = lexer->line;
    tok->column = lexer->column;
    tok->file = lexer->file;

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

static inline void read_digit_chain(lexer* lexer){
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
           printf(RED "[ERROR] " RESET "(%s) Too many decimal points on numeric literal at line %d\n%s", lexer->file, lexer->line, get_error_line(lexer));
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
           printf(RED "[ERROR] " RESET "(%s) Too many decimal points on numeric literal at line %d\n%s", lexer->file, lexer->line, get_error_line(lexer));
        }
        return 1;
    }
}

static inline int read_id_or_keyword(lexer* lexer){
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
            lexer_advance(lexer);
            return symbol_types[search];
        } 
        string_pop(lexer->buffer);
    }
    int search = binary_search(lexer->buffer->value, symbols, SYMBOLS_SIZE);
    if(search == -1){
        printf(RED "[ERROR] " RESET "(%s) Unexpected token '%s' at line %d\n %s", 
    lexer->file, string_copy(lexer->buffer), lexer->line, get_error_line(lexer));
        return T_TOKEN_ERROR;
    }
    return symbol_types[search];
}

static void ignore_comment(lexer* lexer){
    while(lexer_peek(lexer) && lexer_peek(lexer) != '\n'){
        lexer_advance(lexer);
    }
    last_line_pos = lexer->pos;
    lexer_advance(lexer);
} 

static void ignore_multi_comment(lexer* lexer){
    lexer_advance(lexer);
    char next = lexer->input[lexer->pos + 1];
    while(lexer_peek(lexer) != '*' || next != '/'){
        if(lexer_peek(lexer) == '\n'){
            ++lexer->line;
            lexer->column = 1;
            last_line_pos = lexer->pos;
        }
        lexer_advance(lexer);
        next = lexer->input[lexer->pos + 1];
    }
    lexer_advance(lexer);
    lexer_advance(lexer);
}

static bool check_comment(lexer* lexer){
    if(lexer_peek(lexer) == '/'){
        lexer_advance(lexer);
        if(lexer_peek(lexer) == '/'){
            ignore_comment(lexer);
            return 1;
        }
        else if(lexer_peek(lexer) == '*'){
            ignore_multi_comment(lexer);
            return 1;
        }
        else{
            --lexer->pos;
            --lexer->column;
        }
    }
    return 0;
}

char* get_error_line(lexer* lexer){
    string* line = init_string(10);
    char lineno[10];
    uint32_t i = last_line_pos;

    sprintf(lineno, " %d | ", lexer->line);
    string_append(line, lineno);
    
    bool ignored_spaces = false;
    while(lexer->input[i] != '\n'){
        if(isspace(lexer->input[i]) && !ignored_spaces){
            ++i;
            continue;
        }
        if(!ignored_spaces){ignored_spaces = true;}
        string_push(line, lexer->input[i++]);
    }
    string_push(line, '\n');
    i = 0;
    while(i < strlen(lineno)){
        string_push(line, ' ');
        ++i;
    }
    i = 0;
    while(i < lexer->column - 1){
        string_push(line, ' ');
        ++i;
    }
    string_append(line, RED"^^^"RESET);
    string_push(line, '\n');
    char* result = string_copy(line);
    free_string(line);
    return result;
}

token* next_token(lexer* lexer){
    while (isspace(lexer_peek(lexer))){
        lexer_advance(lexer);
    }
    if(check_comment(lexer) == 1){
        return next_token(lexer);
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
    else if(c == '\0'){
        return new_token(T_EOF, lexer);
    }
    
    printf(RED "[ERROR] " RESET "(%s) Unexpected token '%s' at line %d\n %s", 
    lexer->file, string_copy(lexer->buffer), lexer->line, get_error_line(lexer));
    lexer_advance(lexer);
    return new_token(T_TOKEN_ERROR, lexer);
}

inline void read_next(lexer* lexer){
    lexer->tok = next_token(lexer);
}