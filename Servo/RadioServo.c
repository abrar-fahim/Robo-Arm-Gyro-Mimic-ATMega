//
//  manyServos.c
//
//
//  Created by Nafis Abrar  on 8/15/19.
//

#define F_CPU 8000000UL
#include <stdio.h>
#include<math.h>
#include <avr/io.h>
#include<avr/interrupt.h>
#include<util/delay.h>

#define UART_BAUD_RATE 9600
#include "uart.h"
#include "stdutils.h"

volatile unsigned int timerCount = 0;
volatile int servo1 = 90;//base

volatile int servo2 = 90;

volatile int servo3 = 90;
int servo4 = 90;
int servo5 = 90;
int servo6 = 90; //clamp




double roll, pitch, yaw;
double prevYaw, prevPitch, prevRoll;


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

int main(void) {
	uart_init(UART_BAUD_SELECT(UART_BAUD_RATE,F_CPU));
	sei();
	//all servos in port A

	DDRB = 0x00;
	DDRA = 0xFF;
	TCCR1A |= 1 << WGM11;
	TCCR1B |= 1 << WGM12 | 1 << WGM13 | 1 << CS10;
	TIMSK  |= 1 << OCIE1A;
	ICR1 = 19999;

	sei();

	servo1 = 90;
	servo2 = 90;
	servo3 = 90;
    
    
	while(1) {

		//Here, values to servo1 and servo2 etc... will be processed from gyro readings

		if(TCNT1 >= 300 && TCNT1 <= 2300) {
			if(TCNT1 >= getPulseWidth(servo1) && bit_is_set(PORTA, PINA0)) PORTA &= ~(1 << PINA0);
			if(TCNT1 >= getPulseWidth(servo2) && bit_is_set(PORTA, PINA1)) PORTA &= ~(1 << PINA1);
			if(TCNT1 >= getPulseWidth(servo3) && bit_is_set(PORTA, PINA2)) PORTA &= ~(1 << PINA3);
            if(TCNT1 >= getPulseWidth(servo4) && bit_is_set(PORTA, PINA3)) PORTA &= ~(1 << PINA4);
            if(TCNT1 >= getPulseWidth(servo5) && bit_is_set(PORTA, PINA4)) PORTA &= ~(1 << PINA5);
            if(TCNT1 >= getPulseWidth(servo6) && bit_is_set(PORTA, PINA5)) PORTA &= ~(1 << PINA6);
			
		}

		if(TCNT1 < 300 || TCNT1 > 2300) {
            
            //yaw handling
            //servo 1 is yaw
            if(yaw < -90) yaw = -90;
            else if(yaw > 90) yaw = 90;
            
            yaw = yaw + 90;
            
            if( abs(prevYaw - yaw) > 10) continue;
            
            servo1 = yaw;
            
            prevYaw = yaw;
            
            
            //roll
            //same as yaw, with servo 5
            
            if(roll < -90) roll = -90;
            else if(roll > 90) roll = 90;
            
            roll = roll + 90;
            
            if( abs(prevRoll - roll) > 10) continue;
            
            servo5 = roll;
            
            prevRoll = roll;
            
            
            
            //pitch
            //3 motors, servos 2,3,4
            
            if(pitch < -90) pitch = -90;
            else if(pitch > 90) pitch = 90;
            
            pitch = pitch + 90;
            
            if( abs(prevPitch - pitch) > 10) continue;
            
            //processing pitch
            //for movements within +-30 degrees, only servo2 moves
            //if movement > +-30 and < +- 60, only servo 3 moves
            //if movement > +- 60, only servo 4 moves
            
            //say pitch = 80
            if(pitch > -30 + 90 && pitch < 30 + 90) {
                servo2 = pitch;
                //servo2 = 120
            }
            
            else if(pitch > -60 + 90 && pitch < 60 + 90) {
                servo3 = pitch - servo2;
                //servo3 = 30
            }
            
            else {
                servo4 = pitch - servo3 - servo2;
                //servo4 = 50 for pitch = 80
            }
            prevPitch = pitch;
            
            //say pitch = 80
            //so, servo2 = 120, servo3 = 140, servo4 =
		}



	}
}


ISR(TIMER1_COMPA_vect) {
	//interrupt called when TCNT1 value reaches ICR1 value (every 20ms)
    PORTA = 0xFF;
	
}
