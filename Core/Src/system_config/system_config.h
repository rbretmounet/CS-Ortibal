/*
 * system_config.h
 *
 *  Created on: Apr 29, 2023
 *      Author: Tim S. , nithinsenthil , Raphael
 */

#ifndef SRC_SYSTEM_CONFIG_H_
#define SRC_SYSTEM_CONFIG_H_

#include "stm32l476xx.h"

typedef enum { false, true } bool;

// Global Variables
extern int core_MHz;
extern int systick_time;
extern int heartbeat_counter;
extern int ag_counter;

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
 * Initializes the Nested Vector Interrupt Controller (NVIC) for the SysTick timer.
 * The SysTick timer is used for generating periodic interrupts.
 *
 * @param None
 *
 * @returns None
 */
void init_nvic();

#endif /* SRC_SYSTEM_CONFIG_H_ */
