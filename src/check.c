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
    case N_LITERAL:
        return expr->children->value[1];
    case N_CALL_EXPR:
        uint8_t args = expr->children->len - 1;
        // find the associated function, check the argument number is correct 
        // and replace with lambda expr if it is
        symbol* s = hashmap_get(symbol_table, expr->children->value[0]);
        if(!s){
            printf(RED"[ERROR] "RESET"Symbol '%s' does not exist \n", expr->children->value[0]);
            has_errors = true;
            break;
        }
        function_info* i = s->type->info;
        if(!i){
            printf(RED"[ERROR] "RESET"Symbol '%s' is not a callable function \n");
            has_errors = true;
            break;
        }
        if(i->args->len != args){
            printf(RED"[ERROR] "RESET"Incorrect number of arguments for call '%s'\n");
            has_errors = true;
            break;
        }
        // check that the type of the argument expressions (expr->children->value[>1]) are
        // equal to the expected types.
        for(uint8_t n = 1; n < expr->children->len; ++n){
            type* t = check_expr(expr->children->value[n]);
            type* t2 = i->args->value[n - 1];
            if(!compare_types(t,t2)){
                printf(RED"[ERROR] "RESET"Types '%s', '%s' do not match \n", t->name, t2->name);
                break;
            }
        }
        printf("%s (matches arg number: %s)\n",s->mangled_name, (i->args->len == args)? "true":"false");


        if((s->type->flags & LAMBDA_FLAG) > 0){
            // replace the call_expression to the lambda expression, changing the parameters inside the 
            // expression for the argument expressions.
        }
        
        return s->type;
    
    default:
        break;
    }

    return ty_var;
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
