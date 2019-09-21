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
#include<stdlib.h>

#define UART_BAUD_RATE 9600
#include "uart/uart.h"


volatile unsigned int timerCount = 0;
volatile int servo1 = 90;//base

volatile int servo2 = 140;

volatile int servo3 = 115;
volatile int servo4 = 90;
volatile int servo5 = 90;
volatile int servo6 = 90; //clamp

volatile int servo1On = 1;
volatile int servo2On = 1;
volatile int servo3On = 1;
volatile int servo4On = 1;
volatile int servo5On = 1;
volatile int servo6On = 1;
volatile int servo7On = 1;
volatile int servo8On = 1;

volatile int res = 0;






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
	//all servos in port A


	DDRA = 0xFF;
    DDRB = 0xFF;
	TCCR1A |= 1 << WGM11;
	TCCR1B |= 1 << WGM12 | 1 << WGM13 | 1 << CS11;
	TIMSK  |= 1 << OCIE1A;
	ICR1 = 19999;

	sei();

	servo1 = 90;
	servo3 = 125;
    servo2 = 140;
    
    
    int roll = 0;
    int yaw = 0;
    int pitch = 0;
    int prevYaw = 0;
    int prevPitch = 0;
    int prevRoll = 0;
    
    //int state = 0;
    
    int min = -1;
    int isRoll = 1;
    PORTB = 0x00;
    int errorLimit = 30;
    int clawOpen =0;
    int clawClose = 0;
    
    int clawCount = 0;
    
	while(1) {

		//Here, values to servo1 and servo2 etc... will be processed from gyro readings

		if(TCNT1 >= 300 && TCNT1 <= 2300) {
			if(TCNT1 >= getPulseWidth(servo1) && bit_is_set(PORTA, PINA0)) PORTA &= ~(1 << PINA0);
			if(TCNT1 >= getPulseWidth(servo2) && bit_is_set(PORTA, PINA1)) PORTA &= ~(1 << PINA1);
			if(TCNT1 >= getPulseWidth(servo3) && bit_is_set(PORTA, PINA2)) PORTA &= ~(1 << PINA2);
            if(TCNT1 >= getPulseWidth(servo4) && bit_is_set(PORTA, PINA3)) PORTA &= ~(1 << PINA3);
            if(TCNT1 >= getPulseWidth(servo5) && bit_is_set(PORTA, PINA4)) PORTA &= ~(1 << PINA4);
            if(TCNT1 >= getPulseWidth(servo6) && bit_is_set(PORTA, PINA5)) PORTA &= ~(1 << PINA5);
			
		}

		if(TCNT1 < 300 || TCNT1 > 2300) {
            
            //nafiz code
            //sample data = -222,-221,-119\n
            unsigned int c;
            c = uart_getc();
            //char letter = (char) c;
            if(c == ' ') ;
            else if(c=='\n')  {
                res = res * min;
                min = 1;
                //state++;
                char temp[5];
                clawOpen = res/10000;
                clawClose = (res/1000)%10;
            
                
                
                //                if(yaw > 10) {
                //                    PORTB = 0xFF;
                //                }
                //                else if(yaw < 10) {
                //                    PORTB = 0x00;
                //                }
                
                //itoa(yaw, temp, 10);
                //uart_putc('\n'); uart_putc('\n');
                //uart_puts("Yaw: ");
                //uart_puts(temp);
                //uart_putc('\n');
                //uart_flush();
                
                //uart_puts("Yaw: "); uart_puts(temp); uart_puts("   ");
                //uart_puts("\n\n");
                res = 0;
                isRoll = 1;
               // state = 1;
                
            }
            else if(c=='-') min = -1;
            else if(c==',') {
                res = res * min;
                min = 1;
                //state++;
                char temp[5];
                itoa(res, temp, 10);
                //uart_putc('\n'); uart_putc('\n');
                
                //uart_putc('\n');
                //uart_flush();
                if(isRoll == 1) {
                    roll = (int) res;
                    
                    //roll = 50;
                    //uart_puts("Roll: "); uart_puts("  ");
                    //itoa(roll, temp, 10);
                    //uart_puts(temp);
                    //uart_puts("Roll: "); uart_puts(temp); uart_puts("   ");
                    isRoll = 0;
                }
                else if(isRoll == 0) {
                    
                    //uart_puts("Pitch: ");
                    pitch = (int) res;
                    //itoa(pitch, temp, 10);
                    //uart_puts(temp); uart_puts("  ");
                    
                    //uart_puts("Pitch: "); uart_puts(temp); uart_puts("   ");
                    isRoll = 2;
                }
                else if(isRoll == 2){
                    yaw = (int) res;
                    isRoll = 1;
                }
                res = 0;
                
                
            }
            else if(c >= '0' && c <= '9') {
                res = res * 10 + (int)(c - '0');
            }
            else {
                //state = 0;
            }
            
            
            //nafiz code end
            
            
            
            
            //yaw handling
            //servo 1 is yaw
//            if(yaw < -90) yaw = -90;
//            else if(yaw > 90) yaw = 90;
            
            //yaw = yaw + 90;
            
           // if( abs(prevYaw - yaw) < 40) {
                //PORTB = 0x00;
                servo1 = yaw + 90;
                
                prevYaw = yaw;
            //}
           // else {
                //PORTB = 0xFF;
            //}
            
            
            
            
            //roll
            //same as yaw, with servo 5
            
//            if(roll < -90) roll = -90;
//            else if(roll > 90) roll = 90;
            
//            roll = roll + 90;
            
            if( abs(prevRoll - roll) < errorLimit) {
                //PORTB = 0x00;
                servo5 = 180 - (roll + 90);
                
                prevRoll = roll;
            }
            
            else {
                //PORTB = 0xFF;
            }
            
          
            
            
            
            //pitch
            //3 motors, servos 2,3,4
            
//            if(pitch < -90) pitch = -90;
//            else if(pitch > 90) pitch = 90;
            
            //pitch = pitch + 90;
            
            if( abs(prevPitch - pitch) < 30)  {
                //PORTB = 0x00;
                //processing pitch
                //servos 2,3,4
                
                //gyro = 0 -> servo2 = 130, servo3 = 115, servo4 = 90
                //gyro = theta ++ -> servo3 = 115 - theta, servo4 = 90 - theta, servo2 = no power
                
                //servo2 = init as 130
                if(pitch > 0) {
                    //PORTB = 0xFF;
                    servo3 = 115 - pitch;
                    servo4 = 90 - pitch;
                    servo2On = 0;
                }
                else if(pitch < 0) {
                   //PORTB = 0x00;

                    //pitch is negative, so we used +'s instead of -'s in the formulas, we couldve used abs(), but eh
                    //servo2 is 130
                    if((140 + pitch) > 70) {
                        servo2 = 140 + pitch;
                    }
                    else {
                        servo2 = 70;
                    }

                    if((115 + pitch) > 30) {
                        servo3 = 115 + pitch;
                    }
                    else {
                        servo3 = 30;
                    }


                    servo4 = 90 + pitch;
                }
                
                prevPitch = pitch;
                
                
                //the next if statements help in debugging received pitch value
                if(prevPitch < 10 && prevPitch > 0) {
                    PORTB = 0b00000001;
                }
                else if(prevPitch > 10 && prevPitch < 20) {
                    PORTB = 0b00000010;
                }
                
                else if(prevPitch > 20 && prevPitch < 50) {
                    PORTB = 0b00000100;
                }

            }
            
            else {
                //PORTB = 0xFF;
            }
            
            
            if (clawOpen == 1) {
                clawCount++;
                
                if (servo6 - 1 > 45 && clawCount > 10) {
                    servo6 -= 1;
                    clawCount = 0;
                }
            }
            else if (clawClose == 1){
                clawCount++;
                if (servo6 + 1 < 100 && clawCount > 10) {
                    clawCount = 0;
                    servo6 += 1;
                }
            }
            
          
		}



	}
}


ISR(TIMER1_COMPA_vect) {
	//interrupt called when TCNT1 value reaches ICR1 value (every 20ms)
    

    PORTA = 0xFF;
//    if(servo1On == 1) {
//        PORTA |= 1 << PINA0;
//    }
//
//    if(servo2On == 1) {
//        PORTA |= 1 << PINA1;
//    }
//
//    if(servo3On == 1) {
//        PORTA |= 1 << PINA2;
//    }
//
//    if(servo4On == 1) {
//        PORTA |= 1 << PINA3;
//    }
//
//    if(servo5On == 1) {
//        PORTA |= 1 << PINA4;
//    }
//
//    if(servo6On == 1) {
//        PORTA |= 1 << PINA5;
//    }
//    if(servo7On == 1) {
//        PORTA |= 1 << PINA6;
//    }
//    if(servo8On == 1) {
//        PORTA |= 1 << PINA7;
//    }
    
	
}
