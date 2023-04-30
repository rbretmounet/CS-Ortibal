/*
 * system_handlers.h
 *
 *  Created on: Apr 29, 2023
 *      Author: Tim S. , nithinsenthil , Raphael
 */

#ifndef SRC_SYSTEM_CONFIG_INTERRUPT_HANDLERS_H_
#define SRC_SYSTEM_CONFIG_INTERRUPT_HANDLERS_H_

	#include "system_config.h"
	/**
	 * Interrupt handler for the SysTick timer.
	 * Increments the systick_time variable and updates the status of the heartbeat and activity LEDs.
	 *
	 * @param None
	 *
	 * @returns None
	 */
	void SysTick_Handler();

#endif /* SRC_SYSTEM_CONFIG_INTERRUPT_HANDLERS_H_ */
