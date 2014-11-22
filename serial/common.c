#include "common.h"

RGB ** allocatePixelMatrix (int x, int y)
{
  int i;
  RGB **matrix;
  matrix = (RGB **)malloc(x * sizeof(RGB*));
  for (i=0; i<x; i++)
    matrix[i] = (RGB *) malloc(y * sizeof(RGB));

  return matrix;
}