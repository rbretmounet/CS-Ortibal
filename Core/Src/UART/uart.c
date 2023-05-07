/*
 * uart.c
 *
 *  Created on: May 6th, 2023
 *      Author:  Darsh, Raphael
 *      Other Contributors: Jacob, Allison, Raymond
 */
#include "../peripherals/led.h"

#include "uart.h"


void uart3_init(){
	USART3->CR1 &= ~USART_CR1_UE;  // Disable UART3
	USART3->CR1 = 0;  // Clear control register 1

	USART3->BRR = 0x208D; /* Set the desired baud rate divisor here */
	USART3->CR1 &= ~(USART_CR1_M0 | USART_CR1_M1);	// 8 bit word size
	USART3->CR2 &= ~USART_CR2_STOP;  // Configure 1 stop bit (default)

	USART3->CR1 |= USART_CR1_TE | USART_CR1_RE;  // Enable transmitter and receiver
	USART3->CR1 |= USART_CR1_UE;  // Enable UART3
}

int uart3_display(char message[]) {
	op_led_c(0);
	for (int i = 0; i < strlen(message); i++){
		while (!(USART3->ISR & USART_ISR_TXE));	// wait until data register is empty
		USART3->TDR = message[i];				// send the data to the output register to be outputted
	}
	op_led_c(1);
	nop(20);									// wait for the transmission to be complete
	return (USART3->ISR & USART_ISR_TC);		// indicate whether the transmission was properly completed or not
}
