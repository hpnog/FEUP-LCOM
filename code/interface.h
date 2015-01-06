#ifndef __INTERFACE_H
#define __INTERFACE_H

/** @defgroup Interface Interface
 * @{
 * Funcions to inicialize video mode
 */

#include <minix/drivers.h>
#include <minix/syslib.h>
#include <minix/sysutil.h>

#include "timer.h"
#include "vbe.h"
#include "video_gr.h"
#include "lmlib.h"
#include "bitmap.h"
#include "stdio.h"
#include "mouse.h"
#include "i8042.h"
#include "game.h"
#include "i8254.h"
#include "keyboardMacros.h"
#include "rtc.h"

#define SQUARE_MODE 0x105
#define GAME_MODE	0x117

/**
 * @brief Initializes the sreen mode
 */
void video_initialize();

/**
 * @brief Shows the first screen when the program is first ran
 */
void welcome_screen();

/**
 * @brief Shows and controls the main menu
 * @param **double_buffer
 *
 * @return 0 when finished
 */
int main_menu(unsigned char **double_buffer);

/**
 * @brief Shows the options menu and waits for the theme option of the user
 */
void options_menu();

/**
 * @brief Shows and controls the credits menu
 */
void credits_menu();

/**@}*/

#endif /* __INTERFACE_H */
