/*
 * ECSE420 Project Motion Estimation
 * Group 5
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include "common.h"

int m; //Number of rows in the image
int n; //Number of columns in the image
int block_size; //Size of the block
int step_size_initial = 25;
int threadID;

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
		printf("Usage: ./parProgram <Block size>\n");
		exit(1);
	}
	int i,j;
	
        clock_t begin, end;
        double time_spent;

        begin = clock();
        
	// get width and height from file
	FILE *image;
	image=fopen("./1.txt","rb+");
	if(image==NULL)
	{
		printf("Error! Image failed to open.");
	}
	
	char line[80];
	fgets(line, 80, image);
	sscanf (line, "%d", &n); // width
	
	fgets(line, 80, image);
	sscanf (line, "%d", &m); // height
	
	
	img_prev = (RGB**)malloc(m * sizeof(RGB*));
	for (i=0; i<m; i++)
    		img_prev[i] = (RGB *) malloc(n * sizeof(RGB));
    
    img_curr = (RGB**)malloc(m * sizeof(RGB*));
	for (i=0; i<m; i++)
    		img_curr[i] = (RGB *) malloc(n * sizeof(RGB));
    
	//printf("prev img:\n");
	import_image_from_text(img_prev, "./1.txt");
	
	//printf("curr img:\n");
	import_image_from_text(img_curr, "./2.txt");
	
	block_size = atoi(argv[1]);
	
	motion_vectors = (vector **)malloc((m/block_size) * sizeof(vector*)); // THIS MALLOC MESSES EVERYTHING UP
	for (i=0; i<m; i++)
		motion_vectors[i] = (vector *) malloc((n/block_size) * sizeof(vector));
	
	process_images();
	
	int k,l;
	
	/*
	printf("\nmotion vectors: (x,y)\n");
	for(k = 0; k < m / block_size; k++) { // rows
		for(l = 0; l < n / block_size; l++) { // columns
			printf("(%d,%d) ",motion_vectors[k][l].x,motion_vectors[k][l].y);
		}
		printf("\n");
	}
	*/
	
	// in production currently
	img_w_applied_mv = apply_motion_vectors(img_prev, motion_vectors, n, m);
	/*
	printf("\nimage w applied motion vectors:\n");
	for(k = 0; k < m; k++) { // rows
		for(l = 0; l < n; l++) { // columns
			printf("%d,%d,%d | ",img_w_applied_mv[k][l].R,img_w_applied_mv[k][l].G,img_w_applied_mv[k][l].B);
		}
		printf("\n");
	}
	*/
	
	export_image_to_text(img_w_applied_mv,"./test_out");
        
        end = clock();
        time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
        printf("%f", time_spent);
	
}

void process_images() {
	int i, j, k, l;

	printf("Computing the three step block matching algorithm...\n");

        
	//Property 1. Sum of RGB
	// loop through all the blocks
        #pragma omp parallel num_threads(2)
	for(k = 0; k < m / block_size; k++) { // rows
	 	for(l = 0; l < n / block_size; l++) { // columns
			//if(k==0) printf("%d, l: %d\n",img_prev[0][l*block_size].R,l);
			//printf("for loop k: %d\n",k);
			//printf("for loop l: %d\n\n",l);
			int step_size = step_size_initial;
			int curr_k, curr_l; //Temporary k and l variables to keep track of position

			curr_k = k * block_size;
			curr_l = l * block_size;
			
			//printf("|");
			
			// if out of bounds
			if(compute_sum_rgb(curr_k, curr_l, 0) == -1) {
				printf("OoB");
				continue;
			}
			//If the previous image's block was empty
			/*
			if(compute_sum_rgb(curr_k, curr_l, 0) == 0) {
				motion_vectors[k][l].x = 0;
				motion_vectors[k][l].y = 0;
				//printf("(0,0)");
				continue;
			}*/
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
				
				// if the block is the same in both images, keep it there
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
						
						//printf("curr_k: %d, minus: %d\n", curr_k, (k * block_size));
						//printf("curr_l: %d, minus: %d\n\n", curr_l, (l * block_size));
						//printf("(%d,%d)", curr_k - (k * block_size), curr_l - (l * block_size));
					}
				}
				step_size /= 2;
			}
		}
		//printf("|\n");
		
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

RGB compute_avg_rgb(int k, int l, int curr) {
	int i, j;
	int total_R = 0;
	int total_G = 0;
	int total_B = 0;
	RGB avg;

	//Block out of bound
	if(k < 0 || l < 0 || k + block_size > m || l + block_size > n) {
		avg.R = -1;
		avg.G = -1;
		avg.B = -1;
		return avg;
	}

	//Loops through the block and adds up the RGB values
	for(i = k; i < k + block_size; i++) {
		for(j = l; j < l + block_size; j++) {
			if(curr == 0) {
				total_R += img_prev[i][j].R;
				total_G += img_prev[i][j].G;
				total_B += img_prev[i][j].B;
			}
			else if(curr == 1) {
				total_R += img_curr[i][j].R;
				total_G += img_curr[i][j].G;
				total_B += img_curr[i][j].B;
			}
			else {
				printf("Error reading block due to the input matrix\n");
				printf("Exiting...\n");
				exit(1);
			}
		}
	}
	//printf("Block %d %d %d: %d\n", k, l, curr, total_RGB);
	avg.R = total_R / (block_size*block_size);
	avg.G = total_G / (block_size*block_size);
	avg.B = total_B / (block_size*block_size);
	
	//printf("avg R: %d, avg G: %d, avg B: %d\n",avg.R,avg.G,avg.B);
	
	return avg;
}

RGB** apply_motion_vectors(RGB** img_in, vector** motion, int width, int height){
	int i, j, k, l;
	int k_curr, l_curr;

	RGB** img_out;
	img_out = (RGB**)malloc(height * sizeof(RGB*));
	for (i=0; i<height; i++)
		img_out[i] = (RGB *) malloc(width * sizeof(RGB));
		
	int x[8][2] = {{-1,-1},{0,-1},{1,-1},{-1,0},{1,0},{1,1},{0,1},{-1,1}};

    
	for(k = 0; k < height / block_size; k++) { // rows
		for(l = 0; l < width / block_size; l++) { // columns
			// if the motion vector is not (0,0) i.e the block has moved
			// then fill that block with average color surrounding it
			if(motion[k][l].x != 0 || motion[k][l].y != 0){
				int avg_R = 0;
				int avg_G = 0;
				int avg_B = 0;
				RGB a;
				int avg_count=0;
				for(i=0;i<8;i++){
					a = compute_avg_rgb(k*block_size + x[i][0]*block_size, l*block_size + x[i][1]*block_size,0);
					if(a.R != -1){
						avg_R += a.R;
						avg_G += a.G;
						avg_B += a.B;
						avg_count++;
					}
				}
				//printf("avg count: %d\n", avg_count);
				avg_R /= avg_count;
				avg_G /= avg_count;
				avg_B /= avg_count;
				
				for(i=k*block_size; i<k*block_size+block_size; i++){
					for(j=l*block_size; j<l*block_size+block_size; j++){
						img_out[i][j].R = avg_R;
						img_out[i][j].G = avg_G;
						img_out[i][j].B = avg_B;
					}
				}
				
			}
			// else if the block has not moved, copy contents
			else{
				for(i=k*block_size; i<k*block_size+block_size; i++){
					for(j=l*block_size; j<l*block_size+block_size; j++){
						img_out[i][j].R = img_in[i][j].R;
						//if(k==0) printf("%d vs %d\n",img_out[i][j].R, img_in[i][j].R);
						img_out[i][j].G = img_in[i][j].G;
						img_out[i][j].B = img_in[i][j].B;
					}
				}
			}
		}
	}
	
	// apply motion vectors
	for(k = 0; k < height / block_size; k++) { // rows
		for(l = 0; l < width / block_size; l++) { // columns
			// if the motion vector is not (0,0) i.e the block has moved
			// then apply the movement
			if(motion[k][l].x != 0 || motion[k][l].y != 0){
				for(k_curr=k*block_size; k_curr<k*block_size+block_size; k_curr++){
					for(l_curr=l*block_size; l_curr<l*block_size+block_size; l_curr++){
						// simple check to make sure the pixel is in bounds
						if(k_curr + motion[k][l].y < height && l_curr + motion[k][l].x < width){
							img_out[k_curr + motion[k][l].y][l_curr + motion[k][l].x] = img_in[k_curr][l_curr];
						}
					}
				}
			}
		}
	}
	
	return img_out;
}
