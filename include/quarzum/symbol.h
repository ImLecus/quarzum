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
#define constFlag   0b001
#define staticFlag  0b010
#define foreignFlag 0b100

/**
 * @brief Defines the scope of a Symbol. 
 */
typedef enum {
    Global,
    Local,
    Parameter
} Scope;
/**
 * @brief Defines the access scope of a Symbol.
 * Default is used in symbols with no access specifier.
 * Public is avaliable in every code fragment where the symbol is defined.
 * Private is only avaliable for a class.
 * Protected is avaliable for a class and its child classes.
 */
typedef enum {
    Default,
    Public,
    Private,
    Protected
} Access;
/**
 * @brief Defines a Symbol. It contains relevant information for the semantic
 * analisys and code generation. It includes the identifier of the variable,
 * the type, the scope, the access specification and the flags, that defines 
 * special behaviour like constants, foreign declarations or static methods.
 */
typedef struct {
    char* id;
    Type* type;
    Scope scope;
    Access access;
    u_int8_t flags;
} Symbol;

/**
 * @brief Defines a SymbolTable, which is basically a vector of symbols.
 * SymbolTable can merge with another SymbolTable, only if there is no
 * id collission between them.
 */
typedef struct {
    Symbol* content;
    unsigned long size;
    unsigned long capacity;
} SymbolTable;

#define DEFAULT_SYMBOL_SIZE 20

/**
 * @brief Creates a new SymbolTable with size DEFAULT_SYMBOL_SIZE.
 */
SymbolTable* createSymbolTable();
/**
 * @brief Deletes a SymbolTable.
 */
void deleteSymbolTable(SymbolTable* table);
/**
 * @brief Adds a new Symbol to a SymbolTable. 
 * Returns -1 if there is an id collission.
 */
int addSymbol(SymbolTable* table, Symbol symbol);
/**
 * @brief Merges two SymbolTable and stores the result in the first.
 * In case of error, it will return -1.
 */
int mergeSymbolTable(SymbolTable* target, SymbolTable* other);

#endif