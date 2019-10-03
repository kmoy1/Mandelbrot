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
#include <string.h>
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
	printf("Here in MandelMovie.\n"); 
    double multiplier;
    for(int i = 0; i<framecount;i++){
    	multiplier = finalscale/initialscale;
    	// printf("At index %d: Scale = %lf\n", i, initialscale * pow(multiplier, (double)i/(framecount-1)));
    	Mandelbrot(threshold, max_iterations, center, initialscale * pow(multiplier, (double)i/(framecount-1)), resolution, *(output+i)); 
    }
}


/*For sz iterations in the image, with each image converted to a color, which is then written to fp*/
void convertToColorAndWrite(u_int64_t* image, uint8_t** colormap, FILE* fp, u_int64_t sz, int num_colors){
	printf("WE IN CONVERSION.\n");
	u_int64_t pixel;
	int sqr_dims = sqrt((double)(sz));
	int zeros[3] = {0,0,0};
	printf("SIZE SZ: %lu\n", sz);
	printf("NUM_COLORS: %d\n", num_colors);
	fprintf(fp, "P6 %d %d %d\n", sqr_dims, sqr_dims, 255);//print header.
	for(int i=0;i<sz;i++){
		pixel = *(image+i);
		printf("Pixel:  %lu\n", pixel);
		if(pixel == 0){
			fwrite(zeros,1,3,fp);
		}
		else if(pixel>num_colors){
			pixel = pixel % num_colors;
			// printf("Current ASCII: %lu %lu %lu \n", colormap[pixel][0], colormap[pixel][1],colormap[pixel][2]);
			fwrite(colormap[pixel-1],1,3,fp);
		}
		else{
			pixel = *(image+i);
			// printf("Current ASCII: %lu %lu %lu \n", colormap[pixel][0], colormap[pixel][1],colormap[pixel][2]);
			fwrite(colormap[pixel-1],1,3,fp);
		}
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
		printf("WRONG NUMBER OF ARGS!\n");	
		printUsage(argv);
		return 1;
	}
	int* color_count = malloc(sizeof(int));
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
	ComplexNumber* c_ptr = newComplexNumber(center_real, center_imaginary);	//Create our center.
	u_int64_t size = pow(2 * resolution + 1, 2); //length

	//STEP 2: Run MandelMovie on the correct arguments.
	/*
	MandelMovie requires an output array, so make sure you allocate the proper amount of space. 
	If allocation fails, free all the space you have already allocated (including colormap), then return with exit code 1.
	*/

	//Allocate a framecountx ((2*resolution+1)^2) 2D array.
	u_int64_t** output = (u_int64_t**) malloc(framecount*sizeof(u_int64_t*));
	for(int a=0;a<framecount;a++){
		*(output+a) = (u_int64_t *) malloc(size*sizeof(u_int64_t));
	}

	if(output == NULL){
		fclose(ofp);
		free(colormap);
		free(color_count);
		return 1;
	}

	MandelMovie(threshold, maxiterations, c_ptr, initialscale, finalscale, framecount, resolution, output);
	printf("Check1\n");
	//STEP 3: Output the results of MandelMovie to .ppm files.
	/*
	Convert from iteration count to colors, and output the results into output files.
	Use what we showed you in Part B.1.2, create a seqeunce of ppm files in the output folder.
	Feel free to create your own helper function to complete this step.
	As a reminder, we are using P6 format, not P3.
	*/
	//output[i] contains a single iteration image (which is another int array) at index i
	char* file = argv[9];
	// double x_coord, y_coord;
	u_int64_t* iterationImage = (u_int64_t*) malloc(size*sizeof(u_int64_t));
	char* frameEnd = "/frame00000.ppm";

	for(int i=0;i<framecount;i++){
     	// x_coord = floor(i/lw); //Corresponding 2D coordinate from 1D index. 
     	// y_coord = (double) (i % lw);
     	char ppmPATH[strlen(file)+strlen(frameEnd)]; //string for creating new file location.
		sprintf(ppmPATH, "%s/frame%05d.ppm", file, i);
		printf("Adding file %s\n", ppmPATH);
		ofp = fopen(ppmPATH, "w");//Create new file.
		if(ofp==NULL){
			printf("fopen failed\n");
			return 1;
		}
		free(iterationImage);
		iterationImage = output[i]; //Contains Iteration image. Need to turn this into colors in p6. 
		printf("Check2\n");
		convertToColorAndWrite(iterationImage, colormap, ofp, size, *color_count); //Convert image to color and output.
		fclose(ofp);
		printf("To iteration %d\n", i+1);
	}
	//STEP 4: Free all allocated memory
	/*
	Make sure there's no memory leak.
	*/
	free(colormap);
	free(color_count);
	free(output);
	free(iterationImage);

	return 0;
}

