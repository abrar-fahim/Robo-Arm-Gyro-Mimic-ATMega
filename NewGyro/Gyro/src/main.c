#define F_CPU 8000000

#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <math.h>  //include libm

#include "mpu6050/mpu6050.h"
#include <avr/eeprom.h>

#define UART_BAUD_RATE 9600
#include "uart/uart.h"
#define PI 3.14159


volatile uint32_t tot_overflow;

void timer1_init() {
	//TCNT1, TCCR1A, TCCR1B, TIMSK, TIFR, ICR1, TIMER1_OVF_vect
	
	TCCR1B = 1 << CS11;
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
	
	
	
	DDRA = 0x00;
	DDRB = 0xFF;
	
	
	PORTB = 0x00;
	//    uint32_t currentTime = 0;
	//    uint32_t previousTime = 0;
	uint32_t elapsedTime  = 0;
	
	double rollError = 0;
	double pitchError = 0;
	double yawError = 0;
	double roll1=0;
	double pitch1=0;
	double yaw1=0;
	
	double prevRoll=0, prevPitch=0, prevYaw=0;
	
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

	#if MPU6050_GETATTITUDE == 1 || MPU6050_GETATTITUDE == 2
	//long *ptr = 0;
	double qw = 1.0f;
	double qx = 0.0f;
	double qy = 0.0f;
	double qz = 0.0f;
	double roll = 0.0;
	double pitch = 0.0;
	double yaw = 0.0;
	#endif

	//init uart
	uart_init(UART_BAUD_SELECT(UART_BAUD_RATE,F_CPU));

	//init interrupt
	sei();

	//init mpu6050
	mpu6050_init();
	_delay_ms(50);

	//init mpu6050 dmp processor
	#if MPU6050_GETATTITUDE == 2
	mpu6050_dmpInitialize();
	mpu6050_dmpEnable();
	_delay_ms(10);
	#endif
	
	timer1_init();

	int isRecording = 0;
	int address = 0;
	int prevRecordTime = 0;
	int isPlayback = 0;
	int finalAddress = 0;
	int reachedTargetPos = 0;
	
	int targetRoll=0;
	int targetPitch=0;
	int targetYaw=0;
	
	int currRoll =0;
	int currPitch =0;
	int currYaw =0;
	
	
	
	for(;;) {
		//uart_puts("hello");
		#if MPU6050_GETATTITUDE == 0
		mpu6050_getRawData(&ax, &ay, &az, &gx, &gy, &gz);
		mpu6050_getConvData(&axg, &ayg, &azg, &gxds, &gyds, &gzds);
		
		//        previousTime = currentTime;
		//        currentTime = getElapsedTime();
		//        elapsedTime = currentTime - previousTime;
		//
		//getVelocity(&velx, &vely, &velz, elapsedTime);
		#endif

		#if MPU6050_GETATTITUDE == 1
		mpu6050_getQuaternion(&qw, &qx, &qy, &qz);
		mpu6050_getRollPitchYaw(&roll, &pitch, &yaw);
		_delay_ms(10);
		#endif

		#if MPU6050_GETATTITUDE == 2
		if(mpu6050_getQuaternionWait(&qw, &qx, &qy, &qz)) {
			mpu6050_getRollPitchYaw(qw, qx, qy, qz, &roll, &pitch, &yaw);
		}
		_delay_ms(10);
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
		
		
		roll1 = roll * 180 / PI;
		pitch1 = pitch * 180 / PI;
		yaw1 = yaw * 180 / PI;
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
			yaw1 -= yawError;
			
			
			//need to check for appropriate threshold values by trial and error later, using 5 for now
			int yawDiff = abs(prevYaw - yaw1);
			int pitchDiff = abs(prevPitch - pitch1);
			int rollDiff = abs(prevRoll - roll1);
			
			int allowedDiff=80;
			if (!( yawDiff>allowedDiff || rollDiff>allowedDiff || pitchDiff>allowedDiff)){
				prevRoll = roll1;
				prevYaw = yaw1;
				prevPitch = pitch1;
			}
			else continue;
			
			
			
			if(yaw1 > 30 && yawDiff < allowedDiff) {
				// PORTB = 0b11101110;
				//    _delay_ms(delay);
				
			}
			else if(yaw1 < -30 && yawDiff < allowedDiff) {
				//  PORTB = 0b11011101;
				//  _delay_ms(delay);
			}
			else {
				// PORTB = 0b11001100;
				//  _delay_ms(delay);
			}
			
			
			
			if(roll1 > 30 && rollDiff < allowedDiff) {
				//   PORTB = 0b10001000;
				//_delay_ms(delay);
				
			}
			else if(roll1 < -30 && rollDiff < allowedDiff) {
				//  PORTB = 0b01000100;
				//_delay_ms(delay);
			}
			else {
				// PORTB = 0b00000000;
			}
			
			
			
			if(pitch1 > 30 && pitchDiff < allowedDiff) {
				//    PORTB = PORTB & 0b11001100;
				//  PORTB = PORTB | 0b00100010;
				//   _delay_ms(delay);
				
			}
			else if(pitch1 < -30 && pitchDiff < allowedDiff) {
				//      PORTB = PORTB & 0b11001100;
				//    PORTB = PORTB | 0b00010001;
				//  _delay_ms(delay);
			}
			else {
				//      PORTB = PORTB & 0b11001100;
				//PORTB = PORTB | 0b00000000;
				// _delay_ms(delay);
			}
			
			//PORTB = 0b00110011;
			//_delay_ms(delay);


			int clawOpen=0;
			int clawClose=0;
			int trainingStart =0;
			int trainingEnd =0;
			int repeat =0;
			
			PORTB = 0x00;
			PORTB |= PINA;
			

			if ( (PINA & 0b00000001) == 0b00000001){		//CLAW OPEN
				clawOpen=1;
			}
			
			if (PINA & 0b00000010){		//CLAW CLOSE
				clawClose=1;
			}
			if (PINA & 0b00000100){		//Training Start
				if(isRecording == 0) prevRecordTime = getElapsedTime();
				trainingStart=1;
				isRecording = 1;
			}
			if (PINA & 0b00001000) {		//training end
				if (isRecording==1){
					trainingEnd = 1;
					isRecording = 0;
					finalAddress = address;
					
					eeprom_update_word((uint16_t *) 1000, (uint16_t)finalAddress);
					address = 0;
				}
			}
			if (PINA & 0b00010000){		//repeat
				if(isPlayback == 0) prevRecordTime = getElapsedTime();
				isPlayback = 1;
				repeat=1;
				finalAddress = (int16_t) eeprom_read_word ((const uint16_t*) 1000);
				targetRoll = (int8_t) eeprom_read_byte ((const uint8_t*) 0);
				targetPitch = (int8_t) eeprom_read_byte ((const uint8_t*) 1);
				targetYaw = (int8_t) eeprom_read_byte ((const uint8_t*) 2);
				
				currRoll = roll1;
				currPitch = pitch1;
				currYaw = yaw1;
				
				
				
			}
			if(isRecording == 1) {
				
				
				if((getElapsedTime() / 100) - prevRecordTime > 2) {
					
					uint8_t temp = (uint8_t) roll1;
					eeprom_update_byte((uint8_t *) address, temp);
					address++;
					
					temp = (uint8_t) pitch1;
					eeprom_update_byte((uint8_t *) address, temp);
					address++;
					
					temp = (uint8_t) yaw1;
					eeprom_update_byte((uint8_t *) address, temp);
					address++;
					
					temp = (uint8_t) (clawOpen * 10 + clawClose);
					eeprom_update_byte((uint8_t *) address, temp);
					address++;
					prevRecordTime = getElapsedTime();
					
				}
				if(address > 900) {
					isRecording = 0;
					trainingEnd = 1;
					finalAddress = address;
					eeprom_update_word((uint16_t *) 1000, (uint16_t)finalAddress);
					address = 0;
					
				}
				
				
			}
			
			if(isPlayback == 1) {
				char itmp[10];
				int8_t temp;
				
				if (reachedTargetPos == 0){
					if (targetRoll>currRoll)currRoll++;
					else if (targetRoll<currRoll) currRoll--;
					
					if (targetPitch>currPitch)currPitch++;
					else if (targetPitch<currPitch) currPitch--;
					
					if (targetYaw>currYaw)currYaw++;
					else if (targetYaw<currYaw) currYaw--;
					
					itoa(currRoll, itmp, 10); uart_puts(itmp); uart_puts(",");
					itoa(currPitch, itmp, 10); uart_puts(itmp); uart_puts(",");
					itoa(currYaw, itmp, 10); uart_puts(itmp); uart_puts(",");
					uart_puts("0");
					uart_puts("0");
					uart_puts("0");
					uart_puts("0");
					uart_puts("0");
					
					uart_puts("\n");
					if (abs(targetPitch-currPitch) < 2  && abs(targetRoll-currRoll)<2  && abs(targetYaw-currYaw) <2)reachedTargetPos=1;
					
					
					
				}
				
				else if (reachedTargetPos==1){
				
					
					if((getElapsedTime() / 100) - prevRecordTime > 2) {
					
				
						//roll
						temp = (int8_t) eeprom_read_byte ((const uint8_t*) address);
						address++;
				
						currRoll = temp;
				
						itoa(temp, itmp, 10); uart_puts(itmp); uart_puts(",");
				
						//pitch
						temp = (int8_t) eeprom_read_byte ((const uint8_t*) address);
						address++;
						
						currPitch = temp;
				
				
				
						itoa(temp, itmp, 10); uart_puts(itmp); uart_puts(",");
				
						//yaw
						temp = (int8_t) eeprom_read_byte ((const uint8_t*) address);
						address++;
						
						currYaw = temp;
				
				
						itoa(temp, itmp, 10); uart_puts(itmp); uart_puts(",");
				
						//claw open and close
						temp = (int8_t) eeprom_read_byte ((const uint8_t*) address);
						address++;
				
						int clawClose = temp / 10;
						int clawOpen=temp  % 10;
				
						itoa(clawOpen, itmp, 10); uart_puts(itmp);
				
						itoa(clawClose, itmp, 10); uart_puts(itmp);
				
						uart_puts("0");
						uart_puts("0");
						uart_puts("0");
						itoa(finalAddress, itmp, 10); uart_puts(itmp);
				
				
						uart_puts("\n");
						if (address>=finalAddress){
							reachedTargetPos=0;
						}
						address=address % finalAddress;
						if (address==0){
							reachedTargetPos=0;
						}
						
					}
				
				}
				
			}
			else if(isPlayback == 0) {
				dtostrf(roll1, 3, 0, itmp); uart_puts(itmp); uart_puts(",");
				dtostrf(pitch1, 3, 0, itmp);uart_puts(itmp); uart_puts(",");
				dtostrf(yaw1, 3, 0, itmp);uart_puts(itmp);uart_puts(",");
				dtostrf(clawOpen, 3, 0, itmp);uart_puts(itmp);
				dtostrf(clawClose, 3, 0, itmp);uart_puts(itmp);
				dtostrf(trainingStart, 3, 0, itmp);uart_puts(itmp);
				dtostrf(trainingEnd, 3, 0, itmp);uart_puts(itmp);
				dtostrf(repeat, 3, 0, itmp);uart_puts(itmp);
				uart_puts("\n");
				
				
			}
			
			
			
			
			
			
			
			
			if (!(yawDiff>allowedDiff || rollDiff>allowedDiff || pitchDiff>allowedDiff)){
				prevRoll = roll1;
				prevYaw = yaw1;
				prevPitch = pitch1;
			}

		}
		else {
			
			//    PORTB = 0b10011001;
			
			// uart_puts("calibrating...");

		}
		
		
		//
		//
		//        dtostrf(qa, 3, 1, itmp); uart_puts(itmp); uart_putc(' ');
		//        dtostrf(qb, 3, 1, itmp); uart_puts(itmp); uart_putc(' ');
		//        dtostrf(qc, 3, 1, itmp); uart_puts(itmp); uart_putc(' ');
		//        dtostrf(qd, 3, 1, itmp); uart_puts(itmp); uart_putc(' ');
		
		
		
		
		
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
