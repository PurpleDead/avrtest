/*
	Testing the SPI protocol with 2 MCU's. There's not much to say here,
	but much to discover bellow!

	This is the master version of the thing.

	MCU = atmega328p      		Clock = Internal 8MHz

*/

#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>

void delay_ms(uint32_t time) {
  for (uint32_t i = 0; i < time; i++) {
    _delay_ms(1);
  }
}

static void setup_spi( void ){

	PRR &=  ~( 1 << PRSPI );

	//Outputs: PB0, SCK, MOSI, SS/
	DDRB = 0b00101101;

	//Master, SPI enable, fck/16
	SPCR = 0b01010001;

}

void SPIMasterTransmit( uint8_t data ){

	//Start transmission
	SPDR = data;

	//Wait for transmission complete
	while(!(SPSR & (1 << SPIF)));

}

static void setup_clock( void ){

	CLKPR = 0x80;	// For more information on this procedure look at page 37
	CLKPR = 0x00;

}

int main( void ){

	setup_clock();
	setup_spi();

	for(;;){

		SPIMasterTransmit(0x01);
		PORTB = 0x00;
		delay_ms(1000);
		SPIMasterTransmit(0x00);
		PORTB = 0x01;
		delay_ms(1000);

	}
}
