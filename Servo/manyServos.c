//
//  manyServos.c
//
//
//  Created by Nafis Abrar  on 8/15/19.
//

#define F_CPU 1000000UL
#include <stdio.h>
#include<math.h>
#include <avr/io.h>
#include<avr/interrupt.h>
#include<util/delay.h>

volatile unsigned int timerCount = 0;
volatile int state = 0;
volatile int servo1 = 0;
volatile int servo2 = 180;


int getPulseWidth(int angle) {
    //for servo, full clockwise is 0 degrees, full anticlockwise is 180 degrees
    //map values from 0 - 180 to 400-2200
    
    
    int result = angle * 10;
    result = result + 400;
//    double result = angle / 180.0; //= 0-1
//    result = result * 1800.0; //=0-1800
//    result = result + 400.0; //400-2200
    
//    double x = round(result);
//    int y = (int) x;
    
    //extreme values check
    if(result < 400) return 400;
    if(result > 2200) return 2200;
    
    //return y;
    return result;
}

void setAngle(int pin, double angle) {
    
    
    
}

int main(void) {
    //all servos in port b
    DDRB = 0xFF;
    TCCR1A |= 1 << WGM11;
    TCCR1B |= 1 << WGM12 | 1 << WGM13 | 1 << CS10;
    TIMSK  |= 1 << OCIE1A;
    ICR1 = 19999;
    
    sei();

    servo1 = 0;
    servo2 = 120;
    //state = 0; //0 for counting up, 1 for counting down
    while(1) {
        //here, values to servo1 and servo2 etc... will be processed from gyro readings
    
        if(TCNT1 >= 300 && TCNT1 <= 2300) {
            if(TCNT1 >= getPulseWidth(servo1) && bit_is_set(PORTB, PINB0)) PORTB &= ~(1 << PINB0);
            if(TCNT1 >= getPulseWidth(servo2) && bit_is_set(PORTB, PINB1)) PORTB &= ~(1 << PINB1);
            
        }
    
    if(TCNT1 < 300 || TCNT1 > 2300) {
        
    }
    
    
    
}
}

ISR(TIMER1_COMPA_vect) {
    //interrupt called when TCNT1 value reaches ICR1 value (every 20ms)
    PORTB = 0xFF;
    timerCount++;
    if(timerCount >= 10) {  //every 200ms
        if(servo1 < 180) {
            servo1 = servo1 + 10;
        }
        if(servo2 > 30) {
            servo2 = servo2 - 10;;
        }

        timerCount = 0;
    }
}
