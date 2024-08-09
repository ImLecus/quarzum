#include "quarzum.h"
static node* parse_statement(lexer* lexer);
static node* parse_decl(lexer* lexer, int scope);
static const int filled = 1;
static hashmap* imported_files;

node* init_node(unsigned int children, int type){
    node* n = malloc(sizeof(node));
    n->children = init_vector(children);
    n->type = type;
    return n;
}

void expect(token* t, int type, char* what){
    if(t->type != type){
        printf(RED "[ERROR]" RESET " (%s) Expected %s at line %d.\n", 
               t->file, what, t->line);
    }
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
            for(unsigned int i = 0; i < imported_file->ast->children->len; ++i){
                vector_push(ast->children, imported_file->ast->children->value[i]);
            }
            
        }
        else{
            printf("%s -- File already imported.\n", path);
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
        
        default:
            break;
        }
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
    node* module_node = init_node(3, N_MODULE);
    read_next(lexer);
    expect(lexer->tok, T_IDENTIFIER, "identifier");
    vector_push(module_node->children, lexer->tok->value);
    // symbol table check
    read_next(lexer);
    
    expect(lexer->tok, T_LEFT_CURLY, "'{'");

    while(lexer->tok->type != T_RIGHT_CURLY){
        read_next(lexer);
        vector_push(module_node->children, parse_statement(lexer) );
    }
    expect(lexer->tok, T_RIGHT_CURLY, "'}'");

    return module_node;
}

static type* parse_type(lexer* lexer){
    type* t = malloc(sizeof(type));
    t->flags = 0;
    if(lexer->tok->type != T_TYPE){
        // custom type
    }
    else{
        // TO-DO: Create a type table (using a hashmap)

        if(strcmp(lexer->tok->value, "string") == 0){
            memcpy(t, ty_string, sizeof(type));
        }
        else if(strcmp(lexer->tok->value, "int") == 0){
            memcpy(t, ty_int32, sizeof(type));
        }
        else if(strcmp(lexer->tok->value, "char") == 0){
            memcpy(t, ty_char, sizeof(type));
        }
        else{
            // int8 as placeholder
            memcpy(t, ty_int8, sizeof(type));
        }
        
    }

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
    // create a symbol
    vector_push(class_node->children, lexer->tok->value);
    // parse optional inheritance
    read_next(lexer);
    if(lexer->tok->type == T_ARROW){
        // parse type
    }

    expect(lexer->tok, T_LEFT_CURLY, "'{'");
    while(lexer->tok->type != T_RIGHT_CURLY){
        read_next(lexer);
        vector_push(class_node->children, parse_statement(lexer) );
    }
    expect(lexer->tok, T_RIGHT_CURLY, "'}'");

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
    if(lexer->tok->type == T_KEYWORD_STRUCT){
        // parse_struct_decl
        return NULL;
    }
    s->type = parse_type(lexer);
    s->type->flags |= flags;
    expect(lexer->tok, T_IDENTIFIER, "identifier");
    // TO-DO: mangle name
    s->name = lexer->tok->value;
    return s;
}

/**
 * Parses the sequence [symbol] = ([expr]; | {[expr]});
 */
static node* parse_arrow_function(lexer* lexer,symbol* s){
    s->type->flags |= LAMBDA_FLAG;
    bool inside_curly_brackets = lexer->tok->type == T_LEFT_CURLY;
    if(inside_curly_brackets){
        read_next(lexer);
    }
    node* expr = parse_expr(lexer);
    if(inside_curly_brackets){
        expect(lexer->tok, T_RIGHT_CURLY, "'}'");
    }
    else{
        expect(lexer->tok, T_SEMICOLON, "semicolon");
    }
    node* arrow_function = init_node(N_LAMBDA, 1);
    vector_push(arrow_function->children, expr);
    return arrow_function;
}

static node* parse_decl(lexer* lexer, int scope){
    symbol* s = parse_symbol(lexer, scope);
    // if s is a struct, return the struct
    read_next(lexer);
    switch (lexer->tok->value[0])
    {
    case '=':
        if( (s->type->flags & FOREIGN_FLAG) > 0){
            printf(RED"[ERROR]"RESET" (%s) 'foreign' variables can't be initialized at line %i\n",lexer->file,lexer->line);
            // err
        }
        read_next(lexer);
        node* expr = parse_expr(lexer);
        expect(lexer->tok, T_SEMICOLON, "semicolon");

        node* var_decl_node = init_node(2,N_VAR);
        vector_push(var_decl_node->children, s);
        vector_push(var_decl_node->children, expr);
        return var_decl_node;
    
    case ';':
        node* var_def_node = init_node(2,N_VAR);
        vector_push(var_def_node->children, s);
        vector_push(var_def_node->children, null_expr());
        return var_def_node;

    case '(':
        s->type->flags |= FUNCTION_FLAG;
        read_next(lexer);
        function_info* info = s->type->info;

        while(lexer->tok->type != T_RIGHT_PAR){
            symbol* arg = parse_symbol(lexer, true);
            //vector_push(info->args, arg->type);
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
            return parse_arrow_function(lexer, s);
        }


        node* func_decl_node = init_node(2,N_FUNCTION);
        vector_push(func_decl_node->children, s);
        if(lexer->tok->type == T_LEFT_CURLY){
            read_next(lexer);
            function_info* info = malloc(sizeof(function_info)); 
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
                read_next(lexer);
            }
            s->type->info = info;
            read_next(lexer);
        }
        else{
            expect(lexer->tok, T_SEMICOLON, "semicolon");
        }
        //mangle_name(s);
        return func_decl_node;
    default:
        break;
    }
}

parse_tree* parse(char* file){
    string* input = read_file(file);
    if(!input){return NULL;}
    lexer* lexer = init_lexer(file,input->value);
    if(!lexer){return NULL;}

    if(!imported_files){
        imported_files = init_hashmap(20);
    }
    hashmap_add(imported_files, file, &filled);

    parse_tree* result = malloc(sizeof(parse_tree));
    result->has_errors = false;
    read_next(lexer);
    result->ast = init_node(10, N_ROOT);
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
            case T_SPECIFIER:
            case T_TYPE:
                vector_push(result->ast->children, parse_decl(lexer, S_GLOBAL));
                break;
            default:
                read_next(lexer);
                break;
        } 
 
    }
    
    free(lexer);
    return result;
}