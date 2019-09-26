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
#include <math.h>

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
		ComplexNumber* prod = ComplexProduct(z,z);
		freeComplexNumber(z);
		z = ComplexSum(prod, point);
		numIters = numIters + 1; 
		if(ComplexAbs(z)>=threshold){
			freeComplexNumber(z);
			freeComplexNumber(prod);
			return numIters;
		}
		freeComplexNumber(prod);
	}
	freeComplexNumber(z);
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
	 u_int64_t center_x = floor(lw/2); //Save center coordinates: (2,2) in testASimple
	 u_int64_t center_y = floor(lw/2);
	 double center_Re = Re(center); //Save center point as a+bi
	 double center_Im = Im(center);
	 u_int64_t dx;
	 u_int64_t dy;

	 printf("INC: %d", inc);//Should be 2.5
     printf("CENTER IN A+BI: %d + %di\n", center_Re, center_Im); //SHOULD BE 5+3i

	 ComplexNumber* pt = newComplexNumber(0.0,0.0);
     for(i=0;i<pow(lw,2);i++){ //Construct corresponding point C per index.
     	x_coord = floor(i/lw); //Corresponding 2D coordinate from 1D index.
     	y_coord = i % lw;
     	dx = x_coord - center_x;//Displacement of point from center.
     	dy = y_coord - center_y;
     	freeComplexNumber(pt);
     	pt = newComplexNumber(center_Re+inc*dx,center_Im+inc*dy); //Create points in a+bi
     	printf("COORDINATE: %d %d\n", x_coord, y_coord); //POINTS in (x,y) format. 
     	printf("index: %d, value: %lf + %lf\n", i, center_Re+(inc*dy), center_Im+(inc*dx));
     	// printf("%d %d\n", center_Re, center_Im); //Board center,a+bi
     	printf("DISPLACEMENT FROM (2,2): %d %d\n", dx, dy); //Displacements from center.
     	printf("ADDED TO CENTER: %lf %lf \n", inc*dy, inc*dx);
     	// printf("%d %d\n", Re(pt), Im(pt)); //POINTS in a+bi format. 
     	*(output+i) = MandelbrotIterations(max_iterations, pt, 2.0); 
     }
     return;
}