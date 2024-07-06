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
#pragma once
#include <string.h>

/**
 * @brief Defines a buffer with limited size.
 */
typedef struct {
    unsigned long size;
    unsigned long len;
    char* value;
} Buffer;

/**
 * @brief Allocates new memory for a larger buffer of size `newSize`.
 * @return `-1` if the allocation goes wrong, `0` otherwise.
 */
unsigned char resizeBuffer(Buffer* b, long newSize);
/**
 * @brief Creates a new `Buffer`.
 */
Buffer* createBuffer(const long size);
/**
 * @brief Deletes a `Buffer` and free the allocated memory.
 */
void deleteBuffer(const Buffer* b);
/**
 * @brief Adds a new `char` at the last avaliable position.
 * If the `Buffer` is full, it will be resized.
 */
void addToBuffer(Buffer* b,const char c);
/**
 * @brief Deletes the last `char` and returns it.
 */
char popFromBuffer(Buffer* b);
/**
 * @brief Fills the `Buffer` with null characters (\0) and resets the length.
 */
void clearBuffer(Buffer* b);
/**
 * @brief Returns a copy of the `Buffer` value. 
 */
const char* getBuffer(Buffer *b);