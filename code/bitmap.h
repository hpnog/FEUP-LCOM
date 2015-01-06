#pragma once

/** @defgroup Bitmap Bitmap
 * @{
 * Functions for manipulating bitmaps - based on code provided by Henrique Ferrolho
 */

#include "interface.h"

typedef enum {
    ALIGN_LEFT, ALIGN_CENTER, ALIGN_RIGHT
} Alignment;

typedef struct {
    unsigned short type; // specifies the file type
    unsigned int size; // specifies the size in bytes of the bitmap file
    unsigned int reserved; // reserved; must be 0
    unsigned int offset; // specifies the offset in bytes from the bitmapfileheader to the bitmap bits
} __attribute__((packed)) BitmapFileHeader;

typedef struct {
    unsigned int size; // specifies the number of bytes required by the struct
    int width; // specifies width in pixels
    int height; // specifies height in pixels
    unsigned short planes; // specifies the number of color planes, must be 1
    unsigned short bits; // specifies the number of bit per pixel
    unsigned int compression; // specifies the type of compression
    unsigned int imageSize; // size of image in bytes
    int xResolution; // number of pixels per meter in x axis
    int yResolution; // number of pixels per meter in y axis
    unsigned int nColors; // number of colors used by the bitmap
    unsigned int importantColors; // number of colors that are important
} __attribute__((packed)) BitmapInfoHeader;

typedef struct {
    BitmapInfoHeader bitmapInfoHeader;
    unsigned char* bitmapData;
} Bitmap;

typedef struct {
	Bitmap* welcome;
	Bitmap* spaceToContinue;
	Bitmap* arrow;
	Bitmap* arrowCleaner;
	Bitmap* grid;
	Bitmap* pecamarela;
	Bitmap* pecavermelha;
	Bitmap* gamearrow;
	Bitmap* gamearrowCleaner;
	Bitmap* pieceCleaner;
	Bitmap* blank;
	Bitmap* play;
	Bitmap* exit_button;
	Bitmap* options;
	Bitmap* credits;
	Bitmap* progressBar;
	Bitmap* progressPart;
	Bitmap* zero;
	Bitmap* um;
	Bitmap* dois;
	Bitmap* tres;
	Bitmap* quatro;
	Bitmap* cinco;
	Bitmap* seis;
	Bitmap* sete;
	Bitmap* oito;
	Bitmap* nove;
	Bitmap* doispontos;
	Bitmap* slash;
	Bitmap* monday;
	Bitmap* tuesday;
	Bitmap* wednesday;
	Bitmap* thursday;
	Bitmap* friday;
	Bitmap* saturday;
	Bitmap* sunday;
	Bitmap* maintitle;
	Bitmap* credits_screen;
	Bitmap* player1;
	Bitmap* player2;
	Bitmap* secondTitle;
	Bitmap* p1Wins;
	Bitmap* p2Wins;
	Bitmap* tie;
} bitmap_struct;


/**
 * @brief Loads a bmp image - code provided by Henrique Ferrolho
 *
 * @param filename Path of the image to load
 * @return Non NULL pointer to the image buffer
 */
Bitmap* loadBitmap(const char* filename);

/**
 * @brief Draws an unscaled, unrotated bitmap at the given position - Code provided by Henrique Ferrolho but with changes to be able to draw pixel by pixel allowing transparency
 *
 * @param bitmap bitmap to be drawn
 * @param x destiny x coord
 * @param y destiny y coord
 * @param alignment image alignment
 */
void drawBitmap(Bitmap* bitmap, int x, int y, Alignment alignment);

/**
 * @brief Draws an unscaled, unrotated bitmap at the given position - Code provided by Henrique Ferrolho
 *
 * @param bitmap bitmap to be drawn
 * @param x destiny x coord
 * @param y destiny y coord
 * @param alignment image alignment
 */
void drawBitmap_by_line(Bitmap* bitmap, int x, int y, Alignment alignment);

/**
 * @brief Draws an unscaled, unrotated bitmap at the given position in buffer - Code provided by Henrique Ferrolho but with changes to be able to draw pixel by pixel allowing transparency
 *
 * @param bitmap bitmap to be drawn
 * @param x destiny x coord
 * @param y destiny y coord
 * @param alignment image alignment
 */
void drawBitmap_inBuff(Bitmap* bitmap, int x, int y, Alignment alignment, unsigned char **double_buffer);

/**
 * @brief Draws an unscaled, unrotated bitmap at the given position in buffer - Code provided by Henrique Ferrolho
 *
 * @param bitmap bitmap to be drawn
 * @param x destiny x coord
 * @param y destiny y coord
 * @param alignment image alignment
 */
void drawBitmap_by_line_inBuff(Bitmap* bitmap, int x, int y, Alignment alignment, unsigned char **double_buffer);

/**
 * @brief Reads all bitmaps for universe theme
 */
void readAllBitmaps_universe();

/**
 * @brief Reads all bitmaps for pokemon theme
 */
void readAllBitmaps_pokemon();

/**
 * @brief Draws the bitmap on its name
 */
void draw_player1();

/**
 * @brief Draws the bitmap on its name
 */
void draw_player2();

/**
 * @brief Draws the bitmap on its name
 */
void draw_tie();

/**
 * @brief Draws the bitmap on its name
 */
void draw_p1Wins();

/**
 * @brief Draws the bitmap on its name
 */
void draw_p2Wins();

/**
 * @brief Draws the bitmap on its name
 */
void draw_creditsScreen();

/**
 * @brief Draws the bitmap on its name
 */
void draw_secondTitle();

/**
 * @brief Draws the bitmap on its name
 */
void draw_options();


/**
 * @brief Draws the bitmap on its name
 */
void draw_mouse();

/**
 * @brief Draws the bitmap on its name
 */
void draw_welcome();

/**
 * @brief Draws the bitmap on its name
 */
void draw_spaceToContinue();

/**
 * @brief Draws the bitmap on its name
 */
void draw_menu_symblos();

/**
 * @brief Draws the bitmap on its name
 *
 * @param x coordinate of the left top corner
 * @param y coordinate of the left top corner
 */
void draw_arrow(int x, int y);

/**
 * @brief Draws the bitmap on its name
 *
 * @param x coordinate of the left top corner
 * @param y coordinate of the left top corner
 */
void draw_arrowCleaner(int x, int y);

/**
 * @brief Draws the bitmap on its name
 */
void draw_grid();

/**
 * @brief Draws the bitmap on its name
 *
 * @param **double_buffer pointer to double_buffer
 */
void draw_grid_inBuf(unsigned char **double_buffer);

/**
 * @brief Draws the bitmap on its name
 *
 * @param x coordinate of the left top corner
 * @param y coordinate of the left top corner
 * @param **double_buffer pointer to double_buffer
 */
void draw_peca_amarela(int x, int y, unsigned char **double_buffer);

/**
 * @brief Draws the bitmap on its name
 *
 * @param x coordinate of the left top corner
 * @param y coordinate of the left top corner
 * @param **double_buffer pointer to double_buffer
 */
void draw_peca_vermelha(int x, int y, unsigned char **double_buffer);

/**
 * @brief Draws the bitmap on its name
 *
 * @param col coolumn where the arrow is to be drawn
 */
void draw_game_arrow(int col);

/**
 * @brief Lowers the bitmap on its name
 *
 * @param empty_spaces number of empty spaces on chosem column
 * @param col coolumn where the arrow is to be drawn
 * @param**double_buffer pointer to double_buffer
 */
void low_red_piece(int empty_spaces, int col, unsigned char **double_buffer);

/**
 * @brief Lowers the bitmap on its name
 *
 * @param empty_spaces number of empty spaces on chosem column
 * @param col coolumn where the arrow is to be drawn
 * @param**double_buffer pointer to double_buffer
 */
void low_yellow_piece(int empty_spaces, int col, unsigned char **double_buffer);

/**
 * @brief Draws the bitmap on its name
 *
 * @param x coordinate of the left top corner
 * @param y coordinate of the left top corner
 * @param**double_buffer pointer to double_buffer
 */
void draw_peca_cleaner(int x, int y, unsigned char **double_buffer);

/**
 * @brief Draws the bitmap on its name
 *
 * @param xArrowPos coordinate of the arrow
 * @param yArrowPos coordinate of the arrow
 */
void draw_options_arrow(int xArrowPos,int yArrowPos);

/**
 * @brief Draws the bitmap on its name
 */
void draw_progressBar();

/**
 * @brief Draws the bitmap on its name
 *
 * @param n_secs numer of seconds that have passed
 */
void draw_progressBar_15sec(int n_secs);

/**
 * @brief Draws the actual date on screen using the bitmaps of the numbers
 */
void draw_date();

/**
 * @brief Draws the bitmap on its name
 *
 * @param x coordinate of the left top corner
 * @param y coordinate of the left top corner
 */
void draw_0(int x, int y);

/**
 * @brief Draws the bitmap on its name
 *
 * @param x coordinate of the left top corner
 * @param y coordinate of the left top corner
 */
void draw_1(int x, int y);

/**
 * @brief Draws the bitmap on its name
 *
 * @param x coordinate of the left top corner
 * @param y coordinate of the left top corner
 */
void draw_2(int x, int y);

/**
 * @brief Draws the bitmap on its name
 *
 * @param x coordinate of the left top corner
 * @param y coordinate of the left top corner
 */
void draw_3(int x, int y);

/**
 * @brief Draws the bitmap on its name
 *
 * @param x coordinate of the left top corner
 * @param y coordinate of the left top corner
 */
void draw_4(int x, int y);

/**
 * @brief Draws the bitmap on its name
 *
 * @param x coordinate of the left top corner
 * @param y coordinate of the left top corner
 */
void draw_5(int x, int y);

/**
 * @brief Draws the bitmap on its name
 *
 * @param x coordinate of the left top corner
 * @param y coordinate of the left top corner
 */
void draw_6(int x, int y);

/**
 * @brief Draws the bitmap on its name
 *
 * @param x coordinate of the left top corner
 * @param y coordinate of the left top corner
 */
void draw_7(int x, int y);

/**
 * @brief Draws the bitmap on its name
 *
 * @param x coordinate of the left top corner
 * @param y coordinate of the left top corner
 */
void draw_8(int x, int y);

/**
 * @brief Draws the bitmap on its name
 *
 * @param x coordinate of the left top corner
 * @param y coordinate of the left top corner
 */
void draw_9(int x, int y);

/**
 * @brief Draws the bitmap on its name
 */
void draw_doispontos();

/**
 * @brief Draws the bitmap on its name
 *
 * @param x coordinate of the top left corner of the bitmap
 */
void draw_slash(int x);

/**
 * @brief Draws day of the week on screen
 */
void draw_monday();

/**
 * @brief Draws day of the week on screen
 */
void draw_tuesday();

/**
 * @brief Draws day of the week on screen
 */
void draw_wednesday();

/**
 * @brief Draws day of the week on screen
 */
void draw_thursday();

/**
 * @brief Draws day of the week on screen
 */
void draw_friday();

/**
 * @brief Draws day of the week on screen
 */
void draw_saturday();

/**
 * @brief Draws day of the week on screen
 */
void draw_sunday();

/**
 * @brief Draws year on screen
 *
 * @param actual year
 */
void draw_year(int year);

/**
 * @brief Draws day on screen
 *
 * @param actual day
 */
void draw_day(int day);

/**
 * @brief Draws hour on screen
 *
 * @param actual hour
 */
void draw_hour(int hour);

/**
 * @brief Draws minute on screen
 *
 * @param actual minute
 */
void draw_minute(int minute);


/**@}*/
