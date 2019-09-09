#define F_CPU 8000000UL

#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <math.h>  //include libm
#include "USART_RS232_H_file.h"

#include "mpu6050.h"

#define UART_BAUD_RATE 4800
#include "uart.h"
#define PI 3.14159


volatile uint32_t tot_overflow;

void timer1_init() {
    //TCNT1, TCCR1A, TCCR1B, TIMSK, TIFR, ICR1, TIMER1_OVF_vect
    
    TCCR1B = 1 << CS10;
    TIMSK = 1 << TOIE1;
    TCNT1 = 0;
    tot_overflow = 0;
}

ISR(TIMER1_OVF_vect)
{
    // keep a track of number of overflows
    tot_overflow++;
}

uint32_t getElapsedTime() {
    //return tot_overflow * (255 - TCNT0);
    return (tot_overflow * 65536 + (uint32_t) TCNT1) / 1000  ; //time returned in micro seconds / 1000 = millis
    //return TCNT0;
}

int main(void) {
    
    
    
    DDRA = 0xFF;
    DDRB = 0xFF;
    
    PORTA = 0xAA;
    PORTB = 0xFF;
//    uint32_t currentTime = 0;
//    uint32_t previousTime = 0;
    uint32_t elapsedTime  = 0;
    
    
    
    double prevRoll, prevPitch, prevYaw;
    
    int calibrationState = 0;       //0 means not calibrated
    int delay = 100;

	#if MPU6050_GETATTITUDE == 0
    int16_t velx = 0;
    int16_t vely = 0;
    int16_t velz = 0;
    int16_t ax = 0;
    int16_t ay = 0;
    int16_t az = 0;
    int16_t gx = 0;
    int16_t gy = 0;
    int16_t gz = 0;
    double axg = 0;
    double ayg = 0;
    double azg = 0;
    double gxds = 0;
    double gyds = 0;
    double gzds = 0;
	#endif

	int timeCheck = 0;

    //init uart
	uart_init(UART_BAUD_SELECT(UART_BAUD_RATE,F_CPU));

	//init interrupt
	sei();

	//init mpu6050
	mpu6050_init();
	_delay_ms(50);
    
    timer1_init();
	int still = 0;
	
	
	USART_Init(9600);						/* initialize USART with 9600 baud rate */
	

	while(1) {
        //uart_puts("hello");
		#if MPU6050_GETATTITUDE == 0
		
		mpu6050_getRawData(&ax, &ay, &az, &gx, &gy, &gz);
		mpu6050_getConvData(&axg, &ayg, &azg, &gxds, &gyds, &gzds);
        
		if(gxds < 10 && gyds < 10 && gzds < 10)
		{
			if(still == 1)
			{
				timeCheck++;
				if(timeCheck >= 5)
				{
					USART_TxChar('0');
					//PORTB = 0x00;
				}
			}
			else 
			{
				still = 1;
				timeCheck = 0;
				USART_TxChar('1');
				//PORTB = 0xFF;
			}
		}
		else
		{
			still = 0;
			USART_TxChar('1');
			//PORTB = 0xFF;	
		}
		
		
		_delay_ms(500);
		#endif

		

		

		#if MPU6050_GETATTITUDE == 0
        
        //RAW
        char itmp[10];
        
        
        
        
            //ltoa(getElapsedTime(), itmp, 10); uart_putc(' '); uart_puts(itmp); uart_putc(' ');
        
//        ltoa(velx, itmp, 10); uart_putc(' '); uart_puts(itmp); uart_putc(' ');
//        ltoa(vely, itmp, 10); uart_putc(' '); uart_puts(itmp); uart_putc(' ');
//        ltoa(velz, itmp, 10); uart_putc(' '); uart_puts(itmp); uart_putc(' ');
//        ltoa(gy, itmp, 10); uart_putc(' '); uart_puts(itmp); uart_putc(' ');
//        ltoa(gz, itmp, 10); uart_putc(' '); uart_puts(itmp); uart_putc(' ');
        uart_puts("\r\n");
        ltoa(ay, itmp, 10); uart_putc(' '); uart_puts(itmp); uart_putc(' ');
        ltoa(az, itmp, 10); uart_putc(' '); uart_puts(itmp); uart_putc(' ');
        ltoa(gx, itmp, 10); uart_putc(' '); uart_puts(itmp); uart_putc(' ');
        ltoa(gy, itmp, 10); uart_putc(' '); uart_puts(itmp); uart_putc(' ');
        ltoa(gz, itmp, 10); uart_putc(' '); uart_puts(itmp); uart_putc(' ');
        uart_puts("\r\n");



        dtostrf(axg, 3, 2, itmp); uart_puts(itmp); uart_putc(' ');
        dtostrf(ayg, 3, 2, itmp); uart_puts(itmp); uart_putc(' ');
        dtostrf(azg, 3, 2, itmp); uart_puts(itmp); uart_putc(' ');
        dtostrf(gxds, 3, 0, itmp); uart_puts(itmp); uart_putc(' ');
        dtostrf(gyds, 3, 0, itmp); uart_puts(itmp); uart_putc(' ');
        dtostrf(gzds, 3, 0, itmp); uart_puts(itmp); uart_putc(' ');
		uart_puts("\r\n");

		uart_puts("\r\n");

		//_delay_ms(10);
		#endif

		#if MPU6050_GETATTITUDE == 1 || MPU6050_GETATTITUDE == 2

		//quaternion
        
        char itmp[10];
        
        
        double roll1 = roll * 180 / PI;
        double pitch1 = pitch * 180 / PI;
        double yaw1 = yaw * 180 / PI;
//        double qa = qw * 100;
//        double qb = qx * 100;
//        double qc = qy * 100;
//        double qd = qz * 100;
        
        
        //error correction
        elapsedTime = getElapsedTime() / 1000;  //in millis  / 1000 = in seconds
        //dtostrf(elapsedTime, 3, 1, itmp); uart_puts(itmp); uart_puts("   \n");
        
        if(calibrationState == 0 && elapsedTime > 15) {
            rollError = roll1;
            pitchError = pitch1;
            yawError = yaw1;
            calibrationState = 1;
        }
        else if(calibrationState == 1) {
            roll1 -= rollError;
            pitch1 -= pitchError;
            yaw1 -= yawError-30;
            
            
            //need to check for appropriate threshold values by trial and error later, using 5 for now
            int yawDiff = abs(prevYaw - yaw1);
            int pitchDiff = abs(prevPitch - pitch1);
            int rollDiff = abs(prevRoll - roll1);
            
            if(yaw1 > 30 && yawDiff < 5) {
				
                PORTB = 0b11101110;
                _delay_ms(delay);
                
            }
            else if(yaw1 < -30 && yawDiff < 5) {
                PORTB = 0b11011101;
                _delay_ms(delay);
            }
            else {
                PORTB = 0b11001100;
                _delay_ms(delay);
            }
            
            
            
            if(roll1 > 30 && rollDiff < 5) {
                PORTB = 0b10001000;
               //_delay_ms(delay);
                
            }
            else if(roll1 < -30 && rollDiff < 5) {
                PORTB = 0b01000100;
                //_delay_ms(delay);
            }
            else {
                PORTB = 0b00000000;
            }
            
            
            
            if(pitch1 > 30 && pitchDiff < 5) {
                PORTB = PORTB & 0b11001100;
                PORTB = PORTB | 0b00100010;
                _delay_ms(delay);
                
            }
            else if(pitch1 < -30 && pitchDiff < 5) {
                PORTB = PORTB & 0b11001100;
                PORTB = PORTB | 0b00010001;
                _delay_ms(delay);
            }
            else {
                PORTB = PORTB & 0b11001100;
                //PORTB = PORTB | 0b00000000;
                _delay_ms(delay);
            }
            
            PORTB = 0b00110011;
            _delay_ms(delay);

            
            
            dtostrf(roll1, 3, 0, itmp); uart_puts(itmp); uart_puts("   ");
            dtostrf(pitch1, 3, 0, itmp); uart_puts(itmp); uart_puts("   ");
            dtostrf(yaw1, 3, 0, itmp); uart_puts(itmp); uart_puts("   ");
			
            
            prevRoll = roll1;
            prevYaw = yaw1;
            prevPitch = pitch1;

        }
        else {
            
            PORTB = 0b10011001;
            
            uart_puts("calibrating...");

        }
        
        
//
//
//        dtostrf(qa, 3, 1, itmp); uart_puts(itmp); uart_putc(' ');
//        dtostrf(qb, 3, 1, itmp); uart_puts(itmp); uart_putc(' ');
//        dtostrf(qc, 3, 1, itmp); uart_puts(itmp); uart_putc(' ');
//        dtostrf(qd, 3, 1, itmp); uart_puts(itmp); uart_putc(' ');
        
        uart_puts("\r\n");
        
        uart_puts("\r\n");
        
        
        
        
        
//        ptr = (long *)(&qw);
//        uart_putc(*ptr);
//        uart_putc(*ptr>>8);
//        uart_putc(*ptr>>16);
//        uart_putc(*ptr>>24);
//        ptr = (long *)(&qx);
//        uart_putc(*ptr);
//        uart_putc(*ptr>>8);
//        uart_putc(*ptr>>16);
//        uart_putc(*ptr>>24);
//        ptr = (long *)(&qy);
//        uart_putc(*ptr);
//        uart_putc(*ptr>>8);
//        uart_putc(*ptr>>16);
//        uart_putc(*ptr>>24);
//        ptr = (long *)(&qz);
//        uart_putc(*ptr);
//        uart_putc(*ptr>>8);
//        uart_putc(*ptr>>16);
//        uart_putc(*ptr>>24);
//
//        //roll pitch yaw
//        ptr = (long *)(&roll);
//        uart_putc(*ptr);
//        uart_putc(*ptr>>8);
//        uart_putc(*ptr>>16);
//        uart_putc(*ptr>>24);
//        ptr = (long *)(&pitch);
//        uart_putc(*ptr);
//        uart_putc(*ptr>>8);
//        uart_putc(*ptr>>16);
//        uart_putc(*ptr>>24);
//        ptr = (long *)(&yaw);
//        uart_putc(*ptr);
//        uart_putc(*ptr>>8);
//        uart_putc(*ptr>>16);
//        uart_putc(*ptr>>24);

		//uart_putc('\n');
		#endif

	}

}
