/*
   Bluetooth_Interface with ATmega16 to Control LED via smartphone
   http://www.electronicwings.com
 
 */ 

#ifndef F_CPU
#define F_CPU 8000000UL
#endif

#include <util/delay.h>
#include <avr/io.h>
#include "USART_RS232_H_file.h"		/* include USART library */



int main(void)
{
	
	USART_Init(9600);						/* initialize USART with 9600 baud rate */
	
	int power = 0;
	
	while(1)
	{
		if(power == 0) 
		{
			USART_TxChar('1');
			power = 1;
		}
		else if(power == 1)
		{
			USART_TxChar('0');
			power = 0;
		}
		
		_delay_ms(500);
	}		
	return 0;
}