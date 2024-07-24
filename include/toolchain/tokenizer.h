/*
 * Quarzum Compiler - tokenizer.c
 * Version 1.0, 06/07/2024
 *
 * This file is part of the Quarzum project, a proprietary software.
 *
 * Quarzum Project License
 * ------------------------
 *
 * For Contributions License Agreement (CLA), see CONTRIBUTING.md.
 * For full details, see LICENSE.
 */
#include "../quarzum/tokenlist.h"
#include "../core/buffer.h"
#include "../quarzum/defs.h"
#include "../quarzum/io.h"
#include "../core/debug.h"
#include "../core/char.h"

#define DEFAULT_TOKENIZER_BUFFER_SIZE 10

#define ADD_TOKEN(t) addToTokenList(tokens, t);clear_buffer(buffer);

#define TOKEN_INFO {lineNumber, columnNumber, file}

/**
 * @brief Reads a comment line and discards its text. 
 * Complexity: O(n)
 */
void read_comment(Buffer* src, u_int64_t* index, u_int32_t* lineNumber);
/**
 * @brief Reads a comment block. Will throw an error if the
 * comment block is not closed.
 * Complexity: O(n)
 */
void read_comment_block(Buffer* src, u_int64_t* index, u_int32_t* lineNumber);
/**
 * @brief Reads a string literal between double quotes ("").
 * Will throw an error if the string is not closed.
 * Escape characters are supported. Will throw an error if the
 * espace character does not exist.
 * Complexity: O(n)
 */
void read_string_literal(Buffer* src, Buffer* target, u_int64_t* index, u_int32_t* lineNumber);
/**
 * @brief Reads a numeric literal (integer or decimal).
 * Will throw an error if the number has two or more points.
 * @return 1 if the number is a decimal, 0 if it's an integer,
 * -1 if is not valid.
 * Complexity: O(n)
 */
int readNumberLiteral(Buffer* src, Buffer* target, u_int64_t* index, u_int32_t* lineNumber);
/**
 * @brief Reads a file and converts the source file text into a TokenList.
 * It will return NULL if something goes wrong.
 * Complexity: O(n)
 */
TokenList* tokenize(char* file);
