/*
 * system_config.c
 *
 *  Created on: Apr 29, 2023
 *      Author: Tim S. , nithinsenthil , Raphael
 */

#include "system_config.h"

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
	/* volatile int id_w_x = (*(uint32_t*)(UID_BASE) & 0xFFFF0000) >> 16;
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
	}; */

	RCC->AHB2ENR = 0x000530FF; // enable clock to all gpio banks, usb otg, adc, crypto
	RCC->AHB3ENR = 0x00000100; // enable clock to QSPI
	RCC->APB1ENR1 =
		1 << 28 // enable power control
		| 1 << 22 // enable i2c2 //test change
		| 1 << 15 // enable spi3
		| 1 << 14 // enable spi2
		| 1 << 10 // enable rtc apb
		| 1 << 18; // enable USART3
	RCC->APB2ENR = 1; // enable syscfg
	FLASH->ACR = 0x600 | 4 | 1<<8; // flash access latency to four wait states, prefetch enable lol
	//FLASH->ACR = 0x604; // OVERCLOCK! for testing only
	RCC->CR |= 1 << 8; // enable HSI
	RCC->PLLCFGR =
		0x0 << 25 // PLLR = 2
		| 1 << 24 // PLLREN = 1
		| 0x0 << 21 // PLLQ = 2
		| 1 << 20 // PLLQEN (PLL48M1CLK) output enabled
		| 0x0 << 17 // PLLP default
		| 0 << 16 // PLLSAI3 not enabled
		| 10 << 8 // PLLN = 10
	//	| 20 << 8 // OVERCLOCK! for testing only
		| 0 << 4 // PLLM = 1
		| 0x2; // HSI16 as input clock to PLLs
	RCC->PLLSAI1CFGR =
		0x1 << 25 // PLLSAI1R = 4
		| 1 << 24 // PLLSAI1REN = PLLADC1CLK enable
		| 0x1 << 21 // PLLSAI1Q = 4
		| 1 << 20 // PLLSAI1QEN = PLL48M2CLK enable
		| 0x0 << 17 // PLLSAI1P = default
		| 0 << 16 // PLLSAI1PEN = 0
		| 12 << 8; // PLLSAI1N = 12
	RCC->CR |= (1 << 24) | (1 << 26); // enable PLL and PLLSAI1
	while (!(RCC->CR & (1 << 25))); // wait for PLL lock
	while (!(RCC->CR & (1 << 27))); // wait for PLLSAI1 lock
	RCC->CFGR = 0x3; // system clock to PLL
	core_MHz = 80;
}

/**
 * Initializes the Nested Vector Interrupt Controller (NVIC) for the SysTick timer.
 * The SysTick timer is used for generating periodic interrupts.
 *
 * @param None
 *
 * @returns None
 */
void init_nvic() {
	__disable_irq();
	SysTick->LOAD = core_MHz/8*1000; // configure for 1 ms period, use AHB/8
	SysTick->CTRL = 0x3; // use AHB/8 as input clock, enable interrupts and counter
	NVIC_EnableIRQ(SysTick_IRQn);
	__enable_irq();
}
