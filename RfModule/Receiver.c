#define F_CPU 1000000
#include<stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <math.h> //include libm
#define UART_BAUD_RATE 4800
#include "uart/uart.h"
unsigned char byte = 0;
int main(void) {
    //init uart
    uart_init(UART_BAUD_SELECT(UART_BAUD_RATE,F_CPU));
    //init interrupt
    sei();
    DDRB = 0x00;
    DDRA = 0xFF;
    
    unsigned char s[] = "esAqwxyzggzyxwqAse";
    //unsigned char s[] = "esAqwxyzg";
    int delay = 100;
    
    while(1)
    {
        PORTA = PINB;
        while (!((PINB & 0b00001100) == 0b00001100) )PORTA = PINB; // got start|yaw
        unsigned char y;
        y = (PINB << 4);
        y = (y & 0b00110000);
        if (y &0b00100000)uart_puts("Left yaw\n");
        else if (y &0b00010000)uart_puts("Right yaw\n");
        else {uart_puts("YAWN\n");}
        while (((PINB & 0b00001100) == 0b00001100) ) PORTA = PINB; // wait until next data
        
        PORTA = PINB;
        unsigned char c;
        
        c = (PINB << 4);
        c = c & 0b11110000;
        if(c&0b10000000) uart_puts("Roll++\n");
        else if(c&0b01000000) uart_puts("Roll--\n");
        else uart_puts("RollNo\n");
        if(c&0b00100000) uart_puts("Pitch++\n");
        else if(c&0b00010000) uart_puts("Pitch--\n");
        else uart_puts("Unpitch\n");
        
        while ( ( ((PINB << 4) & 0b11110000) == c ) )PORTA = PINB; //wait for next val
        //uart_puts(" q1inv ");
        unsigned char d;
        d = (PINB << 4);
        d = d & 0b11110000;
        if(d&0b10000000) uart_puts("Claw Open\n");
        else if(d&0b01000000) uart_puts("Claw Close\n");
        else uart_puts("CLAWN\n");
        
    }
}
