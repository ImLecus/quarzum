/*
 * Quarzum Compiler - type.h
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
#ifndef TYPE_H
#define TYPE_H
#include "defs.h"
#include "../core/bsearch.h"

typedef enum {
    Integer,
    Number,
    UInteger,
    Character,
    String,
    Function,
    Null,
    Decimal,
    Array,
    Pointer,
    Custom,
} TypeKind;

typedef struct {
    TypeKind base;
    unsigned int sizeInBytes;
    char* customName;
    struct Type* parent;
    struct Type** argTypes;
    unsigned int arraySize;

} Type;

// static Type int8 =  {Integer, 1};
// static Type int16 = {Integer, 2};
// static Type int32 = {Integer, 4};
// static Type int64_t = {Integer, 8};

// static Type uint8 =  {UInteger, 1};
// static Type uint16 = {UInteger, 2};
// static Type u_int32_t = {UInteger, 4};
// static Type u_int64_t = {UInteger, 8};

// static Type character = {Character, 1};

// static Type num = {Number, 4};
// static Type num16 = {Number, 2};
// static Type num64 = {Number, 8};

// static Type decimal = {Decimal, 8};

// static Type function = {Function, 0};

// #define DEFINED_TYPES_SIZE 19

// static const char* definedTypes[DEFINED_TYPES_SIZE] = {
//     "char",
//     "int",
//     "int16",
//     "int32",
//     "int64_t",
//     "int8",
//     "uint",
//     "uint16",
//     "u_int32_t", 
//     "u_int64_t",
//     "uint8",
// };

// Type* getTypeFromToken(token t);

#endif