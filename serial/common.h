#ifndef COMMON_H
#define COMMON_H

#include <stdlib.h>
#include <stdio.h>

typedef struct {
	int R;
	int G;
	int B;
} RGB;

typedef struct {
	unsigned char R;
	unsigned char G;
	unsigned char B;
} char_RGB;

typedef struct {
   unsigned int width;
   unsigned int height;
   unsigned int planes;
   unsigned short bitcount;
   unsigned int size;
} BITMAPINFOHEADER;

typedef struct {
	int x;
	int y;
} vector;

RGB** allocatePixelMatrix (int x, int y);
RGB** import_image_from_text(char* img_path);
void export_image_to_text(RGB** img, char* img_path);
BITMAPINFOHEADER getImageData(FILE *image);


#endif
