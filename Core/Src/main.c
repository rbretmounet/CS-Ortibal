/*
 * UART debugger testing
 */

#include "./system_config/platform_init.h"
#include "./tools/print_scan.h"

int interrupt_count = 0;

int main() {
	init_platform();

	printMsg("Ready!\n");
	while(1) {

	}
}
