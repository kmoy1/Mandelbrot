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
	if(!cfp){//file doesn't exist/empty
		perror("File Opening Error. Terminating.\n");
		free(cfp);
		return NULL;
	}
	int N = 0;
	int num_colors;
	int num_lines = 0;
	char input[256];
	char c;

	for(c=getc(cfp); c!=EOF; c=getc(cfp)){
		if(c=='\n'){
			num_lines = num_lines+1;
		}
	}
	printf("Number of lines: %d\n", num_lines);
	num_colors = num_lines-1;
	fseek(cfp,0,SEEK_SET);//reset cfp to beginning of file.
	fscanf(cfp, "%d", &N); //stores length of color array in N.
	// for(num_colors=0;num_colors<N;num_colors++){//save TRUE number of colors to num_colors.
	// 	fgets();
	// 	if(feof(cfp)){
	// 		break;
	// 	}
	// }
	*colorcount = num_colors;
	printf("Array length N=%d\n", N); //sanity check
	uint8_t** c_arr = (uint8_t**) malloc(N * sizeof(uint8_t**));//malloc a length-N 2D array for colors. 
	for(int i=0; i<N; i++){
		c_arr[i] = (uint8_t *) malloc(3*sizeof(uint8_t)); //allocate space for each 3-int group also
	}
	// for(int num_colors=0;num_colors<N;num_colors++){
	// 	fscanf()
	// }
	free(cfp);
	return c_arr;
}


