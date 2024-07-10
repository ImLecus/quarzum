/*
 * Quarzum Compiler - symbol.h
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
#ifndef SYMBOL_H
#define SYMBOL_H
#include "type.h"
#include <stdlib.h>
#define constFlag 0x001
#define staticFlag 0x010
#define foreignFlag 0x100

typedef enum {
    Global,
    Local,
    Parameter
} Scope;

typedef enum {
    Public,
    Private,
    Protected
} Access;

typedef struct {
    Type* type;
    char* name;
    Scope scope;
    unsigned int flags;
} Symbol;

typedef struct {
    Symbol* content;
    unsigned long size;
    unsigned long capacity;
} SymbolTable;

#define DEFAULT_SYMBOL_SIZE 20

SymbolTable* createSymbolTable();

void deleteSymbolTable(SymbolTable* table);

void addSymbol(SymbolTable* table, Symbol symbol);

#endif