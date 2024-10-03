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
extern Hashmap* symbol_table;

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

typedef struct {
    uint8_t min_args;
    Vector *args, *optional_values, *local_vars;
    uint32_t align;
} function_info;

/**
 * \brief           Defines a symbol.
 */
typedef struct {
    const char *mangled_name, *name;
    Type* type;
    scope_t scope;
    function_info* info;
    pos_t defined_pos;
} symbol;



const char* mangle_name(symbol* s);
const char* mangle_namespace(const char* id, char* last_namespace);


#endif /* SYMBOL_H */
