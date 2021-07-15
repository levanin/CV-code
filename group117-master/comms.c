/** @file   comms.c
    @author Shai Levin & Nick Scadden
    @date   9 Oct 2019
    @brief  Communication module for game which checks whether either player has
            won the round. If a player wins, it prompts a move onto the score
            screen.
*/

#include "ir_uart.h"

/* Initialise communication module ir_uart*/
void comms_init(void)
{

    ir_uart_init ();

}

/* Transmit a victory when the game is won by a player */
void comms_win(void)

{

    ir_uart_putc ('W');

}

/* Returns 1 if the other player has transmitted a victory, otherwise 0 */
bool has_lost(void)

{
char character;
character = ir_uart_getc();
if (character == 'W') {
  return 1;
} else {
  return 0;
}
}

/* Transmit a start request to other player */
void comms_send_start(void)

{

    ir_uart_putc ('S');

}

/* Start request handshake */
bool comms_waiting_start(void)
{
    char character;
    character = ir_uart_getc ();
    if (character == 'S') {
        ir_uart_putc('G');
        return true;}
        else if (character == 'G') {
            return true;
        } else {
            return false;
        }
    }

/* Returns 1 when comms are ready to write */
bool comms_write_ready(void)
{
    return ir_uart_write_ready_p ();
}
