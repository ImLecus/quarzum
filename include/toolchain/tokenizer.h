/*
 * Quarzum Compiler - tokenizer.c
 * Version 1.0, 24/07/2024
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

#define ADD_TOKEN(t) push_tlist(tokens, &(struct token){t, get_buffer(buffer), &(struct token_info){lineNumber, columnNumber, file}});clear_buffer(buffer);

#define t_ch src->value[i]
#define t_next src->value[i + 1]
#define t_advance ++i;++columnNumber

/**
 * @brief Reads a file and converts the source file text into a TokenList.
 * It will return NULL if something goes wrong.
 * Complexity: O(n)
 */
TokenList* tokenize(char* file);
