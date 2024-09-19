#ifndef QUARZUM_H
#define QUARZUM_H
#include <time.h>
#include "frontend/parse.h"
#define QUARZUM_COMPILER_VERSION "1.0"
#define QUARZUM_VERSION "1.0"
#define VERSION_TYPE "experimental"

#define max(a,b) a > b ? a: b

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


#define ERROR_MSG(msg) RED"[ERROR] "RESET"%s\n",msg
#define LOG_MSG(msg) CYAN"[LOG] "RESET"%s\n",msg
#define WARN_MSG(msg) BOLD ORANGE"[WARNING] "RESET"%s\n",msg
#define DEBUG_MSG(msg) MAGENTA"[DEBUG] "RESET"%s\n",msg

//
//  process.c
//
struct process {
    char* name;
    int start, end;
};

struct process start_process(char* name);
void end_process(struct process* process);

// 
//  symbol.c
//
extern hashmap_t* symbol_table;

enum {
    S_GLOBAL,
    S_LOCAL,
    S_PARAMETER,
    S_CLASS,
    S_EXTEND
};

enum {
    ST_VAR,
    ST_FUNC,
    ST_LAMBDA,
    ST_PARAMETER
};

typedef struct {
    char *mangled_name, *name;
    type* type;
    int8_t scope;

    void* info;
} symbol;

typedef struct {
    uint8_t min_args;
    vector *args, *optional_values, *local_vars;
    uint32_t align;
} function_info;

char* mangle_name(symbol* s);
char* mangle_namespace(char* id, char* last_namespace);
//
//  check.c
//

void check_ast(node* tree);

#endif