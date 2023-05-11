/*
 * print_scan.c
 *
 * Change Log:
 *
 * 	- May 11, 2023 (Creation)
 * 		Author : Darsh
 * 		Log    : Created the printMsg function
 */


#include "print_scan.h"
#include <string.h>

/*
 * Prints a variable length string to the console
 *
 * @param message The string (character array) to be printed
 *
 * @returns None
 */
void printMsg(char message[]) {
	// TODO : Add ability to accept variadic strings

	uart_transmitStr(USART3, message);
}
