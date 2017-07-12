/*
	Testing the SPI protocol with 2 MCU's. There's not much to say here,
	but much to discover bellow!

	This is the slave version of the thing.

	MCU = atmega328p      		Clock = Internal 8MHz

*/

#include <avr/io.h>
#include <stdint.h>


static void setup_spi( void ){

	PRR &=  ~( 1 << PRSPI );

	//Outputs: PB0, MISO
	DDRB = 0b00010001;

	//Slave, SPI enable, fck/16
	SPCR = 0b01000001;

}

int SPISlaveReceive ( void ){

	//Wait for reception complete
	while(!(SPSR & (1 << SPIF)));

	//Return data register
	return SPDR;

}

static void setup_clock( void ){

	CLKPR = 0x80;	// For more information on this procedure look at page 37
	CLKPR = 0x00;

}

int main( void ){

	setup_clock();
	setup_spi();

	for(;;){

		PORTB ^= SPISlaveReceive();

	}
}
