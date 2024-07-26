#ifndef QUARZUM_H
#define QUARZUM_H
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "../include/quarzum.h"
//
//  map.c
//
typedef struct {
    char** keys;
    int* values;

    unsigned int size;
} map;

map* init_map(unsigned int size);
void map_add(map* map, char* key, int value);
int map_search(map* map, char* key);

//
//  string.c
//
typedef struct {
    unsigned int size;
    unsigned int len;
    char* value;
} string;

string* init_string(const unsigned int size);
void free_string(string* s);
void string_push(string* s, const char c);
void string_pop(string* s);
void string_clear(string* s);
char* string_copy(string* s);

//
// io.c
//
string* read_file(const char* filename);
FILE* create_file(const char* filename);
void write_file(FILE* file, char* content);

//
//  process.c
//
struct process {
    char* name;
    int start;
    int end;
};

struct process start_process(char* name);
void end_process(struct process* process);

//
//  vector.c
//
#define VECTOR_SIZE_INCREMENT 2

typedef struct {
    unsigned int size;
    unsigned int len;
    void** value;
} vector;

vector* init_vector(unsigned int size);
void free_vector(vector* v);
void vector_push(vector* v, void* element);
void vector_pop(vector* v);
void* vector_get(vector* v, unsigned int index);

//
//  tokenizer.c
//

enum {
    T_TOKEN_ERROR,
    T_KEYWORD,
    T_IDENTIFIER,
    T_EOF,
    T_SYMBOL,
    T_NUMERIC_LITERAL,
    T_STRING_LITERAL,
    T_CHAR_LITERAL,
    T_INT_LITERAL,
    T_NULL_LITERAL
};

struct token_info {
    unsigned int line, column;
    char* file;
};

typedef struct {
    int type;
    char* value;
    struct token_info* info;
} token;

#define KEYWORDS_SIZE 59
#define SYMBOLS_SIZE 39

static const char* keywords[KEYWORDS_SIZE] = {
    "and",
    "bool",
    "break",
    "case",
    "catch",
    "char",
    "class",
    "const",
    "continue",
    "decimal",
    "default",
    "delete",
    "destroy",
    "do",
    "else",
    "enum",
    "exit",
    "false",
    "for",
    "foreach",
    "foreign",
    "from",
    "function",
    "if",
    "import",
    "in",
    "int",
    "int16",
    "int32",
    "int64",
    "int8",
    "module",
    "new",
    "not",
    "null",
    "num",
    "num16",
    "num32",
    "num64",
    "or",
    "persist",
    "private",
    "protected",
    "public",
    "return",
    "setup",
    "string",
    "struct",
    "switch",
    "true",
    "try",
    "typedef",
    "uint",
    "uint16",
    "uint32",
    "uint64",
    "uint8",
    "while",
    "xor"
};

static const char* symbols[SYMBOLS_SIZE] = {
    "!","!=","#","#=","%","%=","&","&=","(",")","*","*=","+","++","+=",",","-","--","-=",
    ".","/","/=",":",";","<","<=","=","==",">",">=","?","[","]","^","^=","{","|","|=","}"
};
vector* tokenize(char* file);

#define DEFAULT_TOKENIZER_BUFFER_SIZE 10

#define ADD_TOKEN(t) vector_push(tokens, &(token){t, string_copy(buffer), &(struct token_info){lineNumber, columnNumber, file}});string_clear(buffer);

#define t_ch src->value[i]
#define t_next src->value[i + 1]
#define t_advance ++i;++columnNumber
#define is_zero(n) n == '0'


// new tokenize implementation

typedef struct {
    char* input;
    unsigned int line;
    unsigned int column;
    unsigned int pos;
    string* buffer;
} lexer;

lexer* init_lexer(char* input);

token* new_token(int type, lexer* lexer);

token* next_token(lexer* lexer);

//
//  parse.c
//

enum {
    N_ROOT,
    N_STRUCT,
    N_IF,
    N_WHILE,
    N_VAR,
    N_MODULE,
    N_BREAK,
    N_CONTINUE,
    N_FUNCTION,
    N_CALL,
    N_IDENTIFIER,
    N_TYPE,
    N_UNARY_EXPR,
    N_BINARY_EXPR,
    N_PAREN_EXPR,
    N_TERNARY_EXPR,
    N_LITERAL
};

typedef struct {
    int type;
    vector* children;
    unsigned int children_count;
} node;

node* init_node(unsigned int children);

node* parse();

//
//  expr.c
//

node* parse_expr(lexer* lexer);

//
//  type.c
//

enum {
    TY_BOOL,
    TY_CHAR,
    TY_STRING,
    TY_FUNCTION,
    TY_INT,
    TY_UINT,
    TY_NUM,
    TY_DECIMAL,
    TY_VAR,
    TY_CUSTOM,
    TY_NULL
};

typedef struct {
    int type;
    int align;
    unsigned int size;
    bool is_unsigned;
    bool is_const;

} type;

//
//  check.c
//

void checkAST(node* ast);
void checkNode(node* node, vector* symbols);

int binary_search(const char* symbol, const char** list, unsigned int size);

#endif