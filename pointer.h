#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include <stdlib.h>
#include <math.h>

#define ARR_SIZE 10

void swap(int *p1,int *p2)
{
	int temp;
	temp = *p1;
	*p1 = *p2;
	*p2 = temp;
}


int length(int* arr){
	return (int)sizeof(arr)/sizeof(arr[0]);
}

void printArr(int* arr){
	for(int i = 0; i < length(arr);++i){
		printf("%d\n", *(arr+i));
	}
}

void print_addr(int *x){
	printf("%d\n", x);
}

void print_array(int *arr){
	for(int i = 0; i < length(arr);++i){
		printf("%d\n", arr[i]);
	}
}

int* createArray(){

	int *array; 
	array = (int*)calloc(ARR_SIZE,sizeof(int));

	for(int i = 0; i < length(array);++i)
		array[i] = i;
	print_array(array);
	return array;
}