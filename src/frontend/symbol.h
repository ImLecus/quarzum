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
 * \brief           Defines the scope of a `symbol_t`
 */
typedef enum {
    S_GLOBAL,
    S_LOCAL,
    S_PARAMETER,
    S_CLASS,
    S_EXTEND
} Scope;

typedef struct {
    uint8_t min_args;
    Vector *args, *optional_values, *local_vars;
    uint32_t align;
} function_info;

/**
 * \brief           Defines a symbol.
 */
typedef struct {
    const char *name;
    Type* type;
    Scope scope;
    function_info* info;
    Position defined_pos;
} Symbol;

typedef struct Table {
    Scope scope;
    const char* name;
    struct Table* parent;
    Hashmap* symbols;
} Table;

typedef struct SymbolTable {
    Table* last_table;
    Vector* tables;
} SymbolTable;

extern SymbolTable* symbol_table;

void init_symbol_table(SymbolTable* symbol_table);

void add_scope(SymbolTable* const table, const char* const name, Scope scope);

void close_scope(SymbolTable* const table);

void insert_symbol(SymbolTable* symbol_table, Symbol* const s);

Symbol* const find_symbol(Table* table, const char* const name);

const char* mangle_namespace(const char* id, char* last_namespace);


#endif /* SYMBOL_H */
