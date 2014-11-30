#include <stdio.h>
#include <stdlib.h>
#include "common.h"

int m; //Number of rows in the image
int n; //Number of columns in the image
int block_size; //Size of the block
int step_size_initial = 1;

RGB **img_prev; //Previous image RGB
RGB **img_curr; //Current image RGB

RGB** img_w_applied_mv;

vector **motion_vectors;

void store_images();
void process_images();
int compute_sum_rgb();
RGB** apply_motion_vectors(RGB** img_in, vector** motion, int width, int height);

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
	int width, height;
	int i;
	
	RGB** img;
	printf("prev img:\n");
	img_prev = import_image_from_text("./test");
	
	printf("curr img:\n");
	img_curr = import_image_from_text("./test2");	
	//export_image_to_text(img,"./test_out");
	
	block_size = atoi(argv[1]);
	m = get_height();
	n = get_width();
	
	motion_vectors = allocate_motion_vector_matrix(m,n,block_size);
	
	

	process_images();
	
	// in production currently
	img_w_applied_mv = apply_motion_vectors(img_prev, motion_vectors, n, m);
	export_image_to_text(img_w_applied_mv,"./test_out");
	
}

void process_images() {
	int i, j, k, l;

	printf("Computing the three step block matching algorithm...\n");

	//Property 1. Sum of RGB
	for(k = 0; k < m / block_size; k++) { // rows
		for(l = 0; l < n / block_size; l++) { // columns
			
			int step_size = step_size_initial;
			int curr_k, curr_l; //Temporary k and l variables to keep track of position

			curr_k = k * block_size;
			curr_l = l * block_size;

			//printf("|");
			
			//If the previous image's block was empty
			if(compute_sum_rgb(curr_k, curr_l, 0) == -1) {
				printf("OoB");
				continue;
			}
			//If the previous image's block was empty
			if(compute_sum_rgb(curr_k, curr_l, 0) == 0) {
				motion_vectors[k][l].x = 0;
				motion_vectors[k][l].y = 0;
				//printf("(0,0)");
				continue;
			}
			//If the block is the same (no movement detected)
			if(compute_sum_rgb(curr_k, curr_l, 0) - compute_sum_rgb(curr_k, curr_l, 1) == 0) {
				motion_vectors[k][l].x = 0;
				motion_vectors[k][l].y = 0;
				//printf("(0,0)");
				continue;
			}
			int old_value = compute_sum_rgb(curr_k, curr_l, 0);
			while(step_size >= 1) {
				//Compare all 8 blocks around it
				int min_difference = -1;
				int block = 4;
				int new_value = compute_sum_rgb(curr_k, curr_l, 1);

				if(old_value - new_value == 0) {
					motion_vectors[k][l].y = curr_k - (k * block_size);
					motion_vectors[k][l].x = curr_l - (l * block_size);
					//printf("(%d,%d)", curr_k - (k * block_size), curr_l - (l * block_size));
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
						
						motion_vectors[k][l].x = 0;
						motion_vectors[k][l].y = 0;
						//printf("(0,0)");
					}
					//Otherwise compute the vector
					else {
						motion_vectors[k][l].y = curr_k - (k * block_size);
						motion_vectors[k][l].x = curr_l - (l * block_size);
						//printf("(%d,%d)", curr_k - (k * block_size), curr_l - (l * block_size));
					}
				}
				step_size /= 2;
			}
		}
		//printf("|\n");
	}
	
	printf("\nmotion vectors: (x,y)\n");
	for(k = 0; k < m / block_size; k++) { // rows
		for(l = 0; l < n / block_size; l++) { // columns
			printf("(%d,%d) ",motion_vectors[k][l].x,motion_vectors[k][l].y);
		}
		printf("\n");
	}
}

//Input curr: 0 to calculate first image, 1 for second image
int compute_sum_rgb(int k, int l, int curr) {
	int i, j;
	int total_RGB = 0;

	//Block out of bound
	if(k < 0 || l < 0 || k + block_size > m || l + block_size > n) {
		return -1;
	}

	//Loops through the block and adds up the RGB values
	for(i = k; i < k + block_size; i++) {
		for(j = l; j < l + block_size; j++) {
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

RGB** apply_motion_vectors(RGB** img_in, vector** motion, int width, int height){
	int i, k, l;
	int k_curr, l_curr;
	
	RGB** img_out;
	img_out = (RGB**)malloc(height * sizeof(RGB*));
	
	for (i=0; i<height; i++)
    		img_out[i] = (RGB *) malloc(width * sizeof(RGB));
    
    for(k = 0; k < height / block_size; k++) { // rows
		for(l = 0; l < width / block_size; l++) { // columns
			for(k_curr=k*block_size; k_curr<k*block_size+block_size; k_curr++){ // fill the block with black
				for(l_curr=l*block_size; l_curr<l*block_size+block_size; l_curr++){
					// if the motion vector is not (0,0) i.e the block has moved
					// then fill that block with black
					if(motion[k][l].x != 0 || motion[k][l].y != 0){
						img_out[k_curr][l_curr].R = 0;
						img_out[k_curr][l_curr].G = 0;
						img_out[k_curr][l_curr].B = 0;
					}
					// else if the block has not moved, copy its contents
					else{
						img_out[k_curr][l_curr].R = img_in[k_curr][l_curr].R;
						img_out[k_curr][l_curr].G = img_in[k_curr][l_curr].G;
						img_out[k_curr][l_curr].B = img_in[k_curr][l_curr].B;
					}
				}
			}
		}	
	}
	
	
	
	return img_out;
}
