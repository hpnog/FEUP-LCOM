#ifndef __KEYBOARD_H
#define __KEYBOARD_H

#include <minix/drivers.h>
#include <minix/syslib.h>
#include <minix/sysutil.h>

#include "interface.h"


/** @defgroup keyboard keyboard
 * @{
 *
 * Functions for controling the keyboard
 */


/** 
 * @brief Waits for the user to press Space-bar
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int waitForSpaceBar();

/**
 * @brief Subscribes the keyboard
 *
 * @return BIT(hook_id) do keyboard
 */
int kbd_subscribe_int();

/**
 * @brief Unsubscribes the keyboard
 *
 * @return 0 on success
 */
int kbs_unsubscribe_int();

/**
 * @brief Prints the info received from the keyboard
 *
 * @param data is the "raw" info received from the keyboard
 *
 * @return the same info it interpreted
 */
long convert_data(long data);

/**
 * @brief Handles the interrupions to the keyboard
 *
 * @param irq_set
 * @param msg
 * @param ipc_status
 *
 * @return returns the info received from the keyboard
 */
long interrupt_handler(int irq_set, message msg, int ipc_status);

/**
 * @brief Handles the interrupions to the keyboard, to the mouse and to the timer
 *
 * @param irq_set
 * @param msg
 * @param ipc_status
 * @param mouse_irq_set
 * @param timer_irq_set
 *
 * @return returns the info received from the keyboard
 */
long interrupt_handler_mouse_kbd(int irq_set, message msg, int ipc_status, int mouse_irq_set, int timer_irq_set);

/**
 * @brief Reads status from keyboard
 * 
 * @param stat
 * 
 * @return a character with the respectivee interpretation
 */
char read_status_kbd(long stat);

/**
 * @brief Issues a command to the keyboard
 *
 * @param stat actual state of the keyboard
 * @param cmd command to give to the keyboard
 *
 * @return a char depending on the outcome
 */
char issue_command_kbd(long stat, long cmd);

/**
 * @brief Selects column of the game grid
 *
 * This function is used actually during the game itself to allow thee user to choose
 * in which coumn does he want to place his piece
 *
 * @return the number of the chosen column
 */
int kbd_select_col();

/**
 * @brief Waits for the user to press an arrow or enter
 *
 * @return a char to symbolize wwhich key was pressed
 */
char waitForArrowsMenu();

/**
 * @brief Waits for the user to press an arrow or enter
 *
 * @return a char to symbolize wwhich key was pressed
 */
char waitForArrowsOptions();

/**@}*/

#endif /* __KEYBOARD_H */
