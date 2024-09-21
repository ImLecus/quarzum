/**
 * \file            parse.h
 * \brief           Parse include file
 * \copyright       2024 Marcos González GPL3
 */
#ifndef PARSE_H
#define PARSE_H
#include "symbol.h"
#include "tokenize.h"

/**
 * \brief           Defines a `node_t` type.
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
    N_CLASS_STMT
} node_type_t;

/**
 * \brief           Defines an AST node.
 */
typedef struct {
    node_type_t type;
    pos_t pos;
    vector_t* children;
} node_t;

/**
 * \brief           Gets the nth child of a `node_t`.
 * \returns         A pointer to the nth child, `NULL` in
 *                  case of error.
 */
void* n_get(node_t* n, uint32_t index);

/**
 * \brief           Allocates a new `node_t` on the heap.
 * \returns         A pointer to the allocated `node_t`.
 * \warning         Returns `NULL` in case of allocation error.
 * \param           children: the initial children number of the node.
 * \param           type: the node type.
 * \param           pos: the node position in the file.
 */
node_t* init_node(uint32_t children, node_type_t type, pos_t pos);

/**
 * \brief           Checks if the token type is the same
 *                  as the type passed. If not, throws an error
 *                  and continues the program.
 * \param           t: the token to be checked
 * \param           type: the expected token type
 * \param           what: error message.
 */
void expect(token_t* t, node_type_t type, char* what);

/**
 * \brief           Parses a type defined in Quarzum.
 * This function reads a list of specifiers and the type
 * keyword itself, which can be a primitive or a custom type.
 * The type to be parsed needs to be parsed first, so a custom
 * type has to be defined in the file before using it.
 * \returns         A pointer to a `type`, `NULL` in case of error. 
 */
type* parse_type(lexer_t* lexer);

/**
 * \brief           Parses a file.
 * \returns         A pointer to a `node_t`, containing the 
 *                  AST as a result of the operation.
 * \note            This function is recursive due to how
 *                  `import` works in Quarzum. Call this function
 *                  only at the main file.
 * \param           file: the file path to be parsed.
 */
node_t* parse(char* file);


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
 * \returns         A pointer to a `node_t`, containing the
 *                  expression. Returns `NULL_EXPR` in case
 *                  of error.
 * \note            This function does not return `NULL` in
 *                  any case.
 */
node_t* parse_expr(lexer_t* lexer);

#endif