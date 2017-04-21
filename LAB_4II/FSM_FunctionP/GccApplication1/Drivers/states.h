#ifndef STATES_H 
#define STATES_H
void delay_ms(int n);	
void mymainMenu(void(* currentState)(void));
void sleepOpt(void(* currentState)(void));	
void ledControl(void(* currentState)(void));	
void ledOn(void(* currentState)(void));			
void moreLedOptions(void(* currentState)(void));	
void ledBlink();	
void ledBright(void(* currentState)(void));	

#endif
