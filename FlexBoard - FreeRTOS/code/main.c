#include "hartstone.h"
#include <p33FJ256MC710.h>

_FOSCSEL(FNOSC_FRCPLL);
_FOSC(FCKSM_CSDCMD & OSCIOFNC_OFF & POSCMD_NONE);
_FWDT(FWDTEN_OFF);                    //Turn off WatchDog Timer
_FGS(GCP_OFF);                        //Turn off code protect
_FPOR(FPWRT_PWR1);                    //Turn off power up timer

#define FFRC 		7370000    
#define PLLPREDIV 	2     			// PLL prescaler value
#define PLLPOSTDIV 	2    			// PLL postscaler value
#define MIPS 		40         		// Desired operating speed (MIPS)

#define PLLMUL 		1000000*2*MIPS*PLLPREDIV*PLLPOSTDIV/FFRC 
#define FCY F		FRC*PLLMUL/PLLPREDIV/PLLPOSTDIV/2 


void vApplicationIdleHook( void )
{
	/* Schedule the co-routines from within the idle task hook. */
	vCoRoutineSchedule();
}


void InitClock() {

// Configure PLL prescaler, PLL postscaler, PLL divisor
PLLFBD = PLLMUL - 2; // M = PLLMUL (= 43 for FRC)
CLKDIVbits.PLLPOST = PLLPOSTDIV - 2;// N2 = 2
CLKDIVbits.PLLPRE = PLLPREDIV - 2; // N1 = 2
	/* Wait for PLL to lock */
	while(OSCCONbits.LOCK!=1);
}

int main(void) {

	char start_char;

	InitClock();	// This is the PLL settings

	console_init();
	
	/* Output message */
	USART_printf("*********************************\r\n");
	USART_printf("****** Hello From FreeRTOS ******\r\n");
	USART_printf("*********************************\r\n");

	USART_printf("Press t to run the benchmark \r\n");
	do{
		readChar(&start_char);	
	}while(start_char != 't');


	hartstone_start();

	while(1);

	return 1;
}

