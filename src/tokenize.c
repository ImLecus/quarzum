#include "quarzum.h"

inline int is_keyword(char* keyword){
    return binary_search(keyword, keywords, KEYWORDS_SIZE) == -1 ? 
    T_IDENTIFIER : T_KEYWORD;
}

inline int is_symbol(char* symbol){
    return binary_search(symbol, symbols, SYMBOLS_SIZE) == -1? 
    T_TOKEN_ERROR: T_SYMBOL;
}

inline static void read_comment(string* src, u_int64_t* index, u_int32_t* lineNumber){
    while(*index < src->len && src->value[*index] != '\n'){
        if(src->value[*index] == '\0'){
            return;
        }
        ++(*index);
    }
    ++(*lineNumber);
}

inline static void read_comment_block(string* src, u_int64_t* index, u_int32_t* lineNumber){
    while(*index < src->len){
        if(src->value[*index] == '\n'){
            ++(*lineNumber);
        }
        if(src->value[*index] == '*' && src->value[*index + 1] == '/'){
            ++(*index);
            ++(*index);
            return;
        }
        ++(*index);
    }
    err("Unclosed comment block",0);
}

static void read_string_literal(string* src, string* target, u_int64_t* index, u_int32_t* lineNumber){
    string_push(target, '"');
     
    ++(*index);
    while(*index < src->len){
        if(src->value[*index] == '"'){
            string_push(target, '"');
            ++(*index);
            return;
        }
        if(src->value[*index] == '\n'){
            ++(*index);
            ++(*lineNumber);
            continue;
        }
        if(src->value[*index] == '\\'){
            switch (src->value[++(*index)])
            {
            case 'n':
                string_push(target, '\n');
                break;
            case 'r':
                string_push(target, '\r');
                break;
            case 'b':
                string_push(target, '\b');
                break;
            case 'f':
                string_push(target, '\f');
                break;
            case '0':
                string_push(target, '\0');
                break;
            case 't':
                string_push(target, '\t');
                break;
            case '"':
                string_push(target, '"');
                break;
            case '\'':
                string_push(target, '\'');
                break;
            case '\\':
                string_push(target, '\\');
                break;
            default:
                err("Undefined escape character",0);
                break;
            }
            continue;
        }
        string_push(target, src->value[*index]);
        ++(*index);
    }
    err("Unclosed string literal",0);
}

inline static int read_number_literal(string* src, string* target, u_int64_t* index, u_int32_t* lineNumber){
    int points = 0;
    while(*index < src->len && (isDigit(src->value[*index]) || src->value[*index] == '.')){
        if(src->value[*index] == '.'){
            ++points;
        }
        string_push(target, src->value[(*index)]);
        ++(*index);
    }
    return points > 1? -1 : points;
}

vector* tokenize(char* file){
    
    string* src = read_file(file);
    if(src == NULL){
        return NULL;
    }

    vector* tokens = init_vector(src->len);

    string* buffer = init_string(DEFAULT_TOKENIZER_BUFFER_SIZE);
    
    unsigned long i = 0;
    unsigned int lineNumber = 1;
    unsigned int columnNumber = 1;

    while(t_ch){
        if(t_ch == 0){
            ADD_TOKEN(T_EOF);
            break;
        }
        if(t_ch == '\n'){
            ++lineNumber;
            columnNumber = 1;
            ++i;
            continue;
        }
        if(t_ch == '/' && t_next == '*'){
            t_advance;
            t_advance;
            read_comment_block(src,&i,&lineNumber);
            continue;
        }
        if(t_ch == '/' && t_next == '/'){
            read_comment(src,&i,&lineNumber);
            continue;
        }
        if(t_ch == '"'){
            read_string_literal(src, buffer, &i, &lineNumber);
            ADD_TOKEN(T_STRING_LITERAL);
            continue;
        }
        if(t_ch == '\''){
            string_push(buffer, '\'');
            t_advance;
            if(t_ch == '\''){
                string_push(buffer, '\'');
                t_advance;
            }
            else if(t_ch == '\\'){
                switch (src->value[++i])
                {
                case 'n':
                    string_push(buffer, '\n');
                    break;
                case 'r':
                    string_push(buffer, '\r');
                    break;
                case 'b':
                    string_push(buffer, '\b');
                    break;
                case 'f':
                    string_push(buffer, '\f');
                    break;
                case '0':
                    string_push(buffer, '\0');
                    break;
                case 't':
                    string_push(buffer, '\t');
                    break;
                case '"':
                    string_push(buffer, '"');
                    break;
                case '\'':
                    string_push(buffer, '\'');
                    break;
                case '\\':
                    string_push(buffer, '\\');
                    break;
                default:
                    err("Undefined escape character",0);
                    break;
                }
            }
            else{
                string_push(buffer, t_ch);
                t_advance;
            }    
            ADD_TOKEN(T_CHAR_LITERAL);
            continue;
        }
        if(isAlpha(t_ch)){
            while(isAlphaNumeric(t_ch)){
                string_push(buffer, t_ch);
                t_advance;
            }
            ADD_TOKEN(is_keyword(buffer->value));
            continue;
        }
        if(isDigit(t_ch)){
            int number = read_number_literal(src,buffer,&i,&lineNumber);
            if(number == -1){
                lexicalErr("Non valid numeric literal",file,buffer->value,lineNumber);
                string_clear(buffer);
                continue;
            }
            ADD_TOKEN(number == 1? T_NUMERIC_LITERAL : T_INT_LITERAL);
            continue;
        }
        if(isSymbol(t_ch)){
            string_push(buffer, t_ch);
            t_advance;
            if(i <= src->len && isSymbol(t_ch)){
                string_push(buffer, t_ch);
                t_advance;
            }
            int t = is_symbol(buffer->value);
            if(t == T_TOKEN_ERROR && buffer->len > 1){
                string_pop(buffer);
                --i;
                --columnNumber;
                t = is_symbol(buffer->value);
            }
            if(t == T_TOKEN_ERROR){
                lexicalErr("Unexpected token", file, buffer->value, lineNumber);
                t_advance;
                continue;
            }
            ADD_TOKEN(t);
            continue;
        }
        if(isSpace(t_ch)){
            t_advance;
            continue;
        }
        lexicalErr("Unexpected token", file, &t_ch, lineNumber);
        t_advance;
    }
    free_string(src);
    free_string(buffer);
    return tokens;
}