/*
 * UART debugger testing
 */

#include "./system_config/platform_init.h"
#include "./tools/print_scan.h"

int main() {
	init_platform();

	while(1) {
		printMsg("Hello!\n");
		nop(10000000);
	}
}
