// This project was based on the document "Demystifying the TLC5940", written by Matthew T. Pandina.
// It's not a copy, it's, kind of, my own implementation. Of course there are some things that are
// almost equal to what Matthew wrote, but, hey, the IC don't change, you need the same operations
// as anyone need to make it work properly. But, reffering to other things that may look similar or equal,
// the ideas were just to praical to not use.

// But, as I said, I tried to do as much as I could on my own. For example, for shifting the data in to
// the IC I didn't use the SPI of the MCU, but it prefered to do it "manually", for certain facilities.
// There are many other things I did to be as "original" as I could. But, again, the IC is always the same.

// Anyway, I hope it will be useful for someone other than me.

// Author: PurpleDead       MCU = atmega328p        Clock = Internal RC 8MHz oscillator
// Low fuses: 0x22          High fuses: 0xd9        Extended fuses: 0xff            

#include <avr/io.h>
#include <stdint.h>
#include <avr/interrupt.h>


// Number of IC's connected in series

#define TLC5940_N 2

// PIN, DDR and PORT definitions according to TLC5940 inputs

#define GSCLK_DDR DDRB
#define GSCLK_PORT PORTB
#define GSCLK_PIN 0

#define SIN_DDR DDRD
#define SIN_PORT PORTD
#define SIN_PIN 1

#define SCLK_DDR DDRD
#define SCLK_PORT PORTD
#define SCLK_PIN 2

#define BLANK_DDR DDRD
#define BLANK_PORT PORTD
#define BLANK_PIN 3

#define DCPRG_DDR DDRD
#define DCPRG_PORT PORTD
#define DCPRG_PIN 4

#define VPRG_DDR DDRD
#define VPRG_PORT PORTD
#define VPRG_PIN 5

#define XLAT_DDR DDRD
#define XLAT_PORT PORTD
#define XLAT_PIN 6


// Utility Macros
#define output(DDR,  PIN) (DDR  |=  _BV(PIN))
#define input(DDR,  PIN)  (DDR  &= ~_BV(PIN))
#define high(PORT, PIN)   (PORT |=  _BV(PIN))
#define low(PORT, PIN)    (PORT &= ~_BV(PIN))
#define state(PORT, PIN)  (PORT &   _BV(PIN))
#define pulse(PORT, PIN)  do{high(PORT, PIN); low(PORT, PIN);} while(0)
#define gsmodled(led, channel, data) (GSData[led * 3 + channel] = data)
#define gsmod(index, data) ( GSData[index] = data)



// Global Arrays
volatile uint8_t DCData[TLC5940_N * 16];

volatile uint16_t GSData[TLC5940_N * 16];





// Setups the clock settings for me MCU
static void setup_clock( void ){

	CLKPR = 0x80;	// For more information on this procedure look at page 37
	CLKPR = 0x00;

}





void SerialInput ( uint8_t size, uint16_t data ) {

    for(uint8_t i = 0; i < size ; i++){
        (data >> (size - i - 1)) & 1 ? high(SIN_PORT, SIN_PIN) : low(SIN_PORT, SIN_PIN);
	    pulse(SCLK_PORT, SCLK_PIN);
    }  

}





// Shitfs in Dot Correction (DC) Data. Only needs to be set once.
void ClockDCData ( void ) {

	high(DCPRG_PORT, DCPRG_PIN);
	high(VPRG_PORT,  VPRG_PIN);

	for(uint8_t counter = 0; counter < (TLC5940_N * 16); counter++){
		SerialInput( 6, DCData[counter]);
	}

	pulse(XLAT_PORT, XLAT_PIN);

}





// Setup everything needed for the TLC5940 to work
void TLC5940_init ( void ){

    // Output settings

	output(GSCLK_DDR, GSCLK_PIN);
	output(SIN_DDR, SIN_PIN);
	output(SCLK_DDR, SCLK_PIN);
	output(BLANK_DDR, BLANK_PIN);
	output(DCPRG_DDR, DCPRG_PIN);
	output(VPRG_DDR, VPRG_PIN);
	output(XLAT_DDR, XLAT_PIN);

    //Inicial logic for output pins

	low(GSCLK_PORT, GSCLK_PIN);
	low(SCLK_PORT, SCLK_PIN);
	low(SIN_PORT, SIN_PIN);
	low(XLAT_PORT, XLAT_PIN);
	low(DCPRG_PORT, DCPRG_PIN);
	high(VPRG_PORT, VPRG_PIN);
	high(BLANK_PORT, BLANK_PIN);

    //Timer 0 CTC Mode with compare match A interruption setup

    TCCR0A = 0b00000010; // Bits 1:0 - CTC mode (WGM01:0)
    TCCR0B = 0b00000101; // Bit 3 - CTC mode (WGM02) Bits 2:0 – CS02:0: Clock Select (clk I/O /1024)
    OCR0A  = 3;          // Generates an interrupt every 4096 clock cycles
    TIMSK0 = 0b00000010; // Bit 1 – OCIE0A (Compare Match A Interrupt Enable on Timer 0)

}





// Occurs every 4096 clock cycles. It XLAT's the GS data to the IC internal registers
ISR(TIMER0_COMPA_vect){
    static uint8_t xlat = 0;
    uint8_t firstCycle = state(VPRG_PORT, VPRG_PIN);


    high(BLANK_PORT, BLANK_PIN);
    

	if(firstCycle) low(VPRG_PORT, VPRG_PIN);
    if(xlat) {
        pulse(XLAT_PORT, XLAT_PIN);
        xlat = 0;
    }
    if(firstCycle) {
        pulse(SCLK_PORT, SCLK_PIN);
        firstCycle = 0;
    }


	low(BLANK_PORT, BLANK_PIN);


    // Shifts Gray Scale (GS) data inside
    EntireGSData();  
    xlat = 1;

}


// Shifts entire Gray Scale (GS) data on GSData array
void EntireGSData ( void ) {

    for(uint8_t counter = 0; counter < TLC5940_N * 16; counter++){
		SerialInput( 12 , GSData[counter] );
	}   

}


void SetEntireDC ( uint8_t value ) {

    for(uint8_t i = 0; i < TLC5940_N * 16; i++){
        DCData[i] = value;
    }

}


int main ( void ){

	setup_clock();
	TLC5940_init();
    
    SetEntireDC( 0x06);
	ClockDCData();

    sei(); // Enables global interruptions

	for(;;){

        //gsmodled(0, 2, 0x0FFF);
        //gsmod(16, 0xFFF);
        //gsmod(0,  0xFFF);
        //gsmod(1,  0xFFF);
        gsmod(31, 0xFFF);
	}


	return 0;
}


