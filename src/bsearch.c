#include "quarzum.h"

int binary_search(const char* symbol, const char** list, unsigned int size){
    int left = 0;
    int right = size;
    while(left <= right){
        int mid = left + (right - left)/2;
        if(mid >= size){
            return -1;
        }
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