/*! \file HWConfig.h
    \brief Specific Hardware configuration functions for dsPic

	@author Daniel Casini, Emiliano Palermiti, Matteo Pampana
*/
#include "globals.h"


#define FFRC 7370000    

/*! \def	PLLPREDIV
    \brief	PLL prescaler value
*/
#define PLLPREDIV 		2     // PLL prescaler value

/*! \def	PLLPOSTDIV
    \brief	PLL postscaler value
*/
#define PLLPOSTDIV 		2    

/*! \def	MIPS
    \brief	Desired operating speed (MIPS)
*/
#define MIPS 			40         

/*! \def	PLLMUL
    \brief	PLL constant to compute the desired clock frequency
*/
#define PLLMUL 			1000000*2*MIPS*PLLPREDIV*PLLPOSTDIV/FFRC

//#define FCY 			FFRC*PLLMUL/PLLPREDIV/PLLPOSTDIV/2*/

/*! \def	TICK_RATE_HZ
    \brief	Tick Hertz Correspondence
*/
#define TICK_RATE_HZ	1000

/*! \def	TIMER_PRESCALE
    \brief	Timer Prescaler parameter
*/
#define TIMER_PRESCALE	8 //related to the hw

/*! \fn		void InitClock()
    \brief  Initializes the clock frequency 
*/
void InitClock();

/*! \fn		void T1_program(void)
    \brief  Program the Timer1 peripheral to raise interrupts
*/
void T1_program(void);

/*! \fn		void T1_clear(void)
    \brief  Clear the Timer1 interrupt status flag 
*/
void T1_clear(void);
