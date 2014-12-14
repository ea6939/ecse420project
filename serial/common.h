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
	int x;
	int y;
} vector;

void import_image_from_text(RGB** img, char* img_path);
void export_image_to_text(RGB** img, char* img_path);

#endif
