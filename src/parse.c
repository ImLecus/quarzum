#include "quarzum.h"

static node* parse_statement(lexer* lexer);
static node* parse_decl(lexer* lexer, int scope);
static node* parse_module(lexer* lexer);

static int filled = 1;
static hashmap* imported_files;
static hashmap* type_map;
static hashmap* symbol_map;
static bool has_errors = false;
static string* last_namespace;

/**
 * Initializes static variables one time per program.
 */
static void start_parsing(lexer* lexer){
    if(!imported_files){
        imported_files = init_hashmap(32);
    }
    hashmap_add(imported_files, lexer->file, &filled);
    if(!type_map){
        type_map = init_type_map();
    }
    if(!symbol_map){
        symbol_map = init_hashmap(256);
    }
    if(!last_namespace){
        last_namespace = init_string(32);
    }
}

static void add_namespace(char* namespace){
    if(last_namespace->len > 0){
        string_push(last_namespace, '@');
    }
    string_append(last_namespace, namespace);
}

static void delete_last_namespace(){
    for(uint32_t i = last_namespace->len; i > 0; --i){
        if(last_namespace->value[i] == '@'){
            string_pop(last_namespace);
            break;
        }
        string_pop(last_namespace);
        
    }
}

static void free_node(node* n){
    free_vector(n->children);
    free(n);
}

void free_parse_tree(parse_tree* tree){
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

void expect(token* t, uint8_t type, char* what){
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
    
    return f;
}

static void parse_import(lexer* lexer, node* ast){
    read_next(lexer);
    if(lexer->tok->type == T_STRING_LITERAL){
        char* path = resolve_path(delete_quotes(lexer->tok->value));
        char* as = NULL; 
        read_next(lexer);
        if(lexer->tok->type == T_KEYWORD_AS){
            read_next(lexer);
            expect(lexer->tok, T_IDENTIFIER, "identifier");
            as = lexer->tok->value;
        }

        struct process importing = start_process("File import");
        if(hashmap_get(imported_files, path) == NULL){
            
            parse_tree* imported_file = parse(path);
            if(!imported_file){
                return;
            }

            if(as){
                // create a virtual module with name {as}
                // maybe transforming the root node?
            }
            for(uint32_t i = 0; i < imported_file->ast->children->len; ++i){
                vector_push(ast->children, imported_file->ast->children->value[i]);
            }
            free_vector(imported_file->ast->children);
            free(imported_file->ast);

            free(imported_file);
            
        }
        else{
            printf(CYAN"[LOG] "RESET"(%s) File already imported, avoided symbol duplication\n", path);
        }
        end_process(&importing);

        return;
    }
}

/**
 * Parses the sequence "return" [expr]? ";"
 */
static node* parse_return_statement(lexer* lexer){
    node* return_node = init_node(1,N_RETURN);
    read_next(lexer);
    if(lexer->tok->type == T_SEMICOLON){
        vector_push(return_node->children, null_expr());
        return return_node;
    }
    node* expr = parse_expr(lexer);
    expect(lexer->tok, T_SEMICOLON, "semicolon");
    vector_push(return_node->children, expr);
    return return_node;
}

static node* parse_function_call(lexer* lexer, char* id){
    node* call_node = init_node(1, N_CALL);
    vector_push(call_node->children, id);
    read_next(lexer);
    while(lexer->tok->type != T_RIGHT_PAR){
        node* argument = parse_expr(lexer);
        vector_push(call_node->children, argument);
        if(lexer->tok->type == T_COMMA){
            read_next(lexer);
        }
        else{
            expect(lexer->tok, T_RIGHT_PAR, "')'");
            break;
        }
    }
    read_next(lexer);
    expect(lexer->tok, T_SEMICOLON, "semicolon");
    read_next(lexer);
    return call_node;
}

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

static node* parse_assign(lexer* lexer, char* id){
    read_next(lexer);
    node* assign_node = init_node(2, N_ASSIGN);
    vector_push(assign_node->children, id);
    node* expr = parse_expr(lexer);
    vector_push(assign_node->children, expr);
    expect(lexer->tok, T_SEMICOLON, "semicolon");
    read_next(lexer);
    return assign_node;
}

static node* parse_statement(lexer* lexer){
    switch (lexer->tok->type)
    {
    case T_KEYWORD_RETURN:
        return parse_return_statement(lexer);

    case T_IDENTIFIER:
        char* id = lexer->tok->value;
        read_next(lexer);
        switch (lexer->tok->type)
        {
        case T_LEFT_PAR:
            return parse_function_call(lexer, id);
        
        case T_EQUAL:
            return parse_assign(lexer, id);
        default:
            break;
        }
    case T_KEYWORD_MODULE:
        return parse_module(lexer);
    case T_KEYWORD_IF:
        return parse_if_statement(lexer);
    case T_KEYWORD_WHILE:
        return parse_while_statement(lexer);
    case T_TYPE:
    case T_SPECIFIER:
        return parse_decl(lexer, S_LOCAL);

    default:
        printf(RED"[ERROR]"RESET" (%s) Invalid statement at line %d\n", lexer->file, lexer->line);
        read_next(lexer);
        return NULL;
    }
   
    return NULL;
}

static node* parse_module(lexer* lexer){
    node* module_node = init_node(2, N_MODULE);
    read_next(lexer);
    expect(lexer->tok, T_IDENTIFIER, "identifier");
    char* id = lexer->tok->value;
    vector_push(module_node->children, id);
    // symbol table check
    read_next(lexer);
    
    expect(lexer->tok, T_LEFT_CURLY, "'{'");
    read_next(lexer);
    add_namespace(id);
    while(lexer->tok->type != T_RIGHT_CURLY){
        vector_push(module_node->children, parse_statement(lexer) );
    }
    delete_last_namespace();
    expect(lexer->tok, T_RIGHT_CURLY, "'}'");
    read_next(lexer);
    return module_node;
}

static type* parse_type(lexer* lexer){
    type* t = malloc(sizeof(type));
    t->flags = 0;
    type* template = hashmap_get(type_map, lexer->tok->value);
    if(!template){
        printf(RED"[ERROR]"RESET" (%s) Undefined type '%s' at line %d\n", lexer->file, lexer->tok->value, lexer->line);
        template = ty_int8;    
    }
    memcpy(t, template, sizeof(type));
    read_next(lexer);
    if(strcmp(lexer->tok->value, "*") == 0){
        t->flags |= POINTER_FLAG;
        read_next(lexer);
    }

    return t;
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
        vector_push(class_node->children, parse_statement(lexer) );
    }
    expect(lexer->tok, T_RIGHT_CURLY, "'}'");
    read_next(lexer);

    return class_node;
}

/**
 * Parses the sequence [specifier]* [type] [identifier]
 */
static symbol* parse_symbol(lexer* lexer, int scope){
    symbol* s = malloc(sizeof(symbol));
    int flags = 0;
    s->scope = scope;
    while(lexer->tok->type == T_SPECIFIER){
        switch (lexer->tok->value[0])
        {
        case 'c':
            if((flags & CONST_FLAG) > 0){
                printf(ORANGE BOLD"[WARNING]"RESET" (%s) Duplicated 'const' specifier at line %i\n",lexer->file,lexer->line);
            }
            flags |= CONST_FLAG;
            break;
        case 'f':
            if((flags & FOREIGN_FLAG) > 0){
                printf(ORANGE BOLD"[WARNING]"RESET" (%s) Duplicated 'foreign' specifier at line %i\n",lexer->file,lexer->line);
            }
            flags |= FOREIGN_FLAG;
        default:
            // err
            break;
        }
        read_next(lexer);
    }
    s->type = parse_type(lexer);
    s->type->flags |= flags;
    expect(lexer->tok, T_IDENTIFIER, "identifier");

    s->name = lexer->tok->value;
    
    return s;
}

/**
 * Parses the sequence [symbol] = ([expr]; | {[expr]});
 */
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

    s->mangled_name = mangle_name(s, last_namespace);
    hashmap_add(symbol_map, s->name, s);

    vector_push(lambda->children, expr);
    return lambda;
}

/**
 * Parses the sequence [symbol] (";" | "=" [expr] ";") 
 */
static node* parse_var(lexer* l, symbol* s, bool has_value){
    node* var_node = init_node(2, N_VAR);
    vector_push(var_node->children, s);

    symbol* dup = hashmap_get(symbol_map, s->name);
    if(dup){
        printf(RED"[ERROR] "RESET"Symbol '%s' already exists \n", s->name);
        has_errors = true;
    }
    s->mangled_name = mangle_name(s, last_namespace);
    hashmap_add(symbol_map, s->name, s);

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
    vector_push(var_node->children, null_expr());
    return var_node;
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
 * Parses the sequence "struct" [id] "{" ([symbol]";")* "}"
 */
static void parse_struct(lexer* l){
    read_next(l); // first token is the keyword
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

static node* parse_decl(lexer* lexer, int scope){
    symbol* s = parse_symbol(lexer, scope);
    read_next(lexer);
    switch (lexer->tok->type)
    {
    case T_EQUAL:
        return parse_var(lexer, s, true);
    
    case T_SEMICOLON:
        return parse_var(lexer, s, false);

    case T_LEFT_PAR:
        s->type->flags |= FUNCTION_FLAG;
        s->type->info = init_function_info();
        function_info* info = s->type->info;
        read_next(lexer);
        while(lexer->tok->type != T_RIGHT_PAR){
            symbol* arg = parse_symbol(lexer, S_PARAMETER);
            vector_push(info->args, arg->type);
            read_next(lexer);
            if(lexer->tok->type == T_COMMA){
                read_next(lexer);
                continue;
            }
            expect(lexer->tok, T_RIGHT_PAR, "')'");
        }
        read_next(lexer);
        if(lexer->tok->type == T_ARROW){
            read_next(lexer);
            return parse_lambda(lexer, s);
        }


        node* func_decl_node = init_node(2,N_FUNCTION);
        vector_push(func_decl_node->children, s);

        s->mangled_name = mangle_name(s, last_namespace);
        hashmap_add(symbol_map, s->name, s);


        if(lexer->tok->type == T_LEFT_CURLY){
            read_next(lexer);
            
            info->local_variables = malloc(sizeof(symbol*) * 4);
            info->local_variables_len = 0;
            info->local_variables_size = 4;
            while(lexer->tok->type != T_RIGHT_CURLY){
                node* stmt = parse_statement(lexer);
                if(!stmt){
                    break;
                }
                if(stmt->type == N_VAR){
                    symbol* local_var = ((symbol*) stmt->children->value[0]); 
                    if(info->local_variables_len >= info->local_variables_size){
                        info->local_variables = realloc(info->local_variables, info->local_variables_size * 2);
                        info->local_variables_size *= 2;
                    }
                    info->local_variables[info->local_variables_len++] = local_var;
                    info->align = local_var->type->size;
                }
                vector_push(func_decl_node->children, stmt);
            }
            s->type->info = info;
            read_next(lexer);
        }
        else{
            expect(lexer->tok, T_SEMICOLON, "semicolon");
            read_next(lexer);
        }

        return func_decl_node;
    default:
        return NULL;
    }
}

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
            case T_KEYWORD_CLASS:
                vector_push(result->ast->children, parse_class(lexer));  
                break;
            case T_KEYWORD_STRUCT:
                parse_struct(lexer);
                break;

            case T_SPECIFIER:
            case T_TYPE:
            case T_IDENTIFIER:
                vector_push(result->ast->children, parse_decl(lexer, S_GLOBAL));
                break;
            default:
                printf(RED"[ERROR] "RESET"(%s) Unexpected token '%s' at line %d\n", lexer->file, lexer->tok->value, lexer->line);
                has_errors = true;
                read_next(lexer);
                break;
        } 
    }
    result->has_errors = has_errors;
    result->symbol_table = symbol_map;
    result->type_table = type_map;
    free(lexer);
    return result;
}