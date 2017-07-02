/*
  This is for test porpuses. I actually may not know what I'm doing here.
  So you were advised. Don't take any of this seriously!

  MCU = atmega328p      Clock = Internal 8MHz (?), maybe divided by 8.
*/

// Mostly copied from CNLohr's avrprojector project. Don't know how much is
// really needed for this little test, but this is it.

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include <stdio.h>

/* This function was copied from CNLohr's avrprojector project.

 NOTE: A static function can only be acessed on this file. Cannot be
 acessed by any other external file. Wildly used for creating private functions.

 ADDITIONAL: static variables will preserve their value even if declared again.
 This may seen confuse at first, but is simple. Look up on the internet for
 examples, I won't give you any in here. */
static void setup_clock( void ){

  // CLKPR is the Clock Prescaler Register
	CLKPR = 0x80;	// For more information on this procedure look at page 37
	CLKPR = 0x00;	// This make the clock stay at 8MHz, regardless the clock divisor fuse
                // OSCCAL register may be used for calibrating the internal clock
}

static void setup_int( void ){
	sei();					 	// Enable Global Interruptions
	/*
	TCCR1B |= _BV(CS10);
	TIMSK1 |= _BV(TOIE1);
	*/

		PRR  &=  ~(0x20); // Enable TIMER0 NOTE NOTE: BE CAREFUL WITH IT. READ IT WELL NOTE NOTE
		TCCR0B 	=  0x05;  // clk I/O /(No prescaling)
		TCNT0 	=  0x00; 	// Timer/Counter Register starts in 0
		TIMSK0 	=  0x01; 	// Timer/Counter0 Overflow Interrupt Enable

}

/* This function was also taken from CNLohr's avrprojector project.

At first it didn't make that much sense. Come on, you're using a _delay_ms
function inside of your delay_ms function! I got to the conclusion that
the _delay_ms function doesn't accept very large numbers. For that you have
that uint32_t on the for loop (time).*/
void delay_ms(uint32_t time) {
  for (uint32_t i = 0; i < time; i++) {
    _delay_ms(1);
  }
}

/* ===== Interruption settings =====

	To enable timers the respective PRTIMx bit on the PRR (Power Reduction Register)
must be set to 1.

Nice definitions:
==================================================================================
BOTTOM: The counter reaches the BOTTOM when it becomes 0x00.
==================================================================================
MAX: The counter reaches its MAXimum when it becomes 0xFF (decimal 255).
==================================================================================
TOP: The counter reaches the TOP when it becomes equal to the highest value in the
count sequence. NOTE: The TOP value can be assigned to be the fixed value 0xFF
(MAX) or the value stored in the OCR0A Register. The assignment is
dependent on the mode of operation.
==================================================================================

To do some configurations on the Timer and Interruptions see TCCRxA (Timer/Counter Control Register A).
To select a clock source and prescaler look at TCCRxB (Timer/Counter Control Register B).

TCNTx is the register that is counted every clock pulse.
*/

/* Nice reference: http://www.avrfreaks.net/forum/tut-newbies-guide-avr-interrupts?page=all
This part is interesting. the ISR (Interrupt Service Routine) "function" is called everytime
an interrupt is fired. To name that you go to the bector table on the datasheet and
put the desired interrupt vector in the ISR argument. For example, we have the Interruptions
bellow being called when the TIMER0 counter is overflowed.*/
ISR(TIMER0_OVF_vect){
	static uint32_t i = 0;
	i ++;
	if(i > 2) {PORTC ^= 0x01; i =0;}
}

// Apparently, main is the function that is called as soon the chip is turned on.
int main( void ){

  setup_clock();

	setup_int();

  /* DDRB = The PORTB Data Direction Register.
	Apparently, 1 in DDR means that it's an output pin.
	"_BV(x) is a synonym for (1<<x)", said CNLohr. */

  DDRB = _BV(0) | _BV(1) | _BV(2) | _BV(3);
	PORTB = 0x01;

	DDRC = _BV(0);
	PORTC = 0x01;

	/*	===== ADC Settings =====

		To start a conversion PRADC bit on PRR register must be set to 0 and writing 1
		on ADCSC bit (ADC Start Conversion bit). NOTE: ADCSC is cleared after conversion
		is completed.
		To read from a input channel the MUX bits in ADMUX register must be changed.
		The ADC is enabled in the ADCSRA register, by setting the ADEN bit.
		The result from the read is 10 bit wide and is present in ADCH and ADCL registers.

		*/

  while(1){
		/*This thing bellow is called a ternary operator.
		It works like that: (CONDITION) ? (COND. IS TRUE) : (COND. IS FALSE)*/

		//PORTB = PORTB == 0x08 ? 0X01 : PORTB << 1;
		//PORTB = PORTB == 0x0C ? 0X09 : (PORTB == 0x09 ? 0x03 : PORTB << 1);

		PORTB = PORTB == 0x08 ? PORTB = 0x09 : (PORTB << 1 | PORTB);
		delay_ms(500);
		PORTB = PORTB == 0x9 ? PORTB = 0x01 : (PORTB << 1 & PORTB);
		delay_ms(500);

		if(TIFR0 & (1<<TOV0)) {
			while(1){
				static uint32_t i = 0;
				delay_ms(10);
				i++;
				PORTB ^= PORTB;
				if(i > 10) {PORTB = 0x01; i = 0;}
			}
		}
  }

  return 0;
}
