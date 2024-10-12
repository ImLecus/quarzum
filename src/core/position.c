/**
 * \file            position.c
 * \brief           Position implementation file
 * \copyright       2024 Marcos González GPL3
 */
#include "position.h"

void update_pos(Position* pos, char c){
    if(c == '\n'){
        ++pos->line;
        pos->column = 0;
    }
    ++pos->column;
}