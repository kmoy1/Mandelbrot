/*********************
**  Color Palette generator
** Skeleton by Justin Yokota
**********************/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <string.h>
#include "ColorMapInput.h"

//You don't need to call this function but it helps you understand how the arguments are passed in 
void usage(char* argv[])
{
	printf("Incorrect usage: Expected arguments are %s <inputfile> <outputfolder> <width> <heightpercolor>", argv[0]);
}

//Creates a color palette image for the given colorfile in outputfile. Width and heightpercolor dictates the dimensions of each color. Output should be in P3 format
int P3colorpalette(char* colorfile, int width, int heightpercolor, char* outputfile)
{
	int i,j,k;
	int* num_cls = malloc(sizeof(int*)); //Number of colors
	// char* color;  //Row of colors in string form.
	uint8_t** color_arr = FileToColorMap(colorfile, num_cls);
	FILE* fpi = fopen(colorfile, "r");
	FILE* fpo = fopen(outputfile, "w+");
	if(fpi == NULL || width < 1 || heightpercolor < 1){
		fclose(fpi); //same as freeing.
		fclose(fpo);
		return 1;
	}
	fprintf(fpo, "P3 %d %d %d\n", width, heightpercolor * *(num_cls), 255);//test header
	printf("P3 %d %d %d\n", width, heightpercolor * *(num_cls), 255);//test header


	for(i=0; i< *num_cls; i++){//PRINTS ALL COLORS.	
		for(j=0;j<heightpercolor;j++){//PRINTS heightpercolor ROWS OF COLORS.
			for(k=0; k<width;k++){//PRINT SINGLE COLOR, WIDTH TIMES.
				if(k==width-1){
					fprintf(fpo, "%d %d %d", color_arr[i][0], color_arr[i][1], color_arr[i][2]);//no whitespace end of LINE.
				}
				else{
					fprintf(fpo, "%d %d %d ", color_arr[i][0], color_arr[i][1], color_arr[i][2]); //SINGLE COLOR,
				}
			}
			fprintf(fpo, "\n");
		}
	}
	fclose(fpi);
	fclose(fpo);
	free(num_cls);
	free(color_arr);
	return 0;
}

//Same as above, but with P6 format
int P6colorpalette(char* colorfile, int width, int heightpercolor, char* outputfile)
{
	int i,j,k;
	int* num_cls = malloc(sizeof(int*)); //Number of colors
	// char* color;  //Row of colors in string form.
	uint8_t** color_arr = FileToColorMap(colorfile, num_cls);
	FILE* fpi = fopen(colorfile, "r");
	FILE* fpo = fopen(outputfile, "w+");
	if(fpi == NULL || width < 1 || heightpercolor < 1){
		fclose(fpi); //same as freeing.
		fclose(fpo);
		return 1;
	}
	fprintf(fpo, "P6 %d %d %d\n", width, heightpercolor * *(num_cls), 255);//test header
	printf("P6 %d %d %d\n", width, heightpercolor * *(num_cls), 255);//test header


	for(i=0; i< *num_cls; i++){//PRINTS ALL COLORS.	
		for(j=0;j<heightpercolor;j++){//PRINTS heightpercolor ROWS OF COLORS.
			for(k=0; k<width;k++){//PRINT SINGLE COLOR, WIDTH TIMES.
				fwrite(color_arr[i], 1, 3, fpo);
			}
		}
	}
	fclose(fpi);
	fclose(fpo);	
	return 0;
}


//The one piece of c code you don't have to read or understand. Still, might as well read it, if you have time.
int main(int argc, char* argv[])
{
	if (argc != 5)
	{
		usage(argv);
		return 1;
	}
	int width = atoi(argv[3]);
	int height = atoi(argv[4]);
	char* P3end = "/colorpaletteP3.ppm";
	char* P6end = "/colorpaletteP6.ppm";
	char buffer[strlen(argv[2]) + strlen(P3end)];
	sprintf(buffer, "%s%s", argv[2], P3end);
	int failed = P3colorpalette(argv[1], width, height, buffer);//passes in total vertical height???
	if (failed)
	{
		printf("Error in making P3colorpalette");
		return 1;
	}
	sprintf(buffer, "%s%s", argv[2], P6end);
	failed = P6colorpalette(argv[1], width, height, buffer);
	if (failed)
	{
		printf("Error in making P6colorpalette");
		return 1;
	}
	printf("P3colorpalette and P6colorpalette ran without error, output should be stored in %s%s, %s%s", argv[2], P3end, argv[2], P6end);
	return 0;
}




