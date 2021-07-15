/** @file comms.h
    @author Shai Levin & Nick Scadden
    @date   14 Oct 2019
    @brief  Header file for the communication module

*/

#ifndef COMMS_H
#define COMMS_H


/* Initialises communications module for use.*/
void comms_init(void);


/* Sending signal when player reaches the win condition*/
void comms_win(void);


/* Send signal for starting maze */
void comms_send_start(void);

/* Sends response for losing maze*/
bool has_lost(void);

/* Use when ready to receive signal*/
bool comms_write_ready(void);

/* Handshake after receiving start signal*/
bool comms_waiting_start(void);




#endif
