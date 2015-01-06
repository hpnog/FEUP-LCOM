#ifndef __VIDEO_GR_H
#define __VIDEO_GR_H

#include "interface.h"

/** @defgroup video_gr video_gr
 * @{
 *
 * Functions for outputing data to screen in graphics mode
 */

#define VRAM_PHYS_ADDR		0xF0000000
#define H_RES             	1024
#define V_RES		  		768
#define BITS_PER_PIXEL		16

static char *video_mem;		/* Process address to which VRAM is mapped */

/**
 * @brief Initializes the video module in graphics mode
 * 
 * Uses the VBE INT 0x10 interface to set the desired
 *  graphics mode, maps VRAM to the process' address space and
 *  initializes static global variables with the resolution of the screen, 
 *  and the number of colors
 * 
 * @param mode 16-bit VBE mode to set
 * @return Virtual address VRAM was mapped to. NULL, upon failure.
 */
void *vg_init(unsigned short mode);

/**
 * @brief Gets the Address of video_mem
 *
 * @return the address
 */
void * getAddr();

 /**
 * @brief Returns to default Minix 3 text mode (0x03: 25 x 80, 16 colors)
 * 
 * @return 0 upon success, non-zero upon failure
 */
int vg_exit(void);

/**
 * @brief Colors one pixel
 *
 * @param x coordinate of the pixel to color
 * @param y coordinate of the pixel to color
 * @color color to put on pixel
 */
void colorPix(unsigned int x, unsigned int y, char color);

/**
 * @brief Checks if the coordinates are within the screen
 *
 * @param xi
 * @param xf
 * @param yi
 * @param yf
 *
 * @return 0 if true and non-zero if false
 */
int checkScreen(unsigned short xi, unsigned short xf, unsigned short yi, unsigned short yf);
 
/**@}*/

#endif /* __VIDEO_GR_H */
