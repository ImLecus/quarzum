/**
 * \file            parse.c
 * \brief           Quarzum parser implementation file.
 * \copyright       2024 Marcos González GPL3
 */
#include "parse.h"

static node_t* parse_statement(lexer_t* lexer);
static node_t* parse_decl(lexer_t* lexer, scope_t scope);

static hashmap_t* imported_files;
static string_t* last_namespace;

/**
 * Initializes static variables one time per program.
 */
static void start_parsing(lexer_t* lexer){
    if(imported_files == NULL) imported_files = init_hashmap(32);
    hashmap_add(imported_files, lexer->position.file, &(int){1});
    if(type_map == NULL) type_map = init_type_map();
    if(last_namespace == NULL) last_namespace = init_string(32);
}

static inline void add_namespace(char* namespace){
    if(last_namespace->length > 0){
        string_append(last_namespace, "::");
    }
    string_append(last_namespace, namespace);
}

static void delete_last_namespace(){
    for(uint32_t i = last_namespace->length; i > 0; --i){
        if(last_namespace->content[i] == ':' && last_namespace->content[i-1] == ':'){
            string_pop(last_namespace);
            string_pop(last_namespace);
            return;
        }
        string_pop(last_namespace);
        
    }
}

inline void* n_get(node_t* n, uint32_t index){
    return vector_get(n->children, index);
}

static inline void free_node(node_t* n){
    free_vector(n->children);
    free(n);
}

node_t* init_node(uint32_t children, node_type_t type, pos_t pos){
    node_t* n = malloc(sizeof(node_t));
    n->children = init_vector(children);
    n->type = type;
    n->pos = pos;
    return n;
}

inline void expect(token_t* t, node_type_t type, char* what){
    if(t->type != type){
        expected_token_err(t->position, what);
    }
}

static function_info* init_function_info(){
    function_info* f = malloc(sizeof(function_info));
    f->align = 0;
    f->args = init_vector(1);
    f->min_args = 0;
    f->optional_values = init_vector(1);
    
    return f;
}

//
// STATEMENTS
//

// Tries to parse a return statement (RETURN EXPR;) | (RETURN;)
static node_t* parse_return_statement(lexer_t* lexer){
    node_t* return_node = init_node(1,N_RETURN, lexer->position);
    next(lexer);
    if(lexer->tok->type == T_SEMICOLON){
        vector_push(return_node->children, NULL_EXPR(lexer->position));
        next(lexer);
        return return_node;
    }
    node_t* expr = parse_expr(lexer);
    expect(lexer->tok, T_SEMICOLON, "semicolon");
    next(lexer);
    vector_push(return_node->children, expr);
    return return_node;
}

// Tries to parse an if statement (IF(expr){statement*})
static node_t* parse_if_statement(lexer_t* lexer){
    node_t* if_stmt = init_node(2,N_IF, lexer->position);
    next(lexer);
    expect(lexer->tok, T_LEFT_PAR, "'(");
    next(lexer);
    node_t* condition = parse_expr(lexer);
    if(condition != NULL){
        vector_push(if_stmt->children, condition);
    }
    expect(lexer->tok, T_RIGHT_PAR, "')");
    next(lexer);
    expect(lexer->tok, T_LEFT_CURLY, "'if' body");
    next(lexer);
    while(lexer->tok->type != T_RIGHT_CURLY){
        node_t* stmt = parse_statement(lexer);
        vector_push(if_stmt->children, stmt);
        next(lexer);
    }
    // skip '}' ?
    return if_stmt;
}

// Tries to parse a while statement (WHILE(expr){statement*})
static node_t* parse_while_statement(lexer_t* lexer){
    node_t* while_stmt = init_node(2,N_WHILE, lexer->position);
    next(lexer);
    expect(lexer->tok, T_LEFT_PAR, "'(");
    next(lexer);
    node_t* condition = parse_expr(lexer);
    if(condition != NULL){
        vector_push(while_stmt->children, condition);
    }
    expect(lexer->tok, T_RIGHT_PAR, "')");
    next(lexer);
    expect(lexer->tok, T_LEFT_CURLY, "'while' body");
    next(lexer);
    while(lexer->tok->type != T_RIGHT_CURLY){
        node_t* stmt = parse_statement(lexer);
        vector_push(while_stmt->children, stmt);
        next(lexer);
    }
    // skip '}' ?
    return while_stmt;
}

// Tries to parse one of the following statements:
// - Return statements
// - Expressions where the first token is an IDENTIFIER
// - If, While, DoWhile, For & Foreach statements
// - Local variables, lambdas or functions
static node_t* parse_statement(lexer_t* lexer){
    switch (lexer->tok->type)
    {
    case T_KEYWORD_RETURN: return parse_return_statement(lexer);

    case T_IDENTIFIER:
        if(hashmap_get(type_map,lexer->tok->value)){
            return parse_decl(lexer, S_LOCAL);
        }
        node_t* result = parse_expr(lexer);
        next(lexer);
        return result;
    case T_KEYWORD_IF: return parse_if_statement(lexer);
    case T_KEYWORD_WHILE: return parse_while_statement(lexer);
    
    case T_SPECIFIER: return parse_decl(lexer, S_LOCAL);
    
    default:
        throw_error(lexer->position, "Invalid statement");
        next(lexer);
        return NULL;
    }
   
    return NULL;
}

type* parse_type(lexer_t* lexer){
    type* t = malloc(sizeof(type));
    type* template = hashmap_get(type_map, lexer->tok->value);
    if(template == NULL){
        undefined_type_err(lexer->position, lexer->tok->value);
        template = ty_null;    
    }
    *t = *template;
    next(lexer);
    while(1){
        if(strcmp(lexer->tok->value, "*") == 0){
            convert_to_pointer(t);
            next(lexer);
            continue;
        }
        if(strcmp(lexer->tok->value, "[") ==  0){
            next(lexer);
            expect(lexer->tok, T_INT_LITERAL, "integer literal");
            next(lexer);
            expect(lexer->tok, T_RIGHT_SQUARE, "']");
            // apply array and size to the type
            next(lexer);
            continue;
        }
        return t;
    }   
}


static node_t* parse_class_statement(lexer_t* lexer) {
    expect(lexer->tok, T_ACCESS, "attribute or method");
    
    next(lexer);
    node_t* class_stmt_node = parse_decl(lexer, S_CLASS);
    return class_stmt_node;
}

static node_t* parse_class(lexer_t* lexer){
    node_t* class_node = init_node(3, N_CLASS, lexer->position);
    next(lexer);
    expect(lexer->tok, T_IDENTIFIER, "identifier");
    
    next(lexer);

    if(lexer->tok->type == T_ARROW){
        next(lexer);
        type* extends = parse_type(lexer);
        vector_push(class_node->children, extends);
    }

    expect(lexer->tok, T_LEFT_CURLY, "'{'");
    next(lexer);
    while(lexer->tok->type != T_RIGHT_CURLY){    
        vector_push(class_node->children, parse_class_statement(lexer) );
    }
    expect(lexer->tok, T_RIGHT_CURLY, "'}'");
    next(lexer);

    return class_node;
}

/**
 * Parses the sequence SPECIFIER* TYPE ID
 */
static symbol* parse_symbol(lexer_t* lexer, scope_t scope){
    symbol* s = malloc(sizeof(symbol));
    int flags = 0;
    s->scope = scope;
    while(lexer->tok->type == T_SPECIFIER){
        switch (lexer->tok->value[0])
        {
        case 'c':
            if(flags & CONST_FLAG){
                duplicated_flag_warning(lexer->position, "const");
            }
            flags |= CONST_FLAG;
            break;
        case 'f':
            if(flags & FOREIGN_FLAG){
                duplicated_flag_warning(lexer->position, "foreign");            
            }
            flags |= FOREIGN_FLAG;
            break;
        default:
            // err
            break;
        }
        next(lexer);
    }
    s->type = parse_type(lexer);
    s->type->flags |= flags;

    // Parse type extension symbols
    type* extends = hashmap_get(type_map, lexer->tok->value);
    if(extends != NULL){
        char* base_type = extends->name;
        s->scope = S_EXTEND;
        next(lexer);
        expect(lexer->tok, T_TYPE_EXTENSION_OP, "'::'");
        add_namespace(base_type);
        next(lexer);
        if(lexer->tok->type == T_KEYWORD_OPERATOR){
            next(lexer);
            if(!(IS_OPERATOR(lexer->tok->type))) {
                expect(lexer->tok, T_COMPARATION_OP, "operator");
            }
            s->name = lexer->tok->value;
            s->mangled_name = mangle_name(s);    
            return s;
        }
    }
    expect(lexer->tok, T_IDENTIFIER, "identifier");
    s->name = mangle_namespace(lexer->tok->value, last_namespace->content);
    s->mangled_name = mangle_name(s);    
    return s;
}

// 
// TOP-LEVEL-STATEMENT PARSING FUNCTIONS
//

// Tries to parse a variable statement (symbol = expr;) | (symbol;)
static node_t* parse_var(lexer_t* l, symbol* s, int has_value){
    node_t* var_node = init_node(2, N_VAR, l->position);
    s->defined_pos = l->position;
    vector_push(var_node->children, s);
    if(has_value){
        expect(l->tok, T_EQUAL, "'='");
        next(l);
        node_t* expr = parse_expr(l);
        expect(l->tok, T_SEMICOLON, "semicolon");
        vector_push(var_node->children, expr);
        next(l);
        return var_node;
    }
    expect(l->tok, T_SEMICOLON, "semicolon or declaration");
    next(l);
    vector_push(var_node->children, NULL_EXPR(l->position));
    return var_node;
}

// Tries to parse a lambda function (symbol(args) => expr;) | (symbol(args) => {expr};)
static node_t* parse_lambda(lexer_t* lexer,symbol* s){
    s->type->flags |= LAMBDA_FLAG;
    s->type->flags |= FUNCTION_FLAG;
    int inside_curly_brackets = lexer->tok->type == T_LEFT_CURLY;
    if(inside_curly_brackets){
        next(lexer);
    }
    node_t* expr = parse_expr(lexer);
    if(inside_curly_brackets){
        expect(lexer->tok, T_RIGHT_CURLY, "'}'");
        next(lexer);
    }
    expect(lexer->tok, T_SEMICOLON, "semicolon");
    next(lexer);

    node_t* lambda = init_node(2, N_LAMBDA, lexer->position);
    vector_push(lambda->children, s);


    vector_push(lambda->children, expr);
    return lambda;
}

static node_t* parse_enum(lexer_t* l, symbol* s){
    node_t* enum_node = init_node(2, N_ENUM, l->position);
    vector_push(enum_node->children, s);
    expect(l->tok, T_LEFT_CURLY, "'{'");
    next(l);
    while(l->tok->type != T_RIGHT_CURLY){
        // symbol* child = parse_symbol(l, S_PARAMETER);
        // next(l);
        // s->type->align = child->type->align > s->type->align ? child->type->align : s->type->align;
        // ++children;
        // expect(l->tok, T_SEMICOLON, "semicolon");
        // vector_push(struct_node->children, child);
        next(l);
    }
    next(l);
    return enum_node;
}

/**
 * Parses the sequence "struct" ID "{" ([symbol]";")* "}"
 */
static void parse_struct(lexer_t* l){
    next(l);
    expect(l->tok, T_IDENTIFIER, "identifier");
    char* id = l->tok->value;
    next(l);
    type* struct_type = malloc(sizeof(type));
    expect(l->tok, T_LEFT_CURLY, "'{'");
    next(l);
    uint32_t children = 0;
    while(l->tok->type != T_RIGHT_CURLY){
        symbol* child = parse_symbol(l, S_PARAMETER);
        next(l);
        struct_type->align = max(child->type->align, struct_type->align);
        ++children;
        expect(l->tok, T_SEMICOLON, "semicolon");
        next(l);
    }
    struct_type->size = struct_type->align * children;
    struct_type->flags = STRUCT_FLAG;
    struct_type->name = id;
    struct_type->type = TY_STRUCT;

    type* duplicated = hashmap_get(type_map, id);
    if(duplicated){
        duplicated_type_err(l->position, id);
        next(l);
        return;
    }
    hashmap_add(type_map, id, struct_type);
    next(l);
}

static node_t* parse_class_special_methods(lexer_t* lexer){

    return NULL;
}   

static function_info* parse_function_args(lexer_t* lexer){
    function_info* info = init_function_info();
    next(lexer);
    int optional_args = 0;
    while(lexer->tok->type != T_RIGHT_PAR){
        symbol* arg = parse_symbol(lexer, S_PARAMETER);
        vector_push(info->args, arg);
        next(lexer);
        if(!optional_args){
            if(lexer->tok->type == T_EQUAL){
                optional_args = 1;
                next(lexer);
                vector_push(info->optional_values, parse_expr(lexer)); 
            }
            else {
                ++info->min_args;    
            }
        }
        else {
            expect(lexer->tok, T_EQUAL, "initializer");
            next(lexer);
            vector_push(info->optional_values, parse_expr(lexer));
        }
        if(lexer->tok->type == T_COMMA){
            next(lexer);
            continue;
        } 
        expect(lexer->tok, T_RIGHT_PAR, "')'");
    }
    return info;
}

static node_t* parse_decl(lexer_t* lexer, scope_t scope){
    if(scope == S_CLASS && 
    (lexer->tok->type == T_KEYWORD_CONSTRUCTOR ||
    lexer->tok->type == T_KEYWORD_DESTRUCTOR)){
        return parse_class_special_methods(lexer);
    }
    symbol* s = parse_symbol(lexer, scope);
    next(lexer);
    switch (lexer->tok->type)
    {
    case T_EQUAL:
        return parse_var(lexer, s, 1);
    
    case T_SEMICOLON:
        return parse_var(lexer, s, 0);

    case T_LEFT_PAR:
        s->mangled_name = mangle_name(s);
        add_namespace(s->name);
        
        s->type->flags |= FUNCTION_FLAG;
        s->info = parse_function_args(lexer);
        delete_last_namespace();
        if(s->scope == S_EXTEND) {delete_last_namespace();}
        next(lexer);
        if(lexer->tok->type == T_ARROW){
            next(lexer);
            return parse_lambda(lexer, s);
        }

        node_t* func_decl_node = init_node(2,N_FUNCTION, lexer->position);
        vector_push(func_decl_node->children, s);
        
        if(lexer->tok->type == T_LEFT_CURLY){
            next(lexer);
            while(lexer->tok->type != T_RIGHT_CURLY){
                node_t* stmt = parse_statement(lexer);
                if(stmt == NULL) break;
                vector_push(func_decl_node->children, stmt);
            }
            next(lexer);
        }
        else{
            expect(lexer->tok, T_SEMICOLON, "semicolon");
            next(lexer);
        }
        
        return func_decl_node;
    default:
        return NULL;
    }
}

// Tries to parse one of this top-level statements:
// - Variable declaration
// - Lambda function declaration
// - Function declaration
// - Classes
// - Structs
// - Enums
static node_t* parse_global(lexer_t* lexer){
    switch (lexer->tok->type)
    {
    case T_KEYWORD_CLASS: return parse_class(lexer);
    case T_KEYWORD_STRUCT: parse_struct(lexer); break;
    case T_SPECIFIER:
    case T_IDENTIFIER: return parse_decl(lexer, S_GLOBAL);

    default:
        unexpected_token_err(lexer->position, lexer->tok->value);        
        next(lexer);
        break;
    }
    return NULL;
}

//
// OTHER TOP-LEVEL STATEMENTS
// 

// Tries to parse a module (MODULE ID { (module | global)* })
static node_t* parse_module(lexer_t* lexer){
    node_t* module_node = init_node(2, N_MODULE, lexer->position);
    next(lexer);
    expect(lexer->tok, T_IDENTIFIER, "identifier");
    char* id = lexer->tok->value;

    symbol* s = malloc(sizeof(symbol));
    s->name = mangle_namespace(id, last_namespace->content);
    s->type = ty_module;
    s->mangled_name = NULL;
    
    vector_push(module_node->children, s);
    next(lexer);
    
    expect(lexer->tok, T_LEFT_CURLY, "'{'");
    next(lexer);
    add_namespace(id);
    while(lexer->tok->type != T_RIGHT_CURLY){
        if(lexer->tok->type == T_KEYWORD_MODULE){
            vector_push(module_node->children, parse_module(lexer)); 
            continue;
        }
        vector_push(module_node->children, parse_global(lexer));     
    }
    delete_last_namespace();
    expect(lexer->tok, T_RIGHT_CURLY, "'}'");
    next(lexer);

    return module_node;
}

// Tries to parse an import (IMPORT STRING) and merges the target AST
// with the main one.
static void parse_import(lexer_t* lexer, node_t* ast){
    next(lexer);
    expect(lexer->tok, T_STRING_LITERAL, "import path");
    char* path = resolve_path(delete_quotes(lexer->tok->value));
    next(lexer);
    if(hashmap_get(imported_files, path) != NULL) return;
    node_t* imported_file = parse(path);
    if(imported_file == NULL) return;
    for(uint32_t i = 0; i < imported_file->children->len; ++i){
        vector_push(ast->children, imported_file->children->value[i]);
    }
    free_node(imported_file);
}

// Tries to parse a type definition (TYPEDEF ID = type)
// Internally, creates an entry on the type table pointing
// to the type.
static void parse_typedef(lexer_t* lexer){
    next(lexer);
    char* id = lexer->tok->value;
    expect(lexer->tok, T_IDENTIFIER, "type name");
    next(lexer);
    expect(lexer->tok, T_EQUAL, "'='");
    next(lexer);
    type* target = parse_type(lexer);
    type* def = malloc(sizeof(type*));
    *def = *target;
    def->name = id; 
    hashmap_add(type_map, id, def);
}

// Parses a list of tokens and returns a ParseTree containing 
// the AST, the type table and the symbol table.
node_t* parse(char* file){
    string_t* input = read_file(file);
    if(!input) return NULL;
    lexer_t lexer = *init_lexer(file, input->content);
    if(!&lexer) return NULL;
    start_parsing(&lexer);

    next(&lexer);
    node_t* ast = init_node(16, N_ROOT, lexer.position);
    while (lexer.tok->type != T_EOF)
    {
        switch(lexer.tok->type){
            case T_KEYWORD_IMPORT:
                parse_import(&lexer, ast);
                break;
            case T_KEYWORD_MODULE:
                vector_push(ast->children, parse_module(&lexer));  
                break;
            case T_KEYWORD_TYPEDEF:
                parse_typedef(&lexer);
                break;
            default:
                vector_push(ast->children, parse_global(&lexer));  
                break;
        } 
    }
    return ast;
}