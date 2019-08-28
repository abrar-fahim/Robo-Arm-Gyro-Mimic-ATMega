
#define F_CPU 1000000
#include <avr/io.h>
#include <util/delay.h>



int main(void)
{
    int i;
    DDRB = 0xFF;
    while(1)
    {
        PORTB = 0xFF;
        //for(i = 0; i < 500; i++) {
            //_delay_us(2);
        //}
        _delay_ms(1);
        PORTB = 0x00;
        _delay_ms(19);
        
    }
}
    
