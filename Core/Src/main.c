// opr1-quickstart1
// Quick start for Orbital Platform R1
// TCS 202304. Yume Research & Friends.
// The above attribution must be included regardless of license.
// This is not permissively licensed software.

#include <stm32l476xx.h>
#include <stm32l4xx_ll_i2c.h>

#define PIN_10 10
#define PIN_11
void nop(int nop_loops) {
	for (int i = 0; i < nop_loops; i++) {__NOP();}
}

int current_core_freq_MHz = 0;
void init_clocks() {
	// read chip identification information. only useful when watching memory through debugger
	volatile int id_w_x = (*(uint32_t*)(UID_BASE) & 0xFFFF0000) >> 16;
	volatile int id_w_y = *(uint32_t*)(UID_BASE) & 0xFFFF;
	volatile int id_w_n = *(uint32_t*)(UID_BASE+0x4) & 0xFF;
	volatile int id_lot_l = *(uint32_t*)(UID_BASE+0x4) & 0xFFFFFF00;
	volatile int id_lot_h = *(uint32_t*)(UID_BASE+0x8);
	volatile char id_lot[7] = {
		(id_lot_l >> 8) & 0xFF,
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
		| 1 << 22 // enable i2c2
		| 1 << 15 // enable spi3
		| 1 << 14 // enable spi2
		| 1 << 10; // enable rtc apb
	RCC->APB2ENR = 1; // enable syscfg
	FLASH->ACR = 0x600 | 4 | 1<<8; // flash access latency to four wait states, prefetch enable lol
	RCC->CR |= 1 << 8; // enable HSI
	RCC->PLLCFGR =
		0x0 << 25 // PLLR = 2
		| 1 << 24 // PLLREN = 1
		| 0x0 << 21 // PLLQ = 2
		| 1 << 20 // PLLQEN (PLL48M1CLK) output enabled
		| 0x0 << 17 // PLLP default
		| 0 << 16 // PLLSAI3 not enabled
		| 10 << 8 // PLLN = 10
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
	current_core_freq_MHz = 80;
}

int systick_time = 0;
int heartbeat_counter = 0;
void SysTick_Handler() {
	systick_time++;

	// use d7 to d0 to display system time
	//GPIOD->ODR &= 0xFFFFFF00;
	//GPIOD->ODR |= (systick_time >> 4) & 0xFF;

	// heartbeat led
	if (!(systick_time % 1000)) {
		heartbeat_counter = 100;
	}
	if (heartbeat_counter != 0) {
		GPIOE->BSRR = 1 << 2;
		heartbeat_counter--;
	} else {
		GPIOE->BSRR = 1 << (2+16);
	}
}

void init_nvic() {
	__disable_irq();
	SysTick->LOAD = current_core_freq_MHz/8*1000; // configure for 1 ms period, use AHB/8
	SysTick->CTRL = 0x3; // use AHB/8 as input clock, enable interrupts and counter
	NVIC_EnableIRQ(SysTick_IRQn);
	__enable_irq();
}

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

	// LEDS
	// 		D7 to D0 	D7 to D0
	// 		HEARTBEAT 	E2
	// 		A 			G11
	// 		B 			G12
	// 		C 			G9
	// 		FAULT		G7
	// 		AG 			G6
	// PINS
	// 		BTN1 		B11
	// 		BTN0 		B10
	GPIOB->MODER = 0x00000000; // digital input for btn 0 and 1
	GPIOB->PUPDR = 0x1 << (10*2) | 0x1 << (11*2); // pull up for btn0 and 1
	GPIOD->MODER = 0x00005555; // leds D7 to D0
	GPIOE->MODER = 0x1 << (2*2); // led
	GPIOG->MODER = 0x01445000; // leds
}

void gpio_set(GPIO_TypeDef * port, int pin, int value) {
	if (value) {
		port->BSRR = 1 << pin;
	} else {
		port->BSRR = 1 << (pin+16);
	}
}

int gpio_read(GPIO_TypeDef * port, int pin) {
	return port->IDR & (1 << pin);
}

void gpio_high(GPIO_TypeDef * port, int pin) {
	port->BSRR = 1 << pin;
}

void gpio_low(GPIO_TypeDef * port, int pin) {
	port->BSRR = 1 << (pin+16);
}

void op_led_a(int value) {
	gpio_set(GPIOG, 11, value);
}

void op_led_b(int value) {
	gpio_set(GPIOG, 12, value);
}

void op_led_c(int value) {
	gpio_set(GPIOG, 9, value);
}

void op_led_fault(int value) {
	gpio_set(GPIOG, 7, value);
}

void op_led_ag(int value) {
	gpio_set(GPIOG, 6, value);
}

void op_led_hb(int value) {
	gpio_set(GPIOE, 2, value);
}

void op_led_dx(int pin, int value) {
	if (pin & 0xFFFFFF00) return;
	gpio_set(GPIOD, pin, value);
}

void init_i2c2() {
	GPIOF->MODER = (2 << 0) | (2 << 1); // AF mode
	GPIOF->OTYPER = (1 << 0) | (1 << 1); // open drain
	GPIOF->OSPEEDR = (3 << 0) | (3 << 1); // very high speed
	GPIOF->PUPDR = (1 << 0) | (1 << 1); // also use internal pullups (extern is proper)
}

int main() {
	init_clocks();
	init_nvic();
	init_gpio();
	init_i2c2();


	while(1) { // blinky
		op_led_c(!gpio_read(GPIOB, 11));
		op_led_a(!gpio_read(GPIOB, 10));
		gpio_set(GPIOD, 0,1);
		gpio_set(GPIOD, 1,1);
		gpio_set(GPIOD, 2,1);
		gpio_set(GPIOD, 3,1);
		gpio_set(GPIOD, 4,1);
		gpio_set(GPIOD, 5,1);
		gpio_set(GPIOD, 6,1);
		gpio_set(GPIOD, 7,1);

	}
}





