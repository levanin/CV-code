/** @file   game.c
    @author Shai Levin & Nick Scadden
    @date   9 Oct 2019
    @brief  Race to solve a maze, first player to solve 3 to win

*/

#include "system.h"
#include "pacer.h"
#include "tinygl.h"
#include "ledmat.h"
#include "navswitch.h"
#include "maze.h"
#include "comms.h"
#include "game.h"
#include "font3x5_1.h"


#define PACER_RATE 500
#define MESSAGE_RATE 10

/** Global vars declaration */
uint8_t score = 0;
uint8_t opscore = 0;
uint8_t mazenum = 0;

/* Print string with score and opscore */
char* print_score(void)
{
    char* out = "CURRENT SCORE 0:0";
    char charscore = '0' + score;
    char charopscore = '0' + opscore;
    *(out+14) = charscore;
    *(out+16) = charopscore;
    return out;
}

/* End screen which displays winner or loser. */
void end_screen(void)
{
    if (score > opscore) {
        tinygl_text("YOU WIN!");
    } else if (score < opscore) {
        tinygl_text("YOU LOSE!");
    } else {
        tinygl_text("DRAW!");
    }

    uint16_t count = 0;
    while (1) {
        pacer_wait();

        tinygl_update();

        if (count > 5000) {
            break;
        }
        count++;
    }
    start_screen();
}

/** Score screen which displays current scores in between play rounds */
void score_screen(void)
{
    if (mazenum == NUM_MAZES) {
        end_screen();
    } else {

        tinygl_text(print_score());
        uint16_t count = 0;

        while (1) {
            pacer_wait();
            count++;
            tinygl_update();
            if (count > 5000) {
                break;
            }
        }
        play_screen();
    }
}

/** Play screen which navigates the maze using functions and the mazes in maze.c */
void play_screen(void)
{
    uint8_t current_column = 0;
    uint8_t bitmap[5];
    set_maze(bitmap, mazenum);
    while(1) {
        pacer_wait();

        navswitch_update();
        if (navswitch_push_event_p (NAVSWITCH_NORTH)) {
            update_maze(bitmap, RIGHT);
        }
        if (navswitch_push_event_p (NAVSWITCH_SOUTH)) {
            update_maze(bitmap, LEFT);
        }
        if (navswitch_push_event_p (NAVSWITCH_EAST)) {
            update_maze(bitmap, DOWN);
        }
        if (navswitch_push_event_p (NAVSWITCH_WEST)) {
            update_maze(bitmap, UP);
        }

        ledmat_display_column(bitmap[current_column], current_column);
        current_column++;

        if (current_column > (LEDMAT_COLS_NUM - 1)) {
            current_column = 0;
        }




        if (has_won()) {
            score++;
            comms_win();
            break;
        }
        if (has_lost()) { ///implement has_lost func in comms.c

            opscore++;
            break;
        }
    }
    mazenum++;
    score_screen();
}

/** Start screen which waits till both players are ready to begin game */
void start_screen(void)
{
    while(1) {
        pacer_wait ();
        if (navswitch_push_event_p (NAVSWITCH_PUSH)) {
            comms_send_start();
        }
        if (comms_write_ready()) {
            if (comms_waiting_start()) {
                break;
            }
        }
    }
    play_screen();
}

/* Main Function which initialises drivers and runs start screen. */
int main (void)
{
    // initialise drivers
    system_init ();
    pacer_init(PACER_RATE);
    ledmat_init();
    navswitch_init();
    tinygl_init (PACER_RATE);
    tinygl_font_set (&font3x5_1);
    tinygl_text_speed_set (MESSAGE_RATE);
    start_screen();

}
