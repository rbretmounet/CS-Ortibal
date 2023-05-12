/*
 * system_config.c
 *
 *	- May 11, 2023
 *		Author : Darsh
 *		Log    : Replacing hardcoded values with placeholders from
 *				 stm32l476xx.h, as well as commenting
 *
 *  - Apr 29, 2023 (Creation)
 *      Author       : Tim S.
 *      Contributors : nithinsenthil , Raphael
 *      Log          : All necessary clock registers configured
 */

#include "clock_nvic_config.h"

// initializing global counters and trackers
int core_MHz = 0;
int heartbeat_counter = 0;
int ag_counter = 0;

void nop(int nop_loops) {
	for (int i = 0; i < nop_loops; i++) {__NOP();}
}

/**
 * Initializes the clocks of the microcontroller.
 *
 * This function sets up the various clock sources and frequencies for the microcontroller.
 * It also reads the unique ID of the microcontroller and stores it in an array.
 *
 * @returns None
 */
void init_clocks() {
	// read chip identification information. only useful when watching memory through debugger
	volatile int id_w_x = (*(uint32_t*)(UID_BASE) & 0xFFFF0000) >> 16;
	volatile int id_w_y = *(uint32_t*)(UID_BASE) & 0xFFFF;
	volatile int id_w_n = *(uint32_t*)(UID_BASE+0x4) & 0xFF;
	volatile int id_lot_l = *(uint32_t*)(UID_BASE+0x4) & 0xFFFFFF00;
	volatile int id_lot_h = *(uint32_t*)(UID_BASE+0x8);
	volatile char id_lot[7] = {
		(id_lot_l >> 8) & 0xFF,//test
		(id_lot_l >> 16) & 0xFF,
		(id_lot_l >> 24) & 0xFF,
		(id_lot_h) & 0xFF,
		(id_lot_h >> 8) & 0xFF,
		(id_lot_h >> 16) & 0xFF,
		(id_lot_h >> 24) & 0xFF
	};

	// enable clock to different peripherals
	RCC->AHB2ENR =
		  RCC_AHB2ENR_GPIOAEN	// enable GPIO Port A
		| RCC_AHB2ENR_GPIOBEN	// enable GPIO Port B
		| RCC_AHB2ENR_GPIOCEN	// enable GPIO Port C
		| RCC_AHB2ENR_GPIODEN	// enable GPIO Port D
		| RCC_AHB2ENR_GPIOEEN	// enable GPIO Port E
		| RCC_AHB2ENR_GPIOFEN	// enable GPIO Port F
		| RCC_AHB2ENR_GPIOGEN	// enable GPIO Port G
		| RCC_AHB2ENR_GPIOHEN	// enable GPIO Port H
		| RCC_AHB2ENR_OTGFSEN	// enable OTG full speed
		| RCC_AHB2ENR_ADCEN		// enable ADC
		| RCC_AHB2ENR_RNGEN;	// enable Random Number Generator

	// enable clock to QSPI
	RCC->AHB3ENR = RCC_AHB3ENR_QSPIEN;
	// enable clock to different perihpherals and protocol hardware
	RCC->APB1ENR1 =
		  RCC_APB1ENR1_PWREN 		// enable Power Control
		| RCC_APB1ENR1_I2C2EN 		// enable I2C2
		| RCC_APB1ENR1_SPI3EN 		// enable SPI3
		| RCC_APB1ENR1_SPI2EN 		// enable SPI2
		| RCC_APB1ENR1_USART3EN;		// enable USART3
	//	| RCC_APB1ENR1_RTCAPBEN;	// enable RTC APB

	// enable Syscfg
	RCC->APB2ENR = RCC_APB2ENR_SYSCFGEN;

	// configure Flash Memory
	FLASH->ACR =
		  FLASH_ACR_DCEN			// Flash data cache enable
		| FLASH_ACR_ICEN			// Flash instruction cache enable
		| FLASH_ACR_PRFTEN			// Flash prefetch enable
		| FLASH_ACR_LATENCY_4WS;	// 4 HCLCK periods of latency in Flash access time

	RCC->CR |= RCC_CR_HSION; 	// enable HSI

	// configure Phased Lock Loop
	RCC->PLLCFGR =
		  0 << RCC_PLLCFGR_PLLR_Pos 	// PLLR = 2
		| RCC_PLLCFGR_PLLREN 			// PLLCLK output enabled
		| 0 << RCC_PLLCFGR_PLLQ_Pos 	//
		| RCC_PLLCFGR_PLLQEN			// PLLQEN (PLL48M1CLK) output enabled
		| 0 << RCC_PLLCFGR_PLLP_Pos 	// PLLP default
		| 0 << RCC_PLLCFGR_PLLPEN_Pos 	// PLLSAI3 not enabled
		| (10 << RCC_PLLCFGR_PLLN_Pos) 	// PLLN = 10
		| 0 << RCC_PLLCFGR_PLLM_Pos 	// PLLM = 1
		| RCC_PLLCFGR_PLLSRC_HSI; 		// HSI16 as input clock to PLLs
	RCC->PLLSAI1CFGR =
		  RCC_PLLSAI1CFGR_PLLSAI1R_0		 	// PLLADC1CLK division factor set to 4
		| RCC_PLLSAI1CFGR_PLLSAI1REN 			// PLLADC1CLK output enable
		| RCC_PLLSAI1CFGR_PLLSAI1Q_0 			// PLL48M2CLK division factor set to 4
		| RCC_PLLSAI1CFGR_PLLSAI1QEN 			// PLL48M2CLK output enable
		| 0 << RCC_PLLSAI1CFGR_PLLSAI1P_Pos		// PLLSAI1CLKC division factor set to 7
		| 0 << RCC_PLLSAI1CFGR_PLLSAI1PEN_Pos 	// PLLSAI1CLK output disabled
		| 12 << RCC_PLLSAI1CFGR_PLLSAI1N_Pos; 	// VCO multiplication factor set to 12
	// enable PLL and PLLSAI1
	RCC->CR |= RCC_CR_PLLON | RCC_CR_PLLSAI1ON;
	// wait for PLL lock
	while (!(RCC->CR & RCC_CR_PLLRDY));
	// wait for PLLSAI1 lock
	while (!(RCC->CR & RCC_CR_PLLSAI1RDY));
	// system clock to PLL
	RCC->CFGR = RCC_CFGR_SW;

	core_MHz = 80;
}

/**
 * Initializes the Nested Vector Interrupt Controller (NVIC) for the SysTick timer.
 * The SysTick timer is used for generating periodic (1ms) interrupts.
 *
 * @param None
 *
 * @returns None
 */
void init_nvic() {
	__disable_irq();
	SysTick->LOAD = (core_MHz / 8) * 1000; // configure for 1 ms period, use AHB/8
	SysTick->CTRL = 0x3; // use AHB/8 as input clock, enable interrupts and counter
	NVIC_EnableIRQ(SysTick_IRQn);
	__enable_irq();
}
