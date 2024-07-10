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

SymbolTable* createSymbolTable(){
    SymbolTable* table = (SymbolTable*)malloc(sizeof(SymbolTable));
    table->content = (Symbol*)malloc(DEFAULT_SYMBOL_SIZE*sizeof(Symbol));
    table->size = 0;
    table->capacity = DEFAULT_SYMBOL_SIZE;
    return table;
}

void deleteSymbolTable(SymbolTable* table){
    free(table->content);
    free(table);
}

void addSymbol(SymbolTable* table, Symbol symbol){
    // if(table->size >= table->capacity){
    //     table->content = (Symbol*)realloc(table->content, table->capacity*2);
    //     table->capacity*=2;
    // }
    table->content[table->size] = symbol;
    table->size++;
}