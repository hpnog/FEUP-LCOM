#ifndef __TEST4_H
#define __TEST4_H

#include <minix/drivers.h>
#include <minix/syslib.h>
#include <minix/sysutil.h>
#include "interface.h"

typedef struct {
	short packets[3];
	int handlerCount;
	int x, y, x_final, y_final, left_button;
} mouse_struct;

mouse_struct mouse;

/** @defgroup mouse mouse
 * @{
 *
 * Functions to control the mouse
 */

/**
 * @brief Subscribees the mouse
 *
 * @return returns BIT(hook_id)
 */
int mouse_subscribe_int();

/**
 * @brief Unsubscribees the mouse
 *
 * @return returns BIT(hook_id)
 */
int mouse_unsubscribe_int();

/**
 * @brief Activates the correct stream mode of the mouse
 *
 * @return returns 0
 */
int activateMouse();

/**
 * @brief To handle the interrupts of the mouse
 *
 * @return return the data
 */
unsigned short mouse_int_handler();

/**
 * @brief Print the info in the packets
 */
void printMoves();

/**
 * @brief To issue a command to the mouse controller
 *
 * @param dest is the destination register
 * @param com is the command to send to the previous register
 * @param *res is the variable where to put the result of the command
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int issueCommand(unsigned long dest, unsigned long com, unsigned long *res);

/**
 * @brief To issue a command to the mouse controller
 *
 * @param dest is the destination register
 * @param com is the command to send to the previous register
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int issueMouseCommand(unsigned long dest, unsigned long com);

/** 
 * @brief To test packet reception via interrupts 
 * 
 * Displays the packets received from the mouse
 * Exits after receiving the number of packets received in argument
 * 
 * @param cnt Number of packets to receive and display before exiting
 * 
 * @return Return 0 upon success and non-zero otherwise
 */
int test_packet(unsigned short cnt);

/**
 * @brief To empty the Out_buffer
 */
void emptyBuffer();

/** 
 * @brief Checks if the buffer is full
 * 
 * @return returns if it is or not full
 */
int bufferFull();

/**@}*/

#endif
