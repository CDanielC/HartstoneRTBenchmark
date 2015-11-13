#define	__USE_STM32F4_DISCOVERY_COM__	1
#define	__USE_STM32F4_DISCOVERY_ALL_	1

#include "hartstone.h"

int main(void) {

	SystemInit();
	console_init();

	readCharUSART();

	hartstone_start();

	while(1);

	return 1;
}

