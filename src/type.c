#include "quarzum.h"

hashmap* init_type_map(){
    hashmap* type_map = init_hashmap(64);
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

    return type_map;
}

bool compare_types(type* a, type* b){
    if(a->type == TY_VAR){return true;}
    if(b->type == TY_NULL){return true;}
    
    if(a->type != b->type){
        return false;
    }
    if(strcmp(a->name, b->name) != 0){
        return false;
    }
    if(a->size != b->size){
        return false;
    }
    return true;
}

type* merge_types(type* a, type* b, char op){
    if(compare_types(a,b)){
        return a;
    }
    return ty_var;
}

inline void convert_to_pointer(type* t){
    t->align = 8;
    t->flags |= POINTER_FLAG;
    t->size = 8;
    char* new_name = malloc(strlen(t->name) + 1);
    strcat(new_name, t->name);
    strcat(new_name, "*");
    t->name = new_name;
    t->type = TY_PTR;
}