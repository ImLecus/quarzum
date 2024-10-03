#include "symbol.h"
#define USER_DEFINED_PREFIX "_Z"

static void mangle_type(Type* t, String* mangled_name){
    if(t->flags & MUTABLE_FLAG){
        string_push(mangled_name, 'C');
    }
    if(t->flags & POINTER_FLAG){
        string_push(mangled_name, 'P');
    }
    if(t->type == TY_CUSTOM || t->type == TY_STRUCT){
        string_append(mangled_name, t->name);
    }
    else{
        char first_char = t->name[0];
        string_push(mangled_name,first_char);
        if(first_char == 'i' || first_char == 'u'){
            char size[2];
            sprintf(size, "%d", t->size * 8);
            string_push(mangled_name,size[0]);
            string_push(mangled_name,size[1]);
        }
    }
}

const char* mangle_namespace(const char* id, char* last_namespace){
    int len = strlen(last_namespace);
    String* ns = init_string(len+strlen(id)+2);
    
    if( len > 0 ) { 
        string_append(ns, last_namespace);
        string_append(ns, "::"); 
    }
    string_append(ns, id);
    const char* result = string_copy(ns);
    free_string(ns);
    return result;
}

const char* mangle_name(symbol* s){
    String* mangled_name = init_string(strlen(s->name)* 2);
    mangle_type(s->type, mangled_name);
    
    string_push(mangled_name, ';');
    string_append(mangled_name, s->name);

    if( (s->type->flags & FUNCTION_FLAG) > 0 ){
        if(strcmp(s->name, "main") == 0){return "main";}

        function_info* info = s->info;
        for(uint8_t i = 0; i < info->args->len; ++i){
            symbol* arg = info->args->value[i];
            string_push(mangled_name, ';');
            mangle_type(arg->type, mangled_name);            
        }
        
    }
    return string_copy(mangled_name);
}