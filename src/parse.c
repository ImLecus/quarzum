#include "quarzum.h"

node* init_node(unsigned int children){
    node* n = (node*)malloc(sizeof(node));
    n->children = init_vector(children);
    n->children_count = 0;
    return n;
}




node* parse(){
    lexer* lexer = init_lexer(read_file("code.qz")->value);
    token* tok;
    while (tok = next_token(lexer))
    {
        parse_expr(lexer);
    }
    free(lexer);
    return NULL;
}