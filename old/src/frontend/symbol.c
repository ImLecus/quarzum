#include "symbol.h"
#define USER_DEFINED_PREFIX "_Z"

Symbol* const find_symbol(Table* table, const char* const name){
    Symbol* s = hashmap_get(table->symbols, name);
    if(s != NULL){
        return s;
    }
    if(table->parent == NULL){
        return NULL;
    }
    return find_symbol(table->parent, name);
}

inline void insert_symbol(SymbolTable* symbol_table, Symbol* const s){
    hashmap_add(symbol_table->last_table->symbols, s->name, s);
}

inline void close_scope(SymbolTable* const table){
    table->last_table = table->last_table->parent;
}

void add_scope(SymbolTable* const table, const char* const name, Scope scope){
    Table* const t = malloc(sizeof(Table));
    if(t == NULL) return;
    t->parent = table->last_table;
    t->name = name;
    t->scope = scope;
    t->symbols = init_hashmap(16);
    table->last_table = t;
}

void init_symbol_table(SymbolTable* symbol_table){
    symbol_table = malloc(sizeof(SymbolTable));
    if(symbol_table == NULL) return;
    symbol_table->last_table = NULL;
    symbol_table->tables = init_vector(128);
    add_scope(symbol_table, "@global", S_GLOBAL);
}


const char* mangle_namespace(const char* id, char* last_namespace){
    int len = strlen(last_namespace);
    String* ns = init_string(len+strlen(id)+2);
    
    if( len > 0 ) { 
        string_append(ns, last_namespace);
        string_append(ns, "::"); 
    }
    string_append(ns, id);
    const char* result = string_copy(ns);
    free_string(ns);
    return result;
}