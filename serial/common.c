#include "common.h"

int width;
int height;

RGB ** allocatePixelMatrix (int x, int y)
{
  int i;
  RGB **matrix;
  matrix = (RGB **)malloc(x * sizeof(RGB*));
  for (i=0; i<x; i++)
    matrix[i] = (RGB *) malloc(y * sizeof(RGB));

  return matrix;
}

vector ** allocate_motion_vector_matrix (int x, int y, int block_size)
{
  int i;
  vector **matrix;
  matrix = (vector **)malloc((x/block_size) * sizeof(vector*));
  for (i=0; i<x; i++)
    matrix[i] = (vector *) malloc((y/block_size) * sizeof(vector));

  return matrix;
}

BITMAPINFOHEADER getImageData(FILE *image)
{
	FILE *x;
	x=image;
	BITMAPINFOHEADER h;

	fseek(x,2,SEEK_SET);
	fread(&h.size,4,1,x);
	printf("Size=%d\n",h.size);
	fseek(x,18,SEEK_SET);
	fread(&h.width,4,1,x);
	fseek(x,22,SEEK_SET);
	fread(&h.height,4,1,x);
	//printf("Width=%d\tHeight=%d\n",h.width,h.height);
	fseek(x,26,SEEK_SET);
	fread(&h.planes,2,1,x);
	//printf("Number of planes:%d\n",h.planes);
	fseek(x,28,SEEK_SET);
	fread(&h.bitcount,2,1,x);
	//printf("Bit Count:%d\n",h.bitcount);

	return h;
}

RGB** import_image_from_text(char* img_path){
	FILE *image;
	
	int i, j;
	
	image=fopen(img_path,"rb+");
	
	if(image==NULL)
	{
		printf("Error! Image failed to open.");
	}
	
	char line[80];
	fgets(line, 80, image);
	sscanf (line, "%d", &width);
	
	fgets(line, 80, image);
	sscanf (line, "%d", &height);
	
	RGB** img;
	img = (RGB**)malloc(height * sizeof(RGB*));
	
	for (i=0; i<height; i++)
    		img[i] = (RGB *) malloc(width * sizeof(RGB));
	
	for(i=0; i<width*height; i++){
		fgets(line, 80, image);
		sscanf (line, "%d", &img[i/width][i%width].R);
		sscanf (line, "%d", &img[i/width][i%width].G);
		sscanf (line, "%d", &img[i/width][i%width].B);
		
		printf("%d %d %d |",img[i/width][i%width].R,img[i/width][i%width].G,img[i/width][i%width].B);
		if(i!=0 && (i+1)%width==0)printf("\n");
	}
	printf("\n");
	
	fclose(image);
	return img;
}

void export_image_to_text(RGB** img, char* img_path_out){
	FILE *image;
	
	int i, j;
	
	image=fopen(img_path_out,"w");
	
	if(image==NULL)
	{
		printf("Error! Image failed to open.");
	}
	
	fprintf(image,"%d\n",width);
	fprintf(image,"%d\n",height);
	
	for(i=0; i<width*height; i++){
		fprintf(image,"%d %d %d\n",img[i/width][i%width].R,img[i/width][i%width].G,img[i/width][i%width].B);
	}
	
	fclose(image);

}

int get_width(){
	return width;
}

int get_height(){
	return height;
}
/*
RGB** import_image(){
	FILE *image;
	char* img_path = "./test.bmp"; // TODO: need to set these paths somewhere
	BITMAPINFOHEADER bih;
	int i=0,j=0;
	
	image=fopen(img_path,"rb+");

	if(image==NULL)
	{
		printf("Error! Image failed to open.");
	}

	bih=getImageData(image);

	//RGB pic[bih.width*bih.height*2],p;
	
	char_RGB img_RGB[bih.width][bih.height];
	char_RGB p;

	//initialize RGB array to 0
	for(i=0;i<bih.height;i++){
		for(j=0; j<bih.width; j++){
			img_RGB[i][j].R=-1;
			img_RGB[i][j].G=-1;
			img_RGB[i][j].B=-1;
		}
	}
	
	printf("Enter path of test file:");
	scanf("%s",tpath);
	test=fopen(tpath,"wb+");

	fseek(image,54,SEEK_SET);

	for(i=0;i<(bih.size-54);i+=3)
	{
		fread(&p,sizeof(p),1,image);

		if(!feof(image))
		{
			img_RGB[j/bih.height][j%bih.width]=p;
			printf("(%d,%d,%d)|",img_RGB[j/bih.height][j%bih.width].R,img_RGB[j/bih.height][j%bih.width].G,img_RGB[j/bih.height][j%bih.width].B);
			j++;
		}
		if(j%bih.width==0)printf("//%d\n",j/10);
	}

	fclose(image);
	
	return img_RGB;

}
*/
