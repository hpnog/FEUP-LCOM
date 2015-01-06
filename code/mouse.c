#include "mouse.h"

int hookId_mouse = 7;

int test_packet(unsigned short cnt){
	unsigned short counter = 0;
	int checker;
	mouse.handlerCount = 0;

	int r, ipc_status;
	message msg;

	while (counter < cnt) {

		//driver receive
		if(driver_receive(ANY, &msg, &ipc_status) != 0) {
			printf("driver_receive failed with: %d", r);
			return 1;
		}
		//driver received

		//get data from mouse
		if (is_ipc_notify(ipc_status)) { //if received notification
			switch (_ENDPOINT_P (msg.m_source)) {
			case HARDWARE:
				if (msg.NOTIFY_ARG & game.mouse_irq) {
					mouse.packets[mouse.handlerCount] = mouse_int_handler();
					if ((BIT(3) & mouse.packets[0])) {
						if (mouse.handlerCount != 2)
							mouse.handlerCount++;
						else{
							printMoves();
							counter++;
						}
					}
				}
				break;
			default:
				break;
			}
		}
	}
	return 0;
}

void printMoves() {
	printf("\nB1=0x");
	printf("%02x", mouse.packets[0]);
	printf(" B2=0x");
	printf("%02x", mouse.packets[1]);
	printf(" B3=0x");
	printf("%02x", mouse.packets[2]);
	printf(" LB=");
	if (BIT(0) & mouse.packets[0])
		printf("1");
	else
		printf("0");
	printf(" MB=");
	if (BIT(2) & mouse.packets[0])
		printf("1");
	else
		printf("0");
	printf(" RB=");
	if (BIT(1) & mouse.packets[0])
		printf("1");
	else
		printf("0");
	printf(" XOV=");
	if (BIT(6) & mouse.packets[0])
		printf("1");
	else
		printf("0");
	printf(" YOV=");
	if (BIT(5) & mouse.packets[0])
		printf("1");
	else
		printf("0");
	printf(" X=");
	int x = mouse.packets[1];
	if (mouse.packets[1] & BIT(7)) {
		x = 255 - x;
		printf("-%d", x);
	}
	else
		printf("%d", x);
	int y = mouse.packets[2];
	printf(" Y=");
	if (mouse.packets[2] & BIT(7)) {
		y = 255 - y;
		printf("-%d", y);
	}
	else
		printf("%d", y);
	mouse.handlerCount = 0;
}

int issueCommand(unsigned long dest, unsigned long com, unsigned long *res) {
	if (sys_outb(dest, com) != OK)
		return 1;
	tickdelay(micros_to_ticks(DELAY_US));
	if (sys_inb(dest, res) != OK)
		return 1;
	return 0;
	}

int issuemouseCommand(unsigned long dest, unsigned long com) {
	emptyBuffer();
	if (sys_outb(dest, com) != OK)
		return 1;

	tickdelay(micros_to_ticks(DELAY_US));

	unsigned long stat;
	sys_inb(OUT_BUF, &stat);
	printf("\nStat (act): 0x%X", stat);

	return 0;
}

void emptyBuffer() {
	while (bufferFull()) {
		unsigned long sta;
		tickdelay(micros_to_ticks(DELAY_US));
		sys_inb(OUT_BUF, &sta);
	}

}

int bufferFull() {
	unsigned long sta;
	sys_inb(STAT_REG, &sta);
	sta &= IBF;
	int st = sta;
	return st;
}

int mouse_subscribe_int() {
	int temp;
	temp = hookId_mouse;
	if (sys_irqsetpolicy(MOUSE_IRQ, (IRQ_REENABLE | IRQ_EXCLUSIVE), &temp) == OK){
		if (sys_irqenable(&temp) == OK) {
			int act = activatemouse();
			return BIT(hookId_mouse);
		}
	}
	return -1;
}

int mouse_unsubscribe_int() {
	unsigned long stat;
	do {
			sys_outb(KBC_CMD_REG, 0xA8);
			sys_outb(KBC_CMD_REG, 0xD4);
			sys_outb(OUT_BUF, 0xF5);
			tickdelay(micros_to_ticks(DELAY_US));
			sys_inb(OUT_BUF, &stat);
		} while (stat != ACK);

	if (sys_irqdisable(&hookId_mouse) == OK){
		if (sys_irqrmpolicy(&hookId_mouse) == OK)											//unsubscribes the mouse
			return 0;
	}
	return -1;
}

int activatemouse() {

	unsigned long stat;

	do {
		sys_outb(KBC_CMD_REG, 0xA8);
		sys_outb(KBC_CMD_REG, 0xD4);
		sys_outb(OUT_BUF, 0xF4);
		tickdelay(micros_to_ticks(DELAY_US));
		sys_inb(OUT_BUF, &stat);
	} while (stat != ACK);

	return 0;
}

unsigned short mouse_int_handler() {
	unsigned long stat, data;
	int attempts = 0;

	while (attempts < 15) {
		sys_inb(STAT_REG, &stat);
		if (stat & OBF) {
			if (sys_inb(OUT_BUF, &data) == OK) {
				return data;
			}
		}
		printf("\nNext attempt");
		tickdelay(micros_to_ticks(DELAY_US));
		attempts++;
	}
	printf("\nNo success");
	return 1;
}
