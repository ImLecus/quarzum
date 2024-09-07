#include "quarzum.h"
static node* parse_statement(lexer* lexer);
static node* parse_decl(lexer* lexer, int scope);

static int filled = 1;
static hashmap* imported_files;
static hashmap* type_map;
static bool has_errors = false;
static string* last_namespace;

/**
 * Initializes static variables one time per program.
 */
static void start_parsing(lexer* lexer){
    if(!imported_files){
        imported_files = init_hashmap(32);
    }
    hashmap_add(imported_files, lexer->file, &(int){1});
    if(!type_map){
        type_map = init_type_map();
    }
    if(!last_namespace){
        last_namespace = init_string(32);
    }
}

static inline void add_namespace(char* namespace){
    if(last_namespace->len > 0){
        string_append(last_namespace, "::");
    }
    string_append(last_namespace, namespace);
}

static void delete_last_namespace(){
    for(uint32_t i = last_namespace->len; i > 0; --i){
        if(last_namespace->value[i] == ':' && last_namespace->value[i-1] == ':'){
            string_pop(last_namespace);
            string_pop(last_namespace);
            break;
        }
        string_pop(last_namespace);
        
    }
}

inline void* n_get(node* n, uint index){
    return vector_get(n->children, index);
}

static inline void free_node(node* n){
    free_vector(n->children);
    free(n);
}

inline void free_parse_tree(parse_tree* tree){
    free_hashmap(tree->symbol_table);
    free_hashmap(tree->type_table);
    free_node(tree->ast);
}

node* init_node(uint32_t children, uint8_t type){
    node* n = malloc(sizeof(node));
    n->children = init_vector(children);
    n->type = type;
    return n;
}

inline void expect(token* t, uint8_t type, char* what){
    if(t->type != type){
        printf(RED "[ERROR]" RESET " (%s) Expected %s at line %d.\n", 
               t->file, what, t->line);
        has_errors = true;
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
static node* parse_return_statement(lexer* lexer){
    node* return_node = init_node(1,N_RETURN);
    read_next(lexer);
    if(lexer->tok->type == T_SEMICOLON){
        vector_push(return_node->children, NULL_EXPR);
        read_next(lexer);
        return return_node;
    }
    node* expr = parse_expr(lexer);
    expect(lexer->tok, T_SEMICOLON, "semicolon");
    read_next(lexer);
    vector_push(return_node->children, expr);
    return return_node;
}

// Tries to parse an if statement (IF(expr){statement*})
static node* parse_if_statement(lexer* lexer){
    node* if_stmt = init_node(2,N_IF);
    read_next(lexer);
    expect(lexer->tok, T_LEFT_PAR, "'(");
    read_next(lexer);
    node* condition = parse_expr(lexer);
    if(condition){
        vector_push(if_stmt->children, condition);
    }
    expect(lexer->tok, T_RIGHT_PAR, "')");
    read_next(lexer);
    expect(lexer->tok, T_LEFT_CURLY, "'if' body");
    read_next(lexer);
    while(lexer->tok->type != T_RIGHT_CURLY){
        node* stmt = parse_statement(lexer);
        vector_push(if_stmt->children, stmt);
        read_next(lexer);
    }
    // skip '}' ?
    return if_stmt;
}

// Tries to parse a while statement (WHILE(expr){statement*})
static node* parse_while_statement(lexer* lexer){
    node* while_stmt = init_node(2,N_WHILE);
    read_next(lexer);
    expect(lexer->tok, T_LEFT_PAR, "'(");
    read_next(lexer);
    node* condition = parse_expr(lexer);
    if(condition){
        vector_push(while_stmt->children, condition);
    }
    expect(lexer->tok, T_RIGHT_PAR, "')");
    read_next(lexer);
    expect(lexer->tok, T_LEFT_CURLY, "'while' body");
    read_next(lexer);
    while(lexer->tok->type != T_RIGHT_CURLY){
        node* stmt = parse_statement(lexer);
        vector_push(while_stmt->children, stmt);
        read_next(lexer);
    }
    // skip '}' ?
    return while_stmt;
}

// Tries to parse one of the following statements:
// - Return statements
// - Expressions where the first token is an IDENTIFIER
// - If, While, DoWhile, For & Foreach statements
// - Local variables, lambdas or functions
static node* parse_statement(lexer* lexer){
    switch (lexer->tok->type)
    {
    case T_KEYWORD_RETURN: return parse_return_statement(lexer);

    case T_IDENTIFIER:
        if(hashmap_get(type_map,lexer->tok->value)){
            return parse_decl(lexer, S_LOCAL);
        }
        node* result = parse_expr(lexer);
        read_next(lexer);
        return result;
    case T_KEYWORD_IF: return parse_if_statement(lexer);
    case T_KEYWORD_WHILE: return parse_while_statement(lexer);
    
    case T_TYPE:
    case T_SPECIFIER: return parse_decl(lexer, S_LOCAL);
    
    default:
        printf(RED"[ERROR]"RESET" (%s) Invalid statement at line %d\n", lexer->file, lexer->line);
        read_next(lexer);
        return NULL;
    }
   
    return NULL;
}

type* parse_type(lexer* lexer){
    type* t = malloc(sizeof(type));
    type* template = hashmap_get(type_map, lexer->tok->value);
    if(!template){
        printf(RED"[ERROR]"RESET" (%s) Undefined type '%s' at line %d\n", lexer->file, lexer->tok->value, lexer->line);
        template = ty_null;    
    }
    memcpy(t, template, sizeof(type));
    read_next(lexer);
    while(true){
        if(strcmp(lexer->tok->value, "*") == 0){
            convert_to_pointer(t);
            read_next(lexer);
            continue;
        }
        if(strcmp(lexer->tok->value, "[") ==  0){
            read_next(lexer);
            token* tok = lexer->tok;
            expect(tok, T_INT_LITERAL, "integer literal");
            read_next(lexer);
            expect(lexer->tok, T_RIGHT_SQUARE, "']");
            // apply array and size to the type
            read_next(lexer);
            continue;
        }
        return t;
    }   
}


static node* parse_class_statement(lexer* lexer) {
    expect(lexer->tok, T_ACCESS, "attribute or method");
    
    read_next(lexer);
    node* class_stmt_node = parse_decl(lexer, S_CLASS);
    return class_stmt_node;
}

static node* parse_class(lexer* lexer){
    node* class_node = init_node(3, N_CLASS);
    read_next(lexer);
    expect(lexer->tok, T_IDENTIFIER, "identifier");
    
    read_next(lexer);

    if(lexer->tok->type == T_ARROW){
        read_next(lexer);
        type* extends = parse_type(lexer);
        vector_push(class_node->children, extends);
    }

    expect(lexer->tok, T_LEFT_CURLY, "'{'");
    read_next(lexer);
    while(lexer->tok->type != T_RIGHT_CURLY){    
        vector_push(class_node->children, parse_class_statement(lexer) );
    }
    expect(lexer->tok, T_RIGHT_CURLY, "'}'");
    read_next(lexer);

    return class_node;
}

/**
 * Parses the sequence SPECIFIER* TYPE ID
 */
static symbol* parse_symbol(lexer* lexer, int scope){
    symbol* s = malloc(sizeof(symbol));
    int flags = 0;
    s->scope = scope;
    while(lexer->tok->type == T_SPECIFIER){
        switch (lexer->tok->value[0])
        {
        case 'c':
            if(flags & CONST_FLAG){
                printf(ORANGE BOLD"[WARNING]"RESET" (%s) Duplicated 'const' specifier at line %i\n",lexer->file,lexer->line);
            }
            flags |= CONST_FLAG;
            break;
        case 'f':
            if(flags & FOREIGN_FLAG){
                printf(ORANGE BOLD"[WARNING]"RESET" (%s) Duplicated 'foreign' specifier at line %i\n",lexer->file,lexer->line);
            }
            flags |= FOREIGN_FLAG;
            break;
        default:
            // err
            break;
        }
        read_next(lexer);
    }
    s->type = parse_type(lexer);
    s->type->flags |= flags;

    // Parse type extension symbols
    type* extends = hashmap_get(type_map, lexer->tok->value);
    if(extends){
        char* base_type = extends->name;
        s->scope = S_EXTEND;
        read_next(lexer);
        expect(lexer->tok, T_TYPE_EXTENSION_OP, "'::'");
        add_namespace(base_type);
        read_next(lexer);
        if(lexer->tok->type == T_KEYWORD_OPERATOR){
            read_next(lexer);
            if(!is_operator(lexer->tok->type)){
                expect(lexer->tok, T_COMPARATION_OP, "operator");
            }
            s->name = lexer->tok->value;
            s->mangled_name = mangle_name(s, last_namespace);    
            return s;
        }
    }
    expect(lexer->tok, T_IDENTIFIER, "identifier");
    s->name = lexer->tok->value;
    s->mangled_name = mangle_name(s, last_namespace);    
    return s;
}

// 
// TOP-LEVEL-STATEMENT PARSING FUNCTIONS
//

// Tries to parse a variable statement (symbol = expr;) | (symbol;)
static node* parse_var(lexer* l, symbol* s, bool has_value){
    node* var_node = init_node(2, N_VAR);
    vector_push(var_node->children, s);
    if(has_value){
        expect(l->tok, T_EQUAL, "'='");
        read_next(l);
        node* expr = parse_expr(l);
        expect(l->tok, T_SEMICOLON, "semicolon");
        vector_push(var_node->children, expr);
        read_next(l);
        return var_node;
    }
    expect(l->tok, T_SEMICOLON, "semicolon or declaration");
    read_next(l);
    vector_push(var_node->children, NULL_EXPR);
    return var_node;
}

// Tries to parse a lambda function (symbol(args) => expr;) | (symbol(args) => {expr};)
static node* parse_lambda(lexer* lexer,symbol* s){
    s->type->flags |= LAMBDA_FLAG;
    s->type->flags |= FUNCTION_FLAG;
    bool inside_curly_brackets = lexer->tok->type == T_LEFT_CURLY;
    if(inside_curly_brackets){
        read_next(lexer);
    }
    node* expr = parse_expr(lexer);
    if(inside_curly_brackets){
        expect(lexer->tok, T_RIGHT_CURLY, "'}'");
        read_next(lexer);
    }
    expect(lexer->tok, T_SEMICOLON, "semicolon");
    read_next(lexer);

    node* lambda = init_node(2, N_LAMBDA);
    vector_push(lambda->children, s);


    vector_push(lambda->children, expr);
    return lambda;
}

static node* parse_enum(lexer* l, symbol* s){
    node* enum_node = init_node(2, N_ENUM);
    vector_push(enum_node->children, s);
    expect(l->tok, T_LEFT_CURLY, "'{'");
    read_next(l);
    while(l->tok->type != T_RIGHT_CURLY){
        // symbol* child = parse_symbol(l, S_PARAMETER);
        // read_next(l);
        // s->type->align = child->type->align > s->type->align ? child->type->align : s->type->align;
        // ++children;
        // expect(l->tok, T_SEMICOLON, "semicolon");
        // vector_push(struct_node->children, child);
        read_next(l);
    }
    read_next(l);
    return enum_node;
}

/**
 * Parses the sequence "struct" ID "{" ([symbol]";")* "}"
 */
static void parse_struct(lexer* l){
    read_next(l);
    expect(l->tok, T_IDENTIFIER, "identifier");
    char* id = l->tok->value;
    read_next(l);
    type* struct_type = malloc(sizeof(type));
    expect(l->tok, T_LEFT_CURLY, "'{'");
    read_next(l);
    uint32_t children = 0;
    while(l->tok->type != T_RIGHT_CURLY){
        symbol* child = parse_symbol(l, S_PARAMETER);
        read_next(l);
        struct_type->align = max(child->type->align, struct_type->align);
        ++children;
        expect(l->tok, T_SEMICOLON, "semicolon");
        read_next(l);
    }
    struct_type->size = struct_type->align * children;
    struct_type->flags = STRUCT_FLAG;
    struct_type->name = id;
    struct_type->type = TY_STRUCT;

    type* duplicated = hashmap_get(type_map, id);
    if(duplicated){
        printf(RED"[ERROR]"RESET" (%s) Type '%s' was already defined\n",l->file,id);
        has_errors = true;
        read_next(l);
        return;
    }
    hashmap_add(type_map, id, struct_type);
    read_next(l);
}

static node* parse_class_special_methods(lexer* lexer){

    return NULL;
}   

static function_info* parse_function_args(lexer* lexer){
    function_info* info = init_function_info();
    read_next(lexer);
    bool optional_args = false;
    while(lexer->tok->type != T_RIGHT_PAR){
        symbol* arg = parse_symbol(lexer, S_PARAMETER);
        vector_push(info->args, arg);
        read_next(lexer);
        if(!optional_args){
            if(lexer->tok->type == T_EQUAL){
                optional_args = true;
                read_next(lexer);
                vector_push(info->optional_values, parse_expr(lexer)); 
            }
            else {
                ++info->min_args;    
            }
        }
        else {
            expect(lexer->tok, T_EQUAL, "initializer");
            read_next(lexer);
            vector_push(info->optional_values, parse_expr(lexer));
        }
        if(lexer->tok->type == T_COMMA){
            read_next(lexer);
            continue;
        } 
        expect(lexer->tok, T_RIGHT_PAR, "')'");
    }
    return info;
}

static node* parse_decl(lexer* lexer, int scope){
    if(scope == S_CLASS && 
    (lexer->tok->type == T_KEYWORD_CONSTRUCTOR ||
    lexer->tok->type == T_KEYWORD_DESTRUCTOR)){
        return parse_class_special_methods(lexer);
    }
    symbol* s = parse_symbol(lexer, scope);
    read_next(lexer);
    switch (lexer->tok->type)
    {
    case T_EQUAL:
        return parse_var(lexer, s, true);
    
    case T_SEMICOLON:
        return parse_var(lexer, s, false);

    case T_LEFT_PAR:
        s->mangled_name = mangle_name(s, last_namespace);
        add_namespace(s->name);
        s->type->flags |= FUNCTION_FLAG;
        s->info = parse_function_args(lexer);

        read_next(lexer);
        if(lexer->tok->type == T_ARROW){
            read_next(lexer);
            return parse_lambda(lexer, s);
        }

        node* func_decl_node = init_node(2,N_FUNCTION);
        vector_push(func_decl_node->children, s);
        
        if(lexer->tok->type == T_LEFT_CURLY){
            read_next(lexer);
            while(lexer->tok->type != T_RIGHT_CURLY){
                node* stmt = parse_statement(lexer);
                if(!stmt){
                    break;
                }
                vector_push(func_decl_node->children, stmt);
            }
            read_next(lexer);
        }
        else{
            expect(lexer->tok, T_SEMICOLON, "semicolon");
            read_next(lexer);
        }
        if(s->scope == S_EXTEND) delete_last_namespace();
        delete_last_namespace();
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
static node* parse_global(lexer* lexer){
    switch (lexer->tok->type)
    {
    case T_KEYWORD_CLASS: return parse_class(lexer);
    case T_KEYWORD_STRUCT: parse_struct(lexer); break;
    case T_SPECIFIER:
    case T_TYPE:
    case T_IDENTIFIER: return parse_decl(lexer, S_GLOBAL);

    default:
        printf(RED"[ERROR] "RESET"(%s) Unexpected token '%s' at line %d\n", lexer->file, lexer->tok->value, lexer->line);
        has_errors = true;
        read_next(lexer);
        break;
    }
    return NULL;
}

//
// OTHER TOP-LEVEL STATEMENTS
// 

// Tries to parse a module (MODULE ID { (module | global)* })
static node* parse_module(lexer* lexer){
    node* module_node = init_node(2, N_MODULE);
    read_next(lexer);
    expect(lexer->tok, T_IDENTIFIER, "identifier");
    char* id = lexer->tok->value;
    symbol* s = &(symbol){};
    s->name = id;
    s->mangled_name = mangle_namespace(id, last_namespace);
    
    vector_push(module_node->children, s);
    read_next(lexer);
    
    expect(lexer->tok, T_LEFT_CURLY, "'{'");
    read_next(lexer);
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
    read_next(lexer);

    return module_node;
}

// Tries to parse an import (IMPORT STRING) and merges the target AST
// with the main one.
static void parse_import(lexer* lexer, node* ast){
    read_next(lexer);
    expect(lexer->tok, T_STRING_LITERAL, "import path");
    char* path = resolve_path(delete_quotes(lexer->tok->value));
    read_next(lexer);
    if(hashmap_get(imported_files, path) != NULL){
        return;
    }
    parse_tree* imported_file = parse(path);
    if(!imported_file){
        return;
    }
    for(uint32_t i = 0; i < imported_file->ast->children->len; ++i){
        vector_push(ast->children, imported_file->ast->children->value[i]);
    }
    free_node(imported_file->ast);
    free(imported_file);
}

// Tries to parse a type definition (TYPEDEF ID = type)
// Internally, creates an entry on the type table pointing
// to the type.
static void parse_typedef(lexer* lexer){
    read_next(lexer);
    char* id = lexer->tok->value;
    expect(lexer->tok, T_IDENTIFIER, "type name");
    read_next(lexer);
    expect(lexer->tok, T_EQUAL, "'='");
    read_next(lexer);
    type* target = parse_type(lexer);
    hashmap_add(type_map, id, target);
}

// Parses a list of tokens and returns a ParseTree containing 
// the AST, the type table and the symbol table.
parse_tree* parse(char* file){
    string* input = read_file(file);
    if(!input){return NULL;}
    lexer* lexer = init_lexer(file,input->value);
    if(!lexer){return NULL;}
    start_parsing(lexer);

    parse_tree* result = malloc(sizeof(parse_tree));
    result->has_errors = false;
    read_next(lexer);
    result->ast = init_node(16, N_ROOT);
    while (lexer->tok->type != T_EOF)
    {
        switch(lexer->tok->type){
            case T_KEYWORD_IMPORT:
                parse_import(lexer, result->ast);
                break;
            case T_KEYWORD_MODULE:
                vector_push(result->ast->children, parse_module(lexer));  
                break;
            case T_KEYWORD_TYPEDEF:
                parse_typedef(lexer);
                break;
            default:
                vector_push(result->ast->children, parse_global(lexer));  
                break;
        } 
    }
    result->has_errors = has_errors;
    result->symbol_table = init_hashmap(256);
    result->type_table = type_map;
    free(lexer);
    return result;
}