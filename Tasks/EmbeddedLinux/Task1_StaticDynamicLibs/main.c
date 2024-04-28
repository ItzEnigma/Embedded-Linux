/************************************************************************************
 * Author:          Hosam Mohamed
 * -------------------------------
 *  FILE DESCRIPTION
 *  ----------------
 *
 *  File:   main.c
 *  Module: Calculator
 *
 *  Description: Contains application implementation
 ***********************************************************************************/

#include "stdio.h"
#include "calculator.h"

int main()
{
	int x = 50;
	int y = 5;

	printf("Addition Result: %ld \n", sum(x, y));
	printf("Subtraction Result: %ld \n", sub(x, y));
	printf("Multiplication Result: %ld \n", mult(x, y));
	printf("Division Result: %f \n", divide(x, y));
	printf("Modulus Result: %d \n", modulo(x, y));

	return 0;
}
