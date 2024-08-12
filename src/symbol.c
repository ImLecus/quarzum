#include "quarzum.h"

#define USER_DEFINED_PREFIX "_Z"

static void mangle_type(type* t, string* mangled_name){
    if((t->flags & CONST_FLAG) > 0){
        string_push(mangled_name, 'C');
    }
    if((t->flags & POINTER_FLAG) > 0){
        string_push(mangled_name, 'P');
    }
    if(t->type == TY_CUSTOM || t->type == TY_STRUCT){
        string_push(mangled_name, 'Z');
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

char* mangle_name(symbol* s){
    string* mangled_name = init_string(strlen(s->name)* 2);
    string_push(mangled_name, '_');
    mangle_type(s->type, mangled_name);
    
    string_push(mangled_name, '_');
    // namespace?
    string_push(mangled_name, '_');
    string_append(mangled_name, s->name);

    if( (s->type->flags & FUNCTION_FLAG) > 0 ){
        if(strcmp("main", s->name )== 0){
            return "main";
        }
        function_info* info = s->type->info;
        for(uint8_t i = 0; i < info->args->len; ++i){
            type* arg = info->args->value[i];
            string_push(mangled_name, '_');
            mangle_type(arg, mangled_name);            
        }
        
    }
    char* result = string_copy(mangled_name);
    printf("%s\n", result);
    return result;
}