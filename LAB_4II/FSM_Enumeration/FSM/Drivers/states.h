#ifndef STATES_H 
#define STATES_H
#include "myState.h"
void delay_ms(int n);	
void mymainMenu(States* currentState);
void sleepOpt(States* currentState);	
void ledControl(States* currentState);	
void ledOn(States* currentState);			
void moreLedOptions(States* currentState);	
void ledBlink();	
void ledBright(States* currentState);	
#endif
