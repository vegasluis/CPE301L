/*
CpE 301L
This file contains the completed Lab 4 Part 1.
This file does not have any external header files, and nothing is put into specific functions.
There will be another file for functions and the header files. 
*/

/**
* @mainpage
* Authors: Joshua Perry & Luis Ruiz 
* \n Class: CPE 301L
* \n Section: 1001
* \n Lab 4 Part II
*/

//!#define TIMER1VAL 16000000
//!! clock speed of Atmega328p - 16MHz
#define F_CPU 16000000	//! Clock Speed
//!! UART Baud Rate - Default: 9600 bauds/second
#define BAUD  9600	//! Baud Rate for UART

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include "temp.h"
#include "array.h"
#include "matrix.h"


//! functions
//! Initialize UART in the Atmega328p
void initUART();
void initialize_timer0();
void writeChar(unsigned char c);
void writeString(char * str);
void menu(char *);
void gotosleep();
void LED_on();
void LED_off();
void initLED();
void custom_LED(int);
void Execute_periodic(int, int);

volatile unsigned char charRead;
//! check if s menu was triggered in ISR
volatile int s_flag = 0; 	
//! check if b menu was triggered in ISR   
volatile int LED_menu_flag = 0;
//! check if LED is on enter its sub menu 
volatile int LED_on_flag = 0; 
//! check if PWM has been selected to enter its menu  
volatile int PWM_menu_flag = 0; 
//! custom PWM was selected
volatile int PWM_custom_flag = 0;
//! how often to blink 
volatile int BlinkTime_flag1 = 0;
//! how long to blink for 
volatile int BlinkTime_flag2 = 0;
//! final flag  
volatile int BlinkTime_flag3 = 0; 
int temp1 = 0;
int temp2 = 0;


//! menu statements
char * menu1_Options = " Select an option:\r\n a. Print Message\r\n s. Other\r\n b. LED Control\r\n\r\n ";
char * menu2_Options = " Select an option:\r\n 1. Sleep\r\n 2. Power Down \r\n\r\n ";
char * menu3_Options = " Select an option:\r\n 1. Turn on LED\r\n 2. Turn off LED\r\n 3. Return to menu\r\n\r\n ";
char * menu4_Options = " Select an LED Option:\r\n 1. Stay On\r\n 2. Blink Once\r\n 3. Make Periodic\r\n 4. PWM\r\n\r\n";
char * menu5_Options = " Select a Periodic Option:\r\n l. Low\r\n m. Medium\r\n h. High\r\n c. Custom\r\n\r\n";
char * error = "\r\n Error, not an available choice.\r\n\r\n";
//! Holds received chars
char buff[10]; 

volatile int pos = 0;
//! number of times it will blink
volatile int intervalTime = 0; 
//! time it will blink for
volatile int blinkDuration = 0; 



/**
* @defgroup mainFunct Main Function
* @brief Runs the entire operation of the menu system along with its functionalities
* @{
*/
//! Main Function
int main(void)
{
	initialize_timer0();
	//! Initialize UART in the Atmega328p
	initUART();
	initLED();
	//! print the main menu
	menu(menu1_Options); 
	while(1)
	{
		if(BlinkTime_flag1 == 1 && BlinkTime_flag2 ==1 && BlinkTime_flag3 == 1)
		{
			Execute_periodic(intervalTime , blinkDuration);
		}
	}//!while

}//!main

//! Transmit/write one character to the output
void writeChar(unsigned char c)
{
	UDR0 = c;	//! Display character on serial (i.e., PuTTY) terminal
	_delay_ms(10);
}


//! Transmit/write a NULL-terminated string to the output
void writeString(char * str)
{
	while(*str != 0x00)
	{
		writeChar(*str);
		str++;
	} //!while
}

/**@}*/

/**
* @defgroup UARTfunct UART I/O
* @brief Enables the UART functionality in the microcontroller
* @{
*/

void initUART()
{
	unsigned int baudrate;

	//! Set baud rate:  UBRR = [F_CPU/(16*BAUD)] -1
	baudrate = ((F_CPU/16)/BAUD) - 1;	//! set baud rate (9600)
	UBRR0H = (unsigned char) (baudrate >> 8);
	UBRR0L = (unsigned char) baudrate;

	//! Enable receiver and transmitter
	UCSR0B |= (1 << RXEN0) | (1 << TXEN0);	
	//! Set data frame: 8 data bits, 1 stop bit, no parity	
	UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00);	
	//! enable receiver interrupt
	UCSR0B |= (1 << RXCIE0);	
	//! enable global interrupts				
	sei();										
}

void menu(char * menu)
{
	writeString(menu);
}

//! UART receiver interrupt service routine
ISR (USART_RX_vect)
{
	if (PWM_custom_flag == 1) //read custom PWM if option was selected
	{
		unsigned char receivedChar;
		receivedChar= UDR0;
		UDR0 = receivedChar;
		charRead = receivedChar;
		buff[pos] = receivedChar;
		if (buff[pos] == '\r')
		{
			//convert entire buff to int
			int intBuff = atoi(buff);
			custom_LED(intBuff); //set PWM
			PWM_custom_flag = 0; //reset custom flag
			writeString("\r\n");
			menu(menu1_Options); //print main menu
		}
		else
		pos++;
	}//!if
	else if (BlinkTime_flag1 == 1 && BlinkTime_flag2 == 0)
	{
		unsigned char receivedChar;
		receivedChar= UDR0;
		UDR0 = receivedChar;
		charRead = receivedChar;
		buff[pos] = receivedChar;
		if (buff[pos] == '\r')
		{
			// convert entire buff to int
			int intBuff = atoi(buff);
			intervalTime = intBuff; // how many times it will blink
			BlinkTime_flag2 = 1;
			writeString("\r\nEnter Time to keep blinking ( miliseconds): "); //enter time for blinking


		}
		else
		pos++;
	}//! else if for first BlinkTime_flag1

	else if (BlinkTime_flag1 == 1 && BlinkTime_flag2 == 1 && BlinkTime_flag3 == 0)
	{
		unsigned char receivedChar;
		receivedChar= UDR0;
		UDR0 = receivedChar;
		charRead = receivedChar;
		buff[pos] = receivedChar;
		if (buff[pos] == '\r')
		{
			// convert entire buff to int
			int intBuff = atoi(buff);
			blinkDuration = intBuff;
			BlinkTime_flag3 = 1; // all flags set now 
		}
		else
		pos++; 
	}
	else
	{
		unsigned char receivedChar;
		receivedChar= UDR0;
		UDR0 = receivedChar;
		charRead = receivedChar;

		/* These are the flag options */
		if(s_flag == 1) // was the s menu previously triggered?
		{
			if (charRead == '1')
			{
				writeString(" you pressed sleep\r\n");
				gotosleep(); // Entered sleep mode
				s_flag = 0;			  // reset the s flag to zero
			}
			else if (charRead == '2')
			{
				writeString(" you pressed power down\r\n");
				//TODO : ded
				s_flag = 0;			  // reset the s flag to zero
			}
			else
			{
				writeString(error);   // Display error message
				menu(menu1_Options);  // display main menu again
				s_flag = 0;			  // reset the s flag to zero
			}
		} //! if s_flag == 1

		else if (LED_menu_flag == 1 && LED_on_flag == 0) // after b was selected, now in LED selection
		{
			if (charRead == '1')
			{
				cli();
				writeString(" LED turning ON\r\n");
				custom_LED(100); // high at all times
				LED_on_flag = 1;
				menu(menu4_Options);
				sei();
			}
			else if (charRead == '2')
			{
				writeString(" LED turning OFF\r\n");
				custom_LED(0); //low at all times
				LED_menu_flag = 0;
				menu(menu1_Options);  // display main menu again
			}
			else if (charRead == '3')
			{
				LED_menu_flag = 0;
				menu(menu1_Options);  // display main menu again
			}
		}

		else if (LED_on_flag == 1 && LED_menu_flag == 1 && PWM_menu_flag == 0)
		{
			if (charRead == '1') // Keep the LED on
			{
				writeString("\r\nLED is staying ON\r\n");
				menu(menu1_Options); // display main menu again
				LED_on_flag = 0;
				LED_menu_flag = 0;

				BlinkTime_flag1 = 0;
				BlinkTime_flag2 = 0;
				BlinkTime_flag3 = 0; 

			}
			else if (charRead == '2') // Blink the LED once
			{
				cli(); 	// disable interrupts
				writeString("\r\nLED turning off for blinking\r\n");
				_delay_ms(1000);
				custom_LED(0); // off
				_delay_ms(1000);
				writeString("\r\nLED will begin blinking\r\n");
				custom_LED(100); // on
				_delay_ms(1000);
				menu(menu1_Options); //display main menu again
				LED_on_flag = 0;
				LED_menu_flag = 0;
				sei(); 	// enable interrupt

				BlinkTime_flag1 = 0;
				BlinkTime_flag2 = 0;
				BlinkTime_flag3 = 0;
			}
			else if (charRead == '3') // Set periodic
			{
				writeString("\r\nEnter Time to Blink (seconds): "); // Promt the user for a custom Periodic Value
				LED_on_flag = 0;
				LED_menu_flag = 0;
				BlinkTime_flag1 = 1;
				int i;
				for(i = 0; i < 10; i++)
					buff[i] = 0x00;
					
				pos = 0; //clear position
			}
			else if (charRead == '4') // PWM -> bring up submenu
			{
				writeString("\r\nPWM Selected\r\n");
				menu(menu5_Options); // display main menu again
				//LED_on_flag = 0;
				//LED_menu_flag = 0;
				PWM_menu_flag = 1; // set PWM menu flag
				BlinkTime_flag1 = 0;
				BlinkTime_flag2 = 0;
				BlinkTime_flag3 = 0;
			}

		}
		else if(LED_on_flag == 1 && LED_menu_flag == 1 && PWM_menu_flag == 1) // PWM Options Menu
		{
			if (charRead == 'l') // Low PWM (10%)
			{
				cli();
				writeString("\r\nPWM set to low\r\n");
				custom_LED(10);
				//reset all flags
				LED_on_flag = 0;
				LED_menu_flag = 0;
				PWM_menu_flag = 0;
				//return to main menu
				menu(menu1_Options);
				sei();
			}//!low
			else if (charRead == 'm') // Medium PWM (50%)
			{
				cli();
				writeString("\r\nPWM set to medium\r\n");
				custom_LED(50);
				//reset all flags
				LED_on_flag = 0;
				LED_menu_flag = 0;
				PWM_menu_flag = 0;
				//return to main menu
				menu(menu1_Options);
				sei();
			}//!medium
			else if (charRead == 'h') // High PWM (90%)
			{
				cli();
				writeString("\r\nPWM set to high\r\n");
				custom_LED(90);
				//reset all flags
				LED_on_flag = 0;
				LED_menu_flag = 0;
				PWM_menu_flag = 0;
				//return to main menu
				menu(menu1_Options);
				sei();
			}//!high
			else if (charRead == 'c') // Custom PWM -> Query the User
			{
				writeString("\r\nEnter Custom PWM: ");
				//reset all flags except PWM
				LED_on_flag = 0;
				LED_menu_flag = 0;
				PWM_menu_flag = 0;
				// custom PWM selected
				PWM_custom_flag = 1;
				//clear buffer
				int i;
				for(i = 0; i < 10; i++)
				buff[i] = 0x00;
				//clear position
				pos = 0;
			}//custom

		}//PWM sub-options menu

		/* There are the main menu options */
		else
		{
			if (charRead == 'a') // a was pressed
			{
				writeString("\r\nHello World!\r\n");
				menu(menu1_Options);
			}
			else if (charRead == 's')  // s was pressed
			{
				s_flag = 1;	// trigger s flag
				menu(menu2_Options);
			}
			else if (charRead == 'b')
			{
				LED_menu_flag = 1; // trigger LED menu flag
				menu(menu3_Options);
			}
			else //incorrect key press, display error message
			{
				writeString(error);   //Display error message
				menu(menu1_Options);  //display main menu again
			}
		} //! else s_flag = 0

	}//!else !(buff[pos] == '\r' && PWM_menu_flag == 1)

} //!ISR

//! Sleep Mode function
void gotosleep()
{
	set_sleep_mode(SLEEP_MODE_IDLE);
	cli();
	sleep_enable();
	sleep_bod_disable();
	sei();
	sleep_cpu();
	sleep_disable();
	sei();
} //! end gotosleep()

//! initialize PB1 as output for timer0
void initLED()
{
	DDRD |= (1<<PD6);	
	//PORTB &= (0<<PB1);  //set to 0, PB1
	
} //! end initLED


void initialize_timer0()
{
	//! Set up Timer0
	//! initial Output Compare for Timer0
	OCR0A = 0;	
	//! Fast PWM + Non-inverted Mode
	TCCR0A = (1 << WGM01)|(1 << WGM00)|(1 << COM0A1) ;	
	TCCR0B |= (1 << CS00);	//No Prescaling
	TCNT0 = 0;	//Initialize to 0
}

//! customize PWM period for LED
void custom_LED(int cust)
{
	unsigned int i = cust;
	//! adjust PWM period
	OCR0A = ((255 * i) / 100); 
}

// ! determine the interval for blinking 
void Execute_periodic(int intervalTime , int blinkDuration)
{
	//! turn on after interval time
	delay_ms(intervalTime*1000); 
	//!turn on
	custom_LED(100);
	//! now once its on, hold the LED for blinkDuration
	delay_ms(blinkDuration);
	 //!turn off LED
	custom_LED(0);
}

// !set a delay 
void delay_ms(int intervalTime)
{
	for(int i = 0; i < intervalTime; i++)
	{
		_delay_ms(1);
	}
}