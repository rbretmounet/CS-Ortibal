/*
 * imu.h
 *
 *  Created on: Apr 29, 2023
 *      Author: Raphael, Darsh
 *      Other Contributors: Jacob, Allison, Raymond
 */

#ifndef SRC_UART_UART_H_
#define SRC_UART_UART_H_

#include "../system_config/gpio.h"
#include <string.h>

void uart3_init();

int uart3_display(char message[]);

#endif
