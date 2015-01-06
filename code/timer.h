#ifndef __TIMER_H
#define __TIMER_H

#include <minix/syslib.h>
#include <minix/drivers.h>
#include "interface.h"

int clock_time;

/** @defgroup timer timer
 * @{
 *
 * Functions for using the i8254 timers
 */

/**
 * @brief Subscribes the timer
 *
 * @return returns BIT(hook_id)
 */
int timer_subscribe_int(void );

/**
 * @brief Unsubscribes the timer
 */
int timer_unsubscribe_int();

/**
 * @brief Handles the timers interrupts
 */
void timer_int_handler();

/**
 * @brief Waits for "time" seons before ending
 *
 * @param time the time this function waits
 *
 * @return 0
 */
int timer_wait_int(unsigned long time);

/**@}*/

#endif /* __TIMER_H */
