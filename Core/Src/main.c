/*
 * RTC testing
 */

#include "./system_config/platform_init.h"
#include "./tools/print_scan.h"

int main() {
	init_platform();

	// Bypass the Shdow registers to read RTC directly
	RTC->CR |= RTC_CR_BYPSHAD;

	// Setup the Prescalers to generate a 1Hz signal (based on formulas from reference manual)
	RTC->PRER = (127 << RTC_PRER_PREDIV_A_Pos) | (249 << RTC_PRER_PREDIV_S_Pos);

	// Setup time and data through shadow registers
	// 20th May (5) 2023  -  14:00:00  (24-Hour Format)

	// time
	RTC->CR &= ~RTC_CR_FMT;
	RTC->TR &= ~RTC_TR_PM;

	RTC->TR &= ~(RTC_TR_HT_Msk);
	RTC->TR |= (1 << RTC_TR_HT_Pos);

	RTC->TR &= ~(RTC_TR_HU_Msk);
	RTC->TR |= (4 << RTC_TR_HU_Pos);

	RTC->TR &= ~(RTC_TR_MNT_Msk);
	RTC->TR |= (0 << RTC_TR_MNT_Pos);

	RTC->TR &= ~(RTC_TR_MNU_Msk);
	RTC->TR |= (0 << RTC_TR_MNU_Pos);

	RTC->TR &= ~(RTC_TR_ST_Msk);
	RTC->TR |= (0 << RTC_TR_ST_Pos);

	RTC->TR &= ~(RTC_TR_SU_Msk);
	RTC->TR |= (0 << RTC_TR_SU_Pos);

	// date
	RTC->DR &= ~(RTC_DR_YT_Msk);
	RTC->DR |= (2 << RTC_DR_YT_Pos);

	RTC->DR &= ~(RTC_DR_YU_Msk);
	RTC->DR |= (3 << RTC_DR_YU_Pos);

	RTC->DR &= ~(RTC_DR_WDU_Msk);
	RTC->DR |= (6 << RTC_DR_WDU_Pos);

	RTC->DR &= ~(RTC_DR_MT_Msk);
	RTC->DR |= (0 << RTC_DR_MT_Pos);

	RTC->DR &= ~(RTC_DR_MU_Msk);
	RTC->DR |= (5 << RTC_DR_MU_Pos);

	RTC->DR &= ~(RTC_DR_DT_Msk);
	RTC->DR |= (2 << RTC_DR_DT_Pos);

	RTC->DR &= ~(RTC_DR_DU_Msk);
	RTC->DR |= (0 << RTC_DR_DU_Pos);

	while(1) {
		// Read and Display the RTC Time


		nop(1000000);
	}
}
