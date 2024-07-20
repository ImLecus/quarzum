/*
 * Quarzum Compiler - symbol.c
 * Version 1.0, 10/07/2024
 *
 * This file is part of the Quarzum project, a proprietary software.
 *
 * Quarzum Project License
 * ------------------------
 *
 * For Contributions License Agreement (CLA), see CONTRIBUTING.md.
 * For full details, see LICENSE.
 */
#include "../../include/quarzum/symbol.h"

inline SymbolTable* createSymbolTable(){
    SymbolTable* table = (SymbolTable*)malloc(sizeof(SymbolTable));
    table->content = (Symbol*)malloc(DEFAULT_SYMBOL_SIZE*sizeof(Symbol));
    table->size = 0;
    table->capacity = DEFAULT_SYMBOL_SIZE;
    return table;
}

inline void deleteSymbolTable(SymbolTable* table){
    free(table->content);
    free(table);
}

inline int addSymbol(SymbolTable* table, Symbol symbol){
    if(table->size >= table->capacity){
        Symbol* newContent = (Symbol*)realloc(table->content, table->capacity * 2);
        if(newContent == NULL){return -1;}
        table->capacity *= 2;
        table->content = newContent;
    }
    table->content[table->size] = symbol;
    table->size++;
    return 0;
}