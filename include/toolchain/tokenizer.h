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
#include <ctype.h>

#define DEFAULT_TOKENIZER_BUFFER_SIZE 10

#define ADD_TOKEN(t) addToTokenList(tokens, t);clearBuffer(buffer);

#define TOKEN_INFO {lineNumber, columnNumber, file}
/**
 * @brief Reads a file and converts the source file text into a TokenList.
 * It will return NULL if something goes wrong.
 */
TokenList* tokenize(char* file);
