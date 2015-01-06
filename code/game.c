#include "game.h"


void startGame(unsigned char **double_buffer) {

	draw_welcome();
	draw_secondTitle();
	draw_grid();

	unsigned int i;
	unsigned int j;
	for (i = 0; i < 7; i++)
		for (j = 0; j < 6; j++)
			game.grid[i][j] = 0;

	int winner = 0;
	int check;

	while(winner == 0){
		do {
			//----------Player-1-plays--------------
			game.sec = 0;
			game.clock_time = 0;
			draw_progressBar();
			draw_player1();
			int turn_col = kbd_select_col();
			if (turn_col == -1)
				return;
			int empty_spaces = empty_spaces_in_col(turn_col);


			if (turn_col == 99)
				break;
			/*
			 * Next cycle checks if column is empty
			 */

			check = 0;
			if (empty_spaces != 0) {
				insert_red_on_grid(turn_col, empty_spaces);
				memcpy(*double_buffer, getAddr(), 1024*768*2);
				low_red_piece(empty_spaces, turn_col, double_buffer);
				check = 1;
			}
			else {
				memcpy(*double_buffer, getAddr(), 1024*768*2);
				break;
			}
		} while (check == 0);

		winner = check_if_ended();
		if (winner != 0)
			break;

		//----------Player-1-ends--------------
		//___________________________________________________________________________________________
		//
		//----------Player-2-plays--------------
		do {
			draw_progressBar();
			draw_player2();
			game.sec = 0;
			game.clock_time = 0;
			int turn_col = kbd_select_col();
			if (turn_col == -1)
				return;
			int empty_spaces = empty_spaces_in_col(turn_col);

			if (turn_col == 99)
				break;

			/*
			 * Next cycle checks if column is empty
			 */
			check = 0;
			if (empty_spaces != 0) {
				insert_yellow_on_grid(turn_col, empty_spaces);
				memcpy(*double_buffer, getAddr(), 1024*768*2);
				low_yellow_piece(empty_spaces, turn_col, double_buffer);
				check = 1;
			}
			else {
				memcpy(*double_buffer, getAddr(), 1024*768*2);
				break;
			}
		} while (check == 0);


		winner = check_if_ended();
		if (winner != 0)
			break;

		//----------Player-2-ends--------------

	}

	if (winner == 1)
		draw_p1Wins();
	else if (winner == 2)
		draw_p2Wins();
	else
		draw_tie();

	waitForSpaceBar();

	return;
}

int empty_spaces_in_col(int col) {
	int empty = 6;
	int i = 0;
	for (i; i < 6; i++) {
		if (game.grid[col-1][i] != 0)
			empty--;
		else
			break;
	}
	return empty;
}

void insert_red_on_grid(int col, int empty) {
	game.grid[col - 1][6 - empty] = 1;
}

void insert_yellow_on_grid(int col, int empty) {
	game.grid[col - 1][6 - empty] = 2;
}

int check_if_ended() {
	int winner;
	winner = check_if_ended_by_column();
	if (winner != 0)
		return winner;
	winner = check_if_ended_by_line();
	if (winner != 0)
		return winner;
	winner = check_if_ended_by_diagonal_left();
	if (winner != 0)
		return winner;
	if (winner != 0)
		return winner;
	winner = check_if_ended_by_diagonal_right();
	if (winner != 0)
		return winner;
	winner = checkIfTie();
	return winner;
}

int check_if_ended_by_column() {
	unsigned int i, j, counter;
	counter = 0;
	for (i = 0; i < 7; i++)
		for (j = 0; j < 3; j++) {
			if (game.grid[i][j] == 1)
				if (game.grid[i][j+1] == 1)
					if (game.grid[i][j+2] == 1)
						if (game.grid[i][j+3] == 1)
							return 1;
			if (game.grid[i][j] == 2)
				if (game.grid[i][j+1] == 2)
					if (game.grid[i][j+2] == 2)
						if (game.grid[i][j+3] == 2)
							return 2;
		}
	return 0;
}

int check_if_ended_by_line() {
	unsigned int i, j, counter;
	counter = 0;
	for (i = 0; i < 4; i++)
		for (j = 0; j < 6; j++) {
			if (game.grid[i][j] == 1)
				if (game.grid[i+1][j] == 1)
					if (game.grid[i+2][j] == 1)
						if (game.grid[i+3][j] == 1)
							return 1;
			if (game.grid[i][j] == 2)
				if (game.grid[i+1][j] == 2)
					if (game.grid[i+2][j] == 2)
						if (game.grid[i+3][j] == 2)
							return 2;
		}
	return 0;
}

int check_if_ended_by_diagonal_left() {
	unsigned int i, j, counter;
	counter = 0;
	for (i = 0; i < 4; i++)
		for (j = 0; j < 3; j++) {
			if (game.grid[i][j] == 1)
				if (game.grid[i+1][j+1] == 1)
					if (game.grid[i+2][j+2] == 1)
						if (game.grid[i+3][j+3] == 1)
							return 1;
			if (game.grid[i][j] == 2)
				if (game.grid[i+1][j+1] == 2)
					if (game.grid[i+2][j+2] == 2)
						if (game.grid[i+3][j+3] == 2)
							return 2;
		}
	return 0;
}

int check_if_ended_by_diagonal_right() {
	unsigned int i, j, counter;
	i = 3;
	counter = 0;
	for (i; i < 7; i++)
		for (j = 0; j < 3; j++) {
			if (game.grid[i][j] == 1)
				if (game.grid[i-1][j+1] == 1)
					if (game.grid[i-2][j+2] == 1)
						if (game.grid[i-3][j+3] == 1)
							return 1;
			if (game.grid[i][j] == 2)
				if (game.grid[i-1][j+1] == 2)
					if (game.grid[i-2][j+2] == 2)
						if (game.grid[i-3][j+3] == 2)
							return 2;
		}
	return 0;
}

int checkIfTie() {
	unsigned int i, j;
	for (i = 0; i < 7; i++)
		for (j = 0; j < 6; j++)
			if (game.grid[i][j] == 0)
				return 0;
	return 3;
}
