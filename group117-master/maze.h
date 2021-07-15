/** @file   maze.h
    @author Shai Levin & Nick Scadden
    @date   9 Oct 2019
    @brief  Contains maze arrays, which are stored in int arrays.
            A bitarray implementation may be more memory efficient but not neccesary
*/

#ifndef MAZE_H
#define MAZE_H

#include "system.h"

#define NUM_MAZES 2
#define MAX_MAZE_SIZE 25

#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3

/** Set current maze number and updates initial bitmap to be displayed by LED
    matrix.
    @param bitmap array of length 5 containing the LEDMAT columns to be displayed
    @param maze_number Sets the max maze number. Max number is NUM_MAZES  */
void set_maze(uint8_t bitmap[5], uint8_t maze_number);

/** Updates player position in maze and changes to new bitmap. If player tries to
    move into a wall then
    @param bitmap array of length 5 containing the LEDMAT columns to be displayed
    @param direction updates */
void update_maze(uint8_t bitmap[5], uint8_t dir);

/** Returns 1 if players position is at the edge of the maze.  */
uint8_t has_won(void);

#endif
