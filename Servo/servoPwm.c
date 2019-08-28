#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>

void initPWM() {

    //OCR0 pin PWM start
    //no prescaling, fast pwm, inverted
    TCCR0 |= (1 << WGM00) | (1 << WGM01) | (1 << COM01) | (1 << COM00)| (1 << CS02);
    DDRB  |= (1 << PB3);  //sets 0C0 pin as output
    //OCR0 pin PWM end
    
    
    
    DDRB |= (1 << DDB1)|(1 << DDB2);
    // PB1 and PB2 is now an output

    TCCR1A |= (1 << COM1A1)|(1 << COM1B1);
    // set none-inverting mode

    ICR1 = 0xFFFF;
    // set TOP to 16bit

    OCR1A = 0x3FFF;
    // set PWM for 25% duty cycle @ 16bit

    OCR1B = 0xBFFF;
    // set PWM for 75% duty cycle @ 16bit

    TCCR1A |= (1 << WGM11);
    TCCR1B |= (1 << WGM12)|(1 << WGM13);
    // set Fast PWM mode using ICR1 as TOP
    TCCR1B |= (1 << CS10);
    // START the timer with no prescaler
}


int main(void)
{
    
    //for output,
    initPWM();
    unsigned char duty = 4;   //0-255
    OCR0 = duty;
    //_delay_ms(1000);
    
    while(1);
    
    
    
    //DDRD |= 0xFF;
//    TCCR1A |= 1<<WGM11 | 1<<COM1A1 | 1<<COM1A0;
//    TCCR1B |= 1<<WGM13 | 1<<WGM12 | 1<<CS10;
//    ICR1 = 19999;
//
//    OCR1A = ICR1 - 2000; //18000
//    _delay_ms(100);
//
//    while (1)
//    {
//        OCR1A = ICR1 - 2000;
//        _delay_ms(2000);
//        OCR1A = ICR1 - 500;
//        _delay_ms(2000);
//    }
}
