/*! \file base_serial.h
    \brief Serial Driver Implementation

	@author Daniel Casini, Emiliano Palermiti, Matteo Pampana
*/
#ifndef SERIAL_H
#define SERIAL_H


#include <stdio.h>
#include <stdarg.h>
#include <string.h>

/*! \fn 	void console_init()
    \brief  Initializes the Serial interface
*/
void console_init();

/*! \fn 	void console_out(char* str)
    \brief  Sends the string pointed by format to the Serial interface, until the character '\0' is reached
    \param str	Sequence to be sent
*/
void console_out(char *msg, unsigned int len);

/*! \fn 	void USART_printf ( const char * format, ... )
    \brief  Sends the string pointed by format to the Serial interface. If format includes format specifiers, the additional arguments following format are formatted and inserted in the resulting string replacing their respective specifiers.
    \param str	Sequence to be sent
*/
void USART_printf(const char *vectcStr,...);

/*! \fn 	void readChar(char *c)
    \brief  Waits until a character is available on the Serial and returns it
    \return Character received
*/
void readChar(char *c);

#endif
