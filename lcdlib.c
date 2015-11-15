#define F_CPU 8000000 //remove this after testing

#include <avr/io.h>
#include <util/delay.h>
#include "RBLT.h"



#define CONTROLPORT 	PORTB  	// The port on which the control pins are
#define CONTROLPORTDIR	DDRB	// The control register for the control pins
#define ENABLE 			0 		// The pin for the enable signal on the control port //1= lcd reads stuff, falling edge = lcd does stuff	
#define REGISTERSELECT 	7		// The pin for the RS signal on the control port // 1=character, 0=command
#define READWRITE 		6		// The pin for the RW signal on the control port // 1= read, 0=write
#define DATAPORT		PORTD	// The port on which the data pins are written to
#define DATAPIN			PIND	// The input register for the data pins
#define DATAPORTDIR		DDRD	// The control register for the data pins
#define REVERSED		1		// Set this to one if the data port between uC and LCD are reversed. 0 Otherwise.
								// Reversed uses a 300 byte lookuptable and  few cycles per character. Not recommended.
								// You can set Reversed to 0 and manually invoke SendCharacter(RBLT(OUTPUT)) to have
								// The preprocessor optimize it out (if its static). Wont work on runtime generated output.

#if REVERSED
#define OUT RBLT
#else
#define OUT(X) X
#endif

#define SHIFTCURSORRIGHT	SendCommand(OUT(0x14))
#define SHIFTCURSORLEFT		SendCommand(OUT(0x10))
#define SHIFTSCREENRIGHT	SendCommand(OUT(0x1C))
#define SHIFTSCREENLEFT		SendCommand(OUT(0x18))
#define CLEARDISPLAY		SendCommand(OUT(0x01))
#define	HOMEDISPLAY			SendCommand(OUT(0x02))

; // just here to fix a syntax highlight bug in sublime
static void SendCommand(unsigned char command){
	SendCommandASM(OUT(command));
}

static void SendCharacter(unsigned char character){
	SendCharacterASM(OUT(character));
}

static void SendString(char *String){
	while(*String){
		SendCharacterASM(*String++);
	}
}

volatile int XD = 0x38;
void InitLcd(void){
	CONTROLPORTDIR |= _BV(ENABLE);
	CONTROLPORTDIR |= _BV(REGISTERSELECT);
	CONTROLPORTDIR |= _BV(READWRITE);
	_delay_ms(40);
	SendCommand(0x38); //Function set, for reset
	_delay_us(40);
	SendCommand(0x38); //Function set, for real
	_delay_us(40);
	SendCommand(0x0E); //Display, cursor and blink off
	_delay_us(40);
	SendCommand(0x01); //Clear the display
	_delay_ms(4);
	SendCommandASM(0x06); //Entry mode set to increment, no shift
}