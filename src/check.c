#include "quarzum.h"
static bool has_errors = false;
static bool has_main_func = false;
static hashmap* symbol_table;
static hashmap* type_table;

static void check_symbol(symbol* s){
    // check for duplicated symbols (except functions with different parameters)
    // mangle names


    if(strcmp(s->type->name, "function") == 0 && strcmp(s->name, "main") == 0){
        has_main_func = true;
    }
}

static type* check_expr(node* expr){
    // check the type rules 

    switch (expr->type)
    {
    case N_CALL_EXPR:
        uint8_t args = expr->children->len - 1;
        // find the associated function, check the argument number is correct 
        // and replace with lambda expr if it is
        break;
    
    default:
        break;
    }

    return NULL;
}

static void check_var(node* var){
    symbol* s = var->children->value[0];
    check_symbol(s);
    node* expr =  var->children->value[1];
    check_expr(expr);
}

static void check_func(node* func){
    symbol* s = func->children->value[0];
    check_symbol(s);

    // check for "return" statements in case the function has a return value
}

static void check_lambda(node* lambda){
    symbol* s = lambda->children->value[0];
    check_symbol(s);
    node* expr = lambda->children->value[1];
    check_expr(expr);
}

void check_parse_tree(parse_tree* tree){
    if(!tree){
        return;
    }
    symbol_table = tree->symbol_table;
    type_table = tree->type_table;
    
    for(uint32_t i = 0; i < tree->ast->children->len; ++i){
        node* n = tree->ast->children->value[i];
        switch (n->type)
        {
        case N_VAR:
            check_var(n);
            break;
        case N_FUNCTION:
            check_func(n);
            break;
        case N_LAMBDA:
            check_lambda(n);
            break;

        // check modules and mangle namespaces

        // check type definitions

        // check typedefs
        
        default:
            break;
        }
    }
}
