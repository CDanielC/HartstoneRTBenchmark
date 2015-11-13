/*! \file base_serial.h
    \brief Serial Driver Implementation
	
	@author Daniel Casini, Emiliano Palermiti, Matteo Pampana
*/
#ifndef SERIAL_H
#define SERIAL_H

#include "stm32f4xx.h"
#include "stm32f4xx_usart.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include <stdio.h>
#include <stdarg.h>

  
/*! \fn 	void console_init()
    \brief  Initializes the USART interface
*/
void console_init();

/*! \fn 	void console_out(char* str)
    \brief  Sends the string pointed by format to the USART interface, until the character '\0' is reached
    \param str	Sequence to be sent
*/
void console_out(char* str);

/*! \fn 	void USART_printf ( const char * format, ... )
    \brief  Sends the string pointed by format to the USART interface. If format includes format specifiers, the additional arguments following format are formatted and inserted in the resulting string replacing their respective specifiers.
    \param str	Sequence to be sent
*/
void USART_printf(const char *vectcStr,...);

/*! \fn 	uint16_t readCharUSART()
    \brief  Waits until a character is available on the USART and returns it
    \return Character received
*/
uint16_t readCharUSART();

#endif /* SERIAL_H */

