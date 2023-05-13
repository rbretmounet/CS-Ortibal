/*
 * interrupt_handlers.h
 *
 *  - Apr 29, 2023 (Creation)
 *      Author       : Tim S.
 *      Contributors : nithinsenthil , Raphael
 *      Log          : wrote the Systick_Handler
 */

#ifndef SRC_SYSTEM_CONFIG_INTERRUPT_HANDLERS_H_
#define SRC_SYSTEM_CONFIG_INTERRUPT_HANDLERS_H_

#include "../../peripherals/led.h"
#include "../clock_nvic_config.h"
#include "../../tools/print_scan.h"

/**
 * Interrupt handler for the SysTick timer.
 * Increments the systick_time variable and updates the status of the heartbeat and activity LEDs.
 *
 * @param None
 *
 * @returns None
 */
void SysTick_Handler();

void Button_Handler();
/**
 * Interrupt handler for the Button Press.
 * Increments the systick_time variable and updates the status of the heartbeat and activity LEDs.
 *
 * @param None
 *
 * @returns None
 */
#endif /* SRC_SYSTEM_CONFIG_INTERRUPT_HANDLERS_H_ */
