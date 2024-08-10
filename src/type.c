#include "quarzum.h"

hashmap* init_type_map(){
    hashmap* type_map = init_hashmap(30);

    hashmap_add(type_map, "string", ty_string);
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
    hashmap_add(type_map, "var", ty_null);

    return type_map;
}