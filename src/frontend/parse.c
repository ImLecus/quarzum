/**
 * \file            parse.c
 * \brief           Quarzum parser implementation file.
 * \copyright       2024 Marcos González GPL3
 */
#include "parse.h"

static void start_parsing(const Lexer* const lexer){
    if(imported_files == NULL) imported_files = init_hashmap(32);
    hashmap_add(imported_files, lexer->position.file, &(int){1});
    if(type_map == NULL) type_map = init_type_map();
    if(last_namespace == NULL) last_namespace = init_string(32);
}

static inline void add_namespace(const char* const namespace){
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

inline void* const n_get(Node* const n, uint32_t index){
    return vector_get(n->children, index);
}

static inline void free_node(Node* const n){
    free_vector(n->children);
    free(n);
}

Node* const init_node(const uint32_t children, const NodeType type, Position pos){
    Node* const n = malloc(sizeof(Node));
    n->children = init_vector(children);
    n->type = type;
    n->pos = pos;
    return n;
}

inline void expect(Lexer* lexer, const TokenType type, const char* what){
    if(lexer->tok->type != type){
        expected_token_err(lexer->position, what);
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
static Node* const parse_return_statement(Lexer* const lexer){
    Node* return_node = init_node(1,N_RETURN, lexer->position);
    next(lexer);
    if(lexer->tok->type == TK_SEMICOLON){
        vector_push(return_node->children, NULL_EXPR(lexer->position));
        next(lexer);
        return return_node;
    }
    Node* expr = parse_expr(lexer);
    expect(lexer, TK_SEMICOLON, "semicolon");
    next(lexer);
    vector_push(return_node->children, expr);
    return return_node;
}

// Tries to parse an if statement (IF(expr){statement*})
static Node* const parse_if_statement(Lexer* const lexer){
    Node* if_stmt = init_node(2,N_IF, lexer->position);
    next(lexer);
    expect(lexer, TK_LEFT_PAR, "'(");
    next(lexer);
    Node* condition = parse_expr(lexer);
    if(condition != NULL){
        vector_push(if_stmt->children, condition);
    }
    expect(lexer, TK_RIGHT_PAR, "')");
    next(lexer);
    expect(lexer, TK_LEFT_CURLY, "'if' body");
    next(lexer);
    while(lexer->tok->type != TK_RIGHT_CURLY){
        Node* stmt = parse_statement(lexer);
        vector_push(if_stmt->children, stmt);
        next(lexer);
    }
    // skip '}' ?
    return if_stmt;
}

// Tries to parse a while statement (WHILE(expr){statement*})
static Node* const parse_while_statement(Lexer* const lexer){
    Node* while_stmt = init_node(2,N_WHILE, lexer->position);
    next(lexer);
    expect(lexer, TK_LEFT_PAR, "'(");
    next(lexer);
    Node* condition = parse_expr(lexer);
    if(condition != NULL){
        vector_push(while_stmt->children, condition);
    }
    expect(lexer, TK_RIGHT_PAR, "')");
    next(lexer);
    expect(lexer, TK_LEFT_CURLY, "'while' body");
    next(lexer);
    while(lexer->tok->type != TK_RIGHT_CURLY){
        Node* stmt = parse_statement(lexer);
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
static Node* parse_statement(Lexer* const lexer){
    switch (lexer->tok->type)
    {
    case TK_RETURN: return parse_return_statement(lexer);

    case TK_IDENTIFIER:
        if(hashmap_get(type_map,lexer->tok->value)){
            return parse_decl(lexer, S_LOCAL);
        }
        Node* result = parse_expr(lexer);
        next(lexer);
        return result;
    case TK_IF: return parse_if_statement(lexer);
    case TK_WHILE: return parse_while_statement(lexer);
    
    case TK_ASYNC: next(lexer);
    case TK_SPECIFIER: return parse_decl(lexer, S_LOCAL);
    
    default:
        throw_error(lexer->position, "Invalid statement");
        next(lexer);
        return NULL;
    }
   
    return NULL;
}

Type* parse_type(Lexer* const lexer){
    Type* t = malloc(sizeof(Type));
    Type* template = hashmap_get(type_map, lexer->tok->value);
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
            expect(lexer, TK_INT_LITERAL, "integer literal");
            next(lexer);
            expect(lexer, TK_RIGHT_SQUARE, "']");
            // apply array and size to the type
            next(lexer);
            continue;
        }
        return t;
    }   
}


static Node* const parse_class_statement(Lexer* const lexer) {
    expect(lexer, TK_ACCESS, "attribute or method");
    
    next(lexer);
    Node* class_stmt_node = parse_decl(lexer, S_CLASS);
    return class_stmt_node;
}

static Node* const parse_class(Lexer* const lexer){
    Node* class_node = init_node(3, N_CLASS, lexer->position);
    next(lexer);
    expect(lexer, TK_IDENTIFIER, "identifier");
    next(lexer);

    if(lexer->tok->type == TK_ARROW){
        next(lexer);
        Type* extends = parse_type(lexer);
        vector_push(class_node->children, extends);
    }

    expect(lexer, TK_LEFT_CURLY, "'{'");
    next(lexer);
    while(lexer->tok->type != TK_RIGHT_CURLY){    
        vector_push(class_node->children, parse_class_statement(lexer) );
    }
    expect(lexer, TK_RIGHT_CURLY, "'}'");
    next(lexer);

    return class_node;
}

/**
 * Parses the sequence SPECIFIER* TYPE ID
 */
static Symbol* const parse_symbol(Lexer* const lexer, Scope scope){
    Symbol* s = malloc(sizeof(Symbol));
    int flags = 0;
    s->scope = scope;
    while(lexer->tok->type == TK_SPECIFIER){
        switch (lexer->tok->value[0])
        {
        case 'm':
            if(flags & MUTABLE_FLAG){
                duplicated_flag_warning(lexer->position, "mutable");
            }
            flags |= MUTABLE_FLAG;
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
    Type* extends = hashmap_get(type_map, lexer->tok->value);
    if(extends != NULL){
        const char* base_type = extends->name;
        s->scope = S_EXTEND;
        next(lexer);
        expect(lexer, TK_TYPE_EXTENSION_OP, "'::'");
        add_namespace(base_type);
        next(lexer);
        if(lexer->tok->type == TK_OPERATOR){
            next(lexer);
            if(!(IS_OPERATOR(lexer->tok->type))) {
                expect(lexer, TK_COMPARATION_OP, "operator");
            }
            s->name = lexer->tok->value;
            return s;
        }
    }
    expect(lexer, TK_IDENTIFIER, "identifier");
    s->name = mangle_namespace(lexer->tok->value, last_namespace->content);
    return s;
}

// 
// TOP-LEVEL-STATEMENT PARSING FUNCTIONS
//

// Tries to parse a variable statement (symbol = expr;) | (symbol;)
static Node* const parse_var(Lexer* const l, Symbol* s, bool has_value){
    Node* var_node = init_node(2, N_VAR, l->position);
    s->defined_pos = l->position;
    vector_push(var_node->children, s);
    if(has_value == false){
        expect(l, TK_SEMICOLON, "semicolon or declaration");
        next(l);
        vector_push(var_node->children, NULL_EXPR(l->position));
        return var_node;
    }
    expect(l, TK_EQUAL, "'='");
    next(l);
    Node* expr = parse_expr(l);
    expect(l, TK_SEMICOLON, "semicolon");
    vector_push(var_node->children, expr);
    next(l);
    return var_node;   
}

// Tries to parse a lambda function (symbol(args) => expr;) | (symbol(args) => {expr};)
static Node* const parse_lambda(Lexer* const lexer, Symbol* s){
    s->type->flags |= LAMBDA_FLAG;
    s->type->flags |= FUNCTION_FLAG;
    int inside_curly_brackets = lexer->tok->type == TK_LEFT_CURLY;
    if(inside_curly_brackets){
        next(lexer);
    }
    Node* expr = parse_expr(lexer);
    if(inside_curly_brackets){
        expect(lexer, TK_RIGHT_CURLY, "'}'");
        next(lexer);
    }
    expect(lexer, TK_SEMICOLON, "semicolon");
    next(lexer);

    Node* lambda = init_node(2, N_LAMBDA, lexer->position);
    vector_push(lambda->children, s);


    vector_push(lambda->children, expr);
    return lambda;
}

static void parse_enum(Lexer* const l){

    expect(l, TK_LEFT_CURLY, "'{'");
    next(l);
    while(l->tok->type != TK_RIGHT_CURLY){
        // symbol* child = parse_symbol(l, S_PARAMETER);
        // next(l);
        // s->type->align = child->type->align > s->type->align ? child->type->align : s->type->align;
        // ++children;
        // expect(l->tok, TK_SEMICOLON, "semicolon");
        // vector_push(struct_node->children, child);
        next(l);
    }
    next(l);
}

/**
 * Parses the sequence "struct" ID "{" ([symbol]";")* "}"
 */
static void parse_struct(Lexer* const l){
    next(l);
    expect(l, TK_IDENTIFIER, "identifier");
    const char* id = l->tok->value;
    next(l);
    Type* struct_type = malloc(sizeof(Type));
    expect(l, TK_LEFT_CURLY, "'{'");
    next(l);
    uint32_t children = 0;
    while(l->tok->type != TK_RIGHT_CURLY){
        Symbol* child = parse_symbol(l, S_PARAMETER);
        next(l);
        struct_type->align = max(child->type->align, struct_type->align);
        ++children;
        expect(l, TK_SEMICOLON, "semicolon");
        next(l);
    }
    struct_type->size = struct_type->align * children;
    struct_type->flags = STRUCT_FLAG;
    struct_type->name = id;
    struct_type->type = TY_STRUCT;

    Type* duplicated = hashmap_get(type_map, id);
    if(duplicated){
        duplicated_type_err(l->position, id);
        next(l);
        return;
    }
    hashmap_add(type_map, id, struct_type);
    next(l);
}

static Node* const parse_class_special_methods(Lexer* const lexer){

    return NULL;
}   

static function_info* parse_function_args(Lexer* const lexer){
    function_info* info = init_function_info();
    next(lexer);
    int optional_args = 0;
    while(lexer->tok->type != TK_RIGHT_PAR){
        Symbol* arg = parse_symbol(lexer, S_PARAMETER);
        vector_push(info->args, arg);
        next(lexer);
        if(!optional_args){
            if(lexer->tok->type == TK_EQUAL){
                optional_args = 1;
                next(lexer);
                vector_push(info->optional_values, parse_expr(lexer)); 
            }
            else {
                ++info->min_args;    
            }
        }
        else {
            expect(lexer, TK_EQUAL, "initializer");
            next(lexer);
            vector_push(info->optional_values, parse_expr(lexer));
        }
        if(lexer->tok->type == TK_COMMA){
            next(lexer);
            continue;
        } 
        expect(lexer, TK_RIGHT_PAR, "')'");
    }
    return info;
}

static Node* parse_decl(Lexer* const lexer, Scope scope){
    if(scope == S_CLASS && 
    (lexer->tok->type == TK_CONSTRUCTOR ||
    lexer->tok->type == TK_DESTRUCTOR)){
        return parse_class_special_methods(lexer);
    }
    Symbol* s = parse_symbol(lexer, scope);
    next(lexer);
    switch (lexer->tok->type)
    {
    case TK_EQUAL: return parse_var(lexer, s, 1);
    case TK_SEMICOLON: return parse_var(lexer, s, 0);

    case TK_LEFT_PAR:
        add_namespace(s->name);
        
        s->type->flags |= FUNCTION_FLAG;
        s->info = parse_function_args(lexer);
        delete_last_namespace();
        if(s->scope == S_EXTEND) {delete_last_namespace();}
        next(lexer);
        if(lexer->tok->type == TK_ARROW){
            next(lexer);
            return parse_lambda(lexer, s);
        }

        Node* func_decl_node = init_node(2,N_FUNCTION, lexer->position);
        vector_push(func_decl_node->children, s);
        
        if(lexer->tok->type == TK_LEFT_CURLY){
            next(lexer);
            while(lexer->tok->type != TK_RIGHT_CURLY){
                Node* stmt = parse_statement(lexer);
                if(stmt == NULL) break;
                vector_push(func_decl_node->children, stmt);
            }
            next(lexer);
        }
        else{
            expect(lexer, TK_SEMICOLON, "semicolon");
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
static Node* const parse_global(Lexer* const lexer){
    switch (lexer->tok->type)
    {
    case TK_CLASS: return parse_class(lexer);
    case TK_STRUCT: parse_struct(lexer); break;
    case TK_ASYNC:
        next(lexer);
        return parse_decl(lexer, S_GLOBAL);
    case TK_SPECIFIER:
    case TK_IDENTIFIER: return parse_decl(lexer, S_GLOBAL);

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

// Tries to parse an import (IMPORT STRING) and merges the target AST
// with the main one.
static void parse_import(Lexer* const lexer, Node* ast){
    next(lexer);
    expect(lexer, TK_STRING_LITERAL, "import path");
    const char* path = resolve_path(delete_quotes(lexer->tok->value));
    next(lexer);
    if(hashmap_get(imported_files, path) != NULL) return;
    Node* imported_file = parse(path);
    if(imported_file == NULL) return;
    for(uint32_t i = 0; i < imported_file->children->len; ++i){
        vector_push(ast->children, imported_file->children->value[i]);
    }
    free_node(imported_file);
}

// Tries to parse a type definition (TYPEDEF ID = type)
// Internally, creates an entry on the type table pointing
// to the type.
static void parse_typedef(Lexer* const lexer){
    next(lexer);
    const char* id = lexer->tok->value;
    expect(lexer, TK_IDENTIFIER, "type name");
    next(lexer);
    expect(lexer, TK_EQUAL, "'='");
    next(lexer);
    Type* target = parse_type(lexer);
    Type* def = malloc(sizeof(Type));
    *def = *target;
    def->name = id; 
    hashmap_add(type_map, id, def);
}

// Parses a list of tokens and returns a ParseTree containing 
// the AST, the type table and the symbol table.
Node* parse(const char* file){
    String* const input = read_file(file);
    if(input == NULL) return NULL;
    Lexer* const lexer = init_lexer(file, input->content);
    if(lexer == NULL) return NULL;
    start_parsing(lexer);

    next(lexer);
    Node* const ast = init_node(16, N_ROOT, lexer->position);
    while (lexer->tok->type != TK_EOF)
    {
        switch(lexer->tok->type){
            case TK_IMPORT:
                parse_import(lexer, ast);
                break;
            case TK_TYPEDEF:
                parse_typedef(lexer);
                break;
            default:
                vector_push(ast->children, parse_global(lexer));  
                break;
        } 
    }
    return ast;
}