#include "base_serial.h"

/**************************************************************************************/

void RCC_Configuration(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6, ENABLE);

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
}

/**************************************************************************************/

void GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_USART6);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_USART6);
}

/**************************************************************************************/

void USART6_Configuration(void)
{
	USART_InitTypeDef USART_InitStructure;

	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;

	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	USART_Init(USART6, &USART_InitStructure);

	USART_Cmd(USART6, ENABLE);
}
void console_out(char* str) {
	uint8_t i = 0;
	while (str[i] != '\0') {
		USART_SendData(USART6, (uint8_t) str[i++]);
		/* Loop until the end of transmission */
		while (USART_GetFlagStatus(USART6, USART_FLAG_TC) == RESET);
	}
}

uint16_t readCharUSART(){
	uint16_t ch;
	while (USART_GetFlagStatus(USART6, USART_FLAG_RXNE) == RESET);

	ch = USART_ReceiveData(USART6);
	return ch;
}

void console_init() {

	RCC_Configuration();
	GPIO_Configuration();
	USART6_Configuration();
}

void USART_printf(const char *vectcStr,...)
{
  char buffer[256];
  va_list arglist;

  va_start(arglist,vectcStr);
  vsprintf(buffer,vectcStr,arglist);
  va_end(arglist);

  console_out((char*)buffer);

}
