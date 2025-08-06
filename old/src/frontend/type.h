/**
 * \file            type.h
 * \brief           Quarzum types include file
 * \copyright       2024 Marcos González GPL3
 */
#ifndef TYPE_H
#define TYPE_H
#include "../core/hashmap.h"
#include "../core/string.h"
typedef enum {
    TY_FUNCTION,
    TY_INT,
    TY_NUM,
    TY_STRUCT,
    TY_CUSTOM,
    TY_VAR, // "var" is basically a pointer of any type
    TY_NULL,
    TY_PTR,
} Datatype;

#define MUTABLE_FLAG    0b00000001
#define UNSIGNED_FLAG   0b00000010
#define FOREIGN_FLAG    0b00000100
#define STRUCT_FLAG     0b00001000
#define POINTER_FLAG    0b00010000
#define FUNCTION_FLAG   0b00100000
#define LAMBDA_FLAG     0b01000000
#define ENUM_FLAG       0b10000000

/**
 * TODO: change this structure. Pointers must have
 * a "struct Type* pointer_to" instead of masking the name 
 */

typedef struct Type {
    Datatype type;
    const char* name;

    unsigned int align, size;
    unsigned char flags;

    struct Type* pointer_to;
    unsigned int array_size;
    
} Type;

static Type* const ty_function = &(Type){TY_FUNCTION,"function", 0, 0};
static Type* const ty_bool =     &(Type){TY_INT,"bool", 1, 1, UNSIGNED_FLAG};
static Type* const ty_char =     &(Type){TY_INT,"char", 1, 1};

static Type* const ty_int8 =     &(Type){TY_INT,"int8", 1, 1};
static Type* const ty_int16 =    &(Type){TY_INT,"int16", 2, 2};
static Type* const ty_int32 =    &(Type){TY_INT,"int32", 4, 4};
static Type* const ty_int64 =    &(Type){TY_INT,"int64", 8, 8};

static Type* const ty_uint8 =     &(Type){TY_INT,"uint8", 1, 1, UNSIGNED_FLAG};
static Type* const ty_uint16 =    &(Type){TY_INT,"uint16", 2, 2, UNSIGNED_FLAG};
static Type* const ty_uint32 =    &(Type){TY_INT,"uint32", 4, 4, UNSIGNED_FLAG};
static Type* const ty_uint64 =    &(Type){TY_INT,"uint64", 8, 8, UNSIGNED_FLAG};

static Type* const ty_num16 =    &(Type){TY_NUM,"num16", 2, 2};
static Type* const ty_num32 =    &(Type){TY_NUM,"num32", 4, 4};
static Type* const ty_num64 =    &(Type){TY_NUM,"num64", 8, 8};

static Type* const ty_str =      &(Type){TY_PTR,"string", 8, 8, POINTER_FLAG};
static Type* const ty_var =      &(Type){TY_VAR,"var", 8, 8, POINTER_FLAG};
static Type* const ty_null =     &(Type){TY_NULL,"null",0,0};

extern Hashmap* type_map;

Hashmap* init_type_map();

int compare_types(Type* a, Type* b);
Type* merge_types(Type* a, Type* b, char op);
void convert_to_pointer(Type* t);

#define is_ptr(TYPE) ((TYPE->flags & POINTER_FLAG) > 0)

#endif /* TYPE_H */