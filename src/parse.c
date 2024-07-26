#include "quarzum.h"

// inline int match(token* tok, char* value){
//     return strcmp(tok->value, value);
// }

node* parse_statement(vector* tokens, unsigned int* pos){
    token* t = (token*)vector_get(tokens, *pos);
    // if(match(t, "break")){
    //     print("Hello!");
    // }
    return NULL;
}

node* parse(vector* tokens){
    unsigned int pos = 0;
    parse_statement(tokens, &pos);
    return NULL;
}