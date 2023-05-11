/*
 * platform_init.h
 *
 *  - May 11, 2023 (Creation)
 *  	Author : Darsh
 *  	Log    : Created the platform initialization function
 */

#ifndef SRC_PLATFORM_INIT_H_
#define SRC_PLATFORM_INIT_H_

#include "clock_nvic_config.h"


/*
 * Configures the system's various features, such as clocks, nvic, gpio, protocol hardware, and more.
 *
 * @param None
 *
 * @returns None
 */
void init_platform();

#endif
