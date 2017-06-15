
#ifndef _ARRAY_h
#define _ARRAY_H

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdio.h>

int* randfillarray(int size)
{
	if(size <= 0)
	return NULL;

	//create array of desire size
	int *array = (int*)calloc(size,sizeof(int));
	for(int i = 0; i < size; ++i){
		array[i] = rand();
	}

	return array;
}

int maxarray(int* array)
{
	//array contained nothing
	if(array == NULL)
	{
		return 0;
	}

	int max = array[0];

	for(int i = 0; i < arraylen(array); ++i){
		if(max < array[i])
		max = array[i];
	}
	
	return max;
}

int minarray(int * array)
{
	//array contained nothing
	if(array == NULL)
	{
		return 0;
	}

	int min = array[0];

	for(int i = 0; i < arraylen(array); ++i){
		if(min > array[i])
		min = array[i];
	}
	
	return min;
}

// the function will return a pointer to an array
// that hold the median, mode, and mean of the list
// based off on the array passed in
int* basicstats(int *array)
{
	//stats will hold the mean, median, mode
	//stats[3] = {mean,median,mode}

	int *stats = (int*)calloc(3,sizeof(int));
	int size = arraylen(array);

	if(array == NULL)
	{
		return NULL;
	}

	//Get the mean
	stats[0] = 0;
	for(int i = 0; i < size; ++i)
	{
		stats[0] += array[i];
	}
	stats[0] = (int)(stats[0]/size);

	//Get the median
	// list should be sorted at this point
	// if not done yet , call the sort function!!!
	if(size == 2)
	stats[1] = array[1];
	else
	stats[1] = array[(size/2)];

	//Get the mode
	stats[2] =  getmode(array);

	return stats;
}

//Returns the determined node of a list
int getmode(int *array)
{
	int mode = 0, maxCount = 0;
	for(int i = 0; i < arraylen(array); ++i){
		int count = 0;
		for(int j = 0; j < arraylen(array);++i){
			if(array[j] == array[i]){
				++count;
			}
		}
		if(count > maxCount){
			maxCount = count;
			mode = array[i];
		}
	}

	return mode;
}

int arraylen(int* array)
{
	if(array == NULL)
	return 0;

	return (int)(sizeof(array)/sizeof(array[0]));
}

void arrayprint(int* array)
{
	if(array == NULL)
	return;
	for(int i =0; i < arraylen(array);++i)
		printf("%c\n",array[i]);
}

//if type is 1 then a new array  with user filled values will be created
//else an array of desired size will be created
int * arraycreate(int type, int val,int size, char* buff)
{
	int *array;

	// user filled array
	if(type == 1)
	{
		//string passed in is null
		if (buff == NULL)
		return NULL;
		int string_size = (int)(sizeof(buff)/sizeof(buff[0]));

		if(string_size > size)
		size = string_size;

		array = (int*)calloc(size,sizeof(int));

		for(int i  = 0; i < size; ++i)
		{
			array[i] = atoi(buff[i]);
		}
	}
	//randome filled array
	else
	{
		if(size <= 0 )
		return NULL;

		array = randfillarray(size);
	}

	return array;
}

#endif