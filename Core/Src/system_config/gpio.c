/*
 * gpio.c
 *
 *  Created on: Apr 29, 2023
 *      Author: Tim S. , nithinsenthil , Raphael
 */

#include "gpio.h"

/**
 * Initializes the GPIO pins for the microcontroller.
 *
 * @param None
 *
 * @returns None
 */
void init_gpio() {
	PWR->CR2 |= 1 << 9; // enable VDDIO2 supply for GPIOG
	// wait until each GPIO is clocked and ready
	while (GPIOA->OTYPER == 0xFFFFFFFF);
	while (GPIOB->OTYPER == 0xFFFFFFFF);
	while (GPIOC->OTYPER == 0xFFFFFFFF);
	while (GPIOD->OTYPER == 0xFFFFFFFF);
	while (GPIOE->OTYPER == 0xFFFFFFFF);
	while (GPIOF->OTYPER == 0xFFFFFFFF);
	while (GPIOG->OTYPER == 0xFFFFFFFF);
	// OP R1 gpio pinout
	// 		D7 to D0 	D7 to D0
	// 		HEARTBEAT 	E2
	// 		A 			G11
	// 		B 			G12
	// 		C 			G9
	// 		FAULT		G7
	// 		AG 			G6
	// 		BTN1 		B11
	// 		BTN0 		B10
	GPIOB->MODER = 0x00000000; // digital input for btn 0 and 1
	GPIOB->PUPDR = 0x1 << (10*2) | 0x1 << (11*2); // pull up for btn0 and 1
	GPIOD->MODER = 0x00005555; // leds D7 to D0
	GPIOE->MODER = 0x1 << (2*2); // led
	GPIOG->MODER = 0x01445000; // leds
}

/**
 * Sets the specified GPIO pin to high.
 *
 * @param port The GPIO port to which the pin belongs.
 * @param pin The pin number to set high.
 *
 * @returns None
 */
void gpio_high(GPIO_TypeDef * port, int pin) {
	port->BSRR = 1 << pin;
}

/**
 * Sets the specified GPIO pin to low logic level.
 *
 * @param port The GPIO port to which the pin belongs.
 * @param pin The pin number to set to low.
 *
 * @returns None
 */
void gpio_low(GPIO_TypeDef * port, int pin) {
	port->BSRR = 1 << (pin+16);
}

/**
 * Sets the value of a GPIO pin on a given port.
 *
 * @param port The GPIO port to set the pin on.
 * @param pin The pin number to set.
 * @param value The value to set the pin to (1 for high, 0 for low).
 *
 * @returns None
 */
void gpio_set(GPIO_TypeDef * port, int pin, int value) {
	if (value) {
		gpio_high(port, pin);
	} else {
		gpio_low(port, pin);
	}
}

/**
 * Reads the value of a GPIO pin.
 *
 * @param port The GPIO port to read from.
 * @param pin The pin number to read.
 *
 * @returns The value of the GPIO pin (1 or 0).
 */
int gpio_read(GPIO_TypeDef * port, int pin) {
	if (port->IDR & (1 << pin)) {
		return 1;
	} else {
		return 0;
	}
}
