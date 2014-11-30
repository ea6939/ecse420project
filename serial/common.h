#ifndef COMMON_H
#define COMMON_H

#include <stdlib.h>
#include <stdio.h>

typedef struct {
	int R;
	int G;
	int B;
} RGB;

RGB ** allocatePixelMatrix (int x, int y);


#endif