

#include <avr/io.h>
#include <util/delay.h> 
#include <stdlib.h>
#include <string.h>

#define BUFSIZE 9
#define F_CPU 16000000

//Conversions
float ctof(float temperature);
float ftoc(float temperature);

void randfillarray();

float maxarray (float *tempArray);
float minarray (float *tempArray);

void basicstats(float *tempArray, float *mean, float *median, float *mode);
void arrayprint(float *tempArray);

void writeString(char * str);
void writeChar(unsigned char c);


float findmean(float *tempArray, float *mean);
float findmedian(float *tempArray, float *median);
float findmode(float *tempArray, float *mode);

int main(void)
{
	float temperatures[5] = {20 , -20, 54, 30};
    /* Replace with your application code */
    while (1) 
    {
    }
}
/*Celsius to Fahrenheit conversion*/
float ctof(float temperature){
	return (temperature * 9/5) + 32; 
}
/*Fahrenheit to Celsius conversion*/
float ftoc(float temperature){	
	return (temperature -32) * (5/9); 
}

/* Creates an array based on user input and fills with 
 * numbers for temperature conversion. Unlike the Array Application,
 * this has constraints. You cannot go above 50c or under -90c
 */ 
void randfillarray(){
	
}
/* Find the lowest temperature in the array and returns the day and temperature */
float maxarray (float *tempArray){
	float max = tempArray[0];
	for(int i = 0; i < sizeof(tempArray) ; i++)
	{
		 // Change < to > if you want to find the smallest element
		 if(tempArray[0] < tempArray[i])
			 max = tempArray[i];
	}
	return max;
}
/* find the highest temperature in the array and returns the day and temperature */ 
float minarray (float *tempArray){
	
	float min = tempArray[0];
	for(int i = 0; i < sizeof(tempArray) ; i++)
	{
		// Change < to > if you want to find the smallest element
		if(tempArray[0] < tempArray[i])
			min = tempArray[i];
	}
	return min;
}
/* Finds the mean, median, and mode of the array */
void basicstats(float *tempArray, float *mean, float *median, float *mode){
	findmean(tempArray, mean);
	findmedian(tempArray, mode);
	findmode(tempArray, mode);
} 

float findmean(float *tempArray, float *mean){
	  double sum = 0;
	  for (int i = 0; i < sizeof(tempArray); i++) {
		  sum += tempArray[i];
	  }
	  return sum / sizeof(tempArray);
}

float findmode(float *tempArray, float *median){
	    int maxValue = 0;
		int maxCount = 0;

	    for (int i = 0; i <  sizeof(tempArray); ++i) {
		    int count = 0;
		    for (int j = 0; j <  sizeof(tempArray); ++j) {
			    if (tempArray[j] == tempArray[i]) ++count;
		    }
		    if (count > maxCount) {
			    maxCount = count;
			    maxValue = tempArray[i];
		    }
	    }

	    return maxValue;
}
float findmedian(float *tempArray, float *median){
	int middle = sizeof(tempArray)/2;
	if (sizeof(tempArray)%2 == 1) {
		return tempArray[middle];
		} else {
		return (tempArray[middle-1] + tempArray[middle]) / 2.0;
	}
}
/* Prints the array in a format. Each index position of the array is
 * related to a day. so [20 -20 54 3 0] would print Day 1: 2 day 2: 0
 */
void arrayprint(float *tempArray) {
	char dayString[BUFSIZE]; //String used for the day 
	char tempString[BUFSIZE]; //String used for the temperature
	for(int i=0; i< sizeof(tempArray); i++){
		itoa(i, dayString, 10);
		dtostrf(tempArray[i], 7, 2, tempString);
	
		writeString(strcat("\r\nDay ", dayString));
		writeString(": ");
		writeString(tempString);
		
	}
}

void writeString(char * str){
	while(*str != 0x00){
		writeChar(*str);
		str++;
	}
}
void writeChar(unsigned char c){
	UDR0 = c;	// Display character on serial terminal (i.e PuTTy) 
	_delay_ms(10);
}

