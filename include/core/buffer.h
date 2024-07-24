/*
 * Quarzum Compiler - buffer.h
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
#ifndef BUFFER_H
#define BUFFER_H
#include <string.h>
#include <stdlib.h>
#include "defines.h"
/**
 * @brief Defines a buffer with limited size.
 */
typedef struct {
    uint64 size;
    uint64 len;
    char* value;
} Buffer;
/**
 * @brief Creates a new `Buffer`.
 * Complexity: O(1)
 */
Buffer* init_buffer(const uint64 size);
/**
 * @brief Deletes a `Buffer` and free the allocated memory.
 * Complexity: O(1)
 */
void delete_buffer(Buffer* buffer);
/**
 * @brief Adds a new `char` at the last avaliable position.
 * If the `Buffer` is full, it will be resized.
 * Complexity: O(1), O(n) if it needs to resize.s
 */
void add_buffer(Buffer* b, const char c);
/**
 * @brief Deletes the last `char` and returns it.
 * Complexity: O(1)
 */
void pop_buffer(Buffer* b);
/**
 * @brief Fills the `Buffer` with null characters (\0) and resets the length.
 * Complexity: O(n)
 */
void clear_buffer(Buffer* b);
/**
 * @brief Returns a copy of the `Buffer` value. 
 * Complexity: O(n)
 */
char* get_buffer(const Buffer *b);
#endif