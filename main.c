//*****************************************************************************
//
// MSP432 main.c
//
// CSci 4454 Assignment #1
// Flashing an RGB LED
//
// Myles Gavic
//
// MSP432 LaunchPad
// S1  P1.1
// S2  P1.4
//
// Assignment 1
// The RGB LED has three lines associated with it: the red LED is 2.0, the green LED is 2.1,
// and the blue LED is 2.2. Write a program that uses the pushbutton on 1.0 to select between
// constant cycle and manual cycle, and cycles through the 8 simple colors of this LED (Off,
// Red, Green, Yellow (Red+Green), Blue, Violet (Red+Blue), Teal (Blue+Green), and White
// (Red+Green+Blue))5 When manually cycling, use the pushbutton on 1.4 to cycle through
// the colors.
//
//
//****************************************************************************


#include <msp.h>


void selectionRegister1 (uint8_t bit)
{
	if (P1SEL0 & bit){
		if (P1SEL1 & bit)
			P1SELC|=bit; // Changing both at the same time
		else
			P1SEL0&=~bit; // Changing only the one in register 0
	}
	else if (P1SEL1 & bit)
		P1SEL1&=~bit; // Changing only the one in register 1
}

void selectionRegister2 (uint8_t bit)
{
	if (P2SEL0 & bit){
		if (P2SEL1 & bit )
			P2SELC|=bit ;
		else
			P2SEL0&=~bit;
	}
	else if (P2SEL1 & bit )
		P2SEL1&=~bit ;
}



void initalizeLED(void){

	//RED
	P2DIR|=BIT0 ;
	selectionRegister2(BIT0) ;

	//GREEN
	P2DIR|=BIT1 ;
	selectionRegister2(BIT1) ;

	//BLUE
	P2DIR|=BIT2 ;
	selectionRegister2(BIT2) ;


}

void setColorRGB(unsigned int n){

	switch(n){

	//001 RED
	case 0 :
		P2OUT&=~BIT0 ;
		P2OUT&=~BIT1 ;
		P2OUT|=BIT2 ;
		break;
		//010 GREEN
	case 1 :
		P2OUT&=~BIT0 ;
		P2OUT|=BIT1 ;
		P2OUT&=~BIT2 ;
		break;
		//011 YELLOW
	case 2 :
		P2OUT&=~BIT0 ;
		P2OUT|=BIT1 ;
		P2OUT|=BIT2 ;
		break;
		//100 BLUE
	case 3 :
		P2OUT|=BIT0 ;
		P2OUT&=~BIT1 ;
		P2OUT&=~BIT2 ;
		break;
		//101 VIOLET
	case 4 :
		P2OUT|=BIT0 ;
		P2OUT&=~BIT1 ;
		P2OUT|=BIT2 ;
		break;
		//110 TEAL
	case 5 :
		P2OUT|=BIT0 ;
		P2OUT|=BIT1 ;
		P2OUT&=~BIT2 ;
		break;
		//111 WHITE
	case 6 :
		P2OUT|=BIT0 ;
		P2OUT|=BIT1 ;
		P2OUT|=BIT2 ;
		break;
		//000 OFF
	case 7 :
		P2OUT&=~BIT0 ;
		P2OUT&=~BIT1 ;
		P2OUT&=~BIT2 ;
		break;
	}
}


void initalize(void){
	// Setting the direction of port 1 line 1 (setting to 0)
	P1DIR&=~BIT1;
	// Setting the direction of port 1 line 4 (setting to 0)
	P1DIR&=~BIT4;

	// Pullup resistors
	P1REN|=BIT1 ;
	P1OUT|=BIT1 ;

	P1REN|=BIT4 ;
	P1OUT|=BIT4 ;
}


void main (void)
{
	WDTCTL = WDTPW | WDTHOLD; // Stop watchdog timer

	initalizeLED();
	initalize();


	// mode state - default is self cycle.
	unsigned int stop = 0;
	// color - default is red
	unsigned int color = 0;

	while(1){
		// read input states
		unsigned char portInput = P1IN ;

		// switch light patterns
		if (!(portInput & BIT1))
			stop=!stop;

		if(stop){
			// If switch is pushed, advance a color.
			if(!(portInput & BIT4)){
				if(color < 8){
					color = color + 1;
					setColorRGB(color);
				}else{
					color = 0;
					setColorRGB(color);
				}
			}

		}else{
			if(color < 8){
				color = color + 1;
				setColorRGB(color);
			}else{
				color = 0;
				setColorRGB(color);
			}
		}

		//interval speed of the flashing light
		volatile int k=0;
		for ( k=0;k<20000;++k ) ;
	}
}
