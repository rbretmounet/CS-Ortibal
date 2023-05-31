/*
 * platform_init.c
 *
 *  - May 11, 2023 (Creation)
 *  	Author : Darsh
 *  	Log    : Created the platform initialization function
 */

#include "platform_init.h"
#include "./UART/uart.h"
#include "./I2C/i2c.h"
#include "./Interrupts/exti_config.h"

/*
 * Configures the system's various features,
 * such as clocks, nvic, gpio, protocol hardware, and more.
 *
 * @param None
 *
 * @returns None
 */
void init_platform() {
	init_clocks();	// initialize the clocks
	init_gpio();	// intitialize the GPIO pins
	EXTI_init();	// configure for GPIO interrupt

	init_nvic();	// initialize the NVIC

	init_softi2c(OP1_I2C2);		// initialize the sotfware implemented I2C for I2C Bus 2
	uart_init(USART3, 9600);	// initialize the hardware for USART Bus 3
}
