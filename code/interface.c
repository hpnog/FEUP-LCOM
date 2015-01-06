#include "interface.h"

int clock_time;

void video_initialize(){
	video_mem = vg_init(GAME_MODE);
}

void welcome_screen() {
	draw_welcome();
	draw_mainTitle();
	draw_spaceToContinue();
	waitForSpaceBar();
}

void options_menu() {

	int yArrowPos = 250;
	int xArrowPos = 350;
	char key;
	do {
		draw_options();
		draw_options_arrow(xArrowPos, yArrowPos);
		key = waitForArrowsOptions();
		if (key == 'R') {
			if (xArrowPos == 350)
				if (yArrowPos == 250) {
					yArrowPos = 450;
					draw_options_arrow(xArrowPos, yArrowPos);
				}
				else {
					yArrowPos = 250;
					draw_options_arrow(xArrowPos, yArrowPos);
				}
		}
		else if (key == 'L') {
			if (xArrowPos == 350)
				if (yArrowPos == 250) {
					yArrowPos = 450;
					draw_options_arrow(xArrowPos, yArrowPos);
				}
				else {
					yArrowPos = 250;
					draw_options_arrow(xArrowPos, yArrowPos);
				}
		}
	} while (key != 'E');

	if (xArrowPos == 350) {
		if (yArrowPos == 250)
			readAllBitmaps_pokemon();
		else
			readAllBitmaps_universe();
	}


	return;
}

void credits_menu() {
	draw_welcome();
	draw_creditsScreen();
	draw_spaceToContinue();
	waitForSpaceBar();
}

int main_menu(unsigned char **double_buffer) {
	int yArrowPos = 300;
	char key;
	do {
		draw_welcome();
		draw_secondTitle();
		draw_menu_symbols();
		draw_date();
		draw_arrow(200, yArrowPos);
		key = waitForArrowsMenu();
		if (key == 'U') {
			draw_arrowCleaner(200, yArrowPos);
			if (yArrowPos != 300)
				yArrowPos -= 100;
			draw_arrow(200, yArrowPos);
		}
		else if (key == 'D') {
			draw_arrowCleaner(200, yArrowPos);
			if (yArrowPos != 600)
				yArrowPos += 100;
			draw_arrow(200, yArrowPos);
		}
	} while (key != 'E');

	if (yArrowPos == 300)
		startGame(double_buffer);
	else if (yArrowPos == 400)
		options_menu();
	else if (yArrowPos == 500)
		credits_menu();
	else if (yArrowPos == 600)
		return 0;

	return 1;
}


