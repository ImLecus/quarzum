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
#include <stdlib.h>
#include <string.h>
typedef const unsigned long b_size_t;
/**
 * @brief Defines a buffer with limited size.
 */
typedef struct {
    u_int64_t size;
    u_int64_t len;
    char* value;
} Buffer;

/**
 * @brief Allocates new memory for a larger buffer of size `newSize`.
 * @return `-1` if the allocation goes wrong, `0` otherwise.
 * Complexity: O(n)
 */
void resizeBuffer(Buffer* b, b_size_t newSize);
/**
 * @brief Creates a new `Buffer`.
 * Complexity: O(1)
 */
Buffer* createBuffer(b_size_t size);
/**
 * @brief Deletes a `Buffer` and free the allocated memory.
 * Complexity: O(1)
 */
void deleteBuffer(Buffer* buffer);
/**
 * @brief Adds a new `char` at the last avaliable position.
 * If the `Buffer` is full, it will be resized.
 * Complexity: O(1), O(n) if it needs to resize.s
 */
void addToBuffer(Buffer* b,const char c);
/**
 * @brief Deletes the last `char` and returns it.
 * Complexity: O(1)
 */
void popFromBuffer(Buffer* b);
/**
 * @brief Fills the `Buffer` with null characters (\0) and resets the length.
 * Complexity: O(n)
 */
void clearBuffer(Buffer* b);
/**
 * @brief Returns a copy of the `Buffer` value. 
 * Complexity: O(n)
 */
char* getBuffer(const Buffer *b);
#endif