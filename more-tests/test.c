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

uint8_t pulse = 0;

static void setup_clock( void ){

	CLKPR = 0x80;	// For more information on this procedure look at page 37
	CLKPR = 0x00; // OSCCAL register may be used for calibrating the internal clock

}

static void setup_int( void ){

	PRR &= ~(1 << PRTIM0);
	TCCR0A = 0x00;
	TCCR0B = 0x01;
	TIMSK0 = 0x01;
	TCNT0  = 0x00;
	sei();

}

void delay_ms(uint32_t time) {
  for (uint32_t i = 0; i < time; i++) {
    _delay_ms(1);
  }
}

ISR(TIMER0_OVF_vect){
	static uint8_t i = 0;
	if( i >= pulse ) PORTB = 0x00;
	else PORTB = 0x01;
	i++;
}delay_ms(1);


int main( void ){

  setup_clock();
	setup_int();

	DDRB = _BV(1);
	PORTB = 0x01;

  while(1){
		uint8_t i = 0;
		for(i; i < 255; i++){
			pulse = i;
			delay_ms(1);
		}

		for(i; i > 0; i--){
			pulse = i;
			delay_ms(1);
		}

  }

  return 0;
}
