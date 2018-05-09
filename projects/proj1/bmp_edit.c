// Chris Mananghaya (cam314)
// CS 0449: Project 1 (part 2)

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#pragma pack(1) // eliminates padding

/*
Objective:
You will make a utility that can transform a 24-bit RGB BMP file with one of two operations: "invert" or "grayscale".
Make it so that it runs with the following command line options: 
$ ./bmp_edit --invert FILENAME, $ ./bmp_edit --grayscale FILENAME
This will destructively alter the BMP file named FILENAME to contain the inverse pixel colors of the original colors. 
When I say "destructively alter," I mean the file is changed in place - no new file is created.
The second command will destructively alter the BMP file named FILENAME to contain the pixel colors converted to grayscale 
as described below.
*/

struct BMPHeader 
{
	char file_type[2];	// format identifier (magic number); offset = 0
	int file_size;		// size of the file in bytes; offset = 2
	short reserved1;	// two-byte reserved value ("reserved" = "not used"); offset = 6
	short reserved2;	// second two-byte reserved value; offset = 8
	int offset; 		// offset from beginning of the file to start of the pixel array; offset = 10
} BMPHeader;

struct DIBHeader
{
	int header_size;	// Size of this DIB header in bytes; offset = 0
	int width;			// Width of the image in pixels; offset = 4
	int height;			// Height of the image in pixels; offset = 8
	short color_panes;	// Number of color planes (don’t worry); offset = 12
	short bits;			// Number of bits per pixel; offset = 14
	int scheme;			// Compression scheme used; offset = 16
	int image_size;		// Image size in bytes; offset = 20
	int horizontal;		// Horizontal resolution; offset = 24
	int vertical;		// Vertical resolution; offset = 28
	int palette;		// Number of colors in the palette; offset = 32
	int import_colors;	// Number of important colors; offset = 36
} DIBHeader;

struct Pixel
{
	unsigned char blue;	// Blue intensity; offset 0
	unsigned char green;// Green intensity; offset 1
	unsigned char red; 	// Red intensity; offset 2
};

int read_BMP(FILE* file)
{
	fread(&BMPHeader, 14, 1, file);
	fread(&DIBHeader, 40, 1, file);

	if((BMPHeader.file_type[0] != 'B' && BMPHeader.file_type[1] != 'M') || DIBHeader.header_size != 40 || DIBHeader.bits != 24)
	{
		if(BMPHeader.file_type[0] != 'B' && BMPHeader.file_type[1] != 'M')
			printf("Issue: the format identifier is not BM.\n");
		if(DIBHeader.header_size != 40)
			printf("Issue: DIB size is not 40.\n");
		if(DIBHeader.bits != 24)
			printf("Issue: 'There are %d bits per pixel' field, instead of 24.\n", DIBHeader.bits);
		printf("Error: file format not supported.\n");
		return 1;
	}
	return 0;
}

int main(int argc, char* argv[]) 
{
	FILE* bmp = fopen(argv[2], "rb+");
	int i;
	int j;

	if(bmp == 0)
	{
		printf("Error: image is a null pointer.\n");
		return 1;
	}

	read_BMP(bmp);

	fseek(bmp, BMPHeader.offset, SEEK_SET);
	int extra_padding = 4 - (DIBHeader.width*3 % 4);
	
	if(extra_padding == 4)
	{
		extra_padding = 0;
	}	

	if(strcmp(argv[1], "--invert") == 0)
	{
		printf("Inverting...\n");
	} 
	else if(strcmp(argv[1], "--grayscale") == 0)
	{
		printf("Converting to grayscale...\n");
	} 
	else
	{
		printf("The first command-line argument was %s. This is invalid. Only accepted transformations are -invert and -grayscale. Exiting...\n", argv[1]);
		return 1;
	}

	for(i = 0; i < DIBHeader.height; i++)
	{
		for(j = 0; j < DIBHeader.width; j++)
		{
			struct Pixel pix;
			fread(&pix, 3, 1, bmp);
			
			if(strcmp("--invert", argv[1]) == 0) //Invert the pixel
			{
				pix.blue = ~pix.blue;
				pix.green = ~pix.green;
				pix.red = ~pix.red;
			} 
			else if(strcmp("--grayscale", argv[1]) == 0) //Grayscale the pixel
			{
				double greyscale_blue = ((double) pix.blue)/255.0;
				double greyscale_green = ((double) pix.green)/255.0;
				double greyscale_red = ((double) pix.red)/255.0;
				double grey_value = .2126*greyscale_red + .7152*greyscale_green + .0722*greyscale_blue;
				
				if(grey_value <= .0031308)
				{
					greyscale_blue = 12.92*grey_value;
					greyscale_green = 12.92*grey_value;
					greyscale_red = 12.92*grey_value;
				} 
				else
				{
					greyscale_blue = 1.055*pow(grey_value, 1.0/2.4) - .055;
					greyscale_green = 1.055*pow(grey_value, 1.0/2.4) - .055;
					greyscale_red = 1.055*pow(grey_value, 1.0/2.4) - .055;
				}

				pix.blue = (unsigned char) (greyscale_blue*255.0);
				pix.green = (unsigned char) (greyscale_green*255.0);
				pix.red = (unsigned char) (greyscale_red*255.0);
			}
			
			fseek(bmp, -3, SEEK_CUR); 
			fwrite(&pix, 3, 1, bmp); //Write the pixel back to its original image
		}
		
		//Make sure the row of pixels is a multiple of 4 bytes, so add some padding if that's not the case.
		fseek(bmp, extra_padding, SEEK_CUR);
	}
	printf("Finished!\n");

	return 1;
}