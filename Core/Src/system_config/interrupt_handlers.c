/*
 * system_handlers.c
 *
 *  Created on: Apr 29, 2023
 *      Author: Tim S. , nithinsenthil , Raphael
 */

#include "../peripherals/led.h"
#include "interrupt_handlers.h"

// initializing Global (external) variables
int systick_time = 0;	// systick interrupt counter

/**
 * Interrupt handler for the SysTick timer.
 * Increments the systick_time variable and updates the status of the heartbeat and activity LEDs.
 *
 * @param None
 *
 * @returns None
 */
void SysTick_Handler() {

	systick_time++;

	// use d7 to d0 to display system time
//	GPIOD->ODR = (GPIOD->ODR & 0xFFFFFF00) | (systick_time >> 4) & 0xFF;

	// heartbeat led
	if (!(systick_time % 1000)) {
		heartbeat_counter = 100;
	}
	if (!(systick_time % 250)) {
		ag_counter = 10;
	}
	if (heartbeat_counter) {
		op_led_hb(1);
		heartbeat_counter--;
	} else {
		op_led_hb(0);
	}
	if (ag_counter) {
		op_led_ag(1);
		ag_counter--;
	} else {
		op_led_ag(0);
	}
}
