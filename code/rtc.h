#ifndef __RTC_H
#define __RTC_H

#include "interface.h"

/** @defgroup rtc rtc
 * @{
 *
 * Functions for controling the Real Time Clock
 */

typedef struct {
	unsigned long minute;
	unsigned long hour;
	unsigned long weekday;
	unsigned long day;
	unsigned long month;
	unsigned long year;
} date_struct;

date_struct date;

#define RTC_ADDR_REG		0x70
#define RTC_DATA_REG		0x71
#define REG_A				0x0A
#define REG_B				0x0B
#define MINUTES				0x02
#define HOURS				0x04
#define WEEK_DAYS			0x06
#define DAYS				0x07
#define MONTHS				0x08
#define YEARS				0x09

/**
 * @brief Gets the actual information about the time to the date structure
 *
 * @return Return 0
 */
int readDate();

/**
 * @brief Checks if rtc is ready to send info and gets it
 */
void rtc_get();

/**
 * @brief Activates the correct mode of the rtc and subscibes it
 *
 * @return BIT(hook_id)
 */
int subscribe_real_time_clock();

/**
 * @brief Unsubscribees thee real time clock
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int unsubscribe_real_time_clock();

/**@}*/

#endif /* __RTC_H */
