/*********************
**  Mandelbrot fractal movie generator
** clang -Xpreprocessor -fopenmp -lomp -o Mandelbrot Mandelbrot.c
** by Dan Garcia <ddgarcia@cs.berkeley.edu>
** Modified for this class by Justin Yokota and Chenyu Shi
**********************/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include "ComplexNumber.h"
#include "Mandelbrot.h"
#include "ColorMapInput.h"
#include <sys/types.h>

void printUsage(char* argv[])
{
  printf("Usage: %s <threshold> <maxiterations> <center_real> <center_imaginary> <initialscale> <finalscale> <framecount> <resolution> <output_folder> <colorfile>\n", argv[0]);
  printf("This program simulates the Mandelbrot Fractal, and creates an iteration map of the given center, scale, and resolution, then saves it in output_file\n");
}
/*
This function calculates the threshold values of every spot on a sequence of frames. The center stays the same throughout the zoom. First frame is at initialscale, and last frame is at finalscale scale.
The remaining frames form a geometric sequence of scales, so 
if initialscale=1024, finalscale=1, framecount=11, then your frames will have scales of 1024, 512, 256, 128, 64, 32, 16, 8, 4, 2, 1.
As another example, if initialscale=10, finalscale=0.01, framecount=5, then your frames will have scale 10, 10 * (0.01/10)^(1/4), 10 * (0.01/10)^(2/4), 10 * (0.01/10)^(3/4), 0.01 .
*/
void MandelMovie(double threshold, u_int64_t max_iterations, ComplexNumber* center, double initialscale, double finalscale, int framecount, u_int64_t resolution, u_int64_t ** output){
    int multiplier;
    for(int i = 0; i<framecount;i++){
    	multiplier = finalscale/initialscale;
    	Mandelbrot(threshold, max_iterations, center, finalscale * pow(multiplier, i/(frame_count-1)), resolution, output[i]); 
    }
}

/*This function converts a color, represented by an integer array, to a string in binary format.*/
void convertToP6AndWrite(int* image, FILE* fp, int img_size, uint8_t** colormap){
	int threshold; //index to access colormap. 
	for(int i=0;i<img_size;i++){
		threshold=image[i];
		fwrite(colormap[threshold], 1, 3, fp); //write a threshold
	}
}

/**************
**This main function converts command line inputs into the format needed to run MandelMovie.
**It then uses the color array from FileToColorMap to create PPM images for each frame, and stores it in output_folder
***************/
int main(int argc, char* argv[])
{
	//Tips on how to get started on main function: 
	//MandelFrame also follows a similar sequence of steps; it may be useful to reference that.
	//Mayke you complete the steps below in order. 

	//STEP 1: Convert command line inputs to local variables, and ensure that inputs are valid.
	/*
	Check the spec for examples of invalid inputs.
	Remember to use your solution to B.1.1 to process colorfile.
	*/
	if (argc != 11){
		usage(argv);
		return 1;
	}
	int* color_count = malloc(sizeof(int*));
	double threshold = atof(argv[1]);
	u_int64_t maxiterations = atoi(argv[2]);
	double center_real = atof(argv[3]);
	double center_imaginary = atof(argv[4]);
	double initialscale = atof(argv[5]);
	double finalscale = atof(argv[6]);
	int framecount = atoi(argv[7]);
	u_int64_t resolution = atoi(argv[8]);

	if(threshold <= 0 || maxiterations <= 0 ||  initialscale <= 0 || finalscale <= 0){
		free(color_count);
		return 1;
	}
	if(framecount <= 0 || framecount > 10000){
		free(color_count);
		return 1;
	}
	if(framecount==1 && initialscale!=finalscale){
		free(color_count);
		return 1;
	}

	FILE* ofp;//file pointer to OUTPUTFILE, which we write to.
	uint8_t** colormap = FileToColorMap(argv[10], color_count); //Create colormap.
	int* color = (int*) malloc(3*sizeof(int));
	ComplexNumber* c_ptr = newComplexNumber(center_real, center_imaginary);	
	
	//STEP 2: Run MandelMovie on the correct arguments.
	/*
	MandelMovie requires an output array, so make sure you allocate the proper amount of space. 
	If allocation fails, free all the space you have already allocated (including colormap), then return with exit code 1.
	*/
	u_int64_t** output = (u_int64_t**) malloc(pow(lw, 2)*sizeof(u_int64_t*));
	if(output == NULL){
		fclose(ofp);
		free(colormap);
		free(color_count);
		return 1;
	}
	MandelMovie(threshold, maxiterations, c_ptr, initialscale, finalscale, framecount, resolution, output);

	//STEP 3: Output the results of MandelMovie to .ppm files.
	/*
	Convert from iteration count to colors, and output the results into output files.
	Use what we showed you in Part B.1.2, create a seqeunce of ppm files in the output folder.
	Feel free to create your own helper function to complete this step.
	As a reminder, we are using P6 format, not P3.
	*/

	//output[i] contains a single iteration image (which is another int array) at index i
	char* file = argv[9];
	char ppmPATH[strlen(file)+18]; //string for creating new file location.
	double x_coord, y_coord;
	int threshold;
	int* iterationImage;
	u_int64_t lw = 2 * resolution + 1; //length

	for(int i=0;i<framecount;i++){
     	// x_coord = floor(i/lw); //Corresponding 2D coordinate from 1D index. 
     	// y_coord = (double) (i % lw);
		sprintf(ppmPATH, "%s/frame%05d.ppm", file, i);
		ofp = fopen(ppmPATH, "w+");//Create new file.
		iterationImage = output[i]; //Contains Iteration image. Need to turn this into colors in p6. 
		convertToP6AndWrite(iterationImage, ofp, pow(lw,2), colormap);//Convert interation image into colors and write into file pointed at by ofp. 
	}

	//STEP 4: Free all allocated memory
	/*
	Make sure there's no memory leak.
	*/
	free(colormap);
	free(color_count);
	free(output);
	fclose(ofp);

	return 0;
}

