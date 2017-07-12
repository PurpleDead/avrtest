/*
  This is for test porpuses. I actually may not know what I'm doing here.
  So you were advised. Don't take any of this seriously!

  MCU = atmega328p      Clock = Internal 8MHz (?), maybe divided by 8.
*/

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include <stdio.h>

uint16_t width = 10000;

static void setup_clock( void ){

	CLKPR = 0x80;	// For more information on this procedure look at page 37
	CLKPR = 0x00; // OSCCAL register may be used for calibrating the internal clock

}

static void setup_int( void ){

	PRR &= ~(1 << PRTIM0); //Enable Timer0
	TCCR0A = 0xC3;
	TCCR0B = 0x0A;
	TIMSK0 = 0x07;
	OCR0A = ICR1 - width;
	ICR1 = 20000;
	sei();

}

void delay_ms(uint32_t time) {
  for (uint32_t i = 0; i < time; i++) {
    _delay_ms(1);
  }
}

int main( void ){

	DDRD |= 0xFF;
	TCCR1A = 0xC3;
	TCCR1B = 0x1A;
	ICR1 = 19999;

	OCR1A = ICR1 - 2000; //18000

	while (1)
	{
	OCR1A = ICR1 - 800;
	delay_ms(100);
	OCR1A = ICR1 - 2200;
	delay_ms(100);
	}

  return 0;
}
