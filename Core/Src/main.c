// opr1-quickstart1 
// Quick start for Orbital Platform R1 
// TCS 202304. Yume Research & Friends. 
// The above attribution must be included regardless of license. 
// This is not permissively licensed software. 

//system_config access
#include "system_config/system_config.h"
#include "system_config/gpio.h"
#include "system_config/i2c.h"

//peripherals access
#include "peripherals/led.h"
#include "peripherals/imu.h"
#include "peripherals/mag.h"

#include "stm32l476xx.h"
#include <math.h> // for sqrt 

// IMU, Gyro Tests
int main() {
	init_clocks();
	init_gpio();
	init_nvic();

	while(1) { 
		// implement the UART



	}
}




