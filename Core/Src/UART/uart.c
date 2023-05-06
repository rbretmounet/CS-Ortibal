/*
 * uart.c
 *
 *  Created on: May 6th, 2023
 *      Author:  Darsh, Raphael
 */

#include "uart.h"
#define CONFIG_WORDLENGTH (USART_CR1_M0 | USART_CR1_M1)


void uart3_init(){
	USART3->CR1 &= ~(CONFIG_WORDLENGTH); // Set the uart word length to 8 bits
	USART3->CR2 &= ~(USART_CR2_STOP); // Set the uart stop bits to 1 bit.
	USART3->BRR = 0x208D; //Set the baud rate to 9600
	USART3->CR1 |= USART_CR1_UE; //Enable UART
}

void uart3_display(char *message[])
{
	transmitted_characters = 0;
	USART3->CR1 |= USART_CR1_TE; //Enables transmission
	for (int i = 0; i < strlen(message); i++){
		USART3->TDR = message[i];
		while (!(USART3->ISR & USART_ISR_TXE))
		{
			;
		}
	}
	nop(20);
	return USART_ISR_TC;
}
