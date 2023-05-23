/*
 * rtc.h
 *
 *  - May 22-23, 2023 (Creation)
 *  	Author : Darsh
 *  	Log    : Wrote the primary rtc interface
 */

#include "rtc.h"

/*
 * Opens the RTC up for making changes to it.
 * NOTE: This action stops the RTC from running, until writing priveledge is closed.
 *
 * @param None
 * @returns None
 */
void rtc_open_writing_priveledge() {
	// Enable RTC Write Priveledge
	RTC->WPR = 0xCA;
	RTC->WPR = 0x53;

	// Enter RTC Initialization mode, wait for confirmation of initialization mode
	RTC->ISR |= RTC_ISR_INIT;
	while (!(RTC->ISR & RTC_ISR_INITF));
}

/*
 * Locks up the RTC so changes can't be made to it. This allows the RTC to start working again.
 *
 * @param None
 * @returns None
 */
void rtc_close_writing_priveledge() {
	// Exit Initialization Mode
	RTC->ISR &= ~RTC_ISR_INIT;

	// Enable RTC Write Protection
	RTC->WPR = 0xFF;
}

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
