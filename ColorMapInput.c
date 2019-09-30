/*********************
**  Color Map generator
** Skeleton by Justin Yokota
**********************/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <string.h>
#include "ColorMapInput.h"


/**************
**This function reads in a file name colorfile.
**It then uses the information in colorfile to create a color array, with each color represented by an int[3].
***************/
uint8_t** FileToColorMap(char* colorfile, int* colorcount)
{
	FILE* cfp = fopen(colorfile, "r");
	if(!cfp){
		perror("File Opening Error. Terminating.\n");
		free(cfp);
		return NULL;
	}
	int N = 0; //
	fscanf(cfp, "%d", &N); //stores length of color array in N. 
	printf("Array length N=%d\n", N); //sanity check
	uint8_t** c_arr = (uint8_t**) malloc(N * sizeof(uint8_t**));//malloc a length-N 2D array for colors. 
	for(int i=0; i<N; i++){
		c_arr[i] = (uint8_t *) malloc(3*sizeof(uint8_t)); //allocate space for each int as well
	}
	
	return c_arr;
}


