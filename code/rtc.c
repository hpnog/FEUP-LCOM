#include "rtc.h"


int subscribe_real_time_clock() {
	game.rtc_irq = 13;

	//--------------------------------------------------------------------

	unsigned long regisB;

	regisB = BIT(4) | BIT(1);

	sys_outb(RTC_ADDR_REG, REG_B);

	sys_outb(RTC_DATA_REG, regisB);

	//--------------------------------------------------------------------

	unsigned long temp = BIT(game.rtc_irq);
	if (sys_irqsetpolicy(8, IRQ_REENABLE | IRQ_EXCLUSIVE, &game.rtc_irq) != OK)
		return -3;
	if (sys_irqenable(&game.rtc_irq) != OK)
		return -2;
	return temp;
}

int unsubscribe_real_time_clock() {
	if (sys_irqdisable(&game.rtc_irq) != OK)
		return -3;
	if (sys_irqrmpolicy(&game.rtc_irq) != OK)
		return -3;
	return 0;
}

int readDate() {
	sys_outb(RTC_ADDR_REG, MINUTES);
	sys_inb(RTC_DATA_REG, &date.minute);

	sys_outb(RTC_ADDR_REG, DAYS);
	sys_inb(RTC_DATA_REG, &date.day);

	sys_outb(RTC_ADDR_REG, MONTHS);
	sys_inb(RTC_DATA_REG, &date.month);

	sys_outb(RTC_ADDR_REG, YEARS);
	sys_inb(RTC_DATA_REG, &date.year);

	sys_outb(RTC_ADDR_REG, HOURS);
	sys_inb(RTC_DATA_REG, &date.hour);

	sys_outb(RTC_ADDR_REG, WEEK_DAYS);
	sys_inb(RTC_DATA_REG, &date.weekday);
	return 0;
}

void rtc_get() {
	int temp = 0;
	unsigned long stat;
	sys_outb(RTC_ADDR_REG, REG_A);
	sys_inb(RTC_DATA_REG, &stat);
	if (stat & BIT(7)) {
		readDate();
		temp = 1;
	}
}
