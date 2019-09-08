/*
   Bluetooth_Interface with ATmega16 to Control LED via smartphone
   http://www.electronicwings.com
 
 */ 

#include <avr/io.h>
#include "USART_RS232_H_file.h"		/* include USART library */

//#define LED PORTB					/* connected LED on PORT pin */

int main(void)
{
	char Data_in;
    DDRB = 0xFF;							/* make PORT as output port */
	PORTB = 0x00;
	USART_Init(9600);						/* initialize USART with 9600 baud rate */
	//LED = 0;
	
	while(1)
	{
		Data_in = USART_RxChar();						/* receive data from Bluetooth device*/
		if(Data_in =='1')
		{
			PORTB = 0xFF;
			//LED |= (1<<PB0);							/* Turn ON LED */	
		}
		else if(Data_in =='0')
		{
			PORTB = 0x00;
			//LED &= ~(1<<PB0);							/* Turn OFF LED */
		}
	}		
	return 0;
}