/**
 * \file            symbol.h
 * \brief           Quarzum symbol include file.
 * \copyright       2024 Marcos González GPL3
 */
#ifndef SYMBOL_H
#define SYMBOL_H
#include "type.h"
#include "../core/vector.h"
#include "../core/error.h"
/**
 * \brief           Defines a unique symbol table to all the program.
 * `symbol_table` is initialized after the parse phase. Do not use it
 * before initialization.
 */
extern hashmap_t* symbol_table;

/**
 * \brief           Defines the scope of a `symbol_t`
 */
typedef enum {
    S_GLOBAL,
    S_LOCAL,
    S_PARAMETER,
    S_CLASS,
    S_EXTEND
} scope_t;

/**
 * \brief           Defines a symbol.
 */
typedef struct {
    char *mangled_name, *name;
    type* type;
    scope_t scope;
    void* info;
    pos_t defined_pos;
} symbol;

typedef struct {
    uint8_t min_args;
    vector_t *args, *optional_values, *local_vars;
    uint32_t align;
} function_info;

char* mangle_name(symbol* s);
char* mangle_namespace(char* id, char* last_namespace);


#endif /* SYMBOL_H */
