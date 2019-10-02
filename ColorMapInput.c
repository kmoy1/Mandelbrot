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
	uint8_t* carr; //holds c1,c2,c3 above in array.
	FILE* cfp = fopen(colorfile, "r");
	if(cfp == NULL){//file doesn't exist/empty
		printf("File Opening Error. Terminating.\n");
		fclose(cfp);
		return NULL;
	}

	for(c=getc(cfp); c!=EOF; c=getc(cfp)){
		if(c=='\n'){
			num_lines = num_lines+1;
		}
	}
	num_lines = num_lines+1; //one more for final line, which doesn't have a '\n'
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
	printf("Array length N=%d\n", N); //sanity check (N != num_colorss necessarily)
	if(num_lines-1 < N){
		printf("Number of lines too big: Terminating.\n");
		fclose(cfp);
		return NULL;
	}
	if(N < 0){
		printf("N cannot be negative.\n");
		fclose(cfp);
		return NULL;
	}
	*colorcount = N; //N passes the validity tests. WE'll have N colors.
	uint8_t** c_arr = (uint8_t**) malloc(N * sizeof(uint8_t*));//malloc a length-N 2D array for colors. 
	for(int i=0; i<N; i++){
		c_arr[i] = (uint8_t *) malloc(3*sizeof(uint8_t)); //allocate space for each 3-int group also
		fscanf(cfp, "%d %d %d", &c1, &c2, &c3);//read a line.
		// printf("Adding: [%d %d %d]\n", c1,c2,c3); //sanity check 2
		c_arr[i][0]=c1;
		c_arr[i][1]=c2;
		c_arr[i][2]=c3;
	}
	fclose(cfp);
	return c_arr;
}
