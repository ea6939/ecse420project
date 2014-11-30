#include <stdio.h>
#include <stdlib.h>
#include "common.h"

int m = 50; //Number of rows in the image
int n = 100; //Number of columns in the image
int b; //Size of the block
int step_size_initial = 16;

RGB **img_prev; //Previous image RGB
RGB **img_curr; //Current image RGB

void store_images();
void process_images();
int compute_sum_rgb();
RGB ** allocatePixelMatrix (int x, int y);
void output_image();

/*
BLOCKS:
0 | 1 | 2
3 | 4 | 5
6 | 7 | 8
*/

int main(int argc, char *argv[]) {
	srand(100);
	if(argc != 2) {
		printf("Usage: ./serialProgram <Block size>\n");
		exit(1);
	}

	b = atoi(argv[1]);

	store_images();

	process_images();

	output_image();
}

void store_images() {
	img_prev = allocatePixelMatrix(m,n);
	img_curr = allocatePixelMatrix(m,n);


	// FRANK'S TEST BELOW -- WILL EVENTUALLY BE DELETED
	int test = rand() % 20 + 30;
	//prev (33,63), curr (33,78)
	img_prev[test][test].R = 100;
	img_prev[test][test].G = 100;
	img_curr[test-5][test+5].R = 100;
	img_curr[test-5][test+5].G = 100;
	printf("TEST %d\n", test);
}

void process_images() {
	int i, j, k, l;

	printf("Computing the three step block matching algorithm...\n");

	//Property 1. Sum of RGB
	for(k = 0; k < m / b; k++) { // rows
		for(l = 0; l < n / b; l++) { // columns
			int step_size = step_size_initial;
			int curr_k, curr_l; //Temporary k and l variables to keep track of position

			curr_k = k * b;
			curr_l = l * b;

			printf("|");
			
			//If the previous image's block was empty
			if(compute_sum_rgb(curr_k, curr_l, 0) == -1) {
				printf("OoB");
				continue;
			}
			
			//If the previous image's block was empty
			if(compute_sum_rgb(curr_k, curr_l, 0) == 0) {
				printf("(0,0)");
				continue;
			}
			//If the block is the same (no movement detected)
			if(compute_sum_rgb(curr_k, curr_l, 0) - compute_sum_rgb(curr_k, curr_l, 1) == 0) {
				printf("(0,0)");
				continue;
			}

			int old_value = compute_sum_rgb(curr_k, curr_l, 0);
			while(step_size >= 1) {
				//Compare all 8 blocks around it
				int min_difference = -1;
				int block = 4;
				int new_value = compute_sum_rgb(curr_k, curr_l, 1);

				if(old_value - new_value == 0) {
					printf("(%d,%d)", curr_k - (k * b), curr_l - (l * b));
					break;
				}
				
				min_difference = (old_value - new_value)  * (old_value - new_value);

				for(i = 0; i < 3; i++) {
					for(j = 0; j < 3; j++) {
						new_value = compute_sum_rgb(curr_k + (i - 1) * step_size, curr_l + (j - 1) * step_size, 1);
						//printf("C%d %d %d\n", curr_k + (i - 1) * step_size, curr_l + (j - 1) * step_size, new_value);
						if(new_value == -1) {
							continue;
						}
						else {
							//A better match has been found
							if(min_difference == -1 || ((old_value - new_value) * (old_value - new_value) >= 0 && (old_value - new_value) * (old_value - new_value) < min_difference)) {
								min_difference = (old_value - new_value) * (old_value - new_value);
								block = i * 3 + j;
							}
						}
					}
				}
				if(block != 4) {
					//printf("BLOCK %d\n", block);
					curr_k += ((block / 3) - 1) * step_size;
					curr_l += ((block % 3) - 1) * step_size;
				}
				if(step_size == 1) {
					//If only black squares have been found
					//TODO: Maybe add a threshold in the future
					if(min_difference == old_value * old_value) {
						printf("(0,0)");
					}
					//Otherwise compute the vector
					else {
						printf("(%d,%d)", curr_k - (k * b), curr_l - (l * b));
					}
				}
				step_size /= 2;
			}
		}
		printf("|\n");
	}
}

//Input curr: 0 to calculate first image, 1 for second image
int compute_sum_rgb(int k, int l, int curr) {
	int i, j;
	int total_RGB = 0;

	//Block out of bound
	if(k < 0 || l < 0 || k + b > m || l + b > n) {
		return -1;
	}

	//Loops through the block and adds up the RGB values
	for(i = k; i < k + b; i++) {
		for(j = l; j < l + b; j++) {
			if(curr == 0) {
				total_RGB += img_prev[i][j].R;
				total_RGB += img_prev[i][j].G;
				total_RGB += img_prev[i][j].B;
			}
			else if(curr == 1) {
				total_RGB += img_curr[i][j].R;
				total_RGB += img_curr[i][j].G;
				total_RGB += img_curr[i][j].B;
			}
			else {
				printf("Error reading block due to the input matrix\n");
				printf("Exiting...\n");
				exit(1);
			}
		}
	}
	//printf("Block %d %d %d: %d\n", k, l, curr, total_RGB);
	return total_RGB;
}

void output_image() {
	FILE *f;
	unsigned char *img = NULL;
	int filesize = 54 + 3 * n * m;
	if(img)
	    free( img );

	img = (unsigned char *)malloc(3 * n * m);
	memset(img, 0, sizeof(img));

	int i, j;
	int x, y;
	int r, g, b;

	for(i = 0; i < n; i++) {
	    for(j = 0; j < m; j++) {
	    x = i; 
	    y = (m - 1) - j;
	    r = img_prev[i][j].R;
	    g = img_prev[i][j].G;
	    b = img_prev[i][j].B;
	    if (r > 255) r = 255;
	    if (g > 255) g = 255;
	    if (b > 255) b = 255;
	    img[(x + y * n) * 3 + 2] = (unsigned char)(r);
	    img[(x + y * n) * 3 + 1] = (unsigned char)(g);
	    img[(x + y * n) * 3 + 0] = (unsigned char)(b);
	}
	}

	unsigned char bmpfileheader[14] = {'B','M', 0,0,0,0, 0,0, 0,0, 54,0,0,0};
	unsigned char bmpinfoheader[40] = {40,0,0,0, 0,0,0,0, 0,0,0,0, 1,0, 24,0};
	unsigned char bmppad[3] = {0,0,0};

	bmpfileheader[2] = (unsigned char)(filesize    );
	bmpfileheader[3] = (unsigned char)(filesize>> 8);
	bmpfileheader[4] = (unsigned char)(filesize>>16);
	bmpfileheader[5] = (unsigned char)(filesize>>24);

	bmpinfoheader[4] = (unsigned char)(       n    );
	bmpinfoheader[5] = (unsigned char)(       n>> 8);
	bmpinfoheader[6] = (unsigned char)(       n>>16);
	bmpinfoheader[7] = (unsigned char)(       n>>24);
	bmpinfoheader[8] = (unsigned char)(       m    );
	bmpinfoheader[9] = (unsigned char)(       m>> 8);
	bmpinfoheader[10] = (unsigned char)(       m>>16);
	bmpinfoheader[11] = (unsigned char)(       m>>24);

	f = fopen("IsmgTest.bmp","wb");
	fwrite(bmpfileheader,1,14,f);
	fwrite(bmpinfoheader,1,40,f);
	for(i = 0; i < m; i++) {
	    fwrite(img + (n * (m - i - 1) * 3), 3, n, f);
	    fwrite(bmppad, 1, (4 - (n * 3) % 4) % 4, f);
	}
	fclose(f);
}
