 #define F_CPU 16000000UL
 #define LEDPORT PORTB5			

 #include <avr/io.h>
 #include <stdlib.h>
 #include <avr/interrupt.h>
 #include <util/delay.h>
 #include <avr/sleep.h>
 #include "ioe.h"
 #include "states.h"

 void delay_ms(int n) {	
	for(int i =0; i < n; ++i)
		_delay_ms(1);
 }

//MENU OPTIONS
void mymainMenu(void(* currentState)())	{
	uint8_t key;	

	printm("Main Menu:\r\n");
	printm("\t<a> Hello World!\r\n");
	printm("\t<s> Sleep options\r\n");
	printm("\t<d> LED control\r\n");

	key = getChar_();		
	switch (key){
		case 'a':
			printm("\r\nHello World!\r\n");	
			currentState = mymainMenu;
			break;
		case 's':
			currentState = sleepOpt;	
			break;
		case 'd':
			currentState = ledControl;		
			break;
		default:
			currentState = mymainMenu;
			printm("Error.mymain(): Unavailable option\r\n");	
			break;
	}
}

//SLEEP MENU
void sleepOpt(void(* currentState)())	
{
	uint8_t sleep;	

	//SLEEP OPTIONS
	printm("Sleep Options:\r\n");	
	printm("\t<1> Sleep\r\n");
	printm("\t<2> Power down\r\n");
	printm("\t<m> Main menu\r\n");

	sleep = getChar_();		//get sleep choice
	switch (sleep){
		case '1':
			printm("Press hardware button to wake\r\n");
			_delay_ms(10);
			//Sleep Mode
			set_sleep_mode(SLEEP_MODE_PWR_SAVE);
			sleep_mode();		
			_delay_ms(1);
			break;
		case '2':
			printm("Press hardware button to wake\r\n");
			_delay_ms(10);
			//Go into power down mode 
			set_sleep_mode(SLEEP_MODE_PWR_DOWN);	
			sleep_mode();		
			_delay_ms(1);
			break;
		case 'm':
			currentState = mymainMenu;	
			break;
		default:
			currentState = mymainMenu;
			printm("Error.sleep(): Unavailable option\r\n");	
			break;
	}
}

//LED Menu
void ledControl(void(* currentState)()){

	uint8_t led;
	//LED PRINTED MENU		
	printm("LED CONTROL:\r\n");		
	printm("\t<1> LED on\r\n");		
	printm("\t<2> More LED options\r\n");		
	printm("\t<3> LED brightness options\r\n");	
	printm("\t<m> Main menu\r\n");
	
	led = getChar_();			
	switch (led){
		case '1':
			currentState = ledOn;		
			break;
		case '2':
			currentState = moreLedOptions;	
			break;
		case '3':
			currentState = ledBright;	
			break;
		case 'm':
			currentState = mymainMenu;	
			break;
		default:
			currentState = mymainMenu;	
			printm("Error.ledControl(): Unavailable option\r\n");	
			break;
	}
}
//LED MENU
void ledOn(void(* currentState)())		
{
	uint8_t off;			
	DDRD |= (1<<LEDPORT);	

	//PRINTED MENU
	printm("Turn LED Off?:\r\n");	
	printm("\t<y> yes\r\n");
	printm("\t<n> no\r\n");

	off = getChar_();		
	switch (off){
		case 'y':
			DDRD &= ~(1<<LEDPORT);	
			currentState = ledControl;			
			break;
		case 'n':
			currentState = ledControl;		
			break;
		default:
			currentState = mymainMenu;
			printm("Error.ledOn(): Unavailable option\r\n");	
			break;
	}
}

//MORE OPTIONS
void moreLedOptions(void(* currentState)())	
{
	uint8_t led;	
	uint8_t *t = (uint8_t *) calloc(8,sizeof(uint8_t));		

	printm("Other LED Options:\r\n");	
	printm("<1> LED remain on\r\n");		
	printm("<2> LED blink one time\r\n");		
	printm("<3> LED blink functions\r\n");	
	printm("<b> Back\r\n");
	printm("<m> Main menu\r\n");
	
	led = getChar_();			
	switch (led){
		case '1':
			currentState = ledControl;		
			break;
		case '2':
			DDRD &= ~(1<<LEDPORT);	
			printm("Please enter the how long you want the led to blink:\r\n");
			scanm(t);			
			DDRD |= (1<<LEDPORT);	
			delay_ms(atoi((const char*)t));		
			DDRD &= ~(1<<LEDPORT);	
			break;
		case '3':
			ledBlink();			
			break;
		case 'b':
			currentState = ledControl;		
			break;
		case 'm':
			currentState = mymainMenu;		
			break;
		default:
			printm("Error.moreLedOptions(): Unavailable option\r\n");	
			break;
	}
	free(t);	
}

void ledBlink()		
{
	uint8_t *t = (uint8_t *) calloc(8,sizeof(char));		
	uint8_t *r = (uint8_t *) calloc(8,sizeof(char));		

	DDRD &= ~(1<<LEDPORT);	
	printm("Repeatly Blink the LED\r\n");
	printm("Input how long you want the LED to blink(ms):\r\n");
	scanm(t);			
	printm("How frequently do you want the LED to blink(ms):\r\n");
	scanm(r);				
	printm("Press any key to stop\r\n");
	while (!(UCSR0A & (1<<RXC0)))
	{
		DDRD |= (1<<LEDPORT);			
		delay_ms(atoi((const char*)t));		
		DDRD &= ~(1<<LEDPORT);				
		delay_ms(atoi((const char*)r));		
	}
	UCSR0B &= ~(1<<RXEN0);		
	UCSR0B |= (1<<RXEN0);		
	free(t);	
	free(r);	
}

//ADJUST THE BRIGHTNESS
void ledBright(void(* currentState)())	
{
	uint8_t led;		
	uint8_t *t = (uint8_t*) calloc(8,sizeof(char));

	DDRD |= (1<<LEDPORT);	//turn on LED

	printm("LED brightness Options:\r\n");	//print brightness menu
	printm("\t<1> Low\r\n");
	printm("\t<2> Medium\r\n");
	printm("\t<3> High\r\n");
	printm("\t<4> Custom\r\n");
	printm("\t<b> Back\r\n");
	printm("\t<m> Main menu\r\n");
	
	led = getChar_();		
	switch (led){
		case '1':
			OCR0A = 26;		//set 10% duty cycle
			break;
		case '2':
			OCR0A = 128;	//set 50% duty cycle
			break;
		case '3':
			OCR0A = 230;	//set 90% duty cycle
			break;
		case '4':
			printm("Input desired brightness? (0 - 99):\r\n");
			scanm(t);
			OCR0A = (atoi((const char*)t)*255)/100;	
			break;
		case 'b':
			currentState = moreLedOptions;	
			break;
		case 'm':
			currentState = mymainMenu;
			break;
		default:
			printm("Error.ledBright(): Unavailable option\r\n");
			break;
	}
	free(t);
}