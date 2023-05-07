/*
 * UART debugger testing
 */

#include "UART/uart.h"
#include "peripherals/led.h"

void uart_gpio_clock_init() {
	// Enable the UART3 peripheral clock.
	RCC->APB1ENR1 |= RCC_APB1ENR1_USART3EN;

	// Configure the UART pins (GPIO pins) for UART3.
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;	// Enable GPIO clock for port C pins

	// Configure GPIO pins
	GPIOC->MODER &= ~(GPIO_MODER_MODE4_Msk | GPIO_MODER_MODE5_Msk);
	GPIOC->MODER |= (GPIO_MODER_MODE4_1 | GPIO_MODER_MODE5_1);  // Alternative function mode

	GPIOC->AFR[0] &= ~(GPIO_AFRL_AFSEL4_Msk | GPIO_AFRL_AFSEL5_Msk);
	GPIOC->AFR[0] |= (7U << GPIO_AFRL_AFSEL4_Pos) | (7U << GPIO_AFRL_AFSEL5_Pos);  // AF7 (UART3) selected
}

int main() {
	init_clocks();
	init_gpio();
	init_nvic();

	uart_gpio_clock_init();

	uart3_init();

	while(1) { 
		uart3_display("Hello");
		nop(1000);
	}
}




