#include "quarzum.h"

void mangle_name(symbol* s){
    string* mangled_name = init_string(strlen(s->name));
    string_append(mangled_name, s->name);
    if( (s->type->flags & FUNCTION_FLAG) > 0 ){
        if(strcmp("main", s->name )== 0){
            return;
        }
        for(unsigned int i = 0; i < s->type->args->len; ++i){
            type* arg = s->type->args->value[i];
            int len = strlen(arg->name);
            string_push(mangled_name, '_');
            char len_buffer[2];
            sprintf(len_buffer,"%i",len);
            string_append(mangled_name, len_buffer);
            string_append(mangled_name, arg->name);
        }
    }
    s->name = string_copy(mangled_name);
    printf("%s\n",s->name);
}