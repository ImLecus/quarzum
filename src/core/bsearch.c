/*
 * Quarzum Compiler - bsearch.c
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
#include "../../include/core/bsearch.h"
#include <stdio.h>

int binarySearch(const char* symbol, const char** list, unsigned int size){
    int left = 0;
    int right = size;
    while(left <= right){
        int mid = left + (right - left)/2;
        int cmp = strcmp(symbol, list[mid]);

        if(cmp == 0){
            return mid;
        }
        else if(cmp < 0){
            right = mid - 1;
        }
        else {
            left = mid + 1;
        }
    }
    return -1;
}