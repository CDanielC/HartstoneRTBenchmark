#include "HWConfig.h"

void InitClock() {

// Configure PLL prescaler, PLL postscaler, PLL divisor
PLLFBD = PLLMUL - 2; // M = PLLMUL (= 43 for FRC)
CLKDIVbits.PLLPOST = PLLPOSTDIV - 2;// N2 = 2
CLKDIVbits.PLLPRE = PLLPREDIV - 2; // N1 = 2
	while(OSCCONbits.LOCK!=1);
}

/* Program the Timer1 peripheral to raise interrupts */

void T1_program(void)
{
	T1CON = 0;		/* Stops the Timer1 and reset control reg	*/
	TMR1  = 0;		/* Clear contents of the timer register	*/
	//PR1   = 0xffff;		/* Load the Period register wit the value 0xffff	*/

	/* The PR1 value is computed as ( ( FCY / TIMER_PRESCALE  ) / TICK_RATE_HZ ) - 1
	 * in order to obtain a tick every 1 ms
	 */
	PR1   = 4999; /* Load the Period register	*/

	IPC0bits.T1IP = 5;	/* Set Timer1 priority to 1		*/
	IFS0bits.T1IF = 0;	/* Clear the Timer1 interrupt status flag	*/
	IEC0bits.T1IE = 1;	/* Enable Timer1 interrupts		*/


	/* Setup the prescale value. */
		T1CONbits.TCKPS0 = 1;
		T1CONbits.TCKPS1 = 0;



	T1CONbits.TON = 1;	/* Start Timer1 with prescaler settings at 1:1
				* and clock source set to the internal
				* instruction cycle			*/

}

/* Clear the Timer1 interrupt status flag */
void T1_clear(void)
{
	IFS0bits.T1IF = 0;
}
