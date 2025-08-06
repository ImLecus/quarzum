/**
 * \file            type.c
 * \brief           Quarzum type implementations
 * \copyright       2024 Marcos González GPL3
 */
#include "type.h"

Hashmap* type_map;

Hashmap* init_type_map(){
    Hashmap* type_map = init_hashmap(64);
    hashmap_add(type_map, "int8", ty_int8);
    hashmap_add(type_map, "int16", ty_int16);
    hashmap_add(type_map, "int32", ty_int32);
    hashmap_add(type_map, "int", ty_int32);
    hashmap_add(type_map, "int64", ty_int64);
    hashmap_add(type_map, "uint8", ty_uint8);
    hashmap_add(type_map, "uint16", ty_uint16);
    hashmap_add(type_map, "uint32", ty_uint32);
    hashmap_add(type_map, "uint", ty_uint32);
    hashmap_add(type_map, "uint64", ty_uint64);
    hashmap_add(type_map, "char", ty_char);
    hashmap_add(type_map, "num16", ty_num16);
    hashmap_add(type_map, "num32", ty_num32);
    hashmap_add(type_map, "num", ty_num32);
    hashmap_add(type_map, "num64", ty_num32);
    hashmap_add(type_map, "function", ty_function);
    hashmap_add(type_map, "bool", ty_bool);
    hashmap_add(type_map, "var", ty_var);
    hashmap_add(type_map, "string", ty_str);

    return type_map;
}

int compare_types(Type* a, Type* b){
    if(a->type == TY_VAR){return 1;}
    if(b->type == TY_NULL){return 1;}
    
    if(a->type != b->type){
        return 0;
    }
    if(a->size != b->size){
        return 0;
    }
    return 1;
}

Type* merge_types(Type* a, Type* b, char op){
    if(compare_types(a,b)){
        return a;
    }
    return ty_var;
}

inline void convert_to_pointer(Type* t){
    t->align = 8;
    t->flags |= POINTER_FLAG;
    t->size = 8;
    String* new_name = init_string(strlen(t->name) + 1);
    string_append(new_name, t->name);
    string_push(new_name, '*');
    t->name = string_copy(new_name);
    free_string(new_name);
    t->type = TY_PTR;
}