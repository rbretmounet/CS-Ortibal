/*
 * RTC testing
 */

#include "./system_config/platform_init.h"
#include "./tools/print_scan.h"
#include "./system_config/RTC/rtc.h"

int count = 0;


int interrupt_count = 0;

int main() {
	init_platform();
	uint8_t hour, minute, sec;
	while(1) {
		// Read and Display the RTC Time
		rtc_get_time(&hour, &minute, &sec);
		printMsg("Current Time : %d : %d : %d String: %s\n", hour, minute, sec, "sss");

		nop(1000000);
	}
}
