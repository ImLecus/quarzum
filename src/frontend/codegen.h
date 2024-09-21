/**
 * \file            codegen.h
 * \brief           AST to QByte conversion include file.
 * \copyright       2024 Marcos González GPL3
 */
#ifndef CODEGEN_H
#define CODEGEN_H
#include "check.h"
#include "../qbyte/qbyte.h"

/**
 * \brief           Converts an AST into a `ir_code_t`.
 * \returns         A pointer to the QByte result code.
 * \note            The function assumes the AST has been
 *                  checked before and it has no errors. 
 */
ir_code_t* generate_qbyte(node_t* ast);

/**
 * \brief           Converts a variable into a `ir_data_t`.
 * \note            The variable must be global. If the initial
 *                  value is a complex expression, the data will
 *                  have a `null` default value and it will generate
 *                  instructions to generate the expression result.
 */
ir_data_t generate_data(node_t* var);

/**
 * \brief           Converts a function node into a `ir_function_t`.
 * \note            This function also generates the instructions 
 *                  inside the function.
 */
ir_function_t generate_function(node_t* func);

/**
 * \brief           Converts a node into a `ir_instruction_t`.
 */
ir_instruction_t generate_instruction(node_t* node);

#endif