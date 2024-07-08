/*
 * Quarzum Compiler - bsearch.h
 * Version 1.0, 07/07/2024
 *
 * This file is part of the Quarzum project, a proprietary software.
 *
 * Quarzum Project License
 * ------------------------
 *
 * For Contributions License Agreement (CLA), see CONTRIBUTING.md.
 * For full details, see LICENSE.
 */
#ifndef BSEARCH_H
#define BSEARCH_H
#include <string.h>

/**
 * @brief Finds a `const char*` inside a list.
 * Returns the index of the symbol in the list.
 * Returns -1 if something goes wrong.
 */
int binarySearch(const char* symbol, const char** list, unsigned int size); 

#endif