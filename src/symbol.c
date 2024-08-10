#include "quarzum.h"

#define USER_DEFINED_PREFIX "_Z"

void mangle_name(symbol* s){
    string* mangled_name = init_string(strlen(s->name)* 2);
    string_push(mangled_name, '_');
    if((s->type->flags & CONST_FLAG) > 0){
        string_push(mangled_name, 'C');
    }
    if((s->type->flags & POINTER_FLAG) > 0){
        string_push(mangled_name, 'P');
    }
    if(s->type->type == TY_CUSTOM || s->type->type == TY_STRUCT){
        string_push(mangled_name, 'Z');
        string_append(mangled_name, s->type->name);
    }
    else{
        char first_char =  s->type->name[0];
        string_push(mangled_name,first_char);
        if(first_char == 'i' || first_char == 'u'){
            char size[2];
            sprintf(size, "%d", s->type->size * 8);
            string_push(mangled_name,size[0]);
            string_push(mangled_name,size[1]);
        }
    }
    
    string_push(mangled_name, '_');
    // namespace?
    string_push(mangled_name, '_');
    string_append(mangled_name, s->name);

    if( (s->type->flags & FUNCTION_FLAG) > 0 ){
        if(strcmp("main", s->name )== 0){
            return;
        }
        function_info* info = s->type->info;

    // TO-DO: solve error when accessing info->args

    //     for(uint32_t i = 0; i < info->args->len; ++i){
    //         type* arg = info->args->value[i];
    //         int len = strlen(arg->name);
    //         string_push(mangled_name, '_');
    //         char len_buffer[2];
    //         sprintf(len_buffer,"%i",len);
    //         string_append(mangled_name, len_buffer);
    //         string_append(mangled_name, arg->name);
    //     }
    }
    //s->name = string_copy(mangled_name);
    //printf("%s\n", string_copy(mangled_name));
}