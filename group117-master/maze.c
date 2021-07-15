/** @file   maze.c
    @author Shai Levin & Nick Scadden
    @date   9 Oct 2019
    @brief  Contains maze arrays, which are stored in int arrays.
            A bitarray implementation may be more memory efficient but not neccesary
*/
#include "system.h"
#include "maze.h"

/** Global vars declaration.
    Note we store the mazes in arrays where 1 is a wall, 0 is a movable space,
    and 2 is an exit. */
static const uint8_t mazes[NUM_MAZES][MAX_MAZE_SIZE][MAX_MAZE_SIZE] = {{
        {   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1   },
        {   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1   },
        {   1,   1,   1,   0,   0,   0,   0,   0,   0,   1,   1,   1   },
        {   1,   1,   1,   0,   1,   1,   1,   1,   0,   1,   1,   1   },
        {   1,   1,   1,   0,   0,   0,   0,   1,   0,   1,   1,   1   },
        {   1,   1,   1,   1,   0,   1,   0,   1,   0,   1,   1,   1   },
        {   1,   1,   1,   1,   1,   1,   0,   1,   0,   1,   1,   1   },
        {   1,   1,   1,   0,   0,   0,   0,   1,   0,   1,   1,   1   },
        {   1,   1,   1,   0,   1,   1,   1,   1,   0,   1,   1,   1   },
        {   1,   1,   1,   1,   1,   0,   0,   0,   0,   1,   1,   1   },
        {   1,   1,   1,   0,   0,   0,   0,   0,   0,   1,   1,   1   },
        {   1,   1,   1,   0,   1,   1,   1,   1,   1,   1,   1,   1   },
        {   1,   1,   1,   0,   0,   0,   0,   1,   2,   0,   0,   0   },
        {   1,   1,   1,   0,   1,   1,   0,   1,   0,   1,   1,   1   },
        {   1,   1,   1,   0,   0,   1,   0,   0,   0,   1,   1,   1   },
        {   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1   },
        {   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1   }
    },
    {
        {   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1   },
        {   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1   },
        {   1,   1,   1,   0,   0,   0,   0,   0,   1,   0,   0,   0,   0,   1,   0,   0,   0,   0,   0,   0,   1,   1,   1   },
        {   1,   1,   1,   1,   1,   1,   1,   0,   1,   0,   1,   1,   0,   0,   0,   1,   1,   1,   1,   0,   1,   1,   1   },
        {   1,   1,   1,   0,   0,   0,   0,   0,   1,   0,   1,   0,   1,   1,   1,   1,   0,   0,   0,   0,   1,   1,   1   },
        {   1,   1,   1,   0,   1,   0,   1,   0,   1,   0,   1,   0,   1,   0,   0,   0,   0,   1,   1,   1,   1,   1,   1   },
        {   1,   1,   1,   0,   1,   0,   1,   0,   0,   0,   0,   0,   0,   0,   1,   0,   0,   1,   0,   0,   1,   1,   1   },
        {   1,   1,   1,   0,   1,   0,   1,   1,   1,   1,   1,   0,   1,   1,   1,   0,   1,   1,   1,   0,   1,   1,   1   },
        {   1,   1,   1,   0,   1,   0,   1,   0,   0,   0,   1,   0,   0,   0,   1,   0,   1,   0,   0,   0,   1,   1,   1   },
        {   1,   1,   1,   0,   1,   0,   1,   0,   1,   0,   1,   1,   1,   0,   1,   0,   1,   0,   1,   1,   1,   1,   1   },
        {   1,   1,   1,   0,   1,   0,   0,   0,   1,   0,   1,   0,   0,   0,   1,   0,   0,   0,   1,   2,   0,   0,   0   },
        {   1,   1,   1,   0,   1,   0,   1,   0,   1,   0,   1,   0,   1,   1,   1,   0,   1,   1,   1,   0,   1,   1,   1   },
        {   1,   1,   1,   0,   1,   0,   1,   0,   0,   0,   1,   0,   0,   1,   0,   0,   0,   0,   1,   0,   1,   1,   1   },
        {   1,   1,   1,   0,   1,   0,   1,   1,   1,   1,   1,   1,   0,   1,   0,   1,   1,   1,   1,   0,   1,   1,   1   },
        {   1,   1,   1,   0,   0,   0,   0,   0,   0,   0,   0,   1,   0,   1,   0,   0,   0,   0,   0,   0,   1,   1,   1   },
        {   1,   1,   1,   1,   1,   1,   1,   1,   0,   1,   0,   1,   0,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1   },
        {   1,   1,   1,   0,   0,   0,   0,   0,   0,   1,   0,   1,   0,   0,   0,   0,   0,   0,   0,   0,   1,   1,   1   },
        {   1,   1,   1,   0,   1,   1,   1,   1,   1,   1,   0,   1,   1,   1,   1,   1,   1,   1,   1,   0,   1,   1,   1   },
        {   1,   1,   1,   0,   0,   0,   0,   0,   0,   1,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   1,   1,   1   },
        {   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1   },
        {   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1   }
    }
};

static uint8_t current_maze = 0;
static uint8_t lpositionx = 0;
static uint8_t lpositiony = 0;


/** Get the current bitmap of the position in the maze to be displayed by the
    LED matrix.
    @param bitmap array of length 5 containing the LEDMAT columns to be displayed  */
static void update_bitmap(uint8_t bitmap[])
{
    uint8_t sum = 0;
    for (uint8_t row = 0; row < 5; row++) {
        for (uint8_t col = 0; col < 7; col++) {
            if (mazes[current_maze][lpositiony+row][lpositionx+col] == 1) {
                sum = sum + (mazes[current_maze][lpositiony+row][lpositionx+col] << (6-col));
            }
        }
        bitmap[row] = sum;
        sum = 0;
    }
}

/** Set current maze number and updates initial bitmap to be displayed by LED
    matrix.
    @param bitmap array of length 5 containing the LEDMAT columns to be displayed
    @param maze_number Sets the max maze number. Max number is NUM_MAZES  */
void set_maze(uint8_t bitmap[], uint8_t maze_number)
{
    current_maze = maze_number;
    lpositionx = 0;
    lpositiony = 0;
    update_bitmap(bitmap);
}

/** Updates player position in maze and changes to new bitmap. If player tries to
    move into a wall then
    @param bitmap array of length 5 containing the LEDMAT columns to be displayed
    @param direction updates */
void update_maze(uint8_t bitmap[], uint8_t dir)
{
    uint8_t playerposx = lpositionx+3;
    uint8_t playerposy = lpositiony+2;
    if (dir == UP && mazes[current_maze][playerposy-1][playerposx] != 1) {
        lpositiony--;
    } else if (dir == DOWN && mazes[current_maze][playerposy+1][playerposx] != 1) {
        lpositiony++;
    } else  if (dir == LEFT && mazes[current_maze][playerposy][playerposx-1] != 1) {
        lpositionx--;
    } else if (dir == RIGHT && mazes[current_maze][playerposy][playerposx+1] != 1) {
        lpositionx++;
    }
    update_bitmap(bitmap);
}

/** Returns 1 if players position is at the edge of the maze.  */
uint8_t has_won(void)
{
    uint8_t playerposx = lpositionx+3;
    uint8_t playerposy = lpositiony+2;
    if (mazes[current_maze][playerposy][playerposx] == 2) {
        return 1;
    }
    return 0;
}
