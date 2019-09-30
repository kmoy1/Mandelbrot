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
	int N = 0;
	int num_colors;
	int num_lines = 0;
	char input[256];
	char c;
	char* i1;
	uint8_t c1,c2,c3;//Color coords

	FILE* cfp = fopen(colorfile, "r");
	if(cfp == NULL){//file doesn't exist/empty
		printf("File Opening Error. Terminating.\n");
		free(cfp);
		return NULL;
	}

	for(c=getc(cfp); c!=EOF; c=getc(cfp)){
		if(c=='\n'){
			num_lines = num_lines+1;
		}
	}
	printf("Number of lines: %d\n", num_lines);
	num_colors = num_lines-1; //accomodates wrong NUMBER OF COLORS specified at beginning of colorfile.
	fseek(cfp,0,SEEK_SET);//reset cfp to beginning of file.
	fscanf(cfp, "%d", &N); //stores length of color array in N.
	// for(num_colors=0;num_colors<N;num_colors++){//save TRUE number of colors to num_colors.
	// 	fgets();
	// 	if(feof(cfp)){
	// 		break;
	// 	}
	// }
	*colorcount = num_colors;
	printf("Array length N=%d\n", N); //sanity check (N != num_colorss necessarily)
	uint8_t** c_arr = (uint8_t**) malloc(num_colors * sizeof(uint8_t**));//malloc a length-N 2D array for colors. 
	for(int i=0; i<num_colors; i++){
		c_arr[i] = (uint8_t *) malloc(3*sizeof(uint8_t)); //allocate space for each 3-int group also
		fscanf(cfp, "%d %d %d", &c1, &c2, &c3);//read a line.
		printf("Adding: [%d %d %d]\n", c1,c2,c3); //sanity check 2
		c_arr[i] = {c1,c2,c3}; //Try test with deref if fail.
	}
	fclose(cfp);
	return c_arr;
}
