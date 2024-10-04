/**
 * \file            check.c
 * \brief           AST check implementation file.
 * \copyright       2024 Marcos González GPL3
 */
#include "check.h"
static int has_errors = 0;
SymbolTable* symbol_table;
static void check_statement(Node* n);
static const char* prefix = "";


// Checks if the symbol has been added to the symbol table and
// its mangled name is the same as the symbol passed.
// If the symbol has not been added yet, adds it to the symbol table.
static void check_symbol(Symbol* s){
    Symbol* dup = find_symbol(symbol_table->last_table, s->name);
    if(dup != NULL){
        duplicated_symbol_err(s->defined_pos, s->name);
        has_errors = 1;
        return;
    }
    printf("%s -> %s \n",symbol_table->last_table->name, s->name);
    insert_symbol(symbol_table, s);
    // Functions
    if(s->type->flags & FUNCTION_FLAG){
        function_info* info = s->info;
        for(uint8_t i = 0; i < info->args->len; ++i){
            check_symbol(info->args->value[i]);
        }
    }
}

static void check_module(Node* module_node){
    Symbol* module = n_get(module_node, 0);
    Symbol* dup = find_symbol(symbol_table->last_table, module->name);
    if(dup){
        printf(RED"[ERROR] "RESET"Module '%s' already exists \n", module->name);
        has_errors = 1;
        return;
    }
    //printf("mod: %s \n", module->name);
    insert_symbol(symbol_table, module);
}


static Symbol* try_get_symbol(char* name, const char* prefix){
    //printf("Trying to get symbol %s...\n", mangle_namespace(name, prefix));
    Symbol* s = find_symbol(symbol_table->last_table, mangle_namespace(name, prefix));
    if(s == NULL){
        printf(RED"[ERROR] "RESET"Symbol '%s' does not exist \n", name);
        has_errors = 1;
    }
    return s;
}

static void check_type_compatibility(Type* a, Type* b){
    if(!b){has_errors = 1; return;}
    if(!compare_types(a, b)){
        printf(RED"[ERROR] "RESET"Expected '%s'; received '%s'\n", a->name, b->name);
        has_errors = 1;
    }
}

// Returns the type that is storing the pointer.
// Example: char* returns char, int** returns int*
Type* unmask_pointer(char* ptr_name){
    ptr_name[strlen(ptr_name) - 1] = 0;
    if(ptr_name[strlen(ptr_name) - 1] == '*'){
        return &(Type){TY_PTR, ptr_name, 8, 8, POINTER_FLAG};
    }
    return hashmap_get(type_map, ptr_name);
}

static Type* check_expr(Node* expr){
    switch (expr->type)
    {
    // case N_CAST:
    //     // TODO: check if the conversion is possible
    //     return expr->children->value[0];
    case N_INDEX_EXPR:
        Node* array = n_get(expr, 0);
        Node* index = n_get(expr, 1);
        Type* array_type = check_expr(array);
        if((array_type->flags & POINTER_FLAG) == 0){
            printf(RED"[ERROR] "RESET"Symbol '%s' is not an array or a pointer \n", array_type->name);
            return ty_var;
        }
        return unmask_pointer(array_type->name);
    case N_TERNARY_EXPR:

        Node* if_1 = n_get(expr, 1);
        check_expr(if_1);
        Node* if_false = n_get(expr, 2);
        check_expr(if_false);
        Symbol* if_1_symbol = if_1->children->value[1];
        Symbol* if_false_symbol = if_false->children->value[1];

        if(compare_types(if_1_symbol->type, if_false_symbol->type)){
            return if_1_symbol->type;
        }
        return ty_var;

    case N_IDENTIFIER:
        Symbol* sym = try_get_symbol(expr->children->value[0], prefix);
        if(!sym){
            break;
        }
        return sym->type;
    case N_BINARY_EXPR:
        Type* left = check_expr(expr->children->value[0]);
        Type* right = check_expr(expr->children->value[1]);
        return left;//merge_types(left, right, expr->children->value[2]);
    case N_LITERAL:
        return n_get(expr, 1);

    case N_MEMBER_EXPR:
        Node* parent = n_get(expr, 0);
        Node* child = n_get(expr, 1);

        // Gets the parent type and checks if the combined 
        // name matches (int t; t.to_string() => int::to_string)
        Type* base = check_expr(parent);
        if(base){
            prefix = base->name;
            if(base->type == TY_MODULE){
                Symbol* namespace = n_get(parent, 0);
                prefix = namespace->name;
            }
        }
        
        return check_expr(child);
    case N_CALL_EXPR:
        uint8_t args = expr->children->len - 1;
        Symbol* s = try_get_symbol(expr->children->value[0], prefix);
        if(!s){
            break;
        }
        // printf(s->name);
        function_info* i = s->info;
        if(!i){
            printf(RED"[ERROR] "RESET"Symbol '%s' is not a callable function \n");
            has_errors = 1;
            break;
        }
        if(args > i->args->len || args < i->min_args){
            printf(RED"[ERROR] "RESET"Incorrect number of arguments for call '%s'\n");
            has_errors = 1;
            break;
        }

        for(uint8_t n = 1; n < args + 1; ++n){
            Type* t = check_expr(expr->children->value[n]);
            Symbol* arg = i->args->value[n - 1];
            check_type_compatibility(t, arg->type);
        }
        // If some args are default, check the type of the default args
        if(args < i->args->len){
            for(uint8_t n = i->min_args; n < i->args->len; ++n){
                Type* t = check_expr(i->optional_values->value[n - i->min_args]);
                Symbol* arg = i->args->value[n];
                check_type_compatibility(arg->type, t);
            }
        }


        if(s->type->flags & LAMBDA_FLAG){
            // replace the call_expression to the lambda expression, changing the parameters inside the 
            // expression for the argument expressions.
        }
        
        return s->type;
    case N_NULL_EXPR: return ty_null;
    default: return ty_var;
       
    }
    return ty_var;
    
}

// Checks if the variable symbol is not defined yet and
// if its type and the expression type can match.
static void check_var(Node* var){
    Symbol* s = n_get(var, 0);
    check_symbol(s);
    Node* expr = n_get(var, 1);
    check_type_compatibility(s->type, check_expr(expr));
    prefix = "";
}

static void check_lambda(Node* lambda){
    Symbol* s = n_get(lambda, 0);
    check_symbol(s);
    Node* expr = n_get(lambda, 1);
    check_type_compatibility(s->type, check_expr(expr));
    prefix = "";
}

static void check_function_statement(Node* n, Type* t){
    switch (n->type)
    {
    case N_VAR:
        check_var(n);
        break;
    case N_LAMBDA:
        check_lambda(n);
        break;
    case N_RETURN:
        Type* return_type = check_expr(n_get(n, 0));
        if(!compare_types(t, return_type)){
            printf(RED "[ERROR] " RESET "Expected a return type '%s', received '%s'\n", t->name, return_type->name);
            has_errors=1;
        }
        break;
    default:
        check_expr(n);
        break;
    }
}


static void check_func(Node* func){
    Symbol* s = n_get(func, 0);
    check_symbol(s);

    for(uint8_t i = 1; i < func->children->len; ++i){
        Node* n = n_get(func, i);
        if(n){
            check_function_statement(n, s->type);
        }
    }

    // check for "return" statements in case the function has a return value
}

static void check_statement(Node* n){
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
        add_scope(symbol_table, "console" , S_LOCAL);
        check_module(n);
        for(uint16_t i = 1; i < n->children->len; ++i){
            Node* child = n->children->value[i];
            if(child != NULL) check_statement(child);    
        }
    default:
        check_expr(n);
        break;
    }
}

void check_ast(Node* ast){
    if(ast == NULL) return;
    init_symbol_table(symbol_table);
    for(uint32_t i = 0; i < ast->children->len; ++i){
        Node* n = ast->children->value[i];
        check_statement(n);
    }
}
