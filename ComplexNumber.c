/*********************
**  Complex Numbers
**  This file contains a few functions that will be useful when performing computations with complex numbers
**  It is advised that you work on this part first.
**********************/

#include "ComplexNumber.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


typedef struct ComplexNumber
{
	double real;
	double imaginary;
}ComplexNumber;

//Returns a pointer to a new Complex Number with the given real and imaginary components
ComplexNumber* newComplexNumber(double real_component, double imaginary_component)
{
	ComplexNumber* c_ptr = (ComplexNumber* ) malloc(sizeof(ComplexNumber));
	c_ptr->real = real_component;
	c_ptr->imaginary = imaginary_component;
	return c_ptr;
}

//Returns a pointer to a new Complex Number equal to a*b
ComplexNumber* ComplexProduct(ComplexNumber* a, ComplexNumber* b)
{
    ComplexNumber* c_prod = (ComplexNumber* ) malloc(sizeof(ComplexNumber));
	double prod_real = (a->real * b->real) - (a->imaginary * b->imaginary);
	double prod_im = (a->real * b->imaginary) + (a->imaginary*b->real);
	c_prod->real = prod_real;
	c_prod->imaginary = prod_im;
	return c_prod;
}

//Returns a pointer to a new Complex Number equal to a+b
ComplexNumber* ComplexSum(ComplexNumber* a, ComplexNumber* b)
{
    ComplexNumber* c_sum = (ComplexNumber* ) malloc(sizeof(ComplexNumber));
	double sum_real = a->real + b->real;
	double sum_im = a->imaginary + b->imaginary;
	c_sum->real = sum_real;
	c_sum->imaginary = sum_im;
	return c_sum;
}	
//Returns the absolute value of Complex Number a
double ComplexAbs(ComplexNumber* a)
{
	return sqrt(pow(a->real, 2) + pow(a->imaginary, 2));
}
//Frees complex number
void freeComplexNumber(ComplexNumber* a)
{
	free(a);
	return;
}
//Gets real cmpt of complex number
double Re(ComplexNumber* a)
{
	return a->real;
}
//gets imaginary cmpt of complex number
double Im(ComplexNumber* a)
{
	return a->imaginary;
}


//Contains a few tests.
int test_complex_number()
{
	ComplexNumber* a = newComplexNumber(2.0, 1.0);
	if (a == NULL)
	{
		printf("Creation of complex numbers not implemented\n");
		return 0;
	}
	ComplexNumber* b = ComplexProduct(a, a);
	if (b == NULL)
	{
		printf("Multiplication of complex numbers not implemented\n");
		free(a);
		return 0;
	}
	ComplexNumber* c = ComplexSum(b, b);
	if (c == NULL)
	{
		printf("Addition of complex numbers not implemented\n");
		free(a);
		free(b);
		return 0;
	}
	float d = ComplexAbs(c);
	if (d == 0)
	{
		printf("Absolute Value of complex numbers not implemented\n");
		free(a);
		free(b);
		free(c);
		return 0;
	}
	else if (fabsf(d - 10) < 0.0001)
	{
		printf("Sample tests for complex numbers all passed\n");
	}
	else
	{
		printf("At least one of your functions is incorrect\n");
	}
	free(a);
	free(b);
	free(c);
	return 0;
}
