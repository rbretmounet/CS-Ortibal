/*
 * rtc.h
 *
 *  - May 22-23, 2023 (Creation)
 *  	Author : Darsh
 *  	Log    : Wrote the primary rtc interface
 */

#ifndef SRC_RTC_H_
#define SRC_RTC_H_

#include "../system/stm32l476xx.h"

/*
 * Functions to interface with the STM32L476ZG's Internal RTC
 *
 * NOTE: The selection of which oscillator the RTC uses is done through the
 * RCC registers, in system_config/clock_nvic_config : rtc_clock_config()
 */

#define January    uint8_t(1)
#define February   uint8_t(2)
#define March      uint8_t(3)
#define April      uint8_t(4)
#define May        uint8_t(5)
#define June	   uint8_t(6)
#define July       uint8_t(7)
#define August     uint8_t(8)
#define September  uint8_t(9)
#define October    uint8_t(10)
#define November   uint8_t(11)
#define December   uint8_t(12)

#define Monday     uint8_t(1)
#define Tuesday    uint8_t(2)
#define Wednesday  uint8_t(3)
#define Thursday   uint8_t(4)
#define Friday     uint8_t(5)
#define Saturday   uint8_t(6)
#define Sunday     uint8_t(7)

/*
 * Sets the appropriate pre-scalers based on the oscillator source of the RTC.
 *
 * @param None
 * @returns None
 */
void rtc_update_prescaler();

/*
 * Sets the Year, Month, Date, and Day in the RTC
 * NOTE : rtc.h provides a few definitions to use for 'month' and 'day'
 * NOTE : 'year' refers only to the tens and ones digits, so 23 in 2023
 *
 * @param None
 * @returns None
 */
void rtc_set_calendar(uint8_t year, uint8_t month, uint8_t date, uint8_ day);

/*
 * Sets a Time on the RTC
 *
 * @param hour    The hour to be set
 * @param minute  The minute to be set
 * @param second  The second to be set
 * @param None
 * @returns None
 */
void rtc_set_time(uint8_t hour, uint8_t minute, uint8_t second);

/*
 *
 *
 * @param None
 * @returns None
 */
void rtc_get_date();

/*
 * Returns the current RTC Time.
 * NOTE : The return values are stored in the function arguements
 *
 * @param hour    Where the function stores the current hour value
 * @param minute  Where the function stores the current minute value
 * @param second  Where the function stores the current second value
 * @returns None
 */
void rtc_get_time(uint8_t *hour, uint8_t *minute, uint8_t *second);


#endif // SRC_RTC_H_
