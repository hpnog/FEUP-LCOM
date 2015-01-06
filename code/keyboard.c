
#include "keyboard.h"

int timedChecker = 0;
int kbd_hookId = 9;
int hookId2 = 0;
int clockTim = 0; //global variable used in timerIntHandler() and timer_test_int()


int kbd_subscribe_int() {
	if (sys_irqsetpolicy(KBD_IRQ, (IRQ_REENABLE | IRQ_EXCLUSIVE), &kbd_hookId) == OK){
		if (sys_irqenable(&kbd_hookId) == OK) {
			//subscribes the keyboard
			return BIT(kbd_hookId);
		}
	}
	return -1;
}

int kbd_unsubscribe_int() {
	if (sys_irqdisable(&kbd_hookId) == OK){
		if (sys_irqrmpolicy(&kbd_hookId) == OK)											//unsubscribes the keyboard
			return 0;
	}
	return -1;
}

/*
 * Function prints the make/break code of the key from the keyboard
 */

long convert_data(long data) {

	if ((data & BIT(7))){
		if(data==WORD){
			printf("\nFirst Byte: ");
		}else{
			printf("\nBreak: ");
		}
	}else{
		printf("\nMake : ");
	}
	printf("%02x", data);
	return data;
}

long interrupt_handler_mouse_kbd(int kbd_irq_set, message msg, int ipc_status, int mouse_irq_set, int timer_irq_set) {
	mouse.handlerCount = 0;

	unsigned long sta, d;
	sys_inb(STAT_REG, &sta);
	if (sta &= OBF)
		sys_inb(OUT_BUF, &d);

	if (is_ipc_notify(ipc_status)) {
		switch (_ENDPOINT_P (msg.m_source)) {
		case HARDWARE:
			if (msg.NOTIFY_ARG & mouse_irq_set) {
				mouse.packets[mouse.handlerCount] = mouse_int_handler();
				if ((BIT(3) & mouse.packets[0])) {
					if (mouse.handlerCount != 2)
						mouse.handlerCount++;
					else{
						printMoves();
						int x = mouse.packets[1];
						if (mouse.packets[1] & BIT(7))
							x = 255 - x;
						if (x < 0) {
							printf("\nSaiu esquerda no rato");
							return LEFT_ARROW_REL;
						}
						else if (x > 0) {
							printf("\nSaiu direita no rato");
							return RIGHT_ARROW_REL;
						}
						else if (BIT(0) & mouse.packets[0]) {
							printf("\nSaiu enter no rato");
							return ENTER_REL;
						}
						else
							return -1;
					}
				}
			}

			if (msg.NOTIFY_ARG & kbd_irq_set) {
				int check2 = 1;
				unsigned long lastData;
				while(check2) {
					unsigned long stat, data;								//stat and data variables initialized
					if (sys_inb(STAT_REG, &stat) == OK) {					//if sys_inb returns OK
						if (sys_inb(OUT_BUF, &data) == OK) {				//if sys_inb returns OK
							if ((stat & (PAR_ERR | TO_ERR)) == 0) {			//if no errors are registered in stat
								timedChecker = 1;
								long temp = convert_data(data);
								if (temp == KBD_ESC_REL)
									printf("\nsaiu pelo kbd");
								return temp;
							}												//convert to int and return
							else
								return -1;
						}
						else
							return -1;
						tickdelay(micros_to_ticks(DELAY_US));
					}
					else
						return -1;
				}
			}

			if (msg.NOTIFY_ARG & timer_irq_set) {
				timer_int_handler();
				if (game.clock_time >= game.sec*60) {	//verifies if another second has passed
					game.sec++;
					draw_progressBar_15sec(game.sec);
					if (game.sec >= 15) {
						printf("\nSaiu pelo tempo");
						return (int)99;
					}
				}
				else
					return -1;
			}
			break;
		default:
			break;
		}
	}
}

long interrupt_handler(int kbd_irq_set, message msg, int ipc_status) {

	unsigned long sta, d;
	sys_inb(STAT_REG, &sta);
	if (sta &= OBF)
		sys_inb(OUT_BUF, &d);

	if (is_ipc_notify(ipc_status)) {
		switch (_ENDPOINT_P (msg.m_source)) {
		case HARDWARE:
			if (msg.NOTIFY_ARG & kbd_irq_set) {
				int check2 = 1;
				unsigned long lastData;
				while(check2) {
					unsigned long stat, data;								//stat and data variables initialized
					if (sys_inb(STAT_REG, &stat) == OK) {					//if sys_inb returns OK
						if (sys_inb(OUT_BUF, &data) == OK) {				//if sys_inb returns OK
							if ((stat & (PAR_ERR | TO_ERR)) == 0) {			//if no errors are registered in stat
								timedChecker = 1;
								return data;
							}												//convert to int and return
							else
								return -1;
						}
						else
							return -1;
						tickdelay(micros_to_ticks(DELAY_US));
					}
					else
						return -1;
				}
			}
			break;
		default:
			break;
		}
	}
}

int waitForSpaceBar() {

	message msg;
	int ipc_status;

	int r;
	int check = 1;
	unsigned long lastData, data;

	unsigned long sta, d;
	sys_inb(STAT_REG, &sta);
	if (sta &= IBF)
		sys_inb(OUT_BUF, &d);

	while (check) {
		r = driver_receive(ANY, &msg, &ipc_status);
		if (r != 0) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		long kb = interrupt_handler(game.kbd_irq, msg, ipc_status);
		if (kb == SPACE_REL)
			check = 0;
		else if (kb == -1)
			return kb;
	}

	return 0;
}

char read_status_kbd(long stat) {
	if (sys_outb(OUT_BUF, LED_SWITCHES) == OK)
		if (sys_inb(OUT_BUF, &stat) == OK)
			if (stat == ERROR)
				return 'E';
			else if (stat == RESEND)
				return 'R';
			else
				return 'A';
}

char issue_command_kbd(long stat, long cmd) {
	if((stat & IBF) == 0)
		if (sys_outb(OUT_BUF, cmd) == OK)
			if (sys_inb(OUT_BUF, &stat) == OK)
				if (stat == ERROR)
					return 'E';
				else if (stat == RESEND)
					return 'R';
				else
					return 'A';
}

char waitForArrowsMenu() {

	message msg;
	int ipc_status;
	long code, kb;

	unsigned long sta, d;
	sys_inb(STAT_REG, &sta);
	if (sta &= IBF)
		sys_inb(OUT_BUF, &d);

	int r;
	int check = 1;
	unsigned long lastData, data;

	while (check) {
		r = driver_receive(ANY, &msg, &ipc_status);
		if (r != 0) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		kb = interrupt_handler(game.kbd_irq, msg, ipc_status);
		if ((kb == UP_ARROW_REL) || (kb == DOWN_ARROW_REL) || (kb == ENTER_REL))
			check = 0;
		else if (kb == -1)
			return kb;
	}
	code = kb;

	printf("\nAquele: %02x\n", kb);

	if (kb == UP_ARROW_REL)
		return 'U';
	else if (kb == DOWN_ARROW_REL)
		return 'D';
	else
		return 'E';
}

int kbd_select_col() {
	message msg;
	int ipc_status;
	int r;

	unsigned long sta, d;
	sys_inb(STAT_REG, &sta);
	if (sta &= OBF)
		sys_inb(OUT_BUF, &d);

	long kb = -1;
	int check = 1;
	int play = 1;
	int col = 1;
	long tempo = (int)99;;
	unsigned long lastData, data;
	draw_game_arrow(col);
	while (play) {
		while (check) {

			r = driver_receive(ANY, &msg, &ipc_status);
			if (r != 0) {
				printf("driver_receive failed with: %d", r);
				continue;
			}
			kb = interrupt_handler_mouse_kbd(game.kbd_irq, msg, ipc_status, game.mouse_irq, game.timer_irq);
			if ((kb == LEFT_ARROW_REL) || (kb == RIGHT_ARROW_REL) || (kb == ENTER_REL) || (kb == KBD_ESC_REL) || (kb == tempo))
				check = 0;
			else {
				kb = -1;
				continue;
			}
		}
		check = 1;
		if ((kb == LEFT_ARROW_REL) && (col != 1)) {
			draw_game_arrow_cleaner(col);
			col--;
			draw_game_arrow(col);
		}
		else if ((kb == RIGHT_ARROW_REL) && (col != 7)) {
			draw_game_arrow_cleaner(col);
			col++;
			draw_game_arrow(col);
		}
		else if (kb == ENTER_REL) {
			draw_game_arrow_cleaner(col);
			play = 0;
		}
		else if (kb == tempo) {
			draw_game_arrow_cleaner(col);
			return 99;
		}
		else if (kb == KBD_ESC_REL)
			return -1;
	}
	return col;
}

char waitForArrowsOptions() {

	message msg;
	int ipc_status;
	long code, kb;

	unsigned long sta, d;
	sys_inb(STAT_REG, &sta);
	if (sta &= IBF)
		sys_inb(OUT_BUF, &d);

	int r;
	int check = 1;
	unsigned long lastData, data;

	while (check) {
		r = driver_receive(ANY, &msg, &ipc_status);
		if (r != 0) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		kb = interrupt_handler(game.kbd_irq, msg, ipc_status);
		if ((kb == RIGHT_ARROW_REL) || (kb == LEFT_ARROW_REL) || (kb == ENTER_REL))
			check = 0;
		else if (kb == -1)
			return kb;
	}
	code = kb;

	printf("\nAquele: %02x\n", kb);

	if (kb == RIGHT_ARROW_REL)
		return 'R';
	else if (kb == LEFT_ARROW_REL)
		return 'L';
	else
		return 'E';
}
