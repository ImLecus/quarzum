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

#define RED "\e[31m"
#define YELLOW "\e[33m"
#define GREEN "\e[32m"
#define CYAN "\e[36m"
#define MAGENTA "\e[35m"
#define GRAY "\e[90m"
#define ORANGE "\e[38;5;214m"

#define UNDERLINE "\e[4m"
#define BOLD "\e[1m"

#define ERROR_MSG(msg) RED"[ERROR] "RESET"%s\n",msg
#define LOG_MSG(msg) CYAN"[LOG] "RESET"%s\n",msg
#define WARN_MSG(msg) ORANGE"[WARNING] "RESET"%s\n",msg
#define DEBUG_MSG(msg) MAGENTA"[DEBUG] "RESET"%s\n",msg

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
    T_KEYWORD_BREAK,
    T_KEYWORD_CASE,
    T_KEYWORD_CATCH,
    T_KEYWORD_CLASS,
    T_KEYWORD_CONTINUE,
    T_KEYWORD_DEFAULT,
    T_KEYWORD_DELETE,
    T_KEYWORD_DO,
    T_KEYWORD_ELSE,
    T_KEYWORD_ENUM,
    T_KEYWORD_EXIT,
    T_KEYWORD_FALSE,
    T_KEYWORD_FOR,
    T_KEYWORD_FOREACH,
    T_KEYWORD_FROM,
    T_KEYWORD_IF,
    T_KEYWORD_IMPORT,
    T_KEYWORD_IN,
    T_KEYWORD_MODULE,
    T_KEYWORD_NEW,
    T_KEYWORD_PERSIST,
    T_KEYWORD_RETURN,
    T_KEYWORD_STRUCT,
    T_KEYWORD_SWITCH,
    T_KEYWORD_TRUE,
    T_KEYWORD_TRY,
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

#define KEYWORDS_SIZE 57
#define SYMBOLS_SIZE 40

static const const char* keywords[KEYWORDS_SIZE] = {
    "and","bool","break","case","catch",
    "char","class","const","continue","decimal","default",
    "delete","do","else","enum","exit",
    "false","for","foreach","foreign","from","function",
    "if","import","in","int","int16","int32","int64","int8",
    "module","new","not","null","num","num16","num32","num64",
    "or","persist","private","protected","public",
    "return","string","struct","switch",
    "true","try","typedef","uint","uint16",
    "uint32","uint64","uint8","while","xor"
};

// There are 27 keywords in the Quarzum language, but internally,
// primitive type names are keywords.
static const int keyword_types[KEYWORDS_SIZE] = {
    T_LOGICAL_OP, T_TYPE, T_KEYWORD_BREAK, T_KEYWORD_CASE, T_KEYWORD_CATCH,
    T_TYPE, T_KEYWORD_CLASS, T_SPECIFIER, T_KEYWORD_CONTINUE, T_TYPE, T_KEYWORD_DEFAULT,
    T_KEYWORD_DELETE, T_KEYWORD_DO, T_KEYWORD_ELSE, T_KEYWORD_ENUM,
    T_KEYWORD_EXIT, T_KEYWORD_FALSE, T_KEYWORD_FOR, T_KEYWORD_FOREACH, T_SPECIFIER,
    T_KEYWORD_FROM, T_TYPE, T_KEYWORD_IF, T_KEYWORD_IMPORT, T_KEYWORD_IN,
    T_TYPE, T_TYPE, T_TYPE, T_TYPE, T_TYPE, T_KEYWORD_MODULE, T_KEYWORD_NEW,
    T_UNARY, T_NULL_LITERAL, T_TYPE, T_TYPE, T_TYPE, T_TYPE, T_LOGICAL_OP,
    T_KEYWORD_PERSIST, T_ACCESS, T_ACCESS, T_ACCESS, T_KEYWORD_RETURN,
    T_TYPE, T_KEYWORD_STRUCT, T_KEYWORD_SWITCH, T_KEYWORD_TRUE,
    T_KEYWORD_TRY, T_KEYWORD_TYPEDEF, T_TYPE, T_TYPE, T_TYPE, T_TYPE,
    T_TYPE, T_KEYWORD_WHILE, T_LOGICAL_OP

};

static const const char* symbols[SYMBOLS_SIZE] = {
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
    string* buffer;
    token* tok;
} lexer;

lexer* init_lexer(char* input);

token* new_token(int type, lexer* lexer);

token* next_token(lexer* lexer);

void read_next(lexer* lexer);

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
    N_MEMBER_EXPR,

    N_LITERAL,
    N_CLASS
};

typedef struct {
    int type;
    vector* children;
} node;

node* init_node(unsigned int children, int type);

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

#define CONST_FLAG      0b000001
#define UNSIGNED_FLAG   0b000010
#define FOREIGN_FLAG    0b000100
#define STRUCT_FLAG     0b001000
#define POINTER_FLAG    0b010000
#define ARRAY_FLAG      0b100000

typedef struct {
    int type;
    int align;
    unsigned int size;
    unsigned int flags;
} type;


// 
//  symbol.c
//

typedef struct {
    char* name;
    type* type;
} symbol;

char* mangle_name(char* module_name);

//
//  check.c
//

void checkAST(node* ast);
void checkNode(node* node, vector* symbols);

int binary_search(const char* symbol, const char** list, unsigned int size);

#endif