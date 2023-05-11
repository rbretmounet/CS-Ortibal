/*
 * print_scan.h
 *
 * Change Log:
 *
 * 	- May 11, 2023 (Creation)
 * 		Author : Darsh
 * 		Log    : Created the printMsg function
 */

#ifndef SRC_PRINT_SCAN_H_
#define SRC_PRINT_SCAN_H_

#include "../system_config/UART/uart.h"

/*
 * Prints a variable length string to the console
 *
 * @param message The string (character array) to be printed
 *
 * @returns None
 */
void printMsg(char message[]);

#endif // SRC_PRINT_SCAN_H_
