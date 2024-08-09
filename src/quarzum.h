#ifndef QUARZUM_H
#define QUARZUM_H
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define QUARZUM_COMPILER_VERSION "1.0"
#define QUARZUM_VERSION "1.0"
#define VERSION_TYPE "experimental"

typedef int bool;
#define true 1
#define false 0

//
//  bsearch.c
//
int binary_search(const char* symbol, const char** list, unsigned int size);

//
//  debug
//
#define RESET "\e[0m"

#define RED "\e[1;31m"
#define YELLOW "\e[1;33m"
#define GREEN "\e[1;32m"
#define CYAN "\e[1;36m"
#define MAGENTA "\e[1;35m"
#define GRAY "\e[1;90m"
#define ORANGE "\e[38;5;214m"

#define UNDERLINE "\e[4m"
#define BOLD "\e[1m"

#define ERROR_MSG(msg) RED"[ERROR] "RESET"%s\n",msg
#define LOG_MSG(msg) CYAN"[LOG] "RESET"%s\n",msg
#define WARN_MSG(msg) BOLD ORANGE"[WARNING] "RESET"%s\n",msg
#define DEBUG_MSG(msg) MAGENTA"[DEBUG] "RESET"%s\n",msg


//
//  hashmap.c
//
#define FNV_OFFSET 14695981039346656037UL
#define FNV_PRIME 1099511628211UL

typedef struct {
    char* key;
    void* value;
} bucket;

typedef struct {
    bucket** content;
    unsigned int size;
    unsigned int len;
} hashmap;

hashmap* init_hashmap(unsigned int size);

void free_hashmap(hashmap* map);

int hash(char* key);

void* hashmap_get(hashmap* map, char* key);

void hashmap_add(hashmap* map, char* key, void* value);

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
void string_append(string* s, char* c);
char* delete_quotes(char* c);

// Temporal library path
#define LIB_PATH "./lib/"
#define LIB_PATH_LEN 6

char* resolve_path(char* c);
//
// io.c
//
string* read_file(const char* filename);
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
    T_LOGICAL_OP,
    T_SPECIFIER,
    T_UNARY,
    T_ACCESS,
    T_TYPE,
    T_BITWISE_OP,
    T_ARITHMETIC_OP,
    T_ASSIGN_OP,
    T_IDENTIFIER,
    T_EOF,
    T_SYMBOL,
    T_NUMERIC_LITERAL,
    T_STRING_LITERAL,
    T_CHAR_LITERAL,
    T_INT_LITERAL,
    T_NULL_LITERAL,
    T_KEYWORD_AS,
    T_KEYWORD_BREAK,
    T_KEYWORD_CASE,
    T_KEYWORD_CLASS,
    T_KEYWORD_CONTINUE,
    T_KEYWORD_DEFAULT,
    T_KEYWORD_DELETE,
    T_KEYWORD_DO,
    T_KEYWORD_ELSE,
    T_KEYWORD_ENUM,
    T_KEYWORD_FALSE,
    T_KEYWORD_FOR,
    T_KEYWORD_FOREACH,
    T_KEYWORD_IF,
    T_KEYWORD_IMPORT,
    T_KEYWORD_IN,
    T_KEYWORD_MODULE,
    T_KEYWORD_NEW,
    T_KEYWORD_PERSIST,
    T_KEYWORD_RETURN,
    T_KEYWORD_SIZEOF,
    T_KEYWORD_STRUCT,
    T_KEYWORD_SWITCH,
    T_KEYWORD_TRUE,
    T_KEYWORD_TYPEDEF,
    T_KEYWORD_WHILE,
    T_LEFT_PAR,
    T_RIGHT_PAR,
    T_COMMA,
    T_DOT,
    T_COLON,
    T_SEMICOLON,
    T_COMPARATION_OP,
    T_TERNARY_OP,
    T_LEFT_SQUARE,
    T_RIGHT_SQUARE,
    T_LEFT_CURLY,
    T_RIGHT_CURLY,
    T_ARROW
};


typedef struct {
    int type;
    char* value;
    unsigned int line;
    unsigned int column;
    char* file;
} token;

#define KEYWORDS_SIZE 55
#define SYMBOLS_SIZE 40

static const char* keywords[KEYWORDS_SIZE] = {
    "and","as","bool","break","case",
    "char","class","const","continue","default",
    "delete","do","else","enum",
    "false","for","foreach","foreign","function",
    "if","import","in","int","int16","int32","int64","int8",
    "module","new","not","null","num","num16","num32","num64",
    "or","persist","private","protected","public",
    "return","sizeof","string","struct","switch",
    "true","typedef","uint","uint16",
    "uint32","uint64","uint8","var","while","xor"
};

// There are 27 keywords in the Quarzum language, but internally,
// primitive type names are keywords.
static const int keyword_types[KEYWORDS_SIZE] = {
    T_LOGICAL_OP, T_KEYWORD_AS, T_TYPE, T_KEYWORD_BREAK, T_KEYWORD_CASE,
    T_TYPE, T_KEYWORD_CLASS, T_SPECIFIER, T_KEYWORD_CONTINUE, T_KEYWORD_DEFAULT,
    T_KEYWORD_DELETE, T_KEYWORD_DO, T_KEYWORD_ELSE, T_KEYWORD_ENUM, T_KEYWORD_FALSE, T_KEYWORD_FOR, T_KEYWORD_FOREACH, T_SPECIFIER,
    T_TYPE, T_KEYWORD_IF, T_KEYWORD_IMPORT, T_KEYWORD_IN,
    T_TYPE, T_TYPE, T_TYPE, T_TYPE, T_TYPE, T_KEYWORD_MODULE, T_KEYWORD_NEW,
    T_UNARY, T_NULL_LITERAL, T_TYPE, T_TYPE, T_TYPE, T_TYPE, T_LOGICAL_OP,
    T_KEYWORD_PERSIST, T_ACCESS, T_ACCESS, T_ACCESS, T_KEYWORD_RETURN,
    T_KEYWORD_SIZEOF,
    T_TYPE, T_KEYWORD_STRUCT, T_KEYWORD_SWITCH, T_KEYWORD_TRUE,
    T_KEYWORD_TYPEDEF, T_TYPE, T_TYPE, T_TYPE, T_TYPE,
    T_TYPE, T_TYPE, T_KEYWORD_WHILE, T_LOGICAL_OP

};

static const char* symbols[SYMBOLS_SIZE] = {
    "!","!=","#","#=","%","%=","&","&=","(",")","*","*=","+","++","+=",",","-","--","-=",
    ".","/","/=",":",";","<","<=","=","==","=>",">",">=","?","[","]","^","^=","{","|","|=","}"
};

static const int symbol_types[SYMBOLS_SIZE] = {
    T_BITWISE_OP, T_ASSIGN_OP, T_BITWISE_OP, T_ASSIGN_OP, T_ARITHMETIC_OP,
    T_ASSIGN_OP, T_BITWISE_OP, T_ASSIGN_OP, T_LEFT_PAR, T_RIGHT_PAR, T_ARITHMETIC_OP,
    T_ASSIGN_OP, T_ARITHMETIC_OP, T_UNARY, T_ASSIGN_OP, T_COMMA, T_ARITHMETIC_OP,
    T_UNARY, T_ASSIGN_OP, T_DOT, T_ARITHMETIC_OP, T_ASSIGN_OP, T_COLON, T_SEMICOLON,
    T_COMPARATION_OP, T_COMPARATION_OP, T_ASSIGN_OP, T_COMPARATION_OP,T_ARROW ,T_COMPARATION_OP,
    T_COMPARATION_OP, T_TERNARY_OP, T_LEFT_SQUARE, T_RIGHT_SQUARE, T_ARITHMETIC_OP,
    T_ASSIGN_OP, T_LEFT_CURLY, T_BITWISE_OP, T_ASSIGN_OP, T_RIGHT_CURLY

};

#define DEFAULT_TOKENIZER_BUFFER_SIZE 10

typedef struct {
    char* input;
    unsigned int line;
    unsigned int column;
    unsigned int pos;
    char* file;
    token* tok;
} lexer;

lexer* init_lexer(char* filename, char* input);

token* new_token(int type, lexer* lexer);

token* next_token(lexer* lexer);

void read_next(lexer* lexer);

char* get_error_line(lexer* lexer);


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
    N_RETURN,
    N_LAMBDA,
    // Expression nodes
    N_UNARY_EXPR,
    N_BINARY_EXPR,
    N_PAREN_EXPR,
    N_TERNARY_EXPR,
    N_MEMBER_EXPR,
    N_CALL_EXPR,
    N_NULL_EXPR,

    N_LITERAL,
    N_CLASS,
    N_INIT_LIST
};

typedef struct {
    int type;
    vector* children;
} node;

typedef struct {
    node* ast;
    vector* symbol_table;
    bool has_errors;
    // type table

} parse_tree;

node* init_node(unsigned int children, int type);
void expect(token* t, int type, char* what);
parse_tree* parse(char* file);

node* null_expr();


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
    TY_VAR,
    TY_CUSTOM,
    TY_NULL
};

#define CONST_FLAG      0b00000001
#define UNSIGNED_FLAG   0b00000010
#define FOREIGN_FLAG    0b00000100
#define STRUCT_FLAG     0b00001000
#define POINTER_FLAG    0b00010000
#define FUNCTION_FLAG   0b00100000
#define LAMBDA_FLAG     0b01000000

typedef struct {
    int type;
    char* name;
    unsigned int align;
    unsigned int size;
    unsigned int flags;
    void* info;
} type;

static type* ty_function = &(type){TY_FUNCTION,"function", 1, 1};
static type* ty_bool =     &(type){TY_BOOL,"bool", 1, 1, UNSIGNED_FLAG};
static type* ty_char =     &(type){TY_CHAR,"char", 1, 1};

static type* ty_int8 =     &(type){TY_INT,"int8", 1, 1};
static type* ty_int16 =    &(type){TY_INT,"int16", 2, 2};
static type* ty_int32 =    &(type){TY_INT,"int32", 4, 4};
static type* ty_int64 =    &(type){TY_INT,"int64", 8, 8};

static type* ty_uint8 =     &(type){TY_UINT,"uint8", 1, 1, UNSIGNED_FLAG};
static type* ty_uint16 =    &(type){TY_UINT,"uint16", 2, 2, UNSIGNED_FLAG};
static type* ty_uint32 =    &(type){TY_UINT,"uint32", 4, 4, UNSIGNED_FLAG};
static type* ty_uint64 =    &(type){TY_UINT,"uint64", 8, 8, UNSIGNED_FLAG};

static type* ty_num16 =    &(type){TY_NUM,"num16", 2, 2};
static type* ty_num32 =    &(type){TY_NUM,"num32", 4, 4};
static type* ty_num64 =    &(type){TY_NUM,"num64", 8, 8};

static type* ty_string =   &(type){TY_STRING,"string", 4, 4, POINTER_FLAG};
static type* ty_null =    &(type){TY_NULL,"null", 1, 1};


// 
//  symbol.c
//

enum {
    S_GLOBAL,
    S_LOCAL,
    S_PARAMETER
};

typedef struct {
    char* name;
    type* type;
    int scope;

} symbol;

typedef struct {
    vector* args;
    unsigned int local_variables_len;
    unsigned int local_variables_size;
    unsigned int align;
    symbol** local_variables; 
} function_info;

void mangle_name(symbol* s);
int try_add_symbol(vector* table, symbol* s);
int merge_symbol_tables(vector* dest, vector* add);
symbol* get_symbol(vector* dest, char* name);

//
//  check.c
//

void checkAST(node* ast);
void checkNode(node* node, vector* symbols);

//
//  codegen.c
//

typedef struct {
    string* data_section;
    string* bss_section;
    string* text_section;
} asm_code;

asm_code* code_gen(vector* ir);

//
//  ir.c
//
enum {
    I_GLOBAL,
    I_ASSIGN,
    I_EXIT,
    I_FUNCTION,
    I_CALL,
    I_PARAM,
    I_LEAVERET,
    I_BRANCH,
    I_CMPTRUE,
    I_IF,
    I_NIF,
    I_JMP,

    I_ADD
};

#define INSTRUCTION_LIST_DEFAULT_SIZE 32
typedef struct {
    int type;
    char* dest;
    char* arg1;
    char* arg2;
    void* data;
} instruction;

vector* generate_ir(node* ast);

#define JMP(dest) vector_push(ir_list, init_instruction(I_JMP, dest, NULL , NULL, NULL))
#define BRANCH(dest) vector_push(ir_list, init_instruction(I_BRANCH, dest, NULL , NULL, NULL))

#endif