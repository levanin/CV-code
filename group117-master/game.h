/** @file   game.h
    @author Shai Levin & Nick Scadden
    @date   9 Oct 2019
    @brief  Race to solve a maze, first player to solve 3 to win

*/

#ifndef GAME_H
#define GAME_H

/*Prints the score*/
char* print_score(void);
/*Displays the end of game screen*/
void end_screen(void);
/*Displays the score screen*/
void score_screen(void);
/*Displays the play screen*/
void play_screen(void);
/*Displays the start screen*/
void start_screen(void);
/*The main body of the game*/
int main (void);

#endif
