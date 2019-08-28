/*
 * trans.c
 *
 * Created: 2019-08-17 12:28:04
 * Author : Nafiz
 */ 

#include <avr/io.h>
#define F_CPU 1000000
#include <util/delay.h>

int main(void)
{
	
	DDRD = 0xFF;
	PORTD = 0x00;
	unsigned char s[] = "esAqwxyzggzyxwqAse";
	//unsigned char s[] = "esAqwxyzg";
	int  delay = 100;
    /* Replace with your application code */
    while (1) 
    {
		int i;
		for (i=0; i<18; i++){
			
			PORTD = 0b11000000;
			_delay_ms(delay);
			
			unsigned char c;
			c = s[i];
			PORTD = 0b10100000;		//UP and LEFT
			_delay_ms(delay);
			
			PORTD = 0b01100000;		// BACKWARD AND CW
			_delay_ms(delay);
						
			PORTD = 0b10010000;		// front tilt and claw close
			_delay_ms(delay);
			

		}
    }
}

