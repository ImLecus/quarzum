#include "quarzum.h"
static bool has_errors = false;
static bool has_main_func = false;
static hashmap* symbol_table;
static hashmap* type_table;
static void check_statement(node* n);

static void check_symbol(symbol* s){
    // check for duplicated symbols (except functions with different parameters)
    


    if(strcmp(s->type->name, "function") == 0 && strcmp(s->name, "main") == 0){
        has_main_func = true;
    }
}


static void check_type_compatibility(type* a, type* b){
    if(!compare_types(a, b)){
        printf(RED"[ERROR] "RESET"Expected '%s'; received '%s'\n", a->name, b->name);
        has_errors = true;
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

        // TO-DO: expand this function for non-mandatory arguments
        for(uint8_t n = 1; n < expr->children->len; ++n){
            type* t = check_expr(expr->children->value[n]);
            type* t2 = i->args->value[n - 1];
            if(!compare_types(t,t2)){
                printf(RED"[ERROR] "RESET"Types '%s' and '%s' do not match \n", t->name, t2->name);
                has_errors = true;
                break;
            }
        }
        //printf("%s (matches arg number: %s)\n",s->mangled_name, (i->args->len == args)? "true":"false");


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
    check_type_compatibility(s->type, check_expr(expr));
}

static void check_assign(node* assign){
    char* id = assign->children->value[0];
    symbol* s = hashmap_get(symbol_table, id);
    if(!s){
        printf(RED"[ERROR] "RESET"Symbol '%s' does not exist \n", assign->children->value[0]);
        has_errors = true;
        return;
    }
    node* expr = assign->children->value[1];
    check_expr(expr);
}

static void check_func(node* func){
    symbol* s = func->children->value[0];
    check_symbol(s);

    for(uint8_t i = 1; i < func->children->len; ++i){
        node* n = func->children->value[i];
        if(n){
            check_statement(n);
        }
    }

    // check for "return" statements in case the function has a return value
}

static void check_lambda(node* lambda){
    symbol* s = lambda->children->value[0];
    check_symbol(s);
    node* expr = lambda->children->value[1];
    check_type_compatibility(s->type, check_expr(expr));
}

static void check_statement(node* n){
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
    case N_ASSIGN:
        check_assign(n);
        break;
    case N_MODULE:
        for(uint16_t i = 1; i < n->children->len; ++i){
            node* child = n->children->value[i];
            if(child){
                check_statement(child);
            }
            
        }
    default:
        break;
    }
}

bool check_parse_tree(parse_tree* tree){
    if(!tree){
        return 0;
    }
    symbol_table = tree->symbol_table;
    type_table = tree->type_table;
    
    for(uint32_t i = 0; i < tree->ast->children->len; ++i){
        node* n = tree->ast->children->value[i];
        check_statement(n);
    }
    return has_errors;
}
