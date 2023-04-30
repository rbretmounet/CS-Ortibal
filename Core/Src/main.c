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

/**
 * Initializes the Analog-to-Digital Converter (ADC) by configuring the GPIO pin for analog input.
 *
 * @param None
 *
 * @returns None
 */
void init_adc() {
	GPIOA->MODER |= 0x3 << (1*2);
	
}

/**
 * Computes the absolute value of a 16-bit integer.
 *
 * @param a The input 16-bit integer.
 *
 * @returns The absolute value of the input integer.
 */
unsigned int abs16(int16_t a) {
	if (a < 0) {
		return (a * -1);
	} else {
		return a;
	}
}

// IMU, Gyro Tests
int main() {
	init_clocks();
	init_gpio();
	init_nvic();
	init_softi2c(OP1_I2C2);
	
	op1_mag_init(MAG_ODR_200_Hz, MAG_FS_8_G, MAG_OVERSAMPLE_512);
	op1_imu_init(IMU_ODR_3333_Hz, IMU_FS_2_g, IMU_ODR_3333_Hz, IMU_FS_1000_dps);

	int state = 0;
	int btn_debounce = 0;

	while(1) { 
		op_led_c(!gpio_read(GPIOB, 11));
		
		if ((!gpio_read(GPIOB, 10))&&(!btn_debounce)) {
			state = (state + 1) % 3;
			btn_debounce = 100;
		} else if (btn_debounce) {
			btn_debounce--;
		}
		
		switch (state) {
			case 0: 
				GPIOD->ODR = ((GPIOD->ODR & 0xFFFFFF00) | (op1_imu_read_acel_x()>>8));
				op_led_a(1);
				op_led_b(systick_time & 1 << 6);
				break; 
			case 1: 
				GPIOD->ODR = ((GPIOD->ODR & 0xFFFFFF00) | (abs16(op1_imu_read_gyro_z())>>8));
				op_led_a(0);
				op_led_a(1);
				break; 
			default: 
				op_led_a(0);
				op_led_b(0);
				break; 
		}
		nop(100000);
	}

/*
	while(1) { // blinky 
		op_led_c(!gpio_read(GPIOB, 11));
		nop(1000000);
		int16_t ax = op1_imu_read_acel_x();
		int16_t ay = op1_imu_read_acel_y();
		int16_t az = op1_imu_read_acel_z();
		int amag = sqrt(ax^2 + ay^2 + az^2);
		int value;
		value = abs16(op1_imu_read_gyro_z()) >> 8; 
		value = op1_mag_read_x();
		value = op1_mag_read_temp();
		value = value >> 8;
		GPIOD->ODR = (GPIOD->ODR & 0xFFFFFF00) | value & 0xFF;
	}
*/

}




