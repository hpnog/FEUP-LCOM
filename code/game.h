#ifndef __GAME_H
#define __GAME_H

/** @defgroup Game Game
 * @{
 * Main functions of the game itself - has the game structure and manages the pieces on the games grid
 */


#include "interface.h"

typedef struct {
	int timer_irq;
	int kbd_irq;
	int mouse_irq;
	int rtc_irq;
	int clock_time;
	int sec;
	int grid[7][6];
} game_struct;

game_struct game;

/**
 * @brief Manages the main game and each of the players moves
 *
 * @param **double_buffer double buffer as parameter
 */
void startGame(unsigned char **double_buffer);

/**
 * @brief Checks the grid in the struct game for the number of empty spaces in the column col
 *
 * @param col number of the column
 * @return number of empty spaces in the column col
 */
int empty_spaces_in_col(int col);

/**
 * @brief Inserts the piece of player 1 on grid and dispays it
 *
 * @param col number of the column
 * @param empty number of empty spaces on column
 */
void insert_red_on_grid(int col, int empty);

/**
 * @brief Inserts the piece of player 2 on grid and dispays it
 *
 * @param col number of the column
 * @param empty number of empty spaces on column
 */
void insert_yellow_on_grid(int col, int empty);

/**
 * @brief Checks if the game ended or not
 *
 * @return number depeending on game. If player 1 won, returns 1, if player 2 won returns 2, else returns 0
 */
int check_if_ended();

/**
 * @brief Checks if the game ended or not (in the column)
 *
 * @return number depeending on game. If player 1 won, returns 1, if player 2 won returns 2, else returns 0
 */
int check_if_ended_by_column();

/**
 * @brief Checks if the game ended or not (in the line)
 *
 * @return number depeending on game. If player 1 won, returns 1, if player 2 won returns 2, else returns 0
 */
int check_if_ended_by_line();

/**
 * @brief Checks if the game ended or not (in one diagonal)
 *
 * @return number depeending on game. If player 1 won, returns 1, if player 2 won returns 2, else returns 0
 */
int check_if_ended_by_diagonal_left();

/**
 * @brief Checks if the game ended or not (in one diagonal)
 *
 * @return number depeending on game. If player 1 won, returns 1, if player 2 won returns 2, else returns 0
 */
int check_if_ended_by_diagonal_right();

/**
 * @brief Checks if the game ended or not (in one diagonal)
 *
 * @return number depeending on game. If it was a draw returns 3, else returns 0
 */
int checkIfTie();

/**@}*/

#endif /* __GAME_H */
