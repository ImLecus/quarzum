#ifndef QUARZUM_H
#define QUARZUM_H
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include "../include/core/string.h"
#include "../include/core/char.h"
#include "../include/quarzum/io.h"
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
    T_INT_LITERAL
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

#define KEYWORDS_SIZE 54
#define SYMBOLS_SIZE 39

static const char* keywords[KEYWORDS_SIZE] = {
    "and",
    "bool",
    "break",
    "catch",
    "char",
    "class",
    "const",
    "continue",
    "decimal",
    "delete",
    "destroy",
    "do",
    "else",
    "enum",
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

int is_keyword(char* keyword);
int is_symbol(char* symbol);
vector* tokenize(char* file);

#define DEFAULT_TOKENIZER_BUFFER_SIZE 10

#define ADD_TOKEN(t) vector_push(tokens, &(token){t, string_copy(buffer), &(struct token_info){lineNumber, columnNumber, file}});string_clear(buffer);

#define t_ch src->value[i]
#define t_next src->value[i + 1]
#define t_advance ++i;++columnNumber

#endif