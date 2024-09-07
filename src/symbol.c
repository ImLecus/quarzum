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

char* mangle_namespace(char* id, string* last_namespace){
    string* ns = init_string(strlen(last_namespace->value)+strlen(id)+2);
    string_append(ns, last_namespace);
    string_append(ns, "::");
    string_append(ns, id);
    char* result = string_copy(ns);
    free_string(ns);
    return result;
}

char* mangle_name(symbol* s, string* last_namespace){
    string* mangled_name = init_string(strlen(s->name)* 2);
    mangle_type(s->type, mangled_name);
    
    string_push(mangled_name, ';');
    string_append(mangled_name, string_copy(last_namespace));
    if(last_namespace->len > 0){
        string_append(mangled_name, "::");
    }
    
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