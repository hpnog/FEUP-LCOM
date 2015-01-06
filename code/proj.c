#include "interface.h"


int main() {

	sef_startup();

	sys_enable_iop(SELF);

	//subscribes timer-kbd-mouse-----------------------
	game.timer_irq = timer_subscribe_int();
	if (game.timer_irq < 0)
		return;
	game.kbd_irq = kbd_subscribe_int();
	if (game.kbd_irq < 0)
		return;
	game.mouse_irq = mouse_subscribe_int();
	if (game.mouse_irq < 0)
		return;
	subscribe_real_time_clock();
	//timer-kbd-mouse----------------------------------

	printf("\n\nReading Bitmaps...");
	readAllBitmaps_universe();
	printf("\n\nPress Spac-bar to begin\n\n");
	waitForSpaceBar();

	lm_init();
	video_initialize();
	welcome_screen(); //Appears first screen and waits the user to press space-bar to continue to main menu


	//--------------------------------------------------------------------------------------------------------
	unsigned char *double_buffer = (unsigned char *) malloc(1024*768*2);

	if (double_buffer==NULL)
	{
	  printf("Not enough memory for double buffer.\n");
	  exit(1);
	}
	//--------------------------------------------------------------------------------------------------------

	int check = 1;
	do {
		check = main_menu(&double_buffer);
	} while (check > 0);

	vg_exit();

	//unsubscribes timer-kbd-mouse-----------------------
	timer_unsubscribe_int();
	kbd_unsubscribe_int();
	mouse_unsubscribe_int();
	unsubscribe_real_time_clock();
	//timer-kbd-mouse------------------------------------

	return 0;
}

