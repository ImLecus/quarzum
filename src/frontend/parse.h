/**
 * \file            parse.h
 * \brief           Parse include file
 * \copyright       2024 Marcos González GPL3
 */
#ifndef PARSE_H
#define PARSE_H
#include "symbol.h"
#include "tokenize.h"
#include "expr.h"
/**
 * \brief           Defines a `Node` type.
 */
typedef enum {
    N_ROOT,
    N_IF,
    N_WHILE,
    N_VAR,
    N_MODULE,
    N_BREAK,
    N_CONTINUE,
    N_FUNCTION,
    N_IDENTIFIER,
    N_TYPE,
    N_RETURN,
    N_LAMBDA,
    N_ENUM,
    // Expression nodes
    N_UNARY_EXPR,
    N_BINARY_EXPR,
    N_PAREN_EXPR,
    N_TERNARY_EXPR,
    N_MEMBER_EXPR,
    N_CALL_EXPR,
    N_NULL_EXPR,
    N_CAST,
    N_INDEX_EXPR,

    N_LITERAL,
    N_CLASS,
    N_INIT_LIST,
    N_CLASS_STMT,

    N_INVALID = 255
} NodeType;

/**
 * \brief           Defines an AST node.
 */
typedef struct {
    NodeType type;
    pos_t pos;
    Vector* children;
} Node;

/**
 * \note            Use `AST` only to indicate that the
 *                  node passed is a root node.
 */
typedef Node AST;

/**
 * \brief           Gets the nth child of a `Node`.
 * \returns         A pointer to the nth child, `NULL` in
 *                  case of error.
 */
void* n_get(Node* const n, uint32_t index);

/**
 * \brief           Allocates a new `Node` on the heap.
 * \returns         A pointer to the allocated `Node`.
 * \warning         Returns `NULL` in case of allocation error.
 * \param           children: the initial children number of the node.
 * \param           type: the node type.
 * \param           pos: the node position in the file.
 */
Node* const init_node(const uint32_t children, const NodeType type, pos_t pos);

/**
 * \brief           Checks if the token type is the same
 *                  as the type passed. If not, throws an error
 *                  and continues the program.
 * \param           t: the token to be checked
 * \param           type: the expected token type
 * \param           what: error message.
 */
void expect(const Token* t, const TokenType type, const char* what);

/**
 * \brief           Parses a type defined in Quarzum.
 * This function reads a list of specifiers and the type
 * keyword itself, which can be a primitive or a custom type.
 * The type to be parsed needs to be parsed first, so a custom
 * type has to be defined in the file before using it.
 * \returns         A pointer to a `type`, `NULL` in case of error. 
 */
Type* parse_type(Lexer* const lexer);

/**
 * \brief           Parses a file.
 * \returns         A pointer to a `Node`, containing the 
 *                  AST as a result of the operation.
 * \note            This function is recursive due to how
 *                  `import` works in Quarzum. Call this function
 *                  only at the main file.
 * \param           file: the file path to be parsed.
 */
Node* parse(const char* file);

static Node* parse_statement(Lexer* lexer);
static Node* parse_decl(Lexer* lexer, scope_t scope);

static Hashmap* imported_files;
static String* last_namespace;


#ifndef NULL_EXPR
/**
 * \brief          Defines a null expression.
 * In Quarzum, `null` represents a null value depending on
 * the target type, generally is equivalent to 0. NULL_EXPR
 * is the same as using the constant `null`.
 */
#define NULL_EXPR(pos) init_node(0, N_NULL_EXPR, pos)
#endif 

/**
 * \brief           Parses an expression.
 * \returns         A pointer to a `Node`, containing the
 *                  expression. Returns `NULL_EXPR` in case
 *                  of error.
 * \note            This function does not return `NULL` in
 *                  any case.
 */
Node* parse_expr(Lexer* lexer);

#endif