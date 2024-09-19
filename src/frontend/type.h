/**
 * \file            type.h
 * \brief           Quarzum types include file
 * \copyright       2024 Marcos González GPL3
 */
#ifndef TYPE_H
#define TYPE_H
#include "../core/hashmap.h"
#include "../core/string.h"
enum {
    TY_FUNCTION,
    TY_INT,
    TY_NUM,
    TY_STRUCT,
    TY_CUSTOM,
    TY_VAR, // "var" is basically a pointer of any type
    TY_NULL,
    TY_PTR,
    TY_MODULE 
};

#define CONST_FLAG      0b00000001
#define UNSIGNED_FLAG   0b00000010
#define FOREIGN_FLAG    0b00000100
#define STRUCT_FLAG     0b00001000
#define POINTER_FLAG    0b00010000
#define FUNCTION_FLAG   0b00100000
#define LAMBDA_FLAG     0b01000000
#define ENUM_FLAG       0b10000000

typedef struct {
    unsigned char type;
    char* name;
    unsigned int align, size;
    unsigned char flags;
} type;

static type* ty_function = &(type){TY_FUNCTION,"function", 0, 0};
static type* ty_bool =     &(type){TY_INT,"bool", 1, 1, UNSIGNED_FLAG};
static type* ty_char =     &(type){TY_INT,"char", 1, 1};

static type* ty_int8 =     &(type){TY_INT,"int8", 1, 1};
static type* ty_int16 =    &(type){TY_INT,"int16", 2, 2};
static type* ty_int32 =    &(type){TY_INT,"int32", 4, 4};
static type* ty_int64 =    &(type){TY_INT,"int64", 8, 8};

static type* ty_uint8 =     &(type){TY_INT,"uint8", 1, 1, UNSIGNED_FLAG};
static type* ty_uint16 =    &(type){TY_INT,"uint16", 2, 2, UNSIGNED_FLAG};
static type* ty_uint32 =    &(type){TY_INT,"uint32", 4, 4, UNSIGNED_FLAG};
static type* ty_uint64 =    &(type){TY_INT,"uint64", 8, 8, UNSIGNED_FLAG};

static type* ty_num16 =    &(type){TY_NUM,"num16", 2, 2};
static type* ty_num32 =    &(type){TY_NUM,"num32", 4, 4};
static type* ty_num64 =    &(type){TY_NUM,"num64", 8, 8};

static type* ty_string =   &(type){TY_PTR,"char*", 8, 8, POINTER_FLAG};
static type* ty_var =      &(type){TY_VAR,"var", 8, 8, POINTER_FLAG};
static type* ty_null =     &(type){TY_NULL,"null",0,0};

static type* ty_module = &(type){TY_MODULE,"module",0,0};

extern hashmap_t* type_map;

hashmap_t* init_type_map();

int compare_types(type* a, type* b);
type* merge_types(type* a, type* b, char op);
void convert_to_pointer(type* t);

#define is_ptr(TYPE) ((TYPE->flags & POINTER_FLAG) > 0)

#endif /* TYPE_H */