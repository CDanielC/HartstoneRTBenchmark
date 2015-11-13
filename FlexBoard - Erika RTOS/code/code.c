#include "ee.h"
#include "ee_irq.h"
#include "whetstone.h"
#include "globals.h"
#include "HWConfig.h"



_FOSCSEL(FNOSC_FRCPLL);
_FOSC(FCKSM_CSDCMD & OSCIOFNC_OFF & POSCMD_NONE);
_FWDT(FWDTEN_OFF);                    //Turn off WatchDog Timer
_FGS(GCP_OFF);                        //Turn off code protect
_FPOR(FPWRT_PWR1);                    //Turn off power up timer

/* This is an ISR Type 2 which is attached to the Timer 1 peripheral IRQ pin
 * The ISR simply calls CounterTick to implement the timing reference
 */

ISR2(_T1Interrupt)
{
	/* clear the interrupt source */
	T1_clear();

	/* count the interrupts, waking up expired alarms */
	CounterTick(myCounter);
}

char buffer[100];





int main(void)
{
	char start_char;
	uint16_t n_chars;
	/* Clock setup */
	InitClock();

	/* Program Timer 1 to raise interrupts */
	T1_program();

	/* Init leds */
	EE_leds_init();
	EE_demoboard_leds_init();

	console_init();
	USART_printf("Hello From Erika RTOS, press t to run the benchmark \r\n");
	//calcRawSpeed();


	do{
		readChar(&start_char);
	}while(start_char != 't');
	//USART_printf("T ok \r\n");
	stop = 0;

	uint8_t i;
	calcRawSpeed();
	initState(kwips_rawspeed, initAlarms, initFreq, initKWPP);

	//for(i=0; i<9999; i++){}

	ActivateTask(SuperTask);

	//for(i=0; i<9999; i++){}

	StartOS(OSDEFAULTAPPMODE);

	/* Forever loop: background activities (if any) should go here */
	for (;;);

	return 0;
}
