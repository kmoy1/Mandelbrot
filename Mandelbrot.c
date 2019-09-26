/*********************
**  Mandelbrot fractal
** clang -Xpreprocessor -fopenmp -lomp -o Mandelbrot Mandelbrot.c
** by Dan Garcia <ddgarcia@cs.berkeley.edu>
** Modified for this class by Justin Yokota and Chenyu Shi
**********************/

#include <stdio.h>
#include <stdlib.h>
#include "ComplexNumber.h"
#include "Mandelbrot.h"
#include <sys/types.h>

/*
This function returns the number of iterations before the initial point >= the threshold.
If the threshold is not exceeded after maxiters, the function returns 0.
*/
uint64_t MandelbrotIterations(u_int64_t maxiters, ComplexNumber * point, double threshold)//Z = 0, point = C
{
	u_int64_t i;
	u_int64_t numIters = 0;
	ComplexNumber* z = newComplexNumber(0.0,0.0); //z=0 initially.
	for(i=0;i<maxiters;i++){
		z = ComplexSum(ComplexProduct(z,z), point);
		numIters = numIters + 1; 
		if(ComplexAbs(z)>=threshold){
			return numIters;
		}
	}
	return 0;
}

/*
This function calculates the Mandelbrot plot and stores the result in output.
The number of pixels in the image is resolution * 2 + 1 in one row/column. It's a square image.
Scale is the the distance between center and the top pixel in one dimension.
*/
void Mandelbrot(double threshold, u_int64_t max_iterations, ComplexNumber* center, double scale, u_int64_t resolution, u_int64_t * output)
{
	 u_int64_t lw = 2 * resolution + 1; //length
	 double inc = scale/resolution; //2.5(i)
	 u_int64_t i;
	 u_int64_t x_coord, y_coord;
	 u_int64_t center_x = floor(pow(lw,2)/2);
	 u_int64_t center_y = floor(pow(lw,2)/2);
	 u_int64_t dx;
	 u_int64_t dy;
	 ComplexNumber* pt = newComplexNumber(0,0);

     for(i=0;i<pow(lw,2);i++){ //Construct corresponding point C per index.
     	x_coord = floor(i/lw);
     	y_coord = i % lw;
     	dx = x_coord - center_x;
     	dy = y_coord - center_y;
     	pt = newComplexNumber(Re(center)+inc*dx, Im(center)+inc*dy);
     	*(output+i) = MandelbrotIterations(max_iterations, pt, 2.0); 
     }
}
