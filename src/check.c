#include "quarzum.h"
static bool has_errors = false;
static hashmap* symbol_table;
static hashmap* type_table;
static void check_statement(node* n);


// Checks if the symbol has been added to the symbol table and
// its mangled name is the same as the symbol passed.
// If the symbol has not been added yet, adds it to the symbol table.
static void check_symbol(symbol* s){
    symbol* dup = hashmap_get(symbol_table, s->name);
    if(dup && strcmp(dup->mangled_name, s->mangled_name)==0){
        printf(RED"[ERROR] "RESET"Symbol '%s' already exists \n", s->name);
        has_errors = true;
        return;
    }
    //printf("%s -> %s \n", s->name, s->mangled_name);
    hashmap_add(symbol_table, s->name, s);
}

static void check_module(node* module_node){
    
}


static symbol* try_get_symbol(char* name){
    symbol* s = hashmap_get(symbol_table, name);
    if(!s){
        printf(RED"[ERROR] "RESET"Symbol '%s' does not exist \n", name);
        has_errors = true;
    }
    return s;
}

static void check_type_compatibility(type* a, type* b){
    if(!b){has_errors = true; return;}
    if(!compare_types(a, b)){
        printf(RED"[ERROR] "RESET"Expected '%s'; received '%s'\n", a->name, b->name);
        has_errors = true;
    }
}

// Returns the type that is storing the pointer.
// Example: char* returns char, int** returns int*
type* unmask_pointer(char* ptr_name){
    ptr_name[strlen(ptr_name) - 1] = 0;
    if(ptr_name[strlen(ptr_name) - 1] == '*'){
        return &(type){TY_PTR, ptr_name, 8, 8, POINTER_FLAG};
    }
    return hashmap_get(type_table, ptr_name);
}

static type* check_expr(node* expr){
    // check the type rules 

    switch (expr->type)
    {
    case N_CAST:
        // TODO: check if the conversion is possible
        return expr->children->value[0];
    case N_INDEX_EXPR:
        node* array = n_get(expr, 0);
        node* index = n_get(expr, 1);
        type* array_type = check_expr(array);
        if((array_type->flags & POINTER_FLAG) == 0){
            printf(RED"[ERROR] "RESET"Symbol '%s' is not an array or a pointer \n", array_type->name);
            return ty_var;
        }
        return unmask_pointer(array_type->name);
    case N_TERNARY_EXPR:

        node* if_true = expr->children->value[1];
        node* if_false = expr->children->value[2];
        symbol* if_true_symbol = if_true->children->value[1];
        symbol* if_false_symbol = if_false->children->value[1];

        if(compare_types(if_true_symbol->type, if_false_symbol->type)){
            return if_true_symbol->type;
        }
        return ty_var;

    case N_IDENTIFIER:
        symbol* sym = try_get_symbol(expr->children->value[0]);
        if(!sym){
            break;
        }
        return sym->type;
    case N_BINARY_EXPR:
        type* left = check_expr(expr->children->value[0]);
        type* right = check_expr(expr->children->value[1]);
        return merge_types(left, right, expr->children->value[2]);
    case N_LITERAL:
        return expr->children->value[1];

    // TODO: convert child expression into an extension of parent
    // x.to_string() =>  [x, x::to_string]
    case N_MEMBER_EXPR:
        node* parent = n_get(expr, 0);
        node* child = n_get(expr, 1);
        
        // Gets the parent type and checks if the combined 
        // name matches (int t; t.to_string() => int::to_string)
        type* base = check_expr(parent);
            
       

        return check_expr(child);
    case N_CALL_EXPR:
        uint8_t args = expr->children->len - 1;
        symbol* s = try_get_symbol(expr->children->value[0]);
        if(!s){
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
            symbol* arg = i->args->value[n - 1];
            if(!compare_types(t,arg->type)){
                printf(RED"[ERROR] "RESET"Types '%s' and '%s' do not match \n", t->name, arg->type->name);
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
    case N_NULL_EXPR: return ty_null;
    default: return ty_var;
       
    }

    
}

// Checks if the variable symbol is not defined yet and
// if its type and the expression type can match.
static void check_var(node* var){
    symbol* s = n_get(var, 0);
    check_symbol(s);
    node* expr = n_get(var, 1);
    check_type_compatibility(s->type, check_expr(expr));
}

static void check_lambda(node* lambda){
    symbol* s = lambda->children->value[0];
    check_symbol(s);
    node* expr = lambda->children->value[1];
    check_type_compatibility(s->type, check_expr(expr));
}

static void check_function_statement(node* n, type* t){
    switch (n->type)
    {
    case N_VAR:
        check_var(n);
        break;
    case N_LAMBDA:
        check_lambda(n);
        break;
    case N_RETURN:
        type* return_type = check_expr(n_get(n, 0));
        if(!compare_types(t, return_type)){
            printf(RED "[ERROR] " RESET "Expected a return type '%s', received '%s'\n", t->name, return_type->name);
            has_errors=true;
        }
        break;
    default:
        check_expr(n);
        break;
    }
}


static void check_func(node* func){
    symbol* s = n_get(func, 0);
    check_symbol(s);

    for(uint8_t i = 1; i < func->children->len; ++i){
        node* n = n_get(func, i);
        if(n){
            check_function_statement(n, s->type);
        }
    }

    // check for "return" statements in case the function has a return value
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
    case N_MODULE:
        check_module(n);
        for(uint16_t i = 1; i < n->children->len; ++i){
            node* child = n->children->value[i];
            if(child){
                check_statement(child);
            }
            
        }
    default:
        check_expr(n);
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
