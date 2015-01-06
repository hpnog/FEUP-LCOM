
#include "timer.h"

int hook_id = t;

int timer_subscribe_int(void )
{

	if (sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hook_id) == OK){
		if (sys_irqenable(&hook_id) == OK) return BIT(t);
	}
	return -1;
}

int timer_unsubscribe_int()
{
	if (sys_irqrmpolicy(&hook_id) == OK){
			if (sys_irqdisable(&hook_id) == OK)
				return 0;
		}
	return -1;
}

void timer_int_handler()
{
	game.clock_time = game.clock_time + 1;		//function only increments the global variable clock_time
}

int timer_wait_int(unsigned long time) {
	
	clock_time = 0;			//initializes the global variable as 0
	int ipc_status;
	message msg;
	int r;
	//printf("\n \n");
	int sec = 0;			//initializes the sec variable as 0
							//variable sec counts the seconds
	while(clock_time < time*60){
		r = driver_receive(ANY, &msg, &ipc_status);
		if ( r != 0 )
		{
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) {
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE:
				if (msg.NOTIFY_ARG & game.timer_irq) {
					timer_int_handler();
					if (clock_time >= sec*60)	//verifies if another second has passed
					sec++;
				}
				break;
			default:
				break;
			}
		} else {}
	}
	return 0;
}

