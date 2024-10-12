/**
 * \file            position.h
 * \brief           File internal position include file
 * \copyright       2024 Marcos González GPL3
 */
#ifndef POSITION_H
#define POSITION_H

/**
 * \brief           Defines a position inside a file
 */
typedef struct Position {
    const char* file;
    int line;
    int column;
} Position;

/**
 * \brief           Advances by one the position column. If the
 *                  provided character is '\n', update_poss the line too. 
 */
void update_pos(Position* pos, char c);

#endif /* POSITION_H */