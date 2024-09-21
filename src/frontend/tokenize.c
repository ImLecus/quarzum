#include "tokenize.h"

lexer_t* init_lexer(char* filename, char* input){
    if(strcmp(get_extension(filename), ".qz") != 0){
        throw_warning((pos_t){0,0,filename},"File format is not '.qz'");
    }
    lexer_t* lex = malloc(sizeof(lexer_t));
    if(!lex) return NULL;

    lex->position = (pos_t){
        .line = 1,
        .column = 1,
        .file = filename
    };
    lex->input = input;
    lex->pos = 0;
    lex->buffer = init_string(32);
    return lex;
}

static inline void advance(lexer_t* lexer){
    if(lexer->input[lexer->pos] == '\n'){
        ++lexer->position.line;
        lexer->position.column = 0;
    }
    ++lexer->pos; 
    ++lexer->position.column;
}

static inline char peek(lexer_t* lexer){
    if(lexer->pos > strlen(lexer->input)) return '\0';
    return lexer->input[lexer->pos];
}

static inline char consume(lexer_t* lexer){
    if(lexer->pos > strlen(lexer->input)) return '\0';
    return lexer->input[lexer->pos++];
}

// Generates a new token in the heap.
// The lexer buffer will be the token value and
// then it will be reseted.
static token_t* new_token(token_type_t  type, lexer_t* lexer){
    token_t* tok = malloc(sizeof(token_t));
    if(tok == NULL) return NULL;

    tok->type = type;
    tok->value = string_copy(lexer->buffer);
    tok->position.line = lexer->position.line;
    tok->position.column = lexer->position.column;
    tok->position.file = lexer->position.file;

    string_clear(lexer->buffer);
    return tok;
}

static void read_escape_char(lexer_t* lexer){
    // the first char is the inverse bar
    advance(lexer);
    char c = peek(lexer);
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
    advance(lexer);
}

static void read_char_literal(lexer_t* lexer){
    string_push(lexer->buffer, consume(lexer));
    char c = peek(lexer);
    if(c == '\\'){
        read_escape_char(lexer);
    }
    if(c != '\''){
        string_push(lexer->buffer, consume(lexer));
    }
    if(peek(lexer) != '\''){
        // err
        return;
    }
    string_push(lexer->buffer, consume(lexer));
}

static void read_string_literal(lexer_t* lexer){
    string_push(lexer->buffer, consume(lexer));
    char c = peek(lexer);
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
            advance(lexer);
        }
        c = peek(lexer);
    }
    string_push(lexer->buffer, consume(lexer));
}

static inline void read_digit_chain(lexer_t* lexer){
    while(isdigit(peek(lexer))){
        string_push(lexer->buffer, consume(lexer));
    }
}

static int read_numeric_literal(lexer_t* lexer){
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

static inline int read_id_or_keyword(lexer_t* lexer){
    while(isalnum(peek(lexer)) || peek(lexer) == '_'){
        string_push(lexer->buffer, consume(lexer));
    }
    int search = binary_search(lexer->buffer->content, keywords, KEYWORDS_SIZE);
    return search == -1 ? T_IDENTIFIER : keyword_types[search];      
}

static int read_symbol(lexer_t* lexer){
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
    int search = binary_search(lexer->buffer->content, symbols, SYMBOLS_SIZE);
    if(search == -1){
        unexpected_token_err(lexer->position, string_copy(lexer->buffer));
        return T_TOKEN_ERROR;
    }
    return symbol_types[search];
}

static void ignore_comment(lexer_t* lexer){
    while(peek(lexer) != '\n'){
        advance(lexer);
    }
    advance(lexer);
} 

static void ignore_multi_comment(lexer_t* lexer){
    advance(lexer);
    char next = lexer->input[lexer->pos + 1];
    while(peek(lexer) != '*' || next != '/'){
        if(peek(lexer) == '\n'){
            ++lexer->position.line;
            lexer->position.column = 1;
        }
        advance(lexer);
        next = lexer->input[lexer->pos + 1];
    }
    advance(lexer);
    advance(lexer);
}

static int check_comment(lexer_t* lexer){
    if(peek(lexer) == '/'){
        advance(lexer);
        if(peek(lexer) == '/'){
            ignore_comment(lexer);
            return 1;
        }
        else if(peek(lexer) == '*'){
            ignore_multi_comment(lexer);
            return 1;
        }
        else{
            --lexer->pos;
            --lexer->position.column;
        }
    }
    return 0;
}

// char* get_error_line(lexer_t* lexer){
//     string_t* line = init_string(10);
//     char lineno[10];
//     uint32_t i = last_line_pos;

//     sprintf(lineno, " %d | ", lexer->position.line);
//     string_append(line, lineno);
    
//     int ignored_spaces = 0;
//     while(lexer->input[i] != '\n'){
//         if(isspace(lexer->input[i]) && !ignored_spaces){
//             ++i;
//             continue;
//         }
//         if(!ignored_spaces){ignored_spaces = 1;}
//         string_push(line, lexer->input[i++]);
//     }
//     string_push(line, '\n');
//     i = 0;
//     while(i < strlen(lineno)){
//         string_push(line, ' ');
//         ++i;
//     }
//     i = 0;
//     while(i < lexer->position.column - 1){
//         string_push(line, ' ');
//         ++i;
//     }
//     string_append(line, RED"^^^"RESET);
//     string_push(line, '\n');
//     char* result = string_copy(line);
//     free_string(line);
//     return result;
// }

token_t* next_token(lexer_t* lexer){
    while (isspace(peek(lexer))){
        advance(lexer);
    }
    if(check_comment(lexer) == 1){
        return next_token(lexer);
    }

    char c = peek(lexer);
    
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
        return new_token(read_symbol(lexer), lexer);
    }
    else if(c == '\0'){
        return new_token(T_EOF, lexer);
    }
    
    unexpected_token_err(lexer->position, string_copy(lexer->buffer));
    advance(lexer);
    return new_token(T_TOKEN_ERROR, lexer);
}

inline void next(lexer_t* lexer){
    lexer->tok = next_token(lexer);
}