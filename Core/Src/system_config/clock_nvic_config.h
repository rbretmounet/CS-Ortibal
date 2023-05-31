/*
 * clock_nvic_config.h
 *
 *  - Created on: Apr 29, 2023
 *      Author       : Tim S.
 *      Contributors : nithinsenthil , Raphael
 *      Log          : All necessary clock registers configured
 */

#ifndef SRC_CLOCK_NVIC_CONFIG_H_
#define SRC_CLOCK_NVIC_CONFIG_H_

#include "../system/stm32l476xx.h"
#include "../globals.h"

typedef enum { false, true } bool;

void nop(int nop_loops);

/**
 * Initializes the clocks of the microcontroller.
 *
 * This function sets up the various clock sources and frequencies for the microcontroller.
 * It also reads the unique ID of the microcontroller and stores it in an array.
 *
 * @returns None
 */
void init_clocks();

/**
 * Initializes the Nested Vector Interrupt Controller (NVIC) for
 * 		- Systick Timer (1ms)
 * 		- GPIO Pins 10-15
 * 			- Buttons 0 & 1
 *
 * @param None
 *
 * @returns None
 */
void init_nvic();

#endif /* SRC_CLOCK_NVIC_CONFIG_H_ */
