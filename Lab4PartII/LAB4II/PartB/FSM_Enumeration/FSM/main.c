/*
 * FSM.c
 *
 * Author : Luis
 */ 

#include <avr/io.h>
#include <stdlib.h>
#include "Drivers/set_timer.h"
#include "Drivers/states.h"


int main(void)
{
	//FAST PWM  TOP 0xFF
	//NON-INVERTING
	//No Prescaling
	TC0_FAST_PWM_D_TOP();
	TCCR0A |= (1<<COM0A1);
	TC0_NOPRESCALING();
	//Initially at 50% duty cycle
	OCR0A = 128;

  States* currentState = (States*)calloc(1,sizeof(States));
  *currentState = mymain;
   while (1){
	   switch (*currentState){	
		   case sleep:sleepOpt(currentState);break;
		   case ledOpt:	ledControl(currentState);break;
		   case ledOnOpt:ledOn(currentState);break;
		   case moreLedOpt:moreLedOptions(currentState);break;
		   case changeBright:ledBright(currentState);break;
		   default:mymainMenu(currentState);
		   break;
	   }
   }
}

