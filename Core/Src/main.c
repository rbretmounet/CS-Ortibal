// opr1-quickstart1 
// Quick start for Orbital Platform R1 
// TCS 202304. Yume Research & Friends. 
// The above attribution must be included regardless of license. 
// This is not permissively licensed software. 

#include "./system/stm32l476xx.h"
#include <math.h> // for sqrt 
typedef enum { false, true } bool;

void nop(int nop_loops) {
	for (int i = 0; i < nop_loops; i++) {__NOP();}
}

int core_MHz = 0;

/**
 * Initializes the clocks of the microcontroller.
 *
 * This function sets up the various clock sources and frequencies for the microcontroller.
 * It also reads the unique ID of the microcontroller and stores it in an array.
 *
 * @returns None
 */
void init_clocks() {
	// read chip identification information. only useful when watching memory through debugger 
	volatile int id_w_x = (*(uint32_t*)(UID_BASE) & 0xFFFF0000) >> 16;
	volatile int id_w_y = *(uint32_t*)(UID_BASE) & 0xFFFF;
	volatile int id_w_n = *(uint32_t*)(UID_BASE+0x4) & 0xFF;
	volatile int id_lot_l = *(uint32_t*)(UID_BASE+0x4) & 0xFFFFFF00;
	volatile int id_lot_h = *(uint32_t*)(UID_BASE+0x8);
	volatile char id_lot[7] = {
		(id_lot_l >> 8) & 0xFF,//test
		(id_lot_l >> 16) & 0xFF,
		(id_lot_l >> 24) & 0xFF,
		(id_lot_h) & 0xFF,
		(id_lot_h >> 8) & 0xFF,
		(id_lot_h >> 16) & 0xFF,
		(id_lot_h >> 24) & 0xFF
	};
	RCC->AHB2ENR = 0x000530FF; // enable clock to all gpio banks, usb otg, adc, crypto 
	RCC->AHB3ENR = 0x00000100; // enable clock to QSPI 
	RCC->APB1ENR1 = 
		1 << 28 // enable power control 
		| 1 << 22 // enable i2c2 //test change
		| 1 << 15 // enable spi3 
		| 1 << 14 // enable spi2 
		| 1 << 10; // enable rtc apb 
	RCC->APB2ENR = 1; // enable syscfg 
	FLASH->ACR = 0x600 | 4 | 1<<8; // flash access latency to four wait states, prefetch enable lol 
	//FLASH->ACR = 0x604; // OVERCLOCK! for testing only 
	RCC->CR |= 1 << 8; // enable HSI 
	RCC->PLLCFGR = 
		0x0 << 25 // PLLR = 2 
		| 1 << 24 // PLLREN = 1 
		| 0x0 << 21 // PLLQ = 2 
		| 1 << 20 // PLLQEN (PLL48M1CLK) output enabled 
		| 0x0 << 17 // PLLP default 
		| 0 << 16 // PLLSAI3 not enabled  
		 | 10 << 8 // PLLN = 10 
	//	| 20 << 8 // OVERCLOCK! for testing only 
		| 0 << 4 // PLLM = 1 
		| 0x2; // HSI16 as input clock to PLLs 
	RCC->PLLSAI1CFGR = 
		0x1 << 25 // PLLSAI1R = 4 
		| 1 << 24 // PLLSAI1REN = PLLADC1CLK enable 
		| 0x1 << 21 // PLLSAI1Q = 4
		| 1 << 20 // PLLSAI1QEN = PLL48M2CLK enable
		| 0x0 << 17 // PLLSAI1P = default 
		| 0 << 16 // PLLSAI1PEN = 0
		| 12 << 8; // PLLSAI1N = 12 
	RCC->CR |= (1 << 24) | (1 << 26); // enable PLL and PLLSAI1 
	while (!(RCC->CR & (1 << 25))); // wait for PLL lock 
	while (!(RCC->CR & (1 << 27))); // wait for PLLSAI1 lock 
	RCC->CFGR = 0x3; // system clock to PLL 
	core_MHz = 80;
}

/**
 * Initializes the GPIO pins for the microcontroller.
 *
 * @param None
 *
 * @returns None
 */
void init_gpio() {
	PWR->CR2 |= 1 << 9; // enable VDDIO2 supply for GPIOG 
	// wait until each GPIO is clocked and ready 
	while (GPIOA->OTYPER == 0xFFFFFFFF); 
	while (GPIOB->OTYPER == 0xFFFFFFFF); 
	while (GPIOC->OTYPER == 0xFFFFFFFF); 
	while (GPIOD->OTYPER == 0xFFFFFFFF); 
	while (GPIOE->OTYPER == 0xFFFFFFFF); 
	while (GPIOF->OTYPER == 0xFFFFFFFF); 
	while (GPIOG->OTYPER == 0xFFFFFFFF); 
	// OP R1 gpio pinout 
	// 		D7 to D0 	D7 to D0 
	// 		HEARTBEAT 	E2 
	// 		A 			G11 
	// 		B 			G12 
	// 		C 			G9 
	// 		FAULT		G7 
	// 		AG 			G6 
	// 		BTN1 		B11 
	// 		BTN0 		B10 
	GPIOB->MODER = 0x00000000; // digital input for btn 0 and 1 
	GPIOB->PUPDR = 0x1 << (10*2) | 0x1 << (11*2); // pull up for btn0 and 1 
	GPIOD->MODER = 0x00005555; // leds D7 to D0 
	GPIOE->MODER = 0x1 << (2*2); // led 
	GPIOG->MODER = 0x01445000; // leds 
}

/**
 * Sets the specified GPIO pin to high.
 *
 * @param port The GPIO port to which the pin belongs.
 * @param pin The pin number to set high.
 *
 * @returns None
 */
void gpio_high(GPIO_TypeDef * port, int pin) {
	port->BSRR = 1 << pin;
}

/**
 * Sets the specified GPIO pin to low logic level.
 *
 * @param port The GPIO port to which the pin belongs.
 * @param pin The pin number to set to low.
 *
 * @returns None
 */
void gpio_low(GPIO_TypeDef * port, int pin) {
	port->BSRR = 1 << (pin+16);
}

/**
 * Sets the value of a GPIO pin on a given port.
 *
 * @param port The GPIO port to set the pin on.
 * @param pin The pin number to set.
 * @param value The value to set the pin to (1 for high, 0 for low).
 *
 * @returns None
 */
void gpio_set(GPIO_TypeDef * port, int pin, int value) {
	if (value) {
		gpio_high(port, pin);
	} else {
		gpio_low(port, pin);
	}
}

/**
 * Reads the value of a GPIO pin.
 *
 * @param port The GPIO port to read from.
 * @param pin The pin number to read.
 *
 * @returns The value of the GPIO pin (1 or 0).
 */
int gpio_read(GPIO_TypeDef * port, int pin) {
	if (port->IDR & (1 << pin)) {
		return 1;
	} else {
		return 0;
	}
}

/**
 * Sets the state of a LED A connected to a GPIO pin.
 *
 * @param value The value to set the LED to (0 off or 1 on).
 *
 * @returns None
 */
void op_led_a(int value) {
	gpio_set(GPIOG, 11, value);
}

/**
 * Sets the state of an LED B connected to a GPIO pin.
 *
 * @param value The value to set the LED to (0 off or 1 on).
 *
 * @returns None
 */
void op_led_b(int value) {
	gpio_set(GPIOG, 12, value);
}

/**
 * Sets the state of an LED C connected to a GPIO pin.
 *
 * @param value The value to set the LED to (0 off or 1 on).
 *
 * @returns None
 */
void op_led_c(int value) {
	gpio_set(GPIOG, 9, value);
}

/**
 * Sets the state of the LED fault indicator.
 *
 * @param value The value to set the LED fault indicator to (0 off or 1 on).
 *
 * @returns None
 */
void op_led_fault(int value) {
	gpio_set(GPIOG, 7, value);
}

/**
 * Sets the state of an LED connected to a GPIO pin.
 *
 * @param value The value to set the LED to (0 off or 1 on).
 *
 * @returns None
 */
void op_led_ag(int value) {
	gpio_set(GPIOG, 6, value);
}

/**
 * Sets the state of an Heart Beat LED connected to a GPIO pin.
 *
 * @param value The value to set the LED to (0 off or 1 on).
 *
 * @returns None
 */
void op_led_hb(int value) {
	gpio_set(GPIOE, 2, value);
}

/**
 * Sets the value of a specified LED D.
 *
 * @param pin The pin number to set the value for.
 * @param value The value to set the pin to.
 *
 * @returns None
 */
void op_led_dx(int pin, int value) {
	if (pin & 0xFFFFFF00) return; 
	gpio_set(GPIOD, pin, value);
}

int systick_time = 0; 
int heartbeat_counter = 0;
int ag_counter = 0;
/**
 * Interrupt handler for the SysTick timer.
 * Increments the systick_time variable and updates the status of the heartbeat and activity LEDs.
 *
 * @param None
 *
 * @returns None
 */
void SysTick_Handler() {
	systick_time++;
	
	// use d7 to d0 to display system time 
//	GPIOD->ODR = (GPIOD->ODR & 0xFFFFFF00) | (systick_time >> 4) & 0xFF; 
	
	// heartbeat led 
	if (!(systick_time % 1000)) {
		heartbeat_counter = 100;
	}
	if (!(systick_time % 250)) {
		ag_counter = 10;
	}
	if (heartbeat_counter) {
		op_led_hb(1);
		heartbeat_counter--;
	} else {
		op_led_hb(0);
	}
	if (ag_counter) {
		op_led_ag(1);
		ag_counter--;
	} else {
		op_led_ag(0);
	}
}

/**
 * Initializes the Nested Vector Interrupt Controller (NVIC) for the SysTick timer.
 * The SysTick timer is used for generating periodic interrupts.
 *
 * @param None
 *
 * @returns None
 */
void init_nvic() {
	__disable_irq();
	SysTick->LOAD = core_MHz/8*1000; // configure for 1 ms period, use AHB/8 
	SysTick->CTRL = 0x3; // use AHB/8 as input clock, enable interrupts and counter 
	NVIC_EnableIRQ(SysTick_IRQn);
	__enable_irq();
}

/**
 * Sets the line mode of a GPIO pin for SoftI2C communication.
 *
 * @param port The GPIO port to set the line mode for.
 * @param pin The pin number to set the line mode for.
 * @param deassert_line A boolean indicating whether to deassert the line or not.
 *                      If true, the line is deasserted. If false, the line is set to output mode.
 *
 * @returns None
 */
void softi2c_line_mode(GPIO_TypeDef * port, int pin, bool deassert_line) {
	if (deassert_line) {
		port->MODER &= ~(0x3 << (pin*2)); 
	} else {
		port->MODER = (port->MODER & ~(0x3 << (pin*2))) | (1 << pin*2); 
	}
}

/**
 * Initializes the software implementation of the I2C protocol.
 *
 * @param scl_port The GPIO port for the SCL line.
 * @param scl_pin The GPIO pin for the SCL line.
 * @param sda_port The GPIO port for the SDA line.
 * @param sda_pin The GPIO pin for the SDA line.
 *
 * @returns None
 */
void init_softi2c(GPIO_TypeDef * scl_port, int scl_pin, GPIO_TypeDef * sda_port, int sda_pin) {
	scl_port->OTYPER = (scl_port->OTYPER & ~(1 << scl_pin)) | (0 << scl_pin); // push pull 
	sda_port->OTYPER = (sda_port->OTYPER & ~(1 << sda_pin)) | (0 << sda_pin); 
	scl_port->OSPEEDR = (scl_port->OSPEEDR & ~(0x3 << (scl_pin*2))) | (3 << scl_pin*2); // very high speed 
	sda_port->OSPEEDR = (sda_port->OSPEEDR & ~(0x3 << (sda_pin*2))) | (3 << sda_pin*2); 
	scl_port->BSRR = 1 << (scl_pin + 16); // drive low when asserted 
	sda_port->BSRR = 1 << (sda_pin + 16);
	softi2c_line_mode(scl_port, scl_pin, 1); 
	softi2c_line_mode(sda_port, sda_pin, 1); 
}

/**
 * Delays the execution of the program for a short period of time to allow for proper timing in the SoftI2C protocol.
 *
 * @returns None
 */
void softi2c_delay() {
	nop(30);
}

/**
 * Generates the start signal for a software-based I2C communication.
 *
 * @param scl_port The GPIO port for the SCL line.
 * @param scl_pin The GPIO pin for the SCL line.
 * @param sda_port The GPIO port for the SDA line.
 * @param sda_pin The GPIO pin for the SDA line.
 *
 * @returns None
 */
void softi2c_sig_start(GPIO_TypeDef * scl_port, int scl_pin, GPIO_TypeDef * sda_port, int sda_pin) {
	softi2c_line_mode(scl_port, scl_pin, 1);
	softi2c_line_mode(sda_port, sda_pin, 1);
	softi2c_delay(); softi2c_delay();
	softi2c_line_mode(sda_port, sda_pin, 0);
	softi2c_delay(); softi2c_delay();
	softi2c_line_mode(scl_port, scl_pin, 0);
	softi2c_delay(); 
}

/**
 * Generates a repeated start signal for the SoftI2C protocol.
 *
 * @param scl_port The GPIO port for the SCL line.
 * @param scl_pin The GPIO pin for the SCL line.
 * @param sda_port The GPIO port for the SDA line.
 * @param sda_pin The GPIO pin for the SDA line.
 *
 * @returns None
 */
void softi2c_sig_repeated_start(GPIO_TypeDef * scl_port, int scl_pin, GPIO_TypeDef * sda_port, int sda_pin) {
	softi2c_line_mode(scl_port, scl_pin, 0);
	softi2c_line_mode(sda_port, sda_pin, 1);
	softi2c_delay();
	softi2c_line_mode(scl_port, scl_pin, 1);
	softi2c_delay(); softi2c_delay();
	softi2c_line_mode(sda_port, sda_pin, 0);
	softi2c_delay(); softi2c_delay();
	softi2c_line_mode(scl_port, scl_pin, 0);
	softi2c_delay(); 
}

/**
 * Sends a stop signal on the I2C bus using software implementation.
 *
 * @param scl_port The GPIO port of the SCL line.
 * @param scl_pin The GPIO pin of the SCL line.
 * @param sda_port The GPIO port of the SDA line.
 * @param sda_pin The GPIO pin of the SDA line.
 *
 * @returns None
 */
void softi2c_sig_stop(GPIO_TypeDef * scl_port, int scl_pin, GPIO_TypeDef * sda_port, int sda_pin) {
	softi2c_line_mode(scl_port, scl_pin, 0);
	softi2c_line_mode(sda_port, sda_pin, 0);
	softi2c_delay(); softi2c_delay();
	softi2c_line_mode(scl_port, scl_pin, 1);
	softi2c_delay(); softi2c_delay();
	softi2c_line_mode(sda_port, sda_pin, 1);
	softi2c_delay(); 
}

/**
 * Sends an 8-bit data over a software I2C bus.
 *
 * @param scl_port The GPIO port of the SCL line.
 * @param scl_pin The GPIO pin of the SCL line.
 * @param sda_port The GPIO port of the SDA line.
 * @param sda_pin The GPIO pin of the SDA line.
 * @param data8 The 8-bit data to be sent.
 *
 * @returns None
 */
void softi2c_send8(GPIO_TypeDef * scl_port, int scl_pin, GPIO_TypeDef * sda_port, int sda_pin, int data8) {
	for (int i = 0; i < 8; i++) {
		softi2c_line_mode(scl_port, scl_pin, 0);
		softi2c_delay();
		softi2c_line_mode(sda_port, sda_pin, (data8 << i) & (1 << 7));
		softi2c_delay(); 
		softi2c_line_mode(scl_port, scl_pin, 1);
		softi2c_delay(); softi2c_delay();
	}
}

/**
 * Reads 8 bits of data from a SoftI2C bus.
 *
 * @param scl_port The GPIO port of the SCL line.
 * @param scl_pin The GPIO pin of the SCL line.
 * @param sda_port The GPIO port of the SDA line.
 * @param sda_pin The GPIO pin of the SDA line.
 *
 * @returns The 8 bits of data read from the SoftI2C bus.
 */
int softi2c_read8(GPIO_TypeDef * scl_port, int scl_pin, GPIO_TypeDef * sda_port, int sda_pin) {
	int data = 0; 
	softi2c_line_mode(sda_port, sda_pin, 1);
	for (int i = 0; i < 8; i++) {
		softi2c_line_mode(scl_port, scl_pin, 0);
		softi2c_delay();
		data |= gpio_read(sda_port, sda_pin) << (7-i);
		softi2c_delay(); 
		softi2c_line_mode(scl_port, scl_pin, 1);
		softi2c_delay(); softi2c_delay();
	}
	return data;
}

/**
 * Sends a NACK (Not Acknowledged) signal over a software I2C bus.
 *
 * @param scl_port The GPIO port of the SCL line.
 * @param scl_pin The GPIO pin of the SCL line.
 * @param sda_port The GPIO port of the SDA line.
 * @param sda_pin The GPIO pin of the SDA line.
 *
 * @returns None
 */
void softi2c_send_nack(GPIO_TypeDef * scl_port, int scl_pin, GPIO_TypeDef * sda_port, int sda_pin) {
	softi2c_line_mode(scl_port, scl_pin, 0);
	softi2c_delay();
	softi2c_line_mode(sda_port, sda_pin, 1);
	softi2c_delay(); 
	softi2c_line_mode(scl_port, scl_pin, 1);
	softi2c_delay(); 
}

/**
 * Reads a single byte from a SoftI2C device with a NACK signal.
 *
 * @param scl_port The GPIO port of the SCL line.
 * @param scl_pin The GPIO pin of the SCL line.
 * @param sda_port The GPIO port of the SDA line.
 * @param sda_pin The GPIO pin of the SDA line.
 *
 * @returns The byte read from the SoftI2C device.
 */
int softi2c_read_nack(GPIO_TypeDef * scl_port, int scl_pin, GPIO_TypeDef * sda_port, int sda_pin) {
	softi2c_line_mode(scl_port, scl_pin, 0);
	softi2c_delay();
	softi2c_line_mode(sda_port, sda_pin, 1);
	softi2c_delay(); 
	softi2c_line_mode(scl_port, scl_pin, 1);
	softi2c_delay(); 
	int nack = gpio_read(sda_port, sda_pin);
	softi2c_delay();
	return nack; 
}

/**
 * Writes data to a register of a device using software I2C protocol.
 *
 * @param scl_port The GPIO port of the SCL pin.
 * @param scl_pin The pin number of the SCL pin.
 * @param sda_port The GPIO port of the SDA pin.
 * @param sda_pin The pin number of the SDA pin.
 * @param device_addr The address of the device to write to.
 * @param reg_addr The address of the register to write to.
 * @param data The data to write to the register.
 *
 * @returns The number of NACKs received during the write operation.
 */
int softi2c_write_reg(GPIO_TypeDef * scl_port, int scl_pin, GPIO_TypeDef * sda_port, int sda_pin, int device_addr, int reg_addr, int data) {
	int nack = 0; 
	softi2c_sig_start(scl_port, scl_pin, sda_port, sda_pin);
	softi2c_send8(scl_port, scl_pin, sda_port, sda_pin, device_addr << 1 | 0);
	nack += softi2c_read_nack(scl_port, scl_pin, sda_port, sda_pin);
	softi2c_send8(scl_port, scl_pin, sda_port, sda_pin, reg_addr);
	nack += softi2c_read_nack(scl_port, scl_pin, sda_port, sda_pin);
	softi2c_send8(scl_port, scl_pin, sda_port, sda_pin, data);
	nack += softi2c_read_nack(scl_port, scl_pin, sda_port, sda_pin);
	softi2c_sig_stop(scl_port, scl_pin, sda_port, sda_pin);
	return nack; 
}

/**
 * Reads a register from a device using software I2C protocol.
 *
 * @param scl_port The GPIO port of the SCL pin.
 * @param scl_pin The GPIO pin number of the SCL pin.
 * @param sda_port The GPIO port of the SDA pin.
 * @param sda_pin The GPIO pin number of the SDA pin.
 * @param device_addr The address of the device to read from.
 * @param reg_addr The address of the register to read.
 *
 * @returns The value of the register read from the device.
 */
int softi2c_read_reg(GPIO_TypeDef * scl_port, int scl_pin, GPIO_TypeDef * sda_port, int sda_pin, int device_addr, int reg_addr) {
	int nack = 0; 
	softi2c_sig_start(scl_port, scl_pin, sda_port, sda_pin);
	softi2c_send8(scl_port, scl_pin, sda_port, sda_pin, device_addr << 1 | 0);
	nack += softi2c_read_nack(scl_port, scl_pin, sda_port, sda_pin);
	softi2c_send8(scl_port, scl_pin, sda_port, sda_pin, reg_addr);
	nack += softi2c_read_nack(scl_port, scl_pin, sda_port, sda_pin);
	softi2c_sig_repeated_start(scl_port, scl_pin, sda_port, sda_pin);
	softi2c_send8(scl_port, scl_pin, sda_port, sda_pin, device_addr << 1 | 1);
	nack += softi2c_read_nack(scl_port, scl_pin, sda_port, sda_pin);
	int data = softi2c_read8(scl_port, scl_pin, sda_port, sda_pin);
	softi2c_send_nack(scl_port, scl_pin, sda_port, sda_pin);
	softi2c_sig_stop(scl_port, scl_pin, sda_port, sda_pin);
	return data;
}

/**
 * Probes a device on the I2C bus using software I2C.
 *
 * @param scl_port The GPIO port for the SCL pin.
 * @param scl_pin The GPIO pin for the SCL pin.
 * @param sda_port The GPIO port for the SDA pin.
 * @param sda_pin The GPIO pin for the SDA pin.
 * @param device_addr The address of the device to probe.
 *
 * @returns True if the device is present on the bus, false otherwise.
 */
bool softi2c_probe(GPIO_TypeDef * scl_port, int scl_pin, GPIO_TypeDef * sda_port, int sda_pin, int device_addr) {
	int nack = 0; 
	softi2c_sig_start(scl_port, scl_pin, sda_port, sda_pin);
	softi2c_send8(scl_port, scl_pin, sda_port, sda_pin, device_addr << 1 | 1);
	nack += softi2c_read_nack(scl_port, scl_pin, sda_port, sda_pin);
	softi2c_sig_stop(scl_port, scl_pin, sda_port, sda_pin);
	if (nack) {
		return 0;
	} else {
		return 1; 
	}
}

/**
 * Reads a 16-bit register value from a device using software I2C protocol.
 *
 * @param scl_port The GPIO port of the SCL pin.
 * @param scl_pin The pin number of the SCL pin.
 * @param sda_port The GPIO port of the SDA pin.
 * @param sda_pin The pin number of the SDA pin.
 * @param device_addr The address of the device to read from.
 * @param high_reg_addr The address of the high byte of the register to read.
 * @param low_reg_addr The address of the low byte of the register to read.
 *
 * @returns The 16-bit register value read from the device.
 */
int16_t softi2c_read_reg_hl(GPIO_TypeDef * scl_port, int scl_pin, GPIO_TypeDef * sda_port, int sda_pin, int device_addr, int high_reg_addr, int low_reg_addr) {
	return softi2c_read_reg(scl_port, scl_pin, sda_port, sda_pin, device_addr, high_reg_addr) << 8 | softi2c_read_reg(scl_port, scl_pin, sda_port, sda_pin, device_addr, low_reg_addr);
}

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

#define OP1_I2C2 GPIOF, 1, GPIOF, 0
#define IMU_ADDR 0x6A 
#define MAG_ADDR 0x0D 

#define IMU_ODR_OFF 		0 
#define IMU_ODR_12_5_Hz 	1 
#define IMU_ODR_26_Hz 		2 
#define IMU_ODR_52_Hz 		3 
#define IMU_ODR_104_Hz 		4 
#define IMU_ODR_208_Hz 		5 
#define IMU_ODR_417_Hz 		6 
#define IMU_ODR_833_Hz 		7 
#define IMU_ODR_1667_Hz 	8 
#define IMU_ODR_3333_Hz 	9 
#define IMU_ODR_6667_Hz 	10 
#define IMU_FS_2_g 			0 
#define IMU_FS_4_g 			2 
#define IMU_FS_8_g 			3 
#define IMU_FS_16_g 		1 
#define IMU_FS_125_dps 		2 
#define IMU_FS_250_dps 		0 
#define IMU_FS_500_dps 		4 
#define IMU_FS_1000_dps 	8 
#define IMU_FS_2000_dps 	12 
#define IMU_FS_4000_dps 	1 

#define MAG_ODR_OFF 		0 
#define MAG_ODR_10_Hz 		1 
#define MAG_ODR_50_Hz 		2 
#define MAG_ODR_100_Hz 		3 
#define MAG_ODR_200_Hz		4 
#define MAG_FS_2_G 			0 // btw: "G" is not a typo, it stands for gauss 
#define MAG_FS_8_G	 		1 
#define MAG_OVERSAMPLE_512 	0 
#define MAG_OVERSAMPLE_256 	1 
#define MAG_OVERSAMPLE_128 	2 
#define MAG_OVERSAMPLE_64  	3 

/**
 * Configures the accelerometer control register of an IMU device.
 *
 * @param acel_rate The rate of the accelerometer.
 * @param acel_scale The scale of the accelerometer.
 * @param digital_filter_on Whether the digital filter is enabled or not.
 *
 * @returns None
 */
void op1_imu_acel_ctrl(int acel_rate, int acel_scale, int digital_filter_on) {
	acel_rate &= 0xFF;
	acel_scale &= 0xF;
	digital_filter_on &= 1;
	int data = acel_rate << 4 | acel_scale << 2 | digital_filter_on << 1;
	softi2c_write_reg(OP1_I2C2, IMU_ADDR, 0x10, data); 
}

/**
 * Configures the gyroscope rate and scale for the IMU sensor.
 *
 * @param gyro_rate The rate of the gyroscope.
 * @param gyro_scale The scale of the gyroscope.
 *
 * @returns None
 */
void op1_imu_gyro_ctrl(int gyro_rate, int gyro_scale) {
	gyro_rate &= 0xFF;
	gyro_scale &= 0xFF; 
	int data = gyro_rate << 4 | gyro_scale;
	softi2c_write_reg(OP1_I2C2, IMU_ADDR, 0x11, data); 
}

/**
 * Initializes the OP1 IMU with the given accelerometer and gyroscope settings.
 *
 * @param acel_rate The accelerometer data rate.
 * @param acel_scale The accelerometer full scale range.
 * @param gyro_rate The gyroscope data rate.
 * @param gyro_scale The gyroscope full scale range.
 *
 * @returns None
 */
void op1_imu_init(int acel_rate, int acel_scale, int gyro_rate, int gyro_scale) {
	softi2c_write_reg(OP1_I2C2, IMU_ADDR, 0x12, 0x01); // soft reset imu  
	nop(100); 
	op1_imu_acel_ctrl(acel_rate, acel_scale, 0);
	op1_imu_gyro_ctrl(gyro_rate, gyro_scale);
} 

/**
 * Reads the X-axis acceleration value from the IMU sensor connected to the I2C2 bus of OP1.
 *
 * @returns The X-axis acceleration value as a 16-bit signed integer.
 */
int16_t op1_imu_read_acel_x() {
	return softi2c_read_reg_hl(OP1_I2C2, IMU_ADDR, 0x29, 0x28);
}

/**
 * Reads the y-axis acceleration value from the IMU sensor connected to the I2C2 bus of OP1.
 *
 * @returns The y-axis acceleration value as a 16-bit signed integer.
 */
int16_t op1_imu_read_acel_y() {
	return softi2c_read_reg_hl(OP1_I2C2, IMU_ADDR, 0x2B, 0x2A);
}

/**
 * Reads the acceleration value of the z-axis from the IMU sensor connected to the I2C2 bus of OP1.
 *
 * @returns The acceleration value of the z-axis.
 */
int16_t op1_imu_read_acel_z() {
	return softi2c_read_reg_hl(OP1_I2C2, IMU_ADDR, 0x2D, 0x2C);
}

/**
 * Reads the X-axis gyroscope data from the IMU sensor connected to the I2C2 bus of OP1.
 *
 * @returns The X-axis gyroscope data as a 16-bit signed integer.
 */
int16_t op1_imu_read_gyro_x() {
	return softi2c_read_reg_hl(OP1_I2C2, IMU_ADDR, 0x23, 0x22);
}

/**
 * Reads the y-axis gyroscope value from the IMU sensor connected to the I2C2 bus of OP1.
 *
 * @returns The y-axis gyroscope value as a 16-bit signed integer.
 */
int16_t op1_imu_read_gyro_y() {
	return softi2c_read_reg_hl(OP1_I2C2, IMU_ADDR, 0x25, 0x24);
}

/**
 * Reads the Z-axis gyroscope data from the IMU sensor connected to the I2C2 bus of OP1.
 *
 * @returns The Z-axis gyroscope data as a 16-bit signed integer.
 */
int16_t op1_imu_read_gyro_z() {
	return softi2c_read_reg_hl(OP1_I2C2, IMU_ADDR, 0x27, 0x26);
}

/**
 * Reads the temperature from the IMU sensor connected to the I2C2 bus of OP1.
 *
 * @returns The temperature value in 16-bit signed integer format.
 */
int16_t op1_imu_read_temp() {
	return softi2c_read_reg_hl(OP1_I2C2, IMU_ADDR, 0x21, 0x20);
}

/**
 * Configures the control registers of the OP1 magnetometer.
 *
 * @param rate The output data rate of the magnetometer.
 * @param scale The full scale range of the magnetometer.
 * @param oversample The number of samples to average for each measurement.
 *
 * @returns None
 */
void op1_mag_ctrl(int rate, int scale, int oversample) {
	if (rate) {
		softi2c_write_reg(OP1_I2C2, MAG_ADDR, 0x09, oversample << 6 | scale << 4 | (rate-1) << 2 | 1); 
	} else {
		softi2c_write_reg(OP1_I2C2, MAG_ADDR, 0x09, 0);
	}
}

/**
 * Initializes the magnetometer for operation.
 *
 * @param rate The data output rate of the magnetometer.
 * @param scale The full-scale range of the magnetometer.
 * @param oversample The oversampling rate of the magnetometer.
 *
 * @returns None
 */
void op1_mag_init(int rate, int scale, int oversample) {
	softi2c_write_reg(OP1_I2C2, MAG_ADDR, 0x0A, 0x80); // soft reset 
	nop(50); 
	softi2c_write_reg(OP1_I2C2, MAG_ADDR, 0x0A, 0x00); 
	softi2c_write_reg(OP1_I2C2, MAG_ADDR, 0x0B, 1);
	nop(50); 
	op1_mag_ctrl(rate, scale, oversample);
}

/**
 * Reads the x-axis magnetometer value from the OP1 sensor.
 *
 * @returns The x-axis magnetometer value.
 */
int16_t op1_mag_read_x() {
	return softi2c_read_reg_hl(OP1_I2C2, MAG_ADDR, 0x01, 0x00);
}

/**
 * Reads the Y-axis magnetometer value from the OP1 sensor.
 *
 * @returns The Y-axis magnetometer value as a 16-bit integer.
 */
int16_t op1_mag_read_y() {
	return softi2c_read_reg_hl(OP1_I2C2, MAG_ADDR, 0x03, 0x02);
}

/**
 * Reads the magnitude value of the z-axis from the magnetometer connected to the I2C2 bus of OP1.
 *
 * @returns The magnitude value of the z-axis.
 */
int16_t op1_mag_read_z() {
	return softi2c_read_reg_hl(OP1_I2C2, MAG_ADDR, 0x05, 0x04);
}

/**
 * Reads the temperature from the magnetometer connected to OP1 via I2C2.
 *
 * @returns The temperature value in 16-bit signed integer format.
 */
int16_t op1_mag_read_temp() {
	return softi2c_read_reg_hl(OP1_I2C2, MAG_ADDR, 0x08, 0x07);
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
			state = (state + 1) % 5;
			btn_debounce = 100;
		} else if (btn_debounce) {
			btn_debounce--;
		}
		
		switch (state) {
			case 0: 
				GPIOD->ODR = (GPIOD->ODR & 0xFFFFFF00) | (op1_imu_read_acel_x()>>8) & 0xFF;
				op_led_a(1);
				op_led_b(systick_time & 1 << 6);
				break; 
			case 1: 
				GPIOD->ODR = (GPIOD->ODR & 0xFFFFFF00) | (abs16(op1_imu_read_gyro_z())>>8) & 0xFF;
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
}




