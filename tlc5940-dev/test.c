#include <avr/io.h>
#include <stdint.h>

#define TLC5940_N 1

#define GSCLK_DDR DDRD
#define GSCLK_PORT PORTD
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

#define output(DDR,  PIN) (DDR |= _BV(PIN))
#define input(DDR,  PIN) (DDR &= ~_BV(PIN))
#define high(PORT, PIN) (PORT |= _BV(PIN))
#define low(PORT, PIN) (PORT &= ~_BV(PIN))
#define state(PORT, PIN) (PORT & _BV(PIN))
#define pulse(PORT, PIN) do{\
													high(PORT, PIN); \
													low(PORT, PIN); \
												  } while(0)

static void setup_clock( void ){

	CLKPR = 0x80;	// For more information on this procedure look at page 37
	CLKPR = 0x00;

}


uint8_t DCData[96 * TLC5940_N] = {
	// MSB					  LSB
			1, 1, 1, 1, 1, 1,	//Channel 15
			1, 1, 1, 1, 1, 1,	//Channel 14
			1, 1, 1, 1, 1, 1,	//Channel 13
			1, 1, 1, 1, 1, 1,	//Channel 12
			1, 1, 1, 1, 1, 1,	//Channel 11
			1, 1, 1, 1, 1, 1,	//Channel 10
			1, 1, 1, 1, 1, 1,	//Channel 9
			1, 1, 1, 1, 1, 1,	//Channel 8
			1, 1, 1, 1, 1, 1,	//Channel 7
			1, 1, 1, 1, 1, 1,	//Channel 6
			1, 1, 1, 1, 1, 1,	//Channel 5
			1, 1, 1, 1, 1, 1,	//Channel 4
			1, 1, 1, 1, 1, 1,	//Channel 3
			1, 1, 1, 1, 1, 1,	//Channel 2
			1, 1, 1, 1, 1, 1,	//Channel 1
			1, 1, 1, 1, 1, 1	//Channel 0
};

uint8_t GSData[TLC5940_N * 192] = {
	// MSB                              LSB
	/*
	0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0,
	*/
	// CHANNEL 15
	//0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,

	// CHANNEL 14
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	//1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,

	// CHANNEL 13
	//0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,

	// CHANNEL 12
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	//1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,

	// CHANNEL 11
	//0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,

	// CHANNEL 10
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	//1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,

	// CHANNEL 9
	//0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,

	// CHANNEL 8
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	//1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,

	// CHANNEL 7
	//0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,

	// CHANNEL 6
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	//1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,

	// CHANNEL 5
	//0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,

	// CHANNEL 4
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	//1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,

	// CHANNEL 3
	//0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,

	// CHANNEL 2
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	//1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,

	// CHANNEL 1
	//0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,

	// CHANNEL 0
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	//1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,

};

void ClockDCData ( void ) {

	high(DCPRG_PORT, DCPRG_PIN);
	high(VPRG_PORT,  VPRG_PIN);

	uint8_t counter;

	for(counter = 0; counter < (TLC5940_N * 96) - 1; counter++){
		DCData[counter] ? high(SIN_PORT, SIN_PIN) : low(SIN_PORT, SIN_PIN);
		pulse(SCLK_PORT, SCLK_PIN);
	}

	pulse(XLAT_PORT, XLAT_PIN);

}

void SetGS_and_GS_PWM ( void ) {
	uint8_t firstCycle = state(VPRG_PORT, VPRG_PIN);
	if(firstCycle) low(VPRG_PORT, VPRG_PIN);

	uint16_t GSCLK_counter = 0;
	uint8_t  Data_counter  = 0;

	low(BLANK_PORT, BLANK_PIN);

	for(GSCLK_counter = 0; GSCLK_counter < 4095; GSCLK_counter++){
		if(Data_counter < (TLC5940_N * 192)) {
			GSData[Data_counter] ? 	high(SIN_PORT, SIN_PIN) : low(SIN_PORT, SIN_PIN);
			pulse(SCLK_PORT, SCLK_PIN);
			Data_counter++;
		}

		pulse(GSCLK_PORT, GSCLK_PIN);
	}

	high(BLANK_PORT, BLANK_PIN);
	pulse(XLAT_PORT, XLAT_PIN);

	if(firstCycle) {
		pulse(SCLK_PORT, SCLK_PIN);
		firstCycle = 0;
	}
}

void TLC5940_init ( void ){

	output(GSCLK_DDR, GSCLK_PIN);
	output(SIN_DDR, SIN_PIN);
	output(SCLK_DDR, SCLK_PIN);
	output(BLANK_DDR, BLANK_PIN);
	output(DCPRG_DDR, DCPRG_PIN);
	output(VPRG_DDR, VPRG_PIN);
	output(XLAT_DDR, XLAT_PIN);

	low(GSCLK_PORT, GSCLK_PIN);
	low(SCLK_PORT, SCLK_PIN);
	low(SIN_PORT, SIN_PIN);
	low(XLAT_PORT, XLAT_PIN);
	low(DCPRG_PORT, DCPRG_PIN);
	high(VPRG_PORT, VPRG_PIN);
	high(BLANK_PORT, BLANK_PIN);

}

int main ( void ){
	setup_clock();
	TLC5940_init();
	ClockDCData();

	for(;;){

		SetGS_and_GS_PWM();

	}

	return 0;
}
